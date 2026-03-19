#include "icg.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
}

void icg_reset(ArcaneICG *icg) {
    if (!icg) {
        return;
    }

    icg->temp_count = 0;
    icg->label_count = 0;
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

    if (!icg || !icg->out || !format) {
        return;
    }

    va_start(args, format);
    vfprintf(icg->out, format, args);
    va_end(args);
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
