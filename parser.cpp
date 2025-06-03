/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser.y"

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

#line 88 "parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_NUM = 3,                        /* NUM  */
  YYSYMBOL_REAL_LITERAL = 4,               /* REAL_LITERAL  */
  YYSYMBOL_IDENT = 5,                      /* IDENT  */
  YYSYMBOL_TRUE_KEYWORD = 6,               /* TRUE_KEYWORD  */
  YYSYMBOL_FALSE_KEYWORD = 7,              /* FALSE_KEYWORD  */
  YYSYMBOL_PROGRAM = 8,                    /* PROGRAM  */
  YYSYMBOL_VAR = 9,                        /* VAR  */
  YYSYMBOL_ARRAY = 10,                     /* ARRAY  */
  YYSYMBOL_OF = 11,                        /* OF  */
  YYSYMBOL_INTEGER_TYPE = 12,              /* INTEGER_TYPE  */
  YYSYMBOL_REAL_TYPE = 13,                 /* REAL_TYPE  */
  YYSYMBOL_BOOLEAN_TYPE = 14,              /* BOOLEAN_TYPE  */
  YYSYMBOL_FUNCTION = 15,                  /* FUNCTION  */
  YYSYMBOL_PROCEDURE = 16,                 /* PROCEDURE  */
  YYSYMBOL_BEGIN_TOKEN = 17,               /* BEGIN_TOKEN  */
  YYSYMBOL_END_TOKEN = 18,                 /* END_TOKEN  */
  YYSYMBOL_IF = 19,                        /* IF  */
  YYSYMBOL_THEN = 20,                      /* THEN  */
  YYSYMBOL_ELSE = 21,                      /* ELSE  */
  YYSYMBOL_WHILE = 22,                     /* WHILE  */
  YYSYMBOL_DO = 23,                        /* DO  */
  YYSYMBOL_NOT_OP = 24,                    /* NOT_OP  */
  YYSYMBOL_AND_OP = 25,                    /* AND_OP  */
  YYSYMBOL_OR_OP = 26,                     /* OR_OP  */
  YYSYMBOL_DIV_OP = 27,                    /* DIV_OP  */
  YYSYMBOL_ASSIGN_OP = 28,                 /* ASSIGN_OP  */
  YYSYMBOL_EQ_OP = 29,                     /* EQ_OP  */
  YYSYMBOL_NEQ_OP = 30,                    /* NEQ_OP  */
  YYSYMBOL_LT_OP = 31,                     /* LT_OP  */
  YYSYMBOL_LTE_OP = 32,                    /* LTE_OP  */
  YYSYMBOL_GT_OP = 33,                     /* GT_OP  */
  YYSYMBOL_GTE_OP = 34,                    /* GTE_OP  */
  YYSYMBOL_DOTDOT = 35,                    /* DOTDOT  */
  YYSYMBOL_36_ = 36,                       /* '+'  */
  YYSYMBOL_37_ = 37,                       /* '-'  */
  YYSYMBOL_38_ = 38,                       /* '*'  */
  YYSYMBOL_39_ = 39,                       /* '/'  */
  YYSYMBOL_UMINUS = 40,                    /* UMINUS  */
  YYSYMBOL_41_ = 41,                       /* ';'  */
  YYSYMBOL_42_ = 42,                       /* '.'  */
  YYSYMBOL_43_ = 43,                       /* ','  */
  YYSYMBOL_44_ = 44,                       /* ':'  */
  YYSYMBOL_45_ = 45,                       /* '['  */
  YYSYMBOL_46_ = 46,                       /* ']'  */
  YYSYMBOL_47_ = 47,                       /* '('  */
  YYSYMBOL_48_ = 48,                       /* ')'  */
  YYSYMBOL_YYACCEPT = 49,                  /* $accept  */
  YYSYMBOL_program_rule = 50,              /* program_rule  */
  YYSYMBOL_id_node = 51,                   /* id_node  */
  YYSYMBOL_identifier_list = 52,           /* identifier_list  */
  YYSYMBOL_declarations = 53,              /* declarations  */
  YYSYMBOL_var_declaration_list_non_empty = 54, /* var_declaration_list_non_empty  */
  YYSYMBOL_var_declaration_item = 55,      /* var_declaration_item  */
  YYSYMBOL_type = 56,                      /* type  */
  YYSYMBOL_int_num_node = 57,              /* int_num_node  */
  YYSYMBOL_real_num_node = 58,             /* real_num_node  */
  YYSYMBOL_standard_type = 59,             /* standard_type  */
  YYSYMBOL_subprogram_declarations = 60,   /* subprogram_declarations  */
  YYSYMBOL_subprogram_declaration_block = 61, /* subprogram_declaration_block  */
  YYSYMBOL_subprogram_declaration = 62,    /* subprogram_declaration  */
  YYSYMBOL_subprogram_head = 63,           /* subprogram_head  */
  YYSYMBOL_arguments = 64,                 /* arguments  */
  YYSYMBOL_parameter_list = 65,            /* parameter_list  */
  YYSYMBOL_parameter_declaration_group = 66, /* parameter_declaration_group  */
  YYSYMBOL_compound_statement = 67,        /* compound_statement  */
  YYSYMBOL_optional_statements = 68,       /* optional_statements  */
  YYSYMBOL_statement_list_terminated = 69, /* statement_list_terminated  */
  YYSYMBOL_statement_list = 70,            /* statement_list  */
  YYSYMBOL_statement = 71,                 /* statement  */
  YYSYMBOL_variable = 72,                  /* variable  */
  YYSYMBOL_procedure_statement = 73,       /* procedure_statement  */
  YYSYMBOL_expression_list = 74,           /* expression_list  */
  YYSYMBOL_expr = 75,                      /* expr  */
  YYSYMBOL_logical_or_expr = 76,           /* logical_or_expr  */
  YYSYMBOL_logical_and_expr = 77,          /* logical_and_expr  */
  YYSYMBOL_not_expr = 78,                  /* not_expr  */
  YYSYMBOL_relational_expr = 79,           /* relational_expr  */
  YYSYMBOL_additive_expr = 80,             /* additive_expr  */
  YYSYMBOL_multiplicative_expr = 81,       /* multiplicative_expr  */
  YYSYMBOL_unary_expr = 82,                /* unary_expr  */
  YYSYMBOL_primary = 83                    /* primary  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  5
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   138

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  49
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  35
/* YYNRULES -- Number of rules.  */
#define YYNRULES  77
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  142

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   291


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
      47,    48,    38,    36,    43,    37,    42,    39,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    44,    41,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    45,     2,    46,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    40
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   114,   114,   118,   122,   124,   129,   130,   134,   136,
     140,   144,   146,   150,   154,   158,   160,   162,   167,   168,
     172,   176,   180,   182,   187,   188,   192,   194,   198,   202,
     207,   208,   212,   214,   218,   220,   224,   226,   228,   230,
     232,   234,   238,   240,   244,   246,   250,   252,   260,   264,
     266,   270,   272,   278,   280,   287,   289,   291,   293,   295,
     297,   299,   303,   305,   307,   311,   313,   315,   317,   321,
     323,   329,   331,   333,   335,   337,   339,   341
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "NUM", "REAL_LITERAL",
  "IDENT", "TRUE_KEYWORD", "FALSE_KEYWORD", "PROGRAM", "VAR", "ARRAY",
  "OF", "INTEGER_TYPE", "REAL_TYPE", "BOOLEAN_TYPE", "FUNCTION",
  "PROCEDURE", "BEGIN_TOKEN", "END_TOKEN", "IF", "THEN", "ELSE", "WHILE",
  "DO", "NOT_OP", "AND_OP", "OR_OP", "DIV_OP", "ASSIGN_OP", "EQ_OP",
  "NEQ_OP", "LT_OP", "LTE_OP", "GT_OP", "GTE_OP", "DOTDOT", "'+'", "'-'",
  "'*'", "'/'", "UMINUS", "';'", "'.'", "','", "':'", "'['", "']'", "'('",
  "')'", "$accept", "program_rule", "id_node", "identifier_list",
  "declarations", "var_declaration_list_non_empty", "var_declaration_item",
  "type", "int_num_node", "real_num_node", "standard_type",
  "subprogram_declarations", "subprogram_declaration_block",
  "subprogram_declaration", "subprogram_head", "arguments",
  "parameter_list", "parameter_declaration_group", "compound_statement",
  "optional_statements", "statement_list_terminated", "statement_list",
  "statement", "variable", "procedure_statement", "expression_list",
  "expr", "logical_or_expr", "logical_and_expr", "not_expr",
  "relational_expr", "additive_expr", "multiplicative_expr", "unary_expr",
  "primary", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-73)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-43)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
       6,    22,    30,   -73,    -4,   -73,    32,    22,   -73,   -73,
       1,    22,   -73,    79,    22,    90,   -73,    22,    22,    37,
     -73,    33,    38,    41,   -73,    39,   -73,   -73,   -73,    60,
     -73,    58,    58,    16,    16,   -19,   -73,    88,   -73,    73,
     -73,    87,   -73,   -73,   -73,   -73,   113,   -73,    22,    74,
      76,   -73,   -73,   -73,   -73,    16,    45,    16,    72,   -73,
     -73,   100,    95,    97,   -73,    48,   -25,    19,   -73,   -73,
     101,    16,    16,   -73,    37,    16,    91,    18,   -17,   -73,
      96,   -73,   -73,   -73,    75,    16,    37,    16,    16,    29,
      29,    29,    29,    29,    29,    29,    29,    29,    29,    29,
      37,    81,    27,   -73,   -73,   -73,   113,    90,    22,   -73,
      84,   -73,    50,   107,    97,   -73,   -25,   -25,   -25,   -25,
     -25,   -25,    19,    19,   -73,   -73,   -73,   -73,   -73,    16,
     -73,    83,   -73,   -73,   -73,   -73,    37,   -73,   119,   -73,
      96,   -73
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     3,     0,     1,     6,     0,    18,     4,
       0,     7,     8,     0,     0,     0,     9,     0,     0,    30,
      19,     0,     0,     0,     5,     0,    15,    16,    17,     0,
      11,    24,    24,     0,     0,    44,    38,     0,    31,    32,
      34,     0,    37,    20,    21,     2,     0,    10,     0,     0,
       0,    13,    14,    74,    75,     0,     0,     0,    71,    72,
      73,     0,    48,    49,    51,    53,    55,    62,    65,    69,
       0,     0,     0,    29,    33,     0,     0,     0,     0,    26,
       0,    23,    54,    70,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    46,    35,    36,     0,     0,     0,    25,
       0,    77,     0,    39,    50,    52,    56,    57,    58,    59,
      60,    61,    63,    64,    68,    66,    67,    41,    43,     0,
      45,     0,    28,    27,    22,    76,     0,    47,     0,    40,
       0,    12
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -73,   -73,    -1,   -44,   -73,   -73,   120,    25,   -39,   -73,
     -72,   -73,   -73,   -73,   -73,   102,   -73,    28,    47,   -73,
     -73,   -73,   -71,   -73,   -73,    52,   -32,   -73,    46,   -50,
     -73,    -3,   -24,    14,    82
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     2,    58,    10,     8,    11,    12,    29,    59,    60,
      30,    13,    20,    21,    22,    49,    78,    79,    36,    37,
      38,    39,    40,    41,    42,   102,   103,    62,    63,    64,
      65,    66,    67,    68,    69
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
       4,    61,    70,   104,    77,    82,     9,    76,   110,   -42,
       9,    95,    96,    24,     1,   113,    31,    32,    35,    51,
      52,     3,    53,    54,   108,    84,    71,     3,    72,   127,
       5,   109,    51,    52,     3,    53,    54,     6,   115,   101,
      55,     7,     3,   105,    14,    15,    97,     9,    51,    52,
       3,    53,    54,    56,    19,    19,    33,    98,    99,    34,
      23,    14,   107,    57,    77,   139,    56,   131,   141,    44,
     129,   122,   123,    35,    43,   130,    57,    89,    90,    91,
      92,    93,    94,    45,    46,    35,   116,   117,   118,   119,
     120,   121,    57,   129,    17,    18,    19,   137,   135,    35,
      25,    47,    26,    27,    28,    48,    73,     9,    26,    27,
      28,   124,   125,   126,    74,    75,    51,    81,    80,    85,
      86,    87,    88,   111,   100,   134,   106,   128,   136,   138,
     140,    16,   132,   114,    50,    35,   133,   112,    83
};

