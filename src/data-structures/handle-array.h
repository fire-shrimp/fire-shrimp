#pragma once

#include <stdbool.h>
#include <stdint.h>

typedef struct handle_array {
    uint32_t *handles;
    uint32_t *indices;
    uint32_t num_handles;
    uint32_t capacity;
} handle_array;

void init_handle_array(handle_array *arr, uint32_t capacity);

void expand_handle_array(handle_array *arr);

uint32_t get_handle(handle_array *arr);

void free_handle(handle_array *arr, uint32_t handle);

bool is_valid_handle(handle_array *arr, uint32_t handle);
