#include "semantic_analyzer.h"
#include <iostream>
#include <sstream> // Needed for building the mangled name

// Constructor: Pre-populate symbol table with built-in I/O procedures
SemanticAnalyzer::SemanticAnalyzer() : currentFunctionContext(nullptr), global_offset(0), local_offset(0), param_offset(0) {
    // MODIFIED: Explicitly define built-ins as procedures.
    // They are handled by special case logic and are not mangled.
    std::vector<std::pair<EntryTypeCategory, ArrayDetails>> empty_signature;
    symbolTable.addSymbol(SymbolEntry("read", SymbolKind::PROCEDURE, EntryTypeCategory::NO_TYPE, 0, 0));
    symbolTable.addSymbol(SymbolEntry("readln", SymbolKind::PROCEDURE, EntryTypeCategory::NO_TYPE, 0, 0));
    symbolTable.addSymbol(SymbolEntry("write", SymbolKind::PROCEDURE, EntryTypeCategory::NO_TYPE, 0, 0));
    symbolTable.addSymbol(SymbolEntry("writeln", SymbolKind::PROCEDURE, EntryTypeCategory::NO_TYPE, 0, 0));
}

void SemanticAnalyzer::recordError(const std::string& message, int line, int col) {
    std::string error = "Semantic Error (L:" + std::to_string(line) + ", C:" + std::to_string(col) + "): " + message;
    semanticErrors.push_back(error);
}

bool SemanticAnalyzer::hasErrors() const {
    return !semanticErrors.empty();
}

void SemanticAnalyzer::printErrors(std::ostream& out) const {
    for (const auto& err : semanticErrors) {
        out << err << std::endl;
    }
}

// ADDED: Implementation of the mangled name builder for resolving overloads.
std::string SemanticAnalyzer::buildMangledName(const std::string& name, SymbolKind kind, ExpressionList* args) {
    std::stringstream mangledName;
    if (kind == SymbolKind::FUNCTION) {
        mangledName << "f_";
    }
    else { // PROCEDURE
        mangledName << "p_";
    }
    mangledName << name;

    if (args) {
        for (const auto& expr : args->expressions) {
            mangledName << "_";
            // Note: This relies on the expression's type having been determined already.
            switch (expr->determinedType) {
            case EntryTypeCategory::PRIMITIVE_INTEGER: mangledName << "i"; break;
            case EntryTypeCategory::PRIMITIVE_REAL:    mangledName << "r"; break;
            case EntryTypeCategory::PRIMITIVE_BOOLEAN: mangledName << "b"; break;
            case EntryTypeCategory::ARRAY:             mangledName << "a"; break;
            default:                                   mangledName << "u"; break; // unknown
            }
        }
    }
    return mangledName.str();
}

EntryTypeCategory SemanticAnalyzer::astStandardTypeToSymbolType(StandardTypeNode* astStandardTypeNode) {
    if (!astStandardTypeNode) return EntryTypeCategory::UNKNOWN_TYPE;
    switch (astStandardTypeNode->category) {
    case StandardTypeNode::TYPE_INTEGER: return EntryTypeCategory::PRIMITIVE_INTEGER;
    case StandardTypeNode::TYPE_REAL:   return EntryTypeCategory::PRIMITIVE_REAL;
    case StandardTypeNode::TYPE_BOOLEAN:return EntryTypeCategory::PRIMITIVE_BOOLEAN;
    default:
        recordError("Internal: Unknown standard type category (" + std::to_string(astStandardTypeNode->category) + ") encountered.", astStandardTypeNode->line, astStandardTypeNode->column);
        return EntryTypeCategory::UNKNOWN_TYPE;
    }
}

EntryTypeCategory SemanticAnalyzer::astToSymbolType(TypeNode* astTypeNode, ArrayDetails& outArrayDetails) {
    outArrayDetails.isInitialized = false;
    if (!astTypeNode) {
        return EntryTypeCategory::UNKNOWN_TYPE;
    }

    if (auto* stn = dynamic_cast<StandardTypeNode*>(astTypeNode)) {
        return astStandardTypeToSymbolType(stn);
    }
    else if (auto* atn = dynamic_cast<ArrayTypeNode*>(astTypeNode)) {
        if (atn->elementType) {
            outArrayDetails.elementType = astStandardTypeToSymbolType(atn->elementType);
        }
        else {
            recordError("Array type declaration is missing its element type.", atn->line, atn->column);
            outArrayDetails.elementType = EntryTypeCategory::UNKNOWN_TYPE;
        }
        if (atn->startIndex && atn->endIndex) {
            outArrayDetails.lowBound = atn->startIndex->value;
            outArrayDetails.highBound = atn->endIndex->value;
            outArrayDetails.isInitialized = true;
            if (outArrayDetails.lowBound > outArrayDetails.highBound) {
                recordError("For array type, lower bound (" + std::to_string(outArrayDetails.lowBound) +
                    ") exceeds upper bound (" + std::to_string(outArrayDetails.highBound) + ").",
                    atn->line, atn->column);
            }
        }
        else {
            recordError("Array type declaration is missing its bounds.", atn->line, atn->column);
        }
        return EntryTypeCategory::ARRAY;
    }
    recordError("Internal: Unrecognized AST type node encountered during type conversion.", astTypeNode->line, astTypeNode->column);
    return EntryTypeCategory::UNKNOWN_TYPE;
}

