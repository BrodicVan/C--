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
#line 2 "./syntax.y"

	#include <stdio.h>
    #include "tree_semantics_raw.h"
	
    extern int yylex();
    void yyerror(const char* msg);
    void my_yyerror(const char* msg);
    extern struct Node* root;
    extern int yylineno;
    extern char* yytext;
    int fail = 0;
    

#line 85 "./syntax.tab.c"

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

#include "syntax.tab.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_INT = 3,                        /* INT  */
  YYSYMBOL_INT_ERR = 4,                    /* INT_ERR  */
  YYSYMBOL_FLOAT = 5,                      /* FLOAT  */
  YYSYMBOL_FLOAT_ERR = 6,                  /* FLOAT_ERR  */
  YYSYMBOL_ID = 7,                         /* ID  */
  YYSYMBOL_SEMI = 8,                       /* SEMI  */
  YYSYMBOL_COMMA = 9,                      /* COMMA  */
  YYSYMBOL_TYPE = 10,                      /* TYPE  */
  YYSYMBOL_LC = 11,                        /* LC  */
  YYSYMBOL_RC = 12,                        /* RC  */
  YYSYMBOL_STRUCT = 13,                    /* STRUCT  */
  YYSYMBOL_RETURN = 14,                    /* RETURN  */
  YYSYMBOL_IF = 15,                        /* IF  */
  YYSYMBOL_ELSE = 16,                      /* ELSE  */
  YYSYMBOL_WHILE = 17,                     /* WHILE  */
  YYSYMBOL_ASSIGNOP = 18,                  /* ASSIGNOP  */
  YYSYMBOL_OR = 19,                        /* OR  */
  YYSYMBOL_AND = 20,                       /* AND  */
  YYSYMBOL_RELOP = 21,                     /* RELOP  */
  YYSYMBOL_PLUS = 22,                      /* PLUS  */
  YYSYMBOL_MINUS = 23,                     /* MINUS  */
  YYSYMBOL_STAR = 24,                      /* STAR  */
  YYSYMBOL_DIV = 25,                       /* DIV  */
  YYSYMBOL_NOT = 26,                       /* NOT  */
  YYSYMBOL_LP = 27,                        /* LP  */
  YYSYMBOL_RP = 28,                        /* RP  */
  YYSYMBOL_LB = 29,                        /* LB  */
  YYSYMBOL_RB = 30,                        /* RB  */
  YYSYMBOL_DOT = 31,                       /* DOT  */
  YYSYMBOL_LOWER_THAN_ELSE = 32,           /* LOWER_THAN_ELSE  */
  YYSYMBOL_LOWER_THAN_RB = 33,             /* LOWER_THAN_RB  */
  YYSYMBOL_YYACCEPT = 34,                  /* $accept  */
  YYSYMBOL_Program = 35,                   /* Program  */
  YYSYMBOL_ExtDefList = 36,                /* ExtDefList  */
  YYSYMBOL_ExtDef = 37,                    /* ExtDef  */
  YYSYMBOL_ExtDecList = 38,                /* ExtDecList  */
  YYSYMBOL_Specifier = 39,                 /* Specifier  */
  YYSYMBOL_StructSpecifier = 40,           /* StructSpecifier  */
  YYSYMBOL_OptTag = 41,                    /* OptTag  */
  YYSYMBOL_Tag = 42,                       /* Tag  */
  YYSYMBOL_VarDec = 43,                    /* VarDec  */
  YYSYMBOL_FunDec = 44,                    /* FunDec  */
  YYSYMBOL_VarList = 45,                   /* VarList  */
  YYSYMBOL_ParamDec = 46,                  /* ParamDec  */
  YYSYMBOL_CompSt = 47,                    /* CompSt  */
  YYSYMBOL_StmtList = 48,                  /* StmtList  */
  YYSYMBOL_Stmt = 49,                      /* Stmt  */
  YYSYMBOL_DefList = 50,                   /* DefList  */
  YYSYMBOL_Def = 51,                       /* Def  */
  YYSYMBOL_DecList = 52,                   /* DecList  */
  YYSYMBOL_Dec = 53,                       /* Dec  */
  YYSYMBOL_Exp = 54,                       /* Exp  */
  YYSYMBOL_Args = 55                       /* Args  */
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

