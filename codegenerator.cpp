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
    int varCount = 0;
    if (symbolTable->isGlobalScope()) {
        if (!node.var_decl_items.empty()) {
            for (auto* decl : node.var_decl_items) {
                if (dynamic_cast<ArrayTypeNode*>(decl->type)) continue;
                varCount += decl->identifiers->identifiers.size();
            }
            if (varCount > 0) {
                emit("pushn", std::to_string(varCount));
            }
        }
    }
    for (auto* varDecl : node.var_decl_items) {
        varDecl->accept(*this);
    }
}

void CodeGenerator::visit(VarDecl& node) {
    if (!symbolTable->isGlobalScope()) {
        ArrayDetails ad;
        EntryTypeCategory var_type = astToSymbolType(node.type, ad);
        int varCount = 0;
        for (auto* ident : node.identifiers->identifiers) {
            SymbolEntry entry(ident->name, SymbolKind::VARIABLE, var_type, ident->line, ident->column);
            entry.offset = local_offset++;
            if (var_type == EntryTypeCategory::ARRAY) {
                entry.arrayDetails = ad;
            }
            else {
                varCount++;
            }
            symbolTable->addSymbol(entry);
        }
        if (varCount > 0) {
            emit("pushn", std::to_string(varCount));
        }
    }
    if (auto* arrayType = dynamic_cast<ArrayTypeNode*>(node.type)) {
        int low = arrayType->startIndex->value;
        int high = arrayType->endIndex->value;
        int size = high - low + 1;
        if (size <= 0) {
            throw std::runtime_error("Array size must be positive.");
        }
        for (auto* ident : node.identifiers->identifiers) {
            SymbolEntry* entry = symbolTable->lookupSymbol(ident->name);
            if (!entry) throw std::runtime_error("CodeGen: Symbol not found during array allocation: " + ident->name);
            emit("alloc", std::to_string(size));
            if (symbolTable->isGlobalScope()) {
                emit("storeg", std::to_string(entry->offset));
            }
            else {
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

void CodeGenerator::visit(SubprogramDeclaration& node) {
    SubprogramHead* previousContext = currentFunctionContext;
    currentFunctionContext = node.head;

    std::string subprogramName = node.head->name->name;
    std::string endLabel = subprogramName + "_end";

    emit("jump", endLabel);
    emitLabel(subprogramName);

    symbolTable->enterScope();
    local_offset = 0;
    param_offset = 0;

    if (node.head->arguments) node.head->arguments->accept(*this);
    if (node.local_declarations) node.local_declarations->accept(*this);
    if (node.body) node.body->accept(*this);

    // FINAL CORRECTED LOGIC: Only add an implicit return for procedures.
    // Functions MUST have an explicit RETURN statement, which generates its own 'return'.
    if (dynamic_cast<ProcedureHeadNode*>(node.head)) {
        emit("return");
    }

    emitLabel(endLabel);

    symbolTable->exitScope();
    currentFunctionContext = previousContext;
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
        if (varNode->index) {
            SymbolEntry* arrayEntry = symbolTable->lookupSymbol(varNode->identifier->name);
            if (!arrayEntry) throw std::runtime_error("CodeGen: Array symbol not found: " + varNode->identifier->name);
            int lowerBound = arrayEntry->arrayDetails.lowBound;

            if (varNode->scope == SymbolScope::LOCAL) {
                emit("pushl", std::to_string(varNode->offset));
            }
            else {
                emit("pushg", std::to_string(varNode->offset));
            }

            if (auto* index_lit = dynamic_cast<IntNumNode*>(varNode->index)) {
                node.expression->accept(*this);
                emit("store", std::to_string(index_lit->value - lowerBound));
            }
            else {
                varNode->index->accept(*this);
                emit("pushi", std::to_string(lowerBound));
                emit("sub");
                node.expression->accept(*this);
                emit("storen");
            }
        }
        else {
            node.expression->accept(*this);
            if (varNode->determinedType == EntryTypeCategory::PRIMITIVE_REAL && node.expression->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) {
                emit("itof");
            }
            SymbolEntry* entry = symbolTable->lookupSymbol(varNode->identifier->name);
            if (!entry) throw std::runtime_error("CodeGen: Symbol not found in assignment: " + varNode->identifier->name);
            if (entry->kind == SymbolKind::PARAMETER) {
                // CORRECTED: Parameter access offset formula
                int negative_offset = -(entry->offset + 1);
                emit("storel", std::to_string(negative_offset));
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
    if (!entry) throw std::runtime_error("CodeGen: Symbol not found: " + node.identifier->name);
    if (entry->kind == SymbolKind::PARAMETER) {
        // CORRECTED: Parameter access offset formula
        int negative_offset = -(entry->offset + 1);
        emit("pushl", std::to_string(negative_offset));
        return;
    }
    if (node.index) {
        if (!entry->arrayDetails.isInitialized) throw std::runtime_error("CodeGen: Array details not found for " + node.identifier->name);
        int lowerBound = entry->arrayDetails.lowBound;
        if (node.scope == SymbolScope::LOCAL) emit("pushl", std::to_string(entry->offset));
        else emit("pushg", std::to_string(entry->offset));
        if (auto* index_lit = dynamic_cast<IntNumNode*>(node.index)) {
            emit("load", std::to_string(index_lit->value - lowerBound));
        }
        else {
            node.index->accept(*this);
            emit("pushi", std::to_string(lowerBound));
            emit("sub");
            emit("loadn");
        }
    }
    else {
        if (node.scope == SymbolScope::LOCAL) emit("pushl", std::to_string(entry->offset));
        else emit("pushg", std::to_string(entry->offset));
    }
}

void CodeGenerator::visit(IdExprNode& node) {
    SymbolEntry* entry = symbolTable->lookupSymbol(node.ident->name);
    if (!entry) throw std::runtime_error("CodeGen: Symbol not found: " + node.ident->name);
    if (entry->kind == SymbolKind::PARAMETER) {
        // CORRECTED: Parameter access offset formula
        int negative_offset = -(entry->offset + 1);
        emit("pushl", std::to_string(negative_offset));
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
    SymbolEntry* entry = symbolTable->lookupSymbol(procName);
    if (!entry) throw std::runtime_error("CodeGen: Procedure not found: " + procName);
    if (node.arguments) {
        auto& exprs = node.arguments->expressions;
        for (auto it = exprs.rbegin(); it != exprs.rend(); ++it) (*it)->accept(*this);
    }
    emit("pusha", procName);
    emit("call");
    if (entry->numParameters > 0) emit("pop", std::to_string(entry->numParameters));
}

void CodeGenerator::visit(FunctionCallExprNode& node) {
    const std::string& funcName = node.funcName->name;
    SymbolEntry* entry = symbolTable->lookupSymbol(funcName);
    if (!entry) throw std::runtime_error("CodeGen: Function not found: " + funcName);
    emit("pushn", "1");
    if (node.arguments) {
        auto& exprs = node.arguments->expressions;
        for (auto it = exprs.rbegin(); it != exprs.rend(); ++it) (*it)->accept(*this);
    }
    emit("pusha", funcName);
    emit("call");
    if (entry->numParameters > 0) emit("pop", std::to_string(entry->numParameters));
}

void CodeGenerator::visit(ReturnStatementNode& node) {
    if (node.returnValue) {
        if (!currentFunctionContext) throw std::runtime_error("CodeGen: Return statement outside function.");
        SymbolEntry* funcEntry = symbolTable->lookupSymbol(currentFunctionContext->name->name);
        if (!funcEntry) throw std::runtime_error("CodeGen: Could not find function entry for return.");
        int num_params = funcEntry->numParameters;
        node.returnValue->accept(*this);
        if (funcEntry->functionReturnType == EntryTypeCategory::PRIMITIVE_REAL && node.returnValue->determinedType == EntryTypeCategory::PRIMITIVE_INTEGER) {
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
    else throw std::runtime_error("CodeGen: Unsupported binary operator '" + node.op + "'");
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
