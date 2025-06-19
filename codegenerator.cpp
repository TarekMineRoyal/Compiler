#include "codegenerator.h"
#include <stdexcept>
#include <iostream>
#include <list>
#include <algorithm> // For std::reverse

// --- Entry Point ---

std::string CodeGenerator::generateCode(ProgramNode& ast_root, SemanticAnalyzer& semanticAnalyzer) {
    this->symbolTable = &semanticAnalyzer.getSymbolTable();
    ast_root.accept(*this);
    return code.str();
}

// --- Helper Methods ---

std::string CodeGenerator::newLabel(const std::string& prefix) {
    return "L_" + prefix + "_" + std::to_string(labelCounter++);
}

void CodeGenerator::emit(const std::string& instruction) {
    code << "    " << instruction << std::endl;
}

void CodeGenerator::emit(const std::string& instruction, const std::string& arg) {
    code << "    " << instruction << " " << arg << std::endl;
}

void CodeGenerator::emitLabel(const std::string& label) {
    code << label << ":" << std::endl;
}

// --- Visitor Implementations ---

void CodeGenerator::visit(ProgramNode& node) {
    emit("start");
    if (node.subprogs && !node.subprogs->subprograms.empty()) {
        emit("jump", "main_entry");
    }
    if (node.subprogs) node.subprogs->accept(*this);
    emitLabel("main_entry");
    if (node.decls) node.decls->accept(*this);
    if (node.mainCompoundStmt) node.mainCompoundStmt->accept(*this);
    emit("stop");
}

void CodeGenerator::visit(Declarations& node) {
    if (symbolTable->isGlobalScope()) {
        int simpleVarCount = 0;
        for (auto* decl : node.var_decl_items) {
            if (dynamic_cast<ArrayTypeNode*>(decl->type)) {
                continue;
            }
            simpleVarCount += decl->identifiers->identifiers.size();
        }
        if (simpleVarCount > 0) {
            emit("pushn", std::to_string(simpleVarCount));
        }
    }

    for (auto* varDecl : node.var_decl_items) {
        varDecl->accept(*this);
    }
}

void CodeGenerator::visit(VarDecl& node) {
    ArrayDetails ad;
    EntryTypeCategory var_type = astToSymbolType(node.type, ad);

    for (auto* ident : node.identifiers->identifiers) {
        SymbolEntry entry(ident->name, SymbolKind::VARIABLE, var_type, ident->line, ident->column);

        entry.offset = local_offset++;

        if (var_type == EntryTypeCategory::ARRAY) {
            entry.arrayDetails = ad;
        }
        symbolTable->addSymbol(entry);
    }

    // This existing logic for handling array heap-allocation will now work
    // because the lookupSymbol call below will succeed.
    if (auto* arrayType = dynamic_cast<ArrayTypeNode*>(node.type)) {
        int low = arrayType->startIndex->value;
        int high = arrayType->endIndex->value;
        int size = high - low + 1;
        if (size <= 0) {
            throw std::runtime_error("Array size must be positive.");
        }
        for (auto* ident : node.identifiers->identifiers) {
            // This lookup will now find the symbol we just added above.
            SymbolEntry* entry = symbolTable->lookupSymbol(ident->name);
            if (!entry) {
                // This should not happen anymore, but keep for safety.
                throw std::runtime_error("CodeGen: Symbol not found during array allocation: " + ident->name);
            }
            emit("alloc", std::to_string(size));

            if (symbolTable->isGlobalScope()) {
                emit("storeg", std::to_string(entry->offset));
            }
            else {
                // This will now correctly store the heap address in the pre-allocated stack slot.
                emit("storel", std::to_string(entry->offset));
            }
        }
    }
}


void CodeGenerator::visit(SubprogramDeclarations& node) {
    for (auto* subprog : node.subprograms) {
        if (subprog) subprog->accept(*this);
    }
}

