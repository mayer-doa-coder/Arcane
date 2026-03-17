%{
/* Arcane Language Lexical Analyzer for Parser Integration */
/* Wizard-inspired programming language */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wizard.tab.h"  /* Bison-generated header with token definitions */

/* Global variables */
int line_number = 1;
int comment_depth = 0;

/* yylval is defined in Bison-generated code */
extern YYSTYPE yylval;

%}

/* Flex options */
%option noyywrap

/* State for multi-line comments */
%x MULTILINE_COMMENT

/* Regular expression definitions */
DIGIT           [0-9]
LETTER          [a-zA-Z]
IDENTIFIER      [a-zA-Z_][a-zA-Z0-9_]*
INTEGER         {DIGIT}+
FLOAT           {DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)?
HEX             0[xX][0-9a-fA-F]+
OCTAL           0[0-7]+

%%

    /* Multi-line comment handling */
"#*"                { comment_depth = 1; BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"#*"    { comment_depth++; }
<MULTILINE_COMMENT>"*#"    { 
                        comment_depth--; 
                        if (comment_depth == 0) {
                            BEGIN(INITIAL);
                        }
                    }
<MULTILINE_COMMENT>\n      { line_number++; }
<MULTILINE_COMMENT>.       { /* ignore comment content */ }

    /* Single-line comments */
"##".*              { /* ignore single-line comments */ }

    /* Control flow keywords */
"spell"             { return KEYWORD_SPELL; }
"ritual"            { return KEYWORD_RITUAL; }
"cast"              { return KEYWORD_CAST; }
"if"                { return KEYWORD_IF; }
"else"              { return KEYWORD_ELSE; }
"elseif"            { return KEYWORD_ELSEIF; }
"while"             { return KEYWORD_WHILE; }
"for"               { return KEYWORD_FOR; }
"repeat"            { return KEYWORD_REPEAT; }
"until"             { return KEYWORD_UNTIL; }
"break"             { return KEYWORD_BREAK; }
"continue"          { return KEYWORD_CONTINUE; }
"return"            { return KEYWORD_RETURN; }

    /* Data type keywords */
"essence"           { return KEYWORD_ESSENCE; }
"spirit"            { return KEYWORD_SPIRIT; }
"rune"              { return KEYWORD_RUNE; }
"scroll"            { return KEYWORD_SCROLL; }
"oracle"            { return KEYWORD_ORACLE; }
"void"              { return KEYWORD_VOID; }
"array"             { return KEYWORD_ARRAY; }
"grimoire"          { return KEYWORD_GRIMOIRE; }

    /* Boolean and null literals */
"true"              { return KEYWORD_TRUE; }
"false"             { return KEYWORD_FALSE; }
"null"              { return KEYWORD_NULL; }

    /* I/O keywords */
"summon"            { return KEYWORD_SUMMON; }
"manifest"          { return KEYWORD_MANIFEST; }
"scry"              { return KEYWORD_SCRY; }

    /* Other keywords */
"const"             { return KEYWORD_CONST; }
"let"               { return KEYWORD_LET; }
"enchant"           { return KEYWORD_ENCHANT; }
"bind"              { return KEYWORD_BIND; }
"transmute"         { return KEYWORD_TRANSMUTE; }

    /* Multi-character operators - must come before single-character */
"**"                { return OP_POWER; }
"++"                { return OP_INCREMENT; }
"--"                { return OP_DECREMENT; }
"=="                { return OP_EQUAL; }
"!="                { return OP_NOT_EQUAL; }
">="                { return OP_GREATER_EQUAL; }
"<="                { return OP_LESS_EQUAL; }
"&&"                { return OP_AND; }
"||"                { return OP_OR; }
"+="                { return OP_PLUS_ASSIGN; }
"-="                { return OP_MINUS_ASSIGN; }
"*="                { return OP_MULTIPLY_ASSIGN; }
"/="                { return OP_DIVIDE_ASSIGN; }
"%="                { return OP_MODULUS_ASSIGN; }
"<<"                { return OP_LEFT_SHIFT; }
">>"                { return OP_RIGHT_SHIFT; }
"->"                { return DELIM_ARROW; }

    /* Single-character operators */
"+"                 { return OP_PLUS; }
"-"                 { return OP_MINUS; }
"*"                 { return OP_MULTIPLY; }
"/"                 { return OP_DIVIDE; }
"%"                 { return OP_MODULUS; }
">"                 { return OP_GREATER; }
"<"                 { return OP_LESS; }
"!"                 { return OP_NOT; }
"="                 { return OP_ASSIGN; }
"&"                 { return OP_BITWISE_AND; }
"|"                 { return OP_BITWISE_OR; }
"^"                 { return OP_BITWISE_XOR; }
"~"                 { return OP_BITWISE_NOT; }

    /* Delimiters */
"("                 { return DELIM_LPAREN; }
")"                 { return DELIM_RPAREN; }
"{"                 { return DELIM_LBRACE; }
"}"                 { return DELIM_RBRACE; }
"["                 { return DELIM_LBRACKET; }
"]"                 { return DELIM_RBRACKET; }
";"                 { return DELIM_SEMICOLON; }
","                 { return DELIM_COMMA; }
"."                 { return DELIM_DOT; }
":"                 { return DELIM_COLON; }

    /* Literals */
{HEX}               { yylval.integer = (int)strtol(yytext, NULL, 16); return INTEGER_LITERAL; }
{OCTAL}             { yylval.integer = (int)strtol(yytext, NULL, 8); return INTEGER_LITERAL; }
{FLOAT}             { yylval.floating = atof(yytext); return FLOAT_LITERAL; }
{INTEGER}           { yylval.integer = atoi(yytext); return INTEGER_LITERAL; }

    /* String literal with escape sequences */
\"([^\"\\\n]|\\.)*\"  { yylval.string = strdup(yytext); return STRING_LITERAL; }

    /* Character literal with escape sequences */
'([^'\\\n]|\\.)'    { yylval.string = strdup(yytext); return CHAR_LITERAL; }

    /* Identifiers - must come after keywords */
{IDENTIFIER}        { yylval.string = strdup(yytext); return IDENTIFIER; }

    /* Whitespace */
[ \t\r]+            { /* ignore whitespace */ }
\n                  { line_number++; }

    /* Unrecognized characters */
.                   { 
                        fprintf(stderr, "Error: Unrecognized character '%s' at line %d\n", 
                                yytext, line_number);
                    }

%%

/* No main function needed - Bison provides it */
