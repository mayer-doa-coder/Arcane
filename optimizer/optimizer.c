#include "optimizer.h"

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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

static int is_identifier_char(char ch) {
    return isalnum((unsigned char)ch) || ch == '_';
}

static int is_temp_name(const char *name) {
    size_t index;

    if (!name || name[0] != 't') {
        return 0;
    }

    if (!isdigit((unsigned char)name[1])) {
        return 0;
    }

    for (index = 2; name[index] != '\0'; ++index) {
        if (!isdigit((unsigned char)name[index])) {
            return 0;
        }
    }

    return 1;
}

static int parse_assignment_parts(const char *line, char *lhs, size_t lhs_size, char *rhs, size_t rhs_size) {
    const char *equal_pos;
    size_t lhs_len;
    size_t rhs_len;

    if (!line || !lhs || !rhs || lhs_size == 0 || rhs_size == 0) {
        return 0;
    }

    equal_pos = strchr(line, '=');
    if (!equal_pos) {
        return 0;
    }

    lhs_len = (size_t)(equal_pos - line);
    if (lhs_len == 0 || lhs_len >= lhs_size) {
        return 0;
    }

    memcpy(lhs, line, lhs_len);
    lhs[lhs_len] = '\0';

    rhs_len = strcspn(equal_pos + 1, "\r\n");
    if (rhs_len == 0 || rhs_len >= rhs_size) {
        return 0;
    }

    memcpy(rhs, equal_pos + 1, rhs_len);
    rhs[rhs_len] = '\0';

    if (strchr(lhs, ' ') || strchr(rhs, ' ')) {
        return 0;
    }

    return 1;
}

