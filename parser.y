%{
#include "ast.h" 
#include <iostream>
#include <string>
#include <vector>
#include <list>

extern int yylex();
extern int yyerror(const char*);
extern int yydebug;

extern int lin;
extern int col;

ProgramNode *root_ast_node; 
%}

%union {
    Node* pNode; 
    ProgramNode* pProgramNode;
    IdentifierList* pIdentifierList;
    IdentNode* pIdentNode; 
    Declarations* pDeclarations;
    VarDecl* pVarDecl;
    TypeNode* pTypeNode;
    StandardTypeNode* pStandardTypeNode;
    ArrayTypeNode* pArrayTypeNode;
    SubprogramDeclarations* pSubprogramDeclarations;
    SubprogramDeclaration* pSubprogramDeclaration;
    SubprogramHead* pSubprogramHead;
    ArgumentsNode* pArgumentsNode; 
    ParameterList* pParameterList;
    ParameterDeclaration* pParameterDeclaration;
    CompoundStatementNode* pCompoundStatementNode;
    StatementList* pStatementList;
    StatementNode* pStatementNode;
    VariableNode* pVariableNode;
    ProcedureCallStatementNode* pProcedureCallStatementNode;
    ExprNode* pExprNode;       
    ExpressionList* pExpressionList;
    IntNumNode* pIntNumNode;
    RealNumNode* pRealNumNode;
    BooleanLiteralNode* pBooleanLiteralNode;

    Num* rawNum;
    RealLit* rawRealLit;
    Ident* rawIdent;

    int token_val;
    char* str_val;
}

%token <rawNum> NUM
%token <rawRealLit> REAL_LITERAL
%token <rawIdent> IDENT
%token TRUE_KEYWORD FALSE_KEYWORD
%token PROGRAM VAR ARRAY OF INTEGER_TYPE REAL_TYPE BOOLEAN_TYPE FUNCTION PROCEDURE
%token BEGIN_TOKEN END_TOKEN IF THEN ELSE WHILE DO NOT_OP AND_OP OR_OP DIV_OP
%token ASSIGN_OP EQ_OP NEQ_OP LT_OP LTE_OP GT_OP GTE_OP DOTDOT 

%type <pProgramNode> program_rule
%type <pIdentifierList> identifier_list
%type <pIdentNode> id_node 
%type <pDeclarations> declarations var_declaration_list_non_empty 
%type <pVarDecl> var_declaration_item 
%type <pTypeNode> type
%type <pStandardTypeNode> standard_type
%type <pIntNumNode> int_num_node
%type <pRealNumNode> real_num_node
%type <pSubprogramDeclarations> subprogram_declarations 
%type <pSubprogramDeclaration> subprogram_declaration_block 
%type <pSubprogramDeclaration> subprogram_declaration
%type <pSubprogramHead> subprogram_head
%type <pArgumentsNode> arguments 
%type <pParameterList> parameter_list 
%type <pParameterDeclaration> parameter_declaration_group 
%type <pCompoundStatementNode> compound_statement
%type <pStatementList> optional_statements statement_list statement_list_terminated // Added statement_list_terminated here
%type <pStatementNode> statement
%type <pVariableNode> variable
%type <pProcedureCallStatementNode> procedure_statement
%type <pExpressionList> expression_list
%type <pExprNode> expression simple_expression term factor primary 

%left OR_OP                                            // 1. or (weakest)
%left AND_OP                                           // 2. and
%left NOT_OP                                           // 3. not (consider %right NOT_OP if needed, but %left mainly ranks it here)
%nonassoc EQ_OP NEQ_OP GT_OP GTE_OP LT_OP LTE_OP       // 4. relational operators (all same level)
%left '+' '-'                                          // 5. additive operators
%left '*' '/' DIV_OP                                   // 6. multiplicative operators (strongest of these)
%right UMINUS                                          // 7. unary minus (highest)

%nonassoc THEN
%nonassoc ELSE 

%start program_rule

%% 

program_rule: PROGRAM id_node ';' declarations subprogram_declarations compound_statement '.'
    { $$ = new ProgramNode($2, $4, $5, $6, lin, col); root_ast_node = $$; }
    ;

