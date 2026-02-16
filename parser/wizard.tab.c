/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989-1990, 2000-2006, 2009-2010 Free Software
   Foundation, Inc.
   
   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

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

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.4.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1

/* Using locations.  */
#define YYLSP_NEEDED 0



/* Copy the first part of user declarations.  */

/* Line 189 of yacc.c  */
#line 1 "wizard.y"

/* Arcane Language Parser */
/* Bison grammar for Wizard-inspired programming language */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Forward declarations */
void yyerror(const char *s);
int yylex(void);

/* AST node structure */
typedef struct ASTNode {
    char *type;
    char *value;
    struct ASTNode **children;
    int num_children;
} ASTNode;

/* AST helper functions */
ASTNode* create_node(const char *type, const char *value);
ASTNode* create_node_with_children(const char *type, int num_children, ...);
void print_ast(ASTNode *node, int depth);
void free_ast(ASTNode *node);

extern int line_number;
ASTNode *root = NULL;



/* Line 189 of yacc.c  */
#line 104 "wizard.tab.c"

/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 1
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     KEYWORD_SPELL = 258,
     KEYWORD_RITUAL = 259,
     KEYWORD_CAST = 260,
     KEYWORD_IF = 261,
     KEYWORD_ELSE = 262,
     KEYWORD_ELSEIF = 263,
     KEYWORD_WHILE = 264,
     KEYWORD_FOR = 265,
     KEYWORD_REPEAT = 266,
     KEYWORD_UNTIL = 267,
     KEYWORD_BREAK = 268,
     KEYWORD_CONTINUE = 269,
     KEYWORD_RETURN = 270,
     KEYWORD_ESSENCE = 271,
     KEYWORD_SPIRIT = 272,
     KEYWORD_RUNE = 273,
     KEYWORD_SCROLL = 274,
     KEYWORD_ORACLE = 275,
     KEYWORD_VOID = 276,
     KEYWORD_ARRAY = 277,
     KEYWORD_GRIMOIRE = 278,
     KEYWORD_TRUE = 279,
     KEYWORD_FALSE = 280,
     KEYWORD_NULL = 281,
     KEYWORD_SUMMON = 282,
     KEYWORD_MANIFEST = 283,
     KEYWORD_SCRY = 284,
     KEYWORD_CONST = 285,
     KEYWORD_LET = 286,
     KEYWORD_ENCHANT = 287,
     KEYWORD_BIND = 288,
     KEYWORD_TRANSMUTE = 289,
     OP_POWER = 290,
     OP_INCREMENT = 291,
     OP_DECREMENT = 292,
     OP_EQUAL = 293,
     OP_NOT_EQUAL = 294,
     OP_GREATER_EQUAL = 295,
     OP_LESS_EQUAL = 296,
     OP_AND = 297,
     OP_OR = 298,
     OP_PLUS_ASSIGN = 299,
     OP_MINUS_ASSIGN = 300,
     OP_MULTIPLY_ASSIGN = 301,
     OP_DIVIDE_ASSIGN = 302,
     OP_MODULUS_ASSIGN = 303,
     OP_LEFT_SHIFT = 304,
     OP_RIGHT_SHIFT = 305,
     OP_PLUS = 306,
     OP_MINUS = 307,
     OP_MULTIPLY = 308,
     OP_DIVIDE = 309,
     OP_MODULUS = 310,
     OP_GREATER = 311,
     OP_LESS = 312,
     OP_NOT = 313,
     OP_ASSIGN = 314,
     OP_BITWISE_AND = 315,
     OP_BITWISE_OR = 316,
     OP_BITWISE_XOR = 317,
     OP_BITWISE_NOT = 318,
     DELIM_LPAREN = 319,
     DELIM_RPAREN = 320,
     DELIM_LBRACE = 321,
     DELIM_RBRACE = 322,
     DELIM_LBRACKET = 323,
     DELIM_RBRACKET = 324,
     DELIM_SEMICOLON = 325,
     DELIM_COMMA = 326,
     DELIM_DOT = 327,
     DELIM_COLON = 328,
     DELIM_ARROW = 329,
     IDENTIFIER = 330,
     STRING_LITERAL = 331,
     CHAR_LITERAL = 332,
     INTEGER_LITERAL = 333,
     FLOAT_LITERAL = 334,
     UNEG = 335
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 214 of yacc.c  */
#line 36 "wizard.y"

    char *string;
    int integer;
    double floating;
    struct ASTNode *node;



/* Line 214 of yacc.c  */
#line 229 "wizard.tab.c"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif


/* Copy the second part of user declarations.  */


/* Line 264 of yacc.c  */
#line 241 "wizard.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int yyi)
#else
static int
YYID (yyi)
    int yyi;
