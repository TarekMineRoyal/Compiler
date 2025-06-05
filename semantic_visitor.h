#ifndef SEMANTIC_VISITOR_H
#define SEMANTIC_VISITOR_H

// Forward declarations of all your AST node classes
class ProgramNode;
class IdentifierList;
class IdentNode;
class Declarations;
class VarDecl;
class TypeNode;
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
class StringLiteralNode;
class BinaryOpNode;
class UnaryOpNode;
class IdExprNode; // << UNCOMMENTED
class FunctionCallExprNode;
class ReturnStatementNode;

class SemanticVisitor {
public:
    virtual ~SemanticVisitor() = default;

    virtual void visit(ProgramNode& node) = 0;
    virtual void visit(IdentifierList& node) = 0;
    virtual void visit(IdentNode& node) = 0;
    virtual void visit(Declarations& node) = 0;
    virtual void visit(VarDecl& node) = 0;
    // TypeNode might not be visited directly if it's abstract in behavior
    // virtual void visit(TypeNode& node) = 0; 
    virtual void visit(StandardTypeNode& node) = 0;
    virtual void visit(ArrayTypeNode& node) = 0;
    virtual void visit(SubprogramDeclarations& node) = 0;
    virtual void visit(SubprogramDeclaration& node) = 0;
    // SubprogramHead might not be visited directly
    // virtual void visit(SubprogramHead& node) = 0; 
    virtual void visit(FunctionHeadNode& node) = 0;
    virtual void visit(ProcedureHeadNode& node) = 0;
    virtual void visit(ArgumentsNode& node) = 0;
    virtual void visit(ParameterList& node) = 0;
    virtual void visit(ParameterDeclaration& node) = 0;
    virtual void visit(CompoundStatementNode& node) = 0;
    virtual void visit(StatementList& node) = 0;
    virtual void visit(AssignStatementNode& node) = 0;
    virtual void visit(IfStatementNode& node) = 0;
    virtual void visit(WhileStatementNode& node) = 0;
    virtual void visit(VariableNode& node) = 0;
    virtual void visit(ProcedureCallStatementNode& node) = 0;
    virtual void visit(ExpressionList& node) = 0;
    virtual void visit(IntNumNode& node) = 0;
    virtual void visit(RealNumNode& node) = 0;
    virtual void visit(BooleanLiteralNode& node) = 0;
    virtual void visit(StringLiteralNode& node) = 0;
    virtual void visit(BinaryOpNode& node) = 0;
    virtual void visit(UnaryOpNode& node) = 0;
    virtual void visit(IdExprNode& node) = 0; // << UNCOMMENTED
    virtual void visit(FunctionCallExprNode& node) = 0;
    virtual void visit(ReturnStatementNode& node) = 0;
};

#endif // SEMANTIC_VISITOR_H