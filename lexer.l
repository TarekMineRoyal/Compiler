%{
    #include "ast.h"
    #include "parser.h" 
    #include <iostream>
    #include <string>
    #include <cstdlib>
    #include <cstring>
    #include <cstdio>
    #include <io.h>

    int lin = 1;
    int col = 1;
    int comment_start_lin = 0;
    int comment_start_col = 0;

    // An external reference to the global error flag
    extern bool compilation_has_error;

    // A helper function to help with generating the tokens.txt file.
    const char* token_to_string(int token, const YYSTYPE& lval) {
        switch (token) {
            // Keywords
            case PROGRAM: return "PROGRAM";
            case VAR: return "VAR";
            case ARRAY: return "ARRAY";
            case OF: return "OF";
            case INTEGER_TYPE: return "INTEGER_TYPE";
            case REAL_TYPE: return "REAL_TYPE";
            case BOOLEAN_TYPE: return "BOOLEAN_TYPE";
            case FUNCTION: return "FUNCTION";
            case PROCEDURE: return "PROCEDURE";
            case BEGIN_TOKEN: return "BEGIN_TOKEN";
            case END_TOKEN: return "END_TOKEN";
            case IF: return "IF";
            case THEN: return "THEN";
            case ELSE: return "ELSE";
            case WHILE: return "WHILE";
            case DO: return "DO";
            case NOT_OP: return "NOT_OP";
            case AND_OP: return "AND_OP";
            case OR_OP: return "OR_OP";
            case DIV_OP: return "DIV_OP";
            case RETURN_KEYWORD: return "RETURN_KEYWORD";
            case TRUE_KEYWORD: return "TRUE_KEYWORD";
            case FALSE_KEYWORD: return "FALSE_KEYWORD";

            // Literals
            case NUM: return "NUM";
            case REAL_LITERAL: return "REAL_LITERAL";
            case IDENT: return "IDENT";
            case STRING_LITERAL: return "STRING_LITERAL";
            
            // Operators & Punctuation
            case ASSIGN_OP: return "ASSIGN_OP";
            case EQ_OP: return "EQ_OP";
            case NEQ_OP: return "NEQ_OP";
            case LT_OP: return "LT_OP";
            case LTE_OP: return "LTE_OP";
            case GT_OP: return "GT_OP";
            case GTE_OP: return "GTE_OP";
            case DOTDOT: return "DOTDOT";
            
            // Single characters are their own representation
            default:
                if (token >= 0 && token <= 255) {
                    static char buffer[2] = {0};
                    buffer[0] = (char)token;
                    return buffer;
                }
                return "UNKNOWN_TOKEN";
        }
    }

%}

%option caseless noyywrap nounistd never-interactive stack
%x ML_COMMENT

DIGIT   [0-9]
ALPHA   [a-zA-Z]

%%

