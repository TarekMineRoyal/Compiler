#ifndef SEMANTIC_ANALYZER_H
#define SEMANTIC_ANALYZER_H

#include "semantic_visitor.h"
#include "symbol_table.h" 
#include "ast.h"          // For FunctionHeadNode pointer and other AST Node definitions
#include <vector>
#include <string>
#include <iostream>       // For std::ostream in printErrors

class SemanticAnalyzer : public SemanticVisitor {
private:
    SymbolTable symbolTable;
    FunctionHeadNode* currentFunctionContext; 
    std::vector<std::string> semanticErrors;  

    void recordError(const std::string& message, int line, int col);
    EntryTypeCategory astToSymbolType(TypeNode* astTypeNode, ArrayDetails& outArrayDetails);
    EntryTypeCategory astStandardTypeToSymbolType(StandardTypeNode* astStandardTypeNode);

    // Helper for checking if a type is printable for write/writeln
    bool isPrintableType(EntryTypeCategory type, ExprNode* argNode); // Added argNode to check for StringLiteralNode
    // Helper for checking if a type is readable for read/readln
    bool isReadableType(EntryTypeCategory type);


public:
    SemanticAnalyzer();

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
    void visit(ProcedureCallStatementNode& node) override; // Will have specific logic for built-ins
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