#endif
{
  return yyi;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

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
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
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
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss_alloc;
  YYSTYPE yyvs_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)				\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack_alloc, Stack, yysize);			\
	Stack = &yyptr->Stack_alloc;					\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  9
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   406

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  81
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  95
/* YYNRULES -- Number of states.  */
#define YYNSTATES  183

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   335

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
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
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     5,     6,     8,    11,    13,    22,    30,
      37,    43,    45,    49,    53,    55,    57,    59,    61,    63,
      65,    69,    72,    74,    77,    79,    81,    83,    85,    87,
      89,    91,    93,    95,    97,   100,   102,   108,   116,   122,
     130,   134,   137,   140,   143,   149,   153,   159,   166,   171,
     176,   181,   183,   185,   187,   191,   193,   197,   199,   203,
     207,   209,   213,   217,   221,   225,   227,   231,   235,   237,
     241,   245,   249,   253,   255,   258,   261,   264,   267,   269,
     272,   275,   280,   284,   289,   291,   295,   297,   299,   303,
     305,   307,   309,   311,   313,   315
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int8 yyrhs[] =
{
      82,     0,    -1,    83,    -1,    -1,    84,    -1,    83,    84,
      -1,    85,    -1,     3,    75,    64,    86,    65,    74,    88,
      89,    -1,     3,    75,    64,    65,    74,    88,    89,    -1,
       4,    75,    64,    86,    65,    89,    -1,     4,    75,    64,
      65,    89,    -1,    87,    -1,    86,    71,    87,    -1,    75,
      73,    88,    -1,    16,    -1,    17,    -1,    18,    -1,    19,
      -1,    20,    -1,    21,    -1,    66,    90,    67,    -1,    66,
      67,    -1,    91,    -1,    90,    91,    -1,    89,    -1,    92,
      -1,    93,    -1,    94,    -1,    95,    -1,    96,    -1,    97,
      -1,    98,    -1,    99,    -1,   100,    -1,   101,    70,    -1,
      70,    -1,     6,    64,   101,    65,    91,    -1,     6,    64,
     101,    65,    91,     7,    91,    -1,     9,    64,   101,    65,
      91,    -1,    10,    64,    92,    92,   101,    65,    91,    -1,
      15,   101,    70,    -1,    15,    70,    -1,    13,    70,    -1,
      14,    70,    -1,    88,    75,    59,   101,    70,    -1,    88,
      75,    70,    -1,    31,    75,    59,   101,    70,    -1,    30,
      88,    75,    59,   101,    70,    -1,    75,    59,   101,    70,
      -1,    75,    44,   101,    70,    -1,    75,    45,   101,    70,
      -1,   102,    -1,   103,    -1,   104,    -1,   103,    43,   104,
      -1,   105,    -1,   104,    42,   105,    -1,   106,    -1,   105,
      38,   106,    -1,   105,    39,   106,    -1,   107,    -1,   106,
      57,   107,    -1,   106,    56,   107,    -1,   106,    41,   107,
      -1,   106,    40,   107,    -1,   108,    -1,   107,    51,   108,
      -1,   107,    52,   108,    -1,   109,    -1,   108,    53,   109,
      -1,   108,    54,   109,    -1,   108,    55,   109,    -1,   108,
      35,   109,    -1,   110,    -1,    52,   109,    -1,    58,   109,
      -1,    36,    75,    -1,    37,    75,    -1,   112,    -1,    75,
      36,    -1,    75,    37,    -1,    75,    64,   111,    65,    -1,
      75,    64,    65,    -1,    28,    64,   101,    65,    -1,   101,
      -1,   111,    71,   101,    -1,    75,    -1,   113,    -1,    64,
     101,    65,    -1,    78,    -1,    79,    -1,    76,    -1,    77,
      -1,    24,    -1,    25,    -1,    26,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   111,   111,   118,   125,   131,   140,   144,   153,   162,
     170,   181,   187,   196,   206,   207,   208,   209,   210,   211,
     215,   221,   227,   233,   242,   243,   244,   245,   246,   247,
     248,   249,   250,   251,   255,   261,   267,   274,   285,   295,
     307,   313,   319,   325,   331,   339,   346,   353,   364,   371,
     378,   389,   393,   397,   398,   408,   409,   419,   420,   427,
     437,   438,   445,   452,   459,   469,   470,   477,   487,   488,
     495,   502,   509,   519,   520,   526,   532,   536,   543,   544,
     548,   552,   559,   566,   575,   581,   590,   594,   595,   599,
     604,   609,   613,   617,   620,   623
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "KEYWORD_SPELL", "KEYWORD_RITUAL",
  "KEYWORD_CAST", "KEYWORD_IF", "KEYWORD_ELSE", "KEYWORD_ELSEIF",
  "KEYWORD_WHILE", "KEYWORD_FOR", "KEYWORD_REPEAT", "KEYWORD_UNTIL",
  "KEYWORD_BREAK", "KEYWORD_CONTINUE", "KEYWORD_RETURN", "KEYWORD_ESSENCE",
  "KEYWORD_SPIRIT", "KEYWORD_RUNE", "KEYWORD_SCROLL", "KEYWORD_ORACLE",
  "KEYWORD_VOID", "KEYWORD_ARRAY", "KEYWORD_GRIMOIRE", "KEYWORD_TRUE",
  "KEYWORD_FALSE", "KEYWORD_NULL", "KEYWORD_SUMMON", "KEYWORD_MANIFEST",
  "KEYWORD_SCRY", "KEYWORD_CONST", "KEYWORD_LET", "KEYWORD_ENCHANT",
  "KEYWORD_BIND", "KEYWORD_TRANSMUTE", "OP_POWER", "OP_INCREMENT",
  "OP_DECREMENT", "OP_EQUAL", "OP_NOT_EQUAL", "OP_GREATER_EQUAL",
  "OP_LESS_EQUAL", "OP_AND", "OP_OR", "OP_PLUS_ASSIGN", "OP_MINUS_ASSIGN",
  "OP_MULTIPLY_ASSIGN", "OP_DIVIDE_ASSIGN", "OP_MODULUS_ASSIGN",
  "OP_LEFT_SHIFT", "OP_RIGHT_SHIFT", "OP_PLUS", "OP_MINUS", "OP_MULTIPLY",
  "OP_DIVIDE", "OP_MODULUS", "OP_GREATER", "OP_LESS", "OP_NOT",
  "OP_ASSIGN", "OP_BITWISE_AND", "OP_BITWISE_OR", "OP_BITWISE_XOR",
  "OP_BITWISE_NOT", "DELIM_LPAREN", "DELIM_RPAREN", "DELIM_LBRACE",
  "DELIM_RBRACE", "DELIM_LBRACKET", "DELIM_RBRACKET", "DELIM_SEMICOLON",
  "DELIM_COMMA", "DELIM_DOT", "DELIM_COLON", "DELIM_ARROW", "IDENTIFIER",
  "STRING_LITERAL", "CHAR_LITERAL", "INTEGER_LITERAL", "FLOAT_LITERAL",
  "UNEG", "$accept", "program", "declaration_list", "declaration",
  "function_declaration", "parameter_list", "parameter", "type_specifier",
  "compound_statement", "statement_list", "statement",
  "expression_statement", "if_statement", "while_statement",
  "for_statement", "return_statement", "break_statement",
  "continue_statement", "declaration_statement", "assignment_statement",
  "expression", "assignment_expression", "logical_or_expression",
  "logical_and_expression", "equality_expression", "relational_expression",
  "additive_expression", "multiplicative_expression", "unary_expression",
  "postfix_expression", "argument_list", "primary_expression", "literal", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,    81,    82,    82,    83,    83,    84,    85,    85,    85,
      85,    86,    86,    87,    88,    88,    88,    88,    88,    88,
      89,    89,    90,    90,    91,    91,    91,    91,    91,    91,
      91,    91,    91,    91,    92,    92,    93,    93,    94,    95,
      96,    96,    97,    98,    99,    99,    99,    99,   100,   100,
     100,   101,   102,   103,   103,   104,   104,   105,   105,   105,
     106,   106,   106,   106,   106,   107,   107,   107,   108,   108,
     108,   108,   108,   109,   109,   109,   109,   109,   110,   110,
     110,   110,   110,   110,   111,   111,   112,   112,   112,   113,
     113,   113,   113,   113,   113,   113
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     1,     0,     1,     2,     1,     8,     7,     6,
       5,     1,     3,     3,     1,     1,     1,     1,     1,     1,
       3,     2,     1,     2,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     2,     1,     5,     7,     5,     7,
       3,     2,     2,     2,     5,     3,     5,     6,     4,     4,
       4,     1,     1,     1,     3,     1,     3,     1,     3,     3,
       1,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     1,     2,     2,     2,     2,     1,     2,
       2,     4,     3,     4,     1,     3,     1,     1,     3,     1,
       1,     1,     1,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       3,     0,     0,     0,     2,     4,     6,     0,     0,     1,
       5,     0,     0,     0,     0,     0,    11,     0,     0,     0,
       0,     0,     0,     0,    10,     0,    14,    15,    16,    17,
      18,    19,     0,    13,     0,    12,     0,     0,     0,     0,
       0,     0,    93,    94,    95,     0,     0,     0,     0,     0,
       0,     0,     0,    21,    35,    86,    91,    92,    89,    90,
       0,    24,     0,    22,    25,    26,    27,    28,    29,    30,
      31,    32,    33,     0,    51,    52,    53,    55,    57,    60,
      65,    68,    73,    78,    87,     9,     8,     0,     0,     0,
       0,    42,    43,    41,    86,     0,     0,     0,     0,    76,
      77,    74,    75,     0,    79,    80,     0,     0,     0,     0,
       0,    20,    23,    34,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     7,     0,
       0,     0,    40,     0,     0,     0,    88,     0,     0,     0,
      82,    84,     0,     0,    45,    54,    56,    58,    59,    64,
      63,    62,    61,    66,    67,    72,    69,    70,    71,     0,
       0,     0,    83,     0,     0,    49,    50,    48,    81,     0,
       0,    36,    38,     0,     0,    46,    85,    44,     0,     0,
      47,    37,    39
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     3,     4,     5,     6,    15,    16,    60,    61,    62,
      63,    64,    65,    66,    67,    68,    69,    70,    71,    72,
      73,    74,    75,    76,    77,    78,    79,    80,    81,    82,
     142,    83,    84
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -83
static const yytype_int16 yypact[] =
{
      68,   -49,   -43,    42,    68,   -83,   -83,   -18,   -12,   -83,
     -83,   -40,   -38,   -30,     0,   -31,   -83,    27,    10,    71,
      71,    21,    28,   117,   -83,    27,   -83,   -83,   -83,   -83,
     -83,   -83,    27,   -83,    71,   -83,    40,    41,    43,    36,
      38,   -19,   -83,   -83,   -83,    45,    71,    35,    46,    54,
     327,   327,   327,   -83,   -83,   -23,   -83,   -83,   -83,   -83,
      64,   -83,   191,   -83,   -83,   -83,   -83,   -83,   -83,   -83,
     -83,   -83,   -83,    49,   -83,    72,    69,    44,    13,    33,
     -25,   -83,   -83,   -83,   -83,   -83,   -83,    27,   327,   327,
      88,   -83,   -83,   -83,   -21,    74,   327,    75,    90,   -83,
     -83,   -83,   -83,    86,   -83,   -83,   327,   327,   327,   296,
     -39,   -83,   -83,   -83,   327,   327,   327,   327,   327,   327,
     327,   327,   327,   327,   327,   327,   327,   327,   -83,    91,
      92,    88,   -83,    94,    96,   327,   -83,    98,   100,   101,
     -83,   -83,    15,   327,   -83,    69,    44,    13,    13,    33,
      33,    33,    33,   -25,   -25,   -83,   -83,   -83,   -83,   265,
     265,   327,   -83,   327,   102,   -83,   -83,   -83,   -83,   327,
     103,   153,   -83,    97,   104,   -83,   -83,   -83,   265,   265,
     -83,   -83,   -83
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
     -83,   -83,   -83,   157,   -83,   164,   155,     4,   -13,   -83,
     -61,   -82,   -83,   -83,   -83,   -83,   -83,   -83,   -83,   -83,
     -41,   -83,   -83,    65,    63,   -20,   -57,   -22,   -48,   -83,
     -83,   -83,   -83
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint8 yytable[] =
{
      95,   112,   101,   102,    24,    42,    43,    44,   131,    45,
     124,   103,    85,   104,   105,   104,   105,    48,    49,    86,
     143,   106,   107,    32,    33,    13,     7,    17,   125,   126,
     127,   144,     8,    50,    21,    14,   108,    14,    87,    51,
      22,   109,     9,   109,    19,    52,    11,   129,   130,   161,
      97,    93,    12,   118,   119,   133,    94,    56,    57,    58,
      59,   149,   150,   151,   152,   137,   138,   139,   141,   120,
     121,     1,     2,    20,   128,    25,   155,   156,   157,   158,
     168,    22,   116,   117,   122,   123,   169,    26,    27,    28,
      29,    30,    31,    23,   164,    34,   147,   148,   171,   172,
     153,   154,   170,    14,    88,    89,    91,    90,    92,    96,
      98,   115,    42,    43,    44,   114,    45,   181,   182,   113,
     173,    99,   174,    36,    48,    49,    37,    38,   176,   100,
      39,    40,    41,    26,    27,    28,    29,    30,    31,   110,
      50,    42,    43,    44,   132,    45,    51,    46,    47,   135,
     134,   136,    52,    48,    49,   163,   159,   160,    54,   162,
     178,    10,   179,    94,    56,    57,    58,    59,   165,    50,
     166,   167,   175,   177,   180,    51,    18,    35,   146,   145,
       0,    52,     0,    23,    53,     0,     0,    54,     0,     0,
       0,     0,    55,    56,    57,    58,    59,    36,     0,     0,
      37,    38,     0,     0,    39,    40,    41,    26,    27,    28,
      29,    30,    31,     0,     0,    42,    43,    44,     0,    45,
       0,    46,    47,     0,     0,     0,     0,    48,    49,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,    50,     0,     0,     0,     0,     0,    51,
       0,     0,     0,     0,     0,    52,     0,    23,   111,     0,
       0,    54,     0,     0,     0,     0,    55,    56,    57,    58,
      59,    36,     0,     0,    37,    38,     0,     0,    39,    40,
      41,    26,    27,    28,    29,    30,    31,     0,     0,    42,
      43,    44,     0,    45,     0,    46,    47,     0,     0,     0,
       0,    48,    49,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,    50,     0,     0,
      42,    43,    44,    51,    45,     0,     0,     0,     0,    52,
       0,    23,    48,    49,     0,    54,     0,     0,     0,     0,
      55,    56,    57,    58,    59,     0,     0,     0,    50,     0,
       0,    42,    43,    44,    51,    45,     0,     0,     0,     0,
      52,   140,     0,    48,    49,     0,     0,     0,     0,     0,
       0,    94,    56,    57,    58,    59,     0,     0,     0,    50,
       0,     0,     0,     0,     0,    51,     0,     0,     0,     0,
       0,    52,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    94,    56,    57,    58,    59
};

static const yytype_int16 yycheck[] =
{
      41,    62,    50,    51,    17,    24,    25,    26,    90,    28,
      35,    52,    25,    36,    37,    36,    37,    36,    37,    32,
      59,    44,    45,    19,    20,    65,    75,    65,    53,    54,
      55,    70,    75,    52,    65,    75,    59,    75,    34,    58,
      71,    64,     0,    64,    74,    64,    64,    88,    89,   131,
      46,    70,    64,    40,    41,    96,    75,    76,    77,    78,
      79,   118,   119,   120,   121,   106,   107,   108,   109,    56,
      57,     3,     4,    73,    87,    65,   124,   125,   126,   127,
      65,    71,    38,    39,    51,    52,    71,    16,    17,    18,
      19,    20,    21,    66,   135,    74,   116,   117,   159,   160,
     122,   123,   143,    75,    64,    64,    70,    64,    70,    64,
      75,    42,    24,    25,    26,    43,    28,   178,   179,    70,
     161,    75,   163,     6,    36,    37,     9,    10,   169,    75,
      13,    14,    15,    16,    17,    18,    19,    20,    21,    75,
      52,    24,    25,    26,    70,    28,    58,    30,    31,    59,
      75,    65,    64,    36,    37,    59,    65,    65,    70,    65,
       7,     4,    65,    75,    76,    77,    78,    79,    70,    52,
      70,    70,    70,    70,    70,    58,    12,    22,   115,   114,
      -1,    64,    -1,    66,    67,    -1,    -1,    70,    -1,    -1,
      -1,    -1,    75,    76,    77,    78,    79,     6,    -1,    -1,
       9,    10,    -1,    -1,    13,    14,    15,    16,    17,    18,
      19,    20,    21,    -1,    -1,    24,    25,    26,    -1,    28,
      -1,    30,    31,    -1,    -1,    -1,    -1,    36,    37,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    52,    -1,    -1,    -1,    -1,    -1,    58,
      -1,    -1,    -1,    -1,    -1,    64,    -1,    66,    67,    -1,
      -1,    70,    -1,    -1,    -1,    -1,    75,    76,    77,    78,
      79,     6,    -1,    -1,     9,    10,    -1,    -1,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    -1,    -1,    24,
      25,    26,    -1,    28,    -1,    30,    31,    -1,    -1,    -1,
      -1,    36,    37,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    52,    -1,    -1,
      24,    25,    26,    58,    28,    -1,    -1,    -1,    -1,    64,
      -1,    66,    36,    37,    -1,    70,    -1,    -1,    -1,    -1,
      75,    76,    77,    78,    79,    -1,    -1,    -1,    52,    -1,
      -1,    24,    25,    26,    58,    28,    -1,    -1,    -1,    -1,
      64,    65,    -1,    36,    37,    -1,    -1,    -1,    -1,    -1,
      -1,    75,    76,    77,    78,    79,    -1,    -1,    -1,    52,
      -1,    -1,    -1,    -1,    -1,    58,    -1,    -1,    -1,    -1,
      -1,    64,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    75,    76,    77,    78,    79
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,     3,     4,    82,    83,    84,    85,    75,    75,     0,
      84,    64,    64,    65,    75,    86,    87,    65,    86,    74,
      73,    65,    71,    66,    89,    65,    16,    17,    18,    19,
      20,    21,    88,    88,    74,    87,     6,     9,    10,    13,
      14,    15,    24,    25,    26,    28,    30,    31,    36,    37,
      52,    58,    64,    67,    70,    75,    76,    77,    78,    79,
      88,    89,    90,    91,    92,    93,    94,    95,    96,    97,
      98,    99,   100,   101,   102,   103,   104,   105,   106,   107,
     108,   109,   110,   112,   113,    89,    89,    88,    64,    64,
      64,    70,    70,    70,    75,   101,    64,    88,    75,    75,
      75,   109,   109,   101,    36,    37,    44,    45,    59,    64,
      75,    67,    91,    70,    43,    42,    38,    39,    40,    41,
      56,    57,    51,    52,    35,    53,    54,    55,    89,   101,
     101,    92,    70,   101,    75,    59,    65,   101,   101,   101,
      65,   101,   111,    59,    70,   104,   105,   106,   106,   107,
     107,   107,   107,   108,   108,   109,   109,   109,   109,    65,
      65,    92,    65,    59,   101,    70,    70,    70,    65,    71,
     101,    91,    91,   101,   101,    70,   101,    70,     7,    65,
      70,    91,    91
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  However,
   YYFAIL appears to be in use.  Nevertheless, it is formally deprecated
   in Bison 2.4.2's NEWS entry, where a plan to phase it out is
   discussed.  */

#define YYFAIL		goto yyerrlab
#if defined YYFAIL
  /* This is here to suppress warnings from the GCC cpp's
     -Wunused-macros.  Normally we don't worry about that warning, but
     some users do, and we want to make it easy for users to remove
     YYFAIL uses, which will produce warnings from Bison 2.5.  */
#endif

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (YYLEX_PARAM)
#else
# define YYLEX yylex ()
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *yybottom, yytype_int16 *yytop)
#else
static void
yy_stack_print (yybottom, yytop)
    yytype_int16 *yybottom;
    yytype_int16 *yytop;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
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



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
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
	    /* Fall through.  */
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

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}

/* Prevent warnings from -Wmissing-prototypes.  */
#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */


/* The lookahead symbol.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;



/*-------------------------.
| yyparse or yypush_parse.  |
`-------------------------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{


    int yystate;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus;

    /* The stacks and their tools:
       `yyss': related to states.
       `yyvs': related to semantic values.

       Refer to the stacks thru separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* The state stack.  */
    yytype_int16 yyssa[YYINITDEPTH];
    yytype_int16 *yyss;
    yytype_int16 *yyssp;

    /* The semantic value stack.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs;
    YYSTYPE *yyvsp;

    YYSIZE_T yystacksize;

  int yyn;
  int yyresult;
  /* Lookahead token as an internal (translated) token number.  */
  int yytoken;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;

#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  yytoken = 0;
  yyss = yyssa;
  yyvs = yyvsa;
  yystacksize = YYINITDEPTH;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY; /* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */
  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;

	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),
		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss_alloc, yyss);
	YYSTACK_RELOCATE (yyvs_alloc, yyvs);
