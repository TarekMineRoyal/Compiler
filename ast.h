#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
#include <list> 

// --- Base Node Class (Define early as it's widely used) ---
class Node {
public:
    int line;
    int column;
    Node* father;
    Node(int l, int c);
    virtual ~Node() {}
};

// --- Lexer Helper Original Classes (Full definitions needed by parser.y actions for $1, etc.) ---
// These are the types the lexer creates. The parser will access their members.
class Expr : public Node { // Your original Expr, used by Num and RealLit
public:
    Expr(int l, int c);
};

class Ident : public Node {
public:
    std::string name;
    Ident(const std::string& n, int l, int c);
};

class Num : public Expr {
public:
    int value;
    Num(int val, int l, int c);
};

class RealLit : public Expr {
public:
    double value;
    RealLit(double val, int l, int c);
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


// --- BASE AST NODE CLASSES (after Node is defined) ---
class ExprNode : public Node {
public:
    ExprNode(int l, int c);
};

class StatementNode : public Node {
public:
    StatementNode(int l, int c);
};

class TypeNode : public Node {
public:
    TypeNode(int l, int c);
};

// --- SPECIFIC AST NODE DECLARATIONS (Full Definitions) ---

class IdentNode : public ExprNode {
public:
    std::string name;
    IdentNode(const std::string& n, int l, int c);
};

class IntNumNode : public ExprNode {
public:
    int value;
    IntNumNode(int val, int l, int c);
};

class RealNumNode : public ExprNode {
public:
    double value;
    RealNumNode(double val, int l, int c);
};

class BooleanLiteralNode : public ExprNode {
public:
    bool value;
    BooleanLiteralNode(bool val, int l, int c);
};

class IdentifierList : public Node {
public:
    std::list<IdentNode*> identifiers;
    IdentifierList(IdentNode* firstIdent, int l, int c);
    void addIdentifier(IdentNode* ident);
};

class StandardTypeNode : public TypeNode {
public:
    enum TypeCategory { TYPE_INTEGER, TYPE_REAL, TYPE_BOOLEAN };
    TypeCategory category;
    StandardTypeNode(TypeCategory cat, int l, int c);
};

class ArrayTypeNode : public TypeNode {
public:
    IntNumNode* startIndex;
    IntNumNode* endIndex;
    StandardTypeNode* elementType;
    ArrayTypeNode(IntNumNode* start, IntNumNode* end, StandardTypeNode* elemType, int l, int c);
};

class VarDecl : public StatementNode {
public:
    IdentifierList* identifiers;
    TypeNode* type;
    VarDecl(IdentifierList* ids, TypeNode* t, int l, int c);
};

class Declarations : public Node {
public:
    std::list<VarDecl*> var_decl_items;
    Declarations(int l, int c);
    void addVarDecl(VarDecl* vd); // Declaration for the method
};

class ExpressionList : public Node {
public:
    std::list<ExprNode*> expressions;
    ExpressionList(int l, int c);
    ExpressionList(ExprNode* firstExpr, int l, int c);
    void addExpression(ExprNode* expr);
};

class ParameterDeclaration : public Node {
public:
    IdentifierList* ids;
    TypeNode* type;
    ParameterDeclaration(IdentifierList* idList, TypeNode* t, int l, int c);
};

class ParameterList : public Node {
public:
    std::list<ParameterDeclaration*> paramDeclarations;
    ParameterList(ParameterDeclaration* firstParamDecl, int l, int c);
    void addParameterDeclarationGroup(ParameterDeclaration* paramDecl);
};

class ArgumentsNode : public Node {
public:
    ParameterList* params;
    ArgumentsNode(int l, int c);
    ArgumentsNode(ParameterList* pList, int l, int c);
};

class SubprogramHead : public Node {
public:
    IdentNode* name;
    ArgumentsNode* arguments;
    SubprogramHead(IdentNode* n, ArgumentsNode* args, int l, int c);
    virtual ~SubprogramHead() {}
};

class FunctionHeadNode : public SubprogramHead {
public:
    StandardTypeNode* returnType;
    FunctionHeadNode(IdentNode* n, ArgumentsNode* args, StandardTypeNode* retType, int l, int c);
};

class ProcedureHeadNode : public SubprogramHead {
public:
    ProcedureHeadNode(IdentNode* n, ArgumentsNode* args, int l, int c);
};

class StatementList : public Node {
public:
    std::list<StatementNode*> statements;
    StatementList(int l, int c);
    StatementList(StatementNode* firstStmt, int l, int c);
    void addStatement(StatementNode* stmt);
};

class CompoundStatementNode : public StatementNode {
public:
    StatementList* stmts;
    CompoundStatementNode(StatementList* sList, int l, int c);
};

class SubprogramDeclaration : public Node {
public:
    SubprogramHead* head;
    CompoundStatementNode* body;
    SubprogramDeclaration(SubprogramHead* h, CompoundStatementNode* b, int l, int c);
};

class SubprogramDeclarations : public Node {
public:
    std::list<SubprogramDeclaration*> subprograms;
    SubprogramDeclarations(int l, int c);
    void addSubprogramDeclaration(SubprogramDeclaration* subprog);
};

class VariableNode : public ExprNode {
public:
    IdentNode* identifier;
    ExprNode* index;
    VariableNode(IdentNode* id, ExprNode* idx, int l, int c);
};

class AssignStatementNode : public StatementNode {
public:
    VariableNode* variable;
    ExprNode* expression;
    AssignStatementNode(VariableNode* var, ExprNode* expr, int l, int c);
};

class IfStatementNode : public StatementNode {
public:
    ExprNode* condition;
    StatementNode* thenStatement;
    StatementNode* elseStatement;
    IfStatementNode(ExprNode* cond, StatementNode* thenStmt, StatementNode* elseStmt, int l, int c);
};

class WhileStatementNode : public StatementNode {
public:
    ExprNode* condition;
    StatementNode* body;
    WhileStatementNode(ExprNode* cond, StatementNode* b, int l, int c);
};

class ProcedureCallStatementNode : public StatementNode {
public:
    IdentNode* procName;
    ExpressionList* arguments;
    ProcedureCallStatementNode(IdentNode* name, ExpressionList* args, int l, int c);
};

class IdExprNode : public ExprNode {
public:
    IdentNode* ident;
    IdExprNode(IdentNode* id, int l, int c);
};

class FunctionCallExprNode : public ExprNode {
public:
    IdentNode* funcName;
    ExpressionList* arguments;
    FunctionCallExprNode(IdentNode* name, ExpressionList* args, int l, int c);
};

class BinaryOpNode : public ExprNode {
public:
    ExprNode* left;
    std::string op;
    ExprNode* right;
    BinaryOpNode(ExprNode* l_node, const std::string& oper, ExprNode* r_node, int l, int c);
};

class UnaryOpNode : public ExprNode {
public:
    std::string op;
    ExprNode* expression;
    UnaryOpNode(const std::string& oper, ExprNode* expr, int l, int c);
};

class ProgramNode : public Node {
public:
    IdentNode* progName;
    Declarations* decls;
    SubprogramDeclarations* subprogs;
    CompoundStatementNode* mainCompoundStmt;
    ProgramNode(IdentNode* name, Declarations* d, SubprogramDeclarations* s, CompoundStatementNode* cStmt, int l, int c);
};

#endif // AST_H
