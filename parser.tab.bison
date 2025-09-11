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
#line 1 "parser.bison"

    #include <stdio.h> 
    #include "expression.hpp"
    #include "utils.hpp"
    #include <stdlib.h>
    #include <string.h>
    #include <memory>

    #define YYSTYPE Expression*

    extern int yylex();
    extern char* yytext;
    extern char* last_identifier;
    int yyerror(const char*);

    Expression* parser_result{nullptr};

    char* copy_string(const char* src) {
        if (!src) return nullptr;
        char* dst = (char*)malloc(strlen(src) + 1);
        strcpy(dst, src);
        return dst;
    }
    
// Función auxiliar para manejar el resultado del parser
void set_parser_result(Expression* expr) {
    parser_result = expr;
}


#line 102 "parser.tab.bison"

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

#include "token.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_TOKEN_EOF = 3,                  /* TOKEN_EOF  */
  YYSYMBOL_TOKEN_IF = 4,                   /* TOKEN_IF  */
  YYSYMBOL_TOKEN_ELSE = 5,                 /* TOKEN_ELSE  */
  YYSYMBOL_TOKEN_CONCAT = 6,               /* TOKEN_CONCAT  */
  YYSYMBOL_TOKEN_RTOS = 7,                 /* TOKEN_RTOS  */
  YYSYMBOL_TOKEN_ETOS = 8,                 /* TOKEN_ETOS  */
  YYSYMBOL_TOKEN_ETOR = 9,                 /* TOKEN_ETOR  */
  YYSYMBOL_TOKEN_RTOE = 10,                /* TOKEN_RTOE  */
  YYSYMBOL_TOKEN_EMPTY = 11,               /* TOKEN_EMPTY  */
  YYSYMBOL_TOKEN_HEAD = 12,                /* TOKEN_HEAD  */
  YYSYMBOL_TOKEN_TAIL = 13,                /* TOKEN_TAIL  */
  YYSYMBOL_TOKEN_ASIG = 14,                /* TOKEN_ASIG  */
  YYSYMBOL_TOKEN_ADD = 15,                 /* TOKEN_ADD  */
  YYSYMBOL_TOKEN_SUBSTRACT = 16,           /* TOKEN_SUBSTRACT  */
  YYSYMBOL_TOKEN_MULTIPLY = 17,            /* TOKEN_MULTIPLY  */
  YYSYMBOL_TOKEN_DIVIDE = 18,              /* TOKEN_DIVIDE  */
  YYSYMBOL_TOKEN_MOD = 19,                 /* TOKEN_MOD  */
  YYSYMBOL_TOKEN_AND = 20,                 /* TOKEN_AND  */
  YYSYMBOL_TOKEN_OR = 21,                  /* TOKEN_OR  */
  YYSYMBOL_TOKEN_NOT = 22,                 /* TOKEN_NOT  */
  YYSYMBOL_TOKEN_XOR = 23,                 /* TOKEN_XOR  */
  YYSYMBOL_TOKEN_LET = 24,                 /* TOKEN_LET  */
  YYSYMBOL_TOKEN_TRUE = 25,                /* TOKEN_TRUE  */
  YYSYMBOL_TOKEN_FALSE = 26,               /* TOKEN_FALSE  */
  YYSYMBOL_TOKEN_INT = 27,                 /* TOKEN_INT  */
  YYSYMBOL_TOKEN_REAL = 28,                /* TOKEN_REAL  */
  YYSYMBOL_TOKEN_STRING = 29,              /* TOKEN_STRING  */
  YYSYMBOL_TOKEN_FUN = 30,                 /* TOKEN_FUN  */
  YYSYMBOL_TOKEN_IN = 31,                  /* TOKEN_IN  */
  YYSYMBOL_TOKEN_IDENTIFIER = 32,          /* TOKEN_IDENTIFIER  */
  YYSYMBOL_TOKEN_UNKNOWN = 33,             /* TOKEN_UNKNOWN  */
  YYSYMBOL_TOKEN_LPAREN = 34,              /* TOKEN_LPAREN  */
  YYSYMBOL_TOKEN_RPAREN = 35,              /* TOKEN_RPAREN  */
  YYSYMBOL_TOKEN_LCORCH = 36,              /* TOKEN_LCORCH  */
  YYSYMBOL_TOKEN_RCORCH = 37,              /* TOKEN_RCORCH  */
  YYSYMBOL_TOKEN_FST = 38,                 /* TOKEN_FST  */
  YYSYMBOL_TOKEN_SND = 39,                 /* TOKEN_SND  */
  YYSYMBOL_TOKEN_LESS = 40,                /* TOKEN_LESS  */
  YYSYMBOL_TOKEN_GREAT = 41,               /* TOKEN_GREAT  */
  YYSYMBOL_TOKEN_LESSEQL = 42,             /* TOKEN_LESSEQL  */
  YYSYMBOL_TOKEN_GREATEQL = 43,            /* TOKEN_GREATEQL  */
  YYSYMBOL_TOKEN_NOTEQUAL = 44,            /* TOKEN_NOTEQUAL  */
  YYSYMBOL_TOKEN_EQUAL = 45,               /* TOKEN_EQUAL  */
  YYSYMBOL_TOKEN_CONMMENT = 46,            /* TOKEN_CONMMENT  */
  YYSYMBOL_TOKEN_LCONMEN = 47,             /* TOKEN_LCONMEN  */
  YYSYMBOL_TOKEN_RCONMEN = 48,             /* TOKEN_RCONMEN  */
  YYSYMBOL_TOKEN_COMA = 49,                /* TOKEN_COMA  */
  YYSYMBOL_TOKEN_END = 50,                 /* TOKEN_END  */
  YYSYMBOL_TOKEN_PRINT = 51,               /* TOKEN_PRINT  */
  YYSYMBOL_TOKEN_DEL_ARRAY = 52,           /* TOKEN_DEL_ARRAY  */
  YYSYMBOL_TOKEN_ADD_ARRAY = 53,           /* TOKEN_ADD_ARRAY  */
  YYSYMBOL_TOKEN_NARRAY = 54,              /* TOKEN_NARRAY  */
  YYSYMBOL_YYACCEPT = 55,                  /* $accept  */
  YYSYMBOL_program = 56,                   /* program  */
  YYSYMBOL_expr = 57,                      /* expr  */
  YYSYMBOL_variable_declaration = 58,      /* variable_declaration  */
  YYSYMBOL_and_expr = 59,                  /* and_expr  */
  YYSYMBOL_equality_expr = 60,             /* equality_expr  */
  YYSYMBOL_relational_expr = 61,           /* relational_expr  */
  YYSYMBOL_concat_expr = 62,               /* concat_expr  */
  YYSYMBOL_additive_expr = 63,             /* additive_expr  */
  YYSYMBOL_multiplicative_expr = 64,       /* multiplicative_expr  */
  YYSYMBOL_unary_expr = 65,                /* unary_expr  */
  YYSYMBOL_primary_expr = 66,              /* primary_expr  */
  YYSYMBOL_literal = 67,                   /* literal  */
  YYSYMBOL_array_literal = 68,             /* array_literal  */
  YYSYMBOL_pair = 69,                      /* pair  */
  YYSYMBOL_elements = 70,                  /* elements  */
  YYSYMBOL_function_call = 71              /* function_call  */
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
#define YYFINAL  58
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   211

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  55
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  17
/* YYNRULES -- Number of rules.  */
#define YYNRULES  58
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  132

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   309


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
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,    98,    98,   112,   118,   124,   125,   126,   136,   144,
     149,   152,   158,   164,   167,   173,   179,   185,   191,   195,
     202,   206,   212,   218,   221,   227,   233,   239,   242,   244,
     246,   250,   251,   252,   253,   256,   258,   260,   266,   268,
     270,   271,   274,   276,   277,   280,   288,   290,   294,   296,
     298,   300,   302,   304,   306,   308,   310,   315,   321
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
  "\"end of file\"", "error", "\"invalid token\"", "TOKEN_EOF",
  "TOKEN_IF", "TOKEN_ELSE", "TOKEN_CONCAT", "TOKEN_RTOS", "TOKEN_ETOS",
  "TOKEN_ETOR", "TOKEN_RTOE", "TOKEN_EMPTY", "TOKEN_HEAD", "TOKEN_TAIL",
  "TOKEN_ASIG", "TOKEN_ADD", "TOKEN_SUBSTRACT", "TOKEN_MULTIPLY",
  "TOKEN_DIVIDE", "TOKEN_MOD", "TOKEN_AND", "TOKEN_OR", "TOKEN_NOT",
  "TOKEN_XOR", "TOKEN_LET", "TOKEN_TRUE", "TOKEN_FALSE", "TOKEN_INT",
  "TOKEN_REAL", "TOKEN_STRING", "TOKEN_FUN", "TOKEN_IN",
  "TOKEN_IDENTIFIER", "TOKEN_UNKNOWN", "TOKEN_LPAREN", "TOKEN_RPAREN",
  "TOKEN_LCORCH", "TOKEN_RCORCH", "TOKEN_FST", "TOKEN_SND", "TOKEN_LESS",
  "TOKEN_GREAT", "TOKEN_LESSEQL", "TOKEN_GREATEQL", "TOKEN_NOTEQUAL",
  "TOKEN_EQUAL", "TOKEN_CONMMENT", "TOKEN_LCONMEN", "TOKEN_RCONMEN",
  "TOKEN_COMA", "TOKEN_END", "TOKEN_PRINT", "TOKEN_DEL_ARRAY",
  "TOKEN_ADD_ARRAY", "TOKEN_NARRAY", "$accept", "program", "expr",
  "variable_declaration", "and_expr", "equality_expr", "relational_expr",
  "concat_expr", "additive_expr", "multiplicative_expr", "unary_expr",
  "primary_expr", "literal", "array_literal", "pair", "elements",
  "function_call", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-35)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     125,   -25,   -15,    -5,     1,     6,   -35,    10,    15,   158,
     158,    25,   -35,   -35,   -35,   -35,   -35,    18,   125,    87,
      40,    43,    46,    76,    17,   -11,   -35,    41,    73,    42,
      61,   140,    88,   -35,   -35,   -35,   -35,   -35,   -35,   125,
     125,   125,   125,   125,   125,   125,   -35,   -35,    72,   125,
      -7,   -35,   -11,   -34,   125,   125,   125,    -6,   -35,   158,
     158,   158,   158,   158,   158,   158,   158,   158,   158,   158,
     158,   158,   158,   158,    11,    20,    24,    27,    30,    37,
      52,   125,    55,   -35,   125,   -35,   125,    58,   107,   -10,
      71,    41,    41,    73,    42,    42,    61,    61,    61,    61,
     140,    88,    88,   -35,   -35,   -35,   125,   -35,   -35,   -35,
     -35,   -35,   -35,    33,   -35,   123,   -11,   -35,   -35,   125,
     125,    -3,   125,   -35,   168,   172,   125,   -11,   -35,   -35,
     -17,   -35
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     0,     0,     0,    44,     0,     0,     0,
       0,     0,    38,    39,    35,    36,    37,    34,     0,     0,
       0,     0,     0,     0,     0,     2,     6,     5,    10,    13,
      18,    20,    23,    27,    30,    32,    40,    41,    33,     0,
       0,     0,     0,     0,     0,     0,    29,    28,     0,     0,
       0,    43,    47,     0,     0,     0,     0,     0,     1,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    31,     0,    42,     0,     0,     0,     0,
       0,     3,     4,     9,    12,    11,    14,    15,    16,    17,
      19,    21,    22,    24,    25,    26,     0,    50,    51,    52,
      53,    54,    55,     0,    56,     0,    46,    48,    49,     0,
       0,     0,     0,    45,     0,     0,     0,     8,    58,    57,
       0,     7
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -35,   -35,   -18,   -35,   113,    28,   119,   134,    54,   106,
      -2,   -35,   -35,    35,   -35,   -35,   -35
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int8 yydefgoto[] =
{
       0,    24,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    35,    36,    37,    53,    38
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      50,    52,   126,    85,    59,     6,    60,    46,    47,    39,
      59,    59,    60,    60,    59,    86,    60,    58,    59,    40,
      60,    74,    75,    76,    77,    78,    79,    80,    83,    41,
      19,    82,    59,   131,    60,    42,    87,    88,    89,   119,
      43,    59,    84,    60,    44,    59,   106,    60,    59,    45,
      60,    59,    49,    60,    59,   107,    60,    48,    59,   108,
      60,    61,   109,   113,   122,   110,   115,    68,   116,   103,
     104,   105,   111,    59,    54,    60,    59,    55,    60,    59,
      56,    60,    64,    65,    66,    67,    81,   112,   121,    93,
     114,     1,    90,   117,     2,     3,     4,     5,     6,     7,
       8,   124,   125,     9,   127,    71,    72,    73,   130,    10,
      57,    11,    12,    13,    14,    15,    16,    62,    63,    17,
     120,    18,   100,    19,    51,    20,    21,     0,    59,     1,
      60,     0,     2,     3,     4,     5,     6,     7,     8,    22,
      23,     9,   118,     0,    59,     0,    60,    10,     0,    11,
      12,    13,    14,    15,    16,    69,    70,    17,   123,    18,
       0,    19,     0,    20,    21,     2,     3,     4,     5,     6,
       7,     8,    91,    92,     9,   101,   102,    22,    23,     0,
      10,    94,    95,    12,    13,    14,    15,    16,     0,    59,
      17,    60,    18,    59,    19,    60,    20,    21,    96,    97,
      98,    99,     0,   128,     0,     0,     0,   129,     0,     0,
      22,    23
};

