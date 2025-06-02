#include "ast.h"
#include <iostream> // For potential debugging, not strictly necessary for all AST nodes

Node::Node(int l, int c) : line(l), column(c), father(nullptr) {}

Expr::Expr(int l, int c) : Node(l, c) {}

Func::Func(int t, Ident* n, Args* as, Stmts* slist, int l, int c)
    : Node(l, c), type(t), name(n), args(as), stmts(slist) {
    if (name) name->father = this;
    if (args) args->father = this;
    if (stmts) stmts->father = this;
}

Arg::Arg(int t, Ident* n, int l, int c)
    : Node(l, c), type(t), name(n) {
    if (name) name->father = this;
}

Args::Args(int l, int c) : Node(l, c) {
    args_list = new std::vector<Arg*>;
}

Args::Args(Arg* first_arg, int l, int c) : Node(l, c) {
    args_list = new std::vector<Arg*>;
    if (first_arg) {
        AddArg(first_arg);
    }
}

void Args::AddArg(Arg* arg) {
    if (arg) {
        args_list->push_back(arg);
        arg->father = this;
    }
}

Stmt::Stmt(int l, int c) : Node(l, c), prev(nullptr), next(nullptr) {}

Stmts::Stmts(int l, int c) : Node(l, c), stmts_list(nullptr) {}

Stmts::Stmts(Stmt* first_stmt, int l, int c) : Node(l, c), stmts_list(nullptr) {
    if (first_stmt) {
        AddStmt(first_stmt);
    }
}

void Stmts::AddStmt(Stmt* s) {
    if (!s) return;
    s->father = this;
    if (stmts_list == nullptr) {
        stmts_list = s;
        s->prev = nullptr;
        s->next = nullptr;
    }
    else {
        Stmt* p = stmts_list;
        while (p->next != nullptr) {
            p = p->next;
        }
        p->next = s;
        s->prev = p;
        s->next = nullptr;
    }
}

VarDecl::VarDecl(int t, Ident* i, int l, int c) : Stmt(l, c), type(t), ident(i) {
    if (ident) ident->father = this;
}

Ident::Ident(const std::string& n, int l, int c) : Node(l, c), name(n) {}

// Constructor for Num was Num(int, int, int) -> Num(value, line, col)
// User's old one: Num::Num(int l, int c, int v) : Expr(l, c) { this->value = v;}
// This means line, col, value. Let's stick to value, line, col for consistency.
Num::Num(int val, int l, int c) : Expr(l, c), value(val) {}

// New RealLit implementation
RealLit::RealLit(double val, int l, int c) : Expr(l, c), value(val) {}

Minus::Minus(Expr* e, int l, int c) : Expr(l, c), expr(e) {
    if (expr) expr->father = this;
}

IdExpr::IdExpr(Ident* id, int l, int c) : Expr(l, c), ident(id) {
    if (ident) ident->father = this;
}

Assign::Assign(Ident* i, Expr* e, int l, int c) : Stmt(l, c), ident(i), expr_val(e) {
    if (ident) ident->father = this;
    if (expr_val) expr_val->father = this;
}

Add::Add(Expr* e1, Expr* e2, int l, int c) : Expr(l, c), left(e1), right(e2) {
    if (left) left->father = this;
    if (right) right->father = this;
}