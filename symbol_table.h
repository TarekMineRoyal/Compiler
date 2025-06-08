#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include "semantic_types.h" 
#include <string>
#include <vector>
#include <map>      
#include <list>     
#include <utility> // For std::pair

class SymbolEntry {
public:
    std::string name;
    SymbolKind kind;
    EntryTypeCategory type;
    int offset;

    ArrayDetails arrayDetails;
    EntryTypeCategory functionReturnType;

    // For FUNCTION/PROCEDURE symbols, store their parameter signature
    // Each pair: <ParameterType, ParameterArrayDetails (valid if ParameterType is ARRAY)>
    std::vector<std::pair<EntryTypeCategory, ArrayDetails>> formalParameterSignature;
    // numParameters can be derived from formalParameterSignature.size() if populated.
    size_t numParameters;

    int declLine;
    int declColumn;

    SymbolEntry();
    // General: Variables, simple parameters, program name
    SymbolEntry(std::string name, SymbolKind kind, EntryTypeCategory type, int line, int col);
    // Arrays: Array variables, array parameters
    SymbolEntry(std::string name, SymbolKind kind, EntryTypeCategory elementType, int low, int high, int line, int col);
    // Functions
    SymbolEntry(std::string name, EntryTypeCategory returnType,
        const std::vector<std::pair<EntryTypeCategory, ArrayDetails>>& signature,
        int line, int col);
    // Procedures
    SymbolEntry(std::string name,
        const std::vector<std::pair<EntryTypeCategory, ArrayDetails>>& signature,
        int line, int col);

    std::string toString() const;
};

class SymbolTable {
private:
    using Scope = std::map<std::string, SymbolEntry>;
    std::list<Scope> scopeStack;
    int currentLevel;

public:
    SymbolTable();

    void enterScope();
    void exitScope();
    bool isGlobalScope() const;
    int getCurrentLevel() const;

    bool addSymbol(const SymbolEntry& entry);
    SymbolEntry* lookupSymbol(const std::string& name);
    SymbolEntry* lookupSymbolInCurrentScope(const std::string& name);

    void printCurrentScope() const;
};

#endif // SYMBOL_TABLE_H