static const yytype_int8 yycheck[] =
{
      18,    19,     5,    37,    21,    11,    23,     9,    10,    34,
      21,    21,    23,    23,    21,    49,    23,     0,    21,    34,
      23,    39,    40,    41,    42,    43,    44,    45,    35,    34,
      36,    49,    21,    50,    23,    34,    54,    55,    56,    49,
      34,    21,    49,    23,    34,    21,    35,    23,    21,    34,
      23,    21,    34,    23,    21,    35,    23,    32,    21,    35,
      23,    20,    35,    81,    31,    35,    84,     6,    86,    71,
      72,    73,    35,    21,    34,    23,    21,    34,    23,    21,
      34,    23,    40,    41,    42,    43,    14,    35,   106,    61,
      35,     4,    57,    35,     7,     8,     9,    10,    11,    12,
      13,   119,   120,    16,   122,    17,    18,    19,   126,    22,
      34,    24,    25,    26,    27,    28,    29,    44,    45,    32,
      49,    34,    68,    36,    37,    38,    39,    -1,    21,     4,
      23,    -1,     7,     8,     9,    10,    11,    12,    13,    52,
      53,    16,    35,    -1,    21,    -1,    23,    22,    -1,    24,
      25,    26,    27,    28,    29,    15,    16,    32,    35,    34,
      -1,    36,    -1,    38,    39,     7,     8,     9,    10,    11,
      12,    13,    59,    60,    16,    69,    70,    52,    53,    -1,
      22,    62,    63,    25,    26,    27,    28,    29,    -1,    21,
      32,    23,    34,    21,    36,    23,    38,    39,    64,    65,
      66,    67,    -1,    35,    -1,    -1,    -1,    35,    -1,    -1,
      52,    53
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     4,     7,     8,     9,    10,    11,    12,    13,    16,
      22,    24,    25,    26,    27,    28,    29,    32,    34,    36,
      38,    39,    52,    53,    56,    57,    58,    59,    60,    61,
      62,    63,    64,    65,    66,    67,    68,    69,    71,    34,
      34,    34,    34,    34,    34,    34,    65,    65,    32,    34,
      57,    37,    57,    70,    34,    34,    34,    34,     0,    21,
      23,    20,    44,    45,    40,    41,    42,    43,     6,    15,
      16,    17,    18,    19,    57,    57,    57,    57,    57,    57,
      57,    14,    57,    35,    49,    37,    49,    57,    57,    57,
      68,    59,    59,    60,    61,    61,    62,    62,    62,    62,
      63,    64,    64,    65,    65,    65,    35,    35,    35,    35,
      35,    35,    35,    57,    35,    57,    57,    35,    35,    49,
      49,    57,    31,    35,    57,    57,     5,    57,    35,    35,
      57,    50
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    55,    56,    57,    57,    57,    57,    57,    58,    59,
      59,    60,    60,    60,    61,    61,    61,    61,    61,    62,
      62,    63,    63,    63,    64,    64,    64,    64,    65,    65,
      65,    66,    66,    66,    66,    67,    67,    67,    67,    67,
      67,    67,    68,    68,    68,    69,    70,    70,    71,    71,
      71,    71,    71,    71,    71,    71,    71,    71,    71
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     3,     3,     1,     1,     8,     6,     3,
       1,     3,     3,     1,     3,     3,     3,     3,     1,     3,
       1,     3,     3,     1,     3,     3,     3,     1,     2,     2,
       1,     3,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     3,     2,     1,     5,     3,     1,     4,     4,
       4,     4,     4,     4,     4,     4,     4,     6,     6
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
  case 2: /* program: expr  */
#line 98 "parser.bison"
               { set_parser_result(yyvsp[0]); }
#line 1264 "parser.tab.bison"
    break;

  case 3: /* expr: expr TOKEN_OR and_expr  */
#line 113 "parser.bison"
    {
        yyval = new OrExpression(
        std::shared_ptr<Expression>(yyvsp[-2]), 
        std::shared_ptr<Expression>(yyvsp[0])
    ); }
#line 1274 "parser.tab.bison"
    break;

  case 4: /* expr: expr TOKEN_XOR and_expr  */
#line 119 "parser.bison"
    {
        yyval = new XorExpression(
        std::shared_ptr<Expression>(yyvsp[-2]), 
        std::shared_ptr<Expression>(yyvsp[0])
    ); }
#line 1284 "parser.tab.bison"
    break;

  case 7: /* expr: TOKEN_IF TOKEN_LPAREN expr TOKEN_RPAREN expr TOKEN_ELSE expr TOKEN_END  */
#line 127 "parser.bison"
    {
        yyval = new IfElseExpression( 
            std::shared_ptr<Expression>(yyvsp[-5]), 
            std::shared_ptr<Expression>(yyvsp[-3]), 
            std::shared_ptr<Expression>(yyvsp[-1])
        );
    }
#line 1296 "parser.tab.bison"
    break;

  case 8: /* variable_declaration: TOKEN_LET TOKEN_IDENTIFIER TOKEN_ASIG expr TOKEN_IN expr  */
#line 137 "parser.bison"
    {
        auto var_name = std::make_shared<NameExpression>(copy_string(last_identifier));
        auto var_expr = std::shared_ptr<Expression>(yyvsp[-2]);
        auto body_expr = std::shared_ptr<Expression>(yyvsp[0]);
        yyval = new LetExpression(var_name, var_expr, body_expr);
    }
#line 1307 "parser.tab.bison"
    break;

  case 9: /* and_expr: and_expr TOKEN_AND equality_expr  */
#line 144 "parser.bison"
                                             {
        yyval = new AndExpression(
        std::shared_ptr<Expression>(yyvsp[-2]), 
        std::shared_ptr<Expression>(yyvsp[0])
    ); }
#line 1317 "parser.tab.bison"
    break;

  case 11: /* equality_expr: equality_expr TOKEN_EQUAL relational_expr  */
#line 153 "parser.bison"
        {
            yyval = new EqualExpression(
            std::shared_ptr<Expression>(yyvsp[-2]), 
            std::shared_ptr<Expression>(yyvsp[0])
        ); }
#line 1327 "parser.tab.bison"
    break;

  case 12: /* equality_expr: equality_expr TOKEN_NOTEQUAL relational_expr  */
#line 159 "parser.bison"
        {
            yyval = new NotEqualExpression(
            std::shared_ptr<Expression>(yyvsp[-2]), 
            std::shared_ptr<Expression>(yyvsp[0])
        ); }
#line 1337 "parser.tab.bison"
    break;

  case 14: /* relational_expr: relational_expr TOKEN_LESS concat_expr  */
#line 168 "parser.bison"
            {
                yyval = new LessExpression(
                std::shared_ptr<Expression>(yyvsp[-2]), 
                std::shared_ptr<Expression>(yyvsp[0])
            ); }
#line 1347 "parser.tab.bison"
    break;

  case 15: /* relational_expr: relational_expr TOKEN_GREAT concat_expr  */
#line 174 "parser.bison"
            {
                yyval = new GreaterExpression(
                std::shared_ptr<Expression>(yyvsp[-2]), 
                std::shared_ptr<Expression>(yyvsp[0])
            ); }
#line 1357 "parser.tab.bison"
    break;

  case 16: /* relational_expr: relational_expr TOKEN_LESSEQL concat_expr  */
#line 180 "parser.bison"
            {
                yyval = new LessEqExpression(
                std::shared_ptr<Expression>(yyvsp[-2]), 
                std::shared_ptr<Expression>(yyvsp[0])
            ); }
#line 1367 "parser.tab.bison"
    break;

  case 17: /* relational_expr: relational_expr TOKEN_GREATEQL concat_expr  */
#line 186 "parser.bison"
            {
                yyval = new GreaterEqExpression(
                std::shared_ptr<Expression>(yyvsp[-2]), 
                std::shared_ptr<Expression>(yyvsp[0])
            ); }
#line 1377 "parser.tab.bison"
    break;

  case 19: /* concat_expr: concat_expr TOKEN_CONCAT additive_expr  */
#line 196 "parser.bison"
            {
                yyval = new ConcatExpression(
                std::shared_ptr<Expression>(yyvsp[-2]), 
                std::shared_ptr<Expression>(yyvsp[0])
            ); 
            }
#line 1388 "parser.tab.bison"
    break;

  case 21: /* additive_expr: additive_expr TOKEN_ADD multiplicative_expr  */
#line 207 "parser.bison"
            {
                yyval = new AddExpression(
                std::shared_ptr<Expression>(yyvsp[-2]), 
                std::shared_ptr<Expression>(yyvsp[0])
            ); }
#line 1398 "parser.tab.bison"
    break;

  case 22: /* additive_expr: additive_expr TOKEN_SUBSTRACT multiplicative_expr  */
#line 213 "parser.bison"
            {
                yyval = new SubExpression(
                std::shared_ptr<Expression>(yyvsp[-2]), 
                std::shared_ptr<Expression>(yyvsp[0])
            ); }
#line 1408 "parser.tab.bison"
    break;

  case 24: /* multiplicative_expr: multiplicative_expr TOKEN_MULTIPLY unary_expr  */
#line 222 "parser.bison"
            {
                yyval = new MulExpression(
                std::shared_ptr<Expression>(yyvsp[-2]), 
                std::shared_ptr<Expression>(yyvsp[0])
            ); }
#line 1418 "parser.tab.bison"
    break;

  case 25: /* multiplicative_expr: multiplicative_expr TOKEN_DIVIDE unary_expr  */
#line 228 "parser.bison"
            {
                yyval = new DivExpression(
                std::shared_ptr<Expression>(yyvsp[-2]), 
                std::shared_ptr<Expression>(yyvsp[0])
            ); }
#line 1428 "parser.tab.bison"
    break;

  case 26: /* multiplicative_expr: multiplicative_expr TOKEN_MOD unary_expr  */
#line 234 "parser.bison"
            {
                yyval = new ModExpression(
                std::shared_ptr<Expression>(yyvsp[-2]), 
                std::shared_ptr<Expression>(yyvsp[0])
            ); }
#line 1438 "parser.tab.bison"
    break;

  case 28: /* unary_expr: TOKEN_NOT unary_expr  */
#line 243 "parser.bison"
                { yyval = new NotExpression(std::shared_ptr<Expression>(yyvsp[0])); }
#line 1444 "parser.tab.bison"
    break;

  case 29: /* unary_expr: TOKEN_SUBSTRACT unary_expr  */
#line 245 "parser.bison"
                { yyval = new NegExpression(std::shared_ptr<Expression>(yyvsp[0])); }
#line 1450 "parser.tab.bison"
    break;

  case 31: /* primary_expr: TOKEN_LPAREN expr TOKEN_RPAREN  */
#line 250 "parser.bison"
                                                { yyval = yyvsp[-1]; }
#line 1456 "parser.tab.bison"
    break;

  case 34: /* primary_expr: TOKEN_IDENTIFIER  */
#line 253 "parser.bison"
                                   { yyval = new NameExpression(copy_string(last_identifier)); }
#line 1462 "parser.tab.bison"
    break;

  case 35: /* literal: TOKEN_INT  */
#line 257 "parser.bison"
            { yyval = new IntExpression(atoi(yytext)); }
#line 1468 "parser.tab.bison"
    break;

  case 36: /* literal: TOKEN_REAL  */
#line 259 "parser.bison"
            { yyval = new RealExpression(atof(yytext)); }
#line 1474 "parser.tab.bison"
    break;

  case 37: /* literal: TOKEN_STRING  */
#line 261 "parser.bison"
            { 
                std::string str(yytext);
                str = str.substr(1, str.length() - 2);
                yyval = new StrExpression(str);
            }
#line 1484 "parser.tab.bison"
    break;

  case 38: /* literal: TOKEN_TRUE  */
#line 267 "parser.bison"
            { yyval = new BoolExpression(true); }
#line 1490 "parser.tab.bison"
    break;

  case 39: /* literal: TOKEN_FALSE  */
#line 269 "parser.bison"
            { yyval = new BoolExpression(false); }
#line 1496 "parser.tab.bison"
    break;

  case 42: /* array_literal: TOKEN_LCORCH elements TOKEN_RCORCH  */
#line 275 "parser.bison"
                { yyval = yyvsp[-1]; }
#line 1502 "parser.tab.bison"
    break;

  case 45: /* pair: TOKEN_LPAREN expr TOKEN_COMA expr TOKEN_RPAREN  */
#line 281 "parser.bison"
    {
        yyval = new PairExpression(
            std::shared_ptr<Expression>(yyvsp[-3]),
            std::shared_ptr<Expression>(yyvsp[-1])
        );
    }
#line 1513 "parser.tab.bison"
    break;

  case 46: /* elements: elements TOKEN_COMA expr  */
#line 289 "parser.bison"
            { yyval = yyvsp[-2]; }
#line 1519 "parser.tab.bison"
    break;

  case 47: /* elements: expr  */
#line 291 "parser.bison"
            { yyval = yyvsp[0]; }
#line 1525 "parser.tab.bison"
    break;

  case 48: /* function_call: TOKEN_FST TOKEN_LPAREN expr TOKEN_RPAREN  */
#line 295 "parser.bison"
                { yyval = new FstExpression(std::shared_ptr<Expression>(yyvsp[-1])); }
#line 1531 "parser.tab.bison"
    break;

  case 49: /* function_call: TOKEN_SND TOKEN_LPAREN expr TOKEN_RPAREN  */
#line 297 "parser.bison"
                { yyval = new SndExpression(std::shared_ptr<Expression>(yyvsp[-1])); }
#line 1537 "parser.tab.bison"
    break;

  case 50: /* function_call: TOKEN_RTOS TOKEN_LPAREN expr TOKEN_RPAREN  */
#line 299 "parser.bison"
                { yyval = new RtoSExpression(std::shared_ptr<Expression>(yyvsp[-1])); }
#line 1543 "parser.tab.bison"
    break;

  case 51: /* function_call: TOKEN_ETOS TOKEN_LPAREN expr TOKEN_RPAREN  */
#line 301 "parser.bison"
                { yyval = new ItoSExpression(std::shared_ptr<Expression>(yyvsp[-1])); }
#line 1549 "parser.tab.bison"
    break;

  case 52: /* function_call: TOKEN_ETOR TOKEN_LPAREN expr TOKEN_RPAREN  */
#line 303 "parser.bison"
                { yyval = new ItoRExpression(std::shared_ptr<Expression>(yyvsp[-1])); }
#line 1555 "parser.tab.bison"
    break;

  case 53: /* function_call: TOKEN_RTOE TOKEN_LPAREN expr TOKEN_RPAREN  */
#line 305 "parser.bison"
                { yyval = new RtoIExpression(std::shared_ptr<Expression>(yyvsp[-1])); }
#line 1561 "parser.tab.bison"
    break;

  case 54: /* function_call: TOKEN_HEAD TOKEN_LPAREN expr TOKEN_RPAREN  */
#line 307 "parser.bison"
                { yyval = new HeadExpression(std::shared_ptr<Expression>(yyvsp[-1])); }
#line 1567 "parser.tab.bison"
    break;

  case 55: /* function_call: TOKEN_TAIL TOKEN_LPAREN expr TOKEN_RPAREN  */
#line 309 "parser.bison"
                { yyval = new TailExpression(std::shared_ptr<Expression>(yyvsp[-1])); }
#line 1573 "parser.tab.bison"
    break;

  case 56: /* function_call: TOKEN_IDENTIFIER TOKEN_LPAREN expr TOKEN_RPAREN  */
#line 311 "parser.bison"
                { 
                    auto func_name = std::make_shared<NameExpression>(copy_string(last_identifier));
                    yyval = new CallExpression(func_name, std::shared_ptr<Expression>(yyvsp[-1]));
                }
#line 1582 "parser.tab.bison"
    break;

  case 57: /* function_call: TOKEN_ADD_ARRAY TOKEN_LPAREN array_literal TOKEN_COMA expr TOKEN_RPAREN  */
#line 316 "parser.bison"
                {
                    yyval = new ArrayAddExpression(
                    std::shared_ptr<Expression>(yyvsp[-3]), 
                    std::shared_ptr<Expression>(yyvsp[-1])
                ); }
#line 1592 "parser.tab.bison"
    break;

  case 58: /* function_call: TOKEN_DEL_ARRAY TOKEN_LPAREN expr TOKEN_COMA expr TOKEN_RPAREN  */
#line 322 "parser.bison"
                 {
                    yyval = new ArrayDelExpression(
                    std::shared_ptr<Expression>(yyvsp[-3]), 
                    std::shared_ptr<Expression>(yyvsp[-1])
                ); }
#line 1602 "parser.tab.bison"
    break;


#line 1606 "parser.tab.bison"

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

#line 331 "parser.bison"
 /* ---------- user code ---------- */

int yyerror(const char* s) {
    printf("Parse error: %s\n", s);
        return 1;
    }