#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;

      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

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
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid lookahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
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
      if (yyn == 0 || yyn == YYTABLE_NINF)
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

  /* Discard the shifted token.  */
  yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

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
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 2:

/* Line 1464 of yacc.c  */
#line 111 "wizard.y"
    {
        (yyval.node) = create_node("Program", "");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (1)].node);
        (yyval.node)->num_children = 1;
        root = (yyval.node);
    ;}
    break;

  case 3:

/* Line 1464 of yacc.c  */
#line 118 "wizard.y"
    {
        (yyval.node) = create_node("Program", "empty");
        root = (yyval.node);
    ;}
    break;

  case 4:

/* Line 1464 of yacc.c  */
#line 125 "wizard.y"
    {
        (yyval.node) = create_node("DeclarationList", "");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (1)].node);
        (yyval.node)->num_children = 1;
    ;}
    break;

  case 5:

/* Line 1464 of yacc.c  */
#line 131 "wizard.y"
    {
        (yyval.node) = (yyvsp[(1) - (2)].node);
        (yyval.node)->children = (ASTNode**)realloc((yyval.node)->children, ((yyval.node)->num_children + 1) * sizeof(ASTNode*));
        (yyval.node)->children[(yyval.node)->num_children] = (yyvsp[(2) - (2)].node);
        (yyval.node)->num_children++;
    ;}
    break;

  case 6:

/* Line 1464 of yacc.c  */
#line 140 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 7:

/* Line 1464 of yacc.c  */
#line 144 "wizard.y"
    {
        (yyval.node) = create_node("FunctionDecl", (yyvsp[(2) - (8)].string));
        (yyval.node)->children = (ASTNode**)malloc(3 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(4) - (8)].node);  /* parameters */
        (yyval.node)->children[1] = (yyvsp[(7) - (8)].node);  /* return type */
        (yyval.node)->children[2] = (yyvsp[(8) - (8)].node);  /* body */
        (yyval.node)->num_children = 3;
        free((yyvsp[(2) - (8)].string));
    ;}
    break;

  case 8:

/* Line 1464 of yacc.c  */
#line 153 "wizard.y"
    {
        (yyval.node) = create_node("FunctionDecl", (yyvsp[(2) - (7)].string));
        (yyval.node)->children = (ASTNode**)malloc(3 * sizeof(ASTNode*));
        (yyval.node)->children[0] = create_node("ParameterList", "empty");
        (yyval.node)->children[1] = (yyvsp[(6) - (7)].node);  /* return type */
        (yyval.node)->children[2] = (yyvsp[(7) - (7)].node);  /* body */
        (yyval.node)->num_children = 3;
        free((yyvsp[(2) - (7)].string));
    ;}
    break;

  case 9:

/* Line 1464 of yacc.c  */
#line 162 "wizard.y"
    {
        (yyval.node) = create_node("RitualDecl", (yyvsp[(2) - (6)].string));
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(4) - (6)].node);  /* parameters */
        (yyval.node)->children[1] = (yyvsp[(6) - (6)].node);  /* body */
        (yyval.node)->num_children = 2;
        free((yyvsp[(2) - (6)].string));
    ;}
    break;

  case 10:

/* Line 1464 of yacc.c  */
#line 170 "wizard.y"
    {
        (yyval.node) = create_node("RitualDecl", (yyvsp[(2) - (5)].string));
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = create_node("ParameterList", "empty");
        (yyval.node)->children[1] = (yyvsp[(5) - (5)].node);  /* body */
        (yyval.node)->num_children = 2;
        free((yyvsp[(2) - (5)].string));
    ;}
    break;

  case 11:

/* Line 1464 of yacc.c  */
#line 181 "wizard.y"
    {
        (yyval.node) = create_node("ParameterList", "");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (1)].node);
        (yyval.node)->num_children = 1;
    ;}
    break;

  case 12:

/* Line 1464 of yacc.c  */
#line 187 "wizard.y"
    {
        (yyval.node) = (yyvsp[(1) - (3)].node);
        (yyval.node)->children = (ASTNode**)realloc((yyval.node)->children, ((yyval.node)->num_children + 1) * sizeof(ASTNode*));
        (yyval.node)->children[(yyval.node)->num_children] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children++;
    ;}
    break;

  case 13:

