
#pragma once

#include <stdint.h>

struct property_db {
    struct property *properties;

    // map from name to property name
};

void property_db_init();

void property_db_add_table();

void property_db_table_id();
