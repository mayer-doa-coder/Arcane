#include "codegen.h"

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char items[ARCANE_MAX_FUNCTION_PARAMS][128];
    int count;
} ArcanePendingArgs;

static int find_matching_function_end(const ArcaneICG *icg, int begin_index, const char *function_name);

static const char *to_c_type(ArcaneType type) {
    switch (type) {
        case ARCANE_TYPE_FLOAT: return "float";
        case ARCANE_TYPE_DOUBLE: return "double";
        case ARCANE_TYPE_LONG: return "long";
        case ARCANE_TYPE_CHAR: return "char";
        case ARCANE_TYPE_BOOL: return "int";
        case ARCANE_TYPE_VOID: return "void";
        case ARCANE_TYPE_STRING: return "const char *";
        case ARCANE_TYPE_INT:
        case ARCANE_TYPE_UNKNOWN:
        default:
            return "int";
    }
}

static int starts_with(const char *text, const char *prefix) {
    size_t prefix_len;

    if (!text || !prefix) {
        return 0;
    }

    prefix_len = strlen(prefix);
    return strncmp(text, prefix, prefix_len) == 0;
}

static size_t line_content_length(const char *line) {
    if (!line) {
        return 0;
    }
    return strcspn(line, "\r\n");
}

static const char *map_math_function_name(const char *name) {
    if (!name) {
        return NULL;
    }
    if (strcmp(name, "POWO") == 0) return "pow";
    if (strcmp(name, "RADIX") == 0) return "sqrt";
    if (strcmp(name, "FLOORUS") == 0) return "floor";
    if (strcmp(name, "CEILUS") == 0) return "ceil";
    if (strcmp(name, "ABSOLUTUS") == 0) return "abs";
    if (strcmp(name, "LOGUS") == 0) return "log";
    if (strcmp(name, "SINUS") == 0) return "sin";
    if (strcmp(name, "COSINUS") == 0) return "cos";
    if (strcmp(name, "TANUS") == 0) return "tan";
    return NULL;
}

static void rewrite_math_calls(const char *input, char *output, size_t output_size) {
    size_t in_index = 0;
    size_t out_index = 0;

    if (!output || output_size == 0) {
        return;
    }
    output[0] = '\0';
    if (!input) {
        return;
    }

    while (input[in_index] != '\0' && out_index + 1 < output_size) {
        if ((isalpha((unsigned char)input[in_index]) || input[in_index] == '_')) {
            char ident[64];
            size_t ident_len = 0;
            size_t start = in_index;
            const char *mapped;

            while ((isalpha((unsigned char)input[in_index]) || isdigit((unsigned char)input[in_index]) || input[in_index] == '_') && ident_len + 1 < sizeof(ident)) {
                ident[ident_len++] = input[in_index++];
            }
            ident[ident_len] = '\0';

            if (ident_len > 0 && input[in_index] == '(') {
                mapped = map_math_function_name(ident);
                if (mapped) {
                    size_t mapped_len = strlen(mapped);
                    if (out_index + mapped_len + 1 >= output_size) {
                        break;
                    }
                    memcpy(output + out_index, mapped, mapped_len);
                    out_index += mapped_len;
                    continue;
                }
            }

            while (start < in_index && out_index + 1 < output_size) {
                output[out_index++] = input[start++];
            }
            continue;
        }

        output[out_index++] = input[in_index++];
    }

    output[out_index] = '\0';
}

static int is_temp_symbol(const char *name) {
    size_t index;

    if (!name || name[0] != 't' || !isdigit((unsigned char)name[1])) {
        return 0;
    }

    for (index = 2; name[index] != '\0'; ++index) {
        if (!isdigit((unsigned char)name[index])) {
            return 0;
        }
    }

    return 1;
}

static int should_emit_assignment(const char *line) {
    if (!line) {
        return 0;
    }

    if (strstr(line, "ifFalse") || strstr(line, "goto ") || strstr(line, " call ") || starts_with(line, "call ")) {
        return 0;
    }

    return strchr(line, '=') != NULL;
}

