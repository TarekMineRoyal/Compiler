// ast.h
#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <list>
#include <iosfwd>

#include "semantic_visitor.h"
#include "semantic_types.h"

// --- Base Node Class ---
class Node {
public:
    int line;
    int column;
    Node* father;
    Node(int l, int c);
    virtual ~Node() {}
    virtual void print(std::ostream& out, int indentLevel = 0) const = 0;
    virtual void accept(SemanticVisitor& visitor) = 0;
};

// --- Lexer Helper Original Classes ---
class Expr : public Node {
public:
    Expr(int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class Ident : public Node {
public:
    std::string name;
    Ident(const std::string& n, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class Num : public Expr {
public:
    int value;
    Num(int val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class RealLit : public Expr {
public:
    double value;
    RealLit(double val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};


// --- BASE AST NODE CLASSES ---
class ExprNode : public Node {
public:
    // Semantic information determined by the analyzer
    EntryTypeCategory determinedType;
    ArrayDetails determinedArrayDetails; // Valid if determinedType is ARRAY

    ExprNode(int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class StatementNode : public Node {
public:
    StatementNode(int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class TypeNode : public Node {
public:
    TypeNode(int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

// --- SPECIFIC AST NODE DECLARATIONS ---
class IdentNode : public ExprNode {
public:
    std::string name;
    IdentNode(const std::string& n, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class IntNumNode : public ExprNode {
public:
    int value;
    IntNumNode(int val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class RealNumNode : public ExprNode {
public:
    double value;
    RealNumNode(double val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class BooleanLiteralNode : public ExprNode {
public:
    bool value;
    BooleanLiteralNode(bool val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class StringLiteralNode : public ExprNode {
public:
    std::string value;
    StringLiteralNode(const char* val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class IdentifierList : public Node {
public:
    std::list<IdentNode*> identifiers;
    IdentifierList(IdentNode* firstIdent, int l, int c);
    void addIdentifier(IdentNode* ident);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class StandardTypeNode : public TypeNode {
public:
    enum TypeCategory { TYPE_INTEGER, TYPE_REAL, TYPE_BOOLEAN };
    TypeCategory category;
    StandardTypeNode(TypeCategory cat, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class ArrayTypeNode : public TypeNode {
public:
    IntNumNode* startIndex;
    IntNumNode* endIndex;
    StandardTypeNode* elementType;
    ArrayTypeNode(IntNumNode* start, IntNumNode* end, StandardTypeNode* elemType, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class VarDecl : public StatementNode {
public:
    IdentifierList* identifiers;
    TypeNode* type;
    VarDecl(IdentifierList* ids, TypeNode* t, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class Declarations : public Node {
public:
    std::list<VarDecl*> var_decl_items;
    Declarations(int l, int c);
    void addVarDecl(VarDecl* vd);
    void print(std::ostream& out, int indentLevel = 0) const override;
    bool isEmpty() const;
    void accept(SemanticVisitor& visitor) override;
};

class ExpressionList : public Node {
public:
    std::list<ExprNode*> expressions;
    ExpressionList(int l, int c);
    ExpressionList(ExprNode* firstExpr, int l, int c);
    void addExpression(ExprNode* expr);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class ParameterDeclaration : public Node {
public:
    IdentifierList* ids;
    TypeNode* type;
    ParameterDeclaration(IdentifierList* idList, TypeNode* t, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class ParameterList : public Node {
public:
    std::list<ParameterDeclaration*> paramDeclarations;
    ParameterList(ParameterDeclaration* firstParamDecl, int l, int c);
    void addParameterDeclarationGroup(ParameterDeclaration* paramDecl);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class ArgumentsNode : public Node {
public:
    ParameterList* params;
    ArgumentsNode(int l, int c);
    ArgumentsNode(ParameterList* pList, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class SubprogramHead : public Node {
public:
    IdentNode* name;
    ArgumentsNode* arguments;
    SubprogramHead(IdentNode* n, ArgumentsNode* args, int l, int c);
    virtual ~SubprogramHead() {}
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class FunctionHeadNode : public SubprogramHead {
public:
    StandardTypeNode* returnType;
    FunctionHeadNode(IdentNode* n, ArgumentsNode* args, StandardTypeNode* retType, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class ProcedureHeadNode : public SubprogramHead {
public:
    ProcedureHeadNode(IdentNode* n, ArgumentsNode* args, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class StatementList : public Node {
public:
    std::list<StatementNode*> statements;
    StatementList(int l, int c);
    StatementList(StatementNode* firstStmt, int l, int c);
    void addStatement(StatementNode* stmt);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class CompoundStatementNode : public StatementNode {
public:
    StatementList* stmts;
    CompoundStatementNode(StatementList* sList, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class SubprogramDeclaration : public Node {
public:
    SubprogramHead* head;
    Declarations* local_declarations;
    CompoundStatementNode* body;
    SubprogramDeclaration(SubprogramHead* h, Declarations* local_decls, CompoundStatementNode* b, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class SubprogramDeclarations : public Node {
public:
    std::list<SubprogramDeclaration*> subprograms;
    SubprogramDeclarations(int l, int c);
    void addSubprogramDeclaration(SubprogramDeclaration* subprog);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

enum class SymbolScope { GLOBAL, LOCAL };
class VariableNode : public ExprNode {
public:
    IdentNode* identifier;
    ExprNode* index;
    int offset;
    SymbolKind kind;
    SymbolScope scope;
    VariableNode(IdentNode* id, ExprNode* idx, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class AssignStatementNode : public StatementNode {
public:
    VariableNode* variable;
    ExprNode* expression;
    AssignStatementNode(VariableNode* var, ExprNode* expr, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class IfStatementNode : public StatementNode {
public:
    ExprNode* condition;
    StatementNode* thenStatement;
    StatementNode* elseStatement;
    IfStatementNode(ExprNode* cond, StatementNode* thenStmt, StatementNode* elseStmt, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class WhileStatementNode : public StatementNode {
public:
    ExprNode* condition;
    StatementNode* body;
    WhileStatementNode(ExprNode* cond, StatementNode* b, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class ProcedureCallStatementNode : public StatementNode {
public:
    IdentNode* procName;
    ExpressionList* arguments;
    ProcedureCallStatementNode(IdentNode* name, ExpressionList* args, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class IdExprNode : public ExprNode {
public:
    IdentNode* ident;
    // Added for semantic analysis info
    int offset;
    SymbolKind kind;
    SymbolScope scope;
    IdExprNode(IdentNode* id, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class FunctionCallExprNode : public ExprNode {
public:
    IdentNode* funcName;
    ExpressionList* arguments;
    FunctionCallExprNode(IdentNode* name, ExpressionList* args, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class BinaryOpNode : public ExprNode {
public:
    ExprNode* left;
    std::string op;
    ExprNode* right;
    BinaryOpNode(ExprNode* l_node, const std::string& oper, ExprNode* r_node, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class UnaryOpNode : public ExprNode {
public:
    std::string op;
    ExprNode* expression;
    UnaryOpNode(const std::string& oper, ExprNode* expr, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class ReturnStatementNode : public StatementNode {
public:
    ExprNode* returnValue;
    ReturnStatementNode(ExprNode* retVal, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

class ProgramNode : public Node {
public:
    IdentNode* progName;
    Declarations* decls;
    SubprogramDeclarations* subprogs;
    CompoundStatementNode* mainCompoundStmt;
    ProgramNode(IdentNode* name, Declarations* d, SubprogramDeclarations* s, CompoundStatementNode* cStmt, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    void accept(SemanticVisitor& visitor) override;
};

#endif // AST_H