#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <algorithm>

// Function to split a production rule by '|'
std::vector<std::string> splitProductions(const std::string& production) {
    std::vector<std::string> parts;
    std::string token;
    std::stringstream ss(production);
    while (getline(ss, token, '|')) {
        parts.push_back(token);
    }
    return parts;
}

// Function to find the longest common prefix among multiple strings
std::string longestCommonPrefix(const std::vector<std::string>& strs) {
    if (strs.empty()) return "";
    std::string prefix = strs[0];
    for (size_t i = 1; i < strs.size(); ++i) {
        size_t j = 0;
        while (j < prefix.size() && j < strs[i].size() && prefix[j] == strs[i][j])
            ++j;
        prefix = prefix.substr(0, j);
        if (prefix.empty()) break;
    }
    return prefix;
}

// Function to factor a single production if a common prefix is found
void factorProduction(const std::string& nonTerminal, std::vector<std::string>& productions, std::map<std::string, std::vector<std::string>>& newGrammar) {
    std::string prefix = longestCommonPrefix(productions);

    if (prefix.empty()) { // No left factoring needed
        newGrammar[nonTerminal] = productions;
        return;
    }

    // Create new non-terminal
    std::string newNonTerminal = nonTerminal + "'";
    std::vector<std::string> newProductions;
    std::vector<std::string> remainingProductions;

    for (const auto& prod : productions) {
        if (prod.substr(0, prefix.size()) == prefix) {
            std::string remaining = prod.substr(prefix.size());
            if (remaining.empty()) remaining = "ε"; // Use epsilon for empty remaining
            newProductions.push_back(remaining);
        } else {
            remainingProductions.push_back(prod); // Keep productions without prefix
        }
    }

    // Update the grammar with the factored production
    newGrammar[nonTerminal] = remainingProductions; // Add non-factored parts, if any
    newGrammar[nonTerminal].push_back(prefix + newNonTerminal); // Add the left-factored part with new non-terminal

    newGrammar[newNonTerminal] = newProductions; // Productions after factoring
}

// Function to factor the entire grammar
std::map<std::string, std::vector<std::string>> factorGrammar(std::map<std::string, std::vector<std::string>>& grammar) {
    std::map<std::string, std::vector<std::string>> newGrammar;

    for (auto& [nonTerminal, productions] : grammar) {
        factorProduction(nonTerminal, productions, newGrammar);
    }
    return newGrammar;
}

// Function to print the grammar
void printGrammar(const std::map<std::string, std::vector<std::string>>& grammar) {
    for (const auto& [nonTerminal, productions] : grammar) {
        std::cout << nonTerminal << " -> ";
        for (size_t i = 0; i < productions.size(); ++i) {
            std::cout << productions[i];
            if (i < productions.size() - 1) std::cout << " | ";
        }
        std::cout << std::endl;
    }
}

int main() {
    std::map<std::string, std::vector<std::string>> grammar;
    std::string line;

    std::cout << "Enter grammar productions (non-terminal -> productions). Type 'end' to finish input:" << std::endl;
    while (true) {
        std::getline(std::cin, line);
        if (line == "end") break;

        size_t pos = line.find("->");
        if (pos == std::string::npos) {
            std::cerr << "Invalid production format. Use '->' for defining productions." << std::endl;
            continue;
        }

        std::string nonTerminal = line.substr(0, pos - 1);
        std::string production = line.substr(pos + 2);

        // Remove extra spaces
        nonTerminal.erase(remove(nonTerminal.begin(), nonTerminal.end(), ' '), nonTerminal.end());

        std::vector<std::string> productions = splitProductions(production);
        grammar[nonTerminal] = productions;
    }

    // Factor the grammar
    std::map<std::string, std::vector<std::string>> factoredGrammar = factorGrammar(grammar);

    // Print the factored grammar
    std::cout << "\nFactored Grammar:" << std::endl;
    printGrammar(factoredGrammar);

    return 0;
}
