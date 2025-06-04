#include "ast.h"
#include <iostream> // For std::cout, std::endl, etc.

// Helper function for indentation
static void print_indent(std::ostream& out, int indentLevel) {
    for (int i = 0; i < indentLevel; ++i) {
        out << "  "; // 2 spaces per indent level
    }
}

// --- Base Node Class Implementation ---
Node::Node(int l, int c) : line(l), column(c), father(nullptr) {}
// Node::print is pure virtual, so no implementation in Node itself.

// --- Lexer Helper Original Classes Implementations ---
Expr::Expr(int l, int c) : Node(l, c) {}
void Expr::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "Lexer::Expr (L:" << line << ", C:" << column << ") (Should not be in final AST)" << std::endl;
}

Ident::Ident(const std::string& n, int l, int c) : Node(l, c), name(n) {}
void Ident::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "Lexer::Ident (Name: " << name << ", L:" << line << ", C:" << column << ") (Should not be in final AST)" << std::endl;
}

Num::Num(int val, int l, int c) : Expr(l, c), value(val) {}
void Num::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "Lexer::Num (Value: " << value << ", L:" << line << ", C:" << column << ") (Should not be in final AST)" << std::endl;
}

RealLit::RealLit(double val, int l, int c) : Expr(l, c), value(val) {}
void RealLit::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "Lexer::RealLit (Value: " << value << ", L:" << line << ", C:" << column << ") (Should not be in final AST)" << std::endl;
}


// --- BASE AST NODE Implementations ---
ExprNode::ExprNode(int l, int c) : Node(l, c) {}
void ExprNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ExprNode (Base) (L:" << line << ", C:" << column << ")" << std::endl;
}

StatementNode::StatementNode(int l, int c) : Node(l, c) {}
void StatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "StatementNode (Base) (L:" << line << ", C:" << column << ")" << std::endl;
}

TypeNode::TypeNode(int l, int c) : Node(l, c) {}
void TypeNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "TypeNode (Base) (L:" << line << ", C:" << column << ")" << std::endl;
}

// --- SPECIFIC AST NODE Implementations ---

IdentNode::IdentNode(const std::string& n, int l, int c) : ExprNode(l, c), name(n) {}
void IdentNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "IdentNode (Name: " << name << ", L:" << line << ", C:" << column << ")" << std::endl;
}

IntNumNode::IntNumNode(int val, int l, int c) : ExprNode(l, c), value(val) {}
void IntNumNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "IntNumNode (Value: " << value << ", L:" << line << ", C:" << column << ")" << std::endl;
}

RealNumNode::RealNumNode(double val, int l, int c) : ExprNode(l, c), value(val) {}
void RealNumNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "RealNumNode (Value: " << value << ", L:" << line << ", C:" << column << ")" << std::endl;
}

BooleanLiteralNode::BooleanLiteralNode(bool val, int l, int c) : ExprNode(l, c), value(val) {}
void BooleanLiteralNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "BooleanLiteralNode (Value: " << (value ? "true" : "false") << ", L:" << line << ", C:" << column << ")" << std::endl;
}

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
void IdentifierList::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "IdentifierList (L:" << line << ", C:" << column << ")" << std::endl;
    for (IdentNode* id : identifiers) {
        if (id) id->print(out, indentLevel + 1);
    }
}

StandardTypeNode::StandardTypeNode(StandardTypeNode::TypeCategory cat, int l, int c)
    : TypeNode(l, c), category(cat) {}
void StandardTypeNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "StandardTypeNode (Type: ";
    switch (category) {
    case TYPE_INTEGER: out << "INTEGER"; break;
    case TYPE_REAL:    out << "REAL";    break;
    case TYPE_BOOLEAN: out << "BOOLEAN"; break;
    default: out << "UNKNOWN"; break;
    }
    out << ", L:" << line << ", C:" << column << ")" << std::endl;
}