static const yytype_uint8 yycheck[] =
{
       1,    33,    34,    74,    48,    55,     7,    46,    80,    28,
      11,    36,    37,    14,     8,    86,    17,    18,    19,     3,
       4,     5,     6,     7,    41,    57,    45,     5,    47,   100,
       0,    48,     3,     4,     5,     6,     7,    41,    88,    71,
      24,     9,     5,    75,    43,    44,    27,    48,     3,     4,
       5,     6,     7,    37,    17,    17,    19,    38,    39,    22,
      13,    43,    44,    47,   108,   136,    37,   106,   140,    22,
      43,    95,    96,    74,    41,    48,    47,    29,    30,    31,
      32,    33,    34,    42,    45,    86,    89,    90,    91,    92,
      93,    94,    47,    43,    15,    16,    17,   129,    48,   100,
      10,    41,    12,    13,    14,    47,    18,   108,    12,    13,
      14,    97,    98,    99,    41,    28,     3,    41,    44,    47,
      20,    26,    25,    48,    23,    41,    35,    46,    21,    46,
      11,    11,   107,    87,    32,   136,   108,    85,    56
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     8,    50,     5,    51,     0,    41,     9,    53,    51,
      52,    54,    55,    60,    43,    44,    55,    15,    16,    17,
      61,    62,    63,    67,    51,    10,    12,    13,    14,    56,
      59,    51,    51,    19,    22,    51,    67,    68,    69,    70,
      71,    72,    73,    41,    67,    42,    45,    41,    47,    64,
      64,     3,     4,     6,     7,    24,    37,    47,    51,    57,
      58,    75,    76,    77,    78,    79,    80,    81,    82,    83,
      75,    45,    47,    18,    41,    28,    57,    52,    65,    66,
      44,    41,    78,    83,    75,    47,    20,    26,    25,    29,
      30,    31,    32,    33,    34,    36,    37,    27,    38,    39,
      23,    75,    74,    75,    71,    75,    35,    44,    41,    48,
      59,    48,    74,    71,    77,    78,    80,    80,    80,    80,
      80,    80,    81,    81,    82,    82,    82,    71,    46,    43,
      48,    57,    56,    66,    41,    48,    21,    75,    46,    71,
      11,    59
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    49,    50,    51,    52,    52,    53,    53,    54,    54,
      55,    56,    56,    57,    58,    59,    59,    59,    60,    60,
      61,    62,    63,    63,    64,    64,    65,    65,    66,    67,
      68,    68,    69,    69,    70,    70,    71,    71,    71,    71,
      71,    71,    72,    72,    73,    73,    74,    74,    75,    76,
      76,    77,    77,    78,    78,    79,    79,    79,    79,    79,
      79,    79,    80,    80,    80,    81,    81,    81,    81,    82,
      82,    83,    83,    83,    83,    83,    83,    83
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     7,     1,     1,     3,     0,     2,     1,     2,
       4,     1,     8,     1,     1,     1,     1,     1,     0,     2,
       2,     2,     6,     4,     0,     3,     1,     3,     3,     3,
       0,     1,     1,     2,     1,     3,     3,     1,     1,     4,
       6,     4,     1,     4,     1,     4,     1,     3,     1,     1,
       3,     1,     3,     1,     2,     1,     3,     3,     3,     3,
       3,     3,     1,     3,     3,     1,     3,     3,     3,     1,
       2,     1,     1,     1,     1,     1,     4,     3
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)




# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  yy_symbol_value_print (yyo, yykind, yyvaluep);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)]);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep)
{
  YY_USE (yyvaluep);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program_rule: PROGRAM id_node ';' declarations subprogram_declarations compound_statement '.'  */
#line 115 "parser.y"
    { (yyval.pProgramNode) = new ProgramNode((yyvsp[-5].pIdentNode), (yyvsp[-3].pDeclarations), (yyvsp[-2].pSubprogramDeclarations), (yyvsp[-1].pCompoundStatementNode), lin, col); root_ast_node = (yyval.pProgramNode); }
#line 1259 "parser.cpp"
    break;

  case 3: /* id_node: IDENT  */
#line 119 "parser.y"
    { (yyval.pIdentNode) = new IdentNode((yyvsp[0].rawIdent)->name, (yyvsp[0].rawIdent)->line, (yyvsp[0].rawIdent)->column); delete (yyvsp[0].rawIdent); }
#line 1265 "parser.cpp"
    break;

  case 4: /* identifier_list: id_node  */
#line 123 "parser.y"
    { (yyval.pIdentifierList) = new IdentifierList((yyvsp[0].pIdentNode), lin, col); }
#line 1271 "parser.cpp"
    break;

  case 5: /* identifier_list: identifier_list ',' id_node  */
#line 125 "parser.y"
    { (yyvsp[-2].pIdentifierList)->addIdentifier((yyvsp[0].pIdentNode)); (yyval.pIdentifierList) = (yyvsp[-2].pIdentifierList); }
#line 1277 "parser.cpp"
    break;

  case 6: /* declarations: %empty  */
#line 129 "parser.y"
    { (yyval.pDeclarations) = new Declarations(lin, col); }
#line 1283 "parser.cpp"
    break;

  case 7: /* declarations: VAR var_declaration_list_non_empty  */
#line 131 "parser.y"
    { (yyval.pDeclarations) = (yyvsp[0].pDeclarations); }
#line 1289 "parser.cpp"
    break;

  case 8: /* var_declaration_list_non_empty: var_declaration_item  */
#line 135 "parser.y"
    { (yyval.pDeclarations) = new Declarations(lin, col); (yyval.pDeclarations)->addVarDecl((yyvsp[0].pVarDecl)); }
#line 1295 "parser.cpp"
    break;

  case 9: /* var_declaration_list_non_empty: var_declaration_list_non_empty var_declaration_item  */
#line 137 "parser.y"
    { (yyvsp[-1].pDeclarations)->addVarDecl((yyvsp[0].pVarDecl)); (yyval.pDeclarations) = (yyvsp[-1].pDeclarations); }
#line 1301 "parser.cpp"
    break;

  case 10: /* var_declaration_item: identifier_list ':' type ';'  */
#line 141 "parser.y"
    { (yyval.pVarDecl) = new VarDecl((yyvsp[-3].pIdentifierList), (yyvsp[-1].pTypeNode), lin, col); }
#line 1307 "parser.cpp"
    break;

  case 11: /* type: standard_type  */
#line 145 "parser.y"
    { (yyval.pTypeNode) = (yyvsp[0].pStandardTypeNode); }
#line 1313 "parser.cpp"
    break;

  case 12: /* type: ARRAY '[' int_num_node DOTDOT int_num_node ']' OF standard_type  */
#line 147 "parser.y"
    { (yyval.pTypeNode) = new ArrayTypeNode((yyvsp[-5].pIntNumNode), (yyvsp[-3].pIntNumNode), (yyvsp[0].pStandardTypeNode), lin, col); }
#line 1319 "parser.cpp"
    break;

  case 13: /* int_num_node: NUM  */
#line 151 "parser.y"
    { (yyval.pIntNumNode) = new IntNumNode((yyvsp[0].rawNum)->value, (yyvsp[0].rawNum)->line, (yyvsp[0].rawNum)->column); delete (yyvsp[0].rawNum); }
#line 1325 "parser.cpp"
    break;

  case 14: /* real_num_node: REAL_LITERAL  */
#line 155 "parser.y"
    { (yyval.pRealNumNode) = new RealNumNode((yyvsp[0].rawRealLit)->value, (yyvsp[0].rawRealLit)->line, (yyvsp[0].rawRealLit)->column); delete (yyvsp[0].rawRealLit); }
#line 1331 "parser.cpp"
    break;

  case 15: /* standard_type: INTEGER_TYPE  */
#line 159 "parser.y"
    { (yyval.pStandardTypeNode) = new StandardTypeNode(StandardTypeNode::TYPE_INTEGER, lin, col); }
#line 1337 "parser.cpp"
    break;

  case 16: /* standard_type: REAL_TYPE  */
#line 161 "parser.y"
    { (yyval.pStandardTypeNode) = new StandardTypeNode(StandardTypeNode::TYPE_REAL, lin, col); }
#line 1343 "parser.cpp"
    break;

  case 17: /* standard_type: BOOLEAN_TYPE  */
#line 163 "parser.y"
    { (yyval.pStandardTypeNode) = new StandardTypeNode(StandardTypeNode::TYPE_BOOLEAN, lin, col); }
#line 1349 "parser.cpp"
    break;

  case 18: /* subprogram_declarations: %empty  */
#line 167 "parser.y"
    { (yyval.pSubprogramDeclarations) = new SubprogramDeclarations(lin, col); }
#line 1355 "parser.cpp"
    break;

  case 19: /* subprogram_declarations: subprogram_declarations subprogram_declaration_block  */
#line 169 "parser.y"
    { (yyvsp[-1].pSubprogramDeclarations)->addSubprogramDeclaration((yyvsp[0].pSubprogramDeclaration)); (yyval.pSubprogramDeclarations) = (yyvsp[-1].pSubprogramDeclarations); }
#line 1361 "parser.cpp"
    break;

  case 20: /* subprogram_declaration_block: subprogram_declaration ';'  */
#line 173 "parser.y"
    { (yyval.pSubprogramDeclaration) = (yyvsp[-1].pSubprogramDeclaration); }
#line 1367 "parser.cpp"
    break;

  case 21: /* subprogram_declaration: subprogram_head compound_statement  */
#line 177 "parser.y"
    { (yyval.pSubprogramDeclaration) = new SubprogramDeclaration((yyvsp[-1].pSubprogramHead), (yyvsp[0].pCompoundStatementNode), lin, col); }
#line 1373 "parser.cpp"
    break;

  case 22: /* subprogram_head: FUNCTION id_node arguments ':' standard_type ';'  */
#line 181 "parser.y"
    { (yyval.pSubprogramHead) = new FunctionHeadNode((yyvsp[-4].pIdentNode), (yyvsp[-3].pArgumentsNode), (yyvsp[-1].pStandardTypeNode), lin, col); }
#line 1379 "parser.cpp"
    break;

  case 23: /* subprogram_head: PROCEDURE id_node arguments ';'  */
#line 183 "parser.y"
    { (yyval.pSubprogramHead) = new ProcedureHeadNode((yyvsp[-2].pIdentNode), (yyvsp[-1].pArgumentsNode), lin, col); }
#line 1385 "parser.cpp"
    break;

  case 24: /* arguments: %empty  */
#line 187 "parser.y"
    { (yyval.pArgumentsNode) = new ArgumentsNode(lin, col); }
#line 1391 "parser.cpp"
    break;

  case 25: /* arguments: '(' parameter_list ')'  */
#line 189 "parser.y"
    { (yyval.pArgumentsNode) = new ArgumentsNode((yyvsp[-1].pParameterList), lin, col); }
#line 1397 "parser.cpp"
    break;

  case 26: /* parameter_list: parameter_declaration_group  */
#line 193 "parser.y"
    { (yyval.pParameterList) = new ParameterList((yyvsp[0].pParameterDeclaration), lin, col); }
#line 1403 "parser.cpp"
    break;

  case 27: /* parameter_list: parameter_list ';' parameter_declaration_group  */
#line 195 "parser.y"
    { (yyvsp[-2].pParameterList)->addParameterDeclarationGroup((yyvsp[0].pParameterDeclaration)); (yyval.pParameterList) = (yyvsp[-2].pParameterList); }
#line 1409 "parser.cpp"
    break;

  case 28: /* parameter_declaration_group: identifier_list ':' type  */
#line 199 "parser.y"
    { (yyval.pParameterDeclaration) = new ParameterDeclaration((yyvsp[-2].pIdentifierList), (yyvsp[0].pTypeNode), lin, col); }
#line 1415 "parser.cpp"
    break;

  case 29: /* compound_statement: BEGIN_TOKEN optional_statements END_TOKEN  */
#line 203 "parser.y"
    { (yyval.pCompoundStatementNode) = new CompoundStatementNode((yyvsp[-1].pStatementList), lin, col); }
#line 1421 "parser.cpp"
    break;

  case 30: /* optional_statements: %empty  */
#line 207 "parser.y"
    { (yyval.pStatementList) = new StatementList(lin, col); }
#line 1427 "parser.cpp"
    break;

  case 31: /* optional_statements: statement_list_terminated  */
#line 209 "parser.y"
    { (yyval.pStatementList) = (yyvsp[0].pStatementList); }
#line 1433 "parser.cpp"
    break;

  case 32: /* statement_list_terminated: statement_list  */
#line 213 "parser.y"
    { (yyval.pStatementList) = (yyvsp[0].pStatementList); }
#line 1439 "parser.cpp"
    break;

  case 33: /* statement_list_terminated: statement_list ';'  */
#line 215 "parser.y"
    { (yyval.pStatementList) = (yyvsp[-1].pStatementList); }
#line 1445 "parser.cpp"
    break;

  case 34: /* statement_list: statement  */
#line 219 "parser.y"
    { (yyval.pStatementList) = new StatementList(lin, col); (yyval.pStatementList)->addStatement((yyvsp[0].pStatementNode)); }
#line 1451 "parser.cpp"
    break;

  case 35: /* statement_list: statement_list ';' statement  */
#line 221 "parser.y"
    { (yyvsp[-2].pStatementList)->addStatement((yyvsp[0].pStatementNode)); (yyval.pStatementList) = (yyvsp[-2].pStatementList); }
#line 1457 "parser.cpp"
    break;

  case 36: /* statement: variable ASSIGN_OP expr  */
#line 225 "parser.y"
    { (yyval.pStatementNode) = new AssignStatementNode((yyvsp[-2].pVariableNode), (yyvsp[0].pExprNode), lin, col); }
#line 1463 "parser.cpp"
    break;

  case 37: /* statement: procedure_statement  */
#line 227 "parser.y"
    { (yyval.pStatementNode) = (yyvsp[0].pProcedureCallStatementNode); }
#line 1469 "parser.cpp"
    break;

  case 38: /* statement: compound_statement  */
#line 229 "parser.y"
    { (yyval.pStatementNode) = (yyvsp[0].pCompoundStatementNode); }
#line 1475 "parser.cpp"
    break;

  case 39: /* statement: IF expr THEN statement  */
#line 231 "parser.y"
    { (yyval.pStatementNode) = new IfStatementNode((yyvsp[-2].pExprNode), (yyvsp[0].pStatementNode), nullptr, lin, col); }
#line 1481 "parser.cpp"
    break;

  case 40: /* statement: IF expr THEN statement ELSE statement  */
#line 233 "parser.y"
    { (yyval.pStatementNode) = new IfStatementNode((yyvsp[-4].pExprNode), (yyvsp[-2].pStatementNode), (yyvsp[0].pStatementNode), lin, col); }
#line 1487 "parser.cpp"
    break;

  case 41: /* statement: WHILE expr DO statement  */
#line 235 "parser.y"
    { (yyval.pStatementNode) = new WhileStatementNode((yyvsp[-2].pExprNode), (yyvsp[0].pStatementNode), lin, col); }
#line 1493 "parser.cpp"
    break;

  case 42: /* variable: id_node  */
#line 239 "parser.y"
    { (yyval.pVariableNode) = new VariableNode((yyvsp[0].pIdentNode), nullptr, lin, col); }
#line 1499 "parser.cpp"
    break;

  case 43: /* variable: id_node '[' expr ']'  */
#line 241 "parser.y"
    { (yyval.pVariableNode) = new VariableNode((yyvsp[-3].pIdentNode), (yyvsp[-1].pExprNode), lin, col); }
#line 1505 "parser.cpp"
    break;

  case 44: /* procedure_statement: id_node  */
#line 245 "parser.y"
    { (yyval.pProcedureCallStatementNode) = new ProcedureCallStatementNode((yyvsp[0].pIdentNode), new ExpressionList(lin,col), lin, col); }
#line 1511 "parser.cpp"
    break;

  case 45: /* procedure_statement: id_node '(' expression_list ')'  */
#line 247 "parser.y"
    { (yyval.pProcedureCallStatementNode) = new ProcedureCallStatementNode((yyvsp[-3].pIdentNode), (yyvsp[-1].pExpressionList), lin, col); }
#line 1517 "parser.cpp"
    break;

  case 46: /* expression_list: expr  */
#line 251 "parser.y"
    { (yyval.pExpressionList) = new ExpressionList(lin, col); (yyval.pExpressionList)->addExpression((yyvsp[0].pExprNode)); }
#line 1523 "parser.cpp"
    break;

  case 47: /* expression_list: expression_list ',' expr  */
#line 253 "parser.y"
    { (yyvsp[-2].pExpressionList)->addExpression((yyvsp[0].pExprNode)); (yyval.pExpressionList) = (yyvsp[-2].pExpressionList); }
#line 1529 "parser.cpp"
    break;

  case 48: /* expr: logical_or_expr  */
#line 261 "parser.y"
    { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1535 "parser.cpp"
    break;

  case 49: /* logical_or_expr: logical_and_expr  */
#line 265 "parser.y"
                 { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1541 "parser.cpp"
    break;

  case 50: /* logical_or_expr: logical_or_expr OR_OP logical_and_expr  */
#line 267 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "OR_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1547 "parser.cpp"
    break;

  case 51: /* logical_and_expr: not_expr  */
#line 271 "parser.y"
                  { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1553 "parser.cpp"
    break;

  case 52: /* logical_and_expr: logical_and_expr AND_OP not_expr  */
#line 273 "parser.y"
                  { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "AND_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1559 "parser.cpp"
    break;

  case 53: /* not_expr: relational_expr  */
#line 279 "parser.y"
          { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1565 "parser.cpp"
    break;

  case 54: /* not_expr: NOT_OP not_expr  */
#line 284 "parser.y"
          { (yyval.pExprNode) = new UnaryOpNode("NOT_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1571 "parser.cpp"
    break;

  case 55: /* relational_expr: additive_expr  */
#line 288 "parser.y"
                 { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1577 "parser.cpp"
    break;

  case 56: /* relational_expr: relational_expr EQ_OP additive_expr  */
#line 290 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "EQ_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1583 "parser.cpp"
    break;

  case 57: /* relational_expr: relational_expr NEQ_OP additive_expr  */
#line 292 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "NEQ_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1589 "parser.cpp"
    break;

  case 58: /* relational_expr: relational_expr LT_OP additive_expr  */
#line 294 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "LT_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1595 "parser.cpp"
    break;

  case 59: /* relational_expr: relational_expr LTE_OP additive_expr  */
#line 296 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "LTE_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1601 "parser.cpp"
    break;

  case 60: /* relational_expr: relational_expr GT_OP additive_expr  */
#line 298 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "GT_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1607 "parser.cpp"
    break;

  case 61: /* relational_expr: relational_expr GTE_OP additive_expr  */
#line 300 "parser.y"
                 { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "GTE_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1613 "parser.cpp"
    break;

  case 62: /* additive_expr: multiplicative_expr  */
#line 304 "parser.y"
               { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1619 "parser.cpp"
    break;

  case 63: /* additive_expr: additive_expr '+' multiplicative_expr  */
#line 306 "parser.y"
               { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "+", (yyvsp[0].pExprNode), lin, col); }
#line 1625 "parser.cpp"
    break;

  case 64: /* additive_expr: additive_expr '-' multiplicative_expr  */
#line 308 "parser.y"
               { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "-", (yyvsp[0].pExprNode), lin, col); }
#line 1631 "parser.cpp"
    break;

  case 65: /* multiplicative_expr: unary_expr  */
#line 312 "parser.y"
                     { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1637 "parser.cpp"
    break;

  case 66: /* multiplicative_expr: multiplicative_expr '*' unary_expr  */
#line 314 "parser.y"
                     { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "*", (yyvsp[0].pExprNode), lin, col); }
#line 1643 "parser.cpp"
    break;

  case 67: /* multiplicative_expr: multiplicative_expr '/' unary_expr  */
#line 316 "parser.y"
                     { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "/", (yyvsp[0].pExprNode), lin, col); }
#line 1649 "parser.cpp"
    break;

  case 68: /* multiplicative_expr: multiplicative_expr DIV_OP unary_expr  */
#line 318 "parser.y"
                     { (yyval.pExprNode) = new BinaryOpNode((yyvsp[-2].pExprNode), "DIV_OP", (yyvsp[0].pExprNode), lin, col); }
#line 1655 "parser.cpp"
    break;

  case 69: /* unary_expr: primary  */
#line 322 "parser.y"
            { (yyval.pExprNode) = (yyvsp[0].pExprNode); }
#line 1661 "parser.cpp"
    break;

  case 70: /* unary_expr: '-' primary  */
#line 324 "parser.y"
            { (yyval.pExprNode) = new UnaryOpNode("-", (yyvsp[0].pExprNode), lin, col); }
#line 1667 "parser.cpp"
    break;

  case 71: /* primary: id_node  */
#line 330 "parser.y"
    { (yyval.pExprNode) = new IdExprNode((yyvsp[0].pIdentNode), lin, col); }
#line 1673 "parser.cpp"
    break;

  case 72: /* primary: int_num_node  */
#line 332 "parser.y"
    { (yyval.pExprNode) = (yyvsp[0].pIntNumNode); }
#line 1679 "parser.cpp"
    break;

  case 73: /* primary: real_num_node  */
#line 334 "parser.y"
    { (yyval.pExprNode) = (yyvsp[0].pRealNumNode); }
#line 1685 "parser.cpp"
    break;

  case 74: /* primary: TRUE_KEYWORD  */
#line 336 "parser.y"
    { (yyval.pExprNode) = new BooleanLiteralNode(true, lin, col); }
#line 1691 "parser.cpp"
    break;

  case 75: /* primary: FALSE_KEYWORD  */
#line 338 "parser.y"
    { (yyval.pExprNode) = new BooleanLiteralNode(false, lin, col); }
#line 1697 "parser.cpp"
    break;

  case 76: /* primary: id_node '(' expression_list ')'  */
#line 340 "parser.y"
    { (yyval.pExprNode) = new FunctionCallExprNode((yyvsp[-3].pIdentNode), (yyvsp[-1].pExpressionList), lin, col); }
#line 1703 "parser.cpp"
    break;

  case 77: /* primary: '(' expr ')'  */
#line 342 "parser.y"
    { (yyval.pExprNode) = (yyvsp[-1].pExprNode); }
#line 1709 "parser.cpp"
    break;


#line 1713 "parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;


      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 345 "parser.y"


int yyerror(const char* s) {
    std::cout << "SYNTAX ERROR: " << s << " at line: " << lin << ", Column: " << col << std::endl;
    return 0;
}
