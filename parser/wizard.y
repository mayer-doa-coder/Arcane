%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/tokens.h"

int yylex(void);
void yyerror(const char *s);
int yyparse(void);

#ifdef USE_FLEX_LEXER
extern FILE *yyin;
#else
FILE *yyin = NULL;
#endif

int current_house = 0;

static const char *house_name_from_id(int house_id) {
	switch (house_id) {
		case 1: return "Gryffindor";
		case 2: return "Slytherin";
		case 3: return "Hufflepuff";
		case 4: return "Ravenclaw";
		default: return "Unknown";
	}
}

#ifndef USE_FLEX_LEXER
static int scan_number(int first);
static int scan_identifier_or_keyword(int first);
static int scan_string(void);
static int scan_char(void);
#endif
%}

%start program

%token ENTER_HOGWARTS EXIT_HOGWARTS HOUSE ENDHOUSE
%token GRYFFINDOR SLYTHERIN HUFFLEPUFF RAVENCLAW
%token INT FLOAT DOUBLE LONG CHAR BOOL VOID
%token DECLARE AS INIT_ASSIGN
%token IF FI ELSE CHECK THEN ENDCHECK
%token LOOP DO ENDLOOP BREAK CONTINUE
%token SPELL ENDSPELL SUMMON WITH RETURN
%token CAST PROPHECY INPUT
%token POTION ENDPOTION SORTING_HAT ENDHAT CHECK_TYPES CHECK_DECLARATIONS
%token AND OR NOT XOR
%token IDENTIFIER NUMBER STRING CHAR_LITERAL BOOL_LITERAL
%token EQ NE GE LE

%left OR XOR
%left AND
%right NOT
%left '>' '<' GE LE EQ NE
%left '+' '-'
%left '*' '/' '%'
%right UMINUS

%%

program:
	ENTER_HOGWARTS house_blocks EXIT_HOGWARTS
;

house_blocks:
	  house_blocks house_block
	|
;

house_block:
	  role_house_block
	| function_house_block
	| ravenclaw_loop_block
	| potion_block
	| sorting_hat_block
;

role_house_block:
	  GRYFFINDOR HOUSE { current_house = 1; } statements ENDHOUSE { current_house = 0; }
	| HUFFLEPUFF HOUSE { current_house = 3; } statements ENDHOUSE { current_house = 0; }
	| RAVENCLAW HOUSE { current_house = 4; } statements ENDHOUSE { current_house = 0; }
	| HOUSE GRYFFINDOR { current_house = 1; } statements ENDHOUSE { current_house = 0; }
	| HOUSE HUFFLEPUFF { current_house = 3; } statements ENDHOUSE { current_house = 0; }
	| HOUSE RAVENCLAW { current_house = 4; } statements ENDHOUSE { current_house = 0; }
	| HOUSE SLYTHERIN { current_house = 2; } statements ENDHOUSE { current_house = 0; }
;

function_house_block:
	SLYTHERIN { current_house = 2; } function { current_house = 0; }
;

ravenclaw_loop_block:
	RAVENCLAW LOOP expr DO statements ENDLOOP
;

potion_block:
	POTION HOUSE statements ENDPOTION
;

sorting_hat_block:
	SORTING_HAT HOUSE semantic_checks ENDHAT
;

function:
	  SPELL IDENTIFIER WITH '(' id_list_opt ')' HOUSE statements ENDSPELL
	| SPELL IDENTIFIER HOUSE statements ENDSPELL
	| SPELL IDENTIFIER statements ENDSPELL
;

semantic_checks:
	  semantic_checks semantic_check
	|
;

semantic_check:
	  CHECK_TYPES ';'
	| CHECK_DECLARATIONS ';'
;

statements:
	  statements statement
	|
;

statement:
	  declaration
	| assignment
	| function
	| print_stmt
	| input_stmt
	| call
	| if_stmt
	| loop_stmt
	| break_stmt
	| continue_stmt
	| return_stmt
	| else_block
;

declaration:
	DECLARE IDENTIFIER AS type_spec init_opt ';'
	{
		if (current_house != 1) {
			fprintf(stderr, "Semantic error: declaration is only allowed in Gryffindor (current: %s)\n", house_name_from_id(current_house));
		}
	}
	| DECLARE IDENTIFIER AS INT '=' NUMBER opt_semi
	{
		if (current_house != 1) {
			fprintf(stderr, "Semantic error: declaration is only allowed in Gryffindor (current: %s)\n", house_name_from_id(current_house));
		}
	}
;

type_spec:
	  INT
	| FLOAT
	| DOUBLE
	| LONG
	| CHAR
	| BOOL
	| VOID
;

init_opt:
	  INIT_ASSIGN expression
	|
;

assignment:
	  IDENTIFIER '=' expr ';'
	| IDENTIFIER INIT_ASSIGN expr ';'
;

print_stmt:
	  CAST '(' arg_list_opt ')' ';'
	| PROPHECY '(' arg_list_opt ')' ';'
	| CAST IDENTIFIER opt_semi
	| CAST STRING opt_semi
	| PROPHECY STRING ',' IDENTIFIER opt_semi
