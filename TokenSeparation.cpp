#include <iostream>
#include <string>
#include <cctype>
#include <vector>

using namespace std;

// Function to check if a string is a keyword
bool isKeyword(string str) {
    string keywords[] = {"auto", "break", "case", "char", "const", "continue", "default",
                         "do", "double", "else", "enum", "extern", "float", "for", "goto",
                         "if", "int", "long", "register", "return", "short", "signed",
                         "sizeof", "static", "struct", "switch", "typedef", "union",
                         "unsigned", "void", "volatile", "while"};

    for (string keyword : keywords) {
        if (str == keyword)
            return true;
    }
    return false;
}

// Function to tokenize the input expression
void tokenizeExpression(string expression) {
    vector<string> tokens;
    string currentToken = "";

    for (int i = 0; i < expression.length(); i++) {
        char c = expression[i];

        if (isalnum(c) || c == '_') {
            currentToken += c;
        } else {
            if (!currentToken.empty()) {
                if (isKeyword(currentToken))
                    tokens.push_back("Keyword: " + currentToken);
                else if (isdigit(currentToken[0]))
                    tokens.push_back("Constant: " + currentToken);
                else
                    tokens.push_back("Identifier: " + currentToken);
                currentToken = "";
            }

            if (c == '+' || c == '-' || c == '*' || c == '/' || c == '=' || c == '>' || c == '<') {
                tokens.push_back("Operator: " + string(1, c));
            } else if (c == '{' || c == '}' || c == '(' || c == ')' || c == ';' || c == ',') {
                tokens.push_back("Symbol: " + string(1, c));
            }
        }
    }

    if (!currentToken.empty()) {
        if (isKeyword(currentToken))
            tokens.push_back("Keyword: " + currentToken);
        else if (isdigit(currentToken[0]))
            tokens.push_back("Constant: " + currentToken);
        else
            tokens.push_back("Identifier: " + currentToken);
    }

    for (string token : tokens) {
        cout << token << endl;
    }
}

int main() {
    string expression;
    cout << "Enter a code expression: ";
    getline(cin, expression);

    tokenizeExpression(expression);

    return 0;
}
