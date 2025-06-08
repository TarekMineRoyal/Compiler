#include "semantic_analyzer.h"
#include <iostream> 

// Constructor: Pre-populate symbol table with built-in I/O procedures
SemanticAnalyzer::SemanticAnalyzer() : currentFunctionContext(nullptr), global_offset(0), local_offset(0), param_offset(0) {
    // Built-in procedures are added to the global scope.
    // They don't have a MiniPascal-defined signature in the same way user procedures do.
    // We'll use an empty signature for their SymbolEntry, and rely on name-based checking
    // for their special argument handling. Line/col are 0 as they are predefined.
    std::vector<std::pair<EntryTypeCategory, ArrayDetails>> empty_signature;

    symbolTable.addSymbol(SymbolEntry("read", empty_signature, 0, 0));
    symbolTable.addSymbol(SymbolEntry("readln", empty_signature, 0, 0));
    symbolTable.addSymbol(SymbolEntry("write", empty_signature, 0, 0));
    symbolTable.addSymbol(SymbolEntry("writeln", empty_signature, 0, 0));
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

// Helper for checking if a type is printable for write/writeln
bool SemanticAnalyzer::isPrintableType(EntryTypeCategory type, ExprNode* argNode) {
    if (type == EntryTypeCategory::PRIMITIVE_INTEGER ||
        type == EntryTypeCategory::PRIMITIVE_REAL ||
        type == EntryTypeCategory::PRIMITIVE_BOOLEAN) {
        return true;
    }
    // Check if it's a StringLiteralNode specifically, since my MiniPascal doesn't have string variables
    if (dynamic_cast<StringLiteralNode*>(argNode)) {
        return true;
    }
    return false;
}

// Helper for checking if a type is readable for read/readln
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
            recordError("Variable '" + identNode->name + "' re-declared in the current scope." + conflictMsg, identNode->line, identNode->column);
        }
    }
}

void SemanticAnalyzer::visit(SubprogramDeclarations& node) {
    for (SubprogramDeclaration* subDecl : node.subprograms) {
        if (subDecl) subDecl->accept(*this);
    }
}

void SemanticAnalyzer::visit(SubprogramDeclaration& node) {
    if (!node.head || !node.head->name) {
        recordError("Subprogram declaration is missing a valid head or name.", node.line, node.column);
        return;
    }

    // First, process the head to add the subprogram name to the current (outer) scope
    node.head->accept(*this);

    // Now, enter a new scope for the parameters and local variables of this subprogram
    symbolTable.enterScope();
    local_offset = 0; // Reset for local variables within this subprogram
    param_offset = 0; // Reset for parameters of this subprogram

    FunctionHeadNode* previousFunctionContext = currentFunctionContext;
    if (auto funcHead = dynamic_cast<FunctionHeadNode*>(node.head)) {
        currentFunctionContext = funcHead;
    }
    else {
        currentFunctionContext = nullptr; // It's a procedure
    }

    // Process parameters - they are added to the new inner scope
    if (node.head->arguments) {
        node.head->arguments->accept(*this);
    }

    // Process local declarations - also added to the new inner scope
    if (node.local_declarations) {
        node.local_declarations->accept(*this);
    }

    // Process the body of the subprogram
    if (node.body) {
        node.body->accept(*this);
    }

    // Exit the scope for this subprogram
    symbolTable.exitScope();
    currentFunctionContext = previousFunctionContext; // Restore context
}

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
            else if (param_decl_group) {
                recordError("Malformed parameter declaration in function '" + node.name->name + "'.", param_decl_group->line, param_decl_group->column);
            }
        }
    }

    SymbolEntry entry(node.name->name, return_type, signature, node.name->line, node.name->column);
    // The function/procedure itself doesn't have an 'offset' in the traditional variable sense.
    // Its "offset" is its starting address in the code, which is handled by labels.
    if (!symbolTable.addSymbol(entry)) {
        SymbolEntry* existing = symbolTable.lookupSymbolInCurrentScope(node.name->name);
        std::string conflictMsg = existing ? " Conflicts with existing " + symbolKindToString(existing->kind) + " declared at L:" + std::to_string(existing->declLine) : "";
        recordError("Function '" + node.name->name + "' re-declared in current scope." + conflictMsg, node.name->line, node.name->column);
    }
}

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
            else if (param_decl_group) {
                recordError("Malformed parameter declaration in procedure '" + node.name->name + "'.", param_decl_group->line, param_decl_group->column);
            }
        }
    }
    SymbolEntry entry(node.name->name, signature, node.name->line, node.name->column);
    if (!symbolTable.addSymbol(entry)) {
        SymbolEntry* existing = symbolTable.lookupSymbolInCurrentScope(node.name->name);
        std::string conflictMsg = existing ? " Conflicts with existing " + symbolKindToString(existing->kind) + " declared at L:" + std::to_string(existing->declLine) : "";
        recordError("Procedure '" + node.name->name + "' re-declared in current scope." + conflictMsg, node.name->line, node.name->column);
    }
}

