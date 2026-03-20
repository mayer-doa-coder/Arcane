%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../lexer/tokens.h"
#include "../symbol_table/symbol_table.h"
#include "../icg/icg.h"
#include "../optimizer/optimizer.h"
#include "../codegen/codegen.h"

int yylex(void);
void yyerror(const char *s);
int yyparse(void);

#ifdef USE_FLEX_LEXER
extern FILE *yyin;
#else
FILE *yyin = NULL;
#endif

int current_house = 0;
int current_scope_level = 0;
char current_function_name[ARCANE_MAX_NAME_LEN] = "";
ArcaneICG g_icg;
char *loop_start_stack[64];
char *loop_end_stack[64];
int loop_stack_top = -1;

static char *arcane_strdup(const char *text) {
	size_t len;
	char *copy;

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

static void sync_symbol_context(void) {
	arcane_symbol_set_context(current_scope_level, current_function_name, (ArcaneHouse)current_house);
}

static void report_semantic_error(const char *message, const char *name) {
	if (name && name[0] != '\0') {
		fprintf(stderr, "Semantic error: %s %s\n", message, name);
		return;
	}

	fprintf(stderr, "Semantic error: %s\n", message);
}

static char *emit_binary_temp(const char *op, char *left, char *right) {
	char *temp;

	if (!left || !right || !op) {
		free(left);
		free(right);
		return icg_dup_text("0");
	}

	temp = icg_new_temp(&g_icg);
	if (!temp) {
		temp = icg_dup_text(left);
		free(left);
		free(right);
		return temp;
	}

	icg_emit_binary(&g_icg, temp, left, op, right);
	free(left);
	free(right);
	return temp;
}

static char *wrap_literal(const char *text, char quote) {
	size_t length;
	char *result;

	if (!text) {
		return NULL;
	}

	length = strlen(text);
	result = (char *)malloc(length + 3);
	if (!result) {
		return NULL;
	}

	result[0] = quote;
	memcpy(result + 1, text, length);
	result[length + 1] = quote;
	result[length + 2] = '\0';
	return result;
}

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

%union {
	int ival;
	char *sval;
}

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
%token <sval> IDENTIFIER STRING CHAR_LITERAL BOOL_LITERAL
%token <ival> NUMBER
%token EQ NE GE LE

%type <ival> type_spec
%type <sval> init_opt expr expression

%left OR XOR
%left AND
%right NOT
%left '>' '<' GE LE EQ NE
%left '+' '-'
%left '*' '/' '%'
%right UMINUS

%%

program:
	ENTER_HOGWARTS
	{
		arcane_symbols_reset();
		icg_init(&g_icg, stdout);
		icg_reset(&g_icg);
		loop_stack_top = -1;
		current_scope_level = 0;
		current_function_name[0] = '\0';
		sync_symbol_context();
		icg_emit(&g_icg, "# ICG BEGIN\n");
	}
	house_blocks EXIT_HOGWARTS
	{
		icg_emit(&g_icg, "# ICG END\n");
		optimizer_run_all(&g_icg);
		print_icg(&g_icg, stdout);
		if (generate_c_code(&g_arcane_symbol_table, &g_icg, "../output/output.c") != 0) {
			fprintf(stderr, "Code generation warning: failed to write output C file\n");
		}
		icg_release(&g_icg);
	}
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
	  GRYFFINDOR HOUSE { current_house = 1; sync_symbol_context(); } statements ENDHOUSE { current_house = 0; sync_symbol_context(); }
	| HUFFLEPUFF HOUSE { current_house = 3; sync_symbol_context(); } statements ENDHOUSE { current_house = 0; sync_symbol_context(); }
	| RAVENCLAW HOUSE { current_house = 4; sync_symbol_context(); } statements ENDHOUSE { current_house = 0; sync_symbol_context(); }
	| HOUSE GRYFFINDOR { current_house = 1; sync_symbol_context(); } statements ENDHOUSE { current_house = 0; sync_symbol_context(); }
	| HOUSE HUFFLEPUFF { current_house = 3; sync_symbol_context(); } statements ENDHOUSE { current_house = 0; sync_symbol_context(); }
	| HOUSE RAVENCLAW { current_house = 4; sync_symbol_context(); } statements ENDHOUSE { current_house = 0; sync_symbol_context(); }
	| HOUSE SLYTHERIN { current_house = 2; sync_symbol_context(); } statements ENDHOUSE { current_house = 0; sync_symbol_context(); }
;

function_house_block:
	SLYTHERIN { current_house = 2; sync_symbol_context(); } function { current_house = 0; sync_symbol_context(); }
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
	  SPELL IDENTIFIER
	  {
		int function_insert_result;
		function_insert_result = arcane_insert_symbol($2, ARCANE_SYMBOL_FUNCTION, ARCANE_TYPE_VOID, (ArcaneHouse)current_house, 0, "", 0);
		if (function_insert_result == ARCANE_SYMBOL_DUPLICATE) {
			report_semantic_error("duplicate function", $2);
		}
		arcane_copy_text(current_function_name, sizeof(current_function_name), $2);
		current_scope_level = 1;
		sync_symbol_context();
		free($2);
	  }
	  WITH '(' id_list_opt ')' HOUSE statements ENDSPELL
	  {
		current_scope_level = 0;
		current_function_name[0] = '\0';
		sync_symbol_context();
	  }
	| SPELL IDENTIFIER
	  {
		int function_insert_result;
		function_insert_result = arcane_insert_symbol($2, ARCANE_SYMBOL_FUNCTION, ARCANE_TYPE_VOID, (ArcaneHouse)current_house, 0, "", 0);
		if (function_insert_result == ARCANE_SYMBOL_DUPLICATE) {
			report_semantic_error("duplicate function", $2);
		}
		arcane_copy_text(current_function_name, sizeof(current_function_name), $2);
		current_scope_level = 1;
		sync_symbol_context();
		free($2);
	  }
	  HOUSE statements ENDSPELL
	  {
		current_scope_level = 0;
		current_function_name[0] = '\0';
		sync_symbol_context();
	  }
	| SPELL IDENTIFIER
	  {
		int function_insert_result;
		function_insert_result = arcane_insert_symbol($2, ARCANE_SYMBOL_FUNCTION, ARCANE_TYPE_VOID, (ArcaneHouse)current_house, 0, "", 0);
		if (function_insert_result == ARCANE_SYMBOL_DUPLICATE) {
			report_semantic_error("duplicate function", $2);
		}
		arcane_copy_text(current_function_name, sizeof(current_function_name), $2);
		current_scope_level = 1;
		sync_symbol_context();
		free($2);
	  }
	  statements ENDSPELL
	  {
		current_scope_level = 0;
		current_function_name[0] = '\0';
		sync_symbol_context();
	  }
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
		int declaration_result;
		sync_symbol_context();
		if (current_house != 1) {
			fprintf(stderr, "Semantic error: declaration is only allowed in Gryffindor (current: %s)\n", house_name_from_id(current_house));
		}

		declaration_result = arcane_insert_variable($2, (ArcaneType)$4, (ArcaneHouse)current_house, current_scope_level, current_function_name, 0);
		if (declaration_result == ARCANE_SYMBOL_DUPLICATE) {
			report_semantic_error("duplicate variable", $2);
		}
		if ($5 && declaration_result == ARCANE_SYMBOL_OK) {
			int symbol_index = lookup_symbol($2);
			if (symbol_index >= 0) {
				arcane_symbol_table_mark_initialized(&g_arcane_symbol_table, symbol_index);
			}
			icg_emit_assign(&g_icg, $2, $5);
		}
		free($5);
		free($2);
	}
	| DECLARE IDENTIFIER AS INT '=' NUMBER opt_semi
	{
		char *literal_value;
		sync_symbol_context();
		if (current_house != 1) {
			fprintf(stderr, "Semantic error: declaration is only allowed in Gryffindor (current: %s)\n", house_name_from_id(current_house));
		}

		if (insert_symbol($2, ARCANE_TYPE_INT, $6) == ARCANE_SYMBOL_DUPLICATE) {
			report_semantic_error("duplicate variable", $2);
		} else {
			literal_value = icg_int_literal($6);
			if (literal_value) {
				icg_emit_assign(&g_icg, $2, literal_value);
			}
			free(literal_value);
		}
		free($2);
	}