ArrayTypeNode::ArrayTypeNode(IntNumNode* start, IntNumNode* end, StandardTypeNode* elemType, int l, int c)
    : TypeNode(l, c), startIndex(start), endIndex(end), elementType(elemType) {
    if (startIndex) startIndex->father = this;
    if (endIndex) endIndex->father = this;
    if (elementType) elementType->father = this;
}
void ArrayTypeNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ArrayTypeNode (L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "StartIndex:" << std::endl;
    if (startIndex) startIndex->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "EndIndex:" << std::endl;
    if (endIndex) endIndex->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "ElementType:" << std::endl;
    if (elementType) elementType->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
}

VarDecl::VarDecl(IdentifierList* ids, TypeNode* t, int l, int c)
    : StatementNode(l, c), identifiers(ids), type(t) { // Note: VarDecl is often not a StatementNode, but a distinct DeclarationNode
    if (identifiers) identifiers->father = this;
    if (type) type->father = this;
}
void VarDecl::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "VarDecl (L:" << line << ", C:" << column << ")" << std::endl;
    if (identifiers) identifiers->print(out, indentLevel + 1); else { print_indent(out, indentLevel + 1); out << "Identifiers: nullptr" << std::endl; }
    if (type) type->print(out, indentLevel + 1); else { print_indent(out, indentLevel + 1); out << "Type: nullptr" << std::endl; }
}

Declarations::Declarations(int l, int c) : Node(l, c) {}
void Declarations::addVarDecl(VarDecl* vd) {
    if (vd) {
        var_decl_items.push_back(vd);
        vd->father = this;
    }
}
bool Declarations::isEmpty() const { // ADDED helper, if needed for print logic elsewhere, or use var_decl_items.empty() directly
    return var_decl_items.empty();
}
void Declarations::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "Declarations (L:" << line << ", C:" << column << ")" << std::endl;
    if (var_decl_items.empty()) {
        print_indent(out, indentLevel + 1);
        out << "(No variable declarations)" << std::endl;
    }
    else {
        for (VarDecl* vd : var_decl_items) {
            if (vd) vd->print(out, indentLevel + 1);
        }
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
void ExpressionList::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ExpressionList (L:" << line << ", C:" << column << ")" << std::endl;
    if (expressions.empty()) {
        print_indent(out, indentLevel + 1);
        out << "(Empty)" << std::endl;
    }
    else {
        for (ExprNode* expr : expressions) {
            if (expr) expr->print(out, indentLevel + 1);
        }
    }
}

ParameterDeclaration::ParameterDeclaration(IdentifierList* idList, TypeNode* t, int l, int c)
    : Node(l, c), ids(idList), type(t) {
    if (ids) ids->father = this;
    if (type) type->father = this;
}
void ParameterDeclaration::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ParameterDeclaration (L:" << line << ", C:" << column << ")" << std::endl;
    if (ids) ids->print(out, indentLevel + 1); else { print_indent(out, indentLevel + 1); out << "Identifiers: nullptr" << std::endl; }
    if (type) type->print(out, indentLevel + 1); else { print_indent(out, indentLevel + 1); out << "Type: nullptr" << std::endl; }
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
void ParameterList::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ParameterList (L:" << line << ", C:" << column << ")" << std::endl;
    if (paramDeclarations.empty()) {
        print_indent(out, indentLevel + 1);
        out << "(No parameters)" << std::endl;
    }
    else {
        for (ParameterDeclaration* pd : paramDeclarations) {
            if (pd) pd->print(out, indentLevel + 1);
        }
    }
}

ArgumentsNode::ArgumentsNode(int l, int c) : Node(l, c), params(nullptr) {}
ArgumentsNode::ArgumentsNode(ParameterList* pList, int l, int c) : Node(l, c), params(pList) {
    if (params) params->father = this;
}
void ArgumentsNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ArgumentsNode (L:" << line << ", C:" << column << ")" << std::endl;
    if (params && !params->paramDeclarations.empty()) { // Check internal list of params
        params->print(out, indentLevel + 1);
    }
    else {
        print_indent(out, indentLevel + 1);
        out << "(No arguments)" << std::endl;
    }
}

