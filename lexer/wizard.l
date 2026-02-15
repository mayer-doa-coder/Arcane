%{
/* Arcane Language Lexical Analyzer */
/* Wizard-inspired programming language */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* Global variables */
int line_number = 1;
int comment_depth = 0;

/* Token output helper */
void print_token(const char* token_type, const char* lexeme) {
    fprintf(yyout, "<%s, \"%s\", line:%d>\n", token_type, lexeme, line_number);
}

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
"spell"             { print_token("KEYWORD_SPELL", yytext); }
"ritual"            { print_token("KEYWORD_RITUAL", yytext); }
"cast"              { print_token("KEYWORD_CAST", yytext); }
"if"                { print_token("KEYWORD_IF", yytext); }
"else"              { print_token("KEYWORD_ELSE", yytext); }
"elseif"            { print_token("KEYWORD_ELSEIF", yytext); }
"while"             { print_token("KEYWORD_WHILE", yytext); }
"for"               { print_token("KEYWORD_FOR", yytext); }
"repeat"            { print_token("KEYWORD_REPEAT", yytext); }
"until"             { print_token("KEYWORD_UNTIL", yytext); }
"break"             { print_token("KEYWORD_BREAK", yytext); }
"continue"          { print_token("KEYWORD_CONTINUE", yytext); }
"return"            { print_token("KEYWORD_RETURN", yytext); }

    /* Data type keywords */
"essence"           { print_token("KEYWORD_ESSENCE", yytext); }
"spirit"            { print_token("KEYWORD_SPIRIT", yytext); }
"rune"              { print_token("KEYWORD_RUNE", yytext); }
"scroll"            { print_token("KEYWORD_SCROLL", yytext); }
"oracle"            { print_token("KEYWORD_ORACLE", yytext); }
"void"              { print_token("KEYWORD_VOID", yytext); }
"array"             { print_token("KEYWORD_ARRAY", yytext); }
"grimoire"          { print_token("KEYWORD_GRIMOIRE", yytext); }

    /* Boolean and null literals */
"true"              { print_token("KEYWORD_TRUE", yytext); }
"false"             { print_token("KEYWORD_FALSE", yytext); }
"null"              { print_token("KEYWORD_NULL", yytext); }

    /* I/O keywords */
"summon"            { print_token("KEYWORD_SUMMON", yytext); }
"manifest"          { print_token("KEYWORD_MANIFEST", yytext); }
"scry"              { print_token("KEYWORD_SCRY", yytext); }

    /* Other keywords */
"const"             { print_token("KEYWORD_CONST", yytext); }
"let"               { print_token("KEYWORD_LET", yytext); }
"enchant"           { print_token("KEYWORD_ENCHANT", yytext); }
"bind"              { print_token("KEYWORD_BIND", yytext); }
"transmute"         { print_token("KEYWORD_TRANSMUTE", yytext); }

    /* Multi-character operators - must come before single-character */
"**"                { print_token("OP_POWER", yytext); }
"++"                { print_token("OP_INCREMENT", yytext); }
"--"                { print_token("OP_DECREMENT", yytext); }
"=="                { print_token("OP_EQUAL", yytext); }
"!="                { print_token("OP_NOT_EQUAL", yytext); }
">="                { print_token("OP_GREATER_EQUAL", yytext); }
"<="                { print_token("OP_LESS_EQUAL", yytext); }
"&&"                { print_token("OP_AND", yytext); }
"||"                { print_token("OP_OR", yytext); }
"+="                { print_token("OP_PLUS_ASSIGN", yytext); }
"-="                { print_token("OP_MINUS_ASSIGN", yytext); }
"*="                { print_token("OP_MULTIPLY_ASSIGN", yytext); }
"/="                { print_token("OP_DIVIDE_ASSIGN", yytext); }
"%="                { print_token("OP_MODULUS_ASSIGN", yytext); }
"<<"                { print_token("OP_LEFT_SHIFT", yytext); }
">>"                { print_token("OP_RIGHT_SHIFT", yytext); }
"->"                { print_token("DELIM_ARROW", yytext); }

    /* Single-character operators */
"+"                 { print_token("OP_PLUS", yytext); }
"-"                 { print_token("OP_MINUS", yytext); }
"*"                 { print_token("OP_MULTIPLY", yytext); }
"/"                 { print_token("OP_DIVIDE", yytext); }
"%"                 { print_token("OP_MODULUS", yytext); }
">"                 { print_token("OP_GREATER", yytext); }
"<"                 { print_token("OP_LESS", yytext); }
"!"                 { print_token("OP_NOT", yytext); }
"="                 { print_token("OP_ASSIGN", yytext); }
"&"                 { print_token("OP_BITWISE_AND", yytext); }
"|"                 { print_token("OP_BITWISE_OR", yytext); }
"^"                 { print_token("OP_BITWISE_XOR", yytext); }
"~"                 { print_token("OP_BITWISE_NOT", yytext); }

    /* Delimiters */
"("                 { print_token("DELIM_LPAREN", yytext); }
")"                 { print_token("DELIM_RPAREN", yytext); }
"{"                 { print_token("DELIM_LBRACE", yytext); }
"}"                 { print_token("DELIM_RBRACE", yytext); }
"["                 { print_token("DELIM_LBRACKET", yytext); }
"]"                 { print_token("DELIM_RBRACKET", yytext); }
";"                 { print_token("DELIM_SEMICOLON", yytext); }
","                 { print_token("DELIM_COMMA", yytext); }
"."                 { print_token("DELIM_DOT", yytext); }
":"                 { print_token("DELIM_COLON", yytext); }

    /* Literals */
{HEX}               { print_token("INTEGER_LITERAL", yytext); }
{OCTAL}             { print_token("INTEGER_LITERAL", yytext); }
{FLOAT}             { print_token("FLOAT_LITERAL", yytext); }
{INTEGER}           { print_token("INTEGER_LITERAL", yytext); }

    /* String literal with escape sequences */
\"([^\"\\\n]|\\.)*\"  { print_token("STRING_LITERAL", yytext); }

    /* Character literal with escape sequences */
'([^'\\\n]|\\.)'    { print_token("CHAR_LITERAL", yytext); }

    /* Identifiers - must come after keywords */
{IDENTIFIER}        { print_token("IDENTIFIER", yytext); }

    /* Whitespace */