void SemanticAnalyzer::visit(ArgumentsNode& node) {
    // This method processes the parameters *within* the new scope of the subprogram.
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
            // If this is an array element access on LHS, it's fine. 
            // If it's a whole array variable, then error.
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

    // Annotate the AST node with info from the symbol table
    node.offset = entry->offset;
    node.kind = entry->kind;
    if (entry->kind == SymbolKind::VARIABLE || entry->kind == SymbolKind::PARAMETER) {
        // Determine scope based on where the symbol entry was found.
        if (symbolTable.getCurrentLevel() > 0) { // Inside a subprogram
            SymbolEntry* current_scope_check = symbolTable.lookupSymbolInCurrentScope(node.identifier->name);
            if (current_scope_check) { // Found in current (local/param) scope
                node.scope = SymbolScope::LOCAL;
            }
            else { // Found in an outer (global) scope
                node.scope = SymbolScope::GLOBAL;
            }
        }
        else { // Global scope
            node.scope = SymbolScope::GLOBAL;
        }
    }
    else {
        // Default for things like function names used as variables (which is an error caught below)
        node.scope = SymbolScope::GLOBAL; // Or UNKNOWN, but GLOBAL is safer for offset if misused
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
        recordError("Procedure call statement is missing procedure name.", node.line, node.column);
        return;
    }
    const std::string& procNameStr = node.procName->name;
    SymbolEntry* entry = symbolTable.lookupSymbol(procNameStr);

    if (!entry) {
        recordError("Procedure or function '" + procNameStr + "' is not declared.", node.procName->line, node.procName->column);
        return;
    }

    // Special handling for built-in I/O
    if (procNameStr == "write" || procNameStr == "writeln") {
        if (node.arguments) {
            for (ExprNode* argExpr : node.arguments->expressions) {
                if (argExpr) {
                    argExpr->accept(*this);
                    if (!isPrintableType(argExpr->determinedType, argExpr)) {
                        recordError("Argument of type " + entryTypeToString(argExpr->determinedType) +
                            " is not printable by '" + procNameStr + "'.", argExpr->line, argExpr->column);
                    }
                }
            }
        } // No arguments is also valid for writeln
        return; // Skip standard procedure checks
    }
    else if (procNameStr == "read" || procNameStr == "readln") {
        if (!node.arguments || node.arguments->expressions.empty()) {
            recordError("Built-in procedure '" + procNameStr + "' requires at least one variable argument.", node.procName->line, node.procName->column);
        }
        else {
            for (ExprNode* argExpr : node.arguments->expressions) {
                if (argExpr) {
                    // Arguments to read/readln must be variables (l-values)
                    VariableNode* varArg = dynamic_cast<VariableNode*>(argExpr);
                    IdExprNode* idArg = dynamic_cast<IdExprNode*>(argExpr); // Handle simple IDs too

                    if (!varArg && !idArg) {
                        recordError("Argument to '" + procNameStr + "' must be a variable.", argExpr->line, argExpr->column);
                        continue;
                    }

                    // Accept the node to determine its type and other properties
                    argExpr->accept(*this);


                    EntryTypeCategory argActualType = EntryTypeCategory::UNKNOWN_TYPE;
                    std::string argName = "";
                    bool isWholeArrayAccess = false;

                    if (varArg) {
                        argActualType = varArg->determinedType;
                        argName = varArg->identifier->name;
                        isWholeArrayAccess = (varArg->determinedType == EntryTypeCategory::ARRAY && varArg->index == nullptr);
                    }
                    else if (idArg) { // It's a simple identifier, check its properties
                        SymbolEntry* idEntry = symbolTable.lookupSymbol(idArg->ident->name);
                        if (idEntry) {
                            argActualType = idEntry->type;
                            if (idEntry->type == EntryTypeCategory::ARRAY) { // If it's an array but used as simple ID
                                isWholeArrayAccess = true;
                            }
                        }
                        argName = idArg->ident->name;
                    }


                    if (!isReadableType(argActualType)) {
                        recordError("Variable argument of type " + entryTypeToString(argActualType) +
                            " cannot be read into by '" + procNameStr + "'.", argExpr->line, argExpr->column);
                    }
                    if (isWholeArrayAccess) {
                        recordError("Cannot read directly into an entire array '" + argName +
                            "' with '" + procNameStr + "'. Specify an element.", argExpr->line, argExpr->column);
                    }
                }
            }
        }
        return; // Skip standard procedure checks
    }

    // Standard user-defined procedure call
    if (entry->kind != SymbolKind::PROCEDURE) {
        recordError("'" + procNameStr + "' is a " + symbolKindToString(entry->kind) + ", not a procedure. Cannot call.", node.procName->line, node.procName->column);
        return;
    }

    size_t actualArgCount = node.arguments ? node.arguments->expressions.size() : 0;
    if (actualArgCount != entry->numParameters) {
        recordError("Procedure '" + procNameStr + "' called with " + std::to_string(actualArgCount) +
            " arguments, but definition expects " + std::to_string(entry->numParameters) + ".",
            node.procName->line, node.procName->column);
        // Continue to check types for the arguments we do have, if counts match somewhat
    }

    if (node.arguments) node.arguments->accept(*this); // Process arguments to determine their types

    if (node.arguments && actualArgCount > 0 && entry->numParameters > 0) {
        auto actualArgIt = node.arguments->expressions.begin();
        for (size_t i = 0; i < std::min(actualArgCount, entry->numParameters); ++i, ++actualArgIt) {
            if (actualArgIt == node.arguments->expressions.end()) break; // Should not happen if counts match
            ExprNode* argExpr = *actualArgIt;
            if (!argExpr) continue;

            const auto& formalParamInfo = entry->formalParameterSignature[i];
            EntryTypeCategory expectedType = formalParamInfo.first;
            const ArrayDetails& expectedArrayDetails = formalParamInfo.second;
            EntryTypeCategory actualType = argExpr->determinedType; // Type determined by visiting the argument
            const ArrayDetails& actualArrayDetails = argExpr->determinedArrayDetails;

            bool compatible = false;
            if (actualType == expectedType) {
                if (expectedType == EntryTypeCategory::ARRAY) {
                    // For array parameters, MiniPascal usually passes by reference conceptually,
                    // but for by-value, the structures (element type, bounds) should match.
                    // Your current system implies by-value.
                    compatible = (actualArrayDetails.isInitialized && expectedArrayDetails.isInitialized &&
                        actualArrayDetails.elementType == expectedArrayDetails.elementType &&
                        (actualArrayDetails.lowBound == expectedArrayDetails.lowBound && actualArrayDetails.highBound == expectedArrayDetails.highBound) // For strict matching
                        // Or just actualArrayDetails.elementType == expectedArrayDetails.elementType if bounds don't need to match for parameters
                        );
                }
                else {
                    compatible = true;
                }
            }
            else if (expectedType == EntryTypeCategory::PRIMITIVE_REAL && actualType == EntryTypeCategory::PRIMITIVE_INTEGER) {
                compatible = true; // INTEGER can be passed to REAL parameter
            }

            if (!compatible) {
                recordError("Type mismatch for argument " + std::to_string(i + 1) + " in call to procedure '" + procNameStr +
                    "'. Expected " + entryTypeToString(expectedType) +
                    (expectedType == EntryTypeCategory::ARRAY ? " of " + entryTypeToString(expectedArrayDetails.elementType) : "") +
                    " but got " + entryTypeToString(actualType) +
                    (actualType == EntryTypeCategory::ARRAY && actualArrayDetails.isInitialized ? " of " + entryTypeToString(actualArrayDetails.elementType) : "") + ".",
                    argExpr->line, argExpr->column);
            }
        }
    }
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
void SemanticAnalyzer::visit(BooleanLiteralNode& node) { // Corrected: Removed extra 'void'
    node.determinedType = EntryTypeCategory::PRIMITIVE_BOOLEAN;
    node.determinedArrayDetails.isInitialized = false;
}
void SemanticAnalyzer::visit(StringLiteralNode& node) { // Corrected: Removed extra 'void'
    // In MiniPascal, string literals are primarily for 'write'. They don't have a formal type
    // that can be assigned to variables. We mark it UNKNOWN_TYPE, and 'isPrintableType' handles it.
    node.determinedType = EntryTypeCategory::UNKNOWN_TYPE;
    node.determinedArrayDetails.isInitialized = false;
}

void SemanticAnalyzer::visit(BinaryOpNode& node) {
    node.determinedType = EntryTypeCategory::UNKNOWN_TYPE; // Default
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
        // Error already recorded for operands
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
            recordError("Operands for binary operator '" + op + "' must both be numeric (INTEGER or REAL). Found " +
                entryTypeToString(leftType) + " and " + entryTypeToString(rightType) + ".", node.line, node.column);
        }
    }
    else if (op == "/") { // Real division
        if ((leftType == EntryTypeCategory::PRIMITIVE_INTEGER || leftType == EntryTypeCategory::PRIMITIVE_REAL) &&
            (rightType == EntryTypeCategory::PRIMITIVE_INTEGER || rightType == EntryTypeCategory::PRIMITIVE_REAL)) {
            node.determinedType = EntryTypeCategory::PRIMITIVE_REAL;
        }
        else {
            recordError("Operands for real division operator '/' must both be numeric. Found " +
                entryTypeToString(leftType) + " and " + entryTypeToString(rightType) + ".", node.line, node.column);
        }
    }
    else if (op == "DIV_OP") { // Integer division
        if (leftType == EntryTypeCategory::PRIMITIVE_INTEGER && rightType == EntryTypeCategory::PRIMITIVE_INTEGER) {
            node.determinedType = EntryTypeCategory::PRIMITIVE_INTEGER;
        }
        else {
            recordError("Operands for integer division operator 'DIV' must both be INTEGER. Found " +
                entryTypeToString(leftType) + " and " + entryTypeToString(rightType) + ".", node.line, node.column);
        }
    }
    else if (op == "AND_OP" || op == "OR_OP") {
        if (leftType == EntryTypeCategory::PRIMITIVE_BOOLEAN && rightType == EntryTypeCategory::PRIMITIVE_BOOLEAN) {
            node.determinedType = EntryTypeCategory::PRIMITIVE_BOOLEAN;
        }
        else {
            recordError("Operands for logical operator '" + op + "' must both be BOOLEAN. Found " +
                entryTypeToString(leftType) + " and " + entryTypeToString(rightType) + ".", node.line, node.column);
        }
    }
    // Relational operators: EQ_OP, NEQ_OP, LT_OP, LTE_OP, GT_OP, GTE_OP
    else if (op == "EQ_OP" || op == "NEQ_OP" || op == "LT_OP" || op == "LTE_OP" || op == "GT_OP" || op == "GTE_OP") {
        bool compatibleTypesForRelational = false;
        // Numeric types can be compared with each other
        if ((leftType == EntryTypeCategory::PRIMITIVE_INTEGER || leftType == EntryTypeCategory::PRIMITIVE_REAL) &&
            (rightType == EntryTypeCategory::PRIMITIVE_INTEGER || rightType == EntryTypeCategory::PRIMITIVE_REAL)) {
            compatibleTypesForRelational = true;
        }
        // Booleans can only be compared for equality/inequality
        else if (leftType == EntryTypeCategory::PRIMITIVE_BOOLEAN && rightType == EntryTypeCategory::PRIMITIVE_BOOLEAN && (op == "EQ_OP" || op == "NEQ_OP")) {
            compatibleTypesForRelational = true;
        }
        // Arrays cannot be compared directly
        else if (leftType == EntryTypeCategory::ARRAY || rightType == EntryTypeCategory::ARRAY) {
            recordError("Cannot directly compare arrays with operator '" + op + "'.", node.line, node.column);
        }


        if (compatibleTypesForRelational) {
            node.determinedType = EntryTypeCategory::PRIMITIVE_BOOLEAN;
        }
        else {
            // If not already caught by array check
            if (!(leftType == EntryTypeCategory::ARRAY || rightType == EntryTypeCategory::ARRAY)) {
                recordError("Operands for relational operator '" + op + "' are not compatible or operation is not supported for types " +
                    entryTypeToString(leftType) + " and " + entryTypeToString(rightType) + ".", node.line, node.column);
            }
        }
    }
    else {
        recordError("Internal: Unknown binary operator '" + op + "'.", node.line, node.column);
    }
}