;

type_spec:
	  INT { $$ = ARCANE_TYPE_INT; }
	| FLOAT { $$ = ARCANE_TYPE_FLOAT; }
	| DOUBLE { $$ = ARCANE_TYPE_DOUBLE; }
	| LONG { $$ = ARCANE_TYPE_LONG; }
	| CHAR { $$ = ARCANE_TYPE_CHAR; }
	| BOOL { $$ = ARCANE_TYPE_BOOL; }
	| VOID { $$ = ARCANE_TYPE_VOID; }
;

init_opt:
	  INIT_ASSIGN expression { $$ = $2; }
	| { $$ = NULL; }
;

assignment:
	  IDENTIFIER '=' expr ';'
	  {
		sync_symbol_context();
		if (lookup_symbol($1) < 0) {
			report_semantic_error("undeclared variable", $1);
		}
		if ($3) {
			icg_emit_assign(&g_icg, $1, $3);
		}
		free($3);
		free($1);
	  }
	| IDENTIFIER INIT_ASSIGN expr ';'
	  {
		sync_symbol_context();
		if (lookup_symbol($1) < 0) {
			report_semantic_error("undeclared variable", $1);
		}
		if ($3) {
			icg_emit_assign(&g_icg, $1, $3);
		}
		free($3);
		free($1);
	  }