/* Line 1464 of yacc.c  */
#line 196 "wizard.y"
    {
        (yyval.node) = create_node("Parameter", (yyvsp[(1) - (3)].string));
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 1;
        free((yyvsp[(1) - (3)].string));
    ;}
    break;

  case 14:

/* Line 1464 of yacc.c  */
#line 206 "wizard.y"
    { (yyval.node) = create_node("Type", "essence"); ;}
    break;

  case 15:

/* Line 1464 of yacc.c  */
#line 207 "wizard.y"
    { (yyval.node) = create_node("Type", "spirit"); ;}
    break;

  case 16:

/* Line 1464 of yacc.c  */
#line 208 "wizard.y"
    { (yyval.node) = create_node("Type", "rune"); ;}
    break;

  case 17:

/* Line 1464 of yacc.c  */
#line 209 "wizard.y"
    { (yyval.node) = create_node("Type", "scroll"); ;}
    break;

  case 18:

/* Line 1464 of yacc.c  */
#line 210 "wizard.y"
    { (yyval.node) = create_node("Type", "oracle"); ;}
    break;

  case 19:

/* Line 1464 of yacc.c  */
#line 211 "wizard.y"
    { (yyval.node) = create_node("Type", "void"); ;}
    break;

  case 20:

/* Line 1464 of yacc.c  */
#line 215 "wizard.y"
    {
        (yyval.node) = create_node("CompoundStmt", "");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(2) - (3)].node);
        (yyval.node)->num_children = 1;
    ;}
    break;

  case 21:

/* Line 1464 of yacc.c  */
#line 221 "wizard.y"
    {
        (yyval.node) = create_node("CompoundStmt", "empty");
    ;}
    break;

  case 22:

/* Line 1464 of yacc.c  */
#line 227 "wizard.y"
    {
        (yyval.node) = create_node("StatementList", "");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (1)].node);
        (yyval.node)->num_children = 1;
    ;}
    break;

  case 23:

/* Line 1464 of yacc.c  */
#line 233 "wizard.y"
    {
        (yyval.node) = (yyvsp[(1) - (2)].node);
        (yyval.node)->children = (ASTNode**)realloc((yyval.node)->children, ((yyval.node)->num_children + 1) * sizeof(ASTNode*));
        (yyval.node)->children[(yyval.node)->num_children] = (yyvsp[(2) - (2)].node);
        (yyval.node)->num_children++;
    ;}
    break;

  case 24:

/* Line 1464 of yacc.c  */
#line 242 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 25:

/* Line 1464 of yacc.c  */
#line 243 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 26:

/* Line 1464 of yacc.c  */
#line 244 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 27:

/* Line 1464 of yacc.c  */
#line 245 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 28:

/* Line 1464 of yacc.c  */
#line 246 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 29:

/* Line 1464 of yacc.c  */
#line 247 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 30:

/* Line 1464 of yacc.c  */
#line 248 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 31:

/* Line 1464 of yacc.c  */
#line 249 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 32:

/* Line 1464 of yacc.c  */
#line 250 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 33:

/* Line 1464 of yacc.c  */
#line 251 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 34:

/* Line 1464 of yacc.c  */
#line 255 "wizard.y"
    {
        (yyval.node) = create_node("ExprStmt", "");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (2)].node);
        (yyval.node)->num_children = 1;
    ;}
    break;

  case 35:

/* Line 1464 of yacc.c  */
#line 261 "wizard.y"
    {
        (yyval.node) = create_node("ExprStmt", "empty");
    ;}
    break;

  case 36:

/* Line 1464 of yacc.c  */
#line 267 "wizard.y"
    {
        (yyval.node) = create_node("IfStmt", "");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(3) - (5)].node);  /* condition */
        (yyval.node)->children[1] = (yyvsp[(5) - (5)].node);  /* then body */
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 37:

/* Line 1464 of yacc.c  */
#line 274 "wizard.y"
    {
        (yyval.node) = create_node("IfElseStmt", "");
        (yyval.node)->children = (ASTNode**)malloc(3 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(3) - (7)].node);  /* condition */
        (yyval.node)->children[1] = (yyvsp[(5) - (7)].node);  /* then body */
        (yyval.node)->children[2] = (yyvsp[(7) - (7)].node);  /* else body */
        (yyval.node)->num_children = 3;
    ;}
    break;

  case 38:

/* Line 1464 of yacc.c  */
#line 285 "wizard.y"
    {
        (yyval.node) = create_node("WhileStmt", "");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(3) - (5)].node);  /* condition */
        (yyval.node)->children[1] = (yyvsp[(5) - (5)].node);  /* body */
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 39:

/* Line 1464 of yacc.c  */
#line 295 "wizard.y"
    {
        (yyval.node) = create_node("ForStmt", "");
        (yyval.node)->children = (ASTNode**)malloc(4 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(3) - (7)].node);  /* init */
        (yyval.node)->children[1] = (yyvsp[(4) - (7)].node);  /* condition */
        (yyval.node)->children[2] = (yyvsp[(5) - (7)].node);  /* increment */
        (yyval.node)->children[3] = (yyvsp[(7) - (7)].node);  /* body */
        (yyval.node)->num_children = 4;
    ;}
    break;

  case 40:

/* Line 1464 of yacc.c  */
#line 307 "wizard.y"
    {
        (yyval.node) = create_node("ReturnStmt", "");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(2) - (3)].node);
        (yyval.node)->num_children = 1;
    ;}
    break;

  case 41:

/* Line 1464 of yacc.c  */
#line 313 "wizard.y"
    {
        (yyval.node) = create_node("ReturnStmt", "empty");
    ;}
    break;

  case 42:

/* Line 1464 of yacc.c  */
#line 319 "wizard.y"
    {
        (yyval.node) = create_node("BreakStmt", "");
    ;}
    break;

  case 43:

/* Line 1464 of yacc.c  */
#line 325 "wizard.y"
    {
        (yyval.node) = create_node("ContinueStmt", "");
    ;}
    break;

  case 44:

