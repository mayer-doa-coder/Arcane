#ifndef ARCANE_SYMBOL_TABLE_H
#define ARCANE_SYMBOL_TABLE_H

#include <stddef.h>
#include <string.h>

#define ARCANE_MAX_SYMBOLS 256
#define ARCANE_MAX_NAME_LEN 64
#define ARCANE_MAX_STRING_VALUE_LEN 128

typedef enum {
    ARCANE_TYPE_UNKNOWN = 0,
    ARCANE_TYPE_INT,
    ARCANE_TYPE_FLOAT,
    ARCANE_TYPE_DOUBLE,
    ARCANE_TYPE_LONG,
    ARCANE_TYPE_CHAR,
    ARCANE_TYPE_BOOL,
    ARCANE_TYPE_VOID,
    ARCANE_TYPE_STRING
} ArcaneType;

typedef enum {
    ARCANE_HOUSE_NONE = 0,
    ARCANE_HOUSE_GRYFFINDOR = 1,
    ARCANE_HOUSE_SLYTHERIN = 2,
    ARCANE_HOUSE_HUFFLEPUFF = 3,
    ARCANE_HOUSE_RAVENCLAW = 4
} ArcaneHouse;

typedef enum {
    ARCANE_SYMBOL_VARIABLE = 1,
    ARCANE_SYMBOL_FUNCTION = 2,
    ARCANE_SYMBOL_PARAMETER = 3
} ArcaneSymbolKind;

typedef union {
    int int_value;
    float float_value;
    double double_value;
    long long_value;
    char char_value;
    int bool_value;
    char string_value[ARCANE_MAX_STRING_VALUE_LEN];
} ArcaneValue;

typedef struct {
    char name[ARCANE_MAX_NAME_LEN];
    ArcaneSymbolKind kind;
    ArcaneType type;
    ArcaneHouse house;
    int scope_level;
    char owner_function[ARCANE_MAX_NAME_LEN];
    int declared_line;
    int initialized;
    int used;
    ArcaneValue value;
} ArcaneSymbol;

typedef struct {
    ArcaneSymbol entries[ARCANE_MAX_SYMBOLS];
    int count;
} ArcaneSymbolTable;

#define ARCANE_SYMBOL_OK 0
#define ARCANE_SYMBOL_DUPLICATE -1
#define ARCANE_SYMBOL_TABLE_FULL -2
#define ARCANE_SYMBOL_INVALID -3

extern ArcaneSymbolTable g_arcane_symbol_table;

void arcane_symbols_reset(void);
void arcane_symbol_set_context(int scope_level, const char *owner_function, ArcaneHouse house);
int arcane_insert_symbol(
    const char *name,
    ArcaneSymbolKind kind,
    ArcaneType type,
    ArcaneHouse house,
    int scope_level,
    const char *owner_function,
    int declared_line
);
int arcane_insert_variable(
    const char *name,
    ArcaneType type,
    ArcaneHouse house,
    int scope_level,
    const char *owner_function,
    int declared_line
);
int arcane_insert_int_variable(
    const char *name,
    ArcaneType type,
    int int_value,
    ArcaneHouse house,
    int scope_level,
    const char *owner_function,
    int declared_line
);
int arcane_find_symbol(
    const char *name,
    int scope_level,
    const char *owner_function
);

int insert_symbol(const char *name, ArcaneType type, int value);
int lookup_symbol(const char *name);

static inline void arcane_copy_text(char *dst, size_t dst_size, const char *src) {
    if (!dst || dst_size == 0) {
        return;
    }
    if (!src) {
        dst[0] = '\0';
        return;
    }
    strncpy(dst, src, dst_size - 1);
    dst[dst_size - 1] = '\0';
}

static inline void arcane_symbol_table_init(ArcaneSymbolTable *table) {
    if (!table) {
        return;
    }
    table->count = 0;
    memset(table->entries, 0, sizeof(table->entries));
}

static inline int arcane_symbol_matches_scope(
    const ArcaneSymbol *symbol,
    int scope_level,
    const char *owner_function
) {
    if (!symbol) {
        return 0;
    }
    if (symbol->scope_level != scope_level) {
        return 0;
    }

    if (!owner_function || owner_function[0] == '\0') {
        return symbol->owner_function[0] == '\0';
    }

    return strcmp(symbol->owner_function, owner_function) == 0;
}