[ \t\r]+            { /* ignore whitespace */ }
\n                  { line_number++; }

    /* Unrecognized characters */
.                   { 
                        fprintf(stderr, "Error: Unrecognized character '%s' at line %d\n", 
                                yytext, line_number);
                        print_token("UNKNOWN", yytext);
                    }

%%

/* User code section */

int main(int argc, char** argv) {
    /* Default to stdin/stdout */
    FILE* input_file = stdin;
    FILE* output_file = stdout;
    
    /* Handle command-line arguments */
    if (argc > 1) {
        input_file = fopen(argv[1], "r");
        if (!input_file) {
            fprintf(stderr, "Error: Cannot open input file '%s'\n", argv[1]);
            return 1;
        }
        yyin = input_file;
    }
    
    if (argc > 2) {
        output_file = fopen(argv[2], "w");
        if (!output_file) {
            fprintf(stderr, "Error: Cannot open output file '%s'\n", argv[2]);
            if (input_file != stdin) fclose(input_file);
            return 1;
        }
        yyout = output_file;
    }
    
    /* Print header */
    fprintf(yyout, "=== Arcane Lexical Analysis ===\n");
    fprintf(yyout, "Tokenizing input...\n\n");
    
    /* Tokenize the input */
    while (yylex()) {
        /* Token output is handled in the rules section */
    }
    
    /* Print footer */
    fprintf(yyout, "\n=== Tokenization Complete ===\n");
    fprintf(yyout, "Total lines processed: %d\n", line_number);
    
    /* Cleanup */
    if (input_file != stdin) fclose(input_file);
    if (output_file != stdout) fclose(output_file);
    
    return 0;
}

