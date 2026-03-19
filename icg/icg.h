#ifndef ARCANE_ICG_H
#define ARCANE_ICG_H

#include <stdio.h>

typedef struct {
    int temp_count;
    int label_count;
    FILE *out;
} ArcaneICG;

void icg_init(ArcaneICG *icg, FILE *output);
void icg_reset(ArcaneICG *icg);

char *icg_new_temp(ArcaneICG *icg);
char *icg_new_label(ArcaneICG *icg);
char *icg_dup_text(const char *text);
char *icg_int_literal(int value);

void icg_emit(ArcaneICG *icg, const char *format, ...);
void icg_emit_binary(ArcaneICG *icg, const char *result, const char *left, const char *op, const char *right);
void icg_emit_assign(ArcaneICG *icg, const char *target, const char *source);
void icg_emit_label(ArcaneICG *icg, const char *label);
void icg_emit_goto(ArcaneICG *icg, const char *label);
void icg_emit_if_false(ArcaneICG *icg, const char *condition_place, const char *label);

#endif
