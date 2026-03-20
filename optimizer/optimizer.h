#ifndef ARCANE_OPTIMIZER_H
#define ARCANE_OPTIMIZER_H

#include "../icg/icg.h"

void optimizer_constant_folding(ArcaneICG *icg);
void optimizer_algebraic_simplification(ArcaneICG *icg);
void optimizer_dead_temp_elimination(ArcaneICG *icg);
void optimizer_run_all(ArcaneICG *icg);

#endif