bool SemanticAnalyzer::isPrintableType(EntryTypeCategory type, ExprNode* argNode) {
    if (type == EntryTypeCategory::PRIMITIVE_INTEGER ||
        type == EntryTypeCategory::PRIMITIVE_REAL ||
        type == EntryTypeCategory::PRIMITIVE_BOOLEAN) {
        return true;
    }
    if (dynamic_cast<StringLiteralNode*>(argNode)) {
        return true;
    }
    return false;
}

bool SemanticAnalyzer::isReadableType(EntryTypeCategory type) {
    return type == EntryTypeCategory::PRIMITIVE_INTEGER ||
        type == EntryTypeCategory::PRIMITIVE_REAL;
}


void SemanticAnalyzer::visit(ProgramNode& node) {
    SymbolEntry progNameEntry(node.progName->name, SymbolKind::PROGRAM_NAME, EntryTypeCategory::NO_TYPE, node.progName->line, node.progName->column);
    if (!symbolTable.addSymbol(progNameEntry)) {
        recordError("Program name '" + node.progName->name + "' conflict (this should generally not occur for the program name itself).", node.progName->line, node.progName->column);
    }

    if (node.decls) node.decls->accept(*this);
    if (node.subprogs) node.subprogs->accept(*this);
    if (node.mainCompoundStmt) node.mainCompoundStmt->accept(*this);
}

void SemanticAnalyzer::visit(Declarations& node) {
    for (VarDecl* varDecl : node.var_decl_items) {
        if (varDecl) varDecl->accept(*this);
    }
}

void SemanticAnalyzer::visit(VarDecl& node) {
    ArrayDetails ad;
    EntryTypeCategory var_type = astToSymbolType(node.type, ad);

    if (var_type == EntryTypeCategory::UNKNOWN_TYPE && node.type != nullptr) {
        recordError("Variable declaration uses an invalid or unknown type.", node.type->line, node.type->column);
    }

    for (IdentNode* identNode : node.identifiers->identifiers) {
        if (!identNode) continue;

        int current_offset;
        if (symbolTable.isGlobalScope()) {
            current_offset = global_offset++;
        }
        else { // Local scope (inside a subprogram)
            current_offset = local_offset++;
        }

        SymbolEntry entry(identNode->name, SymbolKind::VARIABLE, var_type, identNode->line, identNode->column);
        entry.offset = current_offset;
        if (var_type == EntryTypeCategory::ARRAY) {
            entry.arrayDetails = ad;
        }

        if (!symbolTable.addSymbol(entry)) {
            SymbolEntry* existing = symbolTable.lookupSymbolInCurrentScope(identNode->name);
            std::string conflictMsg = existing ? " Conflicts with existing " + symbolKindToString(existing->kind) + " declared at L:" + std::to_string(existing->declLine) : "";
            recordError("Identifier '" + identNode->name + "' re-declared in the current scope." + conflictMsg, identNode->line, identNode->column);
        }
    }
}

// MODIFIED: Use a two-pass approach. First pass adds all headers to the symbol table.
void SemanticAnalyzer::visit(SubprogramDeclarations& node) {
    // First pass: Add all subprogram headers to the symbol table before visiting bodies.
    // This allows for mutual recursion.
    for (SubprogramDeclaration* subDecl : node.subprograms) {
        if (subDecl && subDecl->head) {
            subDecl->head->accept(*this);
        }
    }
    // Second pass: Visit the full declaration bodies to analyze them.
    for (SubprogramDeclaration* subDecl : node.subprograms) {
        if (subDecl) {
            subDecl->accept(*this);
        }
    }
}