#if 1

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
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

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
#define YYFINAL  14
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   513

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  34
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  22
/* YYNRULES -- Number of rules.  */
#define YYNRULES  91
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  163

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   288


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
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_uint8 yyrline[] =
{
       0,    53,    53,    56,    57,    60,    61,    62,    63,    64,
      65,    69,    70,    72,    77,    78,    81,    82,    84,    85,
      86,    87,    91,    92,    95,   100,   101,   102,   103,   106,
     107,   108,   109,   112,   113,   116,   121,   124,   125,   128,
     129,   130,   131,   132,   133,   134,   135,   136,   137,   138,
     139,   144,   145,   148,   151,   152,   153,   156,   157,   158,
     163,   164,   165,   166,   167,   168,   169,   170,   171,   172,
     173,   174,   175,   176,   177,   178,   179,   180,   181,   182,
     184,   185,   186,   187,   188,   189,   190,   191,   192,   193,
     196,   197
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "INT", "INT_ERR",
  "FLOAT", "FLOAT_ERR", "ID", "SEMI", "COMMA", "TYPE", "LC", "RC",
  "STRUCT", "RETURN", "IF", "ELSE", "WHILE", "ASSIGNOP", "OR", "AND",
  "RELOP", "PLUS", "MINUS", "STAR", "DIV", "NOT", "LP", "RP", "LB", "RB",
  "DOT", "LOWER_THAN_ELSE", "LOWER_THAN_RB", "$accept", "Program",
  "ExtDefList", "ExtDef", "ExtDecList", "Specifier", "StructSpecifier",
  "OptTag", "Tag", "VarDec", "FunDec", "VarList", "ParamDec", "CompSt",
  "StmtList", "Stmt", "DefList", "Def", "DecList", "Dec", "Exp", "Args", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-71)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-55)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
      46,     4,   -71,    84,    16,   -71,    46,   105,   -71,   -71,
      17,    30,    61,   -71,   -71,   -71,    41,    31,   -71,    79,
      14,    73,    47,    38,   -71,    76,   -71,    87,    81,   119,
      47,   -71,    12,    85,    47,   102,   112,    65,   -71,    81,
      89,   116,    81,   -71,   -71,   -71,    96,    98,   104,   111,
      26,   125,    91,   -71,   -71,   -71,   -71,   -71,   110,   -71,
      47,   -71,   -71,   -71,     0,   -71,   -71,   -71,   -71,   113,
     333,   114,   117,   333,   333,   333,   -71,   137,   104,    44,
     333,   333,   -71,    12,    12,   -71,   -71,   333,    75,   343,
     200,   333,   324,    11,   389,   -71,   -71,   -71,   -71,   212,
     227,   239,   254,   266,   281,   293,   308,   320,   136,   459,
     459,   -71,   -71,   403,   122,   -71,   360,   123,   -71,   124,
     417,   431,   -71,   -71,   459,   -71,   471,   -71,   482,   -71,
     373,   -71,   324,   -71,   324,   -71,    11,   -71,    11,   126,
     445,   -71,   158,   -71,   333,   -71,   131,   185,   158,   -71,
     -71,   -71,   -71,   158,   -71,    -2,   139,   -71,   -71,   158,
     158,   -71,   -71
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,    14,     0,     0,     2,     0,     0,    15,     9,
      21,    24,     0,    17,     1,     4,    10,    25,     6,     0,
       0,     0,    51,     0,     8,     0,     5,     0,     0,     0,
      51,     7,     0,     0,    51,    20,     0,    32,    30,     0,
       0,    34,     0,    25,    12,    28,     0,     0,     0,     0,
      57,     0,     0,    18,    52,    19,    16,    31,    35,    29,
       0,    13,    26,    27,     0,    76,    77,    78,    79,    75,
       0,     0,     0,     0,     0,     0,    41,     0,     0,     0,
       0,     0,    53,     0,     0,    33,    46,     0,     0,     0,
       0,     0,    69,    70,     0,    36,    38,    40,    39,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    59,
      58,    56,    55,     0,     0,    72,    91,     0,    42,     0,
       0,     0,    68,    80,    60,    82,    62,    81,    61,    83,
      63,    84,    64,    85,    65,    86,    66,    87,    67,     0,
       0,    74,     0,    88,     0,    71,     0,     0,     0,    89,
      73,    50,    90,     0,    47,     0,    43,    45,    49,     0,
       0,    48,    44
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -71,   -71,   147,   -71,    -4,     1,   -71,   -71,   -71,   -30,
     -71,   100,   -71,   145,    90,   246,    60,   -71,   -13,   -71,
     -70,    23
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,     4,     5,     6,    19,    32,     8,    12,    13,    20,
      21,    40,    41,    76,    77,    78,    33,    34,    51,    52,
      79,   117
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      89,     7,    50,    92,    93,    94,    86,     7,    86,    58,
     109,   110,     9,    49,   159,    27,    14,   113,   116,    43,
     120,   121,   -11,    28,    44,    87,    39,    87,    22,   124,
     126,   128,   130,   132,   134,   136,   138,   140,    61,    35,
     107,   -23,   108,    29,    81,    97,    -3,     1,     2,    24,
     -51,     3,    98,    50,    50,    29,     2,     2,    25,     3,
       3,    39,    99,   100,   101,   102,   103,   104,   105,   106,
     111,   112,    23,   107,   116,   108,   114,    37,    65,    66,
      67,    68,    69,    36,    30,    10,     2,    26,    43,     3,
      48,    11,    83,    57,    54,   -22,    42,    53,    73,   -54,
      84,    74,    75,   115,    38,    64,    16,    65,    66,    67,
      68,    69,    17,    18,    55,    30,   -37,    59,    70,    71,
      45,    72,    46,    47,    56,    60,    62,    73,    63,    80,
      74,    75,    64,    82,    65,    66,    67,    68,    69,    29,
      88,    90,    30,   141,    91,    70,    71,   153,    72,    95,
     143,   145,   146,    15,    73,   160,   149,    74,    75,    64,
      85,    65,    66,    67,    68,    69,    31,   152,    96,    30,
       0,     0,    70,    71,     0,    72,     0,     0,     0,     0,
       0,    73,     0,     0,    74,    75,   155,     0,    65,    66,
      67,    68,    69,     0,     0,     0,    30,     0,     0,    70,
      71,   119,    72,    65,    66,    67,    68,    69,    73,     0,
       0,    74,    75,   123,     0,    65,    66,    67,    68,    69,
       0,     0,     0,    73,     0,     0,    74,    75,   125,     0,
      65,    66,    67,    68,    69,    73,     0,     0,    74,    75,
     127,     0,    65,    66,    67,    68,    69,     0,     0,     0,
      73,     0,     0,    74,    75,   129,     0,    65,    66,    67,
      68,    69,    73,     0,     0,    74,    75,   131,     0,    65,
      66,    67,    68,    69,     0,     0,     0,    73,     0,     0,
      74,    75,   133,     0,    65,    66,    67,    68,    69,    73,
       0,     0,    74,    75,   135,     0,    65,    66,    67,    68,
      69,     0,     0,     0,    73,     0,     0,    74,    75,   137,
       0,    65,    66,    67,    68,    69,    73,     0,     0,    74,
      75,   139,     0,    65,    66,    67,    68,    69,     0,     0,
       0,    73,     0,     0,    74,    75,    65,    66,    67,    68,
      69,     0,     0,    73,     0,     0,    74,    75,   105,   106,
       0,   118,     0,   107,     0,   108,    73,     0,     0,    74,
      75,    99,   100,   101,   102,   103,   104,   105,   106,   144,
       0,     0,   107,     0,   108,     0,     0,     0,    99,   100,
     101,   102,   103,   104,   105,   106,     0,     0,   151,   107,
       0,   108,   154,   156,   157,   103,   104,   105,   106,   158,
       0,     0,   107,     0,   108,   161,   162,    99,   100,   101,
     102,   103,   104,   105,   106,     0,     0,   122,   107,     0,
     108,    99,   100,   101,   102,   103,   104,   105,   106,     0,
       0,   142,   107,     0,   108,    99,   100,   101,   102,   103,
     104,   105,   106,     0,     0,   147,   107,     0,   108,    99,
     100,   101,   102,   103,   104,   105,   106,     0,     0,   148,
     107,     0,   108,    99,   100,   101,   102,   103,   104,   105,
     106,     0,     0,     0,   107,   150,   108,    99,   100,   101,
     102,   103,   104,   105,   106,     0,     0,     0,   107,     0,
     108,   101,   102,   103,   104,   105,   106,     0,     0,     0,
     107,     0,   108,   102,   103,   104,   105,   106,     0,     0,
       0,   107,     0,   108
};

