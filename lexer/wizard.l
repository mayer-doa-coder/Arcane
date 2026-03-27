%{
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#if defined(__has_include)
#if __has_include("../parser/wizard.tab.h")
#include "../parser/wizard.tab.h"
#elif __has_include("../parser/y.tab.h")
#include "../parser/y.tab.h"
#else
#error "Bison parser header not found. Run bison -d parser/wizard.y first."
#endif
#else
#include "../parser/wizard.tab.h"
#endif

int comment_depth = 0;
extern int yylineno;
int current_column = 1;
int token_start_column = 1;

static char *dup_text(const char *text) {
	char *copy;
	size_t len;

	if (!text) {
		return NULL;
	}

	len = strlen(text);
	copy = (char *)malloc(len + 1);
	if (!copy) {
		return NULL;
	}

	memcpy(copy, text, len + 1);
	return copy;
}

static char *dup_unquoted(const char *text) {
	char *copy;
	size_t len;

	if (!text) {
		return NULL;
	}

	len = strlen(text);
	if (len >= 2 && ((text[0] == '"' && text[len - 1] == '"') || (text[0] == '\'' && text[len - 1] == '\''))) {
		len -= 2;
		copy = (char *)malloc(len + 1);
		if (!copy) {
			return NULL;
		}
		memcpy(copy, text + 1, len);
		copy[len] = '\0';
		return copy;
	}

	return dup_text(text);
}

static void update_column(const char *text, int length) {
	int i;
	if (!text || length <= 0) {
		return;
	}

	for (i = 0; i < length; ++i) {
		if (text[i] == '\n') {
			current_column = 1;
		} else {
			current_column++;
		}
	}
}

static void report_lexical_error(const char *text) {
	if (!text || text[0] == '\0') {
		return;
	}

	fprintf(stderr, "LEXICAL ERROR: line %d, col %d: Unknown token '%s'\n", yylineno, token_start_column, text);
}

#define YY_USER_ACTION \
	token_start_column = current_column; \
	update_column(yytext, yyleng);
%}

%option noyywrap
%option yylineno
%x MULTILINE_COMMENT

DIGIT           [0-9]
IDENTIFIER      [a-zA-Z_][a-zA-Z0-9_]*
INTEGER         {DIGIT}+
FLOAT_LITERAL   {DIGIT}+\.{DIGIT}+([eE][+-]?{DIGIT}+)?
HEX             0[xX][0-9a-fA-F]+
OCTAL           0[0-7]+

%%

"#*"                        { comment_depth = 1; BEGIN(MULTILINE_COMMENT); }
<MULTILINE_COMMENT>"#*"     { comment_depth++; }
<MULTILINE_COMMENT>"*#"     { if (--comment_depth == 0) BEGIN(INITIAL); }
<MULTILINE_COMMENT>\n       { }
<MULTILINE_COMMENT>.        { }
"##".*                      { }

"ENTER_HOGWARTS"            { return ENTER_HOGWARTS; }
"EXIT_HOGWARTS"             { return EXIT_HOGWARTS; }
"HOUSE"                     { return HOUSE; }
"ENDHOUSE"                  { return ENDHOUSE; }
"Gryffindor"                { return GRYFFINDOR; }
"Slytherin"                 { return SLYTHERIN; }
"Hufflepuff"                { return HUFFLEPUFF; }
"Ravenclaw"                 { return RAVENCLAW; }

"INT"                       { return INT; }
"FLOAT"                     { return FLOAT; }
"DOUBLE"                    { return DOUBLE; }
"LONG"                      { return LONG; }
"CHAR"                      { return CHAR; }
"BOOL"                      { return BOOL; }
"VOID"                      { return VOID; }

"DECLARE"                   { return DECLARE; }
"AS"                        { return AS; }
":="                        { return INIT_ASSIGN; }

"IF"                        { return IF; }
"FI"                        { return FI; }
"ELSE"                      { return ELSE; }
"CHECK"                     { return CHECK; }
"THEN"                      { return THEN; }
"ENDCHECK"                  { return ENDCHECK; }