// MODIFIED: This now correctly finds the symbol entry for the current subprogram
// and uses it to analyze the body, especially for RETURN statements.
void SemanticAnalyzer::visit(SubprogramDeclaration& node) {
    // Find the entry for the subprogram header, which was added in the first pass.
    SymbolEntry* entry = nullptr;
    if (node.head) {
        std::string mangledKey = "p_";
        if (dynamic_cast<FunctionHeadNode*>(node.head)) mangledKey = "f_";
        mangledKey += node.head->name->name;

        // This is a simplified way to reconstruct the mangled name from declaration
        // It does not account for overloading of subprograms within the subprogram itself
        if (node.head->arguments && node.head->arguments->params) {
            for (const auto& param_decl_group : node.head->arguments->params->paramDeclarations) {
                if (param_decl_group && param_decl_group->ids && param_decl_group->type) {
                    ArrayDetails ad_param;
                    EntryTypeCategory param_cat_type = astToSymbolType(param_decl_group->type, ad_param);
                    for (size_t i = 0; i < param_decl_group->ids->identifiers.size(); ++i) {
                        mangledKey += "_";
                        switch (param_cat_type) {
                        case EntryTypeCategory::PRIMITIVE_INTEGER: mangledKey += "i"; break;
                        case EntryTypeCategory::PRIMITIVE_REAL:    mangledKey += "r"; break;
                        case EntryTypeCategory::PRIMITIVE_BOOLEAN: mangledKey += "b"; break;
                        case EntryTypeCategory::ARRAY:             mangledKey += "a"; break;
                        default:                                   mangledKey += "u"; break;
                        }
                    }
                }
            }
        }
        entry = symbolTable.lookupSymbol(mangledKey);
    }

    SymbolEntry* previousSubprogramEntry = currentSubprogramEntry;
    currentSubprogramEntry = entry;

    symbolTable.enterScope();
    local_offset = 0;
    param_offset = 0;

    FunctionHeadNode* previousFunctionContext = currentFunctionContext;
    if (auto funcHead = dynamic_cast<FunctionHeadNode*>(node.head)) {
        currentFunctionContext = funcHead;
    }
    else {
        currentFunctionContext = nullptr;
    }

    if (node.head && node.head->arguments) {
        node.head->arguments->accept(*this);
    }
    if (node.local_declarations) {
        node.local_declarations->accept(*this);
    }
    if (node.body) {
        node.body->accept(*this);
    }

    symbolTable.exitScope();
    currentFunctionContext = previousFunctionContext;
    currentSubprogramEntry = previousSubprogramEntry;
}

// MODIFIED: This now only adds the function header to the symbol table.
void SemanticAnalyzer::visit(FunctionHeadNode& node) {
    EntryTypeCategory return_type = EntryTypeCategory::UNKNOWN_TYPE;
    if (node.returnType) {
        return_type = astStandardTypeToSymbolType(node.returnType);
    }
    else {
        recordError("Function '" + node.name->name + "' is missing its return type specification.", node.line, node.column);
    }

    std::vector<std::pair<EntryTypeCategory, ArrayDetails>> signature;
    if (node.arguments && node.arguments->params) {
        for (const auto& param_decl_group : node.arguments->params->paramDeclarations) {
            if (param_decl_group && param_decl_group->ids && param_decl_group->type) {
                ArrayDetails ad_param; // Use a fresh ArrayDetails for each param group type
                EntryTypeCategory param_cat_type = astToSymbolType(param_decl_group->type, ad_param);
                for (size_t i = 0; i < param_decl_group->ids->identifiers.size(); ++i) {
                    signature.push_back({ param_cat_type, ad_param });
                }
            }
        }
    }
    SymbolEntry entry(node.name->name, return_type, signature, node.name->line, node.name->column);
    if (!symbolTable.addSymbol(entry)) {
        recordError("Function '" + node.name->name + "' with this exact signature is already declared in this scope.", node.name->line, node.name->column);
    }
}

// MODIFIED: This now only adds the procedure header to the symbol table.
void SemanticAnalyzer::visit(ProcedureHeadNode& node) {
    std::vector<std::pair<EntryTypeCategory, ArrayDetails>> signature;
    if (node.arguments && node.arguments->params) {
        for (const auto& param_decl_group : node.arguments->params->paramDeclarations) {
            if (param_decl_group && param_decl_group->ids && param_decl_group->type) {
                ArrayDetails ad_param;
                EntryTypeCategory param_cat_type = astToSymbolType(param_decl_group->type, ad_param);
                for (size_t i = 0; i < param_decl_group->ids->identifiers.size(); ++i) {
                    signature.push_back({ param_cat_type, ad_param });
                }
            }
        }
    }
    SymbolEntry entry(node.name->name, signature, node.name->line, node.name->column);
    if (!symbolTable.addSymbol(entry)) {
        recordError("Procedure '" + node.name->name + "' with this exact signature is already declared in this scope.", node.name->line, node.name->column);
    }
}

