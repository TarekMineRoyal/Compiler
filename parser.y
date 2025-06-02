%{
    #include "ast.h"
    #include <iostream>
    // using std::endl; // Already in your original file
    // using std::cout; // Already in your original file
    extern int yylex();
    extern int yyerror(const char*); // yyerror is defined at the bottom of this file
    extern int yydebug;

    // lin and col are defined in lexer.l and used here for AST node creation and error reporting
    extern int lin;
    extern int col;

    Func *root; // Root of the AST
%}

%union {
    Func *tFunc;
    Args *tArgs;
    Arg *tArg;
    Expr *tExpr;
    Stmts *tStmts;
    Stmt *tStmt;
    Num *tNum;         // For integer literals (e.g., 123)
    RealLit *realLit;  // For real literals (e.g., 3.14) (NEW)
    Ident *tIdent;     // For identifiers
    int tInt;          // For type specifiers (e.g., an enum for integer, real)
}

/* Token declarations */
%token <tNum> NUM                // Integer literal (e.g., 123)
%token <realLit> REAL_LITERAL   // Real literal (e.g., 1.23, 4E-2) (NEW)
%token <tIdent> IDENT            // Identifier

/* MiniPascal Keywords (NEW) */
%token PROGRAM VAR INTEGER_TYPE REAL_TYPE FUNCTION PROCEDURE
%token WHILE DO BEGIN_TOKEN END_TOKEN IF THEN ELSE ARRAY OF
%token DIV_OP NOT_OP OR_OP AND_OP

/* Multi-character operators and other punctuation (NEW or revised) */
%token ASSIGN_OP  /* := */
%token EQ_OP      /* =  (equality comparison) */
%token NEQ_OP     /* <> */
%token LT_OP      /* <  */
%token LTE_OP     /* <= */
%token GT_OP      /* >  */
%token GTE_OP     /* >= */

/* Tokens from your C-style example (IF, ELSE are also keywords) */
/* %token RETURN */ /* RETURN is not in MiniPascal keyword list, remove if not needed */

/* Type specifiers for non-terminals */
%type <tFunc> func
%type <tArgs> args args_e
%type <tArg> arg
%type <tStmts> stmts /* Note: your old grammar had stmts', ensure consistency or pick one */
%type <tStmt> stmt
%type <tExpr> expr
%type <tInt> type  /* This 'type' non-terminal resolves to an integer representing the type */

/* Operator Precedence and Associativity (from your C-style example, may need adjustment for MiniPascal) */
%nonassoc IF_PREC
%nonassoc ELSE

%left '+' '-' // Example, will need MiniPascal operators
%left '*' '/' // Example, will need MiniPascal operators (DIV_OP, regular /)
%left UNARY_OP // Example for unary minus


%% /* Grammar Rules will start here */

/* Your existing C-style grammar rules (will need to be replaced/adapted for MiniPascal) */
/* For now, just keeping the structure and your existing 'func' as a placeholder start */

func : type IDENT '(' args_e ')' '{' stmts '}' /* C-style, not MiniPascal */
    {
        // Example of using lin, col from lexer for the Func node
        $$ = new Func($1, $2, $4, $7, lin, col); // lin, col would be current pos after '}'
                                                 // or better, capture @N positions if using Bison locations
        root = $$;
        // std::cout << "Parsed a function!" << std::endl;
    }
;

arg : type IDENT
    {
        $$ = new Arg($1, $2, lin, col); // Using global lin/col
    }
;

args: arg
    {
        $$ = new Args($1, lin, col); // Using global lin/col
    }
    | args ',' arg
    {
        $$ = $1;
        $$->AddArg($3);
    }
;

args_e: /* empty */
    {
        $$ = new Args(lin, col); // Using global lin/col
    }
    | args
    {
        $$ = $1;
    }
;

type: INTEGER_TYPE /* Changed from INT to INTEGER_TYPE for MiniPascal */
    {
        $$ = 1; // Replace with actual type enum or constant for 'integer'
                // e.g., TYPE_INTEGER
    }
    | REAL_TYPE /* Added for MiniPascal */
    {
        $$ = 2; // Replace with actual type enum or constant for 'real'
                // e.g., TYPE_REAL
    }
;

expr: NUM
    {
        // $$ = $1; // If NUM directly maps to an Expr* subtype
        // Example: create a generic expression node or handle $1 (Num*)
        // std::cout << "expr -> NUM production reduced" << std::endl;
        // Assuming $1 (tNum which is Num*) can be assigned to tExpr (Expr*)
        $$ = $1;
    }
    | REAL_LITERAL // New rule for real literals
    {
        $$ = $1; // Assuming $1 (realLit which is RealLit*) can be assigned to tExpr (Expr*)
        // std::cout << "expr -> REAL_LITERAL production reduced" << std::endl;
    }
    | IDENT
    {
        $$ = new IdExpr($1, lin, col); // Using global lin/col
    }
    | '-' expr %prec UNARY_OP
    {
        $$ = new Minus($2, lin, col); // Using global lin/col
    }
    | expr '+' expr
    {
        $$ = new Add($1, $3, lin, col); // Using global lin/col
    }
    // ... more MiniPascal expressions (boolean, relational, etc.) will be added here
;

stmt: IDENT ASSIGN_OP expr ';' /* MiniPascal assignment: id := expr ; */
    {
        $$ = new Assign($1, $3, lin, col); // Using global lin/col
    }
    | type IDENT ';' /* Variable declaration, but MiniPascal has a VAR section */
                     /* This rule might be part of a different non-terminal in full MiniPascal */
    {
        $$ = new VarDecl($1, $2, lin, col); // Using global lin/col
    }
    | IF '(' expr ')' stmt %prec IF_PREC /* C-style if, needs adaptation */
    {
        // Placeholder for IF statement construction
        // $$ = new IfNode($3, $5, nullptr, lin, col);
    }
    | IF '(' expr ')' stmt ELSE stmt /* C-style if-else, needs adaptation */
    {
        // Placeholder for IF-ELSE statement construction
        // $$ = new IfNode($3, $5, $7, lin, col);
    }
    | '{' stmts '}' /* C-style block, MiniPascal uses BEGIN END */
    {
        // $$ = $2; // If stmts directly returns a Stmts* compatible with Stmt* (if Stmts is a Stmt)
                   // Or, wrap $2 in a BlockStmt node if Stmts is a list container
    }
    // ... more MiniPascal statements (WHILE, FOR, BEGIN_END blocks, etc.) will be added here
;

stmts: /* Empty */
    {
        $$ = new Stmts(lin, col); // Using global lin/col for an empty list
    }
    | stmts stmt
    {
        $$ = $1;
        if ($2) $$->AddStmt($2); // AddStmt should handle null if necessary
    }
;

%% /* C Code Section */

int yyerror(const char* s) {
    // lin and col here are (hopefully) the location of the token that caused the error
    std::cout << "SYNTAX ERROR: " << s << " at line: " << lin << ", Column: " << col << std::endl;
    // You might want to print yytext if available and relevant
    return 0;
}