static const yytype_int16 yycheck[] =
{
      70,     0,    32,    73,    74,    75,     8,     6,     8,    39,
      80,    81,     8,     1,    16,     1,     0,    87,    88,     7,
      90,    91,     8,     9,    28,    27,    25,    27,    11,    99,
     100,   101,   102,   103,   104,   105,   106,   107,    42,     1,
      29,    11,    31,    29,    18,     1,     0,     1,    10,     8,
      12,    13,     8,    83,    84,    29,    10,    10,    27,    13,
      13,    60,    18,    19,    20,    21,    22,    23,    24,    25,
      83,    84,    11,    29,   144,    31,     1,     1,     3,     4,
       5,     6,     7,    23,    11,     1,    10,     8,     7,    13,
      30,     7,     1,    28,    34,    11,     9,    12,    23,     8,
       9,    26,    27,    28,    28,     1,     1,     3,     4,     5,
       6,     7,     7,     8,    12,    11,    12,    28,    14,    15,
       1,    17,     3,     4,    12,     9,    30,    23,    30,    18,
      26,    27,     1,     8,     3,     4,     5,     6,     7,    29,
      27,    27,    11,     7,    27,    14,    15,    16,    17,    12,
      28,    28,    28,     6,    23,    16,    30,    26,    27,     1,
      60,     3,     4,     5,     6,     7,    21,   144,    78,    11,
      -1,    -1,    14,    15,    -1,    17,    -1,    -1,    -1,    -1,
      -1,    23,    -1,    -1,    26,    27,     1,    -1,     3,     4,
       5,     6,     7,    -1,    -1,    -1,    11,    -1,    -1,    14,
      15,     1,    17,     3,     4,     5,     6,     7,    23,    -1,
      -1,    26,    27,     1,    -1,     3,     4,     5,     6,     7,
      -1,    -1,    -1,    23,    -1,    -1,    26,    27,     1,    -1,
       3,     4,     5,     6,     7,    23,    -1,    -1,    26,    27,
       1,    -1,     3,     4,     5,     6,     7,    -1,    -1,    -1,
      23,    -1,    -1,    26,    27,     1,    -1,     3,     4,     5,
       6,     7,    23,    -1,    -1,    26,    27,     1,    -1,     3,
       4,     5,     6,     7,    -1,    -1,    -1,    23,    -1,    -1,
      26,    27,     1,    -1,     3,     4,     5,     6,     7,    23,
      -1,    -1,    26,    27,     1,    -1,     3,     4,     5,     6,
       7,    -1,    -1,    -1,    23,    -1,    -1,    26,    27,     1,
      -1,     3,     4,     5,     6,     7,    23,    -1,    -1,    26,
      27,     1,    -1,     3,     4,     5,     6,     7,    -1,    -1,
      -1,    23,    -1,    -1,    26,    27,     3,     4,     5,     6,
       7,    -1,    -1,    23,    -1,    -1,    26,    27,    24,    25,
      -1,     8,    -1,    29,    -1,    31,    23,    -1,    -1,    26,
      27,    18,    19,    20,    21,    22,    23,    24,    25,     9,
      -1,    -1,    29,    -1,    31,    -1,    -1,    -1,    18,    19,
      20,    21,    22,    23,    24,    25,    -1,    -1,   142,    29,
      -1,    31,   146,   147,   148,    22,    23,    24,    25,   153,
      -1,    -1,    29,    -1,    31,   159,   160,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    28,    29,    -1,
      31,    18,    19,    20,    21,    22,    23,    24,    25,    -1,
      -1,    28,    29,    -1,    31,    18,    19,    20,    21,    22,
      23,    24,    25,    -1,    -1,    28,    29,    -1,    31,    18,
      19,    20,    21,    22,    23,    24,    25,    -1,    -1,    28,
      29,    -1,    31,    18,    19,    20,    21,    22,    23,    24,
      25,    -1,    -1,    -1,    29,    30,    31,    18,    19,    20,
      21,    22,    23,    24,    25,    -1,    -1,    -1,    29,    -1,
      31,    20,    21,    22,    23,    24,    25,    -1,    -1,    -1,
      29,    -1,    31,    21,    22,    23,    24,    25,    -1,    -1,
      -1,    29,    -1,    31
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     1,    10,    13,    35,    36,    37,    39,    40,     8,
       1,     7,    41,    42,     0,    36,     1,     7,     8,    38,
      43,    44,    11,    11,     8,    27,     8,     1,     9,    29,
      11,    47,    39,    50,    51,     1,    50,     1,    28,    39,
      45,    46,     9,     7,    38,     1,     3,     4,    50,     1,
      43,    52,    53,    12,    50,    12,    12,    28,    43,    28,
       9,    38,    30,    30,     1,     3,     4,     5,     6,     7,
      14,    15,    17,    23,    26,    27,    47,    48,    49,    54,
      18,    18,     8,     1,     9,    45,     8,    27,    27,    54,
      27,    27,    54,    54,    54,    12,    48,     1,     8,    18,
      19,    20,    21,    22,    23,    24,    25,    29,    31,    54,
      54,    52,    52,    54,     1,    28,    54,    55,     8,     1,
      54,    54,    28,     1,    54,     1,    54,     1,    54,     1,
      54,     1,    54,     1,    54,     1,    54,     1,    54,     1,
      54,     7,    28,    28,     9,    28,    28,    28,    28,    30,
      30,    49,    55,    16,    49,     1,    49,    49,    49,    16,
      16,    49,    49
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    34,    35,    36,    36,    37,    37,    37,    37,    37,
      37,    38,    38,    38,    39,    39,    40,    40,    40,    40,
      40,    40,    41,    41,    42,    43,    43,    43,    43,    44,
      44,    44,    44,    45,    45,    46,    47,    48,    48,    49,
      49,    49,    49,    49,    49,    49,    49,    49,    49,    49,
      49,    50,    50,    51,    52,    52,    52,    53,    53,    53,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      54,    54,    54,    54,    54,    54,    54,    54,    54,    54,
      55,    55
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     0,     2,     3,     2,     3,     3,     2,
       2,     1,     3,     4,     1,     1,     5,     2,     5,     5,
       4,     2,     0,     1,     1,     1,     4,     4,     3,     4,
       3,     4,     3,     3,     1,     2,     4,     0,     2,     2,
       2,     1,     3,     5,     7,     5,     2,     5,     7,     6,
       5,     0,     2,     3,     1,     3,     3,     1,     3,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     2,
       2,     4,     3,     4,     3,     1,     1,     1,     1,     1,
       3,     3,     3,     3,     3,     3,     3,     3,     4,     4,
       3,     1
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

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


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


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
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
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
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
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
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


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif

#ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYPTRDIFF_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYPTRDIFF_T yyn = 0;
      char const *yyp = yystr;
      for (;;)
        switch (*++yyp)
          {
          case '\'':
          case ',':
            goto do_not_strip_quotes;

          case '\\':
            if (*++yyp != '\\')
              goto do_not_strip_quotes;
            else
              goto append;

          append:
          default:
            if (yyres)
              yyres[yyn] = *yyp;
            yyn++;
            break;

          case '"':
            if (yyres)
              yyres[yyn] = '\0';
            return yyn;
          }
    do_not_strip_quotes: ;
    }

  if (yyres)
    return yystpcpy (yyres, yystr) - yyres;
  else
    return yystrlen (yystr);
}
#endif