void SemanticAnalyzer::visit(ArgumentsNode& node) {
    if (node.params) {
        node.params->accept(*this);
    }
}

void SemanticAnalyzer::visit(ParameterDeclaration& node) {
    ArrayDetails ad;
    EntryTypeCategory param_type = astToSymbolType(node.type, ad);

    if (param_type == EntryTypeCategory::UNKNOWN_TYPE && node.type != nullptr) {
        recordError("Parameter declaration uses an invalid or unknown type.", node.type->line, node.type->column);
    }

    for (IdentNode* identNode : node.ids->identifiers) {
        if (!identNode) continue;

        SymbolEntry entry(identNode->name, SymbolKind::PARAMETER, param_type, identNode->line, identNode->column);
        entry.offset = param_offset++; // Assign and increment parameter offset
        if (param_type == EntryTypeCategory::ARRAY) {
            entry.arrayDetails = ad;
        }

        if (!symbolTable.addSymbol(entry)) { // Add to the current (subprogram's) scope
            SymbolEntry* existing = symbolTable.lookupSymbolInCurrentScope(identNode->name);
            std::string conflictMsg = existing ? " Conflicts with existing " + symbolKindToString(existing->kind) + " declared at L:" + std::to_string(existing->declLine) : "";
            recordError("Parameter '" + identNode->name + "' re-declared in this scope." + conflictMsg, identNode->line, identNode->column);
        }
    }
}

void SemanticAnalyzer::visit(ParameterList& node) {
    for (ParameterDeclaration* paramDecl : node.paramDeclarations) {
        if (paramDecl) paramDecl->accept(*this);
    }
}

void SemanticAnalyzer::visit(CompoundStatementNode& node) {
    if (node.stmts) node.stmts->accept(*this);
}

void SemanticAnalyzer::visit(StatementList& node) {
    for (StatementNode* stmt : node.statements) {
        if (stmt) stmt->accept(*this);
    }
}

void SemanticAnalyzer::visit(IdentifierList& node) { /* Not typically visited directly */ }
void SemanticAnalyzer::visit(IdentNode& node) { /* Not typically visited directly by semantic analysis */ }
void SemanticAnalyzer::visit(StandardTypeNode& node) { /* Processed by parent */ }
void SemanticAnalyzer::visit(ArrayTypeNode& node) { /* Processed by parent */ }

void SemanticAnalyzer::visit(AssignStatementNode& node) {
    if (!node.variable || !node.expression) {
        recordError("Malformed assignment statement (missing variable or expression).", node.line, node.column);
        return;
    }

    node.variable->accept(*this);
    node.expression->accept(*this);

    EntryTypeCategory lhsType = node.variable->determinedType;
    EntryTypeCategory rhsType = node.expression->determinedType;

    if (lhsType == EntryTypeCategory::UNKNOWN_TYPE || rhsType == EntryTypeCategory::UNKNOWN_TYPE) {
        // An error was already recorded for the variable or expression.
        return;
    }

    bool compatible = false;
    if (lhsType == rhsType) {
        if (lhsType == EntryTypeCategory::ARRAY) {
            // VariableNode->index being non-null means it's an element access.
            if (node.variable->index == nullptr) {
                recordError("Whole array assignment to '" + node.variable->identifier->name + "' is not supported.", node.line, node.column);
            }
            else {
                compatible = true; // Assigning to an array element
            }
        }
        else {
            compatible = true;
        }
    }
    else if (lhsType == EntryTypeCategory::PRIMITIVE_REAL && rhsType == EntryTypeCategory::PRIMITIVE_INTEGER) {
        compatible = true;
    }

    if (!compatible) {
        recordError("Type mismatch in assignment to '" + node.variable->identifier->name +
            "'. Cannot assign type " + entryTypeToString(rhsType) +
            " to variable of type " + entryTypeToString(lhsType) + ".",
            node.line, node.column);
    }
}

void SemanticAnalyzer::visit(IfStatementNode& node) {
    if (node.condition) {
        node.condition->accept(*this);
        if (node.condition->determinedType != EntryTypeCategory::PRIMITIVE_BOOLEAN &&
            node.condition->determinedType != EntryTypeCategory::UNKNOWN_TYPE) { // Avoid cascading errors
            recordError("IF condition expression evaluated to " + entryTypeToString(node.condition->determinedType) +
                ", but BOOLEAN was expected.", node.condition->line, node.condition->column);
        }
    }
    else {
        recordError("IF statement missing condition.", node.line, node.column);
    }
    if (node.thenStatement) node.thenStatement->accept(*this);
    if (node.elseStatement) node.elseStatement->accept(*this);
}