<INITIAL>{
    /* Whitespace */
    [ \t]+              { col += yyleng; }
    \r\n|\n|\r          { lin++; col = 1; }

    /* Comments */
    "//"[^\n\r]* { col += yyleng; }
    "{"                 {
                          comment_start_lin = lin;
                          comment_start_col = col;
                          col += yyleng;
                          BEGIN(ML_COMMENT);
                        }
    /* String Literals */
    \'([^'\n\\]|\\.)*\'  { 
                          char* s_val = (char*)malloc(yyleng - 1); 
                          if (s_val) {
                            strncpy(s_val, yytext + 1, yyleng - 2);
                            s_val[yyleng - 2] = '\0';
                            yylval.str_val = s_val;
                          } else {
                            fprintf(stderr, "Lexer: Malloc failed for string literal at line %d, col %d\n", lin, col);
                            yyterminate();
                          }
                          col += yyleng; 
                          return STRING_LITERAL;
                        }

    /* --- OPERATORS - Specific multi-character operators first --- */
    ":="                { col += yyleng; return ASSIGN_OP; }
    "<>"                { col += yyleng; return NEQ_OP; }
    "<="                { col += yyleng; return LTE_OP; }
    ">="                { col += yyleng; return GTE_OP; }
    ".."                { col += yyleng; return DOTDOT; }

    /* Keywords */
    "program"           { col += yyleng; return PROGRAM; }
    "var"               { col += yyleng; return VAR; }
    "integer"           { col += yyleng; return INTEGER_TYPE; }
    "real"              { col += yyleng; return REAL_TYPE; }
    "function"          { col += yyleng; return FUNCTION; }
    "procedure"         { col += yyleng; return PROCEDURE; }
    "while"             { col += yyleng; return WHILE; }
    "do"                { col += yyleng; return DO; }
    "begin"             { col += yyleng; return BEGIN_TOKEN; }
    "end"               { col += yyleng; return END_TOKEN; }
    "if"                { col += yyleng; return IF; }
    "then"              { col += yyleng; return THEN; }
    "else"              { col += yyleng; return ELSE; }
    "array"             { col += yyleng; return ARRAY; }
    "of"                { col += yyleng; return OF; }
    "div"               { col += yyleng; return DIV_OP; }
    "not"               { col += yyleng; return NOT_OP; }
    "or"                { col += yyleng; return OR_OP; }
    "and"               { col += yyleng; return AND_OP; }
    "boolean"           { col += yyleng; return BOOLEAN_TYPE; }
    "true"              { col += yyleng; return TRUE_KEYWORD; }
    "false"             { col += yyleng; return FALSE_KEYWORD; }
    "return"            { col += yyleng; return RETURN_KEYWORD; }
    
    /* --- Literals --- */
    /* Real literals */
    {DIGIT}+\.{DIGIT}+([eE][-+]?{DIGIT}+)? |
    {DIGIT}+\.([eE][-+]?{DIGIT}+)        |
    \.{DIGIT}+([eE][-+]?{DIGIT}+)?        |
    {DIGIT}+[eE][-+]?{DIGIT}+ { /* <<< ACTION BLOCK'S OPENING BRACE IS HERE, ON THE SAME LINE */
        int token_start_col = col; // Capture start column for the AST node
        yylval.rawRealLit = new RealLit(atof(yytext), lin, token_start_col);
        col += yyleng; // Update global column position
        return REAL_LITERAL;
    }


    /* Integer Literal */
    {DIGIT}+ { 
        int token_start_col = col; // Capture start column
        yylval.rawNum = new Num(atoi(yytext), lin, token_start_col);
        col+= yyleng; // Update global column position
        return NUM; 
    }

    /* Identifiers - after keywords */
    ({ALPHA}|_)({ALPHA}|{DIGIT}|_)* {
        int token_start_col = col; // Capture start column
        yylval.rawIdent = new Ident(std::string(yytext), lin, token_start_col);
        col += yyleng; // Update global column position
        return IDENT;
    }

    /* Single Character Operators & Punctuation */
    "+"                 { col += yyleng; return '+'; }
    "-"                 { col += yyleng; return '-'; }
    "*"                 { col += yyleng; return '*'; }
    "/"                 { col += yyleng; return '/'; }
    "="                 { col += yyleng; return EQ_OP; }
    "<"                 { col += yyleng; return LT_OP; }
    ">"                 { col += yyleng; return GT_OP; }
    "("                 { col += yyleng; return '('; }
    ")"                 { col += yyleng; return ')'; }
    "["                 { col += yyleng; return '['; }
    "]"                 { col += yyleng; return ']'; }
    ":"                 { col += yyleng; return ':'; }
    ";"                 { col += yyleng; return ';'; }
    ","                 { col += yyleng; return ','; }
    "."                 { col += yyleng; return '.'; } /* Single dot, AFTER DOTDOT */


    .                   {
                          // Standardized error format
                          fprintf(stderr, "Lexical Error (L:%d, C:%d): Unexpected character '%c'\n", lin, col, *yytext);
                          col += yyleng;
                        }
}

<ML_COMMENT>{
    "}"                 { col += yyleng; BEGIN(INITIAL); }
    [^{}\n\r]+          { col += yyleng; }
    \r\n|\n|\r          { lin++; col = 1; }
    "{"                 {
                          std::cerr << "Warning: Nested comment '{' at L" << lin << ", C" << col << std::endl;
                          col += yyleng;
                        }
    <<EOF>>             {
                          std::cerr << "Lexical Error: Unterminated comment from L" << comment_start_lin << ", C" << comment_start_col << std::endl;
                          BEGIN(INITIAL);
                          yyterminate();
                        }
}

%%