static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yytnamerr (YY_NULLPTR, yytname[yyarg[yyi]]);
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp += yytnamerr (yyp, yytname[yyarg[yyi++]]);
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
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
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
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

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
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
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
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
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

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
      yyerror_range[1] = yylloc;
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
  *++yylsp = yylloc;

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

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* Program: ExtDefList  */
#line 53 "./syntax.y"
                    {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Program,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));root=(yyval.type_node);}
#line 1690 "./syntax.tab.c"
    break;

  case 3: /* ExtDefList: %empty  */
#line 56 "./syntax.y"
             {(yyval.type_node)=createNode(0,ExtDefList,NULL,0,0);(yyval.type_node)->if_empty=1;}
#line 1696 "./syntax.tab.c"
    break;

  case 4: /* ExtDefList: ExtDef ExtDefList  */
#line 57 "./syntax.y"
                    {(yyval.type_node) = createNode((yyvsp[-1].type_node)->row,ExtDefList,NULL,0,0);addNodes((yyval.type_node),2,(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1702 "./syntax.tab.c"
    break;

  case 5: /* ExtDef: Specifier ExtDecList SEMI  */
#line 60 "./syntax.y"
                                  {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,ExtDef,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1708 "./syntax.tab.c"
    break;

  case 6: /* ExtDef: Specifier SEMI  */
#line 61 "./syntax.y"
                 {(yyval.type_node) = createNode((yyvsp[-1].type_node)->row,ExtDef,NULL,0,0);addNodes((yyval.type_node),2,(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1714 "./syntax.tab.c"
    break;

  case 7: /* ExtDef: Specifier FunDec CompSt  */
#line 62 "./syntax.y"
                           {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,ExtDef,NULL,0,0);(yyval.type_node)->generation = ExtDef0;addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1720 "./syntax.tab.c"
    break;

  case 8: /* ExtDef: Specifier error SEMI  */
#line 63 "./syntax.y"
                       {my_yyerror("error ExtDef");}
#line 1726 "./syntax.tab.c"
    break;

  case 9: /* ExtDef: error SEMI  */
#line 64 "./syntax.y"
             {my_yyerror("error ExtDef");}
#line 1732 "./syntax.tab.c"
    break;

  case 10: /* ExtDef: Specifier error  */
#line 65 "./syntax.y"
                  {my_yyerror("error ExtDef");}
#line 1738 "./syntax.tab.c"
    break;

  case 11: /* ExtDecList: VarDec  */
#line 69 "./syntax.y"
                   {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,ExtDecList,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 1744 "./syntax.tab.c"
    break;

  case 12: /* ExtDecList: VarDec COMMA ExtDecList  */
#line 70 "./syntax.y"
                          {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,ExtDefList,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1750 "./syntax.tab.c"
    break;

  case 13: /* ExtDecList: VarDec error COMMA ExtDecList  */
#line 72 "./syntax.y"
                                {my_yyerror("error ExtDecList");}
#line 1756 "./syntax.tab.c"
    break;

  case 14: /* Specifier: TYPE  */
#line 77 "./syntax.y"
                {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Specifier,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 1762 "./syntax.tab.c"
    break;

  case 15: /* Specifier: StructSpecifier  */
#line 78 "./syntax.y"
                  {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Specifier,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 1768 "./syntax.tab.c"
    break;

  case 16: /* StructSpecifier: STRUCT OptTag LC DefList RC  */
#line 81 "./syntax.y"
                                             {(yyval.type_node) = createNode((yyvsp[-4].type_node)->row,StructSpecifier,NULL,0,0);(yyval.type_node)->generation = StructSpecifier0;addNodes((yyval.type_node),5,(yyvsp[-4].type_node),(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1774 "./syntax.tab.c"
    break;

  case 17: /* StructSpecifier: STRUCT Tag  */
#line 82 "./syntax.y"
             {(yyval.type_node) = createNode((yyvsp[-1].type_node)->row,StructSpecifier,NULL,0,0);addNodes((yyval.type_node),2,(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1780 "./syntax.tab.c"
    break;

  case 18: /* StructSpecifier: STRUCT error LC DefList RC  */
#line 84 "./syntax.y"
                             {my_yyerror("error StructSpecifier");}
#line 1786 "./syntax.tab.c"
    break;

  case 19: /* StructSpecifier: STRUCT OptTag LC error RC  */
#line 85 "./syntax.y"
                            {my_yyerror("error StructSpecifier");}
#line 1792 "./syntax.tab.c"
    break;

  case 20: /* StructSpecifier: STRUCT OptTag LC error  */
#line 86 "./syntax.y"
                         {my_yyerror("error StructSpecifier");}
#line 1798 "./syntax.tab.c"
    break;

  case 21: /* StructSpecifier: STRUCT error  */
#line 87 "./syntax.y"
               {my_yyerror("error StructSpecifier");}
#line 1804 "./syntax.tab.c"
    break;

  case 22: /* OptTag: %empty  */
#line 91 "./syntax.y"
         {(yyval.type_node)=createNode(0,OptTag,NULL,0,0);(yyval.type_node)->if_empty=1;}
#line 1810 "./syntax.tab.c"
    break;

  case 23: /* OptTag: ID  */
#line 92 "./syntax.y"
     {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,OptTag,NULL,0,0);(yyval.type_node)->generation = OptTag0;addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 1816 "./syntax.tab.c"
    break;

  case 24: /* Tag: ID  */
#line 95 "./syntax.y"
        {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Tag,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));(yyval.type_node)->generation = Tag0;}
#line 1822 "./syntax.tab.c"
    break;

  case 25: /* VarDec: ID  */
#line 100 "./syntax.y"
           {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,VarDec,NULL,0,0);(yyval.type_node)->generation = VarDec0;addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 1828 "./syntax.tab.c"
    break;

  case 26: /* VarDec: VarDec LB INT RB  */
#line 101 "./syntax.y"
                   {(yyval.type_node) = createNode((yyvsp[-3].type_node)->row,VarDec,NULL,0,0);(yyval.type_node)->generation = VarDec1;addNodes((yyval.type_node),4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1834 "./syntax.tab.c"
    break;

  case 27: /* VarDec: VarDec LB INT_ERR RB  */
#line 102 "./syntax.y"
                       {(yyval.type_node) = createNode((yyvsp[-3].type_node)->row,VarDec,NULL,0,0);addNodes((yyval.type_node),4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1840 "./syntax.tab.c"
    break;

  case 28: /* VarDec: VarDec LB error  */
#line 103 "./syntax.y"
                  {my_yyerror("error VarDec");}
#line 1846 "./syntax.tab.c"
    break;

  case 29: /* FunDec: ID LP VarList RP  */
#line 106 "./syntax.y"
                         {(yyval.type_node) = createNode((yyvsp[-3].type_node)->row,FunDec,NULL,0,0);(yyval.type_node)->generation = FunDec0;addNodes((yyval.type_node),4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1852 "./syntax.tab.c"
    break;

  case 30: /* FunDec: ID LP RP  */
#line 107 "./syntax.y"
           {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,FunDec,NULL,0,0);(yyval.type_node)->generation = FunDec1;addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1858 "./syntax.tab.c"
    break;

  case 31: /* FunDec: ID LP error RP  */
#line 108 "./syntax.y"
                 {my_yyerror("error FunDec");}
#line 1864 "./syntax.tab.c"
    break;

  case 32: /* FunDec: ID LP error  */
#line 109 "./syntax.y"
              {my_yyerror("error FunDec");}
#line 1870 "./syntax.tab.c"
    break;

  case 33: /* VarList: ParamDec COMMA VarList  */
#line 112 "./syntax.y"
                                {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,VarList,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1876 "./syntax.tab.c"
    break;

  case 34: /* VarList: ParamDec  */
#line 113 "./syntax.y"
           {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,VarList,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 1882 "./syntax.tab.c"
    break;

  case 35: /* ParamDec: Specifier VarDec  */
#line 116 "./syntax.y"
                           {(yyval.type_node) = createNode((yyvsp[-1].type_node)->row,ParamDec,NULL,0,0);(yyval.type_node)->generation = ParamDec0;addNodes((yyval.type_node),2,(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1888 "./syntax.tab.c"
    break;

  case 36: /* CompSt: LC DefList StmtList RC  */
#line 121 "./syntax.y"
                               {(yyval.type_node) = createNode((yyvsp[-3].type_node)->row,CompSt,NULL,0,0);(yyval.type_node)->generation = CompSt0;addNodes((yyval.type_node),4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1894 "./syntax.tab.c"
    break;

  case 37: /* StmtList: %empty  */
#line 124 "./syntax.y"
           {(yyval.type_node)=createNode(0,StmtList,NULL,0,0);(yyval.type_node)->if_empty=1;}
#line 1900 "./syntax.tab.c"
    break;

  case 38: /* StmtList: Stmt StmtList  */
#line 125 "./syntax.y"
                {(yyval.type_node) = createNode((yyvsp[-1].type_node)->row,StmtList,NULL,0,0);(yyval.type_node)->generation = StmtList0;addNodes((yyval.type_node),2,(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1906 "./syntax.tab.c"
    break;

  case 39: /* Stmt: Exp SEMI  */
#line 128 "./syntax.y"
               {(yyval.type_node) = createNode((yyvsp[-1].type_node)->row,Stmt,NULL,0,0);addNodes((yyval.type_node),2,(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1912 "./syntax.tab.c"
    break;

  case 40: /* Stmt: Exp error  */
#line 129 "./syntax.y"
            {my_yyerror("missing ';'");}
#line 1918 "./syntax.tab.c"
    break;

  case 41: /* Stmt: CompSt  */
#line 130 "./syntax.y"
         {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Stmt,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 1924 "./syntax.tab.c"
    break;

  case 42: /* Stmt: RETURN Exp SEMI  */
#line 131 "./syntax.y"
                  {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Stmt,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1930 "./syntax.tab.c"
    break;

  case 43: /* Stmt: IF LP Exp RP Stmt  */
#line 132 "./syntax.y"
                                          {(yyval.type_node) = createNode((yyvsp[-4].type_node)->row,Stmt,NULL,0,0);addNodes((yyval.type_node),5,(yyvsp[-4].type_node),(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1936 "./syntax.tab.c"
    break;

  case 44: /* Stmt: IF LP Exp RP Stmt ELSE Stmt  */
#line 133 "./syntax.y"
                              {(yyval.type_node) = createNode((yyvsp[-6].type_node)->row,Stmt,NULL,0,0);addNodes((yyval.type_node),7,(yyvsp[-6].type_node),(yyvsp[-5].type_node),(yyvsp[-4].type_node),(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1942 "./syntax.tab.c"
    break;

  case 45: /* Stmt: WHILE LP Exp RP Stmt  */
#line 134 "./syntax.y"
                       {(yyval.type_node) = createNode((yyvsp[-4].type_node)->row,Stmt,NULL,0,0);addNodes((yyval.type_node),5,(yyvsp[-4].type_node),(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1948 "./syntax.tab.c"
    break;

  case 46: /* Stmt: error SEMI  */
#line 135 "./syntax.y"
             {my_yyerror("error Stmt");}
#line 1954 "./syntax.tab.c"
    break;

  case 47: /* Stmt: IF LP error RP Stmt  */
#line 136 "./syntax.y"
                                            {my_yyerror("error IF Stmt");}
#line 1960 "./syntax.tab.c"
    break;

  case 48: /* Stmt: IF LP Exp RP error ELSE Stmt  */
#line 137 "./syntax.y"
                               {my_yyerror("error IF Stmt");}
#line 1966 "./syntax.tab.c"
    break;

  case 49: /* Stmt: IF LP error RP ELSE Stmt  */
#line 138 "./syntax.y"
                           {my_yyerror("error IF Stmt");}
#line 1972 "./syntax.tab.c"
    break;

  case 50: /* Stmt: error LP Exp RP Stmt  */
#line 139 "./syntax.y"
                       {my_yyerror("error Stmt");}
#line 1978 "./syntax.tab.c"
    break;

  case 51: /* DefList: %empty  */
#line 144 "./syntax.y"
          {(yyval.type_node)=createNode(0,DefList,NULL,0,0);(yyval.type_node)->if_empty=1;}
#line 1984 "./syntax.tab.c"
    break;

  case 52: /* DefList: Def DefList  */
#line 145 "./syntax.y"
              {(yyval.type_node) = createNode((yyvsp[-1].type_node)->row,DefList,NULL,0,0);(yyval.type_node)->generation = DefList0;addNodes((yyval.type_node),2,(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1990 "./syntax.tab.c"
    break;

  case 53: /* Def: Specifier DecList SEMI  */
#line 148 "./syntax.y"
                            {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Def,NULL,0,0);(yyval.type_node)->generation = Def0;addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 1996 "./syntax.tab.c"
    break;

  case 54: /* DecList: Dec  */
#line 151 "./syntax.y"
             {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,DecList,NULL,0,0);(yyval.type_node)->generation = DecList0;addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 2002 "./syntax.tab.c"
    break;

  case 55: /* DecList: Dec COMMA DecList  */
#line 152 "./syntax.y"
                    {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,DecList,NULL,0,0);(yyval.type_node)->generation = DecList1;addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2008 "./syntax.tab.c"
    break;

  case 56: /* DecList: Dec error DecList  */
#line 153 "./syntax.y"
                    {my_yyerror("error DecList");}
#line 2014 "./syntax.tab.c"
    break;

  case 57: /* Dec: VarDec  */
#line 156 "./syntax.y"
            {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Dec,NULL,0,0);(yyval.type_node)->generation = Dec0;addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 2020 "./syntax.tab.c"
    break;

  case 58: /* Dec: VarDec ASSIGNOP Exp  */
#line 157 "./syntax.y"
                      {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Dec,NULL,0,0);(yyval.type_node)->generation = Dec1;addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2026 "./syntax.tab.c"
    break;

  case 59: /* Dec: error ASSIGNOP Exp  */
#line 158 "./syntax.y"
                     {my_yyerror("error Dec");}
#line 2032 "./syntax.tab.c"
    break;

  case 60: /* Exp: Exp ASSIGNOP Exp  */
#line 163 "./syntax.y"
                      {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2038 "./syntax.tab.c"
    break;

  case 61: /* Exp: Exp AND Exp  */
#line 164 "./syntax.y"
              {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2044 "./syntax.tab.c"
    break;

  case 62: /* Exp: Exp OR Exp  */
#line 165 "./syntax.y"
             {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2050 "./syntax.tab.c"
    break;

  case 63: /* Exp: Exp RELOP Exp  */
#line 166 "./syntax.y"
                {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2056 "./syntax.tab.c"
    break;

  case 64: /* Exp: Exp PLUS Exp  */
#line 167 "./syntax.y"
               {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2062 "./syntax.tab.c"
    break;

  case 65: /* Exp: Exp MINUS Exp  */
#line 168 "./syntax.y"
                {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2068 "./syntax.tab.c"
    break;

  case 66: /* Exp: Exp STAR Exp  */
#line 169 "./syntax.y"
               {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2074 "./syntax.tab.c"
    break;

  case 67: /* Exp: Exp DIV Exp  */
#line 170 "./syntax.y"
              {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2080 "./syntax.tab.c"
    break;

  case 68: /* Exp: LP Exp RP  */
#line 171 "./syntax.y"
            {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2086 "./syntax.tab.c"
    break;

  case 69: /* Exp: MINUS Exp  */
#line 172 "./syntax.y"
            {(yyval.type_node) = createNode((yyvsp[-1].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),2,(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2092 "./syntax.tab.c"
    break;

  case 70: /* Exp: NOT Exp  */
#line 173 "./syntax.y"
          {(yyval.type_node) = createNode((yyvsp[-1].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),2,(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2098 "./syntax.tab.c"
    break;

  case 71: /* Exp: ID LP Args RP  */
#line 174 "./syntax.y"
                {(yyval.type_node) = createNode((yyvsp[-3].type_node)->row,Exp,NULL,0,0);(yyval.type_node)->generation = Exp0;addNodes((yyval.type_node),4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2104 "./syntax.tab.c"
    break;

  case 72: /* Exp: ID LP RP  */
#line 175 "./syntax.y"
           {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);(yyval.type_node)->generation = Exp1;addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2110 "./syntax.tab.c"
    break;

  case 73: /* Exp: Exp LB Exp RB  */
#line 176 "./syntax.y"
                {(yyval.type_node) = createNode((yyvsp[-3].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),4,(yyvsp[-3].type_node),(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2116 "./syntax.tab.c"
    break;

  case 74: /* Exp: Exp DOT ID  */
#line 177 "./syntax.y"
             {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Exp,NULL,0,0);(yyval.type_node)->generation = Exp2;addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2122 "./syntax.tab.c"
    break;

  case 75: /* Exp: ID  */
#line 178 "./syntax.y"
     {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Exp,NULL,0,0);(yyval.type_node)->generation = Exp3;addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 2128 "./syntax.tab.c"
    break;

  case 76: /* Exp: INT  */
#line 179 "./syntax.y"
      {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 2134 "./syntax.tab.c"
    break;

  case 77: /* Exp: INT_ERR  */
#line 180 "./syntax.y"
          {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 2140 "./syntax.tab.c"
    break;

  case 78: /* Exp: FLOAT  */
#line 181 "./syntax.y"
        {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 2146 "./syntax.tab.c"
    break;

  case 79: /* Exp: FLOAT_ERR  */
#line 182 "./syntax.y"
            {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Exp,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 2152 "./syntax.tab.c"
    break;

  case 80: /* Exp: Exp ASSIGNOP error  */
#line 184 "./syntax.y"
                     {my_yyerror("error Exp");}
#line 2158 "./syntax.tab.c"
    break;

  case 81: /* Exp: Exp AND error  */
#line 185 "./syntax.y"
                {my_yyerror("error Exp");}
#line 2164 "./syntax.tab.c"
    break;

  case 82: /* Exp: Exp OR error  */
#line 186 "./syntax.y"
               {my_yyerror("error Exp");}
#line 2170 "./syntax.tab.c"
    break;

  case 83: /* Exp: Exp RELOP error  */
#line 187 "./syntax.y"
                  {my_yyerror("error Exp");}
#line 2176 "./syntax.tab.c"
    break;

  case 84: /* Exp: Exp PLUS error  */
#line 188 "./syntax.y"
                 {my_yyerror("error Exp");}
#line 2182 "./syntax.tab.c"
    break;

  case 85: /* Exp: Exp MINUS error  */
#line 189 "./syntax.y"
                  {my_yyerror("error Exp");}
#line 2188 "./syntax.tab.c"
    break;

  case 86: /* Exp: Exp STAR error  */
#line 190 "./syntax.y"
                 {my_yyerror("error Exp");}
#line 2194 "./syntax.tab.c"
    break;

  case 87: /* Exp: Exp DIV error  */
#line 191 "./syntax.y"
                {my_yyerror("error Exp");}
#line 2200 "./syntax.tab.c"
    break;

  case 88: /* Exp: ID LP error RP  */
#line 192 "./syntax.y"
                 {my_yyerror("error Exp");}
#line 2206 "./syntax.tab.c"
    break;

  case 89: /* Exp: Exp LB error RB  */
#line 193 "./syntax.y"
                  {my_yyerror("error Exp");}
#line 2212 "./syntax.tab.c"
    break;

  case 90: /* Args: Exp COMMA Args  */
#line 196 "./syntax.y"
                     {(yyval.type_node) = createNode((yyvsp[-2].type_node)->row,Args,NULL,0,0);addNodes((yyval.type_node),3,(yyvsp[-2].type_node),(yyvsp[-1].type_node),(yyvsp[0].type_node));}
#line 2218 "./syntax.tab.c"
    break;

  case 91: /* Args: Exp  */
#line 197 "./syntax.y"
      {(yyval.type_node) = createNode((yyvsp[0].type_node)->row,Args,NULL,0,0);addNodes((yyval.type_node),1,(yyvsp[0].type_node));}
#line 2224 "./syntax.tab.c"
    break;


#line 2228 "./syntax.tab.c"

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
  *++yylsp = yyloc;

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
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
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
                      yytoken, &yylval, &yylloc);
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

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

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
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 201 "./syntax.y"


void yyerror(const char* msg)
{
    fail = 1;
    extern int yylineno;
	extern char *yytext;
	// int len=strlen(yytext);
	// int i;
	// char buf[512]={0};
	// for (i=0;i<len;++i)
	// {
	// 	sprintf(buf,"%s%d ",buf,yytext[i]);
	// }
	fprintf(stderr, "Error type B at line %d: %s around symbol '%s'\n", yylineno, msg, yytext);
}

void my_yyerror(const char* msg)
{
    fail = 1;
    extern int yylineno;
	extern char *yytext;
	// int len=strlen(yytext);
	// int i;
	// char buf[512]={0};
	// for (i=0;i<len;++i)
	// {
	// 	sprintf(buf,"%s%d ",buf,yytext[i]);
	// }
	fprintf(stderr, "Error type B at line %d: %s around symbol '%s'\n", yylineno, msg, yytext);
}