static int line_is_call_assignment(const char *line) {
    if (!line) {
        return 0;
    }
    return strstr(line, " = call ") != NULL;
}

static int extract_lhs_name(const char *line, char *out, size_t out_size) {
    const char *equal_pos;
    const char *start;
    size_t lhs_len;

    if (!line || !out || out_size == 0) {
        return 0;
    }

    equal_pos = strchr(line, '=');
    if (!equal_pos) {
        return 0;
    }

    start = line;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    lhs_len = (size_t)(equal_pos - start);
    while (lhs_len > 0 && isspace((unsigned char)start[lhs_len - 1])) {
        lhs_len--;
    }

    if (lhs_len == 0 || lhs_len >= out_size) {
        return 0;
    }

    memcpy(out, start, lhs_len);
    out[lhs_len] = '\0';
    return 1;
}

static int line_is_function_marker(const char *line) {
    return starts_with(line, "func_begin ") || starts_with(line, "func_end ") || starts_with(line, "param ");
}

static int line_is_main_ir(const char *line) {
    
    return line && !line_is_function_marker(line);
}

static int parse_function_marker(const char *line, const char *prefix, char *name_out, size_t name_out_size) {
    if (!line || !prefix || !name_out || name_out_size == 0) {
        return 0;
    }

    if (!starts_with(line, prefix)) {
        return 0;
    }

    return sscanf(line + strlen(prefix), "%63s", name_out) == 1;
}

static int line_has_float_math_rhs(const char *line) {
    if (!line) {
        return 0;
    }

    return strstr(line, "POWO(") != NULL ||
        strstr(line, "RADIX(") != NULL ||
        strstr(line, "FLOORUS(") != NULL ||
        strstr(line, "CEILUS(") != NULL ||
        strstr(line, "ABSOLUTUS(") != NULL ||
        strstr(line, "LOGUS(") != NULL ||
        strstr(line, "SINUS(") != NULL ||
        strstr(line, "COSINUS(") != NULL ||
        strstr(line, "TANUS(") != NULL;
}

static int find_function_symbol(const ArcaneSymbolTable *symbols, const char *function_name) {
    int i;

    if (!symbols || !function_name) {
        return -1;
    }

    for (i = 0; i < symbols->count; ++i) {
        const ArcaneSymbol *symbol = &symbols->entries[i];
        if (symbol->kind == ARCANE_SYMBOL_FUNCTION && strcmp(symbol->name, function_name) == 0) {
            return i;
        }
    }

    return -1;
}

static int find_matching_function_end(const ArcaneICG *icg, int begin_index, const char *function_name) {
    int i;
    int nested_depth = 0;

    if (!icg || !function_name || begin_index < 0 || begin_index >= icg->line_count) {
        return -1;
    }

    for (i = begin_index + 1; i < icg->line_count; ++i) {
        char nested_name[64];
        char end_name[64];

        if (parse_function_marker(icg->lines[i], "func_begin ", nested_name, sizeof(nested_name))) {
            nested_depth++;
            continue;
        }

        if (parse_function_marker(icg->lines[i], "func_end ", end_name, sizeof(end_name))) {
            if (nested_depth == 0 && strcmp(end_name, function_name) == 0) {
                return i;
            }
            if (nested_depth > 0) {
                nested_depth--;
            }
        }
    }

    return -1;
}

static int collect_function_param_names(const ArcaneSymbolTable *symbols, const char *function_name, char names[][ARCANE_MAX_NAME_LEN], int max_names) {
    int i;
    int count = 0;

    if (!symbols || !function_name || !names || max_names <= 0) {
        return 0;
    }

    for (i = 0; i < symbols->count && count < max_names; ++i) {
        const ArcaneSymbol *symbol = &symbols->entries[i];
        if (symbol->kind != ARCANE_SYMBOL_PARAMETER) {
            continue;
        }
        if (strcmp(symbol->owner_function, function_name) != 0) {
            continue;
        }
        strncpy(names[count], symbol->name, ARCANE_MAX_NAME_LEN - 1);
        names[count][ARCANE_MAX_NAME_LEN - 1] = '\0';
        count++;
    }

    return count;
}

