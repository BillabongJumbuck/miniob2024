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
#define YYPURE 2

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 2 "yacc_sql.y"


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <algorithm>

#include "common/log/log.h"
#include "common/lang/string.h"
#include "sql/parser/parse_defs.h"
#include "sql/parser/yacc_sql.hpp"
#include "sql/parser/lex_sql.h"
#include "sql/expr/expression.h"

using namespace std;

string token_name(const char *sql_string, YYLTYPE *llocp)
{
  return string(sql_string + llocp->first_column, llocp->last_column - llocp->first_column + 1);
}

int yyerror(YYLTYPE *llocp, const char *sql_string, ParsedSqlResult *sql_result, yyscan_t scanner, const char *msg)
{
  std::unique_ptr<ParsedSqlNode> error_sql_node = std::make_unique<ParsedSqlNode>(SCF_ERROR);
  error_sql_node->error.error_msg = msg;
  error_sql_node->error.line = llocp->first_line;
  error_sql_node->error.column = llocp->first_column;
  sql_result->add_sql_node(std::move(error_sql_node));
  return 0;
}

ArithmeticExpr *create_arithmetic_expression(ArithmeticExpr::Type type,
                                             Expression *left,
                                             Expression *right,
                                             const char *sql_string,
                                             YYLTYPE *llocp)
{
  ArithmeticExpr *expr = new ArithmeticExpr(type, left, right);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}

UnboundAggregateExpr *create_aggregate_expression(const char *aggregate_name,
                                           Expression *child,
                                           const char *sql_string,
                                           YYLTYPE *llocp)
{
  UnboundAggregateExpr *expr = new UnboundAggregateExpr(aggregate_name, child);
  expr->set_name(token_name(sql_string, llocp));
  return expr;
}


#line 125 "yacc_sql.cpp"

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