void SemanticAnalyzer::visit(WhileStatementNode& node) {
    if (node.condition) {
        node.condition->accept(*this);
        if (node.condition->determinedType != EntryTypeCategory::PRIMITIVE_BOOLEAN &&
            node.condition->determinedType != EntryTypeCategory::UNKNOWN_TYPE) { // Avoid cascading errors
            recordError("WHILE condition expression evaluated to " + entryTypeToString(node.condition->determinedType) +
                ", but BOOLEAN was expected.", node.condition->line, node.condition->column);
        }
    }
    else {
        recordError("WHILE statement missing condition.", node.line, node.column);
    }
    if (node.body) node.body->accept(*this);
    else {
        recordError("WHILE statement missing body.", node.line, node.column);
    }
}

void SemanticAnalyzer::visit(VariableNode& node) {
    if (!node.identifier) {
        node.determinedType = EntryTypeCategory::UNKNOWN_TYPE;
        recordError("Internal: VariableNode has no identifier.", node.line, node.column);
        return;
    }
    SymbolEntry* entry = symbolTable.lookupSymbol(node.identifier->name);
    if (!entry) {
        recordError("Identifier '" + node.identifier->name + "' is not declared.", node.identifier->line, node.identifier->column);
        node.determinedType = EntryTypeCategory::UNKNOWN_TYPE;
        return;
    }

    node.offset = entry->offset;
    node.kind = entry->kind;
    if (entry->kind == SymbolKind::VARIABLE || entry->kind == SymbolKind::PARAMETER) {
        if (symbolTable.getCurrentLevel() > 0) { // Inside a subprogram
            SymbolEntry* current_scope_check = symbolTable.lookupSymbolInCurrentScope(node.identifier->name);
            if (current_scope_check) {
                node.scope = SymbolScope::LOCAL;
            }
            else {
                node.scope = SymbolScope::GLOBAL;
            }
        }
        else {
            node.scope = SymbolScope::GLOBAL;
        }
    }
    else {
        node.scope = SymbolScope::GLOBAL;
    }


    if (entry->kind != SymbolKind::VARIABLE && entry->kind != SymbolKind::PARAMETER) {
        recordError("Identifier '" + node.identifier->name + "' (" + symbolKindToString(entry->kind) + ") cannot be used as a variable here.", node.identifier->line, node.identifier->column);
        node.determinedType = EntryTypeCategory::UNKNOWN_TYPE;
        return;
    }

    if (node.index != nullptr) { // Array element access
        if (entry->type != EntryTypeCategory::ARRAY) {
            recordError("Identifier '" + node.identifier->name + "' is of type " + entryTypeToString(entry->type) +
                " and cannot be indexed as an array.", node.identifier->line, node.identifier->column);
            node.determinedType = EntryTypeCategory::UNKNOWN_TYPE;
            return;
        }
        node.index->accept(*this);
        if (node.index->determinedType != EntryTypeCategory::PRIMITIVE_INTEGER &&
            node.index->determinedType != EntryTypeCategory::UNKNOWN_TYPE) {
            recordError("Array index for '" + node.identifier->name + "' must be an INTEGER expression, but found " +
                entryTypeToString(node.index->determinedType) + ".", node.index->line, node.index->column);
        }
        node.determinedType = entry->arrayDetails.elementType;
        node.determinedArrayDetails.isInitialized = false; // This is an an element, not a whole array
    }
    else { // Simple variable or whole array identifier
        node.determinedType = entry->type;
        if (entry->type == EntryTypeCategory::ARRAY) {
            node.determinedArrayDetails = entry->arrayDetails;
        }
        else {
            node.determinedArrayDetails.isInitialized = false;
        }
    }
}

