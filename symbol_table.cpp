#include "symbol_table.h"
#include <sstream> 
#include <iostream> 
#include <iterator> 

SymbolEntry::SymbolEntry()
    : kind(SymbolKind::UNKNOWN), type(EntryTypeCategory::UNKNOWN_TYPE),
    functionReturnType(EntryTypeCategory::NO_TYPE), numParameters(0),
    declLine(0), declColumn(0) {
    arrayDetails.isInitialized = false;
}

SymbolEntry::SymbolEntry(std::string id_name, SymbolKind k, EntryTypeCategory et, int line, int col)
    : name(std::move(id_name)), kind(k), type(et),
    functionReturnType(EntryTypeCategory::NO_TYPE),
    numParameters(0),
    declLine(line), declColumn(col) {
    arrayDetails.isInitialized = false;
}

SymbolEntry::SymbolEntry(std::string id_name, SymbolKind k, EntryTypeCategory el_type, int low, int high, int line, int col)
    : name(std::move(id_name)), kind(k), type(EntryTypeCategory::ARRAY),
    functionReturnType(EntryTypeCategory::NO_TYPE),
    numParameters(0),
    declLine(line), declColumn(col) {
    arrayDetails.elementType = el_type;
    arrayDetails.lowBound = low;
    arrayDetails.highBound = high;
    arrayDetails.isInitialized = true;
}

SymbolEntry::SymbolEntry(std::string func_name, EntryTypeCategory ret_type,
    const std::vector<std::pair<EntryTypeCategory, ArrayDetails>>& signature,
    int line, int col)
    : name(std::move(func_name)), kind(SymbolKind::FUNCTION), type(EntryTypeCategory::NO_TYPE),
    functionReturnType(ret_type),
    formalParameterSignature(signature),
    numParameters(signature.size()),
    declLine(line), declColumn(col) {
    arrayDetails.isInitialized = false;
}

SymbolEntry::SymbolEntry(std::string proc_name,
    const std::vector<std::pair<EntryTypeCategory, ArrayDetails>>& signature,
    int line, int col)
    : name(std::move(proc_name)), kind(SymbolKind::PROCEDURE), type(EntryTypeCategory::NO_TYPE),
    functionReturnType(EntryTypeCategory::NO_TYPE),
    formalParameterSignature(signature),
    numParameters(signature.size()),
    declLine(line), declColumn(col) {
    arrayDetails.isInitialized = false;
}


std::string SymbolEntry::toString() const {
    std::stringstream ss;
    ss << "'" << name << "' (" << symbolKindToString(kind) << ")";
    if (kind == SymbolKind::VARIABLE || kind == SymbolKind::PARAMETER) {
        ss << " Type: " << entryTypeToString(type);
        if (type == EntryTypeCategory::ARRAY && arrayDetails.isInitialized) {
            ss << " [Array Of " << entryTypeToString(arrayDetails.elementType)
                << ", " << arrayDetails.lowBound << ".." << arrayDetails.highBound << "]";
        }
    }
    else if (kind == SymbolKind::FUNCTION) {
        ss << " Returns: " << entryTypeToString(functionReturnType);
        ss << ", Params: (";
        for (size_t i = 0; i < formalParameterSignature.size(); ++i) {
            ss << entryTypeToString(formalParameterSignature[i].first);
            if (formalParameterSignature[i].first == EntryTypeCategory::ARRAY && formalParameterSignature[i].second.isInitialized) {
                ss << " [" << entryTypeToString(formalParameterSignature[i].second.elementType) << "]";
            }
            if (i < formalParameterSignature.size() - 1) ss << ", ";
        }
        ss << ")";
    }
    else if (kind == SymbolKind::PROCEDURE) {
        ss << ", Params: (";
        for (size_t i = 0; i < formalParameterSignature.size(); ++i) {
            ss << entryTypeToString(formalParameterSignature[i].first);
            if (formalParameterSignature[i].first == EntryTypeCategory::ARRAY && formalParameterSignature[i].second.isInitialized) {
                ss << " [" << entryTypeToString(formalParameterSignature[i].second.elementType) << "]";
            }
            if (i < formalParameterSignature.size() - 1) ss << ", ";
        }
        ss << ")";
    }
    ss << " (L:" << declLine << ", C:" << declColumn << ")";
    return ss.str();
}

SymbolTable::SymbolTable() : currentLevel(-1) {
    enterScope();
}

void SymbolTable::enterScope() {
    scopeStack.emplace_back();
    currentLevel++;
}

void SymbolTable::exitScope() {
    if (!scopeStack.empty()) {
        scopeStack.pop_back();
        currentLevel--;
    }
    else {
        std::cerr << "Error: Attempting to exit non-existent scope." << std::endl;
    }
}

bool SymbolTable::isGlobalScope() const {
    return currentLevel == 0;
}

int SymbolTable::getCurrentLevel() const {
    return currentLevel;
}

bool SymbolTable::addSymbol(const SymbolEntry& entry) {
    if (scopeStack.empty()) {
        std::cerr << "Error: No current scope to add symbol '" << entry.name << "'." << std::endl;
        return false;
    }
    Scope& currentScope = scopeStack.back();
    if (currentScope.count(entry.name)) {
        return false;
    }
    currentScope[entry.name] = entry;
    return true;
}

SymbolEntry* SymbolTable::lookupSymbol(const std::string& name) {
    if (scopeStack.empty()) return nullptr;
    // Iterate from current scope (back of list) to global scope (front)
    for (auto list_iter = std::prev(scopeStack.end()); ; /* decrement inside */) {
        Scope& scope = *list_iter; // Non-const reference
        auto foundEntry = scope.find(name);
        if (foundEntry != scope.end()) {
            return &(foundEntry->second);
        }
        if (list_iter == scopeStack.begin()) { // Checked the global scope
            break;
        }
        --list_iter; // Move to previous scope
    }
    return nullptr; // Not found in any scope
}

SymbolEntry* SymbolTable::lookupSymbolInCurrentScope(const std::string& name) {
    if (scopeStack.empty()) {
        return nullptr;
    }
    Scope& currentScope = scopeStack.back();
    auto foundEntry = currentScope.find(name);
    if (foundEntry != currentScope.end()) {
        return &(foundEntry->second);
    }
    return nullptr;
}

void SymbolTable::printCurrentScope() const {
    if (scopeStack.empty()) {
        std::cout << "Symbol Table: No active scope." << std::endl;
        return;
    }
    std::cout << "--- Current Scope (Level " << currentLevel << ") ---" << std::endl;
    const Scope& currentScope = scopeStack.back();
    for (const auto& pair : currentScope) {
        std::cout << "  " << pair.second.toString() << std::endl;
    }
    if (currentScope.empty()) {
        std::cout << "  (empty)" << std::endl;
    }
    std::cout << "---------------------------" << std::endl;
}