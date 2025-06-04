%{
    #include "ast.h"
    #include "parser.h" 
    #include <iostream>
    #include <string>
    #include <cstdlib>
    #include <cstring>   // For strncpy
    #include <cstdio>
    #include <io.h>

    int lin = 1;
    int col = 1;
    int comment_start_lin = 0;
    int comment_start_col = 0;
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
    ".."                { col += yyleng; return DOTDOT; } // Placed BEFORE numbers and single dot

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
    
    /* --- Literals --- */
    /* Real literals - Using the refined patterns with a single action block. */
    {DIGIT}+\.{DIGIT}+([eE][-+]?{DIGIT}+)? |
    {DIGIT}+\.([eE][-+]?{DIGIT}+)        |
    \.{DIGIT}+([eE][-+]?{DIGIT}+)?        |
    {DIGIT}+[eE][-+]?{DIGIT}+ { /* <<< ACTION BLOCK'S OPENING BRACE IS HERE, ON THE SAME LINE */
        int token_start_col = col; // Capture start column for the AST node
        yylval.rawRealLit = new RealLit(atof(yytext), lin, token_start_col);
        col += yyleng; // Update global column position
        return REAL_LITERAL;
    }


    /* Integer Literal - This rule will now correctly grab "1" from "1.." */
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

    /* Single Character Operators & Punctuation - DOTDOT is already handled above */
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
                          std::cerr << "Lexical Error: Unexpected character '" << *yytext
                                    << "' at line " << lin << ", column " << col << std::endl;
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
/* C code section */