;

opt_semi:
	  ';'
	|
;

input_stmt:
	INPUT '(' IDENTIFIER ')' ';'
;

call:
	  SUMMON IDENTIFIER ';'
	| SUMMON IDENTIFIER WITH '(' arg_list_opt ')' ';'
;

if_stmt:
	IF expression HOUSE statements FI
	| CHECK expr THEN statements ENDCHECK
;

else_block:
	ELSE HOUSE statements ENDHOUSE
;

loop_stmt:
	LOOP expr DO statements ENDLOOP
;

break_stmt:
	BREAK ';'
;

continue_stmt:
	CONTINUE ';'
;

return_stmt:
	RETURN expression_opt ';'
;

expression_opt:
	  expression
	|
;

arg_list_opt:
	  arg_list
	|
;

arg_list:
	  expression
	| arg_list ',' expression
;

id_list_opt:
	  id_list
	|
;

id_list:
	  IDENTIFIER
	| id_list ',' IDENTIFIER
;

expr:
	  expr '+' expr
	| expr '-' expr
	| expr '*' expr
	| expr '/' expr
	| expr '>' expr
	| expr '<' expr
	| expr GE expr
	| expr LE expr
	| expr EQ expr
	| expr NE expr
	| IDENTIFIER
	| NUMBER
;

expression:
	  expr
	| STRING
	| CHAR_LITERAL
	| BOOL_LITERAL
	| '(' expression ')'
	| '-' expression %prec UMINUS
	| NOT expression
	| expression '%' expression
	| expression AND expression
	| expression OR expression
	| expression XOR expression
	| IDENTIFIER '(' arg_list_opt ')'
;

%%

void yyerror(const char *s) {
	fprintf(stderr, "Parse error: %s\n", s);
}

#ifndef USE_FLEX_LEXER
static int scan_identifier_or_keyword(int first) {
	char buffer[256];
	int length = 0;
	int ch = first;

	while (isalnum(ch) || ch == '_') {
		if (length < (int)sizeof(buffer) - 1) {
			buffer[length++] = (char)ch;
		}
		ch = fgetc(yyin);
	}
	buffer[length] = '\0';

	if (ch != EOF) {
		ungetc(ch, yyin);
	}

	if (strcmp(buffer, WZ_KW_ENTER_HOGWARTS) == 0) return ENTER_HOGWARTS;
	if (strcmp(buffer, WZ_KW_EXIT_HOGWARTS) == 0) return EXIT_HOGWARTS;
	if (strcmp(buffer, WZ_KW_HOUSE) == 0) return HOUSE;
	if (strcmp(buffer, WZ_KW_ENDHOUSE) == 0) return ENDHOUSE;
	if (strcmp(buffer, WZ_KW_GRYFFINDOR) == 0) return GRYFFINDOR;
	if (strcmp(buffer, WZ_KW_SLYTHERIN) == 0) return SLYTHERIN;
	if (strcmp(buffer, WZ_KW_HUFFLEPUFF) == 0) return HUFFLEPUFF;
	if (strcmp(buffer, WZ_KW_RAVENCLAW) == 0) return RAVENCLAW;
	if (strcmp(buffer, WZ_KW_INT) == 0) return INT;
	if (strcmp(buffer, WZ_KW_FLOAT) == 0) return FLOAT;
	if (strcmp(buffer, WZ_KW_DOUBLE) == 0) return DOUBLE;
	if (strcmp(buffer, WZ_KW_LONG) == 0) return LONG;
	if (strcmp(buffer, WZ_KW_CHAR) == 0) return CHAR;
	if (strcmp(buffer, WZ_KW_BOOL) == 0) return BOOL;
	if (strcmp(buffer, WZ_KW_VOID) == 0) return VOID;
	if (strcmp(buffer, WZ_KW_DECLARE) == 0) return DECLARE;
	if (strcmp(buffer, WZ_KW_AS) == 0) return AS;
	if (strcmp(buffer, WZ_KW_IF) == 0) return IF;
	if (strcmp(buffer, WZ_KW_FI) == 0) return FI;
	if (strcmp(buffer, WZ_KW_ELSE) == 0) return ELSE;
	if (strcmp(buffer, WZ_KW_CHECK) == 0) return CHECK;
	if (strcmp(buffer, WZ_KW_THEN) == 0) return THEN;
	if (strcmp(buffer, WZ_KW_ENDCHECK) == 0) return ENDCHECK;
	if (strcmp(buffer, WZ_KW_LOOP) == 0) return LOOP;
	if (strcmp(buffer, WZ_KW_DO) == 0) return DO;
	if (strcmp(buffer, WZ_KW_ENDLOOP) == 0) return ENDLOOP;
	if (strcmp(buffer, WZ_KW_BREAK) == 0) return BREAK;
	if (strcmp(buffer, WZ_KW_CONTINUE) == 0) return CONTINUE;
	if (strcmp(buffer, WZ_KW_SPELL) == 0) return SPELL;
	if (strcmp(buffer, WZ_KW_ENDSPELL) == 0) return ENDSPELL;
	if (strcmp(buffer, WZ_KW_SUMMON) == 0) return SUMMON;
	if (strcmp(buffer, WZ_KW_WITH) == 0) return WITH;
	if (strcmp(buffer, WZ_KW_RETURN) == 0) return RETURN;
	if (strcmp(buffer, WZ_KW_CAST) == 0) return CAST;
	if (strcmp(buffer, WZ_KW_PROPHECY) == 0) return PROPHECY;
	if (strcmp(buffer, WZ_KW_INPUT) == 0) return INPUT;
	if (strcmp(buffer, WZ_KW_POTION) == 0) return POTION;
	if (strcmp(buffer, WZ_KW_ENDPOTION) == 0) return ENDPOTION;
	if (strcmp(buffer, WZ_KW_SORTING_HAT) == 0) return SORTING_HAT;
	if (strcmp(buffer, WZ_KW_ENDHAT) == 0) return ENDHAT;
	if (strcmp(buffer, WZ_KW_CHECK_TYPES) == 0) return CHECK_TYPES;
	if (strcmp(buffer, WZ_KW_CHECK_DECLARATIONS) == 0) return CHECK_DECLARATIONS;
	if (strcmp(buffer, WZ_KW_AND) == 0) return AND;
	if (strcmp(buffer, WZ_KW_OR) == 0) return OR;
	if (strcmp(buffer, WZ_KW_NOT) == 0) return NOT;
	if (strcmp(buffer, WZ_KW_XOR) == 0) return XOR;
	if (strcmp(buffer, WZ_KW_TRUE) == 0 || strcmp(buffer, WZ_KW_FALSE) == 0) return BOOL_LITERAL;
	return IDENTIFIER;
}

