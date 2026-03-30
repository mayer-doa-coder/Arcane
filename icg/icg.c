#include "icg.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static void icg_free_lines(ArcaneICG *icg) {
    int index;

    if (!icg || !icg->lines) {
        return;
    }

    for (index = 0; index < icg->line_count; ++index) {
        free(icg->lines[index]);
    }

    free(icg->lines);
    icg->lines = NULL;
    icg->line_count = 0;
    icg->line_capacity = 0;
}

static int icg_append_line(ArcaneICG *icg, char *line_text) {
    char **grown;
    int new_capacity;

    if (!icg || !line_text) {
        free(line_text);
        return 0;
    }

    if (icg->line_count >= icg->line_capacity) {
        new_capacity = (icg->line_capacity == 0) ? 32 : icg->line_capacity * 2;
        grown = (char **)realloc(icg->lines, sizeof(char *) * (size_t)new_capacity);
        if (!grown) {
            free(line_text);
            return 0;
        }
        icg->lines = grown;
        icg->line_capacity = new_capacity;
    }

    icg->lines[icg->line_count] = line_text;
    icg->line_count++;
    return 1;
}

static char *icg_alloc_formatted(const char *prefix, int index) {
    char buffer[64];
    int written;
    char *result;

    written = snprintf(buffer, sizeof(buffer), "%s%d", prefix, index);
    if (written <= 0 || written >= (int)sizeof(buffer)) {
        return NULL;
    }

    result = (char *)malloc((size_t)written + 1);
    if (!result) {
        return NULL;
    }

    memcpy(result, buffer, (size_t)written + 1);
    return result;
}

void icg_init(ArcaneICG *icg, FILE *output) {
    if (!icg) {
        return;
    }

    icg->temp_count = 0;
    icg->label_count = 0;
    icg->out = output ? output : stdout;
    icg->lines = NULL;
    icg->line_count = 0;
    icg->line_capacity = 0;
}

void icg_reset(ArcaneICG *icg) {
    if (!icg) {
        return;
    }

    icg_free_lines(icg);
    icg->temp_count = 0;
    icg->label_count = 0;
}

void icg_release(ArcaneICG *icg) {
    if (!icg) {
        return;
    }

    icg_free_lines(icg);
    icg->temp_count = 0;
    icg->label_count = 0;
    icg->out = NULL;
}

char *icg_new_temp(ArcaneICG *icg) {
    if (!icg) {
        return NULL;
    }

    icg->temp_count++;
    return icg_alloc_formatted("t", icg->temp_count);
}

char *icg_new_label(ArcaneICG *icg) {
    if (!icg) {
        return NULL;
    }

    icg->label_count++;
    return icg_alloc_formatted("L", icg->label_count);
}

char *icg_dup_text(const char *text) {
    size_t length;
    char *copy;

    if (!text) {
        return NULL;
    }

    length = strlen(text);
    copy = (char *)malloc(length + 1);
    if (!copy) {
        return NULL;
    }

    memcpy(copy, text, length + 1);
    return copy;
}

char *icg_int_literal(int value) {
    char buffer[64];
    int written;
    char *result;

    written = snprintf(buffer, sizeof(buffer), "%d", value);
    if (written <= 0 || written >= (int)sizeof(buffer)) {
        return NULL;
    }

    result = (char *)malloc((size_t)written + 1);
    if (!result) {
        return NULL;
    }

    memcpy(result, buffer, (size_t)written + 1);
    return result;
}

void icg_emit(ArcaneICG *icg, const char *format, ...) {
    va_list args;
    va_list args_copy;
    int required;
    char *line_text;

    if (!icg || !format) {
        return;
    }

    va_start(args, format);
    va_copy(args_copy, args);
    required = vsnprintf(NULL, 0, format, args_copy);
    va_end(args_copy);

    if (required < 0) {
        va_end(args);
        return;
    }

    line_text = (char *)malloc((size_t)required + 1);
    if (!line_text) {
        va_end(args);
        return;
    }

    vsnprintf(line_text, (size_t)required + 1, format, args);
    va_end(args);

    (void)icg_append_line(icg, line_text);
}

void icg_emit_binary(ArcaneICG *icg, const char *result, const char *left, const char *op, const char *right) {
    if (!result || !left || !op || !right) {
        return;
    }

    icg_emit(icg, "%s=%s%s%s\n", result, left, op, right);
}

void icg_emit_assign(ArcaneICG *icg, const char *target, const char *source) {
    if (!target || !source) {
        return;
    }

    icg_emit(icg, "%s=%s\n", target, source);
}

void icg_emit_label(ArcaneICG *icg, const char *label) {
    if (!label) {
        return;
    }

    icg_emit(icg, "%s:\n", label);
}

void icg_emit_goto(ArcaneICG *icg, const char *label) {
    if (!label) {
        return;
    }

    icg_emit(icg, "goto %s\n", label);
}

void icg_emit_if_false(ArcaneICG *icg, const char *condition_place, const char *label) {
    if (!condition_place || !label) {
        return;
    }

    icg_emit(icg, "ifFalse %s goto %s\n", condition_place, label);
}

void icg_emit_func_begin(ArcaneICG *icg, const char *function_name) {
    if (!function_name) {
        return;
    }

    icg_emit(icg, "func_begin %s\n", function_name);
}

void icg_emit_func_end(ArcaneICG *icg, const char *function_name) {
    if (!function_name) {
        return;
    }

    icg_emit(icg, "func_end %s\n", function_name);
}

void icg_emit_param(ArcaneICG *icg, const char *param_name) {
    if (!param_name) {
        return;
    }

    icg_emit(icg, "param %s\n", param_name);
}

void icg_emit_arg(ArcaneICG *icg, const char *arg_place) {
    if (!arg_place) {
        return;
    }

    icg_emit(icg, "arg %s\n", arg_place);
}

void icg_emit_call(ArcaneICG *icg, const char *function_name, int arg_count) {
    if (!function_name) {
        return;
    }

    icg_emit(icg, "call %s %d\n", function_name, arg_count);
}

void icg_emit_call_assign(ArcaneICG *icg, const char *target, const char *function_name, int arg_count) {
    if (!target || !function_name) {
        return;
    }

    icg_emit(icg, "%s = call %s %d\n", target, function_name, arg_count);
}

void icg_emit_return(ArcaneICG *icg, const char *value_place) {
    if (!value_place || value_place[0] == '\0') {
        icg_emit(icg, "return\n");
        return;
    }

    icg_emit(icg, "return %s\n", value_place);
}

void icg_emit_input(ArcaneICG *icg, const char *target) {
    if (!target || target[0] == '\0') {
        return;
    }

    icg_emit(icg, "input %s\n", target);
}

void icg_emit_math_call(ArcaneICG *icg, const char *target, const char *function_name, const char *arg1, const char *arg2, int arg_count) {
    if (!target || !function_name || !arg1) {
        return;
    }

    if (arg_count == 2) {
        if (!arg2) {
            return;
        }
        icg_emit(icg, "%s=%s(%s,%s)\n", target, function_name, arg1, arg2);
        return;
    }

    icg_emit(icg, "%s=%s(%s)\n", target, function_name, arg1);
}

void print_icg(const ArcaneICG *icg, FILE *output) {
    int index;
    FILE *stream;

    if (!icg) {
        return;
    }

    stream = output ? output : icg->out;
    if (!stream) {
        stream = stdout;
    }

    for (index = 0; index < icg->line_count; ++index) {
        if (icg->lines[index]) {
            fputs(icg->lines[index], stream);
        }
    }
}
