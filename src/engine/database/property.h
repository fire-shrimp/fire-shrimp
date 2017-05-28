#pragma once

#include <stdint.h>

#include "inline-map.h"
#include "property-schema.h"

struct property {
    void *data;
    uint32_t capacity, count;
    size_t stride;
    struct inline_map index;
    struct schema *schema;
};