;

print_stmt:
	  CAST '(' arg_list_opt ')' ';'
	| PROPHECY '(' arg_list_opt ')' ';'
	| CAST IDENTIFIER opt_semi { icg_emit(&g_icg, "print %s\n", $2); free($2); }
	| CAST STRING opt_semi { icg_emit(&g_icg, "print \"%s\"\n", $2); free($2); }
	| PROPHECY STRING ',' IDENTIFIER opt_semi { icg_emit(&g_icg, "print \"%s\"\n", $2); icg_emit(&g_icg, "print %s\n", $4); free($2); free($4); }
;

opt_semi:
	  ';'
	|
;

input_stmt:
	INPUT '(' IDENTIFIER ')' ';'
	{
		sync_symbol_context();
		if (lookup_symbol($3) < 0) {
			report_semantic_error("undeclared variable", $3);
		}
		free($3);
	}
;

call:
	  SUMMON IDENTIFIER ';'
	  {
		sync_symbol_context();
		if (lookup_symbol($2) < 0) {
			report_semantic_error("undeclared function", $2);
		}
		icg_emit(&g_icg, "call %s\n", $2);
		free($2);
	  }
	| SUMMON IDENTIFIER WITH '(' arg_list_opt ')' ';'
	  {
		sync_symbol_context();
		if (lookup_symbol($2) < 0) {
			report_semantic_error("undeclared function", $2);
		}
		icg_emit(&g_icg, "call %s\n", $2);
		free($2);
	  }
;

if_stmt:
	IF expression
	{
		char *end_label = icg_new_label(&g_icg);
		icg_emit_if_false(&g_icg, $2, end_label);
		free($2);
		$<sval>$ = end_label;
	}
	HOUSE statements FI
	{
		icg_emit_label(&g_icg, $<sval>3);
		free($<sval>3);
	}
	| CHECK expr
	{
		char *end_label = icg_new_label(&g_icg);
		icg_emit_if_false(&g_icg, $2, end_label);
		free($2);
		$<sval>$ = end_label;
	}
	THEN statements ENDCHECK
	{
		icg_emit_label(&g_icg, $<sval>3);
		free($<sval>3);
	}
