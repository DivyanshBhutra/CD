#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>

using namespace std;

// Structure to represent a symbol table entry
struct Symbol {
    string name;
    string type;
    string scope;
    void* address;
    string value;
    string additionalInfo;
};

// Symbol table class to manage symbols
class SymbolTable {
private:
    unordered_map<string, Symbol> table;

public:
    void addSymbol(string name, string type, string scope, void* address, string value, string additionalInfo = "") {
        Symbol symbol = {name, type, scope, address, value, additionalInfo};
        table[name] = symbol;
    }

    void displayTable() {
        cout << "Name\tType\tScope\tAddress\t\tValue\tAdditional Info" << endl;
        cout << "-------------------------------------------------------------" << endl;
        for (const auto& entry : table) {
            const Symbol& sym = entry.second;
            cout << sym.name << "\t" << sym.type << "\t" << sym.scope << "\t" << sym.address
                 << "\t" << sym.value << "\t" << sym.additionalInfo << endl;
        }
    }
};

// Function to parse a line and add to symbol table
void parseLine(string line, SymbolTable& symTable) {
    istringstream iss(line);
    string type, name, equals, value;
    if (iss >> type >> name >> equals >> value) {
        if (type == "int" || type == "string") {
            void* address = malloc(sizeof(type));  // Mock memory address
            symTable.addSymbol(name, type, "global", address, value, "N/A");
        }
    }
}

int main() {
    SymbolTable symTable;
    string code, line;

    cout << "Enter your code (type 'END' on a new line to finish):" << endl;

    // Taking multiline input
    while (getline(cin, line)) {
        if (line == "END") break;
        parseLine(line, symTable);
    }

    // Display the symbol table
    symTable.displayTable();

    return 0;
}
