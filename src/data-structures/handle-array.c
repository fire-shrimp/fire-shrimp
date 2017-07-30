#include <stdalign.h>

#include "error.h"
#include "handle-array.h"
#include "mem.h"

void init_handle_array(handle_array *arr, uint32_t capacity)
{
    ASSERT(arr, "arr is null");
    ASSERT(capacity % 2 == 0, "invalid capacity");

    void *mem = mem_alloc(HEAP_MEM, capacity * sizeof(uint32_t) * 2, alignof(uint32_t), "handle_array");

    arr->handles = (uint32_t *)mem;
    arr->indices = (uint32_t *)((uint8_t *)arr->handles + capacity * sizeof(uint32_t));
    arr->num_handles = 0;
    arr->capacity = capacity;

    for (uint32_t i = 0; i < arr->capacity; i++) {
        arr->handles[i] = i;
        arr->indices[i] = 0;
    }
}

void expand_handle_array(handle_array *arr)
{
    ASSERT(arr, "arr is null");

    uint32_t prev_capacity = arr->capacity;
    uint32_t new_capacity = arr->capacity * 2;

    void *new_mem = mem_alloc(HEAP_MEM, new_capacity * sizeof(uint32_t) * 2, alignof(uint32_t), "handle_array");

    uint32_t *new_handles = new_mem;
    uint32_t *new_indices = (uint32_t *)((uint8_t *)new_handles + new_capacity * sizeof(uint32_t));

    //copy old data
    for (uint32_t i = 0; i < prev_capacity; i++) {
        new_handles[i] = arr->handles[i];
        new_indices[i] = arr->indices[i];
    }

    //initialise new data
    for (uint32_t i = prev_capacity; i < new_capacity; i++) {
        new_handles[i] = i;
        new_indices[i] = 0;
    }

    void *old_mem = arr->handles;

    arr->capacity = new_capacity;
    arr->handles = new_handles;
    arr->indices = new_indices;

    mem_free(HEAP_MEM, old_mem, "handle_array");
}

uint32_t get_handle(handle_array *arr)
{
    ASSERT(arr, "arr is null");

    if (arr->num_handles == arr->capacity) {
        expand_handle_array(arr);
    }

    ASSERT(arr->num_handles < arr->capacity, "handle_array overflow");

    uint32_t index = arr->num_handles;
    uint32_t handle = arr->handles[index];
    arr->indices[handle] = index;
    arr->num_handles++;

    return handle;
}

void free_handle(handle_array *arr, uint32_t handle)
{
    ASSERT(arr, "arr is null");
    ASSERT(handle < arr->capacity, "handle >= arr->capacity");

    uint32_t index = arr->indices[handle];

    ASSERT(index < arr->capacity, "index >= capacity");

    arr->num_handles--;
    uint32_t temp = arr->handles[arr->num_handles];
    arr->handles[arr->num_handles] = handle;
    arr->indices[temp] = index;
    arr->handles[index] = temp;
}

bool is_valid_handle(handle_array *arr, uint32_t handle)
{
    ASSERT(arr, "arr is null");

    if (handle >= arr->capacity) {
        return false;
    }

    uint32_t index = arr->indices[handle];

    return index < arr->num_handles && arr->handles[index] == handle;
}