void SemanticAnalyzer::visit(UnaryOpNode& node) {
    node.determinedType = EntryTypeCategory::UNKNOWN_TYPE; // Default
    node.determinedArrayDetails.isInitialized = false;

    if (!node.expression) {
        recordError("Unary operation missing operand.", node.line, node.column);
        return;
    }
    node.expression->accept(*this);
    EntryTypeCategory operandType = node.expression->determinedType;

    if (operandType == EntryTypeCategory::UNKNOWN_TYPE) {
        // Error already recorded for operand
        return;
    }

    if (node.op == "-") { // Unary minus
        if (operandType == EntryTypeCategory::PRIMITIVE_INTEGER || operandType == EntryTypeCategory::PRIMITIVE_REAL) {
            node.determinedType = operandType;
        }
        else {
            recordError("Operand for unary '-' operator must be numeric (INTEGER or REAL), but found " +
                entryTypeToString(operandType) + ".", node.expression->line, node.expression->column);
        }
    }
    else if (node.op == "NOT_OP") {
        if (operandType == EntryTypeCategory::PRIMITIVE_BOOLEAN) {
            node.determinedType = EntryTypeCategory::PRIMITIVE_BOOLEAN;
        }
        else {
            recordError("Operand for 'NOT' operator must be BOOLEAN, but found " +
                entryTypeToString(operandType) + ".", node.expression->line, node.expression->column);
        }
    }
    else {
        recordError("Internal: Unknown unary operator '" + node.op + "'.", node.line, node.column);
    }
}

