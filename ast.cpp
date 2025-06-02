#include "ast.h"
#include <iostream> 

// --- Base Node Class Implementation ---
Node::Node(int l, int c) : line(l), column(c), father(nullptr) {}

// --- Lexer Helper Original Classes Implementations ---
Expr::Expr(int l, int c) : Node(l, c) {}
Ident::Ident(const std::string& n, int l, int c) : Node(l, c), name(n) {}
Num::Num(int val, int l, int c) : Expr(l, c), value(val) {}
RealLit::RealLit(double val, int l, int c) : Expr(l, c), value(val) {}

// --- BASE AST NODE Implementations ---
ExprNode::ExprNode(int l, int c) : Node(l, c) {}
StatementNode::StatementNode(int l, int c) : Node(l, c) {}
TypeNode::TypeNode(int l, int c) : Node(l, c) {}

// --- SPECIFIC AST NODE Implementations ---

IdentNode::IdentNode(const std::string& n, int l, int c) : ExprNode(l, c), name(n) {}

IntNumNode::IntNumNode(int val, int l, int c) : ExprNode(l, c), value(val) {}

RealNumNode::RealNumNode(double val, int l, int c) : ExprNode(l, c), value(val) {}

BooleanLiteralNode::BooleanLiteralNode(bool val, int l, int c) : ExprNode(l, c), value(val) {}

IdentifierList::IdentifierList(IdentNode* firstIdent, int l, int c) : Node(l, c) {
    if (firstIdent) {
        identifiers.push_back(firstIdent);
        if (firstIdent) firstIdent->father = this;
    }
}
void IdentifierList::addIdentifier(IdentNode* ident) {
    if (ident) {
        identifiers.push_back(ident);
        ident->father = this;
    }
}

StandardTypeNode::StandardTypeNode(StandardTypeNode::TypeCategory cat, int l, int c)
    : TypeNode(l, c), category(cat) {}

ArrayTypeNode::ArrayTypeNode(IntNumNode* start, IntNumNode* end, StandardTypeNode* elemType, int l, int c)
    : TypeNode(l, c), startIndex(start), endIndex(end), elementType(elemType) {
    if (startIndex) startIndex->father = this;
    if (endIndex) endIndex->father = this;
    if (elementType) elementType->father = this;
}

VarDecl::VarDecl(IdentifierList* ids, TypeNode* t, int l, int c)
    : StatementNode(l, c), identifiers(ids), type(t) {
    if (identifiers) identifiers->father = this;
    if (type) type->father = this;
}

Declarations::Declarations(int l, int c) : Node(l, c) {}
void Declarations::addVarDecl(VarDecl* vd) {
    if (vd) {
        var_decl_items.push_back(vd);
        vd->father = this;
    }
}

ExpressionList::ExpressionList(int l, int c) : Node(l, c) {}
ExpressionList::ExpressionList(ExprNode* firstExpr, int l, int c) : Node(l, c) {
    if (firstExpr) {
        expressions.push_back(firstExpr);
        if (firstExpr) firstExpr->father = this;
    }
}
void ExpressionList::addExpression(ExprNode* expr) {
    if (expr) {
        expressions.push_back(expr);
        expr->father = this;
    }
}

ParameterDeclaration::ParameterDeclaration(IdentifierList* idList, TypeNode* t, int l, int c)
    : Node(l, c), ids(idList), type(t) {
    if (ids) ids->father = this;
    if (type) type->father = this;
}

ParameterList::ParameterList(ParameterDeclaration* firstParamDecl, int l, int c) : Node(l, c) {
    if (firstParamDecl) {
        paramDeclarations.push_back(firstParamDecl);
        if (firstParamDecl) firstParamDecl->father = this;
    }
}
void ParameterList::addParameterDeclarationGroup(ParameterDeclaration* paramDecl) {
    if (paramDecl) {
        paramDeclarations.push_back(paramDecl);
        paramDecl->father = this;
    }
}

ArgumentsNode::ArgumentsNode(int l, int c) : Node(l, c), params(nullptr) {}
ArgumentsNode::ArgumentsNode(ParameterList* pList, int l, int c) : Node(l, c), params(pList) {
    if (params) params->father = this;
}

SubprogramHead::SubprogramHead(IdentNode* n, ArgumentsNode* args_in, int l, int c)
    : Node(l, c), name(n), arguments(args_in) {
    if (name) name->father = this;
    if (arguments) arguments->father = this;
}

