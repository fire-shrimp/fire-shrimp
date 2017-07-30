
#pragma once

#include <stdint.h>

#include "inline-map.h"

typedef struct array_map {
    void *values;
    uint32_t capacity;
    uint32_t count;
    size_t stride;
    inline_map index;
} array_map;

void init_array_map(array_map *arr, uint32_t capacity, size_t stride);

void *insert_array_map(array_map *arr, uint64_t key);

void *get_at_index_array_map(array_map *arr, uint32_t index);

void *look_up_array_map(array_map *arr, uint64_t key);

