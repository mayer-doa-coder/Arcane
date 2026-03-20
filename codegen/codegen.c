#include "codegen.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

static const char *to_c_type(ArcaneType type) {
    switch (type) {
        case ARCANE_TYPE_FLOAT: return "float";
        case ARCANE_TYPE_DOUBLE: return "double";
        case ARCANE_TYPE_LONG: return "long";
        case ARCANE_TYPE_CHAR: return "char";
        case ARCANE_TYPE_BOOL: return "int";
        case ARCANE_TYPE_INT:
        case ARCANE_TYPE_UNKNOWN:
        default:
            return "int";
    }
}

static int should_emit_assignment(const char *line) {
    if (!line) {
        return 0;
    }

    if (strstr(line, "ifFalse") || strstr(line, "goto ") || strstr(line, "call ")) {
        return 0;
    }

    if (strchr(line, '=') == NULL) {
        return 0;
    }

    return 1;
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

static int extract_lhs_name(const char *line, char *out, size_t out_size) {
    const char *equal_pos;
    size_t lhs_len;

    if (!line || !out || out_size == 0) {
        return 0;
    }

    equal_pos = strchr(line, '=');
    if (!equal_pos) {
        return 0;
    }

    lhs_len = (size_t)(equal_pos - line);
    while (lhs_len > 0 && isspace((unsigned char)line[lhs_len - 1])) {
        lhs_len--;
    }
    if (lhs_len == 0 || lhs_len >= out_size) {
        return 0;
    }

    memcpy(out, line, lhs_len);
    out[lhs_len] = '\0';
    return 1;
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

static int symbol_declared(const ArcaneSymbolTable *symbols, const char *name) {
    int index;

    if (!symbols || !name) {
        return 0;
    }

    for (index = 0; index < symbols->count; ++index) {
        const ArcaneSymbol *symbol = &symbols->entries[index];
        if ((symbol->kind == ARCANE_SYMBOL_VARIABLE || symbol->kind == ARCANE_SYMBOL_PARAMETER) && strcmp(symbol->name, name) == 0) {
            return 1;
        }
    }

    return 0;
}

static void emit_ir_line_as_c(FILE *fp, const char *line) {
    size_t len;

    if (!fp || !line) {
        return;
    }

    len = line_content_length(line);
    if (len == 0) {
        return;
    }

    if (line[0] == '#') {
        fprintf(fp, "    /* %.*s */\n", (int)len, line);
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

    if (starts_with(line, "call ")) {
        char function_name[64];
        if (sscanf(line, "call %63s", function_name) == 1) {
            fprintf(fp, "    /* call %s */\n", function_name);
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
                fprintf(fp, "    printf(\"%%d\\n\", %.*s);\n", (int)operand_len, operand);
            }
        }
        return;
    }

    if (line[len - 1] == ':') {
        fprintf(fp, "    %.*s ;\n", (int)len, line);
        return;
    }

    if (should_emit_assignment(line)) {
        fprintf(fp, "    %.*s;\n", (int)len, line);
    }
}

int generate_c_code(const ArcaneSymbolTable *symbols, const ArcaneICG *icg, const char *output_path) {
    FILE *fp;
    int index;

    if (!symbols || !icg) {
        return -1;
    }

    fp = fopen(output_path ? output_path : "output.c", "w");
    if (!fp) {
        return -2;
    }

    fprintf(fp, "#include <stdio.h>\n");
    fprintf(fp, "\n");
    fprintf(fp, "int main() {\n");

    for (index = 0; index < symbols->count; ++index) {
        const ArcaneSymbol *symbol = &symbols->entries[index];
        if (symbol->kind == ARCANE_SYMBOL_VARIABLE || symbol->kind == ARCANE_SYMBOL_PARAMETER) {
            fprintf(fp, "    %s %s;\n", to_c_type(symbol->type), symbol->name);
        }
    }

    for (index = 0; index < icg->line_count; ++index) {
        char lhs_name[64];
        const char *line = icg->lines[index];

        if (!line || !should_emit_assignment(line)) {
            continue;
        }

        if (!extract_lhs_name(line, lhs_name, sizeof(lhs_name))) {
            continue;
        }

        if (is_temp_symbol(lhs_name) && !symbol_declared(symbols, lhs_name)) {
            fprintf(fp, "    int %s;\n", lhs_name);
        }
    }

    if (symbols->count > 0) {
        fprintf(fp, "\n");
    }

    for (index = 0; index < icg->line_count; ++index) {
        emit_ir_line_as_c(fp, icg->lines[index]);
    }

    fprintf(fp, "\n");
    fprintf(fp, "    return 0;\n");
    fprintf(fp, "}\n");

    fclose(fp);
    return 0;
}