static void emit_function_signature(FILE *fp, const ArcaneSymbol *function_symbol, const ArcaneSymbolTable *symbols, int with_semicolon) {
    int i;
    int param_name_count;
    char param_names[ARCANE_MAX_FUNCTION_PARAMS][ARCANE_MAX_NAME_LEN];

    if (!fp || !function_symbol || !symbols) {
        return;
    }

    fprintf(fp, "%s %s(", to_c_type(function_symbol->type), function_symbol->name);

    param_name_count = collect_function_param_names(symbols, function_symbol->name, param_names, ARCANE_MAX_FUNCTION_PARAMS);
    if (function_symbol->param_count == 0) {
        fprintf(fp, "void");
    } else {
        for (i = 0; i < function_symbol->param_count; ++i) {
            if (i > 0) {
                fprintf(fp, ", ");
            }
            fprintf(
                fp,
                "%s %s",
                to_c_type(function_symbol->param_types[i]),
                (i < param_name_count) ? param_names[i] : "arg"
            );
            if (i >= param_name_count) {
                fprintf(fp, "%d", i + 1);
            }
        }
    }

    if (with_semicolon) {
        fprintf(fp, ");\n");
    } else {
        fprintf(fp, ")");
    }
}

static void emit_temp_declarations_for_range(FILE *fp, const ArcaneSymbolTable *symbols, const ArcaneICG *icg, int start_index, int end_index) {
    int i;
    char declared[ARCANE_MAX_SYMBOLS][64];
    int declared_count = 0;

    (void)symbols;

    if (!fp || !symbols || !icg) {
        return;
    }

    for (i = start_index; i <= end_index && i < icg->line_count; ++i) {
        char lhs_name[64];
        const char *line = icg->lines[i];
        char nested_name[64];

        if (parse_function_marker(line, "func_begin ", nested_name, sizeof(nested_name))) {
            int nested_end = find_matching_function_end(icg, i, nested_name);
            if (nested_end > i) {
                i = nested_end;
                continue;
            }
        }

        if (!line || (!should_emit_assignment(line) && !line_is_call_assignment(line))) {
            continue;
        }

        if (!extract_lhs_name(line, lhs_name, sizeof(lhs_name))) {
            continue;
        }

        if (is_temp_symbol(lhs_name)) {
            int seen = 0;
            int j;
            const char *temp_type = line_has_float_math_rhs(line) ? "double" : "int";
            for (j = 0; j < declared_count; ++j) {
                if (strcmp(declared[j], lhs_name) == 0) {
                    seen = 1;
                    break;
                }
            }
            if (seen) {
                continue;
            }
            fprintf(fp, "    %s %s;\n", temp_type, lhs_name);
            if (declared_count < ARCANE_MAX_SYMBOLS) {
                strncpy(declared[declared_count], lhs_name, sizeof(declared[declared_count]) - 1);
                declared[declared_count][sizeof(declared[declared_count]) - 1] = '\0';
                declared_count++;
            }
        }
    }
}

static void emit_function_local_declarations(FILE *fp, const ArcaneSymbolTable *symbols, const char *function_name) {
    int i;

    if (!fp || !symbols || !function_name) {
        return;
    }

    for (i = 0; i < symbols->count; ++i) {
        const ArcaneSymbol *symbol = &symbols->entries[i];
        if (symbol->kind != ARCANE_SYMBOL_VARIABLE) {
            continue;
        }
        if (strcmp(symbol->owner_function, function_name) != 0) {
            continue;
        }
        fprintf(fp, "    %s %s;\n", to_c_type(symbol->type), symbol->name);
    }
}

static void pending_args_reset(ArcanePendingArgs *pending) {
    if (!pending) {
        return;
    }
    pending->count = 0;
}

