#include <iostream>
#include <string>
#include <unordered_map>
#include <memory>

using namespace std;

// Node structure for parse tree
struct Node {
    string value;
    unique_ptr<Node> left;
    unique_ptr<Node> right;

    Node(string val) : value(std::move(val)), left(nullptr), right(nullptr) {}
};

// Map to store grammar rules
unordered_map<string, string> grammar;

// Parse the grammar input from user
void parseGrammar() {
    cout << "Enter grammar rules (LHS -> RHS). Enter 'END' to finish:\n";
    string line;
    while (getline(cin, line)) {
        if (line == "END") break;
        auto pos = line.find("->");
        if (pos != string::npos) {
            string lhs = line.substr(0, pos - 1); // Left-hand side
            string rhs = line.substr(pos + 3);     // Right-hand side
            grammar[lhs] = rhs;
        }
    }
}

// Recursive function to build parse tree
unique_ptr<Node> buildParseTree(const string& symbol, const string& input, int& pos) {
    if (pos >= input.size()) return nullptr;

    // Create a node for the current symbol
    auto node = make_unique<Node>(symbol);

    // Check if it's a terminal or a non-terminal in the grammar
    if (grammar.find(symbol) == grammar.end()) {
        // Terminal: match directly with input
        if (pos < input.size() && input[pos] == symbol[0]) {
            pos++;  // Move to the next character
            return node;
        }
        return nullptr;  // Mismatch case
    }

    // Non-terminal: expand using RHS
    string rhs = grammar[symbol];
    for (char c : rhs) {
        string nextSymbol(1, c);
        auto child = buildParseTree(nextSymbol, input, pos);
        if (!child) return nullptr;  // If unable to build child, parsing fails
        if (!node->left) node->left = std::move(child);  // Left child for first symbol
        else node->right = std::move(child);             // Right child for second symbol
    }

    return node;
}

// Traversal functions for different tree orders
void prefix(const unique_ptr<Node>& node) {
    if (node) {
        cout << node->value << " ";
        prefix(node->left);
        prefix(node->right);
    }
}

void infix(const unique_ptr<Node>& node) {
    if (node) {
        if (node->left) cout << "( ";
        infix(node->left);
        cout << node->value << " ";
        infix(node->right);
        if (node->right) cout << ") ";
    }
}

void postfix(const unique_ptr<Node>& node) {
    if (node) {
        postfix(node->left);
        postfix(node->right);
        cout << node->value << " ";
    }
}

// Main function
int main() {
    parseGrammar();  // Parse the grammar from user input

    string inputString;
    cout << "Enter the input string to parse: ";
    cin >> inputString;

    int pos = 0;
    auto parseTree = buildParseTree("S", inputString, pos);  // Assuming 'S' is the start symbol

    if (parseTree && pos == inputString.size()) {
        cout << "\nParse Tree (Prefix notation): ";
        prefix(parseTree);
        cout << "\nParse Tree (Infix notation): ";
        infix(parseTree);
        cout << "\nParse Tree (Postfix notation): ";
        postfix(parseTree);
        cout << "\nParsing successful: Parsed\n";
    } else {
        cout << "\nParsing failed: Not parsed\n";
    }

    return 0;
}
