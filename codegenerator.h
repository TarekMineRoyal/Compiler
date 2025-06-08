#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "ast.h"
#include "semantic_analyzer.h" 
#include <string>
#include <vector>
#include <sstream>
#include <map>

class CodeGenerator : public SemanticVisitor {
public:
    std::string generateCode(ProgramNode& ast_root, SemanticAnalyzer& semanticAnalyzer);

private:
    std::stringstream code;
    int labelCounter = 0;
    SymbolTable* symbolTable = nullptr;

    // --- FIX ---
    // Added this member to track the current function being compiled,
    // which is needed for handling RETURN statements correctly.
    FunctionHeadNode* currentFunctionContext = nullptr;

    // Helper Methods
    std::string newLabel(const std::string& prefix);
    void emit(const std::string& instruction);
    void emit(const std::string& instruction, const std::string& arg);
    void emitLabel(const std::string& label);

    // Visitor Method Overrides
    void visit(ProgramNode& node) override;
    void visit(SubprogramDeclarations& node) override;
    void visit(SubprogramDeclaration& node) override;
    void visit(CompoundStatementNode& node) override;
    void visit(StatementList& node) override;
    void visit(AssignStatementNode& node) override;
    void visit(IfStatementNode& node) override;
    void visit(WhileStatementNode& node) override;
    void visit(ProcedureCallStatementNode& node) override;
    void visit(ReturnStatementNode& node) override;
    void visit(IntNumNode& node) override;
    void visit(RealNumNode& node) override;
    void visit(BooleanLiteralNode& node) override;
    void visit(StringLiteralNode& node) override;
    void visit(BinaryOpNode& node) override;
    void visit(UnaryOpNode& node) override;
    void visit(VariableNode& node) override;
    void visit(FunctionCallExprNode& node) override;
    void visit(IdExprNode& node) override;

    // Unused or trivial visitor methods
    void visit(Declarations& node) override {}
    void visit(VarDecl& node) override {}
    void visit(IdentifierList& node) override {}
    void visit(IdentNode& node) override {}
    void visit(StandardTypeNode& node) override {}
    void visit(ArrayTypeNode& node) override {}
    void visit(FunctionHeadNode& node) override {}
    void visit(ProcedureHeadNode& node) override {}
    void visit(ArgumentsNode& node) override {}
    void visit(ParameterList& node) override {}
    void visit(ParameterDeclaration& node) override {}
    void visit(ExpressionList& node) override {}
};

#endif // CODEGENERATOR_H