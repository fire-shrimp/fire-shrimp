
#pragma once

#include <stdint.h>

#include "inline-map.h"

enum mem_arena;

typedef struct static_handle_array {
    void *values;
    uint32_t capacity;
    uint32_t count;
    size_t stride;
    inline_map index;
} static_handle_array;

void init_static_handle_array(static_handle_array *arr, uint32_t capacity, size_t stride);

void *insert_static_handle_array(static_handle_array *arr, uint64_t handle);

void *get_at_index_static_handle_array(static_handle_array *arr, uint32_t index);

void *look_up_static_handle_array(static_handle_array *arr, uint64_t handle);

