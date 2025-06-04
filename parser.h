/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 1
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NUM = 258,                     /* NUM  */
    REAL_LITERAL = 259,            /* REAL_LITERAL  */
    IDENT = 260,                   /* IDENT  */
    TRUE_KEYWORD = 261,            /* TRUE_KEYWORD  */
    FALSE_KEYWORD = 262,           /* FALSE_KEYWORD  */
    PROGRAM = 263,                 /* PROGRAM  */
    VAR = 264,                     /* VAR  */
    ARRAY = 265,                   /* ARRAY  */
    OF = 266,                      /* OF  */
    INTEGER_TYPE = 267,            /* INTEGER_TYPE  */
    REAL_TYPE = 268,               /* REAL_TYPE  */
    BOOLEAN_TYPE = 269,            /* BOOLEAN_TYPE  */
    FUNCTION = 270,                /* FUNCTION  */
    PROCEDURE = 271,               /* PROCEDURE  */
    BEGIN_TOKEN = 272,             /* BEGIN_TOKEN  */
    END_TOKEN = 273,               /* END_TOKEN  */
    IF = 274,                      /* IF  */
    THEN = 275,                    /* THEN  */
    ELSE = 276,                    /* ELSE  */
    WHILE = 277,                   /* WHILE  */
    DO = 278,                      /* DO  */
    NOT_OP = 279,                  /* NOT_OP  */
    AND_OP = 280,                  /* AND_OP  */
    OR_OP = 281,                   /* OR_OP  */
    DIV_OP = 282,                  /* DIV_OP  */
    ASSIGN_OP = 283,               /* ASSIGN_OP  */
    EQ_OP = 284,                   /* EQ_OP  */
    NEQ_OP = 285,                  /* NEQ_OP  */
    LT_OP = 286,                   /* LT_OP  */
    LTE_OP = 287,                  /* LTE_OP  */
    GT_OP = 288,                   /* GT_OP  */
    GTE_OP = 289,                  /* GTE_OP  */
    DOTDOT = 290,                  /* DOTDOT  */
    STRING_LITERAL = 291,          /* STRING_LITERAL  */
    UMINUS = 292                   /* UMINUS  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 18 "parser.y"

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
    StringLiteralNode* pStringLiteralNode;

    Num* rawNum;
    RealLit* rawRealLit;
    Ident* rawIdent;

    int token_val;
    char* str_val;

#line 137 "parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_PARSER_H_INCLUDED  */