;

else_block:
	ELSE HOUSE statements ENDHOUSE
;

loop_stmt:
	LOOP
	{
		char *loop_start = icg_new_label(&g_icg);
		char *loop_end = icg_new_label(&g_icg);
		if (loop_start && loop_end && loop_stack_top < 63) {
			loop_stack_top++;
			loop_start_stack[loop_stack_top] = loop_start;
			loop_end_stack[loop_stack_top] = loop_end;
			icg_emit_label(&g_icg, loop_start);
		}
	}
	expr DO
	{
		if (loop_stack_top >= 0) {
			icg_emit_if_false(&g_icg, $3, loop_end_stack[loop_stack_top]);
		}
		free($3);
	}
	statements ENDLOOP
	{
		if (loop_stack_top >= 0) {
			icg_emit_goto(&g_icg, loop_start_stack[loop_stack_top]);
			icg_emit_label(&g_icg, loop_end_stack[loop_stack_top]);
			free(loop_start_stack[loop_stack_top]);
			free(loop_end_stack[loop_stack_top]);
			loop_stack_top--;
		}
	}
;

break_stmt:
	BREAK ';'
	{
		if (loop_stack_top >= 0) {
			icg_emit_goto(&g_icg, loop_end_stack[loop_stack_top]);
		}
	}
;

continue_stmt:
	CONTINUE ';'
	{
		if (loop_stack_top >= 0) {
			icg_emit_goto(&g_icg, loop_start_stack[loop_stack_top]);
		}
	}
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
	  expression { free($1); }
	| arg_list ',' expression { free($3); }
;

id_list_opt:
	  id_list
	|
;

id_list:
	  IDENTIFIER
	  {
		int parameter_insert_result;
		sync_symbol_context();
		parameter_insert_result = arcane_insert_symbol($1, ARCANE_SYMBOL_PARAMETER, ARCANE_TYPE_UNKNOWN, (ArcaneHouse)current_house, current_scope_level, current_function_name, 0);
		if (parameter_insert_result == ARCANE_SYMBOL_DUPLICATE) {
			report_semantic_error("duplicate parameter", $1);
		}
		free($1);
	  }
	| id_list ',' IDENTIFIER
	  {
		int parameter_insert_result;
		sync_symbol_context();
		parameter_insert_result = arcane_insert_symbol($3, ARCANE_SYMBOL_PARAMETER, ARCANE_TYPE_UNKNOWN, (ArcaneHouse)current_house, current_scope_level, current_function_name, 0);
		if (parameter_insert_result == ARCANE_SYMBOL_DUPLICATE) {
			report_semantic_error("duplicate parameter", $3);
		}
		free($3);
	  }
;

expr:
	  expr '+' expr { $$ = emit_binary_temp("+", $1, $3); }
	| expr '-' expr { $$ = emit_binary_temp("-", $1, $3); }
	| expr '*' expr { $$ = emit_binary_temp("*", $1, $3); }
	| expr '/' expr { $$ = emit_binary_temp("/", $1, $3); }
	| expr '>' expr { $$ = emit_binary_temp(">", $1, $3); }
	| expr '<' expr { $$ = emit_binary_temp("<", $1, $3); }
	| expr GE expr { $$ = emit_binary_temp(">=", $1, $3); }
	| expr LE expr { $$ = emit_binary_temp("<=", $1, $3); }
	| expr EQ expr { $$ = emit_binary_temp("==", $1, $3); }
	| expr NE expr { $$ = emit_binary_temp("!=", $1, $3); }
	| IDENTIFIER
	  {
		sync_symbol_context();
		if (lookup_symbol($1) < 0) {
			report_semantic_error("undeclared variable", $1);
		}
		$$ = icg_dup_text($1);
		free($1);
	  }
	| NUMBER { $$ = icg_int_literal($1); }
