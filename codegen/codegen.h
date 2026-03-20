#ifndef ARCANE_CODEGEN_H
#define ARCANE_CODEGEN_H

#include "../symbol_table/symbol_table.h"
#include "../icg/icg.h"

int generate_c_code(const ArcaneSymbolTable *symbols, const ArcaneICG *icg, const char *output_path);

#endif