// MODIFIED: This function now finds and stores the subprogram's symbol entry for context.
void CodeGenerator::visit(SubprogramDeclaration& node) {
    SubprogramHead* previousContext = currentFunctionContext;
    currentFunctionContext = node.head;

    // Look up the symbol entry that the semantic analyzer created.
    SymbolEntry* entry = nullptr;
    if (node.head) {
        // We have to reconstruct the mangled name from the declaration to look it up,
        // since the symbol table is keyed by mangled names for subprograms.
        std::string mangledKey;
        if (dynamic_cast<FunctionHeadNode*>(node.head)) {
            mangledKey = "f_" + node.head->name->name;
        }
        else {
            mangledKey = "p_" + node.head->name->name;
        }

        if (node.head->arguments && node.head->arguments->params) {
            for (const auto& param_decl_group : node.head->arguments->params->paramDeclarations) {
                if (param_decl_group && param_decl_group->ids && param_decl_group->type) {
                    ArrayDetails ad;
                    EntryTypeCategory type = astToSymbolType(param_decl_group->type, ad);
                    for (size_t i = 0; i < param_decl_group->ids->identifiers.size(); ++i) {
                        mangledKey += "_";
                        switch (type) {
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
        entry = symbolTable->lookupSymbol(mangledKey);
    }
    if (!entry) throw std::runtime_error("CodeGen: Could not find symbol table entry for subprogram: " + node.head->name->name);

    SymbolEntry* previousEntry = currentSubprogramEntry;
    currentSubprogramEntry = entry;

    std::string mangledName = entry->getMangledName();
    std::string endLabel = mangledName + "_end";

    emit("jump", endLabel);
    emitLabel(mangledName);

    symbolTable->enterScope();
    local_offset = 0;
    param_offset = 0;

    int totalLocalVars = 0;
    if (node.local_declarations) {
        for (auto* decl : node.local_declarations->var_decl_items) {
            if (decl && decl->identifiers) {
                totalLocalVars += decl->identifiers->identifiers.size();
            }
        }
    }

    // Emit a single PUSHN to allocate the entire stack frame for locals.
    if (totalLocalVars > 0) {
        emit("pushn", std::to_string(totalLocalVars));
    }

    if (node.head->arguments) node.head->arguments->accept(*this);
    if (node.local_declarations) node.local_declarations->accept(*this);
    if (node.body) node.body->accept(*this);

    if (dynamic_cast<ProcedureHeadNode*>(node.head)) {
        emit("return");
    }

    emitLabel(endLabel);
    symbolTable->exitScope();
    currentFunctionContext = previousContext;
    currentSubprogramEntry = previousEntry;
}

void CodeGenerator::visit(ArgumentsNode& node) {
    if (node.params) node.params->accept(*this);
}

void CodeGenerator::visit(ParameterList& node) {
    for (auto* param : node.paramDeclarations) {
        param->accept(*this);
    }
}

void CodeGenerator::visit(ParameterDeclaration& node) {
    ArrayDetails ad;
    EntryTypeCategory param_type = astToSymbolType(node.type, ad);
    for (auto* ident : node.ids->identifiers) {
        SymbolEntry entry(ident->name, SymbolKind::PARAMETER, param_type, ident->line, ident->column);
        entry.offset = param_offset++;
        if (param_type == EntryTypeCategory::ARRAY) {
            entry.arrayDetails = ad;
        }
        symbolTable->addSymbol(entry);
    }
}

void CodeGenerator::visit(CompoundStatementNode& node) {
    if (node.stmts) node.stmts->accept(*this);
}

void CodeGenerator::visit(StatementList& node) {
    for (StatementNode* stmt : node.statements) {
        if (stmt) stmt->accept(*this);
    }
}

void CodeGenerator::visit(AssignStatementNode& node) {
    if (auto* varNode = dynamic_cast<VariableNode*>(node.variable)) {
        if (varNode->index) { // Assignment to an array element
            SymbolEntry* arrayEntry = symbolTable->lookupSymbol(varNode->identifier->name);
            if (!arrayEntry) {
                throw std::runtime_error("CodeGen: Array symbol not found: " + varNode->identifier->name);
            }
            int lowerBound = arrayEntry->arrayDetails.lowBound;

            // --- MODIFIED LOGIC STARTS HERE ---
            // The special case for literal indices has been removed.
            // This single, unified logic now handles all array assignments.

            // 1. Push the base address of the array (global or local).
            if (varNode->scope == SymbolScope::LOCAL) {
                emit("pushl", std::to_string(varNode->offset));
            }
            else {
                emit("pushg", std::to_string(varNode->offset));
            }

            // 2. Evaluate the index expression and push its value onto the stack.
            //    This works for both literals (e.g., [5]) and variables (e.g., [i]).
            varNode->index->accept(*this);

            // 3. Subtract the array's lower bound to get the correct offset from the base address.
            if (lowerBound != 0) {
                emit("pushi", std::to_string(lowerBound));
                emit("sub");
            }

            // 4. Evaluate the right-hand side expression and push its value.
            node.expression->accept(*this);
            if (varNode->determinedType == EntryTypeCategory::PRIMITIVE_REAL && node.expression->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) {
                emit("itof"); // Handle type promotion if necessary
            }

            // 5. Use 'storen', which takes address, index, and value from the stack.
            emit("storen");

            // --- MODIFIED LOGIC ENDS HERE ---

        }
        else { // Assignment to a simple variable (this part remains unchanged)
            node.expression->accept(*this);
            if (varNode->determinedType == EntryTypeCategory::PRIMITIVE_REAL && node.expression->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) {
                emit("itof");
            }
            SymbolEntry* entry = symbolTable->lookupSymbol(varNode->identifier->name);
            if (!entry) throw std::runtime_error("CodeGen: Symbol not found in assignment: " + varNode->identifier->name);

            if (entry->kind == SymbolKind::PARAMETER) {
                emit("storel", std::to_string(-(entry->offset + 1)));
            }
            else {
                if (varNode->scope == SymbolScope::LOCAL) {
                    emit("storel", std::to_string(entry->offset));
                }
                else {
                    emit("storeg", std::to_string(entry->offset));
                }
            }
        }
    }
}

void CodeGenerator::visit(VariableNode& node) {
    SymbolEntry* entry = symbolTable->lookupSymbol(node.identifier->name);
    if (!entry) {
        throw std::runtime_error("CodeGen: Symbol not found: " + node.identifier->name);
    }

    // This part for parameter access remains the same
    if (entry->kind == SymbolKind::PARAMETER) {
        emit("pushl", std::to_string(-(entry->offset + 1)));
        return;
    }

    if (node.index) { // This is for reading from an array element
        if (!entry->arrayDetails.isInitialized) {
            throw std::runtime_error("CodeGen: Array details not found for " + node.identifier->name);
        }
        int lowerBound = entry->arrayDetails.lowBound;

        // --- MODIFIED LOGIC STARTS HERE ---
        // The special case for literal indices is removed.
        // This unified logic now handles all array reads.

        // 1. Push the base address of the array.
        if (node.scope == SymbolScope::LOCAL) {
            emit("pushl", std::to_string(entry->offset));
        }
        else {
            emit("pushg", std::to_string(entry->offset));
        }

        // 2. Evaluate the index expression and push its value onto the stack.
        node.index->accept(*this);

        // 3. Subtract the array's lower bound to get the correct offset.
        if (lowerBound != 0) {
            emit("pushi", std::to_string(lowerBound));
            emit("sub");
        }

        // 4. Use 'loadn', which takes the address and calculated index from the stack.
        emit("loadn");

        // --- MODIFIED LOGIC ENDS HERE ---

    }
    else { // Reading from a simple variable (this part remains unchanged)
        if (node.scope == SymbolScope::LOCAL) {
            emit("pushl", std::to_string(entry->offset));
        }
        else {
            emit("pushg", std::to_string(entry->offset));
        }
    }
}

void CodeGenerator::visit(IdExprNode& node) {
    if (node.kind == SymbolKind::FUNCTION) {
        std::string mangledName = "f_" + node.ident->name;
        emit("pushn", "1");
        emit("pusha", mangledName);
        emit("call");
        return;
    }

    SymbolEntry* entry = symbolTable->lookupSymbol(node.ident->name);
    if (!entry) throw std::runtime_error("CodeGen: Symbol not found for identifier: " + node.ident->name);

    if (entry->kind == SymbolKind::PARAMETER) {
        emit("pushl", std::to_string(-(entry->offset + 1)));
    }
    else if (node.scope == SymbolScope::LOCAL) {
        emit("pushl", std::to_string(entry->offset));
    }
    else {
        emit("pushg", std::to_string(entry->offset));
    }
}

void CodeGenerator::visit(IfStatementNode& node) {
    std::string elseLabel = newLabel("ELSE");
    std::string endIfLabel = newLabel("END_IF");
    node.condition->accept(*this);
    emit("jz", elseLabel);
    node.thenStatement->accept(*this);
    if (node.elseStatement) emit("jump", endIfLabel);
    emitLabel(elseLabel);
    if (node.elseStatement) node.elseStatement->accept(*this);
    emitLabel(endIfLabel);
}

void CodeGenerator::visit(WhileStatementNode& node) {
    std::string loopStartLabel = newLabel("WHILE_START");
    std::string loopEndLabel = newLabel("WHILE_END");
    emitLabel(loopStartLabel);
    node.condition->accept(*this);
    emit("jz", loopEndLabel);
    node.body->accept(*this);
    emit("jump", loopStartLabel);
    emitLabel(loopEndLabel);
}

void CodeGenerator::visit(ProcedureCallStatementNode& node) {
    const std::string& procName = node.procName->name;
    if (procName == "write" || procName == "writeln") {
        if (node.arguments && !node.arguments->expressions.empty()) {
            for (auto* arg : node.arguments->expressions) {
                arg->accept(*this);
                if (dynamic_cast<StringLiteralNode*>(arg)) emit("writes");
                else if (arg->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER || arg->determinedType == EntryTypeCategory::PRIMITIVE_BOOLEAN) emit("writei");
                else if (arg->determinedType == EntryTypeCategory::PRIMITIVE_REAL) emit("writef");
            }
        }
        if (procName == "writeln") {
            emit("pushs", "\"\n\"");
            emit("writes");
        }
        return;
    }
    // In codegenerator.cpp, inside CodeGenerator::visit(ProcedureCallStatementNode& node)

    // In codegenerator.cpp, inside CodeGenerator::visit(ProcedureCallStatementNode& node)

    if (procName == "read" || procName == "readln") {
        if (node.arguments && !node.arguments->expressions.empty()) {
            for (auto* arg : node.arguments->expressions) {
                // Step 1: Emit VM code to read input from the user and convert it.
                // The result (an integer or real) is left on top of the stack.
                emit("read");
                if (arg->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) {
                    emit("atoi");
                }
                else if (arg->determinedType == EntryTypeCategory::PRIMITIVE_REAL) {
                    emit("atof");
                }

                // Step 2: Determine where to store the converted value from the stack.
                // Check if the argument is an array access (VariableNode with an index).
                if (auto* varNode = dynamic_cast<VariableNode*>(arg)) {
                    SymbolEntry* entry = symbolTable->lookupSymbol(varNode->identifier->name);
                    if (!entry) throw std::runtime_error("CodeGen: Symbol not found: " + varNode->identifier->name);

                    if (varNode->index) {
                        // Argument is an array element, e.g., read(myArray[i])
                        if (varNode->scope == SymbolScope::LOCAL) emit("pushl", std::to_string(entry->offset));
                        else emit("pushg", std::to_string(entry->offset));
                        emit("swap");
                        varNode->index->accept(*this);
                        emit("pushi", std::to_string(entry->arrayDetails.lowBound));
                        emit("sub");
                        emit("storen");
                    }
                    else {
                        // Argument is a simple variable that was parsed as a VariableNode.
                        if (entry->kind == SymbolKind::PARAMETER) emit("storel", std::to_string(-(entry->offset + 1)));
                        else if (varNode->scope == SymbolScope::LOCAL) emit("storel", std::to_string(entry->offset));
                        else emit("storeg", std::to_string(entry->offset));
                    }
                }
                else if (auto* idNode = dynamic_cast<IdExprNode*>(arg)) {
                    // Argument is a simple variable, e.g., read(input_val)
                    SymbolEntry* entry = symbolTable->lookupSymbol(idNode->ident->name);
                    if (!entry) throw std::runtime_error("CodeGen: Symbol not found: " + idNode->ident->name);

                    if (entry->kind == SymbolKind::PARAMETER) {
                        emit("storel", std::to_string(-(entry->offset + 1)));
                    }
                    else if (idNode->scope == SymbolScope::LOCAL) {
                        emit("storel", std::to_string(entry->offset));
                    }
                    else {
                        emit("storeg", std::to_string(entry->offset));
                    }
                }
                else {
                    // This case should not be reached if semantic analysis passed.
                    throw std::runtime_error("CodeGen Error: Argument to 'read' must be a variable.");
                }
            }
        }
        return; // Finished handling read/readln
    }

    if (!node.resolved_entry) {
        throw std::runtime_error("CodeGen Error: Procedure call to '" + procName + "' was not resolved by semantic analyzer.");
    }

    std::string mangledName = node.resolved_entry->getMangledName();
    if (node.arguments) {
        auto& exprs = node.arguments->expressions;
        for (auto it = exprs.rbegin(); it != exprs.rend(); ++it) (*it)->accept(*this);
    }
    emit("pusha", mangledName);
    emit("call");
    if (node.resolved_entry->numParameters > 0) {
        emit("pop", std::to_string(node.resolved_entry->numParameters));
    }
}

void CodeGenerator::visit(FunctionCallExprNode& node) {
    if (!node.resolved_entry) {
        throw std::runtime_error("CodeGen Error: Function call to '" + node.funcName->name + "' was not resolved by semantic analyzer.");
    }
    std::string mangledName = node.resolved_entry->getMangledName();
    emit("pushn", "1");
    if (node.arguments) {
        auto& exprs = node.arguments->expressions;
        for (auto it = exprs.rbegin(); it != exprs.rend(); ++it) (*it)->accept(*this);
    }
    emit("pusha", mangledName);
    emit("call");
    if (node.resolved_entry->numParameters > 0) {
        emit("pop", std::to_string(node.resolved_entry->numParameters));
    }
}

// MODIFIED: Corrected logic to use the new context pointer 'currentSubprogramEntry'
void CodeGenerator::visit(ReturnStatementNode& node) {
    if (node.returnValue) {
        if (!currentSubprogramEntry) {
            throw std::runtime_error("CodeGen: Return statement found with no subprogram context.");
        }

        int num_params = currentSubprogramEntry->numParameters;
        node.returnValue->accept(*this);

        if (currentSubprogramEntry->functionReturnType == EntryTypeCategory::PRIMITIVE_REAL && node.returnValue->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) {
            emit("itof");
        }
        emit("storel", std::to_string(-(num_params + 1)));
    }
    emit("return");
}

void CodeGenerator::visit(IntNumNode& node) { emit("pushi", std::to_string(node.value)); }
void CodeGenerator::visit(RealNumNode& node) { emit("pushf", std::to_string(node.value)); }
void CodeGenerator::visit(BooleanLiteralNode& node) { emit("pushi", node.value ? "1" : "0"); }
void CodeGenerator::visit(StringLiteralNode& node) { emit("pushs", "\"" + node.value + "\""); }

void CodeGenerator::visit(UnaryOpNode& node) {
    node.expression->accept(*this);
    if (node.op == "-") {
        if (node.expression->determinedType == EntryTypeCategory::PRIMITIVE_REAL) {
            emit("pushf", "0.0"); emit("swap"); emit("fsub");
        }
        else {
            emit("pushi", "0"); emit("swap"); emit("sub");
        }
    }
    else if (node.op == "NOT_OP") {
        emit("not");
    }
}

void CodeGenerator::visit(BinaryOpNode& node) {
    bool is_real_op = (node.left->determinedType == EntryTypeCategory::PRIMITIVE_REAL ||
        node.right->determinedType == EntryTypeCategory::PRIMITIVE_REAL ||
        node.op == "/");
    if (node.op == "AND_OP" || node.op == "OR_OP") is_real_op = false;
    node.left->accept(*this);
    if (is_real_op && node.left->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) emit("itof");
    node.right->accept(*this);
    if (is_real_op && node.right->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) emit("itof");

    if (node.op == "+") emit(is_real_op ? "fadd" : "add");
    else if (node.op == "-") emit(is_real_op ? "fsub" : "sub");
    else if (node.op == "*") emit(is_real_op ? "fmul" : "mul");
    else if (node.op == "/") emit("fdiv");
    else if (node.op == "DIV_OP") emit("div");
    else if (node.op == "EQ_OP") emit("equal");
    else if (node.op == "NEQ_OP") { emit("equal"); emit("not"); }
    else if (node.op == "LT_OP") emit(is_real_op ? "finf" : "inf");
    else if (node.op == "LTE_OP") emit(is_real_op ? "finfeq" : "infeq");
    else if (node.op == "GT_OP") emit(is_real_op ? "fsup" : "sup");
    else if (node.op == "GTE_OP") emit(is_real_op ? "fsupeq" : "supeq");
    else if (node.op == "AND_OP") emit("mul");
    else if (node.op == "OR_OP") { emit("add"); emit("pushi", "0"); emit("sup"); }
    else throw std::runtime_error("CodeGen: Unsupported binary op '" + node.op + "'");
}

EntryTypeCategory CodeGenerator::astToSymbolType(TypeNode* astTypeNode, ArrayDetails& outArrayDetails) {
    outArrayDetails.isInitialized = false;
    if (!astTypeNode) return EntryTypeCategory::UNKNOWN_TYPE;
    if (auto* stn = dynamic_cast<StandardTypeNode*>(astTypeNode)) {
        switch (stn->category) {
        case StandardTypeNode::TYPE_INTEGER: return EntryTypeCategory::PRIMITIVE_INTEGER;
        case StandardTypeNode::TYPE_REAL: return EntryTypeCategory::PRIMITIVE_REAL;
        case StandardTypeNode::TYPE_BOOLEAN: return EntryTypeCategory::PRIMITIVE_BOOLEAN;
        default: return EntryTypeCategory::UNKNOWN_TYPE;
        }
    }
    else if (auto* atn = dynamic_cast<ArrayTypeNode*>(astTypeNode)) {
        if (atn->elementType) {
            switch (atn->elementType->category) {
            case StandardTypeNode::TYPE_INTEGER: outArrayDetails.elementType = EntryTypeCategory::PRIMITIVE_INTEGER; break;
            case StandardTypeNode::TYPE_REAL: outArrayDetails.elementType = EntryTypeCategory::PRIMITIVE_REAL; break;
            case StandardTypeNode::TYPE_BOOLEAN: outArrayDetails.elementType = EntryTypeCategory::PRIMITIVE_BOOLEAN; break;
            default: outArrayDetails.elementType = EntryTypeCategory::UNKNOWN_TYPE; break;
            }
        }
        if (atn->startIndex && atn->endIndex) {
            outArrayDetails.lowBound = atn->startIndex->value;
            outArrayDetails.highBound = atn->endIndex->value;
            outArrayDetails.isInitialized = true;
        }
        return EntryTypeCategory::ARRAY;
    }
    return EntryTypeCategory::UNKNOWN_TYPE;
}