#include "yacc_sql.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_SEMICOLON = 3,                  /* SEMICOLON  */
  YYSYMBOL_BY = 4,                         /* BY  */
  YYSYMBOL_CREATE = 5,                     /* CREATE  */
  YYSYMBOL_DROP = 6,                       /* DROP  */
  YYSYMBOL_GROUP = 7,                      /* GROUP  */
  YYSYMBOL_TABLE = 8,                      /* TABLE  */
  YYSYMBOL_TABLES = 9,                     /* TABLES  */
  YYSYMBOL_INDEX = 10,                     /* INDEX  */
  YYSYMBOL_CALC = 11,                      /* CALC  */
  YYSYMBOL_SELECT = 12,                    /* SELECT  */
  YYSYMBOL_DESC = 13,                      /* DESC  */
  YYSYMBOL_SHOW = 14,                      /* SHOW  */
  YYSYMBOL_SYNC = 15,                      /* SYNC  */
  YYSYMBOL_INSERT = 16,                    /* INSERT  */
  YYSYMBOL_DELETE = 17,                    /* DELETE  */
  YYSYMBOL_UPDATE = 18,                    /* UPDATE  */
  YYSYMBOL_LBRACE = 19,                    /* LBRACE  */
  YYSYMBOL_RBRACE = 20,                    /* RBRACE  */
  YYSYMBOL_COMMA = 21,                     /* COMMA  */
  YYSYMBOL_TRX_BEGIN = 22,                 /* TRX_BEGIN  */
  YYSYMBOL_TRX_COMMIT = 23,                /* TRX_COMMIT  */
  YYSYMBOL_TRX_ROLLBACK = 24,              /* TRX_ROLLBACK  */
  YYSYMBOL_INT_T = 25,                     /* INT_T  */
  YYSYMBOL_STRING_T = 26,                  /* STRING_T  */
  YYSYMBOL_DATE_T = 27,                    /* DATE_T  */
  YYSYMBOL_FLOAT_T = 28,                   /* FLOAT_T  */
  YYSYMBOL_VECTOR_T = 29,                  /* VECTOR_T  */
  YYSYMBOL_HELP = 30,                      /* HELP  */
  YYSYMBOL_EXIT = 31,                      /* EXIT  */
  YYSYMBOL_DOT = 32,                       /* DOT  */
  YYSYMBOL_INTO = 33,                      /* INTO  */
  YYSYMBOL_VALUES = 34,                    /* VALUES  */
  YYSYMBOL_FROM = 35,                      /* FROM  */
  YYSYMBOL_WHERE = 36,                     /* WHERE  */
  YYSYMBOL_AND = 37,                       /* AND  */
  YYSYMBOL_SET = 38,                       /* SET  */
  YYSYMBOL_ON = 39,                        /* ON  */
  YYSYMBOL_LOAD = 40,                      /* LOAD  */
  YYSYMBOL_DATA = 41,                      /* DATA  */
  YYSYMBOL_INFILE = 42,                    /* INFILE  */
  YYSYMBOL_EXPLAIN = 43,                   /* EXPLAIN  */
  YYSYMBOL_STORAGE = 44,                   /* STORAGE  */
  YYSYMBOL_FORMAT = 45,                    /* FORMAT  */
  YYSYMBOL_EQ = 46,                        /* EQ  */
  YYSYMBOL_LT = 47,                        /* LT  */
  YYSYMBOL_GT = 48,                        /* GT  */
  YYSYMBOL_LE = 49,                        /* LE  */
  YYSYMBOL_GE = 50,                        /* GE  */
  YYSYMBOL_NE = 51,                        /* NE  */
  YYSYMBOL_LIKE_OP = 52,                   /* LIKE_OP  */
  YYSYMBOL_NOT = 53,                       /* NOT  */
  YYSYMBOL_NULL_TYPE = 54,                 /* NULL_TYPE  */
  YYSYMBOL_NULLABLE = 55,                  /* NULLABLE  */
  YYSYMBOL_IS = 56,                        /* IS  */
  YYSYMBOL_JOIN = 57,                      /* JOIN  */
  YYSYMBOL_INNER = 58,                     /* INNER  */
  YYSYMBOL_L2_DISTANCE = 59,               /* L2_DISTANCE  */
  YYSYMBOL_COSINE_DISTANCE = 60,           /* COSINE_DISTANCE  */
  YYSYMBOL_INNER_PRODUCT = 61,             /* INNER_PRODUCT  */
  YYSYMBOL_IN_OP = 62,                     /* IN_OP  */
  YYSYMBOL_EXISTS_OP = 63,                 /* EXISTS_OP  */
  YYSYMBOL_UNIQUE = 64,                    /* UNIQUE  */
  YYSYMBOL_LENGTH = 65,                    /* LENGTH  */
  YYSYMBOL_ROUND = 66,                     /* ROUND  */
  YYSYMBOL_DATE_FORMAT = 67,               /* DATE_FORMAT  */
  YYSYMBOL_AS = 68,                        /* AS  */
  YYSYMBOL_ORDER = 69,                     /* ORDER  */
  YYSYMBOL_ASC = 70,                       /* ASC  */
  YYSYMBOL_NUMBER = 71,                    /* NUMBER  */
  YYSYMBOL_FLOAT = 72,                     /* FLOAT  */
  YYSYMBOL_ID = 73,                        /* ID  */
  YYSYMBOL_SSS = 74,                       /* SSS  */
  YYSYMBOL_OR = 75,                        /* OR  */
  YYSYMBOL_76_ = 76,                       /* '+'  */
  YYSYMBOL_77_ = 77,                       /* '-'  */
  YYSYMBOL_78_ = 78,                       /* '*'  */
  YYSYMBOL_79_ = 79,                       /* '/'  */
  YYSYMBOL_UMINUS = 80,                    /* UMINUS  */
  YYSYMBOL_YYACCEPT = 81,                  /* $accept  */
  YYSYMBOL_commands = 82,                  /* commands  */
  YYSYMBOL_command_wrapper = 83,           /* command_wrapper  */
  YYSYMBOL_exit_stmt = 84,                 /* exit_stmt  */
  YYSYMBOL_help_stmt = 85,                 /* help_stmt  */
  YYSYMBOL_sync_stmt = 86,                 /* sync_stmt  */
  YYSYMBOL_begin_stmt = 87,                /* begin_stmt  */
  YYSYMBOL_commit_stmt = 88,               /* commit_stmt  */
  YYSYMBOL_rollback_stmt = 89,             /* rollback_stmt  */
  YYSYMBOL_drop_table_stmt = 90,           /* drop_table_stmt  */
  YYSYMBOL_show_tables_stmt = 91,          /* show_tables_stmt  */
  YYSYMBOL_desc_table_stmt = 92,           /* desc_table_stmt  */
  YYSYMBOL_create_index_stmt = 93,         /* create_index_stmt  */
  YYSYMBOL_unique_tag = 94,                /* unique_tag  */
  YYSYMBOL_drop_index_stmt = 95,           /* drop_index_stmt  */
  YYSYMBOL_create_table_stmt = 96,         /* create_table_stmt  */
  YYSYMBOL_attr_def_list = 97,             /* attr_def_list  */
  YYSYMBOL_attr_def = 98,                  /* attr_def  */
  YYSYMBOL_null_option = 99,               /* null_option  */
  YYSYMBOL_number = 100,                   /* number  */
  YYSYMBOL_type = 101,                     /* type  */
  YYSYMBOL_insert_stmt = 102,              /* insert_stmt  */
  YYSYMBOL_value_list = 103,               /* value_list  */
  YYSYMBOL_value = 104,                    /* value  */
  YYSYMBOL_storage_format = 105,           /* storage_format  */
  YYSYMBOL_delete_stmt = 106,              /* delete_stmt  */
  YYSYMBOL_update_stmt = 107,              /* update_stmt  */
  YYSYMBOL_update_item_list = 108,         /* update_item_list  */
  YYSYMBOL_update_item = 109,              /* update_item  */
  YYSYMBOL_select_stmt = 110,              /* select_stmt  */
  YYSYMBOL_calc_stmt = 111,                /* calc_stmt  */
  YYSYMBOL_expression_list = 112,          /* expression_list  */
  YYSYMBOL_alias = 113,                    /* alias  */
  YYSYMBOL_expression = 114,               /* expression  */
  YYSYMBOL_aggr_func_expr = 115,           /* aggr_func_expr  */
  YYSYMBOL_sub_query_expr = 116,           /* sub_query_expr  */
  YYSYMBOL_value_list_expr = 117,          /* value_list_expr  */
  YYSYMBOL_value_list_ssq = 118,           /* value_list_ssq  */
  YYSYMBOL_func_expr = 119,                /* func_expr  */
  YYSYMBOL_func_type = 120,                /* func_type  */
  YYSYMBOL_rel_attr = 121,                 /* rel_attr  */
  YYSYMBOL_relation = 122,                 /* relation  */
  YYSYMBOL_rel_list = 123,                 /* rel_list  */
  YYSYMBOL_relation_with_alias = 124,      /* relation_with_alias  */
  YYSYMBOL_join_list = 125,                /* join_list  */
  YYSYMBOL_from_list = 126,                /* from_list  */
  YYSYMBOL_where = 127,                    /* where  */
  YYSYMBOL_condition = 128,                /* condition  */
  YYSYMBOL_comp_op = 129,                  /* comp_op  */
  YYSYMBOL_is_null_comp = 130,             /* is_null_comp  */
  YYSYMBOL_group_by = 131,                 /* group_by  */
  YYSYMBOL_order_by = 132,                 /* order_by  */
  YYSYMBOL_order_by_items = 133,           /* order_by_items  */
  YYSYMBOL_order_by_item = 134,            /* order_by_item  */
  YYSYMBOL_load_data_stmt = 135,           /* load_data_stmt  */
  YYSYMBOL_explain_stmt = 136,             /* explain_stmt  */
  YYSYMBOL_set_variable_stmt = 137,        /* set_variable_stmt  */
  YYSYMBOL_opt_semicolon = 138             /* opt_semicolon  */
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
typedef yytype_int16 yy_state_t;

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
#define YYFINAL  78
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   288

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  81
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  58
/* YYNRULES -- Number of rules.  */
#define YYNRULES  143
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  261

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   331


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
       2,     2,    78,    76,     2,    77,     2,    79,     2,     2,
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    80
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   237,   237,   245,   246,   247,   248,   249,   250,   251,
     252,   253,   254,   255,   256,   257,   258,   259,   260,   261,
     262,   263,   264,   268,   274,   279,   285,   291,   297,   303,
     310,   316,   324,   344,   347,   354,   364,   388,   391,   404,
     413,   426,   429,   433,   437,   443,   446,   447,   448,   449,
     450,   453,   470,   473,   484,   488,   492,   496,   500,   506,
     513,   516,   523,   534,   551,   557,   570,   578,   586,   616,
     625,   633,   648,   651,   654,   660,   663,   666,   669,   672,
     676,   679,   684,   690,   693,   696,   699,   702,   705,   709,
     712,   715,   721,   728,   736,   751,   754,   766,   775,   778,
     781,   787,   792,   802,   807,   812,   825,   838,   842,   850,
     854,   867,   870,   875,   878,   883,   886,   892,   893,   894,
     895,   896,   897,   898,   899,   900,   901,   902,   903,   907,
     911,   920,   927,   930,   937,   941,   954,   960,   966,   974,
     987,   995,  1005,  1006
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
  "\"end of file\"", "error", "\"invalid token\"", "SEMICOLON", "BY",
  "CREATE", "DROP", "GROUP", "TABLE", "TABLES", "INDEX", "CALC", "SELECT",
  "DESC", "SHOW", "SYNC", "INSERT", "DELETE", "UPDATE", "LBRACE", "RBRACE",
  "COMMA", "TRX_BEGIN", "TRX_COMMIT", "TRX_ROLLBACK", "INT_T", "STRING_T",
  "DATE_T", "FLOAT_T", "VECTOR_T", "HELP", "EXIT", "DOT", "INTO", "VALUES",
  "FROM", "WHERE", "AND", "SET", "ON", "LOAD", "DATA", "INFILE", "EXPLAIN",
  "STORAGE", "FORMAT", "EQ", "LT", "GT", "LE", "GE", "NE", "LIKE_OP",
  "NOT", "NULL_TYPE", "NULLABLE", "IS", "JOIN", "INNER", "L2_DISTANCE",
  "COSINE_DISTANCE", "INNER_PRODUCT", "IN_OP", "EXISTS_OP", "UNIQUE",
  "LENGTH", "ROUND", "DATE_FORMAT", "AS", "ORDER", "ASC", "NUMBER",
  "FLOAT", "ID", "SSS", "OR", "'+'", "'-'", "'*'", "'/'", "UMINUS",
  "$accept", "commands", "command_wrapper", "exit_stmt", "help_stmt",
  "sync_stmt", "begin_stmt", "commit_stmt", "rollback_stmt",
  "drop_table_stmt", "show_tables_stmt", "desc_table_stmt",
  "create_index_stmt", "unique_tag", "drop_index_stmt",
  "create_table_stmt", "attr_def_list", "attr_def", "null_option",
  "number", "type", "insert_stmt", "value_list", "value", "storage_format",
  "delete_stmt", "update_stmt", "update_item_list", "update_item",
  "select_stmt", "calc_stmt", "expression_list", "alias", "expression",
  "aggr_func_expr", "sub_query_expr", "value_list_expr", "value_list_ssq",
  "func_expr", "func_type", "rel_attr", "relation", "rel_list",
  "relation_with_alias", "join_list", "from_list", "where", "condition",
  "comp_op", "is_null_comp", "group_by", "order_by", "order_by_items",
  "order_by_item", "load_data_stmt", "explain_stmt", "set_variable_stmt",
  "opt_semicolon", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-233)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
     184,    -3,    39,    62,    62,   -58,    16,  -233,    17,     3,
     -11,  -233,  -233,  -233,  -233,  -233,    -6,    28,   184,    89,
      77,  -233,  -233,  -233,  -233,  -233,  -233,  -233,  -233,  -233,
    -233,  -233,  -233,  -233,  -233,  -233,  -233,  -233,  -233,  -233,
    -233,    26,  -233,   110,    52,    58,     5,  -233,   118,   119,
     122,  -233,  -233,  -233,  -233,  -233,    -5,  -233,   105,  -233,
    -233,  -233,   -57,  -233,  -233,  -233,  -233,   129,  -233,   114,
    -233,  -233,    94,    95,   112,   106,   132,  -233,  -233,  -233,
    -233,   156,   107,  -233,   142,   163,   172,     8,    62,    62,
      62,    62,   -55,  -233,  -233,  -233,   120,  -233,    62,    62,
      62,    62,   173,    62,   136,   176,   175,   139,   -41,   147,
     140,   186,   153,   -41,   203,  -233,  -233,    35,    79,   109,
      15,  -233,  -233,  -233,   -21,   -21,  -233,  -233,    62,   216,
    -233,   -39,  -233,   -17,   175,   218,    62,  -233,   193,   175,
     219,    47,  -233,   208,   191,   221,   170,  -233,   163,  -233,
      62,    62,    62,  -233,  -233,  -233,  -233,   136,   189,  -233,
     -41,   182,   -27,    62,  -233,   139,  -233,  -233,   239,  -233,
    -233,  -233,  -233,  -233,   150,   140,   228,   230,  -233,    19,
      31,    68,  -233,   136,   181,   231,  -233,  -233,  -233,  -233,
    -233,  -233,  -233,    80,    49,  -233,  -233,    62,  -233,    62,
      62,    84,  -233,   178,   183,   199,  -233,  -233,  -233,   221,
     211,   136,  -233,  -233,  -233,   217,   253,  -233,   -41,   242,
    -233,  -233,  -233,   209,  -233,    84,  -233,   227,  -233,  -233,
     245,  -233,  -233,   222,  -233,   247,   246,    62,   196,   231,
    -233,  -233,    20,   224,   136,  -233,   -27,   240,   -10,  -233,
     250,  -233,  -233,   200,  -233,   201,  -233,  -233,   196,  -233,
    -233
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       0,    33,     0,     0,     0,     0,     0,    25,     0,     0,
       0,    26,    27,    28,    24,    23,     0,     0,     0,     0,
     142,    22,    21,    14,    15,    16,    17,     9,    10,    11,
      12,    13,     8,     5,     7,     6,     4,     3,    18,    19,
      20,     0,    34,     0,     0,     0,     0,    59,     0,     0,
       0,    98,    99,   100,    54,    56,   101,    58,     0,    83,
      81,    69,    72,    86,    87,    88,    85,     0,    82,    67,
      31,    30,     0,     0,     0,     0,     0,   140,     1,   143,
       2,     0,     0,    29,     0,    81,     0,     0,     0,     0,
       0,     0,     0,    54,    56,    80,     0,    73,     0,     0,
       0,     0,    70,     0,     0,     0,   111,     0,     0,     0,
       0,     0,     0,     0,     0,    93,    79,     0,     0,     0,
       0,   102,    84,    74,    75,    76,    77,    78,     0,     0,
     103,    72,   107,   109,   111,     0,     0,    62,     0,   111,
      64,     0,   141,     0,     0,    37,     0,    35,    95,    94,
       0,     0,     0,    92,    71,    97,   106,     0,     0,   131,
       0,     0,   112,     0,    63,     0,    55,    57,     0,    46,
      47,    48,    49,    50,    41,     0,     0,     0,    96,     0,
       0,     0,   110,     0,   132,    52,   117,   118,   119,   120,
     121,   122,   123,     0,     0,   125,   127,     0,   114,     0,
       0,    66,    65,     0,     0,     0,    43,    42,    40,    37,
      60,     0,    89,    90,    91,     0,     0,    68,     0,     0,
     124,   126,   128,     0,   129,   113,   115,   116,   139,    45,
       0,    44,    38,     0,    36,   104,     0,     0,     0,    52,
      51,   130,    41,     0,     0,    32,   108,   101,   136,   133,
     134,    53,    39,     0,   105,     0,   138,   137,     0,    61,
     135
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -233,  -233,   257,  -233,  -233,  -233,  -233,  -233,  -233,  -233,
    -233,  -233,  -233,  -233,  -233,  -233,    67,   102,    36,  -233,
    -233,  -233,    40,   -45,  -233,  -233,  -233,   115,  -233,   235,
    -233,    -2,   151,   -46,  -233,  -233,  -233,   135,  -233,  -233,
    -232,  -204,    41,   101,  -233,   130,  -102,  -191,  -233,  -233,
    -233,  -233,    30,  -233,  -233,  -233,  -233,  -233
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,    19,    20,    21,    22,    23,    24,    25,    26,    27,
      28,    29,    30,    43,    31,    32,   176,   145,   208,   230,
     174,    33,   219,    60,   234,    34,    35,   139,   140,    36,
      37,    61,   102,    62,    63,    64,    65,   114,    66,    67,
      68,   131,   236,   132,   133,   134,   137,   162,   197,   198,
     184,   217,   249,   250,    38,    39,    40,    80
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      87,    85,    69,   256,   157,    41,   248,   235,   226,   227,
     199,    96,    95,    47,    91,    70,    97,     4,   121,    98,
      99,   100,   101,   122,    46,    71,   248,    92,   116,    96,
      54,    55,   159,    57,    97,   153,   141,   164,    73,   212,
     235,   158,   117,   118,   119,   120,   246,    44,   200,    45,
      72,   213,   124,   125,   126,   127,   150,   100,   101,    47,
     257,    42,    74,   142,    48,    49,    50,    75,   148,    76,
      51,    52,    53,   205,   206,   207,    54,    55,    56,    57,
      79,    46,    58,    59,    98,    99,   100,   101,   214,    78,
     161,    98,    99,   100,   101,    98,    99,   100,   101,    81,
     151,   129,   223,   224,   179,   180,   181,    98,    99,   100,
     101,    98,    99,   100,   101,   185,    47,   201,   166,   167,
      82,    48,    49,    50,    46,    83,   154,    51,    52,    53,
     152,    84,   220,    54,    55,    56,    57,    88,    89,    58,
      59,    90,   221,   222,    98,    99,   100,   101,   103,   104,
     107,   225,   108,   161,   161,    98,    99,   100,   101,    47,
      98,    99,   100,   101,    48,    49,    50,   105,   106,   204,
      51,    52,    53,   239,   109,   110,    93,    94,    56,    57,
     111,   112,    58,    59,   113,    98,    99,   100,   101,     1,
       2,   161,   115,   123,   128,     3,     4,     5,     6,     7,
       8,     9,    10,   205,   206,   207,    11,    12,    13,   130,
     135,   136,   138,   144,    14,    15,   169,   170,   171,   172,
     173,   143,    16,   149,    17,   146,   147,    18,   186,   187,
     188,   189,   190,   191,   192,   193,   155,   160,   194,   163,
     165,   168,   175,   177,   195,   196,   183,   203,   210,   211,
     216,   228,   218,   231,   229,   233,   237,   238,    98,    99,
     100,   101,   240,   241,   199,   242,   245,   243,   244,   247,
     253,   258,   255,   259,   121,    77,   232,   209,   252,   251,
     202,    86,   156,   178,   215,   254,     0,   182,   260
};

