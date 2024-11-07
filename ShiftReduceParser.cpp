#include <iostream>
#include <vector>
#include <string>

using namespace std;

// Structure to hold production rules
struct Production {
    char lhs;    // Left-hand side of the production (non-terminal)
    string rhs;  // Right-hand side of the production (terminals/non-terminals)
};

// Function to parse grammar from input
void parseGrammar(vector<Production>& grammar, string& inputString) {
    cout << "Enter grammar rules (LHS -> RHS). Enter 'END' to finish:\n";
    string line;
    while (getline(cin, line)) {
        if (line == "END") break;
        char lhs = line[0];
        string rhs = line.substr(5);  // Skipping " -> " part
        grammar.push_back({lhs, rhs});
    }
    cout << "Enter the input string: ";
    cin >> inputString;
}

// Function to perform a reduction step based on the current stack contents and grammar
bool reduce(vector<string>& parseStack, const vector<Production>& grammar) {
    for (const auto& prod : grammar) {
        if (parseStack.size() >= prod.rhs.size()) {
            // Check if the top of the stack matches the RHS of a production
            string top = "";
            for (int i = parseStack.size() - prod.rhs.size(); i < parseStack.size(); ++i) {
                top += parseStack[i];
            }

            if (top == prod.rhs) {
                // Perform the reduction
                for (size_t i = 0; i < prod.rhs.size(); ++i) {
                    parseStack.pop_back();
                }
                parseStack.push_back(string(1, prod.lhs));  // Push LHS of production onto the stack
                cout << "Reduced by rule " << prod.lhs << " -> " << prod.rhs << "\n";
                return true;
            }
        }
    }
    return false;
}

// Shift-Reduce Parser function
bool shiftReduceParser(const vector<Production>& grammar, const string& inputString) {
    vector<string> parseStack;  // Using vector instead of stack for easier access
    int inputIdx = 0;

    cout << "Parsing steps:\n";
    while (inputIdx < inputString.size()) {
        parseStack.push_back(string(1, inputString[inputIdx]));  // Shift
        cout << "Shift: " << inputString[inputIdx] << "\n";
        inputIdx++;

        // Apply reductions as long as possible
        while (reduce(parseStack, grammar));
    }

    // Final reductions after all shifts are done
    while (reduce(parseStack, grammar));

    // Final parse result
    return parseStack.size() == 1 && parseStack[0] == "S";  // Assuming 'S' is the start symbol
}

// Main function
int main() {
    vector<Production> grammar;
    string inputString;

    // Parse grammar and input string
    parseGrammar(grammar, inputString);

    // Parse the input string
    if (shiftReduceParser(grammar, inputString)) {
        cout << "Parsing successful: Parsed\n";
    } else {
        cout << "Parsing failed: Not parsed\n";
    }

    return 0;
}