static void pending_args_push(ArcanePendingArgs *pending, const char *arg_expr) {
    if (!pending || !arg_expr) {
        return;
    }
    if (pending->count >= ARCANE_MAX_FUNCTION_PARAMS) {
        return;
    }

    strncpy(pending->items[pending->count], arg_expr, sizeof(pending->items[pending->count]) - 1);
    pending->items[pending->count][sizeof(pending->items[pending->count]) - 1] = '\0';
    pending->count++;
}

static void emit_call_from_pending(FILE *fp, ArcanePendingArgs *pending, const char *target, const char *function_name, int arg_count) {
    int i;

    if (!fp || !pending || !function_name) {
        return;
    }

    if (target && target[0] != '\0') {
        fprintf(fp, "    %s = %s(", target, function_name);
    } else {
        fprintf(fp, "    %s(", function_name);
    }

    for (i = 0; i < arg_count; ++i) {
        const char *arg_text = (i < pending->count) ? pending->items[i] : "0";
        if (i > 0) {
            fprintf(fp, ", ");
        }
        fprintf(fp, "%s", arg_text);
    }

    fprintf(fp, ");\n");
    pending_args_reset(pending);
}

static void emit_ir_line_as_c(FILE *fp, const char *line, ArcanePendingArgs *pending) {
    size_t len;

    if (!fp || !line || !pending) {
        return;
    }

    len = line_content_length(line);
    if (len == 0) {
        return;
    }

    if (line_is_function_marker(line)) {
        return;
    }

    if (line[0] == '#') {
        fprintf(fp, "    /* %.*s */\n", (int)len, line);
        return;
    }

    if (starts_with(line, "arg ")) {
        char arg_expr[128];
        if (sscanf(line + 4, "%127[^\r\n]", arg_expr) == 1) {
            pending_args_push(pending, arg_expr);
        }
        return;
    }

    if (strstr(line, " = call ")) {
        char lhs[64];
        char fn[64];
        int argc = 0;
        if (sscanf(line, " %63[^=]= call %63s %d", lhs, fn, &argc) == 3) {
            size_t lhs_len = strlen(lhs);
            while (lhs_len > 0 && isspace((unsigned char)lhs[lhs_len - 1])) {
                lhs[--lhs_len] = '\0';
            }
            emit_call_from_pending(fp, pending, lhs, fn, argc);
        }
        return;
    }

    if (starts_with(line, "call ")) {
        char fn[64];
        int argc = 0;
        if (sscanf(line, "call %63s %d", fn, &argc) == 2) {
            emit_call_from_pending(fp, pending, NULL, fn, argc);
        }
        return;
    }

    if (starts_with(line, "return")) {
        char ret_expr[128];
        if (sscanf(line, "return %127[^\r\n]", ret_expr) == 1) {
            char mapped_expr[256];
            rewrite_math_calls(ret_expr, mapped_expr, sizeof(mapped_expr));
            fprintf(fp, "    return %s;\n", mapped_expr);
        } else {
            fprintf(fp, "    return;\n");
        }
        return;
    }

    if (starts_with(line, "ifFalse ")) {
        char condition[128];
        char label[64];
        if (sscanf(line, "ifFalse %127s goto %63s", condition, label) == 2) {
            fprintf(fp, "    if (!(%s)) goto %s;\n", condition, label);
        }
        return;
    }

    if (starts_with(line, "goto ")) {
        char label[64];
        if (sscanf(line, "goto %63s", label) == 1) {
            fprintf(fp, "    goto %s;\n", label);
        }
        return;
    }

    if (starts_with(line, "print ")) {
        const char *operand = line + 6;
        size_t operand_len = strcspn(operand, "\r\n");
        if (operand_len > 0) {
            if (operand[0] == '"') {
                fprintf(fp, "    printf(\"%%s\\n\", %.*s);\n", (int)operand_len, operand);
            } else {
                /* Cast to double so integer and floating expressions are both printed safely. */
                fprintf(fp, "    printf(\"%%g\\n\", (double)(%.*s));\n", (int)operand_len, operand);
            }
        }
        return;
    }

    if (line[len - 1] == ':') {
        fprintf(fp, "    %.*s ;\n", (int)len, line);
        return;
    }

    if (should_emit_assignment(line)) {
        char lhs_name[64];
        const char *equal_pos;
        const char *rhs_start;
        char rhs_text[256];
        char mapped_rhs[256];
        size_t rhs_len;

        if (!extract_lhs_name(line, lhs_name, sizeof(lhs_name))) {
            fprintf(fp, "    %.*s;\n", (int)len, line);
            return;
        }

        equal_pos = strchr(line, '=');
        if (!equal_pos) {
            fprintf(fp, "    %.*s;\n", (int)len, line);
            return;
        }

        rhs_start = equal_pos + 1;
        while (*rhs_start && isspace((unsigned char)*rhs_start)) {
            rhs_start++;
        }
        rhs_len = strcspn(rhs_start, "\r\n");
        if (rhs_len >= sizeof(rhs_text)) {
            rhs_len = sizeof(rhs_text) - 1;
        }
        memcpy(rhs_text, rhs_start, rhs_len);
        rhs_text[rhs_len] = '\0';

        rewrite_math_calls(rhs_text, mapped_rhs, sizeof(mapped_rhs));
        fprintf(fp, "    %s=%s;\n", lhs_name, mapped_rhs);
    }
}

