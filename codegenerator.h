#ifndef CODEGENERATOR_H
#define CODEGENERATOR_H

#include "ast.h"
#include "semantic_analyzer.h" 
#include "symbol_table.h" 
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
    SubprogramHead* currentFunctionContext = nullptr;
    // ADDED: A pointer to the symbol entry for the current subprogram being generated.
    SymbolEntry* currentSubprogramEntry = nullptr;

    int local_offset = 0;
    int param_offset = 0;

    std::string currentSubprogramExitLabel;

    // Helper Methods
    std::string newLabel(const std::string& prefix);
    void emit(const std::string& instruction);
    void emit(const std::string& instruction, const std::string& arg);
    void emitLabel(const std::string& label);
    EntryTypeCategory astToSymbolType(TypeNode* astTypeNode, ArrayDetails& outArrayDetails);

    // Visitor Method Overrides
    void visit(ProgramNode& node) override;
    void visit(Declarations& node) override;
    void visit(VarDecl& node) override;
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
    void visit(ArgumentsNode& node) override;
    void visit(ParameterList& node) override;
    void visit(ParameterDeclaration& node) override;

    // Unused or trivial visitor methods
    void visit(IdentifierList& node) override {}
    void visit(IdentNode& node) override {}
    void visit(StandardTypeNode& node) override {}
    void visit(ArrayTypeNode& node) override {}
    void visit(FunctionHeadNode& node) override {}
    void visit(ProcedureHeadNode& node) override {}
    void visit(ExpressionList& node) override {}
};

#endif // CODEGENERATOR_H