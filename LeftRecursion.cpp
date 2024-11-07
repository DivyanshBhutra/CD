#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

// Function to split a string by a delimiter
vector<string> split(const string& str, char delimiter) {
    vector<string> tokens;
    string token;
    for (char ch : str) {
        if (ch == delimiter) {
            if (!token.empty()) tokens.push_back(token);
            token.clear();
        } else {
            token += ch;
        }
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}

// Function to eliminate left recursion
void eliminateLeftRecursion(unordered_map<string, vector<string>>& grammar) {
    unordered_map<string, vector<string>> newGrammar;

    // Iterate over each non-terminal in the grammar
    for (const auto& [nonTerminal, productions] : grammar) {
        vector<string> alpha, beta;

        // Split productions into alpha (recursive) and beta (non-recursive)
        for (const auto& production : productions) {
            if (production[0] == nonTerminal[0]) { // Left recursion (starts with the non-terminal itself)
                alpha.push_back(production.substr(1)); // α part
            } else {
                beta.push_back(production);            // β part
            }
        }

        // If no left recursion, we simply keep the same productions
        if (alpha.empty()) {
            newGrammar[nonTerminal] = productions;
        } else {
            // If left recursion exists, create new non-terminal
            string newNonTerminal = nonTerminal + "'";
            vector<string> newBeta, newAlpha;

            // β X' productions
            for (const auto& b : beta) {
                newBeta.push_back(b + newNonTerminal);
            }

            // α X' | ε productions
            for (const auto& a : alpha) {
                newAlpha.push_back(a + newNonTerminal);
            }
            newAlpha.push_back("ε"); // Add ε production

            // Update grammar with new non-terminal
            newGrammar[nonTerminal] = newBeta;
            newGrammar[newNonTerminal] = newAlpha;
        }
    }

    // Display the updated grammar
    cout << "\nGrammar after eliminating left recursion:\n";
    for (const auto& [nonTerminal, productions] : newGrammar) {
        cout << nonTerminal << " -> ";
        for (size_t i = 0; i < productions.size(); ++i) {
            cout << productions[i];
            if (i != productions.size() - 1) cout << " | ";
        }
        cout << endl;
    }
}

// Main function
int main() {
    unordered_map<string, vector<string>> grammar;

    cout << "Enter grammar rules (LHS -> RHS). Use '|' to separate multiple productions. Enter 'END' to finish:\n";
    string line;
    while (getline(cin, line)) {
        if (line == "END") break;

        auto pos = line.find("->");
        if (pos != string::npos) {
            string lhs = line.substr(0, pos - 1);  // Left-hand side (non-terminal)
            string rhs = line.substr(pos + 3);     // Right-hand side (productions)
            grammar[lhs] = split(rhs, '|');
        }
    }

    eliminateLeftRecursion(grammar);
    return 0;
}