static const yytype_int16 yycheck[] =
{
      46,    46,     4,    13,    21,     8,   238,   211,   199,   200,
      37,    68,    58,    54,    19,    73,    73,    12,    73,    76,
      77,    78,    79,    78,    19,     9,   258,    32,    20,    68,
      71,    72,   134,    74,    73,    20,    77,   139,    35,    20,
     244,    58,    88,    89,    90,    91,   237,     8,    75,    10,
      33,    20,    98,    99,   100,   101,    21,    78,    79,    54,
      70,    64,    73,   108,    59,    60,    61,    73,   113,    41,
      65,    66,    67,    53,    54,    55,    71,    72,    73,    74,
       3,    19,    77,    78,    76,    77,    78,    79,    20,     0,
     136,    76,    77,    78,    79,    76,    77,    78,    79,    73,
      21,   103,    53,    54,   150,   151,   152,    76,    77,    78,
      79,    76,    77,    78,    79,   160,    54,   163,    71,    72,
      10,    59,    60,    61,    19,    73,   128,    65,    66,    67,
      21,    73,    52,    71,    72,    73,    74,    19,    19,    77,
      78,    19,    62,    63,    76,    77,    78,    79,    19,    35,
      38,   197,    46,   199,   200,    76,    77,    78,    79,    54,
      76,    77,    78,    79,    59,    60,    61,    73,    73,    19,
      65,    66,    67,   218,    42,    19,    71,    72,    73,    74,
      73,    39,    77,    78,    21,    76,    77,    78,    79,     5,
       6,   237,    20,    73,    21,    11,    12,    13,    14,    15,
      16,    17,    18,    53,    54,    55,    22,    23,    24,    73,
      34,    36,    73,    73,    30,    31,    25,    26,    27,    28,
      29,    74,    38,    20,    40,    39,    73,    43,    46,    47,
      48,    49,    50,    51,    52,    53,    20,    19,    56,    46,
      21,    33,    21,    73,    62,    63,    57,     8,    20,    19,
      69,    73,    21,    54,    71,    44,    39,     4,    76,    77,
      78,    79,    20,    54,    37,    20,    20,    45,    21,    73,
      46,    21,    32,    73,    73,    18,   209,   175,   242,   239,
     165,    46,   131,   148,   183,   244,    -1,   157,   258
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     5,     6,    11,    12,    13,    14,    15,    16,    17,
      18,    22,    23,    24,    30,    31,    38,    40,    43,    82,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      93,    95,    96,   102,   106,   107,   110,   111,   135,   136,
     137,     8,    64,    94,     8,    10,    19,    54,    59,    60,
      61,    65,    66,    67,    71,    72,    73,    74,    77,    78,
     104,   112,   114,   115,   116,   117,   119,   120,   121,   112,
      73,     9,    33,    35,    73,    73,    41,    83,     0,     3,
     138,    73,    10,    73,    73,   104,   110,   114,    19,    19,
      19,    19,    32,    71,    72,   114,    68,    73,    76,    77,
      78,    79,   113,    19,    35,    73,    73,    38,    46,    42,
      19,    73,    39,    21,   118,    20,    20,   114,   114,   114,
     114,    73,    78,    73,   114,   114,   114,   114,    21,   112,
      73,   122,   124,   125,   126,    34,    36,   127,    73,   108,
     109,    77,   104,    74,    73,    98,    39,    73,   104,    20,
      21,    21,    21,    20,   112,    20,   113,    21,    58,   127,
      19,   114,   128,    46,   127,    21,    71,    72,    33,    25,
      26,    27,    28,    29,   101,    21,    97,    73,   118,   114,
     114,   114,   126,    57,   131,   104,    46,    47,    48,    49,
      50,    51,    52,    53,    56,    62,    63,   129,   130,    37,
      75,   114,   108,     8,    19,    53,    54,    55,    99,    98,
      20,    19,    20,    20,    20,   124,    69,   132,    21,   103,
      52,    62,    63,    53,    54,   114,   128,   128,    73,    71,
     100,    54,    97,    44,   105,   122,   123,    39,     4,   104,
      20,    54,    20,    45,    21,    20,   128,    73,   121,   133,
     134,   103,    99,    46,   123,    32,    13,    70,    21,    73,
     133
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_uint8 yyr1[] =
{
       0,    81,    82,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    83,    83,    83,    83,    83,    83,    83,
      83,    83,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,    93,    94,    94,    95,    96,    97,    97,    98,
      98,    99,    99,    99,    99,   100,   101,   101,   101,   101,
     101,   102,   103,   103,   104,   104,   104,   104,   104,   104,
     105,   105,   106,   107,   108,   108,   109,   110,   110,   111,
     112,   112,   113,   113,   113,   114,   114,   114,   114,   114,
     114,   114,   114,   114,   114,   114,   114,   114,   114,   114,
     114,   114,   115,   116,   117,   118,   118,   119,   120,   120,
     120,   121,   121,   122,   123,   123,   124,   125,   125,   126,
     126,   127,   127,   128,   128,   128,   128,   129,   129,   129,
     129,   129,   129,   129,   129,   129,   129,   129,   129,   130,
     130,   131,   132,   132,   133,   133,   134,   134,   134,   135,
     136,   137,   138,   138
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     2,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     3,
       2,     2,     9,     0,     1,     5,     8,     0,     3,     6,
       3,     0,     1,     1,     2,     1,     1,     1,     1,     1,
       1,     8,     0,     3,     1,     2,     1,     2,     1,     1,
       0,     4,     4,     5,     1,     3,     3,     2,     7,     2,
       2,     4,     0,     1,     2,     3,     3,     3,     3,     3,
       2,     1,     1,     1,     3,     1,     1,     1,     1,     6,
       6,     6,     4,     3,     4,     0,     3,     4,     1,     1,
       1,     1,     3,     1,     1,     3,     2,     1,     6,     1,
       3,     0,     2,     3,     2,     3,     3,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     2,     1,     2,     2,
       3,     0,     0,     3,     1,     3,     1,     2,     2,     7,
       2,     4,     0,     1
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
        yyerror (&yylloc, sql_string, sql_result, scanner, YY_("syntax error: cannot back up")); \
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
                  Kind, Value, Location, sql_string, sql_result, scanner); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
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
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, sql_string, sql_result, scanner);
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
                 int yyrule, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
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
                       &(yylsp[(yyi + 1) - (yynrhs)]), sql_string, sql_result, scanner);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, sql_string, sql_result, scanner); \
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
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (sql_string);
  YY_USE (sql_result);
  YY_USE (scanner);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}






