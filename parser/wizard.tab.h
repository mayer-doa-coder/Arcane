/* A Bison parser, made by GNU Bison 2.4.2.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
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

/* Line 1685 of yacc.c  */
#line 36 "wizard.y"

    char *string;
    int integer;
    double floating;
    struct ASTNode *node;



/* Line 1685 of yacc.c  */
#line 140 "wizard.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


