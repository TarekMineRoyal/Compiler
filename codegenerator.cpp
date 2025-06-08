#include "codegenerator.h"
#include <stdexcept>
#include <iostream>

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
    emit("START");
    if (node.subprogs && !node.subprogs->subprograms.empty()) {
        emit("JUMP", "main_entry");
    }

    if (node.subprogs) node.subprogs->accept(*this);

    emitLabel("main_entry");

    if (node.decls) {
        node.decls->accept(*this);
    }

    if (node.mainCompoundStmt) node.mainCompoundStmt->accept(*this);

    emit("STOP");
}

void CodeGenerator::visit(Declarations& node) {
    int varCount = 0;
    if (!node.var_decl_items.empty()) {
        for (auto* decl : node.var_decl_items) {
            varCount += decl->identifiers->identifiers.size();
        }
        if (varCount > 0) {
            emit("PUSHN", std::to_string(varCount));
        }
    }

    for (auto* varDecl : node.var_decl_items) {
        varDecl->accept(*this);
    }
}

void CodeGenerator::visit(VarDecl& node) {
    if (auto* arrayType = dynamic_cast<ArrayTypeNode*>(node.type)) {
        int low = arrayType->startIndex->value;
        int high = arrayType->endIndex->value;
        int size = high - low + 1;

        if (size <= 0) {
            throw std::runtime_error("Array size must be positive.");
        }

        for (auto* ident : node.identifiers->identifiers) {
            SymbolEntry* entry = symbolTable->lookupSymbol(ident->name);
            if (!entry) {
                throw std::runtime_error("CodeGen: Symbol not found during array allocation: " + ident->name);
            }

            emit("ALLOC", std::to_string(size));

            if (symbolTable->isGlobalScope()) { // This check might need refinement for local arrays
                emit("STOREG", std::to_string(entry->offset));
            }
            else {
                emit("STOREL", std::to_string(entry->offset));
            }
        }
    }
}

void CodeGenerator::visit(SubprogramDeclarations& node) {
    for (auto* subprog : node.subprograms) {
        if (subprog) subprog->accept(*this);
    }
}

void CodeGenerator::visit(SubprogramDeclaration& node) {
    FunctionHeadNode* previousContext = currentFunctionContext;
    if (auto* funcHead = dynamic_cast<FunctionHeadNode*>(node.head)) {
        currentFunctionContext = funcHead;
    }
    else {
        currentFunctionContext = nullptr;
    }

    std::string subprogramLabel = node.head->name->name;
    emitLabel(subprogramLabel);

    if (node.local_declarations) {
        node.local_declarations->accept(*this);
    }

    if (node.body) node.body->accept(*this);

    emit("RETURN");

    currentFunctionContext = previousContext;
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
    if (node.variable->index) { // Array assignment: a[i] := value
        if (auto* index_lit = dynamic_cast<IntNumNode*>(node.variable->index)) {
            // --- OPTIMIZED CASE: Index is a literal ---
            // Push array address, then value, then use 'store <literal>'
            if (node.variable->scope == SymbolScope::LOCAL) {
                emit("PUSHL", std::to_string(node.variable->offset));
            }
            else {
                emit("PUSHG", std::to_string(node.variable->offset));
            }
            node.expression->accept(*this);
            emit("STORE", std::to_string(index_lit->value));

        }
        else {
            // --- GENERAL CASE: Index is an expression ---
            // Push array address, then index value, then value, then use 'storen'
            if (node.variable->scope == SymbolScope::LOCAL) {
                emit("PUSHL", std::to_string(node.variable->offset));
            }
            else {
                emit("PUSHG", std::to_string(node.variable->offset));
            }
            node.variable->index->accept(*this);
            node.expression->accept(*this);
            emit("STOREN");
        }
    }
    else { // Simple variable assignment
        node.expression->accept(*this);
        if (node.variable->determinedType == EntryTypeCategory::PRIMITIVE_REAL && node.expression->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) {
            emit("ITOF");
        }
        if (node.variable->scope == SymbolScope::LOCAL) {
            emit("STOREL", std::to_string(node.variable->offset));
        }
        else {
            emit("STOREG", std::to_string(node.variable->offset));
        }
    }
}