void SemanticAnalyzer::visit(ProcedureCallStatementNode& node) {
    if (!node.procName) {
        recordError("Procedure call missing name.", node.line, node.column);
        return;
    }
    const std::string& procNameStr = node.procName->name;

    // Handle built-in procedures as special cases
    if (procNameStr == "write" || procNameStr == "writeln") {
        if (node.arguments) {
            for (ExprNode* argExpr : node.arguments->expressions) {
                if (argExpr) {
                    argExpr->accept(*this);
                    if (!isPrintableType(argExpr->determinedType, argExpr)) {
                        recordError("Argument type " + entryTypeToString(argExpr->determinedType) + " is not printable.", argExpr->line, argExpr->column);
                    }
                }
            }
        }
        return;
    }
    if (procNameStr == "read" || procNameStr == "readln") {
        if (!node.arguments || node.arguments->expressions.empty()) {
            recordError("'" + procNameStr + "' requires at least one variable argument.", node.procName->line, node.procName->column);
        }
        else {
            for (ExprNode* argExpr : node.arguments->expressions) {
                if (argExpr) {
                    auto* varArg = dynamic_cast<VariableNode*>(argExpr);
                    auto* idArg = dynamic_cast<IdExprNode*>(argExpr);
                    if (!varArg && !idArg) {
                        recordError("Argument to '" + procNameStr + "' must be a variable.", argExpr->line, argExpr->column);
                        continue;
                    }
                    argExpr->accept(*this);
                    if (!isReadableType(argExpr->determinedType)) {
                        recordError("Cannot read into variable of type " + entryTypeToString(argExpr->determinedType) + ".", argExpr->line, argExpr->column);
                    }
                    if (argExpr->determinedType == EntryTypeCategory::ARRAY) {
                        recordError("Cannot read directly into an entire array.", argExpr->line, argExpr->column);
                    }
                }
            }
        }
        return;
    }

    // --- Overload Resolution for User-Defined Procedures ---
    if (node.arguments) {
        node.arguments->accept(*this);
    }
    std::string mangledKey = buildMangledName(procNameStr, SymbolKind::PROCEDURE, node.arguments);
    SymbolEntry* entry = symbolTable.lookupSymbol(mangledKey);

    if (!entry) {
        std::string argTypes;
        if (node.arguments) {
            for (auto it = node.arguments->expressions.begin(); it != node.arguments->expressions.end(); ++it) {
                argTypes += entryTypeToString((*it)->determinedType);
                if (std::next(it) != node.arguments->expressions.end()) argTypes += ", ";
            }
        }
        recordError("No matching procedure '" + procNameStr + "' for arguments (" + argTypes + ").", node.procName->line, node.procName->column);
        return;
    }
    node.resolved_entry = entry;
}

void SemanticAnalyzer::visit(ExpressionList& node) {
    for (ExprNode* expr : node.expressions) {
        if (expr) expr->accept(*this);
    }
}

void SemanticAnalyzer::visit(IntNumNode& node) {
    node.determinedType = EntryTypeCategory::PRIMITIVE_INTEGER;
    node.determinedArrayDetails.isInitialized = false;
}
void SemanticAnalyzer::visit(RealNumNode& node) {
    node.determinedType = EntryTypeCategory::PRIMITIVE_REAL;
    node.determinedArrayDetails.isInitialized = false;
}
void SemanticAnalyzer::visit(BooleanLiteralNode& node) {
    node.determinedType = EntryTypeCategory::PRIMITIVE_BOOLEAN;
    node.determinedArrayDetails.isInitialized = false;
}
void SemanticAnalyzer::visit(StringLiteralNode& node) {
    node.determinedType = EntryTypeCategory::UNKNOWN_TYPE;
    node.determinedArrayDetails.isInitialized = false;
}