static void emit_function_definitions(FILE *fp, const ArcaneSymbolTable *symbols, const ArcaneICG *icg) {
    int i;

    for (i = 0; i < icg->line_count; ++i) {
        char function_name[64];
        int function_symbol_index;
        int end_index;
        int has_explicit_return = 0;
        ArcanePendingArgs pending;

        if (!parse_function_marker(icg->lines[i], "func_begin ", function_name, sizeof(function_name))) {
            continue;
        }

        function_symbol_index = find_function_symbol(symbols, function_name);
        if (function_symbol_index < 0) {
            continue;
        }

        end_index = find_matching_function_end(icg, i, function_name);
        if (end_index < 0) {
            continue;
        }

        emit_function_signature(fp, &symbols->entries[function_symbol_index], symbols, 0);
        fprintf(fp, " {\n");

        emit_function_local_declarations(fp, symbols, function_name);
        emit_temp_declarations_for_range(fp, symbols, icg, i + 1, end_index - 1);

        pending_args_reset(&pending);
        fprintf(fp, "\n");

        for (int k = i + 1; k < end_index; ++k) {
            char nested_name[64];
            if (starts_with(icg->lines[k], "return")) {
                has_explicit_return = 1;
            }

            if (parse_function_marker(icg->lines[k], "func_begin ", nested_name, sizeof(nested_name))) {
                int nested_end = find_matching_function_end(icg, k, nested_name);
                if (nested_end > k) {
                    k = nested_end;
                    continue;
                }
            }

            emit_ir_line_as_c(fp, icg->lines[k], &pending);
        }

        if (!has_explicit_return && (symbols->entries[function_symbol_index].type == ARCANE_TYPE_INT ||
            symbols->entries[function_symbol_index].type == ARCANE_TYPE_LONG ||
            symbols->entries[function_symbol_index].type == ARCANE_TYPE_FLOAT ||
            symbols->entries[function_symbol_index].type == ARCANE_TYPE_DOUBLE ||
            symbols->entries[function_symbol_index].type == ARCANE_TYPE_CHAR ||
            symbols->entries[function_symbol_index].type == ARCANE_TYPE_BOOL ||
            symbols->entries[function_symbol_index].type == ARCANE_TYPE_UNKNOWN)) {
            fprintf(fp, "    return 0;\n");
        }

        fprintf(fp, "}\n\n");
    }
}

