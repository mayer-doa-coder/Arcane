#ifndef ARCANE_ICG_H
#define ARCANE_ICG_H

#include <stdio.h>

typedef struct {
    int temp_count;
    int label_count;
    FILE *out;
    char **lines;
    int line_count;
    int line_capacity;
} ArcaneICG;

void icg_init(ArcaneICG *icg, FILE *output);
void icg_reset(ArcaneICG *icg);
void icg_release(ArcaneICG *icg);

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
void icg_emit_func_begin(ArcaneICG *icg, const char *function_name);
void icg_emit_func_end(ArcaneICG *icg, const char *function_name);
void icg_emit_param(ArcaneICG *icg, const char *param_name);
void icg_emit_arg(ArcaneICG *icg, const char *arg_place);
void icg_emit_call(ArcaneICG *icg, const char *function_name, int arg_count);
void icg_emit_call_assign(ArcaneICG *icg, const char *target, const char *function_name, int arg_count);
void icg_emit_return(ArcaneICG *icg, const char *value_place);
void print_icg(const ArcaneICG *icg, FILE *output);

#endif