;

expression:
	  expr { $$ = $1; }
	| STRING
	  {
		$$ = wrap_literal($1, '"');
		free($1);
	  }
	| CHAR_LITERAL
	  {
		$$ = wrap_literal($1, '\'');
		free($1);
	  }
	| BOOL_LITERAL { $$ = $1; }
	| '(' expression ')' { $$ = $2; }
	| '-' expression %prec UMINUS
	  {
		$$ = emit_binary_temp("*", icg_int_literal(-1), $2);
	  }
	| NOT expression
	  {
		char *temp = icg_new_temp(&g_icg);
		if (temp && $2) {
			icg_emit(&g_icg, "%s = NOT %s\n", temp, $2);
		}
		free($2);
		$$ = temp;
	  }
	| expression '%' expression { $$ = emit_binary_temp("%", $1, $3); }
	| expression AND expression { $$ = emit_binary_temp("AND", $1, $3); }
	| expression OR expression { $$ = emit_binary_temp("OR", $1, $3); }
	| expression XOR expression { $$ = emit_binary_temp("XOR", $1, $3); }
	| IDENTIFIER '(' arg_list_opt ')'
	  {
		char *temp;
		sync_symbol_context();
		if (lookup_symbol($1) < 0) {
			report_semantic_error("undeclared function", $1);
		}
		temp = icg_new_temp(&g_icg);
		if (temp) {
			icg_emit(&g_icg, "%s = call %s\n", temp, $1);
		}
		$$ = temp;
		free($1);
	  }
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
	if (strcmp(buffer, WZ_KW_TRUE) == 0 || strcmp(buffer, WZ_KW_FALSE) == 0) {
		yylval.sval = arcane_strdup(buffer);
		return BOOL_LITERAL;
	}

	yylval.sval = arcane_strdup(buffer);
	return IDENTIFIER;
}

static int scan_number(int first) {
	char buffer[256];
	int length = 0;
	int ch = first;
	int seen_dot = 0;

	while (isdigit(ch) || (!seen_dot && ch == '.')) {
		if (ch == '.') {
			seen_dot = 1;
		}
		if (length < (int)sizeof(buffer) - 1) {
			buffer[length++] = (char)ch;
		}
		ch = fgetc(yyin);
	}

	buffer[length] = '\0';
	yylval.ival = atoi(buffer);

	if (ch != EOF) {
		ungetc(ch, yyin);
	}
	return NUMBER;
}

static int scan_string(void) {
	char buffer[256];
	int length = 0;
	int ch;
	while ((ch = fgetc(yyin)) != EOF) {
		if (ch == '\\') {
			int escaped = fgetc(yyin);
			if (escaped == EOF) {
				break;
			}
			if (length < (int)sizeof(buffer) - 2) {
				buffer[length++] = '\\';
				buffer[length++] = (char)escaped;
			}
			continue;
		}
		if (ch == '"') {
			buffer[length] = '\0';
			yylval.sval = arcane_strdup(buffer);
			return STRING;
		}
		if (length < (int)sizeof(buffer) - 1) {
			buffer[length++] = (char)ch;
		}
	}
	return 0;
}

static int scan_char(void) {
	char buffer[4] = {0};
	int length = 0;
	int ch = fgetc(yyin);
	if (ch == '\\') {
		int escaped = fgetc(yyin);
		if (escaped == EOF) {
			return 0;
		}
		buffer[length++] = '\\';
		buffer[length++] = (char)escaped;
	} else if (ch != EOF) {
		buffer[length++] = (char)ch;
	}
	ch = fgetc(yyin);
	if (ch == '\'') {
		buffer[length] = '\0';
		yylval.sval = arcane_strdup(buffer);
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