SubprogramHead::SubprogramHead(IdentNode* n, ArgumentsNode* args_in, int l, int c)
    : Node(l, c), name(n), arguments(args_in) {
    if (name) name->father = this;
    if (arguments) arguments->father = this;
}
void SubprogramHead::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "SubprogramHead (Base) (L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "Name:" << std::endl;
    if (name) name->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "Arguments:" << std::endl;
    if (arguments) arguments->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
}


FunctionHeadNode::FunctionHeadNode(IdentNode* n, ArgumentsNode* args_in, StandardTypeNode* retType, int l, int c)
    : SubprogramHead(n, args_in, l, c), returnType(retType) {
    if (returnType) returnType->father = this;
}
void FunctionHeadNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "FunctionHeadNode (L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "Name:" << std::endl;
    if (name) name->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "Arguments:" << std::endl;
    if (arguments) arguments->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "ReturnType:" << std::endl;
    if (returnType) returnType->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
}

ProcedureHeadNode::ProcedureHeadNode(IdentNode* n, ArgumentsNode* args_in, int l, int c)
    : SubprogramHead(n, args_in, l, c) {}
void ProcedureHeadNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ProcedureHeadNode (L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "Name:" << std::endl;
    if (name) name->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "Arguments:" << std::endl;
    if (arguments) arguments->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
}

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
void StatementList::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "StatementList (L:" << line << ", C:" << column << ")" << std::endl;
    if (statements.empty()) {
        print_indent(out, indentLevel + 1);
        out << "(Empty)" << std::endl;
    }
    else {
        for (StatementNode* stmt : statements) {
            if (stmt) stmt->print(out, indentLevel + 1);
        }
    }
}

CompoundStatementNode::CompoundStatementNode(StatementList* sList, int l, int c)
    : StatementNode(l, c), stmts(sList) {
    if (stmts) stmts->father = this;
}
void CompoundStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "CompoundStatementNode (L:" << line << ", C:" << column << ")" << std::endl;
    if (stmts) stmts->print(out, indentLevel + 1); else { print_indent(out, indentLevel + 1); out << "Statements: nullptr" << std::endl; }
}

// --- SubprogramDeclaration MODIFIED ---
SubprogramDeclaration::SubprogramDeclaration(SubprogramHead* h, Declarations* local_decls, CompoundStatementNode* b, int l, int c) // MODIFIED signature
    : Node(l, c), head(h), local_declarations(local_decls), body(b) { // MODIFIED to include local_declarations
    if (head) head->father = this;
    if (local_declarations) local_declarations->father = this; // ADDED father assignment
    if (body) body->father = this;
}

void SubprogramDeclaration::print(std::ostream& out, int indentLevel) const { // MODIFIED print method
    print_indent(out, indentLevel);
    out << "SubprogramDeclaration (L:" << line << ", C:" << column << ")" << std::endl;

    if (head) {
        head->print(out, indentLevel + 1);
    }
    else {
        print_indent(out, indentLevel + 1); out << "Head: nullptr" << std::endl;
    }

    // ADDED printing for local_declarations
    if (local_declarations) { // Check if the pointer is not null
        // The Declarations::print method already handles being empty gracefully
        print_indent(out, indentLevel + 1); out << "LocalDeclarations:" << std::endl;
        local_declarations->print(out, indentLevel + 2);
    }
    else {
        // Optionally print something if local_declarations pointer is null, or just skip
        // For consistency with how ProgramNode prints its Declarations, we can let Declarations::print handle it
        // However, ProgramNode directly passes its decls pointer. Here, local_declarations might be genuinely null
        // if the grammar allows constructing SubprogramDeclaration with a null Declarations* for some reason
        // (though our current parser.y rule for subprogram_declaration always provides $2 as Declarations*)
        // A simple check to print "none" if local_declarations is null or effectively empty
        print_indent(out, indentLevel + 1); out << "LocalDeclarations:" << std::endl;
        print_indent(out, indentLevel + 2); out << "(None or nullptr)" << std::endl;
    }

    if (body) {
        body->print(out, indentLevel + 1);
    }
    else {
        print_indent(out, indentLevel + 1); out << "Body: nullptr" << std::endl;
    }
}
// --- End SubprogramDeclaration MODIFIED ---


