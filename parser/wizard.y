%{
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
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
extern int yylineno;
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
char *if_false_stack[64];
char *if_end_stack[64];
int if_stack_top = -1;
#define ARCANE_MAX_FUNCTION_NESTING 64
char function_name_stack[ARCANE_MAX_FUNCTION_NESTING][ARCANE_MAX_NAME_LEN];
int function_scope_stack[ARCANE_MAX_FUNCTION_NESTING];
int function_stack_top = -1;
static char *emit_binary_temp(const char *op, char *left, char *right);
static void report_semantic_errorf(const char *format, ...);

#ifndef ARCANE_PARSER_SEM_TYPES_DEFINED
#define ARCANE_PARSER_SEM_TYPES_DEFINED
typedef struct {
	char *place;
	ArcaneType type;
	int valid;
} ArcaneExprSem;

typedef struct {
	int count;
	ArcaneType types[ARCANE_MAX_FUNCTION_PARAMS];
	char *places[ARCANE_MAX_FUNCTION_PARAMS];
	int valid;
} ArcaneArgSem;

typedef struct {
	int count;
	char *names[ARCANE_MAX_FUNCTION_PARAMS];
	ArcaneType types[ARCANE_MAX_FUNCTION_PARAMS];
	int valid;
} ArcaneParamSem;
#endif

static int semantic_error_count = 0;
#ifndef USE_FLEX_LEXER
static int g_manual_line = 1;
#endif

static int semantic_line(void) {
#ifdef USE_FLEX_LEXER
	return yylineno;
#else
	return g_manual_line;
#endif
}

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

static void push_function_context(void) {
	if (function_stack_top >= ARCANE_MAX_FUNCTION_NESTING - 1) {
		report_semantic_errorf("Function nesting depth exceeded (max %d)", ARCANE_MAX_FUNCTION_NESTING);
		return;
	}

	function_stack_top++;
	function_scope_stack[function_stack_top] = current_scope_level;
	arcane_copy_text(function_name_stack[function_stack_top], ARCANE_MAX_NAME_LEN, current_function_name);
}

static void pop_function_context(void) {
	if (function_stack_top < 0) {
		current_scope_level = 0;
		current_function_name[0] = '\0';
		return;
	}

	current_scope_level = function_scope_stack[function_stack_top];
	arcane_copy_text(current_function_name, sizeof(current_function_name), function_name_stack[function_stack_top]);
	function_stack_top--;
}

static void report_semantic_errorf(const char *format, ...) {
	va_list args;

	semantic_error_count++;
	fprintf(stderr, "Semantic Error (line %d): ", semantic_line());
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
	fprintf(stderr, "\n");
}

static void report_semantic_error(const char *message, const char *name) {
	if (name && name[0] != '\0') {
		report_semantic_errorf("%s %s", message, name);
		return;
	}

	report_semantic_errorf("%s", message);
}

static int arcane_type_is_numeric(ArcaneType type) {
	return type == ARCANE_TYPE_INT || type == ARCANE_TYPE_FLOAT || type == ARCANE_TYPE_DOUBLE ||
		type == ARCANE_TYPE_LONG || type == ARCANE_TYPE_CHAR;
}

static int arcane_type_is_integral(ArcaneType type) {
	return type == ARCANE_TYPE_INT || type == ARCANE_TYPE_LONG || type == ARCANE_TYPE_CHAR;
}

static ArcaneType arcane_numeric_result_type(ArcaneType left, ArcaneType right) {
	if (left == ARCANE_TYPE_DOUBLE || right == ARCANE_TYPE_DOUBLE) {
		return ARCANE_TYPE_DOUBLE;
	}
	if (left == ARCANE_TYPE_FLOAT || right == ARCANE_TYPE_FLOAT) {
		return ARCANE_TYPE_FLOAT;
	}
	if (left == ARCANE_TYPE_LONG || right == ARCANE_TYPE_LONG) {
		return ARCANE_TYPE_LONG;
	}
	return ARCANE_TYPE_INT;
}

static int arcane_assignment_compatible(ArcaneType destination, ArcaneType source) {
	if (destination == source) {
		return 1;
	}

	if (destination == ARCANE_TYPE_BOOL && source == ARCANE_TYPE_BOOL) {
		return 1;
	}

	if (arcane_type_is_numeric(destination) && arcane_type_is_numeric(source)) {
		ArcaneType promoted = arcane_numeric_result_type(destination, source);
		return promoted == destination;
	}

	return 0;
}

static int is_temp_name_text(const char *name) {
	size_t i;
	if (!name || name[0] != 't' || !isdigit((unsigned char)name[1])) {
		return 0;
	}
	for (i = 2; name[i] != '\0'; ++i) {
		if (!isdigit((unsigned char)name[i])) {
			return 0;
		}
	}
	return 1;
}

static int is_label_name_text(const char *name) {
	size_t i;
	if (!name || name[0] != 'L' || !isdigit((unsigned char)name[1])) {
		return 0;
	}
	for (i = 2; name[i] != '\0'; ++i) {
		if (!isdigit((unsigned char)name[i])) {
			return 0;
		}
	}
	return 1;
}

static int is_numeric_text(const char *text) {
	char *end;
	if (!text || text[0] == '\0') {
		return 0;
	}
	(void)strtod(text, &end);
	return end && *end == '\0';
}

static int symbol_exists_any(const char *name) {
	int i;
	if (!name || name[0] == '\0') {
		return 0;
	}
	for (i = 0; i < g_arcane_symbol_table.count; ++i) {
		if (strcmp(g_arcane_symbol_table.entries[i].name, name) == 0) {
			return 1;
		}
	}
	return 0;
}

static ArcaneType infer_expression_type(const char *expr_text) {
	char expr[256];
	char *end;
	ArcaneType t = ARCANE_TYPE_UNKNOWN;
	size_t len;

	if (!expr_text) {
		return ARCANE_TYPE_UNKNOWN;
	}

	while (*expr_text && isspace((unsigned char)*expr_text)) {
		expr_text++;
	}
	len = strcspn(expr_text, "\r\n");
	while (len > 0 && isspace((unsigned char)expr_text[len - 1])) {
		len--;
	}
	if (len == 0) {
		return ARCANE_TYPE_UNKNOWN;
	}
	if (len >= sizeof(expr)) {
		len = sizeof(expr) - 1;
	}
	memcpy(expr, expr_text, len);
	expr[len] = '\0';

	if (expr[0] == '"') return ARCANE_TYPE_STRING;
	if (expr[0] == '\'') return ARCANE_TYPE_CHAR;
	if (strcmp(expr, "true") == 0 || strcmp(expr, "false") == 0) return ARCANE_TYPE_BOOL;

	if (is_numeric_text(expr)) {
		if (strchr(expr, '.') || strchr(expr, 'e') || strchr(expr, 'E')) {
			return ARCANE_TYPE_DOUBLE;
		}
		return ARCANE_TYPE_INT;
	}

	if (strncmp(expr, "POWO(", 5) == 0 || strncmp(expr, "RADIX(", 6) == 0 || strncmp(expr, "LOGUS(", 6) == 0 ||
		strncmp(expr, "SINUS(", 6) == 0 || strncmp(expr, "COSINUS(", 8) == 0 || strncmp(expr, "TANUS(", 6) == 0 ||
		strncmp(expr, "FLOORUS(", 8) == 0 || strncmp(expr, "CEILUS(", 7) == 0 || strncmp(expr, "ABSOLUTUS(", 10) == 0) {
		return ARCANE_TYPE_DOUBLE;
	}

	if (lookup_symbol(expr) >= 0) {
		ArcaneType t = ARCANE_TYPE_UNKNOWN;
		(void)arcane_symbol_get_type(expr, &t);
		return t;
	}

	if (is_temp_name_text(expr)) {
		return ARCANE_TYPE_INT;
	}

	end = strchr(expr, '(');
	if (end) {
		char fn[64];
		size_t fn_len = (size_t)(end - expr);
		if (fn_len >= sizeof(fn)) {
			fn_len = sizeof(fn) - 1;
		}
		memcpy(fn, expr, fn_len);
		fn[fn_len] = '\0';
		if (arcane_symbol_get_function_signature(fn, &t, NULL, NULL, 0) == ARCANE_SYMBOL_OK) {
			return t;
		}
	}

	return ARCANE_TYPE_UNKNOWN;
}

static void perform_type_check(void) {
	int i;
	int local_errors = 0;

	for (i = 0; i < g_icg.line_count; ++i) {
		const char *line = g_icg.lines[i];
		const char *eq;
		const char *rhs_start;
		char lhs[64];
		char rhs[256];
		ArcaneType lhs_type;
		ArcaneType rhs_type;
		size_t lhs_len;
		size_t rhs_len;

		if (!line || line[0] == '#') {
			continue;
		}
		eq = strchr(line, '=');
		if (!eq) {
			continue;
		}

		lhs_len = (size_t)(eq - line);
		while (lhs_len > 0 && isspace((unsigned char)line[lhs_len - 1])) {
			lhs_len--;
		}
		if (lhs_len == 0 || lhs_len >= sizeof(lhs)) {
			continue;
		}
		memcpy(lhs, line, lhs_len);
		lhs[lhs_len] = '\0';

		if (is_temp_name_text(lhs)) {
			continue;
		}
		if (lookup_symbol(lhs) < 0) {
			continue;
		}

		rhs_start = eq + 1;
		while (*rhs_start && isspace((unsigned char)*rhs_start)) {
			rhs_start++;
		}

		rhs_len = strcspn(rhs_start, "\r\n");
		while (rhs_len > 0 && isspace((unsigned char)rhs_start[rhs_len - 1])) {
			rhs_len--;
		}
		if (rhs_len == 0 || rhs_len >= sizeof(rhs)) {
			continue;
		}
		memcpy(rhs, rhs_start, rhs_len);
		rhs[rhs_len] = '\0';

		lhs_type = ARCANE_TYPE_UNKNOWN;
		(void)arcane_symbol_get_type(lhs, &lhs_type);
		rhs_type = infer_expression_type(rhs);

		if (rhs_type != ARCANE_TYPE_UNKNOWN && !arcane_assignment_compatible(lhs_type, rhs_type)) {
			report_semantic_errorf("Type mismatch for variable '%s': %s <- %s", lhs, arcane_type_name(lhs_type), arcane_type_name(rhs_type));
			local_errors++;
		}
	}

	if (local_errors == 0) {
		printf("Semantic Check: CHECK_TYPES completed (no additional issues)\n");
	}
}

static void perform_declaration_check(void) {
	int i;
	int j;
	int local_errors = 0;

	for (i = 0; i < g_arcane_symbol_table.count; ++i) {
		ArcaneSymbol *sym = &g_arcane_symbol_table.entries[i];
		if ((sym->kind == ARCANE_SYMBOL_VARIABLE || sym->kind == ARCANE_SYMBOL_PARAMETER) && sym->used && !sym->initialized) {
			report_semantic_errorf("Variable '%s' may be used before initialization", sym->name);
			local_errors++;
		}
		if ((sym->kind == ARCANE_SYMBOL_VARIABLE || sym->kind == ARCANE_SYMBOL_PARAMETER) && sym->type == ARCANE_TYPE_UNKNOWN) {
			report_semantic_errorf("Variable '%s' has unknown type", sym->name);
			local_errors++;
		}
	}

	for (i = 0; i < g_arcane_symbol_table.count; ++i) {
		for (j = i + 1; j < g_arcane_symbol_table.count; ++j) {
			ArcaneSymbol *a = &g_arcane_symbol_table.entries[i];
			ArcaneSymbol *b = &g_arcane_symbol_table.entries[j];
			if (a->kind != b->kind) continue;
			if (a->scope_level != b->scope_level) continue;
			if (strcmp(a->owner_function, b->owner_function) != 0) continue;
			if (strcmp(a->name, b->name) == 0) {
				report_semantic_errorf("Duplicate declaration detected for '%s'", a->name);
				local_errors++;
			}
		}
	}

	for (i = 0; i < g_icg.line_count; ++i) {
		const char *line = g_icg.lines[i];
		int k = 0;
		if (!line || line[0] == '#') continue;
		while (line[k]) {
			if (line[k] == '"') {
				k++;
				while (line[k] && line[k] != '"') {
					if (line[k] == '\\' && line[k + 1]) {
						k += 2;
					} else {
						k++;
					}
				}
				if (line[k] == '"') {
					k++;
				}
				continue;
			}
			if (isalpha((unsigned char)line[k]) || line[k] == '_') {
				char tok[64];
				int t = 0;
				while ((isalpha((unsigned char)line[k]) || isdigit((unsigned char)line[k]) || line[k] == '_') && t < (int)sizeof(tok) - 1) {
					tok[t++] = line[k++];
				}
				tok[t] = '\0';

				if (strcmp(tok, "ifFalse") == 0 || strcmp(tok, "goto") == 0 || strcmp(tok, "call") == 0 ||
					strcmp(tok, "func_begin") == 0 || strcmp(tok, "func_end") == 0 || strcmp(tok, "param") == 0 ||
					strcmp(tok, "arg") == 0 || strcmp(tok, "return") == 0 || strcmp(tok, "print") == 0 ||
					strcmp(tok, "AND") == 0 || strcmp(tok, "OR") == 0 || strcmp(tok, "NOT") == 0 || strcmp(tok, "XOR") == 0 ||
					strcmp(tok, "POWO") == 0 || strcmp(tok, "RADIX") == 0 || strcmp(tok, "FLOORUS") == 0 || strcmp(tok, "CEILUS") == 0 ||
					strcmp(tok, "ABSOLUTUS") == 0 || strcmp(tok, "LOGUS") == 0 || strcmp(tok, "SINUS") == 0 || strcmp(tok, "COSINUS") == 0 || strcmp(tok, "TANUS") == 0 ||
					is_temp_name_text(tok) || is_label_name_text(tok)) {
					continue;
				}

				if (!symbol_exists_any(tok) && strcmp(tok, "true") != 0 && strcmp(tok, "false") != 0) {
					report_semantic_errorf("Undeclared variable '%s'", tok);
					local_errors++;
				}
				continue;
			}
			k++;
		}
	}

	if (local_errors == 0) {
		printf("Semantic Check: CHECK_DECLARATIONS completed (no additional issues)\n");
	}
}

static ArcaneExprSem arcane_expr_invalid(void) {
	ArcaneExprSem expr;

	expr.place = icg_dup_text("0");
	expr.type = ARCANE_TYPE_UNKNOWN;
	expr.valid = 0;
	return expr;
}

static ArcaneExprSem arcane_expr_make(char *place, ArcaneType type, int valid) {
	ArcaneExprSem expr;

	expr.place = place;
	expr.type = type;
	expr.valid = valid;
	return expr;
}

static ArcaneExprSem arcane_expr_identifier(const char *name) {
	int symbol_index;
	ArcaneType symbol_type;
	int initialized = 0;

	symbol_index = lookup_symbol(name);
	if (symbol_index < 0) {
		report_semantic_error("undeclared variable", name);
		return arcane_expr_invalid();
	}

	if (arcane_symbol_get_type(name, &symbol_type) != ARCANE_SYMBOL_OK) {
		symbol_type = ARCANE_TYPE_UNKNOWN;
	}

	if (arcane_symbol_get_initialized(name, &initialized) == ARCANE_SYMBOL_OK) {
		if (!initialized) {
			report_semantic_errorf("Variable '%s' used before initialization", name);
		}
	}

	(void)arcane_symbol_mark_used(name);
	return arcane_expr_make(icg_dup_text(name), symbol_type, initialized != 0);
}

static ArcaneExprSem arcane_expr_binary_numeric(const char *op, ArcaneExprSem left, ArcaneExprSem right) {
	ArcaneType result_type;

	if (!left.valid || !right.valid) {
		free(left.place);
		free(right.place);
		return arcane_expr_invalid();
	}

	if (!arcane_type_is_numeric(left.type) || !arcane_type_is_numeric(right.type)) {
		report_semantic_errorf("Invalid operand types for '%s': %s and %s", op, arcane_type_name(left.type), arcane_type_name(right.type));
		free(left.place);
		free(right.place);
		return arcane_expr_invalid();
	}

	result_type = arcane_numeric_result_type(left.type, right.type);
	return arcane_expr_make(emit_binary_temp(op, left.place, right.place), result_type, 1);
}

static ArcaneExprSem arcane_expr_binary_compare(const char *op, ArcaneExprSem left, ArcaneExprSem right) {
	if (!left.valid || !right.valid) {
		free(left.place);
		free(right.place);
		return arcane_expr_invalid();
	}

	if (!arcane_type_is_numeric(left.type) || !arcane_type_is_numeric(right.type)) {
		report_semantic_errorf("Invalid comparison operand types for '%s': %s and %s", op, arcane_type_name(left.type), arcane_type_name(right.type));
		free(left.place);
		free(right.place);
		return arcane_expr_invalid();
	}

	return arcane_expr_make(emit_binary_temp(op, left.place, right.place), ARCANE_TYPE_BOOL, 1);
}

static ArcaneArgSem arcane_args_empty(void) {
	ArcaneArgSem args;
	int i;

	args.count = 0;
	args.valid = 1;
	for (i = 0; i < ARCANE_MAX_FUNCTION_PARAMS; ++i) {
		args.places[i] = NULL;
	}
	return args;
}

static void arcane_args_release(ArcaneArgSem *args) {
	int i;

	if (!args) {
		return;
	}

	for (i = 0; i < args->count; ++i) {
		free(args->places[i]);
		args->places[i] = NULL;
	}
	args->count = 0;
	args->valid = 0;
}

static ArcaneArgSem arcane_args_append(ArcaneArgSem args, ArcaneType type, char *place, int value_valid) {
	if (!value_valid) {
		args.valid = 0;
	}

	if (args.count < ARCANE_MAX_FUNCTION_PARAMS) {
		args.types[args.count++] = type;
		args.places[args.count - 1] = place;
	} else {
		report_semantic_errorf("Too many arguments in function call (max %d)", ARCANE_MAX_FUNCTION_PARAMS);
		args.valid = 0;
		free(place);
	}

	return args;
}

static ArcaneParamSem arcane_params_empty(void) {
	ArcaneParamSem params;
	int i;

	params.count = 0;
	params.valid = 1;
	for (i = 0; i < ARCANE_MAX_FUNCTION_PARAMS; ++i) {
		params.names[i] = NULL;
		params.types[i] = ARCANE_TYPE_UNKNOWN;
	}
	return params;
}

static void arcane_params_release(ArcaneParamSem *params) {
	int i;

	if (!params) {
		return;
	}

	for (i = 0; i < params->count; ++i) {
		free(params->names[i]);
		params->names[i] = NULL;
	}
	params->count = 0;
	params->valid = 0;
}

static ArcaneParamSem arcane_params_append(ArcaneParamSem params, char *name, ArcaneType type) {
	int i;

	if (!name) {
		params.valid = 0;
		return params;
	}

	for (i = 0; i < params.count; ++i) {
		if (strcmp(params.names[i], name) == 0) {
			report_semantic_errorf("duplicate parameter %s", name);
			params.valid = 0;
			free(name);
			return params;
		}
	}

	if (params.count >= ARCANE_MAX_FUNCTION_PARAMS) {
		report_semantic_errorf("Too many function parameters (max %d)", ARCANE_MAX_FUNCTION_PARAMS);
		params.valid = 0;
		free(name);
		return params;
	}

	params.names[params.count] = name;
	params.types[params.count] = type;
	params.count++;
	return params;
}

static ArcaneType arcane_validate_function_call(const char *name, ArcaneArgSem args, int used_in_expression) {
	int i;
	int expected_arity;
	ArcaneType expected_param_types[ARCANE_MAX_FUNCTION_PARAMS];
	ArcaneType return_type = ARCANE_TYPE_UNKNOWN;

	if (arcane_symbol_get_function_signature(name, &return_type, &expected_arity, expected_param_types, ARCANE_MAX_FUNCTION_PARAMS) != ARCANE_SYMBOL_OK) {
		report_semantic_error("undeclared function", name);
		return ARCANE_TYPE_UNKNOWN;
	}

	if (expected_arity != args.count) {
		report_semantic_errorf("Function '%s' expects %d argument(s) but got %d", name, expected_arity, args.count);
	}

	for (i = 0; i < args.count && i < expected_arity; ++i) {
		if (expected_param_types[i] == ARCANE_TYPE_UNKNOWN || args.types[i] == ARCANE_TYPE_UNKNOWN) {
			continue;
		}
		if (!arcane_assignment_compatible(expected_param_types[i], args.types[i])) {
			report_semantic_errorf(
				"Type mismatch in function call argument %d for '%s': expected %s but got %s",
				i + 1,
				name,
				arcane_type_name(expected_param_types[i]),
				arcane_type_name(args.types[i])
			);
		}
	}

	if (!args.valid) {
		report_semantic_errorf("Invalid argument list in call to '%s'", name);
	}

	if (used_in_expression && return_type == ARCANE_TYPE_VOID) {
		report_semantic_errorf("Function '%s' returns VOID and cannot be used in an expression", name);
	}

	return return_type;
}

static void arcane_begin_function_declaration(const char *name, ArcaneType return_type, ArcaneParamSem params) {
	char parent_function[ARCANE_MAX_NAME_LEN];
	int function_scope;
	int existing_index;
	const char *function_owner;
	int i;
	ArcaneType existing_return_type;
	int existing_param_count;
	ArcaneType existing_param_types[ARCANE_MAX_FUNCTION_PARAMS];

	arcane_copy_text(parent_function, sizeof(parent_function), current_function_name);
	function_scope = (parent_function[0] == '\0') ? 0 : current_scope_level;
	function_owner = (parent_function[0] == '\0') ? "" : parent_function;

	sync_symbol_context();
	existing_index = arcane_find_symbol(name, function_scope, function_owner);
	if (existing_index >= 0) {
		if (g_arcane_symbol_table.entries[existing_index].kind == ARCANE_SYMBOL_FUNCTION &&
			arcane_symbol_get_function_signature(name, &existing_return_type, &existing_param_count, existing_param_types, ARCANE_MAX_FUNCTION_PARAMS) == ARCANE_SYMBOL_OK) {
			int consistent = 1;
			if (existing_return_type != return_type || existing_param_count != params.count) {
				consistent = 0;
			} else {
				for (i = 0; i < params.count; ++i) {
					if (existing_param_types[i] != params.types[i]) {
						consistent = 0;
						break;
					}
				}
			}

			if (consistent) {
				report_semantic_errorf("duplicate function declaration '%s'", name);
			} else {
				report_semantic_errorf("inconsistent redeclaration of function '%s'", name);
			}
		}
	} else {
		int function_insert_result;
		function_insert_result = arcane_insert_symbol(name, ARCANE_SYMBOL_FUNCTION, return_type, (ArcaneHouse)current_house, function_scope, function_owner, 0);
		if (function_insert_result != ARCANE_SYMBOL_OK) {
			report_semantic_error("duplicate function", name);
		} else {
			(void)arcane_symbol_set_function_signature(name, return_type, params.count, params.types);
		}
	}

	push_function_context();
	arcane_copy_text(current_function_name, sizeof(current_function_name), name);
	current_scope_level = function_scope + 1;
	sync_symbol_context();
	icg_emit_func_begin(&g_icg, current_function_name);

	for (i = 0; i < params.count; ++i) {
		int parameter_insert_result = arcane_insert_symbol(
			params.names[i],
			ARCANE_SYMBOL_PARAMETER,
			params.types[i],
			(ArcaneHouse)current_house,
			current_scope_level,
			current_function_name,
			0
		);
		if (parameter_insert_result == ARCANE_SYMBOL_DUPLICATE) {
			report_semantic_error("duplicate parameter", params.names[i]);
		}
		icg_emit_param(&g_icg, params.names[i]);
	}
}

static void arcane_end_function_declaration(void) {
	char finished_name[ARCANE_MAX_NAME_LEN];

	if (current_function_name[0] == '\0') {
		return;
	}

	arcane_copy_text(finished_name, sizeof(finished_name), current_function_name);
	icg_emit_func_end(&g_icg, finished_name);
	pop_function_context();
	sync_symbol_context();
}

static ArcaneExprSem arcane_expr_binary_logic(const char *op, ArcaneExprSem left, ArcaneExprSem right) {
	if (!left.valid || !right.valid) {
		free(left.place);
		free(right.place);
		return arcane_expr_invalid();
	}

	if (left.type != ARCANE_TYPE_BOOL || right.type != ARCANE_TYPE_BOOL) {
		report_semantic_errorf("Invalid logical operand types for '%s': %s and %s", op, arcane_type_name(left.type), arcane_type_name(right.type));
		free(left.place);
		free(right.place);
		return arcane_expr_invalid();
	}

	return arcane_expr_make(emit_binary_temp(op, left.place, right.place), ARCANE_TYPE_BOOL, 1);
}

static ArcaneExprSem arcane_expr_math_call(const char *keyword, ArcaneExprSem arg1, ArcaneExprSem arg2, int arity) {
	char *temp;
	ArcaneType result_type;

	if (!arg1.valid || (arity == 2 && !arg2.valid)) {
		if (arity == 2) {
			report_semantic_errorf("%s expects 2 arguments", keyword);
		} else {
			report_semantic_errorf("%s expects 1 argument", keyword);
		}
		free(arg1.place);
		free(arg2.place);
		return arcane_expr_invalid();
	}

	if (!arcane_type_is_numeric(arg1.type) || (arity == 2 && !arcane_type_is_numeric(arg2.type))) {
		if (arity == 2) {
			report_semantic_errorf("%s expects numeric arguments", keyword);
		} else {
			report_semantic_errorf("%s expects numeric input", keyword);
		}
		free(arg1.place);
		free(arg2.place);
		return arcane_expr_invalid();
	}

	temp = icg_new_temp(&g_icg);
	if (!temp) {
		free(arg1.place);
		free(arg2.place);
		return arcane_expr_invalid();
	}

	icg_emit_math_call(&g_icg, temp, keyword, arg1.place, (arity == 2 ? arg2.place : NULL), arity);

	result_type = arg1.type;
	if (arity == 2) {
		result_type = arcane_numeric_result_type(arg1.type, arg2.type);
	}
	if (strcmp(keyword, "RADIX") == 0 || strcmp(keyword, "LOGUS") == 0 || strcmp(keyword, "SINUS") == 0 || strcmp(keyword, "COSINUS") == 0 || strcmp(keyword, "TANUS") == 0) {
		if (result_type == ARCANE_TYPE_INT || result_type == ARCANE_TYPE_LONG || result_type == ARCANE_TYPE_CHAR) {
			result_type = ARCANE_TYPE_DOUBLE;
		}
	}

	free(arg1.place);
	free(arg2.place);
	return arcane_expr_make(temp, result_type, 1);
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

%code requires {
#include "../symbol_table/symbol_table.h"

#ifndef ARCANE_PARSER_SEM_TYPES_DEFINED
#define ARCANE_PARSER_SEM_TYPES_DEFINED
typedef struct {
	char *place;
	ArcaneType type;
	int valid;
} ArcaneExprSem;

typedef struct {
	int count;
	ArcaneType types[ARCANE_MAX_FUNCTION_PARAMS];
	char *places[ARCANE_MAX_FUNCTION_PARAMS];
	int valid;
} ArcaneArgSem;

typedef struct {
	int count;
	char *names[ARCANE_MAX_FUNCTION_PARAMS];
	ArcaneType types[ARCANE_MAX_FUNCTION_PARAMS];
	int valid;
} ArcaneParamSem;
#endif
}

%start program

%union {
	int ival;
	char *sval;
	ArcaneExprSem expr;
	ArcaneArgSem args;
	ArcaneParamSem params;
}

%token ENTER_HOGWARTS EXIT_HOGWARTS HOUSE ENDHOUSE
%token GRYFFINDOR SLYTHERIN HUFFLEPUFF RAVENCLAW
%token INT FLOAT DOUBLE LONG CHAR BOOL VOID
%token DECLARE AS INIT_ASSIGN
%token IF_CHARM OTHERWISE ELSE_CHARM THEN END_CHARM
%token TIME_TURNER BEGIN_LOOP END_TIME_TURNER BREAK_SPELL KEEP_CASTING
%token SPELL ENDSPELL SUMMON WITH RETURN
%token CAST PROPHECY INPUT
%token POTION ENDPOTION SORTING_HAT ENDHAT CHECK_TYPES CHECK_DECLARATIONS
%token AND OR NOT XOR
%token POWO RADIX FLOORUS CEILUS ABSOLUTUS LOGUS SINUS COSINUS TANUS
%token <sval> IDENTIFIER STRING CHAR_LITERAL BOOL_LITERAL
%token <ival> NUMBER
%token EQ NE GE LE

%type <ival> type_spec
%type <ival> function_return_opt
%type <expr> init_opt expr expression expression_opt
%type <args> arg_list_opt arg_list
%type <params> param_list_opt param_list param_decl

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
		if_stack_top = -1;
		function_stack_top = -1;
		current_scope_level = 0;
		current_function_name[0] = '\0';
		sync_symbol_context();
		icg_emit(&g_icg, "# ICG BEGIN\n");
	}
	house_blocks EXIT_HOGWARTS
	{
		icg_emit(&g_icg, "# ICG END\n");
		printf("=== RAW_ICG_START ===\n");
		print_icg(&g_icg, stdout);
		printf("=== RAW_ICG_END ===\n");
		optimizer_run_all(&g_icg);
		printf("=== OPT_ICG_START ===\n");
		print_icg(&g_icg, stdout);
		printf("=== OPT_ICG_END ===\n");
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
	RAVENCLAW TIME_TURNER expr BEGIN_LOOP statements END_TIME_TURNER
;

potion_block:
	POTION HOUSE statements ENDPOTION
;

sorting_hat_block:
	SORTING_HAT HOUSE semantic_checks ENDHAT
;

function:
	  SPELL IDENTIFIER function_return_opt WITH '(' param_list_opt ')'
	  {
		if (current_house != 2) {
			report_semantic_errorf("function definition is only allowed in Slytherin (current: %s)", house_name_from_id(current_house));
		}
		arcane_begin_function_declaration($2, (ArcaneType)$3, $6);
		arcane_params_release(&$6);
		free($2);
	  }
	  HOUSE statements ENDSPELL
	  {
		arcane_end_function_declaration();
	  }
	| SPELL IDENTIFIER function_return_opt
	  {
		ArcaneParamSem no_params = arcane_params_empty();
		if (current_house != 2) {
			report_semantic_errorf("function definition is only allowed in Slytherin (current: %s)", house_name_from_id(current_house));
		}
		arcane_begin_function_declaration($2, (ArcaneType)$3, no_params);
		free($2);
	  }
	  HOUSE statements ENDSPELL
	  {
		arcane_end_function_declaration();
	  }
	| SPELL IDENTIFIER function_return_opt
	  {
		ArcaneParamSem no_params = arcane_params_empty();
		if (current_house != 2) {
			report_semantic_errorf("function definition is only allowed in Slytherin (current: %s)", house_name_from_id(current_house));
		}
		arcane_begin_function_declaration($2, (ArcaneType)$3, no_params);
		free($2);
	  }
	  statements ENDSPELL
	  {
		arcane_end_function_declaration();
	  }
;

function_return_opt:
	  AS type_spec { $$ = $2; }
	| { $$ = ARCANE_TYPE_VOID; }
;

semantic_checks:
	  semantic_checks semantic_check
	|
;

semantic_check:
	  CHECK_TYPES ';'
	  {
		perform_type_check();
	  }
	| CHECK_DECLARATIONS ';'
	  {
		perform_declaration_check();
	  }
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
;

declaration:
	DECLARE IDENTIFIER AS type_spec init_opt ';'
	{
		int declaration_result;
		int symbol_index;
		ArcaneType lhs_type;
		sync_symbol_context();
		if (current_house != 1) {
			report_semantic_errorf("declaration is only allowed in Gryffindor (current: %s)", house_name_from_id(current_house));
		}

		declaration_result = arcane_insert_variable($2, (ArcaneType)$4, (ArcaneHouse)current_house, current_scope_level, current_function_name, 0);
		if (declaration_result == ARCANE_SYMBOL_DUPLICATE) {
			report_semantic_error("duplicate variable", $2);
		}

		if ($5.valid && declaration_result == ARCANE_SYMBOL_OK) {
			symbol_index = lookup_symbol($2);
			lhs_type = (ArcaneType)$4;
			if (!arcane_assignment_compatible(lhs_type, $5.type)) {
				report_semantic_errorf("Type mismatch in assignment to '%s': %s <- %s", $2, arcane_type_name(lhs_type), arcane_type_name($5.type));
			} else if (symbol_index >= 0) {
				arcane_symbol_table_mark_initialized(&g_arcane_symbol_table, symbol_index);
				icg_emit_assign(&g_icg, $2, $5.place);
			}
		}
		free($5.place);
		free($2);
	}
	| DECLARE IDENTIFIER AS INT '=' NUMBER opt_semi
	{
		char *literal_value;
		sync_symbol_context();
		if (current_house != 1) {
			report_semantic_errorf("declaration is only allowed in Gryffindor (current: %s)", house_name_from_id(current_house));
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
	| {
		$$.place = NULL;
		$$.type = ARCANE_TYPE_UNKNOWN;
		$$.valid = 0;
	}
;

assignment:
	  IDENTIFIER '=' expression ';'
	  {
		int symbol_index;
		ArcaneType lhs_type;
		symbol_index = -1;
		sync_symbol_context();
		symbol_index = lookup_symbol($1);
		if (symbol_index < 0) {
			report_semantic_error("undeclared variable", $1);
		} else {
			lhs_type = g_arcane_symbol_table.entries[symbol_index].type;
			if (!$3.valid) {
				report_semantic_errorf("Type mismatch in assignment to '%s'", $1);
			} else if (!arcane_assignment_compatible(lhs_type, $3.type)) {
				report_semantic_errorf("Type mismatch in assignment to '%s': %s <- %s", $1, arcane_type_name(lhs_type), arcane_type_name($3.type));
			} else {
				icg_emit_assign(&g_icg, $1, $3.place);
				arcane_symbol_table_mark_initialized(&g_arcane_symbol_table, symbol_index);
			}
		}
		free($3.place);
		free($1);
	  }
	| IDENTIFIER INIT_ASSIGN expression ';'
	  {
		int symbol_index;
		ArcaneType lhs_type;
		symbol_index = -1;
		sync_symbol_context();
		symbol_index = lookup_symbol($1);
		if (symbol_index < 0) {
			report_semantic_error("undeclared variable", $1);
		} else {
			lhs_type = g_arcane_symbol_table.entries[symbol_index].type;
			if (!$3.valid) {
				report_semantic_errorf("Type mismatch in assignment to '%s'", $1);
			} else if (!arcane_assignment_compatible(lhs_type, $3.type)) {
				report_semantic_errorf("Type mismatch in assignment to '%s': %s <- %s", $1, arcane_type_name(lhs_type), arcane_type_name($3.type));
			} else {
				icg_emit_assign(&g_icg, $1, $3.place);
				arcane_symbol_table_mark_initialized(&g_arcane_symbol_table, symbol_index);
			}
		}
		free($3.place);
		free($1);
	  }
;

print_stmt:
	  CAST '(' arg_list_opt ')' ';' { arcane_args_release(&$3); }
	| PROPHECY '(' arg_list_opt ')' ';' { arcane_args_release(&$3); }
	| CAST IDENTIFIER opt_semi {
		ArcaneType print_type;
		int initialized = 0;
		if (lookup_symbol($2) < 0) {
			report_semantic_error("undeclared variable", $2);
		} else {
			(void)arcane_symbol_get_type($2, &print_type);
			if (arcane_symbol_get_initialized($2, &initialized) == ARCANE_SYMBOL_OK && !initialized) {
				report_semantic_errorf("Variable '%s' used before initialization", $2);
			}
			(void)arcane_symbol_mark_used($2);
			icg_emit(&g_icg, "print %s\n", $2);
		}
		free($2);
	}
	| CAST STRING opt_semi { icg_emit(&g_icg, "print \"%s\"\n", $2); free($2); }
	| PROPHECY STRING ',' IDENTIFIER opt_semi {
		int initialized = 0;
		icg_emit(&g_icg, "print \"%s\"\n", $2);
		if (lookup_symbol($4) < 0) {
			report_semantic_error("undeclared variable", $4);
		} else {
			if (arcane_symbol_get_initialized($4, &initialized) == ARCANE_SYMBOL_OK && !initialized) {
				report_semantic_errorf("Variable '%s' used before initialization", $4);
			}
			(void)arcane_symbol_mark_used($4);
			icg_emit(&g_icg, "print %s\n", $4);
		}
		free($2);
		free($4);
	}
;

opt_semi:
	  ';'
	|
;

input_stmt:
	INPUT '(' IDENTIFIER ')' ';'
	{
		int symbol_index;
		sync_symbol_context();
		symbol_index = lookup_symbol($3);
		if (symbol_index < 0) {
			report_semantic_error("undeclared variable", $3);
		} else {
			arcane_symbol_table_mark_initialized(&g_arcane_symbol_table, symbol_index);
		}
		free($3);
	}
;

call:
	  SUMMON IDENTIFIER ';'
	  {
		ArcaneArgSem args;
		sync_symbol_context();
		args = arcane_args_empty();
		(void)arcane_validate_function_call($2, args, 0);
		icg_emit_call(&g_icg, $2, 0);
		arcane_args_release(&args);
		free($2);
	  }
	| SUMMON IDENTIFIER WITH '(' arg_list_opt ')' ';'
	  {
		int i;
		sync_symbol_context();
		(void)arcane_validate_function_call($2, $5, 0);
		for (i = 0; i < $5.count; ++i) {
			icg_emit_arg(&g_icg, $5.places[i] ? $5.places[i] : "0");
		}
		icg_emit_call(&g_icg, $2, $5.count);
		arcane_args_release(&$5);
		free($2);
	  }
;

if_stmt:
	IF_CHARM expr
	{
		char *false_label = icg_new_label(&g_icg);
		char *end_label = icg_new_label(&g_icg);
		if (current_house != 3) {
			report_semantic_errorf("IF_CHARM is only allowed in Hufflepuff (current: %s)", house_name_from_id(current_house));
		}
		if ($2.type != ARCANE_TYPE_BOOL || !$2.valid) {
			report_semantic_errorf("Condition must be boolean");
		}
		icg_emit_if_false(&g_icg, $2.place ? $2.place : "0", false_label);
		if (if_stack_top < 63) {
			if_stack_top++;
			if_false_stack[if_stack_top] = false_label;
			if_end_stack[if_stack_top] = end_label;
		} else {
			free(false_label);
			free(end_label);
			report_semantic_errorf("Nested IF_CHARM depth exceeded");
		}
		free($2.place);
	}
	THEN statements if_else_part END_CHARM
;

if_else_part:
	{
		if (if_stack_top >= 0) {
			icg_emit_label(&g_icg, if_false_stack[if_stack_top]);
			icg_emit_label(&g_icg, if_end_stack[if_stack_top]);
			free(if_false_stack[if_stack_top]);
			free(if_end_stack[if_stack_top]);
			if_stack_top--;
		}
	}
	| OTHERWISE
	{
		if (if_stack_top >= 0) {
			icg_emit_goto(&g_icg, if_end_stack[if_stack_top]);
			icg_emit_label(&g_icg, if_false_stack[if_stack_top]);
			free(if_false_stack[if_stack_top]);
			if_false_stack[if_stack_top] = NULL;
		}
	}
	statements
	{
		if (if_stack_top >= 0) {
			icg_emit_label(&g_icg, if_end_stack[if_stack_top]);
			free(if_end_stack[if_stack_top]);
			if_end_stack[if_stack_top] = NULL;
			if_stack_top--;
		}
	}
	| ELSE_CHARM
	{
		char *false_label = icg_new_label(&g_icg);
		if (if_stack_top >= 0) {
			icg_emit_goto(&g_icg, if_end_stack[if_stack_top]);
			icg_emit_label(&g_icg, if_false_stack[if_stack_top]);
			free(if_false_stack[if_stack_top]);
			if_false_stack[if_stack_top] = false_label;
		}
	}
	expr
	{
		if ($3.type != ARCANE_TYPE_BOOL || !$3.valid) {
			report_semantic_errorf("Condition must be boolean");
		}
		if (if_stack_top >= 0 && if_false_stack[if_stack_top]) {
			icg_emit_if_false(&g_icg, $3.place ? $3.place : "0", if_false_stack[if_stack_top]);
		}
		free($3.place);
	}
	THEN statements if_else_part
;

loop_stmt:
	TIME_TURNER
	{
		char *loop_start = icg_new_label(&g_icg);
		char *loop_end = icg_new_label(&g_icg);
		if (current_house != 4) {
			report_semantic_errorf("TIME_TURNER loop is only allowed in Ravenclaw (current: %s)", house_name_from_id(current_house));
		}
		if (loop_start && loop_end && loop_stack_top < 63) {
			loop_stack_top++;
			loop_start_stack[loop_stack_top] = loop_start;
			loop_end_stack[loop_stack_top] = loop_end;
			icg_emit_label(&g_icg, loop_start);
		}
	}
	expr BEGIN_LOOP
	{
		if ($3.type != ARCANE_TYPE_BOOL || !$3.valid) {
			report_semantic_errorf("Condition must be boolean");
		}
		if (loop_stack_top >= 0) {
			icg_emit_if_false(&g_icg, $3.place ? $3.place : "0", loop_end_stack[loop_stack_top]);
		}
		free($3.place);
	}
	statements END_TIME_TURNER
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
	BREAK_SPELL ';'
	{
		if (loop_stack_top >= 0) {
			icg_emit_goto(&g_icg, loop_end_stack[loop_stack_top]);
		} else {
			report_semantic_errorf("'BREAK_SPELL' used outside loop");
		}
	}
;

continue_stmt:
	KEEP_CASTING ';'
	{
		if (loop_stack_top >= 0) {
			icg_emit_goto(&g_icg, loop_start_stack[loop_stack_top]);
		} else {
			report_semantic_errorf("'KEEP_CASTING' used outside loop");
		}
	}
;

return_stmt:
	RETURN expression_opt ';'
	{
		ArcaneType expected_type = ARCANE_TYPE_VOID;
		if (current_function_name[0] == '\0') {
			report_semantic_errorf("'RETURN' used outside function");
		} else {
			(void)arcane_symbol_get_function_signature(current_function_name, &expected_type, NULL, NULL, 0);

			if (expected_type == ARCANE_TYPE_VOID) {
				if ($2.type != ARCANE_TYPE_VOID) {
					report_semantic_errorf("Void function '%s' cannot return a value", current_function_name);
				}
			} else if ($2.type == ARCANE_TYPE_VOID) {
				report_semantic_errorf("Function '%s' must return a value of type %s", current_function_name, arcane_type_name(expected_type));
			} else if (!arcane_assignment_compatible(expected_type, $2.type)) {
				report_semantic_errorf("Return type mismatch in '%s': expected %s but got %s", current_function_name, arcane_type_name(expected_type), arcane_type_name($2.type));
			}
			if ($2.type == ARCANE_TYPE_VOID || !$2.valid) {
				icg_emit_return(&g_icg, NULL);
			} else {
				icg_emit_return(&g_icg, $2.place);
			}
		}
		free($2.place);
	}
;

expression_opt:
	  expression
	| {
		$$.place = NULL;
		$$.type = ARCANE_TYPE_VOID;
		$$.valid = 1;
	}
;

arg_list_opt:
	  arg_list { $$ = $1; }
	| { $$ = arcane_args_empty(); }
;

arg_list:
	  expression {
		ArcaneArgSem args = arcane_args_empty();
		args = arcane_args_append(args, $1.type, $1.place, $1.valid);
		$$ = args;
	}
	| arg_list ',' expression {
		ArcaneArgSem args = $1;
		args = arcane_args_append(args, $3.type, $3.place, $3.valid);
		$$ = args;
	}
;

param_list_opt:
	  param_list { $$ = $1; }
	| { $$ = arcane_params_empty(); }
;

param_list:
	  param_decl { $$ = $1; }
	| param_list ',' param_decl {
		ArcaneParamSem merged = $1;
		int i;
		for (i = 0; i < $3.count; ++i) {
			merged = arcane_params_append(merged, $3.names[i], $3.types[i]);
			$3.names[i] = NULL;
		}
		arcane_params_release(&$3);
		$$ = merged;
	}
;

param_decl:
	  IDENTIFIER {
		ArcaneParamSem p = arcane_params_empty();
		p = arcane_params_append(p, $1, ARCANE_TYPE_UNKNOWN);
		$$ = p;
	}
	| IDENTIFIER AS type_spec {
		ArcaneParamSem p = arcane_params_empty();
		p = arcane_params_append(p, $1, (ArcaneType)$3);
		$$ = p;
	}
;

expr:
	  expr '+' expr { $$ = arcane_expr_binary_numeric("+", $1, $3); }
	| expr '-' expr { $$ = arcane_expr_binary_numeric("-", $1, $3); }
	| expr '*' expr { $$ = arcane_expr_binary_numeric("*", $1, $3); }
	| expr '/' expr {
		if ($3.valid && $3.place && strcmp($3.place, "0") == 0) {
			report_semantic_errorf("Possible division by zero");
		}
		$$ = arcane_expr_binary_numeric("/", $1, $3);
	}
	| expr '>' expr { $$ = arcane_expr_binary_compare(">", $1, $3); }
	| expr '<' expr { $$ = arcane_expr_binary_compare("<", $1, $3); }
	| expr GE expr { $$ = arcane_expr_binary_compare(">=", $1, $3); }
	| expr LE expr { $$ = arcane_expr_binary_compare("<=", $1, $3); }
	| expr EQ expr { $$ = arcane_expr_binary_compare("==", $1, $3); }
	| expr NE expr { $$ = arcane_expr_binary_compare("!=", $1, $3); }
	| IDENTIFIER
	  {
		sync_symbol_context();
		$$ = arcane_expr_identifier($1);
		free($1);
	  }
	| NUMBER { $$ = arcane_expr_make(icg_int_literal($1), ARCANE_TYPE_INT, 1); }
;

expression:
	  expr { $$ = $1; }
	| STRING
	  {
		$$ = arcane_expr_make(wrap_literal($1, '"'), ARCANE_TYPE_STRING, 1);
		free($1);
	  }
	| CHAR_LITERAL
	  {
		$$ = arcane_expr_make(wrap_literal($1, '\''), ARCANE_TYPE_CHAR, 1);
		free($1);
	  }
	| BOOL_LITERAL { $$ = arcane_expr_make($1, ARCANE_TYPE_BOOL, 1); }
	| '(' expression ')' { $$ = $2; }
	| '-' expression %prec UMINUS
	  {
		ArcaneExprSem minus_one;
		if (!arcane_type_is_numeric($2.type) || !$2.valid) {
			report_semantic_errorf("Invalid unary '-' operand type: %s", arcane_type_name($2.type));
			free($2.place);
			$$ = arcane_expr_invalid();
		} else {
			minus_one = arcane_expr_make(icg_int_literal(-1), ARCANE_TYPE_INT, 1);
			$$ = arcane_expr_binary_numeric("*", minus_one, $2);
		}
	  }
	| NOT expression
	  {
		char *temp = NULL;
		if ($2.type != ARCANE_TYPE_BOOL || !$2.valid) {
			report_semantic_errorf("Invalid NOT operand type: %s", arcane_type_name($2.type));
			free($2.place);
			$$ = arcane_expr_invalid();
		} else {
			temp = icg_new_temp(&g_icg);
			if (temp && $2.place) {
				icg_emit(&g_icg, "%s = NOT %s\n", temp, $2.place);
			}
			free($2.place);
			$$ = arcane_expr_make(temp, ARCANE_TYPE_BOOL, temp != NULL);
		}
	  }
	| expression '%' expression {
		if (!$1.valid || !$3.valid || !arcane_type_is_integral($1.type) || !arcane_type_is_integral($3.type)) {
			report_semantic_errorf("Invalid operand types for '%%': %s and %s", arcane_type_name($1.type), arcane_type_name($3.type));
			free($1.place);
			free($3.place);
			$$ = arcane_expr_invalid();
		} else {
			$$ = arcane_expr_make(emit_binary_temp("%", $1.place, $3.place), ARCANE_TYPE_INT, 1);
		}
	}
	| expression AND expression { $$ = arcane_expr_binary_logic("AND", $1, $3); }
	| expression OR expression { $$ = arcane_expr_binary_logic("OR", $1, $3); }
	| expression XOR expression { $$ = arcane_expr_binary_logic("XOR", $1, $3); }
	| POWO '(' expression ')' {
		report_semantic_errorf("POWO expects 2 arguments");
		free($3.place);
		$$ = arcane_expr_invalid();
	}
	| POWO '(' expression ',' expression ')' {
		$$ = arcane_expr_math_call("POWO", $3, $5, 2);
	}
	| RADIX '(' expression ',' expression ')' {
		report_semantic_errorf("RADIX expects 1 argument");
		free($3.place);
		free($5.place);
		$$ = arcane_expr_invalid();
	}
	| RADIX '(' expression ')' {
		$$ = arcane_expr_math_call("RADIX", $3, arcane_expr_invalid(), 1);
	}
	| FLOORUS '(' expression ',' expression ')' {
		report_semantic_errorf("FLOORUS expects 1 argument");
		free($3.place);
		free($5.place);
		$$ = arcane_expr_invalid();
	}
	| FLOORUS '(' expression ')' {
		$$ = arcane_expr_math_call("FLOORUS", $3, arcane_expr_invalid(), 1);
	}
	| CEILUS '(' expression ',' expression ')' {
		report_semantic_errorf("CEILUS expects 1 argument");
		free($3.place);
		free($5.place);
		$$ = arcane_expr_invalid();
	}
	| CEILUS '(' expression ')' {
		$$ = arcane_expr_math_call("CEILUS", $3, arcane_expr_invalid(), 1);
	}
	| ABSOLUTUS '(' expression ',' expression ')' {
		report_semantic_errorf("ABSOLUTUS expects 1 argument");
		free($3.place);
		free($5.place);
		$$ = arcane_expr_invalid();
	}
	| ABSOLUTUS '(' expression ')' {
		$$ = arcane_expr_math_call("ABSOLUTUS", $3, arcane_expr_invalid(), 1);
	}
	| LOGUS '(' expression ',' expression ')' {
		report_semantic_errorf("LOGUS expects 1 argument");
		free($3.place);
		free($5.place);
		$$ = arcane_expr_invalid();
	}
	| LOGUS '(' expression ')' {
		$$ = arcane_expr_math_call("LOGUS", $3, arcane_expr_invalid(), 1);
	}
	| SINUS '(' expression ',' expression ')' {
		report_semantic_errorf("SINUS expects 1 argument");
		free($3.place);
		free($5.place);
		$$ = arcane_expr_invalid();
	}
	| SINUS '(' expression ')' {
		$$ = arcane_expr_math_call("SINUS", $3, arcane_expr_invalid(), 1);
	}
	| COSINUS '(' expression ',' expression ')' {
		report_semantic_errorf("COSINUS expects 1 argument");
		free($3.place);
		free($5.place);
		$$ = arcane_expr_invalid();
	}
	| COSINUS '(' expression ')' {
		$$ = arcane_expr_math_call("COSINUS", $3, arcane_expr_invalid(), 1);
	}
	| TANUS '(' expression ',' expression ')' {
		report_semantic_errorf("TANUS expects 1 argument");
		free($3.place);
		free($5.place);
		$$ = arcane_expr_invalid();
	}
	| TANUS '(' expression ')' {
		$$ = arcane_expr_math_call("TANUS", $3, arcane_expr_invalid(), 1);
	}
	| IDENTIFIER '(' arg_list_opt ')'
	  {
		int i;
		char *temp;
		ArcaneType return_type;
		sync_symbol_context();
		return_type = arcane_validate_function_call($1, $3, 1);
		temp = NULL;
		for (i = 0; i < $3.count; ++i) {
			icg_emit_arg(&g_icg, $3.places[i] ? $3.places[i] : "0");
		}
		if (return_type != ARCANE_TYPE_VOID) {
			temp = icg_new_temp(&g_icg);
			if (temp) {
				icg_emit_call_assign(&g_icg, temp, $1, $3.count);
			}
			$$ = arcane_expr_make(temp, return_type, temp != NULL);
		} else {
			icg_emit_call(&g_icg, $1, $3.count);
			$$ = arcane_expr_invalid();
		}
		arcane_args_release(&$3);
		free($1);
	  }
;

%%

void yyerror(const char *s) {
	if (s && strcmp(s, "syntax error") == 0) {
		fprintf(stderr, "Parse error: syntax error (possible misplaced OTHERWISE/ELSE_CHARM or missing END_CHARM)\n");
		return;
	}
	fprintf(stderr, "Parse error: %s\n", s ? s : "unknown parser error");
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
	if (strcmp(buffer, WZ_KW_IF_CHARM) == 0) return IF_CHARM;
	if (strcmp(buffer, WZ_KW_OTHERWISE) == 0) return OTHERWISE;
	if (strcmp(buffer, WZ_KW_ELSE_CHARM) == 0) return ELSE_CHARM;
	if (strcmp(buffer, WZ_KW_THEN) == 0) return THEN;
	if (strcmp(buffer, WZ_KW_END_CHARM) == 0) return END_CHARM;
	if (strcmp(buffer, WZ_KW_TIME_TURNER) == 0) return TIME_TURNER;
	if (strcmp(buffer, WZ_KW_BEGIN_LOOP) == 0) return BEGIN_LOOP;
	if (strcmp(buffer, WZ_KW_END_TIME_TURNER) == 0) return END_TIME_TURNER;
	if (strcmp(buffer, WZ_KW_BREAK_SPELL) == 0) return BREAK_SPELL;
	if (strcmp(buffer, WZ_KW_KEEP_CASTING) == 0) return KEEP_CASTING;
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
	if (strcmp(buffer, WZ_KW_POWO) == 0) return POWO;
	if (strcmp(buffer, WZ_KW_RADIX) == 0) return RADIX;
	if (strcmp(buffer, WZ_KW_FLOORUS) == 0) return FLOORUS;
	if (strcmp(buffer, WZ_KW_CEILUS) == 0) return CEILUS;
	if (strcmp(buffer, WZ_KW_ABSOLUTUS) == 0) return ABSOLUTUS;
	if (strcmp(buffer, WZ_KW_LOGUS) == 0) return LOGUS;
	if (strcmp(buffer, WZ_KW_SINUS) == 0) return SINUS;
	if (strcmp(buffer, WZ_KW_COSINUS) == 0) return COSINUS;
	if (strcmp(buffer, WZ_KW_TANUS) == 0) return TANUS;
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
			if (escaped == '\n') {
				g_manual_line++;
			}
			if (length < (int)sizeof(buffer) - 2) {
				buffer[length++] = '\\';
				buffer[length++] = (char)escaped;
			}
			continue;
		}
		if (ch == '\n') {
			g_manual_line++;
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
		if (escaped == '\n') {
			g_manual_line++;
		}
		buffer[length++] = '\\';
		buffer[length++] = (char)escaped;
	} else if (ch != EOF) {
		if (ch == '\n') {
			g_manual_line++;
		}
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
			if (ch == '\n') {
				g_manual_line++;
			}
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
					if (ch == '\n') {
						g_manual_line++;
					}
					continue;
				}
				if (next == '*') {
					int prev = 0;
					while ((ch = fgetc(yyin)) != EOF) {
						if (ch == '\n') {
							g_manual_line++;
						}
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
#ifndef USE_FLEX_LEXER
	g_manual_line = 1;
#endif
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
