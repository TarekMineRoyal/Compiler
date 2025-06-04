#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <list>
#include <iosfwd> // For std::ostream forward declaration

// --- Base Node Class ---
class Node {
public:
    int line;
    int column;
    Node* father;
    Node(int l, int c);
    virtual ~Node() {}
    virtual void print(std::ostream& out, int indentLevel = 0) const = 0;
};

// --- Lexer Helper Original Classes ---
class Expr : public Node {
public:
    Expr(int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class Ident : public Node {
public:
    std::string name;
    Ident(const std::string& n, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class Num : public Expr {
public:
    int value;
    Num(int val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class RealLit : public Expr {
public:
    double value;
    RealLit(double val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

// --- FORWARD DECLARATIONS for main AST structure nodes ---
class ExprNode;
class StatementNode;
class TypeNode;
class IdentNode;
class Declarations;
class VarDecl;
class StandardTypeNode;
class ArrayTypeNode;
class SubprogramDeclarations;
class SubprogramDeclaration;
class SubprogramHead;
class FunctionHeadNode;
class ProcedureHeadNode;
class ArgumentsNode;
class ParameterList;
class ParameterDeclaration;
class CompoundStatementNode;
class StatementList;
class AssignStatementNode;
class IfStatementNode;
class WhileStatementNode;
class VariableNode;
class ProcedureCallStatementNode;
class ExpressionList;
class IntNumNode;
class RealNumNode;
class BooleanLiteralNode;
class BinaryOpNode;
class UnaryOpNode;
class IdExprNode;
class FunctionCallExprNode;
class ProgramNode;
class StringLiteralNode;
class ReturnStatementNode;


// --- BASE AST NODE CLASSES ---
class ExprNode : public Node {
public:
    ExprNode(int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override; // Basic implementation for abstract base
};

class StatementNode : public Node {
public:
    StatementNode(int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override; // Basic implementation for abstract base
};

class TypeNode : public Node {
public:
    TypeNode(int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override; // Basic implementation for abstract base
};

// --- SPECIFIC AST NODE DECLARATIONS ---

class IdentNode : public ExprNode {
public:
    std::string name;
    IdentNode(const std::string& n, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class IntNumNode : public ExprNode {
public:
    int value;
    IntNumNode(int val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class RealNumNode : public ExprNode {
public:
    double value;
    RealNumNode(double val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class BooleanLiteralNode : public ExprNode {
public:
    bool value;
    BooleanLiteralNode(bool val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class IdentifierList : public Node {
public:
    std::list<IdentNode*> identifiers;
    IdentifierList(IdentNode* firstIdent, int l, int c);
    void addIdentifier(IdentNode* ident);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class StandardTypeNode : public TypeNode {
public:
    enum TypeCategory { TYPE_INTEGER, TYPE_REAL, TYPE_BOOLEAN };
    TypeCategory category;
    StandardTypeNode(TypeCategory cat, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class ArrayTypeNode : public TypeNode {
public:
    IntNumNode* startIndex;
    IntNumNode* endIndex;
    StandardTypeNode* elementType;
    ArrayTypeNode(IntNumNode* start, IntNumNode* end, StandardTypeNode* elemType, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class VarDecl : public StatementNode { // Note: Consider if VarDecl should inherit from a different base if it's not strictly a statement
public:
    IdentifierList* identifiers;
    TypeNode* type;
    VarDecl(IdentifierList* ids, TypeNode* t, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class Declarations : public Node {
public:
    std::list<VarDecl*> var_decl_items;
    Declarations(int l, int c);
    void addVarDecl(VarDecl* vd);
    void print(std::ostream& out, int indentLevel = 0) const override;
    bool isEmpty() const; // << ADDED THIS DECLARATION
};

class ExpressionList : public Node {
public:
    std::list<ExprNode*> expressions;
    ExpressionList(int l, int c);
    ExpressionList(ExprNode* firstExpr, int l, int c);
    void addExpression(ExprNode* expr);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class ParameterDeclaration : public Node {
public:
    IdentifierList* ids;
    TypeNode* type;
    ParameterDeclaration(IdentifierList* idList, TypeNode* t, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class ParameterList : public Node {
public:
    std::list<ParameterDeclaration*> paramDeclarations;
    ParameterList(ParameterDeclaration* firstParamDecl, int l, int c);
    void addParameterDeclarationGroup(ParameterDeclaration* paramDecl);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class ArgumentsNode : public Node {
public:
    ParameterList* params;
    ArgumentsNode(int l, int c);
    ArgumentsNode(ParameterList* pList, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class SubprogramHead : public Node {
public:
    IdentNode* name;
    ArgumentsNode* arguments;
    SubprogramHead(IdentNode* n, ArgumentsNode* args, int l, int c);
    virtual ~SubprogramHead() {} // Keep virtual destructor if it's a polymorphic base
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class FunctionHeadNode : public SubprogramHead {
public:
    StandardTypeNode* returnType;
    FunctionHeadNode(IdentNode* n, ArgumentsNode* args, StandardTypeNode* retType, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class ProcedureHeadNode : public SubprogramHead {
public:
    ProcedureHeadNode(IdentNode* n, ArgumentsNode* args, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class StatementList : public Node {
public:
    std::list<StatementNode*> statements;
    StatementList(int l, int c);
    StatementList(StatementNode* firstStmt, int l, int c);
    void addStatement(StatementNode* stmt);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class CompoundStatementNode : public StatementNode {
public:
    StatementList* stmts;
    CompoundStatementNode(StatementList* sList, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class SubprogramDeclaration : public Node {
public:
    SubprogramHead* head;
    Declarations* local_declarations; // Member already present from your file
    CompoundStatementNode* body;

    // MODIFIED constructor declaration to include local_declarations:
    SubprogramDeclaration(SubprogramHead* h, Declarations* local_decls, CompoundStatementNode* b, int l, int c);

    void print(std::ostream& out, int indentLevel = 0) const override;
};

class SubprogramDeclarations : public Node {
public:
    std::list<SubprogramDeclaration*> subprograms;
    SubprogramDeclarations(int l, int c);
    void addSubprogramDeclaration(SubprogramDeclaration* subprog);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class VariableNode : public ExprNode {
public:
    IdentNode* identifier;
    ExprNode* index; // Can be null for non-array variables
    VariableNode(IdentNode* id, ExprNode* idx, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class AssignStatementNode : public StatementNode {
public:
    VariableNode* variable;
    ExprNode* expression;
    AssignStatementNode(VariableNode* var, ExprNode* expr, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class IfStatementNode : public StatementNode {
public:
    ExprNode* condition;
    StatementNode* thenStatement;
    StatementNode* elseStatement; // Can be null
    IfStatementNode(ExprNode* cond, StatementNode* thenStmt, StatementNode* elseStmt, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class WhileStatementNode : public StatementNode {
public:
    ExprNode* condition;
    StatementNode* body;
    WhileStatementNode(ExprNode* cond, StatementNode* b, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class ProcedureCallStatementNode : public StatementNode {
public:
    IdentNode* procName;
    ExpressionList* arguments;
    ProcedureCallStatementNode(IdentNode* name, ExpressionList* args, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class IdExprNode : public ExprNode { // Likely for identifiers used as expressions, might be redundant with VariableNode
public:
    IdentNode* ident;
    IdExprNode(IdentNode* id, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class FunctionCallExprNode : public ExprNode {
public:
    IdentNode* funcName;
    ExpressionList* arguments;
    FunctionCallExprNode(IdentNode* name, ExpressionList* args, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class BinaryOpNode : public ExprNode {
public:
    ExprNode* left;
    std::string op;
    ExprNode* right;
    BinaryOpNode(ExprNode* l_node, const std::string& oper, ExprNode* r_node, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class UnaryOpNode : public ExprNode {
public:
    std::string op;
    ExprNode* expression;
    UnaryOpNode(const std::string& oper, ExprNode* expr, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class ProgramNode : public Node {
public:
    IdentNode* progName;
    Declarations* decls; // Global declarations
    SubprogramDeclarations* subprogs;
    CompoundStatementNode* mainCompoundStmt;
    ProgramNode(IdentNode* name, Declarations* d, SubprogramDeclarations* s, CompoundStatementNode* cStmt, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};

class StringLiteralNode : public ExprNode {
public:
    std::string value;
    StringLiteralNode(const char* val, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
};
class ReturnStatementNode : public StatementNode {
public:
    ExprNode* returnValue; // The expression being returned

    ReturnStatementNode(ExprNode* retVal, int l, int c);
    void print(std::ostream& out, int indentLevel = 0) const override;
    // Consider a destructor if returnValue needs to be deleted by this node
    // virtual ~ReturnStatementNode() { delete returnValue; }
};

#endif // AST_H