id_node: IDENT 
    { $$ = new IdentNode($1->name, $1->line, $1->column); delete $1; } 
    ;

identifier_list: id_node
    { $$ = new IdentifierList($1, lin, col); }
    | identifier_list ',' id_node
    { $1->addIdentifier($3); $$ = $1; }
    ;

declarations: /* empty */
    { $$ = new Declarations(lin, col); }
    | VAR var_declaration_list_non_empty
    { $$ = $2; } 
    ;

var_declaration_list_non_empty: var_declaration_item
    { $$ = new Declarations(lin, col); $$->addVarDecl($1); } 
    | var_declaration_list_non_empty var_declaration_item
    { $1->addVarDecl($2); $$ = $1; } 
    ;

var_declaration_item: identifier_list ':' type ';' 
    { $$ = new VarDecl($1, $3, lin, col); } 
    ;

type: standard_type
    { $$ = $1; } 
    | ARRAY '[' int_num_node DOTDOT int_num_node ']' OF standard_type
    { $$ = new ArrayTypeNode($3, $5, $8, lin, col); }
    ;

int_num_node: NUM 
    { $$ = new IntNumNode($1->value, $1->line, $1->column); delete $1; } 
    ;

real_num_node: REAL_LITERAL 
    { $$ = new RealNumNode($1->value, $1->line, $1->column); delete $1; } 
    ;

standard_type: INTEGER_TYPE
    { $$ = new StandardTypeNode(StandardTypeNode::TYPE_INTEGER, lin, col); }
    | REAL_TYPE
    { $$ = new StandardTypeNode(StandardTypeNode::TYPE_REAL, lin, col); }
    | BOOLEAN_TYPE
    { $$ = new StandardTypeNode(StandardTypeNode::TYPE_BOOLEAN, lin, col); }
    ;

subprogram_declarations: /* empty */
    { $$ = new SubprogramDeclarations(lin, col); }
    | subprogram_declarations subprogram_declaration_block 
    { $1->addSubprogramDeclaration($2); $$ = $1; }
    ;

subprogram_declaration_block: subprogram_declaration ';' 
    { $$ = $1; } 
    ;

subprogram_declaration: subprogram_head compound_statement
    { $$ = new SubprogramDeclaration($1, $2, lin, col); }
    ;

subprogram_head: FUNCTION id_node arguments ':' standard_type ';'
    { $$ = new FunctionHeadNode($2, $3, $5, lin, col); }
    | PROCEDURE id_node arguments ';'
    { $$ = new ProcedureHeadNode($2, $3, lin, col); }
    ;

arguments: /* empty */
    { $$ = new ArgumentsNode(lin, col); } 
    | '(' parameter_list ')' 
    { $$ = new ArgumentsNode($2, lin, col); } 
    ;

parameter_list: parameter_declaration_group 
    { $$ = new ParameterList($1, lin, col); }
    | parameter_list ';' parameter_declaration_group
    { $1->addParameterDeclarationGroup($3); $$ = $1; }
    ;

parameter_declaration_group: identifier_list ':' type
    { $$ = new ParameterDeclaration($1, $3, lin, col); } 
    ;

compound_statement: BEGIN_TOKEN optional_statements END_TOKEN
    { $$ = new CompoundStatementNode($2, lin, col); }
    ;

// Modified rules for handling optional trailing semicolon for statements
optional_statements: /* empty */
    { $$ = new StatementList(lin, col); } 
    | statement_list_terminated // Use the new rule here
    { $$ = $1; }
    ;

statement_list_terminated: statement_list
    { $$ = $1; } // A statement list without a trailing semicolon
    | statement_list ';' // A statement list *with* a trailing semicolon
    { $$ = $1; /* The list is $1, the semicolon is consumed */ }
    ;

// Original statement_list (semicolon as separator, not terminator for the whole list)
statement_list: statement
    { $$ = new StatementList(lin, col); $$->addStatement($1); } // Create list with first statement
    | statement_list ';' statement
    { $1->addStatement($3); $$ = $1; }
    ;
