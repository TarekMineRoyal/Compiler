#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "semantic_visitor.h"
#include "symbol_table.h" 
#include "ast.h"          
#include <vector>
#include <string>
#include <iostream>

class SemanticAnalyzer : public SemanticVisitor {
private:
    SymbolTable symbolTable;
    FunctionHeadNode* currentFunctionContext;
    SymbolEntry* currentSubprogramEntry = nullptr;
    std::vector<std::string> semanticErrors;
    int global_offset = 0;
    int local_offset = 0;
    int param_offset = 0;

    void recordError(const std::string& message, int line, int col);
    EntryTypeCategory astToSymbolType(TypeNode* astTypeNode, ArrayDetails& outArrayDetails);
    EntryTypeCategory astStandardTypeToSymbolType(StandardTypeNode* astStandardTypeNode);

    bool isPrintableType(EntryTypeCategory type, ExprNode* argNode);
    bool isReadableType(EntryTypeCategory type);

    std::string buildMangledName(const std::string& name, SymbolKind kind, ExpressionList* args);

public:
    SemanticAnalyzer();
    SymbolTable& getSymbolTable() { return symbolTable; }

    // Visitor overrides
    void visit(ProgramNode& node) override;
    void visit(IdentifierList& node) override;
    void visit(IdentNode& node) override;
    void visit(Declarations& node) override;
    void visit(VarDecl& node) override;
    void visit(StandardTypeNode& node) override;
    void visit(ArrayTypeNode& node) override;
    void visit(SubprogramDeclarations& node) override;
    void visit(SubprogramDeclaration& node) override;
    void visit(FunctionHeadNode& node) override;
    void visit(ProcedureHeadNode& node) override;
    void visit(ArgumentsNode& node) override;
    void visit(ParameterList& node) override;
    void visit(ParameterDeclaration& node) override;
    void visit(CompoundStatementNode& node) override;
    void visit(StatementList& node) override;
    void visit(AssignStatementNode& node) override;
    void visit(IfStatementNode& node) override;
    void visit(WhileStatementNode& node) override;
    void visit(VariableNode& node) override;
    void visit(ProcedureCallStatementNode& node) override;
    void visit(ExpressionList& node) override;
    void visit(IntNumNode& node) override;
    void visit(RealNumNode& node) override;
    void visit(BooleanLiteralNode& node) override;
    void visit(StringLiteralNode& node) override;
    void visit(BinaryOpNode& node) override;
    void visit(UnaryOpNode& node) override;
    void visit(IdExprNode& node) override;
    void visit(FunctionCallExprNode& node) override;
    void visit(ReturnStatementNode& node) override;

    bool hasErrors() const;
    void printErrors(std::ostream& out) const;
};

#endif // SEMANTIC_ANALYZER_H