static int scan_number(int first) {
	int ch = first;
	int seen_dot = 0;

	while (isdigit(ch) || (!seen_dot && ch == '.')) {
		if (ch == '.') {
			seen_dot = 1;
		}
		ch = fgetc(yyin);
	}

	if (ch != EOF) {
		ungetc(ch, yyin);
	}
	return NUMBER;
}

static int scan_string(void) {
	int ch;
	while ((ch = fgetc(yyin)) != EOF) {
		if (ch == '\\') {
			(void)fgetc(yyin);
			continue;
		}
		if (ch == '"') {
			return STRING;
		}
	}
	return 0;
}

static int scan_char(void) {
	int ch = fgetc(yyin);
	if (ch == '\\') {
		(void)fgetc(yyin);
	}
	ch = fgetc(yyin);
	if (ch == '\'') {
		return CHAR_LITERAL;
	}
	return 0;
}

int yylex(void) {
	int ch;
	if (yyin == NULL) {
		yyin = stdin;
	}

	while ((ch = fgetc(yyin)) != EOF) {
		if (isspace(ch)) {
			continue;
		}

		if (isalpha(ch) || ch == '_') {
			return scan_identifier_or_keyword(ch);
		}

		if (isdigit(ch)) {
			return scan_number(ch);
		}

		switch (ch) {
			case '#': {
				int next = fgetc(yyin);
				if (next == '#') {
					while ((ch = fgetc(yyin)) != EOF && ch != '\n') { }
					continue;
				}
				if (next == '*') {
					int prev = 0;
					while ((ch = fgetc(yyin)) != EOF) {
						if (prev == '*' && ch == '#') {
							break;
						}
						prev = ch;
					}
					continue;
				}
				if (next != EOF) ungetc(next, yyin);
				return '#';
			}
			case '(': return '(';
			case ')': return ')';
			case ',': return ',';
			case ';': return ';';
			case '+': return '+';
			case '-': return '-';
			case '*': return '*';
			case '/': return '/';
			case '%': return '%';
			case ':': {
				int next = fgetc(yyin);
				if (next == '=') return INIT_ASSIGN;
				if (next != EOF) ungetc(next, yyin);
				return ':';
			}
			case '=': {
				int next = fgetc(yyin);
				if (next == '=') return EQ;
				if (next != EOF) ungetc(next, yyin);
				return '=';
			}
			case '!': {
				int next = fgetc(yyin);
				if (next == '=') return NE;
				if (next != EOF) ungetc(next, yyin);
				return '!';
			}
			case '>': {
				int next = fgetc(yyin);
				if (next == '=') return GE;
				if (next != EOF) ungetc(next, yyin);
				return '>';
			}
			case '<': {
				int next = fgetc(yyin);
				if (next == '=') return LE;
				if (next != EOF) ungetc(next, yyin);
				return '<';
			}
			case '"': return scan_string();
			case '\'': return scan_char();
			default:
				return ch;
		}
	}

	return 0;
}

#endif

int main(int argc, char **argv) {
	if (argc > 1) {
		yyin = fopen(argv[1], "r");
		if (!yyin) {
			fprintf(stderr, "Cannot open input file: %s\n", argv[1]);
			return 1;
		}
	}

	if (yyparse() == 0) {
		printf("Parse successful.\n");
	}

	if (yyin && yyin != stdin) {
		fclose(yyin);
	}
	return 0;
}