/*----------.
| yyparse.  |
`----------*/

int
yyparse (const char * sql_string, ParsedSqlResult * sql_result, void * scanner)
{
/* Lookahead token kind.  */
int yychar;


/* The semantic value of the lookahead symbol.  */
/* Default value used for initialization, for pacifying older GCCs
   or non-GCC compilers.  */
YY_INITIAL_VALUE (static YYSTYPE yyval_default;)
YYSTYPE yylval YY_INITIAL_VALUE (= yyval_default);

/* Location data for the lookahead symbol.  */
static YYLTYPE yyloc_default
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
YYLTYPE yylloc = yyloc_default;

    /* Number of syntax errors so far.  */
    int yynerrs = 0;

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
      yychar = yylex (&yylval, &yylloc, scanner);
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
  case 2: /* commands: command_wrapper opt_semicolon  */
#line 238 "yacc_sql.y"
  {
    std::unique_ptr<ParsedSqlNode> sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[-1].sql_node));
    sql_result->add_sql_node(std::move(sql_node));
  }
#line 1857 "yacc_sql.cpp"
    break;

  case 23: /* exit_stmt: EXIT  */
#line 268 "yacc_sql.y"
         {
      (void)yynerrs;  // 这么写为了消除yynerrs未使用的告警。如果你有更好的方法欢迎提PR
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXIT);
    }
#line 1866 "yacc_sql.cpp"
    break;

  case 24: /* help_stmt: HELP  */
#line 274 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_HELP);
    }
#line 1874 "yacc_sql.cpp"
    break;

  case 25: /* sync_stmt: SYNC  */
#line 279 "yacc_sql.y"
         {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SYNC);
    }
#line 1882 "yacc_sql.cpp"
    break;

  case 26: /* begin_stmt: TRX_BEGIN  */
#line 285 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_BEGIN);
    }
#line 1890 "yacc_sql.cpp"
    break;

  case 27: /* commit_stmt: TRX_COMMIT  */
#line 291 "yacc_sql.y"
               {
      (yyval.sql_node) = new ParsedSqlNode(SCF_COMMIT);
    }
#line 1898 "yacc_sql.cpp"
    break;

  case 28: /* rollback_stmt: TRX_ROLLBACK  */
#line 297 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_ROLLBACK);
    }
#line 1906 "yacc_sql.cpp"
    break;

  case 29: /* drop_table_stmt: DROP TABLE ID  */
#line 303 "yacc_sql.y"
                  {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_TABLE);
      (yyval.sql_node)->drop_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1916 "yacc_sql.cpp"
    break;

  case 30: /* show_tables_stmt: SHOW TABLES  */
#line 310 "yacc_sql.y"
                {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SHOW_TABLES);
    }
#line 1924 "yacc_sql.cpp"
    break;

  case 31: /* desc_table_stmt: DESC ID  */
#line 316 "yacc_sql.y"
             {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DESC_TABLE);
      (yyval.sql_node)->desc_table.relation_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 1934 "yacc_sql.cpp"
    break;

  case 32: /* create_index_stmt: CREATE unique_tag INDEX ID ON ID LBRACE rel_list RBRACE  */
#line 325 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_INDEX);
      CreateIndexSqlNode &create_index = (yyval.sql_node)->create_index;
      create_index.unique_tag = (yyvsp[-7].boolean);
      create_index.index_name = (yyvsp[-5].string);
      create_index.relation_name = (yyvsp[-3].string);

      if ((yyvsp[-1].relation_list) != nullptr) {
        create_index.attribute_name.swap(*(yyvsp[-1].relation_list));
        delete (yyvsp[-1].relation_list);
      }

      free((yyvsp[-5].string));
      free((yyvsp[-3].string));
    }
#line 1954 "yacc_sql.cpp"
    break;

  case 33: /* unique_tag: %empty  */
#line 344 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 1962 "yacc_sql.cpp"
    break;

  case 34: /* unique_tag: UNIQUE  */
#line 348 "yacc_sql.y"
    {
      (yyval.boolean) = true;
    }
