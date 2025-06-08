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
    // Allocate space for global variables
    if (node.decls) {
        int globalVarCount = 0;
        for (auto* decl : node.decls->var_decl_items) {
            globalVarCount += decl->identifiers->identifiers.size();
        }
        if (globalVarCount > 0) {
            emit("PUSHN", std::to_string(globalVarCount));
        }
    }

    if (node.mainCompoundStmt) node.mainCompoundStmt->accept(*this);

    emit("STOP");
}

void CodeGenerator::visit(SubprogramDeclarations& node) {
    for (auto* subprog : node.subprograms) {
        if (subprog) subprog->accept(*this);
    }
}

void CodeGenerator::visit(SubprogramDeclaration& node) {
    // --- FIX ---
    // Set the current function context before compiling the body
    FunctionHeadNode* previousContext = currentFunctionContext;
    if (auto* funcHead = dynamic_cast<FunctionHeadNode*>(node.head)) {
        currentFunctionContext = funcHead;
    }
    else {
        currentFunctionContext = nullptr; // It's a procedure
    }

    std::string subprogramLabel = node.head->name->name;
    emitLabel(subprogramLabel);

    // Prologue: allocate space for local variables
    int localVarCount = 0;
    if (node.local_declarations) {
        for (auto* decl : node.local_declarations->var_decl_items) {
            localVarCount += decl->identifiers->identifiers.size();
        }
    }
    if (localVarCount > 0) {
        emit("PUSHN", std::to_string(localVarCount));
    }

    if (node.body) node.body->accept(*this);

    // Epilogue
    emit("RETURN");

    // --- FIX ---
    // Restore the previous function context
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
    if (node.variable->index) {
        if (node.variable->scope == SymbolScope::LOCAL) {
            emit("PUSHFP");
            emit("PUSHI", std::to_string(node.variable->offset));
            emit("ADD");
        }
        else {
            emit("PUSHGP");
            emit("PUSHI", std::to_string(node.variable->offset));
            emit("ADD");
        }
        node.variable->index->accept(*this);
        node.expression->accept(*this);
        emit("STOREN");
    }
    else {
        node.expression->accept(*this);
        if (node.variable->scope == SymbolScope::LOCAL) {
            emit("STOREL", std::to_string(node.variable->offset));
        }
        else {
            emit("STOREG", std::to_string(node.variable->offset));
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
            for (auto* arg : node.arguments->expressions) {
                arg->accept(*this);
                if (dynamic_cast<StringLiteralNode*>(arg) || arg->determinedType == EntryTypeCategory::ARRAY) { // Array of char not supported, assume string
                    emit("WRITES");
                }
                else if (arg->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER || arg->determinedType == EntryTypeCategory::PRIMITIVE_BOOLEAN) {
                    emit("WRITEI");
                }
                else if (arg->determinedType == EntryTypeCategory::PRIMITIVE_REAL) {
                    emit("WRITEF");
                }
                emit("PUSHS", "\" \""); // Separator for multiple args
                emit("WRITES");
            }
        }
        if (procName == "writeln") {
            emit("PUSHS", "\"\\n\"");
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
        // --- FIX ---
        // This check now works because currentFunctionContext is a valid member
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

void CodeGenerator::visit(VariableNode& node) {
    if (node.index) {
        if (node.scope == SymbolScope::LOCAL) {
            emit("PUSHFP");
            emit("PUSHI", std::to_string(node.offset));
            emit("ADD");
        }
        else {
            emit("PUSHGP");
            emit("PUSHI", std::to_string(node.offset));
            emit("ADD");
        }
        node.index->accept(*this);
        emit("LOADN");
    }
    else {
        if (node.scope == SymbolScope::LOCAL) {
            emit("PUSHL", std::to_string(node.offset));
        }
        else {
            emit("PUSHG", std::to_string(node.offset));
        }
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
    node.left->accept(*this);
    node.right->accept(*this);

    bool is_real = (node.determinedType == EntryTypeCategory::PRIMITIVE_REAL);

    const std::map<std::string, std::pair<std::string, std::string>> op_map = {
        {"+", {"ADD", "FADD"}},
        {"-", {"SUB", "FSUB"}},
        {"*", {"MUL", "FMUL"}},
        {"/", {"FDIV", "FDIV"}},
        {"DIV_OP", {"DIV", "DIV"}},
        {"EQ_OP", {"EQUAL", "EQUAL"}},
        {"NEQ_OP", {"EQUAL", "EQUAL"}},
        {"LT_OP", {"INF", "FINF"}},
        {"LTE_OP", {"INFEQ", "FINFEQ"}},
        {"GT_OP", {"SUP", "FSUP"}},
        {"GTE_OP", {"SUPEQ", "FSUPEQ"}},
        {"AND_OP", {"ADD", "ADD"}},
        {"OR_OP", {"ADD", "ADD"}}
    };

    auto it = op_map.find(node.op);
    if (it != op_map.end()) {
        emit(is_real ? it->second.second : it->second.first);
        if (node.op == "NEQ_OP") {
            emit("NOT");
        }
        if (node.op == "AND_OP") {
            // A and B -> if A+B == 2 then 1 else 0
            emit("PUSHI", "2");
            emit("EQUAL");
        }
        if (node.op == "OR_OP") {
            // A or B -> if A+B > 0 then 1 else 0
            emit("PUSHI", "0");
            emit("SUP");
        }
    }
    else {
        throw std::runtime_error("Unsupported binary operator in CodeGen: " + node.op);
    }
}