SubprogramDeclarations::SubprogramDeclarations(int l, int c) : Node(l, c) {}
void SubprogramDeclarations::addSubprogramDeclaration(SubprogramDeclaration* subprog) {
    if (subprog) {
        subprograms.push_back(subprog);
        subprog->father = this;
    }
}
void SubprogramDeclarations::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "SubprogramDeclarations (L:" << line << ", C:" << column << ")" << std::endl;
    if (subprograms.empty()) {
        print_indent(out, indentLevel + 1);
        out << "(No subprogram declarations)" << std::endl;
    }
    else {
        for (SubprogramDeclaration* sub : subprograms) {
            if (sub) sub->print(out, indentLevel + 1);
        }
    }
}

VariableNode::VariableNode(IdentNode* id, ExprNode* idx, int l, int c)
    : ExprNode(l, c), identifier(id), index(idx) {
    if (identifier) identifier->father = this;
    if (index) index->father = this;
}
void VariableNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "VariableNode (L:" << line << ", C:" << column << ")" << std::endl;
    if (identifier) identifier->print(out, indentLevel + 1); else { print_indent(out, indentLevel + 1); out << "Identifier: nullptr" << std::endl; }
    if (index) {
        print_indent(out, indentLevel + 1);
        out << "Index:" << std::endl;
        index->print(out, indentLevel + 2);
    }
}

AssignStatementNode::AssignStatementNode(VariableNode* var, ExprNode* expr, int l, int c)
    : StatementNode(l, c), variable(var), expression(expr) {
    if (variable) variable->father = this;
    if (expression) expression->father = this;
}
void AssignStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "AssignStatementNode (L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "Variable:" << std::endl;
    if (variable) variable->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "Expression:" << std::endl;
    if (expression) expression->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
}

IfStatementNode::IfStatementNode(ExprNode* cond, StatementNode* thenStmt, StatementNode* elseStmt, int l, int c)
    : StatementNode(l, c), condition(cond), thenStatement(thenStmt), elseStatement(elseStmt) {
    if (condition) condition->father = this;
    if (thenStatement) thenStatement->father = this;
    if (elseStatement) elseStatement->father = this;
}
void IfStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "IfStatementNode (L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "Condition:" << std::endl;
    if (condition) condition->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "ThenStatement:" << std::endl;
    if (thenStatement) thenStatement->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    if (elseStatement) {
        print_indent(out, indentLevel + 1); out << "ElseStatement:" << std::endl;
        elseStatement->print(out, indentLevel + 2);
    }
    else {
        print_indent(out, indentLevel + 1); out << "ElseStatement: (none)" << std::endl;
    }
}

WhileStatementNode::WhileStatementNode(ExprNode* cond, StatementNode* b, int l, int c)
    : StatementNode(l, c), condition(cond), body(b) {
    if (condition) condition->father = this;
    if (body) body->father = this;
}
void WhileStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "WhileStatementNode (L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "Condition:" << std::endl;
    if (condition) condition->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "Body:" << std::endl;
    if (body) body->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
}

ProcedureCallStatementNode::ProcedureCallStatementNode(IdentNode* name, ExpressionList* args, int l, int c)
    : StatementNode(l, c), procName(name), arguments(args) {
    if (procName) procName->father = this;
    if (arguments) arguments->father = this;
}
void ProcedureCallStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ProcedureCallStatementNode (L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "Procedure Name:" << std::endl;
    if (procName) procName->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "Arguments:" << std::endl;
    if (arguments && !arguments->expressions.empty()) {
        arguments->print(out, indentLevel + 2);
    }
    else {
        print_indent(out, indentLevel + 2); out << "(No arguments or nullptr)" << std::endl;
    }
}

IdExprNode::IdExprNode(IdentNode* id_node, int l, int c) : ExprNode(l, c), ident(id_node) {
    if (ident) ident->father = this;
}
void IdExprNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "IdExprNode (L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "Identifier:" << std::endl;
    if (ident) ident->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
}