FunctionHeadNode::FunctionHeadNode(IdentNode* n, ArgumentsNode* args_in, StandardTypeNode* retType, int l, int c)
    : SubprogramHead(n, args_in, l, c), returnType(retType) {
    if (returnType) returnType->father = this;
}

ProcedureHeadNode::ProcedureHeadNode(IdentNode* n, ArgumentsNode* args_in, int l, int c)
    : SubprogramHead(n, args_in, l, c) {}

StatementList::StatementList(int l, int c) : Node(l, c) {}
StatementList::StatementList(StatementNode* firstStmt, int l, int c) : Node(l, c) {
    if (firstStmt) {
        statements.push_back(firstStmt);
        if (firstStmt) firstStmt->father = this;
    }
}
void StatementList::addStatement(StatementNode* stmt) {
    if (stmt) {
        statements.push_back(stmt);
        stmt->father = this;
    }
}

CompoundStatementNode::CompoundStatementNode(StatementList* sList, int l, int c)
    : StatementNode(l, c), stmts(sList) {
    if (stmts) stmts->father = this;
}

SubprogramDeclaration::SubprogramDeclaration(SubprogramHead* h, CompoundStatementNode* b, int l, int c)
    : Node(l, c), head(h), body(b) {
    if (head) head->father = this;
    if (body) body->father = this;
}

SubprogramDeclarations::SubprogramDeclarations(int l, int c) : Node(l, c) {}
void SubprogramDeclarations::addSubprogramDeclaration(SubprogramDeclaration* subprog) {
    if (subprog) {
        subprograms.push_back(subprog);
        subprog->father = this;
    }
}

VariableNode::VariableNode(IdentNode* id, ExprNode* idx, int l, int c)
    : ExprNode(l, c), identifier(id), index(idx) {
    if (identifier) identifier->father = this;
    if (index) index->father = this;
}

AssignStatementNode::AssignStatementNode(VariableNode* var, ExprNode* expr, int l, int c)
    : StatementNode(l, c), variable(var), expression(expr) {
    if (variable) variable->father = this;
    if (expression) expression->father = this;
}

IfStatementNode::IfStatementNode(ExprNode* cond, StatementNode* thenStmt, StatementNode* elseStmt, int l, int c)
    : StatementNode(l, c), condition(cond), thenStatement(thenStmt), elseStatement(elseStmt) {
    if (condition) condition->father = this;
    if (thenStatement) thenStatement->father = this;
    if (elseStatement) elseStatement->father = this;
}

WhileStatementNode::WhileStatementNode(ExprNode* cond, StatementNode* b, int l, int c)
    : StatementNode(l, c), condition(cond), body(b) {
    if (condition) condition->father = this;
    if (body) body->father = this;
}

ProcedureCallStatementNode::ProcedureCallStatementNode(IdentNode* name, ExpressionList* args, int l, int c)
    : StatementNode(l, c), procName(name), arguments(args) {
    if (procName) procName->father = this;
    if (arguments) arguments->father = this;
}

IdExprNode::IdExprNode(IdentNode* id_node, int l, int c) : ExprNode(l, c), ident(id_node) {
    if (ident) ident->father = this;
}

FunctionCallExprNode::FunctionCallExprNode(IdentNode* name, ExpressionList* args, int l, int c)
    : ExprNode(l, c), funcName(name), arguments(args) {
    if (funcName) funcName->father = this;
    if (arguments) arguments->father = this;
}

BinaryOpNode::BinaryOpNode(ExprNode* l_node, const std::string& oper, ExprNode* r_node, int l, int c)
    : ExprNode(l, c), left(l_node), op(oper), right(r_node) {
    if (left) left->father = this;
    if (right) right->father = this;
}

UnaryOpNode::UnaryOpNode(const std::string& oper, ExprNode* expr, int l, int c)
    : ExprNode(l, c), op(oper), expression(expr) {
    if (expression) expression->father = this;
}

ProgramNode::ProgramNode(IdentNode* name, Declarations* d, SubprogramDeclarations* s, CompoundStatementNode* cStmt, int l, int c)
    : Node(l, c), progName(name), decls(d), subprogs(s), mainCompoundStmt(cStmt) {
    if (progName) progName->father = this;
    if (decls) decls->father = this;
    if (subprogs) subprogs->father = this;
    if (mainCompoundStmt) mainCompoundStmt->father = this;
}
