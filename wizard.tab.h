
/* A Bison parser, made by GNU Bison 2.4.1.  */

/* Skeleton interface for Bison's Yacc-like parsers in C
   
      Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.
   
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
     ENTER_HOGWARTS = 258,
     EXIT_HOGWARTS = 259,
     HOUSE = 260,
     ENDHOUSE = 261,
     GRYFFINDOR = 262,
     SLYTHERIN = 263,
     HUFFLEPUFF = 264,
     RAVENCLAW = 265,
     INT = 266,
     FLOAT = 267,
     DOUBLE = 268,
     LONG = 269,
     CHAR = 270,
     BOOL = 271,
     VOID = 272,
     DECLARE = 273,
     AS = 274,
     INIT_ASSIGN = 275,
     IF = 276,
     FI = 277,
     ELSE = 278,
     CHECK = 279,
     THEN = 280,
     ENDCHECK = 281,
     LOOP = 282,
     DO = 283,
     ENDLOOP = 284,
     BREAK = 285,
     CONTINUE = 286,
     SPELL = 287,
     ENDSPELL = 288,
     SUMMON = 289,
     WITH = 290,
     RETURN = 291,
     CAST = 292,
     PROPHECY = 293,
     INPUT = 294,
     POTION = 295,
     ENDPOTION = 296,
     SORTING_HAT = 297,
     ENDHAT = 298,
     CHECK_TYPES = 299,
     CHECK_DECLARATIONS = 300,
     AND = 301,
     OR = 302,
     NOT = 303,
     XOR = 304,
     IDENTIFIER = 305,
     STRING = 306,
     CHAR_LITERAL = 307,
     BOOL_LITERAL = 308,
     NUMBER = 309,
     EQ = 310,
     NE = 311,
     GE = 312,
     LE = 313,
     UMINUS = 314
   };
#endif



#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
{

/* Line 1676 of yacc.c  */
#line 74 "parser/wizard.y"

	int ival;
	char *sval;



/* Line 1676 of yacc.c  */
#line 118 "wizard.tab.h"
} YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
#endif

extern YYSTYPE yylval;


