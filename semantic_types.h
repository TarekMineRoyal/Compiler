#ifndef SEMANTIC_TYPES_H
#define SEMANTIC_TYPES_H

#include <string>
#include <vector>
#include <memory> // For std::shared_ptr later if complex types are needed

// Kind of symbol
enum class SymbolKind {
    UNKNOWN,
    VARIABLE,
    PARAMETER,
    FUNCTION,
    PROCEDURE,
    PROGRAM_NAME // For the main program identifier
};

// Basic type categories in MiniPascal
enum class EntryTypeCategory {
    NO_TYPE,        // For symbols that don't have a traditional type (e.g., Procedure)
    UNKNOWN_TYPE,   // Default/Error state
    PRIMITIVE_INTEGER,
    PRIMITIVE_REAL,
    PRIMITIVE_BOOLEAN,
    ARRAY
};

// Details for array types
struct ArrayDetails {
    EntryTypeCategory elementType = EntryTypeCategory::UNKNOWN_TYPE; // e.g., PRIMITIVE_INTEGER
    int lowBound = 0;
    int highBound = 0;
    bool isInitialized = false; // Flag to know if these details are set
};

// Helper to convert SymbolKind to string (for debugging/logging)
inline std::string symbolKindToString(SymbolKind kind) {
    switch (kind) {
    case SymbolKind::VARIABLE: return "Variable";
    case SymbolKind::PARAMETER: return "Parameter";
    case SymbolKind::FUNCTION: return "Function";
    case SymbolKind::PROCEDURE: return "Procedure";
    case SymbolKind::PROGRAM_NAME: return "ProgramName";
    default: return "UnknownKind";
    }
}

// Helper to convert EntryTypeCategory to string
inline std::string entryTypeToString(EntryTypeCategory type) {
    switch (type) {
    case EntryTypeCategory::NO_TYPE: return "NoType";
    case EntryTypeCategory::PRIMITIVE_INTEGER: return "Integer";
    case EntryTypeCategory::PRIMITIVE_REAL: return "Real";
    case EntryTypeCategory::PRIMITIVE_BOOLEAN: return "Boolean";
    case EntryTypeCategory::ARRAY: return "Array";
    case EntryTypeCategory::UNKNOWN_TYPE: return "UnknownType"; // Added for completeness
    default: return "OtherType"; // Should ideally not happen with current enum
    }
}

#endif // SEMANTIC_TYPES_H