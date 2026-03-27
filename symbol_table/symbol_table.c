#include "symbol_table.h"

ArcaneSymbolTable g_arcane_symbol_table;
static int g_current_scope_level = 0;
static ArcaneHouse g_current_house = ARCANE_HOUSE_NONE;
static char g_current_owner_function[ARCANE_MAX_NAME_LEN] = "";

static int arcane_map_add_result(int add_result) {
    if (add_result >= 0) {
        return ARCANE_SYMBOL_OK;
    }
    if (add_result == -2) {
        return ARCANE_SYMBOL_TABLE_FULL;
    }
    if (add_result == -3) {
        return ARCANE_SYMBOL_DUPLICATE;
    }
    return ARCANE_SYMBOL_INVALID;
}

void arcane_symbols_reset(void) {
    arcane_symbol_table_init(&g_arcane_symbol_table);
    g_current_scope_level = 0;
    g_current_house = ARCANE_HOUSE_NONE;
    g_current_owner_function[0] = '\0';
}

void arcane_symbol_set_context(int scope_level, const char *owner_function, ArcaneHouse house) {
    g_current_scope_level = scope_level;
    g_current_house = house;
    arcane_copy_text(g_current_owner_function, sizeof(g_current_owner_function), owner_function);
}

int arcane_insert_symbol(
    const char *name,
    ArcaneSymbolKind kind,
    ArcaneType type,
    ArcaneHouse house,
    int scope_level,
    const char *owner_function,
    int declared_line
) {
    int add_result;

    add_result = arcane_symbol_table_add(
        &g_arcane_symbol_table,
        name,
        kind,
        type,
        house,
        scope_level,
        owner_function,
        declared_line
    );

    if (add_result >= 0 && kind == ARCANE_SYMBOL_PARAMETER) {
        g_arcane_symbol_table.entries[add_result].initialized = 1;
    }

    return arcane_map_add_result(add_result);
}

int arcane_insert_variable(
    const char *name,
    ArcaneType type,
    ArcaneHouse house,
    int scope_level,
    const char *owner_function,
    int declared_line
) {
    return arcane_insert_symbol(
        name,
        ARCANE_SYMBOL_VARIABLE,
        type,
        house,
        scope_level,
        owner_function,
        declared_line
    );
}

int arcane_insert_int_variable(
    const char *name,
    ArcaneType type,
    int int_value,
    ArcaneHouse house,
    int scope_level,
    const char *owner_function,
    int declared_line
) {
    int add_result;

    add_result = arcane_symbol_table_add(
        &g_arcane_symbol_table,
        name,
        ARCANE_SYMBOL_VARIABLE,
        type,
        house,
        scope_level,
        owner_function,
        declared_line
    );

    if (add_result >= 0) {
        g_arcane_symbol_table.entries[add_result].value.int_value = int_value;
        g_arcane_symbol_table.entries[add_result].initialized = 1;
        return ARCANE_SYMBOL_OK;
    }

    return arcane_map_add_result(add_result);
}

int arcane_find_symbol(
    const char *name,
    int scope_level,
    const char *owner_function
) {
    return arcane_symbol_table_find_visible(
        &g_arcane_symbol_table,
        name,
        scope_level,
        owner_function
    );
}

int insert_symbol(const char *name, ArcaneType type, int value) {
    int add_result;

    add_result = arcane_symbol_table_add(
        &g_arcane_symbol_table,
        name,
        ARCANE_SYMBOL_VARIABLE,
        type,
        g_current_house,
        g_current_scope_level,
        g_current_owner_function,
        0
    );

    if (add_result >= 0) {
        g_arcane_symbol_table.entries[add_result].initialized = 1;
        g_arcane_symbol_table.entries[add_result].value.int_value = value;
        return ARCANE_SYMBOL_OK;
    }

    return arcane_map_add_result(add_result);
}

int lookup_symbol(const char *name) {
    return arcane_symbol_table_find_visible(
        &g_arcane_symbol_table,
        name,
        g_current_scope_level,
        g_current_owner_function
    );
}

int arcane_find_global_function(const char *name) {
    int i;

    if (!name || name[0] == '\0') {
        return ARCANE_SYMBOL_INVALID;
    }

    for (i = g_arcane_symbol_table.count - 1; i >= 0; --i) {
        ArcaneSymbol *symbol = &g_arcane_symbol_table.entries[i];
        if (symbol->kind != ARCANE_SYMBOL_FUNCTION) {
            continue;
        }
        if (symbol->scope_level != 0 || symbol->owner_function[0] != '\0') {
            continue;
        }
        if (strcmp(symbol->name, name) == 0) {
            return i;
        }
    }

    return ARCANE_SYMBOL_INVALID;
}