void CodeGenerator::visit(VariableNode& node) {
    if (node.index) { // Array access: e.g., in write(a[i])
        if (auto* index_lit = dynamic_cast<IntNumNode*>(node.index)) {
            // --- OPTIMIZED CASE: Index is a literal ---
            if (node.scope == SymbolScope::LOCAL) {
                emit("PUSHL", std::to_string(node.offset));
            }
            else {
                emit("PUSHG", std::to_string(node.offset));
            }
            emit("LOAD", std::to_string(index_lit->value));
        }
        else {
            // --- GENERAL CASE: Index is an expression ---
            if (node.scope == SymbolScope::LOCAL) {
                emit("PUSHL", std::to_string(node.offset));
            }
            else {
                emit("PUSHG", std::to_string(node.offset));
            }
            node.index->accept(*this);
            emit("LOADN");
        }
    }
    else { // Simple variable access
        if (node.scope == SymbolScope::LOCAL) {
            emit("PUSHL", std::to_string(node.offset));
        }
        else {
            emit("PUSHG", std::to_string(node.offset));
        }
    }
}

void CodeGenerator::visit(IfStatementNode& node) {
    std::string elseLabel = newLabel("ELSE");
    std::string endIfLabel = newLabel("END_IF");

    node.condition->accept(*this);
    emit("JZ", elseLabel);

    node.thenStatement->accept(*this);
    if (node.elseStatement) {
        emit("JUMP", endIfLabel);
    }

    emitLabel(elseLabel);
    if (node.elseStatement) {
        node.elseStatement->accept(*this);
    }

    emitLabel(endIfLabel);
}

void CodeGenerator::visit(WhileStatementNode& node) {
    std::string loopStartLabel = newLabel("WHILE_START");
    std::string loopEndLabel = newLabel("WHILE_END");

    emitLabel(loopStartLabel);
    node.condition->accept(*this);
    emit("JZ", loopEndLabel);

    node.body->accept(*this);
    emit("JUMP", loopStartLabel);

    emitLabel(loopEndLabel);
}

void CodeGenerator::visit(ProcedureCallStatementNode& node) {
    const std::string& procName = node.procName->name;

    if (procName == "write" || procName == "writeln") {
        if (node.arguments && !node.arguments->expressions.empty()) {
            auto& expressions = node.arguments->expressions;
            bool isFirst = true;

            for (auto* arg : expressions) {
                arg->accept(*this);

                if (arg->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER || arg->determinedType == EntryTypeCategory::PRIMITIVE_BOOLEAN) {
                    emit("STRI");
                }
                else if (arg->determinedType == EntryTypeCategory::PRIMITIVE_REAL) {
                    emit("STRF");
                }

                if (!isFirst) {
                    emit("CONCAT");
                }
                isFirst = false;
            }
            emit("WRITES");
        }

        if (procName == "writeln") {
            emit("PUSHS", "\"\n\"");
            emit("WRITES");
        }
        return;
    }

    SymbolEntry* entry = symbolTable->lookupSymbol(procName);
    if (!entry) throw std::runtime_error("CodeGen: Procedure '" + procName + "' not found in symbol table.");

    if (node.arguments) {
        for (auto* expr : node.arguments->expressions) {
            expr->accept(*this);
        }
    }

    emit("PUSHA", procName);
    emit("CALL");

    if (entry->numParameters > 0) {
        emit("POP", std::to_string(entry->numParameters));
    }
}

void CodeGenerator::visit(FunctionCallExprNode& node) {
    const std::string& funcName = node.funcName->name;
    SymbolEntry* entry = symbolTable->lookupSymbol(funcName);
    if (!entry) throw std::runtime_error("CodeGen: Function '" + funcName + "' not found.");

    emit("PUSHN", "1");

    if (node.arguments) {
        for (auto* expr : node.arguments->expressions) {
            expr->accept(*this);
        }
    }

    emit("PUSHA", funcName);
    emit("CALL");

    if (entry->numParameters > 0) {
        emit("POP", std::to_string(entry->numParameters));
    }
}