/* Line 1464 of yacc.c  */
#line 331 "wizard.y"
    {
        (yyval.node) = create_node("VarDeclStmt", (yyvsp[(2) - (5)].string));
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (5)].node);  /* type */
        (yyval.node)->children[1] = (yyvsp[(4) - (5)].node);  /* initializer */
        (yyval.node)->num_children = 2;
        free((yyvsp[(2) - (5)].string));
    ;}
    break;

  case 45:

/* Line 1464 of yacc.c  */
#line 339 "wizard.y"
    {
        (yyval.node) = create_node("VarDeclStmt", (yyvsp[(2) - (3)].string));
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);  /* type */
        (yyval.node)->num_children = 1;
        free((yyvsp[(2) - (3)].string));
    ;}
    break;

  case 46:

/* Line 1464 of yacc.c  */
#line 346 "wizard.y"
    {
        (yyval.node) = create_node("LetStmt", (yyvsp[(2) - (5)].string));
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(4) - (5)].node);  /* initializer */
        (yyval.node)->num_children = 1;
        free((yyvsp[(2) - (5)].string));
    ;}
    break;

  case 47:

/* Line 1464 of yacc.c  */
#line 353 "wizard.y"
    {
        (yyval.node) = create_node("ConstDeclStmt", (yyvsp[(3) - (6)].string));
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(2) - (6)].node);  /* type */
        (yyval.node)->children[1] = (yyvsp[(5) - (6)].node);  /* initializer */
        (yyval.node)->num_children = 2;
        free((yyvsp[(3) - (6)].string));
    ;}
    break;

  case 48:

/* Line 1464 of yacc.c  */
#line 364 "wizard.y"
    {
        (yyval.node) = create_node("AssignStmt", (yyvsp[(1) - (4)].string));
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(3) - (4)].node);
        (yyval.node)->num_children = 1;
        free((yyvsp[(1) - (4)].string));
    ;}
    break;

  case 49:

/* Line 1464 of yacc.c  */
#line 371 "wizard.y"
    {
        (yyval.node) = create_node("PlusAssignStmt", (yyvsp[(1) - (4)].string));
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(3) - (4)].node);
        (yyval.node)->num_children = 1;
        free((yyvsp[(1) - (4)].string));
    ;}
    break;

  case 50:

/* Line 1464 of yacc.c  */
#line 378 "wizard.y"
    {
        (yyval.node) = create_node("MinusAssignStmt", (yyvsp[(1) - (4)].string));
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(3) - (4)].node);
        (yyval.node)->num_children = 1;
        free((yyvsp[(1) - (4)].string));
    ;}
    break;

  case 51:

/* Line 1464 of yacc.c  */
#line 389 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 52:

/* Line 1464 of yacc.c  */
#line 393 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 53:

/* Line 1464 of yacc.c  */
#line 397 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 54:

/* Line 1464 of yacc.c  */
#line 398 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "||");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 55:

/* Line 1464 of yacc.c  */
#line 408 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 56:

/* Line 1464 of yacc.c  */
#line 409 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "&&");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 57:

/* Line 1464 of yacc.c  */
#line 419 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 58:

/* Line 1464 of yacc.c  */
#line 420 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "==");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 59:

/* Line 1464 of yacc.c  */
#line 427 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "!=");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 60:

/* Line 1464 of yacc.c  */
#line 437 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 61:

/* Line 1464 of yacc.c  */
#line 438 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "<");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 62:

/* Line 1464 of yacc.c  */
#line 445 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", ">");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 63:

/* Line 1464 of yacc.c  */
#line 452 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "<=");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 64:

/* Line 1464 of yacc.c  */
#line 459 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", ">=");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 65:

/* Line 1464 of yacc.c  */
#line 469 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 66:

/* Line 1464 of yacc.c  */
#line 470 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "+");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 67:

/* Line 1464 of yacc.c  */
#line 477 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "-");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 68:

/* Line 1464 of yacc.c  */
#line 487 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 69:

/* Line 1464 of yacc.c  */
#line 488 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "*");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 70:

/* Line 1464 of yacc.c  */
#line 495 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "/");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 71:

/* Line 1464 of yacc.c  */
#line 502 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "%");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 72:

/* Line 1464 of yacc.c  */
#line 509 "wizard.y"
    {
        (yyval.node) = create_node("BinaryOp", "**");
        (yyval.node)->children = (ASTNode**)malloc(2 * sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (3)].node);
        (yyval.node)->children[1] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children = 2;
    ;}
    break;

  case 73:

/* Line 1464 of yacc.c  */
#line 519 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 74:

/* Line 1464 of yacc.c  */
#line 520 "wizard.y"
    {
        (yyval.node) = create_node("UnaryOp", "-");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(2) - (2)].node);
        (yyval.node)->num_children = 1;
    ;}
    break;

  case 75:

/* Line 1464 of yacc.c  */
#line 526 "wizard.y"
    {
        (yyval.node) = create_node("UnaryOp", "!");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(2) - (2)].node);
        (yyval.node)->num_children = 1;
    ;}
    break;

  case 76:

/* Line 1464 of yacc.c  */
#line 532 "wizard.y"
    {
        (yyval.node) = create_node("PreIncrement", (yyvsp[(2) - (2)].string));
        free((yyvsp[(2) - (2)].string));
    ;}
    break;

  case 77:

/* Line 1464 of yacc.c  */
#line 536 "wizard.y"
    {
        (yyval.node) = create_node("PreDecrement", (yyvsp[(2) - (2)].string));
        free((yyvsp[(2) - (2)].string));
    ;}
    break;

  case 78:

/* Line 1464 of yacc.c  */
#line 543 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 79:

/* Line 1464 of yacc.c  */
#line 544 "wizard.y"
    {
        (yyval.node) = create_node("PostIncrement", (yyvsp[(1) - (2)].string));
        free((yyvsp[(1) - (2)].string));
    ;}
    break;

  case 80:

