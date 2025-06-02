#include "ast.h"
#include <iostream>


Node::Node(int line, int column)
{
    this->line = line;
    this->column = column;
    this->father = NULL;
}

Func::Func(int t, Ident *n, Args *as, Stmts *ss, int l, int c) : Node(l, c)
    {
        this->type= t;
        this->name = n;
        this->args = as;
        this->stmts = ss;
        // t->father = this;/

        n->father = this;

        as->father = this;
    // ss->father = this;
}


Arg::Arg(int t, Ident *n, int l, int c) : Node(l, c)
{
    this->type= t;
    this->name = n;
    // t->father = this;
    n->father = this;
}

Args::Args(int l, int c) : Node(l, c)
{
    this->args = new std::vector<Arg *>;
}
Args::Args(Arg *a, int l, int c) : Node(l, c)
{
    this->args = new std::vector<Arg *>;
    this->AddArg(a);
}
void Args::AddArg(Arg *a)
{
    this->args->push_back(a);
    a->father = this;
}

Stmt::Stmt(int l, int c) : Node(l, c)
{
    this->next = NULL;
    this->prev = NULL;
}

Stmts::Stmts(int l, int c) : Node(l, c)
{
    this->stmts = NULL;
}
Stmts::Stmts(Stmt *s, int l, int c) : Node(l, c)
{
    this->stmts = NULL;
    this->AddStmt(s);
}

void Stmts::AddStmt(Stmt *s){
    if(this->stmts == NULL)
    {
        this->stmts = s;
    }
    else
    {
        Stmt *p;
        p = this->stmts;
        while(p->next != NULL)
            p = p->next;
        p->next = s;
        s->prev = p;
        s->father = this;
    }
}

VarDecl::VarDecl(int t, Ident* i, int l, int c) : Stmt(l, c)
{
    this->type = t;
    this->ident = i;
    ident->father = this;
}

Ident::Ident(std::string n, int l, int c) : Node(l, c)
{
    this->name = n;
}

Expr::Expr(int l, int c) : Node(l, c)
{
}
Num::Num(int l, int c, int v) : Expr(l, c)
{
    this->value = v;
}
Minus::Minus(Expr *e,int l, int c) : Expr(l, c)
{
    this->expr = e;
    e->father = this;
}

IdExpr::IdExpr(Ident *id,int l, int c) : Expr(l, c)
{
    this->ident = id;
}

// Assign::Assign(){

// }
Add::Add(Expr *e1, Expr *e2, int l, int c) : Expr(l, c)
{
    this->left = e1;
    this->right = e2;
}

Assign::Assign(Ident * i  , Expr * e, int l , int  c) : Stmt(l,c)
{
    this->ident =i;
    this->exp = e;
}
