%{
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
%}

%option noyywrap
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

"POWER"                     { return IDENTIFIER; }
"ROOT"                      { return IDENTIFIER; }
"FLOOR"                     { return IDENTIFIER; }
"CEIL"                      { return IDENTIFIER; }
"ABS"                       { return IDENTIFIER; }
"LOG"                       { return IDENTIFIER; }
"SIN"                       { return IDENTIFIER; }
"COS"                       { return IDENTIFIER; }
"TAN"                       { return IDENTIFIER; }

"=="                        { return EQ; }
"!="                        { return NE; }
">="                        { return GE; }
"<="                        { return LE; }

"true"                      { return BOOL_LITERAL; }
"false"                     { return BOOL_LITERAL; }
"\""([^"\\\n]|\\.)*"\""  { return STRING; }
'([^'\\\n]|\\.)'          { return CHAR_LITERAL; }

{HEX}                       { return NUMBER; }
{OCTAL}                     { return NUMBER; }
{FLOAT_LITERAL}             { return NUMBER; }
{INTEGER}                   { return NUMBER; }
{IDENTIFIER}                { return IDENTIFIER; }

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

.                           { return yytext[0]; }

%%

#ifndef USE_FLEX_LEXER
extern FILE *yyin;
extern FILE *yyout;
extern char *yytext;

static void print_token_line(int token) {
	fprintf(yyout, "<%d, \"%s\">\n", token, yytext);
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

