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