FunctionCallExprNode::FunctionCallExprNode(IdentNode* name, ExpressionList* args, int l, int c)
    : ExprNode(l, c), funcName(name), arguments(args) {
    if (funcName) funcName->father = this;
    if (arguments) arguments->father = this;
}
void FunctionCallExprNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "FunctionCallExprNode (L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "Function Name:" << std::endl;
    if (funcName) funcName->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "Arguments:" << std::endl;
    if (arguments && !arguments->expressions.empty()) {
        arguments->print(out, indentLevel + 2);
    }
    else {
        print_indent(out, indentLevel + 2); out << "(No arguments or nullptr)" << std::endl;
    }
}

BinaryOpNode::BinaryOpNode(ExprNode* l_node, const std::string& oper, ExprNode* r_node, int l, int c)
    : ExprNode(l, c), left(l_node), op(oper), right(r_node) {
    if (left) left->father = this;
    if (right) right->father = this;
}
void BinaryOpNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "BinaryOpNode (Operator: " << op << ", L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "LeftOperand:" << std::endl;
    if (left) left->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
    print_indent(out, indentLevel + 1); out << "RightOperand:" << std::endl;
    if (right) right->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
}

UnaryOpNode::UnaryOpNode(const std::string& oper, ExprNode* expr, int l, int c)
    : ExprNode(l, c), op(oper), expression(expr) {
    if (expression) expression->father = this;
}
void UnaryOpNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "UnaryOpNode (Operator: " << op << ", L:" << line << ", C:" << column << ")" << std::endl;
    print_indent(out, indentLevel + 1); out << "Expression:" << std::endl;
    if (expression) expression->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
}

ProgramNode::ProgramNode(IdentNode* name, Declarations* d, SubprogramDeclarations* s, CompoundStatementNode* cStmt, int l, int c)
    : Node(l, c), progName(name), decls(d), subprogs(s), mainCompoundStmt(cStmt) {
    if (progName) progName->father = this;
    if (decls) decls->father = this;
    if (subprogs) subprogs->father = this;
    if (mainCompoundStmt) mainCompoundStmt->father = this;
}
void ProgramNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "ProgramNode (L:" << line << ", C:" << column << ")" << std::endl;

    print_indent(out, indentLevel + 1); out << "ProgramName:" << std::endl;
    if (progName) progName->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }

    print_indent(out, indentLevel + 1); out << "Declarations:" << std::endl;
    if (decls) decls->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }

    print_indent(out, indentLevel + 1); out << "SubprogramDeclarations:" << std::endl;
    if (subprogs) subprogs->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }

    print_indent(out, indentLevel + 1); out << "MainCompoundStatement:" << std::endl;
    if (mainCompoundStmt) mainCompoundStmt->print(out, indentLevel + 2); else { print_indent(out, indentLevel + 2); out << "nullptr" << std::endl; }
}

StringLiteralNode::StringLiteralNode(const char* val, int l, int c)
    : ExprNode(l, c), value(val ? val : "") {
    // if (val) free((void*)val); // This was commented out in a previous version I saw from the user.
                               // Decided to keep it as is from the user's provided code.
                               // Proper memory management of str_val from lexer needs care.
                               // If yylval.str_val is new char[] or strdup, AST should delete[]. If malloc, AST should free.
}
void StringLiteralNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel);
    out << "StringLiteralNode (Value: \"" << value << "\", L:" << line << ", C:" << column << ")" << std::endl;
}

ReturnStatementNode::ReturnStatementNode(ExprNode* retVal, int l, int c)
    : StatementNode(l, c), returnValue(retVal) {
    if (returnValue) returnValue->father = this;
}
void ReturnStatementNode::print(std::ostream& out, int indentLevel) const {
    print_indent(out, indentLevel); // Assuming your print_indent helper
    out << "ReturnStatementNode (L:" << line << ", C:" << column << ")" << std::endl;

    if (returnValue) {
        print_indent(out, indentLevel + 1);
        out << "ReturnValue:" << std::endl;
        returnValue->print(out, indentLevel + 2);
    }
    else {
        // Should not happen if grammar is "RETURN expr" for functions
        print_indent(out, indentLevel + 1);
        out << "ReturnValue: (nullptr or void return - check grammar)" << std::endl;
    }
}