#line 1970 "yacc_sql.cpp"
    break;

  case 35: /* drop_index_stmt: DROP INDEX ID ON ID  */
#line 355 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DROP_INDEX);
      (yyval.sql_node)->drop_index.index_name = (yyvsp[-2].string);
      (yyval.sql_node)->drop_index.relation_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 1982 "yacc_sql.cpp"
    break;

  case 36: /* create_table_stmt: CREATE TABLE ID LBRACE attr_def attr_def_list RBRACE storage_format  */
#line 365 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CREATE_TABLE);
      CreateTableSqlNode &create_table = (yyval.sql_node)->create_table;
      create_table.relation_name = (yyvsp[-5].string);
      free((yyvsp[-5].string));

      std::vector<AttrInfoSqlNode> *src_attrs = (yyvsp[-2].attr_infos);

      if (src_attrs != nullptr) {
        create_table.attr_infos.swap(*src_attrs);
        delete src_attrs;
      }
      create_table.attr_infos.emplace_back(*(yyvsp[-3].attr_info));
      std::reverse(create_table.attr_infos.begin(), create_table.attr_infos.end());
      delete (yyvsp[-3].attr_info);
      if ((yyvsp[0].string) != nullptr) {
        create_table.storage_format = (yyvsp[0].string);
        free((yyvsp[0].string));
      }
    }
#line 2007 "yacc_sql.cpp"
    break;

  case 37: /* attr_def_list: %empty  */
#line 388 "yacc_sql.y"
    {
      (yyval.attr_infos) = nullptr;
    }
#line 2015 "yacc_sql.cpp"
    break;

  case 38: /* attr_def_list: COMMA attr_def attr_def_list  */
#line 392 "yacc_sql.y"
    {
      if ((yyvsp[0].attr_infos) != nullptr) {
        (yyval.attr_infos) = (yyvsp[0].attr_infos);
      } else {
        (yyval.attr_infos) = new std::vector<AttrInfoSqlNode>;
      }
      (yyval.attr_infos)->emplace_back(*(yyvsp[-1].attr_info));
      delete (yyvsp[-1].attr_info);
    }
#line 2029 "yacc_sql.cpp"
    break;

  case 39: /* attr_def: ID type LBRACE number RBRACE null_option  */
#line 405 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-4].number);
      (yyval.attr_info)->name = (yyvsp[-5].string);
      (yyval.attr_info)->length = (yyvsp[-2].number);
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-5].string));
    }
#line 2042 "yacc_sql.cpp"
    break;

  case 40: /* attr_def: ID type null_option  */
#line 414 "yacc_sql.y"
    {
      (yyval.attr_info) = new AttrInfoSqlNode;
      (yyval.attr_info)->type = (AttrType)(yyvsp[-1].number);
      (yyval.attr_info)->name = (yyvsp[-2].string);
      (yyval.attr_info)->length = 4;
      (yyval.attr_info)->nullable = (yyvsp[0].boolean);
      free((yyvsp[-2].string));
    }
#line 2055 "yacc_sql.cpp"
    break;

  case 41: /* null_option: %empty  */
#line 426 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2063 "yacc_sql.cpp"
    break;

  case 42: /* null_option: NULLABLE  */
#line 430 "yacc_sql.y"
    {
      (yyval.boolean) = true;
    }
#line 2071 "yacc_sql.cpp"
    break;

  case 43: /* null_option: NULL_TYPE  */
#line 434 "yacc_sql.y"
    {
      (yyval.boolean) = true;
    }
#line 2079 "yacc_sql.cpp"
    break;

  case 44: /* null_option: NOT NULL_TYPE  */
#line 438 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2087 "yacc_sql.cpp"
    break;

  case 45: /* number: NUMBER  */
#line 443 "yacc_sql.y"
           {(yyval.number) = (yyvsp[0].number);}
#line 2093 "yacc_sql.cpp"
    break;

  case 46: /* type: INT_T  */
#line 446 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::INTS); }
#line 2099 "yacc_sql.cpp"
    break;

  case 47: /* type: STRING_T  */
#line 447 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::CHARS); }
#line 2105 "yacc_sql.cpp"
    break;

  case 48: /* type: DATE_T  */
#line 448 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::DATES); }
#line 2111 "yacc_sql.cpp"
    break;

  case 49: /* type: FLOAT_T  */
#line 449 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::FLOATS); }
#line 2117 "yacc_sql.cpp"
    break;

  case 50: /* type: VECTOR_T  */
#line 450 "yacc_sql.y"
               { (yyval.number) = static_cast<int>(AttrType::VECTORS); }
#line 2123 "yacc_sql.cpp"
    break;

  case 51: /* insert_stmt: INSERT INTO ID VALUES LBRACE value value_list RBRACE  */
#line 454 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_INSERT);
      (yyval.sql_node)->insertion.relation_name = (yyvsp[-5].string);
      if ((yyvsp[-1].value_list) != nullptr) {
        (yyval.sql_node)->insertion.values.swap(*(yyvsp[-1].value_list));
        delete (yyvsp[-1].value_list);
      }
      (yyval.sql_node)->insertion.values.emplace_back(*(yyvsp[-2].value));
      std::reverse((yyval.sql_node)->insertion.values.begin(), (yyval.sql_node)->insertion.values.end());
      delete (yyvsp[-2].value);
      free((yyvsp[-5].string));
    }
#line 2140 "yacc_sql.cpp"
    break;

  case 52: /* value_list: %empty  */
#line 470 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 2148 "yacc_sql.cpp"
    break;

  case 53: /* value_list: COMMA value value_list  */
