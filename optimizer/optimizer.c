#include "optimizer.h"

#include <ctype.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCANE_OPT_MAX_TOKEN 128
#define ARCANE_OPT_MAX_LINE 512

static size_t line_content_length(const char *line) {
    if (!line) {
        return 0;
    }
    return strcspn(line, "\r\n");
}

static void copy_trimmed_line(const char *line, char *out, size_t out_size) {
    const char *start;
    size_t len;

    if (!out || out_size == 0) {
        return;
    }

    out[0] = '\0';
    if (!line) {
        return;
    }

    start = line;
    while (*start && isspace((unsigned char)*start)) {
        start++;
    }

    len = line_content_length(start);
    while (len > 0 && isspace((unsigned char)start[len - 1])) {
        len--;
    }

    if (len >= out_size) {
        len = out_size - 1;
    }

    memcpy(out, start, len);
    out[len] = '\0';
}

static int is_identifier_char(char ch) {
    return isalnum((unsigned char)ch) || ch == '_';
}

static int is_temp_name(const char *name) {
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

static int is_identifier_token(const char *text) {
    size_t i;

    if (!text || !text[0]) {
        return 0;
    }

    if (!(isalpha((unsigned char)text[0]) || text[0] == '_')) {
        return 0;
    }

    for (i = 1; text[i] != '\0'; ++i) {
        if (!is_identifier_char(text[i])) {
            return 0;
        }
    }

    return 1;
}

static int parse_integer_token(const char *text, int *value_out) {
    char *end_ptr;
    long parsed;

    if (!text || !*text || !value_out) {
        return 0;
    }

    parsed = strtol(text, &end_ptr, 10);
    if (*end_ptr != '\0') {
        return 0;
    }

    if (parsed < INT_MIN || parsed > INT_MAX) {
        return 0;
    }

    *value_out = (int)parsed;
    return 1;
}

static int line_is_unsafe_for_optimization(const char *line) {
    char trimmed[ARCANE_OPT_MAX_LINE];
    size_t len;

    copy_trimmed_line(line, trimmed, sizeof(trimmed));
    len = strlen(trimmed);
    if (len == 0) {
        return 1;
    }

    if (trimmed[0] == '#') {
        return 1;
    }

    if (trimmed[len - 1] == ':') {
        return 1;
    }

    if (strncmp(trimmed, "goto ", 5) == 0 ||
        strncmp(trimmed, "ifFalse ", 8) == 0 ||
        strncmp(trimmed, "func_begin ", 11) == 0 ||
        strncmp(trimmed, "func_end ", 9) == 0 ||
        strncmp(trimmed, "param ", 6) == 0 ||
        strncmp(trimmed, "input ", 6) == 0 ||
        strncmp(trimmed, "arg ", 4) == 0 ||
        strncmp(trimmed, "call ", 5) == 0 ||
        strncmp(trimmed, "return", 6) == 0 ||
        strncmp(trimmed, "print ", 6) == 0 ||
        strstr(trimmed, "POWO(") != NULL ||
        strstr(trimmed, "RADIX(") != NULL ||
        strstr(trimmed, "FLOORUS(") != NULL ||
        strstr(trimmed, "CEILUS(") != NULL ||
        strstr(trimmed, "ABSOLUTUS(") != NULL ||
        strstr(trimmed, "LOGUS(") != NULL ||
        strstr(trimmed, "SINUS(") != NULL ||
        strstr(trimmed, "COSINUS(") != NULL ||
        strstr(trimmed, "TANUS(") != NULL) {
        return 1;
    }

    return 0;
}

static int parse_assignment_parts(const char *line, char *lhs, size_t lhs_size, char *rhs, size_t rhs_size) {
    char trimmed[ARCANE_OPT_MAX_LINE];
    char *equal_pos;
    char *lhs_end;
    char *rhs_start;
    size_t lhs_len;
    size_t rhs_len;

    if (!line || !lhs || !rhs || lhs_size == 0 || rhs_size == 0) {
        return 0;
    }

    copy_trimmed_line(line, trimmed, sizeof(trimmed));
    equal_pos = strchr(trimmed, '=');
    if (!equal_pos) {
        return 0;
    }

    *equal_pos = '\0';
    lhs_end = equal_pos - 1;
    while (lhs_end >= trimmed && isspace((unsigned char)*lhs_end)) {
        *lhs_end = '\0';
        lhs_end--;
    }

    rhs_start = equal_pos + 1;
    while (*rhs_start && isspace((unsigned char)*rhs_start)) {
        rhs_start++;
    }

    lhs_len = strlen(trimmed);
    rhs_len = strlen(rhs_start);
    if (lhs_len == 0 || rhs_len == 0 || lhs_len >= lhs_size || rhs_len >= rhs_size) {
        return 0;
    }

    memcpy(lhs, trimmed, lhs_len + 1);
    memcpy(rhs, rhs_start, rhs_len + 1);
    return 1;
}

static int parse_binary_rhs(const char *rhs, char *left, size_t left_size, char *op, size_t op_size, char *right, size_t right_size) {
    const char *ops[] = {"==", "!=", ">=", "<=", "+", "-", "*", "/", "%", ">", "<"};
    size_t op_index;

    if (!rhs || !left || !op || !right || left_size == 0 || op_size == 0 || right_size == 0) {
        return 0;
    }

    for (op_index = 0; op_index < sizeof(ops) / sizeof(ops[0]); ++op_index) {
        const char *found = strstr(rhs, ops[op_index]);
        size_t lhs_len;
        size_t rhs_len;
        const char *rhs_part;

        if (!found) {
            continue;
        }

        lhs_len = (size_t)(found - rhs);
        rhs_part = found + strlen(ops[op_index]);
        while (*rhs_part && isspace((unsigned char)*rhs_part)) {
            rhs_part++;
        }
        while (lhs_len > 0 && isspace((unsigned char)rhs[lhs_len - 1])) {
            lhs_len--;
        }
        rhs_len = strlen(rhs_part);

        if (lhs_len == 0 || rhs_len == 0 || lhs_len >= left_size || rhs_len >= right_size || strlen(ops[op_index]) >= op_size) {
            continue;
        }

        memcpy(left, rhs, lhs_len);
        left[lhs_len] = '\0';
        memcpy(op, ops[op_index], strlen(ops[op_index]) + 1);
        memcpy(right, rhs_part, rhs_len + 1);
        return 1;
    }

    return 0;
}

static int eval_binary_int(int left, int right, const char *op, int *result_out) {
    if (!op || !result_out) {
        return 0;
    }

    if (strcmp(op, "+") == 0) {
        *result_out = left + right;
        return 1;
    }
    if (strcmp(op, "-") == 0) {
        *result_out = left - right;
        return 1;
    }
    if (strcmp(op, "*") == 0) {
        *result_out = left * right;
        return 1;
    }
    if (strcmp(op, "/") == 0) {
        if (right == 0) {
            return 0;
        }
        *result_out = left / right;
        return 1;
    }
    if (strcmp(op, "%") == 0) {
        if (right == 0) {
            return 0;
        }
        *result_out = left % right;
        return 1;
    }
    if (strcmp(op, ">") == 0) {
        *result_out = left > right;
        return 1;
    }
    if (strcmp(op, "<") == 0) {
        *result_out = left < right;
        return 1;
    }
    if (strcmp(op, ">=") == 0) {
        *result_out = left >= right;
        return 1;
    }
    if (strcmp(op, "<=") == 0) {
        *result_out = left <= right;
        return 1;
    }
    if (strcmp(op, "==") == 0) {
        *result_out = left == right;
        return 1;
    }
    if (strcmp(op, "!=") == 0) {
        *result_out = left != right;
        return 1;
    }

    return 0;
}

static int replace_line_with_text(char **line_ptr, const char *new_line_without_newline) {
    int required;
    char *replacement;

    if (!line_ptr || !new_line_without_newline) {
        return 0;
    }

    required = snprintf(NULL, 0, "%s\n", new_line_without_newline);
    if (required <= 0) {
        return 0;
    }

    replacement = (char *)malloc((size_t)required + 1);
    if (!replacement) {
        return 0;
    }

    snprintf(replacement, (size_t)required + 1, "%s\n", new_line_without_newline);
    free(*line_ptr);
    *line_ptr = replacement;
    return 1;
}

static int replace_line_with_assignment(char **line_ptr, const char *lhs, const char *rhs) {
    char buffer[ARCANE_OPT_MAX_LINE];

    if (!lhs || !rhs) {
        return 0;
    }

    snprintf(buffer, sizeof(buffer), "%s=%s", lhs, rhs);
    return replace_line_with_text(line_ptr, buffer);
}

static int replace_line_with_int(char **line_ptr, const char *lhs, int value) {
    char buffer[ARCANE_OPT_MAX_LINE];

    snprintf(buffer, sizeof(buffer), "%s=%d", lhs, value);
    return replace_line_with_text(line_ptr, buffer);
}

static int line_uses_identifier(const char *line, const char *identifier) {
    size_t id_len;
    const char *cursor;

    if (!line || !identifier || !identifier[0]) {
        return 0;
    }

    id_len = strlen(identifier);
    cursor = line;

    while ((cursor = strstr(cursor, identifier)) != NULL) {
        char prev = (cursor == line) ? '\0' : cursor[-1];
        char next = cursor[id_len];

        if (!is_identifier_char(prev) && !is_identifier_char(next)) {
            return 1;
        }

        cursor += id_len;
    }

    return 0;
}

static int line_redefines_identifier(const char *line, const char *identifier) {
    char lhs[ARCANE_OPT_MAX_TOKEN];
    char rhs[ARCANE_OPT_MAX_LINE];

    if (!identifier || !identifier[0]) {
        return 0;
    }

    if (!parse_assignment_parts(line, lhs, sizeof(lhs), rhs, sizeof(rhs))) {
        return 0;
    }

    return strcmp(lhs, identifier) == 0;
}

static int pass_constant_folding(ArcaneICG *icg) {
    int line_index;
    int changed = 0;

    if (!icg || !icg->lines) {
        return 0;
    }

    for (line_index = 0; line_index < icg->line_count; ++line_index) {
        char lhs[ARCANE_OPT_MAX_TOKEN];
        char rhs[ARCANE_OPT_MAX_LINE];
        char left[ARCANE_OPT_MAX_TOKEN];
        char right[ARCANE_OPT_MAX_TOKEN];
        char op[3];
        int left_value;
        int right_value;
        int folded_value;

        if (!icg->lines[line_index] || line_is_unsafe_for_optimization(icg->lines[line_index])) {
            continue;
        }

        if (!parse_assignment_parts(icg->lines[line_index], lhs, sizeof(lhs), rhs, sizeof(rhs))) {
            continue;
        }

        if (!parse_binary_rhs(rhs, left, sizeof(left), op, sizeof(op), right, sizeof(right))) {
            continue;
        }

        if (!parse_integer_token(left, &left_value) || !parse_integer_token(right, &right_value)) {
            continue;
        }

        if (!eval_binary_int(left_value, right_value, op, &folded_value)) {
            continue;
        }

        if (replace_line_with_int(&icg->lines[line_index], lhs, folded_value)) {
            changed = 1;
        }
    }

    return changed;
}

static int pass_algebraic_simplification(ArcaneICG *icg) {
    int line_index;
    int changed = 0;

    if (!icg || !icg->lines) {
        return 0;
    }

    for (line_index = 0; line_index < icg->line_count; ++line_index) {
        char lhs[ARCANE_OPT_MAX_TOKEN];
        char rhs[ARCANE_OPT_MAX_LINE];
        char left[ARCANE_OPT_MAX_TOKEN];
        char right[ARCANE_OPT_MAX_TOKEN];
        char op[3];

        if (!icg->lines[line_index] || line_is_unsafe_for_optimization(icg->lines[line_index])) {
            continue;
        }

        if (!parse_assignment_parts(icg->lines[line_index], lhs, sizeof(lhs), rhs, sizeof(rhs))) {
            continue;
        }

        if (!parse_binary_rhs(rhs, left, sizeof(left), op, sizeof(op), right, sizeof(right))) {
            continue;
        }

        if (strcmp(op, "+") == 0) {
            if (strcmp(right, "0") == 0) {
                changed |= replace_line_with_assignment(&icg->lines[line_index], lhs, left);
                continue;
            }
            if (strcmp(left, "0") == 0) {
                changed |= replace_line_with_assignment(&icg->lines[line_index], lhs, right);
                continue;
            }
        }

        if (strcmp(op, "-") == 0 && strcmp(right, "0") == 0) {
            changed |= replace_line_with_assignment(&icg->lines[line_index], lhs, left);
            continue;
        }

        if (strcmp(op, "*") == 0) {
            if (strcmp(right, "1") == 0) {
                changed |= replace_line_with_assignment(&icg->lines[line_index], lhs, left);
                continue;
            }
            if (strcmp(left, "1") == 0) {
                changed |= replace_line_with_assignment(&icg->lines[line_index], lhs, right);
                continue;
            }
            if (strcmp(left, "0") == 0 || strcmp(right, "0") == 0) {
                changed |= replace_line_with_int(&icg->lines[line_index], lhs, 0);
                continue;
            }
        }

        if (strcmp(op, "/") == 0 && strcmp(right, "1") == 0) {
            changed |= replace_line_with_assignment(&icg->lines[line_index], lhs, left);
            continue;
        }
    }

    return changed;
}

static int pass_copy_propagation(ArcaneICG *icg) {
    int i;
    int changed = 0;

    if (!icg || !icg->lines) {
        return 0;
    }

    for (i = 0; i < icg->line_count; ++i) {
        char lhs[ARCANE_OPT_MAX_TOKEN];
        char rhs[ARCANE_OPT_MAX_LINE];
        int j;

        if (!icg->lines[i] || line_is_unsafe_for_optimization(icg->lines[i])) {
            continue;
        }

        if (!parse_assignment_parts(icg->lines[i], lhs, sizeof(lhs), rhs, sizeof(rhs))) {
            continue;
        }

        if (!is_identifier_token(rhs)) {
            continue;
        }

        for (j = i + 1; j < icg->line_count; ++j) {
            char use_lhs[ARCANE_OPT_MAX_TOKEN];
            char use_rhs[ARCANE_OPT_MAX_LINE];

            if (!icg->lines[j]) {
                continue;
            }

            if (line_is_unsafe_for_optimization(icg->lines[j])) {
                break;
            }

            if (line_redefines_identifier(icg->lines[j], lhs) || line_redefines_identifier(icg->lines[j], rhs)) {
                break;
            }

            if (!parse_assignment_parts(icg->lines[j], use_lhs, sizeof(use_lhs), use_rhs, sizeof(use_rhs))) {
                continue;
            }

            if (strcmp(use_rhs, lhs) == 0) {
                changed |= replace_line_with_assignment(&icg->lines[j], use_lhs, rhs);
            }
        }
    }

    return changed;
}

static int pass_dead_code_elimination(ArcaneICG *icg) {
    int i;
    int changed = 0;

    if (!icg || !icg->lines) {
        return 0;
    }

    for (i = 0; i < icg->line_count; ++i) {
        int j;
        int used_later = 0;
        char lhs[ARCANE_OPT_MAX_TOKEN];
        char rhs[ARCANE_OPT_MAX_LINE];

        if (!icg->lines[i] || line_is_unsafe_for_optimization(icg->lines[i])) {
            continue;
        }

        if (!parse_assignment_parts(icg->lines[i], lhs, sizeof(lhs), rhs, sizeof(rhs))) {
            continue;
        }

        if (!is_temp_name(lhs)) {
            continue;
        }

        for (j = i + 1; j < icg->line_count; ++j) {
            if (!icg->lines[j]) {
                continue;
            }

            if (line_uses_identifier(icg->lines[j], lhs)) {
                used_later = 1;
                break;
            }

            if (line_redefines_identifier(icg->lines[j], lhs)) {
                break;
            }
        }

        if (!used_later) {
            free(icg->lines[i]);
            icg->lines[i] = NULL;
            changed = 1;
        }
    }

    return changed;
}

void optimizer_constant_folding(ArcaneICG *icg) {
    (void)pass_constant_folding(icg);
    (void)pass_algebraic_simplification(icg);
}

void optimizer_copy_propagation(ArcaneICG *icg) {
    (void)pass_copy_propagation(icg);
}

void optimizer_dead_code_elimination(ArcaneICG *icg) {
    (void)pass_dead_code_elimination(icg);
}

void optimizer_run_all(ArcaneICG *icg) {
    int changed;
    int iteration = 0;
    int max_iterations;

    if (!icg) {
        return;
    }

    max_iterations = (icg->line_count > 0 ? icg->line_count * 4 : 16);
    if (max_iterations < 16) {
        max_iterations = 16;
    }

    do {
        changed = 0;
        changed |= pass_constant_folding(icg);
        changed |= pass_algebraic_simplification(icg);
        changed |= pass_copy_propagation(icg);
        changed |= pass_dead_code_elimination(icg);
        iteration++;
    } while (changed && iteration < max_iterations);
}
