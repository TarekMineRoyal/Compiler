#ifndef AST_H
#define AST_H

#include <vector>
#include <string>
//using namespace std; // It's generally better to qualify std:: or use 'using' in .cpp files / specific scopes

class Node;
class Func;
// class Type; // Not used as a class in your example, 'type' in parser refers to int
class Ident;
class Args;
class Arg; // Added for completeness, was in your original parser %type
class Stmt;
class Stmts;
class Expr; // Added for completeness, base for Num, RealLit, etc.
class Num; // Your existing class for integer literals
class RealLit; // New class for real literals
class VarDecl;
class IdExpr; // From your original parser
class Assign; // From your original parser
class Minus;  // From your original parser
class Add;    // From your original parser


class Node {
public:
    int line;
    int column;
    Node* father; // Optional: consider if all nodes need a father pointer immediately
    Node(int l, int c);
    virtual ~Node() {} // Good practice to have a virtual destructor for base classes
};

class Expr : public Node { // Base class for all expressions
public:
    Expr(int l, int c);
    // virtual void accept(Visitor* v) = 0; // Example for visitor pattern
};

class Func : public Node {
public:
    int type; // You used 'int' for type, consider an enum or a Type class later
    Ident* name;
    Args* args;
    Stmts* stmts;
    Func(int type, Ident* name, Args* args, Stmts* stmts, int l, int c);
};

class Arg : public Node {
public:
    int type; // Again, 'int' for type
    Ident* name;
    Arg(int type, Ident* name, int l, int c);
};

class Args : public Node {
public:
    std::vector<Arg*>* args_list; // Renamed to avoid conflict with class name, was 'args'
    Args(int l, int c);
    Args(Arg* first_arg, int l, int c);
    void AddArg(Arg* arg);
};

class Stmt : public Node { // Base class for all statements
public:
    Stmt* prev; // For linked list of statements in Stmts
    Stmt* next; // For linked list of statements in Stmts
    Stmt(int l, int c);
};

class Stmts : public Node {
public:
    Stmt* stmts_list; // Head of the linked list, was 'stmts'
    Stmts(int l, int c);
    Stmts(Stmt* first_stmt, int l, int c);
    void AddStmt(Stmt* stmt);
};

class VarDecl : public Stmt {
public:
    int type;
    Ident* ident;
    VarDecl(int type, Ident* i, int l, int c);
};

class Ident : public Node {
public:
    std::string name;
    Ident(const std::string& n, int l, int c); // Pass string by const reference
};

class Num : public Expr { // For Integer Literals
public:
    int value;
    Num(int val, int l, int c); // Changed constructor for clarity: value, line, column
};

class RealLit : public Expr { // For Real Literals (NEW)
public:
    double value;
    RealLit(double val, int l, int c);
};

class Minus : public Expr {
public:
    Expr* expr;
    Minus(Expr* e, int l, int c);
};

class IdExpr : public Expr {
public:
    Ident* ident;
    IdExpr(Ident* id, int l, int c);
};

class Assign : public Stmt {
public:
    Ident* ident;
    Expr* expr_val; // Renamed from 'exp' to be more descriptive
    Assign(Ident* i, Expr* e, int l, int c);
};

class Add : public Expr {
public:
    Expr* left;
    Expr* right;
    Add(Expr* e1, Expr* e2, int l, int c);
};

#endif // AST_H