void SemanticAnalyzer::visit(BinaryOpNode& node) {
    node.determinedType = EntryTypeCategory::UNKNOWN_TYPE;
    node.determinedArrayDetails.isInitialized = false;

    if (!node.left || !node.right) {
        recordError("Binary operation missing operand(s).", node.line, node.column);
        return;
    }
    node.left->accept(*this);
    node.right->accept(*this);

    EntryTypeCategory leftType = node.left->determinedType;
    EntryTypeCategory rightType = node.right->determinedType;

    if (leftType == EntryTypeCategory::UNKNOWN_TYPE || rightType == EntryTypeCategory::UNKNOWN_TYPE) {
        return;
    }

    const std::string& op = node.op;
    if (op == "+" || op == "-" || op == "*") {
        if ((leftType == EntryTypeCategory::PRIMITIVE_INTEGER || leftType == EntryTypeCategory::PRIMITIVE_REAL) &&
            (rightType == EntryTypeCategory::PRIMITIVE_INTEGER || rightType == EntryTypeCategory::PRIMITIVE_REAL)) {
            node.determinedType = (leftType == EntryTypeCategory::PRIMITIVE_REAL || rightType == EntryTypeCategory::PRIMITIVE_REAL) ?
                EntryTypeCategory::PRIMITIVE_REAL : EntryTypeCategory::PRIMITIVE_INTEGER;
        }
        else {
            recordError("Operands for binary operator '" + op + "' must be numeric.", node.line, node.column);
        }
    }
    else if (op == "/") {
        if ((leftType == EntryTypeCategory::PRIMITIVE_INTEGER || leftType == EntryTypeCategory::PRIMITIVE_REAL) &&
            (rightType == EntryTypeCategory::PRIMITIVE_INTEGER || rightType == EntryTypeCategory::PRIMITIVE_REAL)) {
            node.determinedType = EntryTypeCategory::PRIMITIVE_REAL;
        }
        else {
            recordError("Operands for real division operator '/' must be numeric.", node.line, node.column);
        }
    }
    else if (op == "DIV_OP") {
        if (leftType == EntryTypeCategory::PRIMITIVE_INTEGER && rightType == EntryTypeCategory::PRIMITIVE_INTEGER) {
            node.determinedType = EntryTypeCategory::PRIMITIVE_INTEGER;
        }
        else {
            recordError("Operands for integer division operator 'DIV' must both be INTEGER.", node.line, node.column);
        }
    }
    else if (op == "AND_OP" || op == "OR_OP") {
        if (leftType == EntryTypeCategory::PRIMITIVE_BOOLEAN && rightType == EntryTypeCategory::PRIMITIVE_BOOLEAN) {
            node.determinedType = EntryTypeCategory::PRIMITIVE_BOOLEAN;
        }
        else {
            recordError("Operands for logical operator '" + op + "' must both be BOOLEAN.", node.line, node.column);
        }
    }
    else if (op == "EQ_OP" || op == "NEQ_OP" || op == "LT_OP" || op == "LTE_OP" || op == "GT_OP" || op == "GTE_OP") {
        bool compatible = false;
        if ((leftType == EntryTypeCategory::PRIMITIVE_INTEGER || leftType == EntryTypeCategory::PRIMITIVE_REAL) &&
            (rightType == EntryTypeCategory::PRIMITIVE_INTEGER || rightType == EntryTypeCategory::PRIMITIVE_REAL)) {
            compatible = true;
        }
        else if (leftType == EntryTypeCategory::PRIMITIVE_BOOLEAN && rightType == EntryTypeCategory::PRIMITIVE_BOOLEAN && (op == "EQ_OP" || op == "NEQ_OP")) {
            compatible = true;
        }
        else if (leftType == EntryTypeCategory::ARRAY || rightType == EntryTypeCategory::ARRAY) {
            recordError("Cannot directly compare arrays with operator '" + op + "'.", node.line, node.column);
        }

        if (compatible) {
            node.determinedType = EntryTypeCategory::PRIMITIVE_BOOLEAN;
        }
        else if (!(leftType == EntryTypeCategory::ARRAY || rightType == EntryTypeCategory::ARRAY)) {
            recordError("Operands for relational operator '" + op + "' are not compatible.", node.line, node.column);
        }
    }
    else {
        recordError("Internal: Unknown binary operator '" + op + "'.", node.line, node.column);
    }
}

void SemanticAnalyzer::visit(UnaryOpNode& node) {
    node.determinedType = EntryTypeCategory::UNKNOWN_TYPE;
    node.determinedArrayDetails.isInitialized = false;

    if (!node.expression) {
        recordError("Unary operation missing operand.", node.line, node.column);
        return;
    }
    node.expression->accept(*this);
    EntryTypeCategory operandType = node.expression->determinedType;

    if (operandType == EntryTypeCategory::UNKNOWN_TYPE) {
        return;
    }

    if (node.op == "-") {
        if (operandType == EntryTypeCategory::PRIMITIVE_INTEGER || operandType == EntryTypeCategory::PRIMITIVE_REAL) {
            node.determinedType = operandType;
        }
        else {
            recordError("Operand for unary '-' operator must be numeric.", node.expression->line, node.expression->column);
        }
    }
    else if (node.op == "NOT_OP") {
        if (operandType == EntryTypeCategory::PRIMITIVE_BOOLEAN) {
            node.determinedType = EntryTypeCategory::PRIMITIVE_BOOLEAN;
        }
        else {
            recordError("Operand for 'NOT' operator must be BOOLEAN.", node.expression->line, node.expression->column);
        }
    }
    else {
        recordError("Internal: Unknown unary operator '" + node.op + "'.", node.line, node.column);
    }
}