static int parse_binary_rhs(const char *rhs, char *left, size_t left_size, char *op, size_t op_size, char *right, size_t right_size) {
    size_t index;
    size_t rhs_len;
    size_t op_len;
    size_t left_len;
    size_t right_len;

    if (!rhs || !left || !op || !right || left_size == 0 || op_size < 2 || right_size == 0) {
        return 0;
    }

    rhs_len = strlen(rhs);
    if (rhs_len < 3) {
        return 0;
    }

    for (index = 1; index < rhs_len; ++index) {
        char ch = rhs[index];

        if (ch == '+' || ch == '*' || ch == '/' || ch == '%' || ch == '>' || ch == '<' || ch == '=' || ch == '!') {
            break;
        }

        if (ch == '-') {
            break;
        }
    }

    if (index >= rhs_len) {
        return 0;
    }

    op_len = 1;
    if ((rhs[index] == '>' || rhs[index] == '<' || rhs[index] == '=' || rhs[index] == '!') &&
        (index + 1 < rhs_len) && rhs[index + 1] == '=') {
        op_len = 2;
    }

    left_len = index;
    right_len = rhs_len - index - op_len;

    if (left_len == 0 || right_len == 0 || left_len >= left_size || right_len >= right_size || op_len >= op_size) {
        return 0;
    }

    memcpy(left, rhs, left_len);
    left[left_len] = '\0';

    memcpy(op, rhs + index, op_len);
    op[op_len] = '\0';

    memcpy(right, rhs + index + op_len, right_len);
    right[right_len] = '\0';

    return 1;
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

static int replace_line_with_assignment(char **line_ptr, const char *lhs, const char *rhs) {
    int required;
    char *replacement;

    if (!line_ptr || !*line_ptr || !lhs || !rhs) {
        return 0;
    }

    required = snprintf(NULL, 0, "%s=%s\n", lhs, rhs);
    if (required <= 0) {
        return 0;
    }

    replacement = (char *)malloc((size_t)required + 1);
    if (!replacement) {
        return 0;
    }

    snprintf(replacement, (size_t)required + 1, "%s=%s\n", lhs, rhs);
    free(*line_ptr);
    *line_ptr = replacement;
    return 1;
}

static int replace_line_with_int(char **line_ptr, const char *lhs, int value) {
    int required;
    char *replacement;

    if (!line_ptr || !*line_ptr || !lhs) {
        return 0;
    }

    required = snprintf(NULL, 0, "%s=%d\n", lhs, value);
    if (required <= 0) {
        return 0;
    }

    replacement = (char *)malloc((size_t)required + 1);
    if (!replacement) {
        return 0;
    }

    snprintf(replacement, (size_t)required + 1, "%s=%d\n", lhs, value);
    free(*line_ptr);
    *line_ptr = replacement;
    return 1;
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

void optimizer_constant_folding(ArcaneICG *icg) {
    int line_index;

    if (!icg || !icg->lines) {
        return;
    }

    for (line_index = 0; line_index < icg->line_count; ++line_index) {
        char lhs[64];
        char rhs[256];
        char left[128];
        char right[128];
        char op[3];
        int left_value;
        int right_value;
        int folded_value;

        if (!icg->lines[line_index]) {
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

        (void)replace_line_with_int(&icg->lines[line_index], lhs, folded_value);
    }
}

void optimizer_algebraic_simplification(ArcaneICG *icg) {
    int line_index;

    if (!icg || !icg->lines) {
        return;
    }

    for (line_index = 0; line_index < icg->line_count; ++line_index) {
        char lhs[64];
        char rhs[256];
        char left[128];
        char right[128];
        char op[3];

        if (!icg->lines[line_index]) {
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
                (void)replace_line_with_assignment(&icg->lines[line_index], lhs, left);
                continue;
            }
            if (strcmp(left, "0") == 0) {
                (void)replace_line_with_assignment(&icg->lines[line_index], lhs, right);
                continue;
            }
        }

        if (strcmp(op, "-") == 0) {
            if (strcmp(right, "0") == 0) {
                (void)replace_line_with_assignment(&icg->lines[line_index], lhs, left);
                continue;
            }
        }

        if (strcmp(op, "*") == 0) {
            if (strcmp(right, "1") == 0) {
                (void)replace_line_with_assignment(&icg->lines[line_index], lhs, left);
                continue;
            }
            if (strcmp(left, "1") == 0) {
                (void)replace_line_with_assignment(&icg->lines[line_index], lhs, right);
                continue;
            }
            if (strcmp(right, "0") == 0 || strcmp(left, "0") == 0) {
                (void)replace_line_with_int(&icg->lines[line_index], lhs, 0);
                continue;
            }
        }

        if (strcmp(op, "/") == 0) {
            if (strcmp(right, "1") == 0) {
                (void)replace_line_with_assignment(&icg->lines[line_index], lhs, left);
                continue;
            }
        }
    }
}

void optimizer_dead_temp_elimination(ArcaneICG *icg) {
    int line_index;

    if (!icg || !icg->lines) {
        return;
    }

    for (line_index = 0; line_index < icg->line_count; ++line_index) {
        int later_index;
        int used_later = 0;
        char lhs[64];
        char rhs[256];

        if (!icg->lines[line_index]) {
            continue;
        }

        if (!parse_assignment_parts(icg->lines[line_index], lhs, sizeof(lhs), rhs, sizeof(rhs))) {
            continue;
        }

        if (!is_temp_name(lhs)) {
            continue;
        }

        for (later_index = line_index + 1; later_index < icg->line_count; ++later_index) {
            if (!icg->lines[later_index]) {
                continue;
            }
            if (line_uses_identifier(icg->lines[later_index], lhs)) {
                used_later = 1;
                break;
            }
        }

        if (!used_later) {
            free(icg->lines[line_index]);
            icg->lines[line_index] = NULL;
        }
    }
}

void optimizer_run_all(ArcaneICG *icg) {
    optimizer_constant_folding(icg);
    optimizer_algebraic_simplification(icg);
    optimizer_dead_temp_elimination(icg);
}