"LOOP"                      { return LOOP; }
"DO"                        { return DO; }
"ENDLOOP"                   { return ENDLOOP; }
"BREAK"                     { return BREAK; }
"CONTINUE"                  { return CONTINUE; }

"SPELL"                     { return SPELL; }
"ENDSPELL"                  { return ENDSPELL; }
"SUMMON"                    { return SUMMON; }
"WITH"                      { return WITH; }
"RETURN"                    { return RETURN; }

"CAST"                      { return CAST; }
"PROPHECY"                  { return PROPHECY; }
"INPUT"                     { return INPUT; }

"POTION"                    { return POTION; }
"ENDPOTION"                 { return ENDPOTION; }
"SORTING_HAT"               { return SORTING_HAT; }
"ENDHAT"                    { return ENDHAT; }
"CHECK_TYPES"               { return CHECK_TYPES; }
"CHECK_DECLARATIONS"        { return CHECK_DECLARATIONS; }

"AND"                       { return AND; }
"OR"                        { return OR; }
"NOT"                       { return NOT; }
"XOR"                       { return XOR; }

"POWO"                      { return POWO; }
"RADIX"                     { return RADIX; }
"FLOORUS"                   { return FLOORUS; }
"CEILUS"                    { return CEILUS; }
"ABSOLUTUS"                 { return ABSOLUTUS; }
"LOGUS"                     { return LOGUS; }
"SINUS"                     { return SINUS; }
"COSINUS"                   { return COSINUS; }
"TANUS"                     { return TANUS; }

"=="                        { return EQ; }
"!="                        { return NE; }
">="                        { return GE; }
"<="                        { return LE; }

"true"                      { yylval.sval = dup_text(yytext); return BOOL_LITERAL; }
"false"                     { yylval.sval = dup_text(yytext); return BOOL_LITERAL; }
"\""([^"\\\n]|\\.)*"\""  { yylval.sval = dup_unquoted(yytext); return STRING; }
'([^'\\\n]|\\.)'          { yylval.sval = dup_unquoted(yytext); return CHAR_LITERAL; }

{HEX}                       { yylval.ival = (int)strtol(yytext, NULL, 0); return NUMBER; }
{OCTAL}                     { yylval.ival = (int)strtol(yytext, NULL, 0); return NUMBER; }
{FLOAT_LITERAL}             { yylval.ival = (int)strtol(yytext, NULL, 10); return NUMBER; }
{INTEGER}{IDENTIFIER}       { report_lexical_error(yytext); }
{INTEGER}                   { yylval.ival = (int)strtol(yytext, NULL, 10); return NUMBER; }
{IDENTIFIER}                { yylval.sval = dup_text(yytext); return IDENTIFIER; }

[ \t\r\n]+                { }

"("                         { return '('; }
")"                         { return ')'; }
"{"                         { return '{'; }
"}"                         { return '}'; }
"["                         { return '['; }
"]"                         { return ']'; }
";"                         { return ';'; }
","                         { return ','; }
":"                         { return ':'; }
"+"                         { return '+'; }
"-"                         { return '-'; }
"*"                         { return '*'; }
"/"                         { return '/'; }
"%"                         { return '%'; }
"="                         { return '='; }
">"                         { return '>'; }
"<"                         { return '<'; }

.                           { report_lexical_error(yytext); }

<MULTILINE_COMMENT><<EOF>>  {
							fprintf(stderr, "LEXICAL ERROR: line %d, col %d: Unterminated multiline comment\n", yylineno, token_start_column);
							BEGIN(INITIAL);
							return 0;
						}

%%

#ifndef USE_FLEX_LEXER
extern FILE *yyin;
extern FILE *yyout;
extern char *yytext;

static void print_token_line(int token) {
	fprintf(yyout, "<%d, \"%s\", line:%d, col:%d>\n", token, yytext, yylineno, token_start_column);
}

int main(int argc, char **argv) {
	FILE *input_file = stdin;
	FILE *output_file = stdout;
	int token;

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

	while ((token = yylex()) != 0) {
		print_token_line(token);
	}

	if (input_file != stdin) fclose(input_file);
	if (output_file != stdout) fclose(output_file);
	return 0;
}
#endif