static inline int arcane_symbol_table_find_in_scope(
    const ArcaneSymbolTable *table,
    const char *name,
    int scope_level,
    const char *owner_function
) {
    int index;

    if (!table || !name) {
        return -1;
    }

    for (index = table->count - 1; index >= 0; --index) {
        const ArcaneSymbol *symbol = &table->entries[index];
        if (strcmp(symbol->name, name) != 0) {
            continue;
        }
        if (arcane_symbol_matches_scope(symbol, scope_level, owner_function)) {
            return index;
        }
    }

    return -1;
}

static inline int arcane_symbol_table_find_visible(
    const ArcaneSymbolTable *table,
    const char *name,
    int scope_level,
    const char *owner_function
) {
    int index;

    if (!table || !name) {
        return -1;
    }

    for (index = table->count - 1; index >= 0; --index) {
        const ArcaneSymbol *symbol = &table->entries[index];
        if (strcmp(symbol->name, name) != 0) {
            continue;
        }

        if (scope_level >= symbol->scope_level) {
            if (owner_function && owner_function[0] != '\0') {
                if (symbol->owner_function[0] == '\0' || strcmp(symbol->owner_function, owner_function) == 0) {
                    return index;
                }
            } else if (symbol->owner_function[0] == '\0') {
                return index;
            }
        }
    }

    return -1;
}

static inline int arcane_symbol_table_add(
    ArcaneSymbolTable *table,
    const char *name,
    ArcaneSymbolKind kind,
    ArcaneType type,
    ArcaneHouse house,
    int scope_level,
    const char *owner_function,
    int declared_line
) {
    ArcaneSymbol *symbol;

    if (!table || !name || name[0] == '\0') {
        return -1;
    }

    if (table->count >= ARCANE_MAX_SYMBOLS) {
        return -2;
    }

    if (arcane_symbol_table_find_in_scope(table, name, scope_level, owner_function) >= 0) {
        return -3;
    }

    symbol = &table->entries[table->count];
    memset(symbol, 0, sizeof(*symbol));

    arcane_copy_text(symbol->name, sizeof(symbol->name), name);
    symbol->kind = kind;
    symbol->type = type;
    symbol->house = house;
    symbol->scope_level = scope_level;
    arcane_copy_text(symbol->owner_function, sizeof(symbol->owner_function), owner_function);
    symbol->declared_line = declared_line;
    symbol->initialized = 0;
    symbol->used = 0;

    table->count++;
    return table->count - 1;
}

static inline int arcane_symbol_table_mark_used(ArcaneSymbolTable *table, int symbol_index) {
    if (!table || symbol_index < 0 || symbol_index >= table->count) {
        return -1;
    }

    table->entries[symbol_index].used = 1;
    return 0;
}

static inline int arcane_symbol_table_mark_initialized(ArcaneSymbolTable *table, int symbol_index) {
    if (!table || symbol_index < 0 || symbol_index >= table->count) {
        return -1;
    }

    table->entries[symbol_index].initialized = 1;
    return 0;
}

static inline const char *arcane_type_name(ArcaneType type) {
    switch (type) {
        case ARCANE_TYPE_INT: return "INT";
        case ARCANE_TYPE_FLOAT: return "FLOAT";
        case ARCANE_TYPE_DOUBLE: return "DOUBLE";
        case ARCANE_TYPE_LONG: return "LONG";
        case ARCANE_TYPE_CHAR: return "CHAR";
        case ARCANE_TYPE_BOOL: return "BOOL";
        case ARCANE_TYPE_VOID: return "VOID";
        case ARCANE_TYPE_STRING: return "STRING";
        default: return "UNKNOWN";
    }
}

static inline const char *arcane_house_name(ArcaneHouse house) {
    switch (house) {
        case ARCANE_HOUSE_GRYFFINDOR: return "Gryffindor";
        case ARCANE_HOUSE_SLYTHERIN: return "Slytherin";
        case ARCANE_HOUSE_HUFFLEPUFF: return "Hufflepuff";
        case ARCANE_HOUSE_RAVENCLAW: return "Ravenclaw";
        default: return "None";
    }
}

#endif