void CodeGenerator::visit(ReturnStatementNode& node) {
    if (node.returnValue) {
        if (!currentFunctionContext) {
            throw std::runtime_error("CodeGen: Return statement found outside of a function.");
        }
        SymbolEntry* funcEntry = symbolTable->lookupSymbol(currentFunctionContext->name->name);
        if (!funcEntry) {
            throw std::runtime_error("CodeGen: Could not find function entry for return.");
        }
        int num_params = funcEntry->numParameters;

        emit("PUSHFP");
        emit("PUSHI", std::to_string(num_params + 1));
        emit("SUB");

        node.returnValue->accept(*this);

        emit("STORE");
    }
}

void CodeGenerator::visit(IdExprNode& node) {
    if (node.scope == SymbolScope::LOCAL) {
        emit("PUSHL", std::to_string(node.offset));
    }
    else {
        emit("PUSHG", std::to_string(node.offset));
    }
}

void CodeGenerator::visit(IntNumNode& node) { emit("PUSHI", std::to_string(node.value)); }
void CodeGenerator::visit(RealNumNode& node) { emit("PUSHF", std::to_string(node.value)); }
void CodeGenerator::visit(BooleanLiteralNode& node) { emit("PUSHI", node.value ? "1" : "0"); }
void CodeGenerator::visit(StringLiteralNode& node) { emit("PUSHS", "\"" + node.value + "\""); }

void CodeGenerator::visit(UnaryOpNode& node) {
    node.expression->accept(*this);
    if (node.op == "-") {
        if (node.expression->determinedType == EntryTypeCategory::PRIMITIVE_REAL) {
            emit("PUSHF", "0.0");
            emit("SWAP");
            emit("FSUB");
        }
        else {
            emit("PUSHI", "0");
            emit("SWAP");
            emit("SUB");
        }
    }
    else if (node.op == "NOT_OP") {
        emit("NOT");
    }
}

void CodeGenerator::visit(BinaryOpNode& node) {
    bool is_real_op = (node.left->determinedType == EntryTypeCategory::PRIMITIVE_REAL ||
        node.right->determinedType == EntryTypeCategory::PRIMITIVE_REAL ||
        node.op == "/");

    if (node.op == "AND_OP" || node.op == "OR_OP") {
        is_real_op = false;
    }

    node.left->accept(*this);
    if (is_real_op && node.left->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) {
        emit("ITOF");
    }

    node.right->accept(*this);
    if (is_real_op && node.right->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) {
        emit("ITOF");
    }

    if (node.op == "+") { emit(is_real_op ? "FADD" : "ADD"); }
    else if (node.op == "-") { emit(is_real_op ? "FSUB" : "SUB"); }
    else if (node.op == "*") { emit(is_real_op ? "FMUL" : "MUL"); }
    else if (node.op == "/") { emit("FDIV"); }
    else if (node.op == "DIV_OP") { emit("DIV"); }
    else if (node.op == "EQ_OP") { emit("EQUAL"); }
    else if (node.op == "NEQ_OP") { emit("EQUAL"); emit("NOT"); }
    else if (node.op == "LT_OP") { emit(is_real_op ? "FINF" : "INF"); }
    else if (node.op == "LTE_OP") { emit(is_real_op ? "FINFEQ" : "INFEQ"); }
    else if (node.op == "GT_OP") { emit(is_real_op ? "FSUP" : "FSUP"); }
    else if (node.op == "GTE_OP") { emit(is_real_op ? "FSUPEQ" : "SUPEQ"); }
    else if (node.op == "AND_OP") { emit("MUL"); }
    else if (node.op == "OR_OP") {
        emit("ADD");
        emit("PUSHI", "0");
        emit("SUP");
    }
    else {
        throw std::runtime_error("CodeGen: Unsupported binary operator '" + node.op + "'");
    }
}
