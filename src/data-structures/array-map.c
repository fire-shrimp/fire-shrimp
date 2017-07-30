#include "array-map.h"
#include "error.h"
#include "math-util.h"
#include "mem.h"

void init_array_map(array_map *arr, uint32_t capacity, size_t stride)
{
    ASSERT(arr, "arr is null");

    arr->values = mem_alloc(STATIC_MEM, capacity * stride, 0, "array_map");
    arr->capacity = capacity;
    arr->count = 0;
    arr->stride = stride;

    //index ~2x capacity of array for increased look up performance
    init_inline_map(&arr->index, sizeof(uint32_t), round_up_multiple(capacity * 2, 2));
}

void *insert_array_map(array_map *arr, uint64_t key)
{
    ASSERT(arr, "arr is null");
    ASSERT(arr->count < arr->capacity, "arr overflow");

    insert_inline_map(&arr->index, key, &arr->count);

    return (uint8_t *)arr->values + (arr->count++) * arr->stride;
}

void *get_at_index_array_map(array_map *arr, uint32_t index)
{
    ASSERT(arr, "arr is null");
    ASSERT(index < arr->capacity, "index invalid");

    return (uint8_t *)arr->values + index * arr->stride;
}

void *look_up_array_map(array_map *arr, uint64_t key)
{
    ASSERT(arr, "arr is null");

    uint32_t index = 0;
    bool success = look_up_inline_map(&arr->index, key, &index);

    ASSERT(success, "index lookup failed");
    ASSERT(index < arr->capacity, "index invalid");

    return (uint8_t *)arr->values + index * arr->stride;
}