int arcane_symbol_get_type(const char *name, ArcaneType *type_out) {
    int symbol_index;

    if (!type_out) {
        return ARCANE_SYMBOL_INVALID;
    }

    symbol_index = lookup_symbol(name);
    if (symbol_index < 0) {
        return ARCANE_SYMBOL_INVALID;
    }

    *type_out = g_arcane_symbol_table.entries[symbol_index].type;
    return ARCANE_SYMBOL_OK;
}

int arcane_symbol_get_kind(const char *name, ArcaneSymbolKind *kind_out) {
    int symbol_index;

    if (!kind_out) {
        return ARCANE_SYMBOL_INVALID;
    }

    symbol_index = lookup_symbol(name);
    if (symbol_index < 0) {
        return ARCANE_SYMBOL_INVALID;
    }

    *kind_out = g_arcane_symbol_table.entries[symbol_index].kind;
    return ARCANE_SYMBOL_OK;
}

int arcane_symbol_get_initialized(const char *name, int *initialized_out) {
    int symbol_index;

    if (!initialized_out) {
        return ARCANE_SYMBOL_INVALID;
    }

    symbol_index = lookup_symbol(name);
    if (symbol_index < 0) {
        return ARCANE_SYMBOL_INVALID;
    }

    *initialized_out = g_arcane_symbol_table.entries[symbol_index].initialized;
    return ARCANE_SYMBOL_OK;
}

int arcane_symbol_mark_used(const char *name) {
    int symbol_index;

    symbol_index = lookup_symbol(name);
    if (symbol_index < 0) {
        return ARCANE_SYMBOL_INVALID;
    }

    return arcane_symbol_table_mark_used(&g_arcane_symbol_table, symbol_index) == 0
        ? ARCANE_SYMBOL_OK
        : ARCANE_SYMBOL_INVALID;
}

int arcane_symbol_mark_initialized(const char *name) {
    int symbol_index;

    symbol_index = lookup_symbol(name);
    if (symbol_index < 0) {
        return ARCANE_SYMBOL_INVALID;
    }

    return arcane_symbol_table_mark_initialized(&g_arcane_symbol_table, symbol_index) == 0
        ? ARCANE_SYMBOL_OK
        : ARCANE_SYMBOL_INVALID;
}

int arcane_symbol_set_function_signature(const char *name, ArcaneType return_type, int param_count, const ArcaneType *param_types) {
    int function_index;
    int i;

    if (param_count < 0 || param_count > ARCANE_MAX_FUNCTION_PARAMS) {
        return ARCANE_SYMBOL_INVALID;
    }

    function_index = arcane_find_global_function(name);
    if (function_index < 0) {
        return ARCANE_SYMBOL_INVALID;
    }

    g_arcane_symbol_table.entries[function_index].type = return_type;
    g_arcane_symbol_table.entries[function_index].param_count = param_count;
    for (i = 0; i < param_count; ++i) {
        g_arcane_symbol_table.entries[function_index].param_types[i] = param_types ? param_types[i] : ARCANE_TYPE_UNKNOWN;
    }
    for (; i < ARCANE_MAX_FUNCTION_PARAMS; ++i) {
        g_arcane_symbol_table.entries[function_index].param_types[i] = ARCANE_TYPE_UNKNOWN;
    }

    return ARCANE_SYMBOL_OK;
}

int arcane_symbol_get_function_signature(const char *name, ArcaneType *return_type_out, int *param_count_out, ArcaneType *param_types_out, int max_param_types) {
    int function_index;
    int i;
    int copy_count;

    function_index = arcane_find_global_function(name);
    if (function_index < 0) {
        return ARCANE_SYMBOL_INVALID;
    }

    if (return_type_out) {
        *return_type_out = g_arcane_symbol_table.entries[function_index].type;
    }

    if (param_count_out) {
        *param_count_out = g_arcane_symbol_table.entries[function_index].param_count;
    }

    if (param_types_out && max_param_types > 0) {
        copy_count = g_arcane_symbol_table.entries[function_index].param_count;
        if (copy_count > max_param_types) {
            copy_count = max_param_types;
        }
        for (i = 0; i < copy_count; ++i) {
            param_types_out[i] = g_arcane_symbol_table.entries[function_index].param_types[i];
        }
    }

    return ARCANE_SYMBOL_OK;
}
