#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>

using namespace std;

// Define the grammar as a set of production rules
unordered_map<char, vector<string>> grammar;

// Recursive Descent Parser Class
class RecursiveDescentParser {
private:
    string input;
    int pos;

public:
    RecursiveDescentParser(string input) : input(input), pos(0) {}

    // Parse function to start parsing from the start symbol
    bool parse(char startSymbol) {
        cout << "Parse Tree: \n";
        bool result = parseRule(startSymbol, 0);
        return result && pos == input.size();
    }

    // Parse specific rule
    bool parseRule(char symbol, int level) {
        // Output the parse tree level
        cout << string(level * 2, ' ') << symbol << "\n";

        if (grammar.find(symbol) == grammar.end()) {
            if (pos < input.size() && input[pos] == symbol) {
                pos++;
                return true;
            }
            return false;
        }

        int initialPos = pos;
        for (const string &rule : grammar[symbol]) {
            pos = initialPos;
            bool matched = true;
            for (char c : rule) {
                if (!parseRule(c, level + 1)) {
                    matched = false;
                    break;
                }
            }
            if (matched) return true;
        }
        return false;
    }
};

int main() {
    string line, inputString;
    cout << "Enter the grammar rules (one per line in the form A->aB | c). Enter an empty line to finish:\n";

    // Read grammar rules from the user
    while (getline(cin, line) && !line.empty()) {
        istringstream iss(line);
        char lhs;
        string arrow, rhs;
        iss >> lhs >> arrow;

        while (iss >> rhs) {
            grammar[lhs].push_back(rhs);
            iss >> arrow; // ignore the '|' symbol
        }
    }

    cout << "Enter the string to parse: ";
    cin >> inputString;

    // Initialize the parser
    RecursiveDescentParser parser(inputString);

    // Parse starting from the start symbol 'S'
    bool result = parser.parse('S');

    cout << "\nResult: " << (result ? "true" : "false") << endl;

    return 0;
}
