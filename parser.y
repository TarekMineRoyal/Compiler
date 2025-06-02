%{
    #include "ast.h"
    #include <iostream>
    using std::endl;
    using std::cout;
    extern int yylex();
    extern int yyerror(const char*);
    extern int yydebug;
    extern int lin, col;
    Func *root;
%}

%union{
    Func *tFunc;
    Args *tArgs;
    Arg *tArg;
    Expr *tExpr;
    Stmts *tStmts;
    Stmt *tStmt;
    Num *tNum;
    Ident *tIdent;
    int tInt;
}

%token <tNum> NUM
%token <tIdent> IDENT 
%token INT 
%type <tFunc> func
%type <tArgs> args args_e
%type <tArg> arg
%type <tStmts> stmts
%type <tStmt> stmt
%type <tExpr> expr
%type <tInt> type

%token IF
%token ELSE
%token RETURN

%nonassoc IF_PREC
%nonassoc ELSE

%left '+' 
%left '*'
%left UNARY_OP
%nonassoc PLUS


%%

func : type IDENT '(' args_e ')' '{' stmts'}'
    {
        $$ = new Func($1, $2, $4, $7, lin, col);
        root = $$;
    }
;
arg : type IDENT
    {
        $$ = new Arg($1, $2, lin, col);
    }
;
args: arg
    {
        $$ = new Args($1, lin, col);
    }
    | args ',' arg
    {
        $$ = $1;
        $$->AddArg($3);
    }
;
args_e: /* empty */
    {
        $$ = new Args(lin, col);
    }
    | args
    {
        $$ = $1;
    }
;
type: INT
    {
        $$ = 1;
    }
;

expr: NUM 
    {
        cout << "expr -> NUM production us reduced" << endl;
    }
    | IDENT
    {
        $$ = new IdExpr($1, lin, col);
    }
    | '-' expr %prec UNARY_OP
    {

    }
    | expr '+' expr
    {

    }
;
stmt: IDENT '=' expr ';'
    {
        $$ = new Assign($1, $3, lin,col);
    }
    | type IDENT ';' 
    {
        $$ = new VarDecl($1, $2, lin, col);
    }
    | IF '(' expr ')' stmt %prec IF_PREC
    {}
    | IF '(' expr ')' stmt ELSE stmt
    {}
    | '{' stmts '}'
    {}
;
stmts: /* Empty */
    {}
    |stmts stmt
    {}
;

%%

int yyerror(const char* s){
    cout << "SYNTAX ERROR: " << s  << " line: "<<lin<<" Column: " <<col << endl;
    return 0; 
}