// MODIFIED: Logic to handle ambiguity between variables and parameter-less functions.
void SemanticAnalyzer::visit(IdExprNode& node) {
    node.determinedType = EntryTypeCategory::UNKNOWN_TYPE;
    node.determinedArrayDetails.isInitialized = false;

    if (!node.ident) {
        recordError("Internal: IdExprNode missing identifier.", node.line, node.column);
        return;
    }

    // First, check if it's a variable or parameter using its simple name.
    SymbolEntry* entry = symbolTable.lookupSymbol(node.ident->name);
    if (entry && (entry->kind == SymbolKind::VARIABLE || entry->kind == SymbolKind::PARAMETER)) {
        node.offset = entry->offset;
        node.kind = entry->kind;
        node.determinedType = entry->type;
        if (entry->type == EntryTypeCategory::ARRAY) {
            node.determinedArrayDetails = entry->arrayDetails;
        }
        if (symbolTable.getCurrentLevel() > 0) {
            node.scope = symbolTable.lookupSymbolInCurrentScope(node.ident->name) ? SymbolScope::LOCAL : SymbolScope::GLOBAL;
        }
        else {
            node.scope = SymbolScope::GLOBAL;
        }
        return;
    }

    // If not a variable, check if it's a parameter-less function call using its mangled name.
    std::string mangledKey = buildMangledName(node.ident->name, SymbolKind::FUNCTION, nullptr);
    SymbolEntry* funcEntry = symbolTable.lookupSymbol(mangledKey);

    if (funcEntry && funcEntry->kind == SymbolKind::FUNCTION) {
        if (funcEntry->numParameters == 0) {
            node.kind = funcEntry->kind;
            node.determinedType = funcEntry->functionReturnType;
            // The codegen will need to be smart about this IdExprNode.
            // A better solution would be to transform this node into a FunctionCallExprNode here.
            // For now, we annotate it and let codegen handle it.
        }
        else {
            recordError("Function '" + node.ident->name + "' requires arguments and parentheses for call.", node.ident->line, node.ident->column);
        }
        return;
    }

    recordError("Identifier '" + node.ident->name + "' is not declared or used incorrectly.", node.ident->line, node.ident->column);
}

// MODIFIED: Complete rewrite for Overload Resolution
void SemanticAnalyzer::visit(FunctionCallExprNode& node) {
    node.determinedType = EntryTypeCategory::UNKNOWN_TYPE;
    if (!node.funcName) {
        recordError("Function call is missing a name.", node.line, node.column);
        return;
    }

    if (node.arguments) {
        node.arguments->accept(*this);
    }

    std::string mangledKey = buildMangledName(node.funcName->name, SymbolKind::FUNCTION, node.arguments);
    SymbolEntry* entry = symbolTable.lookupSymbol(mangledKey);

    if (!entry) {
        std::string argTypes;
        if (node.arguments) {
            for (auto it = node.arguments->expressions.begin(); it != node.arguments->expressions.end(); ++it) {
                argTypes += entryTypeToString((*it)->determinedType);
                if (std::next(it) != node.arguments->expressions.end()) argTypes += ", ";
            }
        }
        recordError("No matching function named '" + node.funcName->name + "' with arguments (" + argTypes + ") was found.", node.funcName->line, node.funcName->column);
        return;
    }

    node.resolved_entry = entry;
    node.determinedType = entry->functionReturnType;

    if (node.arguments && !node.arguments->expressions.empty()) {
        auto actualArgIt = node.arguments->expressions.begin();
        for (size_t i = 0; i < entry->formalParameterSignature.size(); ++i, ++actualArgIt) {
            auto expectedType = entry->formalParameterSignature[i].first;
            auto actualType = (*actualArgIt)->determinedType;
            if (expectedType != actualType && !(expectedType == EntryTypeCategory::PRIMITIVE_REAL && actualType == EntryTypeCategory::PRIMITIVE_INTEGER)) {
                recordError("Argument type mismatch in call to '" + node.funcName->name + "'.", node.line, node.column);
            }
        }
    }
}


void SemanticAnalyzer::visit(ReturnStatementNode& node) {
    if (!currentSubprogramEntry) {
        recordError("RETURN statement found outside of a function.", node.line, node.column);
        return;
    }
    if (currentSubprogramEntry->kind != SymbolKind::FUNCTION) {
        recordError("RETURN statement can only be used inside a function.", node.line, node.column);
        return;
    }

    if (!node.returnValue) {
        recordError("RETURN statement in function '" + currentSubprogramEntry->name + "' must have a return value.", node.line, node.column);
        return;
    }

    node.returnValue->accept(*this);
    EntryTypeCategory actualReturnType = node.returnValue->determinedType;
    EntryTypeCategory expectedReturnType = currentSubprogramEntry->functionReturnType;

    bool compatible = (actualReturnType == expectedReturnType) ||
        (expectedReturnType == EntryTypeCategory::PRIMITIVE_REAL && actualReturnType == EntryTypeCategory::PRIMITIVE_INTEGER);

    if (!compatible && actualReturnType != EntryTypeCategory::UNKNOWN_TYPE) {
        recordError("Return type mismatch in function '" + currentSubprogramEntry->name +
            "'. Expected " + entryTypeToString(expectedReturnType) +
            " but got " + entryTypeToString(actualReturnType) + ".",
            node.returnValue->line, node.returnValue->column);
    }
}