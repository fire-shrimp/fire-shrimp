#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "inline-map.h"
#include "macros.h"
#include "mem.h"
#include "murmur-hash-2.h"

#define HASH_MASK ~1ul
#define BUCKET_STATE_MASK 1ul
#define EMPTY_BUCKET 0ul
#define FULL_BUCKET 1ul

void init_inline_map(struct inline_map *arr, uint32_t capacity, size_t stride)
{
    LOG_ASSERT(capacity > 2 && capacity % 2 == 0, "invalid capacity");

    size_t bucket_sz = capacity * sizeof(uint64_t);
    size_t key_sz = capacity * sizeof(uint64_t);
    size_t value_sz = capacity * stride;
    size_t total = bucket_sz + key_sz + value_sz;

    void *ptr = MALLOC_HEAP(total, 0, "associative array");

    arr->buckets = ptr;
    arr->keys = (uint64_t *)((uint8_t *)ptr + bucket_sz);
    arr->values = (uint8_t *)ptr + bucket_sz + key_sz;
    arr->capacity = capacity;
    arr->count = 0;
    arr->stride = stride;
}

bool insert_inline_map(struct inline_map *arr, uint64_t key, void *buf)
{
    // resize if size is over load factor
    if (arr->count / (float)arr->capacity > 0.66f) {
        rehash_inline_map(arr, arr->capacity * 2);
    }

    // create hash and find the start bucket
    uint64_t hash = murmur_hash_aligned_2(&key, sizeof(uint64_t), 0) & HASH_MASK;
    uint32_t start = hash % arr->capacity;
    uint32_t target_index = 0;

    // search the buckets until we find an empty one
    for (uint32_t i = 0; i < arr->capacity; i++) {
        target_index = i + start;

        // wrap index
        if (target_index >= arr->capacity) {
            target_index -= arr->capacity;
        }

        // put item in first empty bucket
        if ((arr->buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET) {
            break;
        }

        // check if the key already exists, and fail if it does
        if (arr->buckets[target_index] == (hash | FULL_BUCKET)) {
            if (arr->keys[target_index] == key) {
                return false;
            }
        }
    }

    // array is full, which should never happen
    bool full = (arr->buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET;
    LOG_CHECK(full, "associative array full");

    // insert
    void *dest = ((uint8_t *)arr->values) + target_index * arr->stride;
    void *src = buf;
    memcpy_s(dest, arr->stride, src, arr->stride);

    arr->buckets[target_index] = hash | FULL_BUCKET;
    arr->keys[target_index] = key;
    arr->count++;

    return true;
}

bool look_up_inline_map(struct inline_map *arr, uint64_t key, void *buf)
{
    // create hash and find the start bucket
    uint64_t hash = murmur_hash_aligned_2(&key, sizeof(uint64_t), 0) & HASH_MASK;
    uint32_t start = hash % arr->capacity;

    // search the buckets until we find an empty one
    for (uint32_t i = 0; i < arr->capacity; i++) {
        uint32_t target_index = i + start;

        // wrap index
        if (target_index >= arr->capacity) {
            target_index -= arr->capacity;
        }

        // found an empty bucket, so key doesn't exist
        if ((arr->buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET) {
            return false;
        }

        // check for match, and return value if found
        if (arr->buckets[target_index] == (hash | FULL_BUCKET)) {
            if (arr->keys[target_index] == key) {
                void *dest = buf;
                void *src = ((uint8_t *)arr->values) + target_index * arr->stride;
                memcpy_s(dest, arr->stride, src, arr->stride);
                return true;
            }
        }
    }

    return false;
}

bool remove_inline_map(struct inline_map *arr, uint64_t key)
{
    // create hash and find the start bucket
    uint64_t hash = murmur_hash_aligned_2(&key, sizeof(uint64_t), 0) & HASH_MASK;
    uint32_t start = hash % arr->capacity;

    for (uint32_t i = 0; i < arr->capacity; i++) {
        uint32_t target_index = i + start;

        // wrap index
        if (target_index >= arr->capacity)
            target_index -= arr->capacity;

        // if we find an empty bucket, then search failed
        if ((arr->buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET)
            return false;

        // check for match and remove
        if (arr->buckets[target_index] == (hash | FULL_BUCKET)) {
            if (arr->keys[target_index] == key) {
                arr->buckets[target_index] = 0;
                arr->count--;
                return true;
            }
        }
    }

    return false;
}

void rehash_inline_map(struct inline_map *arr, uint32_t capacity)
{
    LOG_ASSERT(capacity > arr->capacity && capacity % 2 == 0, "invalid capacity");

    // allocate new mem
    size_t bucket_sz = capacity * sizeof(uint64_t);
    size_t key_sz = capacity * sizeof(uint64_t);
    size_t value_sz = capacity * arr->stride;
    size_t total = bucket_sz + key_sz + value_sz;

    void *ptr = MALLOC_HEAP(total, 0, "associative arr");

    uint64_t *buckets = ptr;
    uint64_t *keys = (uint64_t *)((uint8_t *)ptr + bucket_sz);
    uint8_t *values = (uint8_t *)ptr + bucket_sz + key_sz;

    // Walk through all the current elements and insert them into the new buckets
    for (uint32_t i = 0; i < arr->capacity; i++) {
        if ((arr->buckets[i] & BUCKET_STATE_MASK) == EMPTY_BUCKET)
            continue;

        // Find the starting bucket. Have to remove the bucket state flag
        // from the hash first, before we index
        uint32_t start = (arr->buckets[i] & HASH_MASK) % capacity;
        uint32_t target_index = 0;

        for (uint32_t j = 0; j < capacity; j++) {
            target_index = j + start;

            // wrap index
            if (target_index >= arr->capacity)
                target_index -= arr->capacity;

            // put item in first empty bucket
            if ((buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET)
                break;
        }

        // array is full, which should never happen
        bool full = (buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET;
        LOG_CHECK(full, "associative array full");

        // insert
        void *dest = values + target_index * arr->stride;
        void *src = ((uint8_t *)arr->values) + i * arr->stride;
        memcpy_s(dest, arr->stride, src, arr->stride);

        buckets[target_index] = arr->buckets[i] | FULL_BUCKET;
        keys[target_index] = arr->keys[i];
    }

    // delete old memory

    // update array
    arr->buckets = buckets;
    arr->keys = keys;
    arr->values = values;
    arr->capacity = capacity;
}
