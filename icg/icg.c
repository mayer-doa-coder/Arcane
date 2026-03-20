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
