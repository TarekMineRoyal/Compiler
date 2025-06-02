// #include <bits/stdc++.h>
#ifndef AST_H
#define AST_H
#include <vector>
#include <string>
//using namespace std;

class Node;
class Func;
class Type;
class Ident;
class Args;
class Stmt;
class Stmts;
class Num;
class VarDecl;

class Node {
public:
    int line;
    int column;
    Node *father;
    Node(int, int);
};

class Func : public Node
{
    public:
    int type;
    Ident *name;
    Args *args;
    Stmts *stmts;
    Func(int , Ident *, Args *, Stmts *, int, int);
};

class Arg : public Node
{
    public:
    int type;
    Ident *name;
    Arg(int, Ident *, int, int);
    // void accept(Visitor *);
};

class Args : public Node
{
    public:
    std::vector<Arg *> *args;
    Args(int, int);
    Args(Arg *, int, int);
    void AddArg(Arg *);
};

class Stmt : public Node
{
    public:
    Stmt *prev;
    Stmt *next;
    Stmt(int, int);
};

class Stmts : public Node
{
public:
    Stmt *stmts;
    Stmts(int, int);
    Stmts(Stmt *, int, int);
    void AddStmt(Stmt *);
};
class VarDecl : public Stmt
{
    public:
    int type;
    Ident *ident;
    VarDecl(int, Ident*, int, int);
};

class Ident : public Node
{
    public:
    std::string name;
    Ident(std::string n, int l, int c);
};

class Expr : public Node
{
    public:
        Expr(int, int);
};

class Num : public Expr
{
    public:
    int value;
    Num(int, int, int);
};


class Minus : public Expr
{
    public:
    Expr *expr;
    Minus(Expr *, int, int);
};
class IdExpr : public Expr
{
    public:
    Ident *ident;
    IdExpr(Ident *, int, int);
};

class Assign : public Stmt
{
    public:
    Ident* ident;
    Expr* exp;
    Assign(Ident *, Expr *, int, int);
};
class Add : public Expr
{
    public:
    Expr*left;
    Expr*right;
    Add(Expr *, Expr *, int, int);
};



#endif