#line 473 "yacc_sql.y"
                              { 
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2162 "yacc_sql.cpp"
    break;

  case 54: /* value: NUMBER  */
#line 484 "yacc_sql.y"
           {
      (yyval.value) = new Value((int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2171 "yacc_sql.cpp"
    break;

  case 55: /* value: '-' NUMBER  */
#line 488 "yacc_sql.y"
                              {
      (yyval.value) = new Value(-(int)(yyvsp[0].number));
      (yyloc) = (yylsp[0]);
    }
#line 2180 "yacc_sql.cpp"
    break;

  case 56: /* value: FLOAT  */
#line 492 "yacc_sql.y"
            {
      (yyval.value) = new Value((float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2189 "yacc_sql.cpp"
    break;

  case 57: /* value: '-' FLOAT  */
#line 496 "yacc_sql.y"
                             {
      (yyval.value) = new Value(-(float)(yyvsp[0].floats));
      (yyloc) = (yylsp[0]);
    }
#line 2198 "yacc_sql.cpp"
    break;

  case 58: /* value: SSS  */
#line 500 "yacc_sql.y"
         {
      char *tmp = common::substr((yyvsp[0].string),1,strlen((yyvsp[0].string))-2);
      (yyval.value) = new Value(tmp);
      free(tmp);
      free((yyvsp[0].string));
    }
#line 2209 "yacc_sql.cpp"
    break;

  case 59: /* value: NULL_TYPE  */
#line 506 "yacc_sql.y"
                {
      (yyval.value) = new Value();
      (yyval.value)->set_null();
    }
#line 2218 "yacc_sql.cpp"
    break;

  case 60: /* storage_format: %empty  */
#line 513 "yacc_sql.y"
    {
      (yyval.string) = nullptr;
    }
#line 2226 "yacc_sql.cpp"
    break;

  case 61: /* storage_format: STORAGE FORMAT EQ ID  */
#line 517 "yacc_sql.y"
    {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2234 "yacc_sql.cpp"
    break;

  case 62: /* delete_stmt: DELETE FROM ID where  */
#line 524 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_DELETE);
      (yyval.sql_node)->deletion.relation_name = (yyvsp[-1].string);
      if ((yyvsp[0].condition) != nullptr) {
        (yyval.sql_node)->deletion.conditions = (yyvsp[0].condition);
      }
      free((yyvsp[-1].string));
    }
#line 2247 "yacc_sql.cpp"
    break;

  case 63: /* update_stmt: UPDATE ID SET update_item_list where  */
#line 535 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_UPDATE);
      (yyval.sql_node)->update.relation_name = (yyvsp[-3].string);
      for(auto &item: *(yyvsp[-1].update_item_list_ptr)) {
        (yyval.sql_node)->update.attribute_names.push_back(item.attr_name);
        (yyval.sql_node)->update.values.push_back(item.value);
      }
      std::reverse((yyval.sql_node)->update.attribute_names.begin(), (yyval.sql_node)->update.attribute_names.end());
      std::reverse((yyval.sql_node)->update.values.begin(), (yyval.sql_node)->update.values.end());
      if ((yyvsp[0].condition) != nullptr) {
        (yyval.sql_node)->update.conditions = (yyvsp[0].condition);
      }
      free((yyvsp[-3].string));
    }
#line 2266 "yacc_sql.cpp"
    break;

  case 64: /* update_item_list: update_item  */
#line 552 "yacc_sql.y"
    {
        (yyval.update_item_list_ptr) = new std::vector<UpdateItem>;
        (yyval.update_item_list_ptr)->emplace_back(*(yyvsp[0].update_item_ptr));
        delete (yyvsp[0].update_item_ptr);
    }
#line 2276 "yacc_sql.cpp"
    break;

  case 65: /* update_item_list: update_item COMMA update_item_list  */
#line 558 "yacc_sql.y"
    {
        if ((yyvsp[0].update_item_list_ptr) != nullptr) {
            (yyval.update_item_list_ptr) = (yyvsp[0].update_item_list_ptr);
        } else {
            (yyval.update_item_list_ptr) = new std::vector<UpdateItem>;
        }
        (yyval.update_item_list_ptr)->emplace_back(*(yyvsp[-2].update_item_ptr));
        delete (yyvsp[-2].update_item_ptr);
    }
#line 2290 "yacc_sql.cpp"
    break;

  case 66: /* update_item: ID EQ expression  */
#line 571 "yacc_sql.y"
    {
      (yyval.update_item_ptr) = new UpdateItem;
      (yyval.update_item_ptr)->attr_name = (yyvsp[-2].string);
      (yyval.update_item_ptr)->value = (yyvsp[0].expression);
    }
#line 2300 "yacc_sql.cpp"
    break;

  case 67: /* select_stmt: SELECT expression_list  */
#line 579 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.sql_node)->selection.expressions.swap(*(yyvsp[0].expression_list));
        delete (yyvsp[0].expression_list);
      }
    }
#line 2312 "yacc_sql.cpp"
    break;

  case 68: /* select_stmt: SELECT expression_list FROM from_list where group_by order_by  */
#line 587 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SELECT);
      if ((yyvsp[-5].expression_list) != nullptr) {
        (yyval.sql_node)->selection.expressions.swap(*(yyvsp[-5].expression_list));
        delete (yyvsp[-5].expression_list);
      }

      if ((yyvsp[-3].join_list) != nullptr) {
        (yyval.sql_node)->selection.relations.swap(*(yyvsp[-3].join_list));
        delete (yyvsp[-3].join_list);
      }

      if ((yyvsp[-2].condition) != nullptr) {
        (yyval.sql_node)->selection.conditions = (yyvsp[-2].condition);
      }

      if ((yyvsp[-1].expression_list) != nullptr) {
        (yyval.sql_node)->selection.group_by.swap(*(yyvsp[-1].expression_list));
        delete (yyvsp[-1].expression_list);
      }

      if ((yyvsp[0].orderby_list) != nullptr) {
        (yyval.sql_node)->selection.order_by.swap(*(yyvsp[0].orderby_list));
        delete (yyvsp[0].orderby_list);
      }
    }
#line 2343 "yacc_sql.cpp"
    break;

  case 69: /* calc_stmt: CALC expression_list  */
#line 617 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_CALC);
      (yyval.sql_node)->calc.expressions.swap(*(yyvsp[0].expression_list));
      delete (yyvsp[0].expression_list);
    }
#line 2353 "yacc_sql.cpp"
    break;

  case 70: /* expression_list: expression alias  */
#line 626 "yacc_sql.y"
    {
      (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      if((yyvsp[0].string) != nullptr){
        (yyvsp[-1].expression) -> set_alias((yyvsp[0].string));
      }
      (yyval.expression_list)->emplace_back((yyvsp[-1].expression));
    }
#line 2365 "yacc_sql.cpp"
    break;

  case 71: /* expression_list: expression alias COMMA expression_list  */
#line 634 "yacc_sql.y"
    {
      if ((yyvsp[0].expression_list) != nullptr) {
        (yyval.expression_list) = (yyvsp[0].expression_list);
      } else {
        (yyval.expression_list) = new std::vector<std::unique_ptr<Expression>>;
      }
      if((yyvsp[-2].string) != nullptr){
        (yyvsp[-3].expression) -> set_alias((yyvsp[-2].string));
      }
      (yyval.expression_list)->emplace((yyval.expression_list)->begin(), (yyvsp[-3].expression));
    }
#line 2381 "yacc_sql.cpp"
    break;

  case 72: /* alias: %empty  */
#line 648 "yacc_sql.y"
                {
      (yyval.string) = nullptr;
    }
#line 2389 "yacc_sql.cpp"
    break;

  case 73: /* alias: ID  */
#line 651 "yacc_sql.y"
         {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2397 "yacc_sql.cpp"
    break;

  case 74: /* alias: AS ID  */
#line 654 "yacc_sql.y"
            {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2405 "yacc_sql.cpp"
    break;

  case 75: /* expression: expression '+' expression  */
#line 660 "yacc_sql.y"
                              {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::ADD, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2413 "yacc_sql.cpp"
    break;

  case 76: /* expression: expression '-' expression  */
#line 663 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::SUB, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2421 "yacc_sql.cpp"
    break;

  case 77: /* expression: expression '*' expression  */
#line 666 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::MUL, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2429 "yacc_sql.cpp"
    break;

  case 78: /* expression: expression '/' expression  */
#line 669 "yacc_sql.y"
                                {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::DIV, (yyvsp[-2].expression), (yyvsp[0].expression), sql_string, &(yyloc));
    }
#line 2437 "yacc_sql.cpp"
    break;

  case 79: /* expression: LBRACE expression RBRACE  */
#line 672 "yacc_sql.y"
                               {
      (yyval.expression) = (yyvsp[-1].expression);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
    }
#line 2446 "yacc_sql.cpp"
    break;

  case 80: /* expression: '-' expression  */
#line 676 "yacc_sql.y"
                                  {
      (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::NEGATIVE, (yyvsp[0].expression), nullptr, sql_string, &(yyloc));
    }
#line 2454 "yacc_sql.cpp"
    break;

  case 81: /* expression: value  */
#line 679 "yacc_sql.y"
            {
      (yyval.expression) = new ValueExpr(*(yyvsp[0].value));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].value);
    }
#line 2464 "yacc_sql.cpp"
    break;

  case 82: /* expression: rel_attr  */
#line 684 "yacc_sql.y"
               {
      RelAttrSqlNode *node = (yyvsp[0].rel_attr);
      (yyval.expression) = new UnboundFieldExpr(node->relation_name, node->attribute_name);
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[0].rel_attr);
    }
#line 2475 "yacc_sql.cpp"
    break;

  case 83: /* expression: '*'  */
#line 690 "yacc_sql.y"
          {
      (yyval.expression) = new StarExpr();
    }
#line 2483 "yacc_sql.cpp"
    break;

  case 84: /* expression: ID DOT '*'  */
#line 693 "yacc_sql.y"
                {
      (yyval.expression) = new StarExpr((yyvsp[-2].string));
    }
#line 2491 "yacc_sql.cpp"
    break;

  case 85: /* expression: func_expr  */
#line 696 "yacc_sql.y"
                {
      (yyval.expression) = (yyvsp[0].expression); // FuncExpr
    }
#line 2499 "yacc_sql.cpp"
    break;

  case 86: /* expression: aggr_func_expr  */
#line 699 "yacc_sql.y"
                     {
      (yyval.expression) = (yyvsp[0].expression); // AggrFuncExpr
    }
#line 2507 "yacc_sql.cpp"
    break;

  case 87: /* expression: sub_query_expr  */
#line 702 "yacc_sql.y"
                     {
      (yyval.expression) = (yyvsp[0].expression); // SubQueryExpr
    }
#line 2515 "yacc_sql.cpp"
    break;

  case 88: /* expression: value_list_expr  */
#line 705 "yacc_sql.y"
                      {
      (yyval.expression) = (yyvsp[0].expression); // ValueListExpr
    }
#line 2523 "yacc_sql.cpp"
    break;

  case 89: /* expression: L2_DISTANCE LBRACE expression COMMA expression RBRACE  */
#line 709 "yacc_sql.y"
                                                            {
        (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::LD, (yyvsp[-3].expression), (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2531 "yacc_sql.cpp"
    break;

  case 90: /* expression: COSINE_DISTANCE LBRACE expression COMMA expression RBRACE  */
#line 712 "yacc_sql.y"
                                                                {
        (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::CD, (yyvsp[-3].expression), (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2539 "yacc_sql.cpp"
    break;

  case 91: /* expression: INNER_PRODUCT LBRACE expression COMMA expression RBRACE  */
#line 715 "yacc_sql.y"
                                                              {
        (yyval.expression) = create_arithmetic_expression(ArithmeticExpr::Type::IP, (yyvsp[-3].expression), (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2547 "yacc_sql.cpp"
    break;

  case 92: /* aggr_func_expr: ID LBRACE expression RBRACE  */
#line 722 "yacc_sql.y"
    {
      (yyval.expression) = create_aggregate_expression((yyvsp[-3].string), (yyvsp[-1].expression), sql_string, &(yyloc));
    }
#line 2555 "yacc_sql.cpp"
    break;

  case 93: /* sub_query_expr: LBRACE select_stmt RBRACE  */
#line 729 "yacc_sql.y"
    {
      (yyval.expression) = new SubQueryExpr(std::move((yyvsp[-1].sql_node)->selection));
      delete (yyvsp[-1].sql_node);
    }
#line 2564 "yacc_sql.cpp"
    break;

  case 94: /* value_list_expr: LBRACE value value_list_ssq RBRACE  */
#line 737 "yacc_sql.y"
    {
      ValueListExpr* value_list_expr = new ValueListExpr();
      if((yyvsp[-1].value_list) != nullptr){
        value_list_expr->get_value_list().swap(*(yyvsp[-1].value_list));
        delete (yyvsp[-1].value_list);
      }
      value_list_expr->get_value_list().emplace_back(*(yyvsp[-2].value));
      std::reverse(value_list_expr->get_value_list().begin(), value_list_expr->get_value_list().end());
      delete (yyvsp[-2].value);
      (yyval.expression) = value_list_expr;
    }
#line 2580 "yacc_sql.cpp"
    break;

  case 95: /* value_list_ssq: %empty  */
#line 751 "yacc_sql.y"
    {
      (yyval.value_list) = nullptr;
    }
#line 2588 "yacc_sql.cpp"
    break;

  case 96: /* value_list_ssq: COMMA value value_list_ssq  */
#line 754 "yacc_sql.y"
                                  {
      if ((yyvsp[0].value_list) != nullptr) {
        (yyval.value_list) = (yyvsp[0].value_list);
      } else {
        (yyval.value_list) = new std::vector<Value>;
      }
      (yyval.value_list)->emplace_back(*(yyvsp[-1].value));
      delete (yyvsp[-1].value);
    }
#line 2602 "yacc_sql.cpp"
    break;

  case 97: /* func_expr: func_type LBRACE expression_list RBRACE  */
#line 767 "yacc_sql.y"
    {
      (yyval.expression) = new FuncExpr((FuncExpr::FuncType)(yyvsp[-3].number),*(yyvsp[-1].expression_list));
      (yyval.expression)->set_name(token_name(sql_string, &(yyloc)));
      delete (yyvsp[-1].expression_list);
    }
#line 2612 "yacc_sql.cpp"
    break;

  case 98: /* func_type: LENGTH  */
#line 775 "yacc_sql.y"
           {
      (yyval.number) = FuncExpr::FuncType::LENGTH;
    }
#line 2620 "yacc_sql.cpp"
    break;

  case 99: /* func_type: ROUND  */
#line 778 "yacc_sql.y"
            {
      (yyval.number) = FuncExpr::FuncType::ROUND;
    }
#line 2628 "yacc_sql.cpp"
    break;

  case 100: /* func_type: DATE_FORMAT  */
#line 781 "yacc_sql.y"
                  {
      (yyval.number) = FuncExpr::FuncType::DATE_FORMAT;
    }
#line 2636 "yacc_sql.cpp"
    break;

  case 101: /* rel_attr: ID  */
#line 787 "yacc_sql.y"
       {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[0].string));
    }
#line 2646 "yacc_sql.cpp"
    break;

  case 102: /* rel_attr: ID DOT ID  */
#line 792 "yacc_sql.y"
                {
      (yyval.rel_attr) = new RelAttrSqlNode;
      (yyval.rel_attr)->relation_name  = (yyvsp[-2].string);
      (yyval.rel_attr)->attribute_name = (yyvsp[0].string);
      free((yyvsp[-2].string));
      free((yyvsp[0].string));
    }
#line 2658 "yacc_sql.cpp"
    break;

  case 103: /* relation: ID  */
#line 802 "yacc_sql.y"
       {
      (yyval.string) = (yyvsp[0].string);
    }
#line 2666 "yacc_sql.cpp"
    break;

  case 104: /* rel_list: relation  */
#line 807 "yacc_sql.y"
             {
      (yyval.relation_list) = new std::vector<std::string>();
      (yyval.relation_list)->push_back((yyvsp[0].string));
      free((yyvsp[0].string));
    }
#line 2676 "yacc_sql.cpp"
    break;

  case 105: /* rel_list: relation COMMA rel_list  */
#line 812 "yacc_sql.y"
                              {
      if ((yyvsp[0].relation_list) != nullptr) {
        (yyval.relation_list) = (yyvsp[0].relation_list);
      } else {
        (yyval.relation_list) = new std::vector<std::string>;
      }

      (yyval.relation_list)->insert((yyval.relation_list)->begin(), (yyvsp[-2].string));
      free((yyvsp[-2].string));
    }
#line 2691 "yacc_sql.cpp"
    break;

  case 106: /* relation_with_alias: relation alias  */
#line 825 "yacc_sql.y"
                  {
        (yyval.alias) = new TableWithAlias;
        (yyval.alias)->table_name = (yyvsp[-1].string);
        if((yyvsp[0].string) != nullptr){
            (yyval.alias)->alias = (yyvsp[0].string);
        }
        else
        {
            (yyval.alias)->alias = "";
        }
    }
#line 2707 "yacc_sql.cpp"
    break;

  case 107: /* join_list: relation_with_alias  */
#line 838 "yacc_sql.y"
                        {
      (yyval.join) = new InnerJoinSqlNode;
      (yyval.join)->tables.push_back(*(yyvsp[0].alias));
    }
#line 2716 "yacc_sql.cpp"
    break;

  case 108: /* join_list: join_list INNER JOIN relation_with_alias ON condition  */
#line 843 "yacc_sql.y"
    {
      (yyval.join) = (yyvsp[-5].join);
      (yyval.join)->tables.push_back(*(yyvsp[-2].alias));
      (yyval.join)->conditions.push_back((yyvsp[0].expression));
    }
#line 2726 "yacc_sql.cpp"
    break;

  case 109: /* from_list: join_list  */
#line 850 "yacc_sql.y"
              {
      (yyval.join_list) = new std::vector<InnerJoinSqlNode>();
      (yyval.join_list)->push_back(*(yyvsp[0].join));
    }
#line 2735 "yacc_sql.cpp"
    break;

  case 110: /* from_list: join_list COMMA from_list  */
#line 854 "yacc_sql.y"
                                {
      if ((yyvsp[0].join_list) != nullptr) {
        (yyval.join_list) = (yyvsp[0].join_list);
      } else {
        (yyval.join_list) = new std::vector<InnerJoinSqlNode>;
      }

      (yyval.join_list)->insert((yyval.join_list)->begin(), *(yyvsp[-2].join));
    }
#line 2749 "yacc_sql.cpp"
    break;

  case 111: /* where: %empty  */
#line 867 "yacc_sql.y"
    {
      (yyval.condition) = nullptr;
    }
#line 2757 "yacc_sql.cpp"
    break;

  case 112: /* where: WHERE condition  */
#line 870 "yacc_sql.y"
                      {
      (yyval.condition) = (yyvsp[0].expression);  
    }
#line 2765 "yacc_sql.cpp"
    break;

  case 113: /* condition: expression comp_op expression  */
#line 875 "yacc_sql.y"
                                  {
      (yyval.expression) = new ComparisonExpr((yyvsp[-1].comp), (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 2773 "yacc_sql.cpp"
    break;

  case 114: /* condition: expression is_null_comp  */
#line 879 "yacc_sql.y"
    {
      ValueExpr *value_expr = new ValueExpr(Value(1));
      (yyval.expression) = new ComparisonExpr((yyvsp[0].boolean) ? IS_NULL : IS_NOT_NULL, (yyvsp[-1].expression), value_expr);
    }
#line 2782 "yacc_sql.cpp"
    break;

  case 115: /* condition: condition AND condition  */
#line 883 "yacc_sql.y"
                              {
      (yyval.expression) = new ConjunctionExpr(ConjunctionExpr::Type::AND, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 2790 "yacc_sql.cpp"
    break;

  case 116: /* condition: condition OR condition  */
#line 886 "yacc_sql.y"
                             {
      (yyval.expression) = new ConjunctionExpr(ConjunctionExpr::Type::OR, (yyvsp[-2].expression), (yyvsp[0].expression));
    }
#line 2798 "yacc_sql.cpp"
    break;

  case 117: /* comp_op: EQ  */
#line 892 "yacc_sql.y"
         { (yyval.comp) = EQUAL_TO; }
#line 2804 "yacc_sql.cpp"
    break;

  case 118: /* comp_op: LT  */
#line 893 "yacc_sql.y"
         { (yyval.comp) = LESS_THAN; }
#line 2810 "yacc_sql.cpp"
    break;

  case 119: /* comp_op: GT  */
#line 894 "yacc_sql.y"
         { (yyval.comp) = GREAT_THAN; }
#line 2816 "yacc_sql.cpp"
    break;

  case 120: /* comp_op: LE  */
#line 895 "yacc_sql.y"
         { (yyval.comp) = LESS_EQUAL; }
#line 2822 "yacc_sql.cpp"
    break;

  case 121: /* comp_op: GE  */
#line 896 "yacc_sql.y"
         { (yyval.comp) = GREAT_EQUAL; }
#line 2828 "yacc_sql.cpp"
    break;

  case 122: /* comp_op: NE  */
#line 897 "yacc_sql.y"
         { (yyval.comp) = NOT_EQUAL; }
#line 2834 "yacc_sql.cpp"
    break;

  case 123: /* comp_op: LIKE_OP  */
#line 898 "yacc_sql.y"
              { (yyval.comp) = LIKE; }
#line 2840 "yacc_sql.cpp"
    break;

  case 124: /* comp_op: NOT LIKE_OP  */
#line 899 "yacc_sql.y"
                  { (yyval.comp) = NOT_LIKE; }
#line 2846 "yacc_sql.cpp"
    break;

  case 125: /* comp_op: IN_OP  */
#line 900 "yacc_sql.y"
            {(yyval.comp) = IN; }
#line 2852 "yacc_sql.cpp"
    break;

  case 126: /* comp_op: NOT IN_OP  */
#line 901 "yacc_sql.y"
                {(yyval.comp) = NOT_IN;}
#line 2858 "yacc_sql.cpp"
    break;

  case 127: /* comp_op: EXISTS_OP  */
#line 902 "yacc_sql.y"
                {(yyval.comp) = EXISTS;}
#line 2864 "yacc_sql.cpp"
    break;

  case 128: /* comp_op: NOT EXISTS_OP  */
#line 903 "yacc_sql.y"
                    {(yyval.comp) = NOT_EXISTS; }
#line 2870 "yacc_sql.cpp"
    break;

  case 129: /* is_null_comp: IS NULL_TYPE  */
#line 908 "yacc_sql.y"
    {
      (yyval.boolean) = true;
    }
#line 2878 "yacc_sql.cpp"
    break;

  case 130: /* is_null_comp: IS NOT NULL_TYPE  */
#line 912 "yacc_sql.y"
    {
      (yyval.boolean) = false;
    }
#line 2886 "yacc_sql.cpp"
    break;

  case 131: /* group_by: %empty  */
#line 920 "yacc_sql.y"
    {
      (yyval.expression_list) = nullptr;
    }
#line 2894 "yacc_sql.cpp"
    break;

  case 132: /* order_by: %empty  */
#line 927 "yacc_sql.y"
    {
      (yyval.orderby_list) = nullptr;
    }
#line 2902 "yacc_sql.cpp"
    break;

  case 133: /* order_by: ORDER BY order_by_items  */
#line 931 "yacc_sql.y"
    {
      (yyval.orderby_list) = (yyvsp[0].orderby_list);
    }
#line 2910 "yacc_sql.cpp"
    break;

  case 134: /* order_by_items: order_by_item  */
#line 937 "yacc_sql.y"
                  {
      (yyval.orderby_list) = new std::vector<OrderAttrSqlNode>();
      (yyval.orderby_list)->push_back(*(yyvsp[0].orderby_attr));
    }
#line 2919 "yacc_sql.cpp"
    break;

  case 135: /* order_by_items: order_by_item COMMA order_by_items  */
#line 942 "yacc_sql.y"
    {
        if((yyvsp[0].orderby_list) != nullptr) {
            (yyval.orderby_list) = (yyvsp[0].orderby_list);
        } else {
            (yyval.orderby_list) = new std::vector<OrderAttrSqlNode>;
        }

        (yyval.orderby_list)->insert((yyval.orderby_list)->begin(), *(yyvsp[-2].orderby_attr));
    }
#line 2933 "yacc_sql.cpp"
    break;

  case 136: /* order_by_item: rel_attr  */
#line 955 "yacc_sql.y"
    {
        (yyval.orderby_attr) = new OrderAttrSqlNode;
        (yyval.orderby_attr)->attr = *(yyvsp[0].rel_attr);
        (yyval.orderby_attr)->is_asc = true;
    }
#line 2943 "yacc_sql.cpp"
    break;

  case 137: /* order_by_item: rel_attr ASC  */
#line 961 "yacc_sql.y"
    {
        (yyval.orderby_attr) = new OrderAttrSqlNode;
        (yyval.orderby_attr)->attr = *(yyvsp[-1].rel_attr);
        (yyval.orderby_attr)->is_asc = true;
    }
#line 2953 "yacc_sql.cpp"
    break;

  case 138: /* order_by_item: rel_attr DESC  */
#line 967 "yacc_sql.y"
    {
        (yyval.orderby_attr) = new OrderAttrSqlNode;
        (yyval.orderby_attr)->attr = *(yyvsp[-1].rel_attr);
        (yyval.orderby_attr)->is_asc = false;
    }
#line 2963 "yacc_sql.cpp"
    break;

  case 139: /* load_data_stmt: LOAD DATA INFILE SSS INTO TABLE ID  */
#line 975 "yacc_sql.y"
    {
      char *tmp_file_name = common::substr((yyvsp[-3].string), 1, strlen((yyvsp[-3].string)) - 2);
      
      (yyval.sql_node) = new ParsedSqlNode(SCF_LOAD_DATA);
      (yyval.sql_node)->load_data.relation_name = (yyvsp[0].string);
      (yyval.sql_node)->load_data.file_name = tmp_file_name;
      free((yyvsp[0].string));
      free(tmp_file_name);
    }
#line 2977 "yacc_sql.cpp"
    break;

  case 140: /* explain_stmt: EXPLAIN command_wrapper  */
#line 988 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_EXPLAIN);
      (yyval.sql_node)->explain.sql_node = std::unique_ptr<ParsedSqlNode>((yyvsp[0].sql_node));
    }
#line 2986 "yacc_sql.cpp"
    break;

  case 141: /* set_variable_stmt: SET ID EQ value  */
#line 996 "yacc_sql.y"
    {
      (yyval.sql_node) = new ParsedSqlNode(SCF_SET_VARIABLE);
      (yyval.sql_node)->set_variable.name  = (yyvsp[-2].string);
      (yyval.sql_node)->set_variable.value = *(yyvsp[0].value);
      free((yyvsp[-2].string));
      delete (yyvsp[0].value);
    }
#line 2998 "yacc_sql.cpp"
    break;


#line 3002 "yacc_sql.cpp"

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
        yyerror (&yylloc, sql_string, sql_result, scanner, yymsgp);
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
                      yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
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
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, sql_string, sql_result, scanner);
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
  yyerror (&yylloc, sql_string, sql_result, scanner, YY_("memory exhausted"));
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
                  yytoken, &yylval, &yylloc, sql_string, sql_result, scanner);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, sql_string, sql_result, scanner);
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

#line 1008 "yacc_sql.y"

//_____________________________________________________________________
extern void scan_string(const char *str, yyscan_t scanner);

int sql_parse(const char *s, ParsedSqlResult *sql_result) {
  yyscan_t scanner;
  yylex_init(&scanner);
  scan_string(s, scanner);
  int result = yyparse(s, sql_result, scanner);
  yylex_destroy(scanner);
  return result;
}