void SemanticAnalyzer::visit(IdExprNode& node) {
    // This node represents an identifier used directly as an expression.
    // It could be a simple variable, a parameter, or a function call if the function takes no arguments.
    node.determinedType = EntryTypeCategory::UNKNOWN_TYPE; // Default
    node.determinedArrayDetails.isInitialized = false;

    if (!node.ident) {
        recordError("Internal: IdExprNode missing identifier.", node.line, node.column);
        return;
    }
    SymbolEntry* entry = symbolTable.lookupSymbol(node.ident->name);
    if (!entry) {
        recordError("Identifier '" + node.ident->name + "' is not declared.", node.ident->line, node.ident->column);
        return;
    }

    // Annotate IdExprNode with the details from the symbol table
    node.offset = entry->offset;
    node.kind = entry->kind;

    if (entry->kind == SymbolKind::VARIABLE || entry->kind == SymbolKind::PARAMETER) {
        node.determinedType = entry->type;
        if (entry->type == EntryTypeCategory::ARRAY) {
            node.determinedArrayDetails = entry->arrayDetails;
        }
        // Determine scope for IdExprNode as well (similar to VariableNode)
        if (symbolTable.getCurrentLevel() > 0) { // Inside a subprogram
            SymbolEntry* current_scope_check = symbolTable.lookupSymbolInCurrentScope(node.ident->name);
            if (current_scope_check && current_scope_check->declLine == entry->declLine && current_scope_check->declColumn == entry->declColumn) {
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
    else if (entry->kind == SymbolKind::FUNCTION) {
        if (entry->numParameters == 0) { // Parameterless function call
            node.determinedType = entry->functionReturnType;
        }
        else {
            recordError("Function '" + node.ident->name + "' requires arguments and parentheses for call.", node.ident->line, node.ident->column);
        }
        node.scope = SymbolScope::GLOBAL; // Functions are always global in this context
    }
    else {
        recordError("Identifier '" + node.ident->name + "' (" + symbolKindToString(entry->kind) + ") cannot be used as a simple expression here (e.g., might be a procedure).", node.ident->line, node.ident->column);
    }
}

void SemanticAnalyzer::visit(FunctionCallExprNode& node) {
    node.determinedType = EntryTypeCategory::UNKNOWN_TYPE; // Default
    node.determinedArrayDetails.isInitialized = false;

    if (!node.funcName) {
        recordError("Function call is missing function name.", node.line, node.column);
        return;
    }
    SymbolEntry* entry = symbolTable.lookupSymbol(node.funcName->name);
    if (!entry) {
        recordError("Function '" + node.funcName->name + "' is not declared.", node.funcName->line, node.funcName->column);
        return;
    }
    if (entry->kind != SymbolKind::FUNCTION) {
        recordError("'" + node.funcName->name + "' is a " + symbolKindToString(entry->kind) + ", not a function. Cannot call.", node.funcName->line, node.funcName->column);
        return;
    }

    node.determinedType = entry->functionReturnType; // Set the type of the expression to the function's return type

    size_t actualArgCount = node.arguments ? node.arguments->expressions.size() : 0;
    if (actualArgCount != entry->numParameters) {
        recordError("Function '" + node.funcName->name + "' called with " + std::to_string(actualArgCount) +
            " arguments, but definition expects " + std::to_string(entry->numParameters) + ".",
            node.funcName->line, node.funcName->column);
    }

    if (node.arguments) node.arguments->accept(*this); // Process arguments to determine their types

    // Argument type checking (same as for procedures)
    if (node.arguments && actualArgCount > 0 && entry->numParameters > 0) {
        auto actualArgIt = node.arguments->expressions.begin();
        for (size_t i = 0; i < std::min(actualArgCount, entry->numParameters); ++i, ++actualArgIt) {
            if (actualArgIt == node.arguments->expressions.end()) break;
            ExprNode* argExpr = *actualArgIt;
            if (!argExpr) continue;

            const auto& formalParamInfo = entry->formalParameterSignature[i];
            EntryTypeCategory expectedType = formalParamInfo.first;
            const ArrayDetails& expectedArrayDetails = formalParamInfo.second; // if formal is array
            EntryTypeCategory actualType = argExpr->determinedType;
            const ArrayDetails& actualArrayDetails = argExpr->determinedArrayDetails; // if actual is array

            bool compatible = false;
            if (actualType == expectedType) {
                if (expectedType == EntryTypeCategory::ARRAY) {
                    compatible = (actualArrayDetails.isInitialized && expectedArrayDetails.isInitialized &&
                        actualArrayDetails.elementType == expectedArrayDetails.elementType);
                    // Bounds checking for parameters might be too strict for by-value,
                    // often only element type matters.
                }
                else {
                    compatible = true;
                }
            }
            else if (expectedType == EntryTypeCategory::PRIMITIVE_REAL && actualType == EntryTypeCategory::PRIMITIVE_INTEGER) {
                compatible = true;
            }

            if (!compatible) {
                recordError("Type mismatch for argument " + std::to_string(i + 1) + " in call to function '" + node.funcName->name +
                    "'. Expected " + entryTypeToString(expectedType) +
                    (expectedType == EntryTypeCategory::ARRAY ? " of " + entryTypeToString(expectedArrayDetails.elementType) : "") +
                    " but got " + entryTypeToString(actualType) +
                    (actualType == EntryTypeCategory::ARRAY && actualArrayDetails.isInitialized ? " of " + entryTypeToString(actualArrayDetails.elementType) : "") + ".",
                    argExpr->line, argExpr->column);
            }
        }
    }
}

void SemanticAnalyzer::visit(ReturnStatementNode& node) {
    if (!currentFunctionContext || !currentFunctionContext->name) {
        recordError("RETURN statement found outside of a function or in a function with no name context.", node.line, node.column);
        // If it's a procedure, RETURN is allowed without a value (implicitly).
        // But the grammar for RETURN probably always expects an expression.
        // Let's assume for now the grammar forces an expression.
        if (!node.returnValue) { // If grammar allows 'RETURN;' in procedure
            // If currentFunctionContext is null, it means we are in a procedure.
            // RETURN; is valid in a procedure.
            if (currentFunctionContext != nullptr) { // Error only if in function and no value
                recordError("RETURN statement in function '" + (currentFunctionContext->name ? currentFunctionContext->name->name : "unknown") +
                    "' has no return value, which is required.", node.line, node.column);
            }
        }
        return;
    }
    // We are in a function context.
    if (!node.returnValue) {
        recordError("RETURN statement in function '" + currentFunctionContext->name->name +
            "' has no return value, which is required.", node.line, node.column);
        return;
    }

    node.returnValue->accept(*this);
    EntryTypeCategory actualReturnType = node.returnValue->determinedType;

    SymbolEntry* funcEntry = symbolTable.lookupSymbol(currentFunctionContext->name->name);
    if (!funcEntry || funcEntry->kind != SymbolKind::FUNCTION) {
        recordError("Internal error: Could not retrieve definition for current function '" +
            currentFunctionContext->name->name + "' for return check.", node.line, node.column);
        return;
    }
    EntryTypeCategory expectedReturnType = funcEntry->functionReturnType;

    bool compatible = false;
    if (actualReturnType == expectedReturnType) {
        compatible = true;
    }
    else if (expectedReturnType == EntryTypeCategory::PRIMITIVE_REAL && actualReturnType == EntryTypeCategory::PRIMITIVE_INTEGER) {
        compatible = true; // INTEGER can be returned for a REAL function type
    }

    if (!compatible && actualReturnType != EntryTypeCategory::UNKNOWN_TYPE) { // Avoid cascading
        recordError("Return type mismatch in function '" + currentFunctionContext->name->name +
            "'. Function expects " + entryTypeToString(expectedReturnType) +
            " but RETURN statement provides " + entryTypeToString(actualReturnType) + ".",
            node.returnValue->line, node.returnValue->column);
    }
}