static void emit_main_variable_declarations(FILE *fp, const ArcaneSymbolTable *symbols) {
    int i;

    for (i = 0; i < symbols->count; ++i) {
        const ArcaneSymbol *symbol = &symbols->entries[i];
        if (symbol->kind != ARCANE_SYMBOL_VARIABLE) {
            continue;
        }
        if (symbol->owner_function[0] != '\0') {
            continue;
        }
        fprintf(fp, "    %s %s;\n", to_c_type(symbol->type), symbol->name);
    }
}

static void emit_main_temps(FILE *fp, const ArcaneSymbolTable *symbols, const ArcaneICG *icg) {
    int i = 0;
    char declared[ARCANE_MAX_SYMBOLS][64];
    int declared_count = 0;

    while (i < icg->line_count) {
        char lhs_name[64];
        const char *line = icg->lines[i];
        char function_name[64];

        if (parse_function_marker(line, "func_begin ", function_name, sizeof(function_name))) {
            i++;
            while (i < icg->line_count) {
                char end_name[64];
                if (parse_function_marker(icg->lines[i], "func_end ", end_name, sizeof(end_name)) && strcmp(function_name, end_name) == 0) {
                    break;
                }
                i++;
            }
            i++;
            continue;
        }

        if (!line_is_main_ir(line) || (!should_emit_assignment(line) && !line_is_call_assignment(line))) {
            i++;
            continue;
        }

        if (!extract_lhs_name(line, lhs_name, sizeof(lhs_name))) {
            i++;
            continue;
        }
        if (is_temp_symbol(lhs_name)) {
            int seen = 0;
            int j;
            const char *temp_type = line_has_float_math_rhs(line) ? "double" : "int";
            for (j = 0; j < declared_count; ++j) {
                if (strcmp(declared[j], lhs_name) == 0) {
                    seen = 1;
                    break;
                }
            }
            if (!seen) {
                fprintf(fp, "    %s %s;\n", temp_type, lhs_name);
                if (declared_count < ARCANE_MAX_SYMBOLS) {
                    strncpy(declared[declared_count], lhs_name, sizeof(declared[declared_count]) - 1);
                    declared[declared_count][sizeof(declared[declared_count]) - 1] = '\0';
                    declared_count++;
                }
            }
        }
        i++;
    }
}

static void emit_main_body(FILE *fp, const ArcaneICG *icg) {
    int i = 0;
    ArcanePendingArgs pending;

    pending_args_reset(&pending);

    while (i < icg->line_count) {
        char function_name[64];
        if (parse_function_marker(icg->lines[i], "func_begin ", function_name, sizeof(function_name))) {
            i++;
            while (i < icg->line_count) {
                char end_name[64];
                if (parse_function_marker(icg->lines[i], "func_end ", end_name, sizeof(end_name)) && strcmp(function_name, end_name) == 0) {
                    break;
                }
                i++;
            }
            i++;
            continue;
        }

        emit_ir_line_as_c(fp, icg->lines[i], &pending);
        i++;
    }
}

int generate_c_code(const ArcaneSymbolTable *symbols, const ArcaneICG *icg, const char *output_path) {
    FILE *fp;
    int i;

    if (!symbols || !icg) {
        return -1;
    }

    fp = fopen(output_path ? output_path : "output.c", "w");
    if (!fp) {
        return -2;
    }

    fprintf(fp, "#include <stdio.h>\n");
    fprintf(fp, "#include <math.h>\n\n");

    for (i = 0; i < symbols->count; ++i) {
        const ArcaneSymbol *symbol = &symbols->entries[i];
        if (symbol->kind == ARCANE_SYMBOL_FUNCTION) {
            emit_function_signature(fp, symbol, symbols, 1);
        }
    }

    fprintf(fp, "\n");
    emit_function_definitions(fp, symbols, icg);

    fprintf(fp, "int main() {\n");
    emit_main_variable_declarations(fp, symbols);
    emit_main_temps(fp, symbols, icg);
    fprintf(fp, "\n");
    emit_main_body(fp, icg);
    fprintf(fp, "\n    return 0;\n");
    fprintf(fp, "}\n");

    fclose(fp);
    return 0;
}