// End of modified statement rules

statement: variable ASSIGN_OP expression
    { $$ = new AssignStatementNode($1, $3, lin, col); }
    | procedure_statement
    { $$ = $1; } 
    | compound_statement
    { $$ = $1; } 
    | IF expression THEN statement %prec THEN 
    { $$ = new IfStatementNode($2, $4, nullptr, lin, col); }
    | IF expression THEN statement ELSE statement
    { $$ = new IfStatementNode($2, $4, $6, lin, col); }
    | WHILE expression DO statement
    { $$ = new WhileStatementNode($2, $4, lin, col); }
    ;

variable: id_node
    { $$ = new VariableNode($1, nullptr, lin, col); } 
    | id_node '[' expression ']'
    { $$ = new VariableNode($1, $3, lin, col); } 
    ;

procedure_statement: id_node
    { $$ = new ProcedureCallStatementNode($1, new ExpressionList(lin,col), lin, col); } 
    | id_node '(' expression_list ')'
    { $$ = new ProcedureCallStatementNode($1, $3, lin, col); } 
    ;

expression_list: expression
    { $$ = new ExpressionList($1, lin, col); $$->addExpression($1); } // Create list with first expression
    | expression_list ',' expression
    { $1->addExpression($3); $$ = $1; }
    ;

expression: simple_expression
    { $$ = $1; }
    | simple_expression EQ_OP simple_expression
    { $$ = new BinaryOpNode($1, "EQ_OP", $3, lin, col); }
    | simple_expression NEQ_OP simple_expression
    { $$ = new BinaryOpNode($1, "NEQ_OP", $3, lin, col); }
    | simple_expression LT_OP simple_expression
    { $$ = new BinaryOpNode($1, "LT_OP", $3, lin, col); }
    | simple_expression LTE_OP simple_expression
    { $$ = new BinaryOpNode($1, "LTE_OP", $3, lin, col); }
    | simple_expression GT_OP simple_expression
    { $$ = new BinaryOpNode($1, "GT_OP", $3, lin, col); }
    | simple_expression GTE_OP simple_expression
    { $$ = new BinaryOpNode($1, "GTE_OP", $3, lin, col); }
    ;

simple_expression: term
    { $$ = $1; }
    | simple_expression '+' term
    { $$ = new BinaryOpNode($1, "+", $3, lin, col); }
    | simple_expression '-' term
    { $$ = new BinaryOpNode($1, "-", $3, lin, col); }
    | simple_expression OR_OP term 
    { $$ = new BinaryOpNode($1, "OR_OP", $3, lin, col); }
    ;

term: factor
    { $$ = $1; }
    | term '*' factor
    { $$ = new BinaryOpNode($1, "*", $3, lin, col); }
    | term '/' factor
    { $$ = new BinaryOpNode($1, "/", $3, lin, col); }
    | term DIV_OP factor
    { $$ = new BinaryOpNode($1, "DIV_OP", $3, lin, col); }
    | term AND_OP factor 
    { $$ = new BinaryOpNode($1, "AND_OP", $3, lin, col); }
    ;

factor: primary
    { $$ = $1; }
    | '-' factor %prec UMINUS
    { $$ = new UnaryOpNode("-", $2, lin, col); }
    | NOT_OP factor 
    { $$ = new UnaryOpNode("NOT_OP", $2, lin, col); }
    ;

primary: id_node
    { $$ = new IdExprNode($1, lin, col); } 
    | int_num_node
    { $$ = $1; } 
    | real_num_node
    { $$ = $1; } 
    | TRUE_KEYWORD
    { $$ = new BooleanLiteralNode(true, lin, col); }
    | FALSE_KEYWORD
    { $$ = new BooleanLiteralNode(false, lin, col); }
    | id_node '(' expression_list ')' 
    { $$ = new FunctionCallExprNode($1, $3, lin, col); }
    | '(' expression ')'
    { $$ = $2; } 
    ;

%% 

int yyerror(const char* s) {
    std::cout << "SYNTAX ERROR: " << s << " at line: " << lin << ", Column: " << col << std::endl;
    return 0;
}
