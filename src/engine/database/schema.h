

#pragma once

#include <stdint.h>

#include "config.h"

struct field {
    char name[FIELD_NAME_SIZE];
    size_t offset, stride;
};

struct schema {
    struct field *fields;
    struct associate_array *index;
};

void schema_init(struct schema *schema);
struct field *schema_get_field(struct schema *schema, uint64_t name_hash);