/* Line 1464 of yacc.c  */
#line 548 "wizard.y"
    {
        (yyval.node) = create_node("PostDecrement", (yyvsp[(1) - (2)].string));
        free((yyvsp[(1) - (2)].string));
    ;}
    break;

  case 81:

/* Line 1464 of yacc.c  */
#line 552 "wizard.y"
    {
        (yyval.node) = create_node("FunctionCall", (yyvsp[(1) - (4)].string));
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(3) - (4)].node);
        (yyval.node)->num_children = 1;
        free((yyvsp[(1) - (4)].string));
    ;}
    break;

  case 82:

/* Line 1464 of yacc.c  */
#line 559 "wizard.y"
    {
        (yyval.node) = create_node("FunctionCall", (yyvsp[(1) - (3)].string));
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = create_node("ArgumentList", "empty");
        (yyval.node)->num_children = 1;
        free((yyvsp[(1) - (3)].string));
    ;}
    break;

  case 83:

/* Line 1464 of yacc.c  */
#line 566 "wizard.y"
    {
        (yyval.node) = create_node("ManifestCall", "");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(3) - (4)].node);
        (yyval.node)->num_children = 1;
    ;}
    break;

  case 84:

/* Line 1464 of yacc.c  */
#line 575 "wizard.y"
    {
        (yyval.node) = create_node("ArgumentList", "");
        (yyval.node)->children = (ASTNode**)malloc(sizeof(ASTNode*));
        (yyval.node)->children[0] = (yyvsp[(1) - (1)].node);
        (yyval.node)->num_children = 1;
    ;}
    break;

  case 85:

/* Line 1464 of yacc.c  */
#line 581 "wizard.y"
    {
        (yyval.node) = (yyvsp[(1) - (3)].node);
        (yyval.node)->children = (ASTNode**)realloc((yyval.node)->children, ((yyval.node)->num_children + 1) * sizeof(ASTNode*));
        (yyval.node)->children[(yyval.node)->num_children] = (yyvsp[(3) - (3)].node);
        (yyval.node)->num_children++;
    ;}
    break;

  case 86:

/* Line 1464 of yacc.c  */
#line 590 "wizard.y"
    {
        (yyval.node) = create_node("Identifier", (yyvsp[(1) - (1)].string));
        free((yyvsp[(1) - (1)].string));
    ;}
    break;

  case 87:

/* Line 1464 of yacc.c  */
#line 594 "wizard.y"
    { (yyval.node) = (yyvsp[(1) - (1)].node); ;}
    break;

  case 88:

/* Line 1464 of yacc.c  */
#line 595 "wizard.y"
    { (yyval.node) = (yyvsp[(2) - (3)].node); ;}
    break;

  case 89:

/* Line 1464 of yacc.c  */
#line 599 "wizard.y"
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "%d", (yyvsp[(1) - (1)].integer));
        (yyval.node) = create_node("IntegerLiteral", buf);
    ;}
    break;

  case 90:

/* Line 1464 of yacc.c  */
#line 604 "wizard.y"
    {
        char buf[32];
        snprintf(buf, sizeof(buf), "%f", (yyvsp[(1) - (1)].floating));
        (yyval.node) = create_node("FloatLiteral", buf);
    ;}
    break;

  case 91:

/* Line 1464 of yacc.c  */
#line 609 "wizard.y"
    {
        (yyval.node) = create_node("StringLiteral", (yyvsp[(1) - (1)].string));
        free((yyvsp[(1) - (1)].string));
    ;}
    break;

  case 92:

/* Line 1464 of yacc.c  */
#line 613 "wizard.y"
    {
        (yyval.node) = create_node("CharLiteral", (yyvsp[(1) - (1)].string));
        free((yyvsp[(1) - (1)].string));
    ;}
    break;

  case 93:

/* Line 1464 of yacc.c  */
#line 617 "wizard.y"
    {
        (yyval.node) = create_node("BooleanLiteral", "true");
    ;}
    break;

  case 94:

/* Line 1464 of yacc.c  */
#line 620 "wizard.y"
    {
        (yyval.node) = create_node("BooleanLiteral", "false");
    ;}
    break;

  case 95:

/* Line 1464 of yacc.c  */
#line 623 "wizard.y"
    {
        (yyval.node) = create_node("NullLiteral", "null");
    ;}
    break;



/* Line 1464 of yacc.c  */
#line 2662 "wizard.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;

  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
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

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
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
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
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
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#if !defined(yyoverflow) || YYERROR_VERBOSE
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}



/* Line 1684 of yacc.c  */
#line 628 "wizard.y"


/* C code section */

/* Error handling */
void yyerror(const char *s) {
    fprintf(stderr, "Parse error at line %d: %s\n", line_number, s);
}

/* AST helper functions */
ASTNode* create_node(const char *type, const char *value) {
    ASTNode *node = (ASTNode*)malloc(sizeof(ASTNode));
    node->type = strdup(type);
    node->value = strdup(value);
    node->children = NULL;
    node->num_children = 0;
    return node;
}

void print_ast(ASTNode *node, int depth) {
    if (!node) return;
    
    /* Print indentation */
    for (int i = 0; i < depth; i++) {
        printf("  ");
    }
    
    /* Print node */
    printf("%s", node->type);
    if (strlen(node->value) > 0) {
        printf(": %s", node->value);
    }
    printf("\n");
    
    /* Print children */
    for (int i = 0; i < node->num_children; i++) {
        print_ast(node->children[i], depth + 1);
    }
}

void free_ast(ASTNode *node) {
    if (!node) return;
    
    free(node->type);
    free(node->value);
    
    for (int i = 0; i < node->num_children; i++) {
        free_ast(node->children[i]);
    }
    
    free(node->children);
    free(node);
}

/* Main function */
int main(int argc, char **argv) {
    printf("=== Arcane Parser ===\n");
    printf("Parsing input...\n\n");
    
    /* Parse the input */
    int result = yyparse();
    
    if (result == 0) {
        printf("=== Parsing Successful ===\n\n");
        printf("Abstract Syntax Tree:\n");
        print_ast(root, 0);
        free_ast(root);
        return 0;
    } else {
        printf("=== Parsing Failed ===\n");
        return 1;
    }
}

