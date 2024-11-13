#include <iostream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

struct Quadruple {
    string op, arg1, arg2, result;
};

struct Triple {
    string op, arg1, arg2;
};

struct IndirectTriple {
    int index;
    Triple triple;
};

// Utility function to parse tokens
vector<string> tokenize(const string &expr) {
    istringstream stream(expr);
    string token;
    vector<string> tokens;
    while (stream >> token) {
        tokens.push_back(token);
    }
    return tokens;
}

// Function to generate TAC, identify common subexpressions
void generateTAC(const vector<string>& lines) {
    unordered_map<string, string> tempVars;  // Tracks common subexpressions
    vector<Quadruple> quadruples;
    vector<Triple> triples;
    vector<IndirectTriple> indirectTriples;

    int tempCounter = 1;  // Counter for temporary variables

    // Process each line
    for (const string &line : lines) {
        auto tokens = tokenize(line);
        if (tokens.size() < 5) continue;

        string result = tokens[0];
        string expr1 = tokens[2] + " " + tokens[3] + " " + tokens[4];
        string op1 = tokens[3];
        string arg1 = tokens[2], arg2 = tokens[4];

        string tempVar1;

        // Check if this expression is a common subexpression
        if (tempVars.find(expr1) != tempVars.end()) {
            tempVar1 = tempVars[expr1];
        } else {
            tempVar1 = "t" + to_string(tempCounter++);
            tempVars[expr1] = tempVar1;

            quadruples.push_back({op1, arg1, arg2, tempVar1});
            triples.push_back({op1, arg1, arg2});
            indirectTriples.push_back({static_cast<int>(triples.size() - 1), triples.back()});
        }

        if (tokens.size() > 5) {
            string op2 = tokens[5];
            string arg3 = tokens[6];
            string expr2 = tempVar1 + " " + op2 + " " + arg3;

            string tempVar2;
            if (tempVars.find(expr2) != tempVars.end()) {
                tempVar2 = tempVars[expr2];
            } else {
                tempVar2 = "t" + to_string(tempCounter++);
                tempVars[expr2] = tempVar2;

                quadruples.push_back({op2, tempVar1, arg3, tempVar2});
                triples.push_back({op2, tempVar1, arg3});
                indirectTriples.push_back({static_cast<int>(triples.size() - 1), triples.back()});
            }

            quadruples.push_back({"=", tempVar2, "", result});
            triples.push_back({"=", tempVar2, ""});
            indirectTriples.push_back({static_cast<int>(triples.size() - 1), triples.back()});
        } else {
            quadruples.push_back({"=", tempVar1, "", result});
            triples.push_back({"=", tempVar1, ""});
            indirectTriples.push_back({static_cast<int>(triples.size() - 1), triples.back()});
        }
    }

    // Display Quadruples
    cout << "Quadruples:\n";
    for (const auto &q : quadruples) {
        cout << "(" << q.op << ", " << q.arg1 << ", " << q.arg2 << ", " << q.result << ")\n";
    }

    // Display Triples
    cout << "\nTriples:\n";
    for (size_t i = 0; i < triples.size(); ++i) {
        cout << i << ": (" << triples[i].op << ", " << triples[i].arg1 << ", " << triples[i].arg2 << ")\n";
    }

    // Display Indirect Triples
    cout << "\nIndirect Triples:\n";
    for (const auto &it : indirectTriples) {
        cout << "[" << it.index << "] (" << it.triple.op << ", " << it.triple.arg1 << ", " << it.triple.arg2 << ")\n";
    }
}

int main() {
    cout << "Enter expressions (end with an empty line):\n";
    vector<string> lines;
    string line;
    while (getline(cin, line) && !line.empty()) {
        lines.push_back(line);
    }
    generateTAC(lines);
    return 0;
}
