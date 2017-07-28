#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "config.h"
#include "error.h"
#include "inline-map.h"
#include "mem.h"
#include "murmur-hash-2.h"

#define LOAD_FACTOR (0.66f)
#define HASH_MASK ~1ul
#define BUCKET_STATE_MASK 1ul
#define EMPTY_BUCKET 0ul
#define FULL_BUCKET 1ul

void init_inline_map(struct inline_map *map, uint32_t capacity, size_t stride)
{
    ASSERT(capacity > 2 && capacity % 2 == 0, "invalid capacity");

    size_t bucket_sz = capacity * sizeof(uint64_t);
    size_t key_sz = capacity * sizeof(uint64_t);
    size_t value_sz = capacity * stride;
    size_t total = bucket_sz + key_sz + value_sz;

    void *ptr = mem_alloc(HEAP_MEM, total, 0, "inline_map");

    map->buckets = ptr;
    map->keys = (uint64_t *)((uint8_t *)ptr + bucket_sz);
    map->values = (uint8_t *)ptr + bucket_sz + key_sz;
    map->capacity = capacity;
    map->count = 0;
    map->stride = stride;
}

bool insert_inline_map(struct inline_map *map, uint64_t key, void *buf)
{
    // resize if size is over load factor
    if (map->count / (float)map->capacity > LOAD_FACTOR) {
        rehash_inline_map(map, map->capacity * 2);
    }

    // create hash and find the start bucket
    uint64_t hash = murmur_hash_aligned_2(&key, sizeof(uint64_t), 0) & HASH_MASK;
    uint32_t start = hash % map->capacity;
    uint32_t target_index = 0;

    // search the buckets until we find an empty one
    for (uint32_t i = 0; i < map->capacity; i++) {
        target_index = i + start;

        // wrap index
        if (target_index >= map->capacity) {
            target_index -= map->capacity;
        }

        // put item in first empty bucket
        if ((map->buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET) {
            break;
        }

        // check if the key already exists, and fail if it does
        if (map->buckets[target_index] == (hash | FULL_BUCKET)) {
            if (map->keys[target_index] == key) {
                return false;
            }
        }
    }

    // map is full, which should never happen
    bool full = (map->buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET;
    CHECK(full, "inline map full");

    void *dest = ((uint8_t *)map->values) + target_index * map->stride;
    void *src = buf;
    errno_t err = memcpy_s(dest, map->capacity * map->stride, src, map->stride);
    CHECK(err == 0, "memcpy_s failed");

    map->buckets[target_index] = hash | FULL_BUCKET;
    map->keys[target_index] = key;
    map->count++;

    return true;
}

bool look_up_inline_map(struct inline_map *map, uint64_t key, void *buf)
{
    // create hash and find the start bucket
    uint64_t hash = murmur_hash_aligned_2(&key, sizeof(uint64_t), 0) & HASH_MASK;
    uint32_t start = hash % map->capacity;

    // search the buckets until we find an empty one
    for (uint32_t i = 0; i < map->capacity; i++) {
        uint32_t target_index = i + start;

        // wrap index
        if (target_index >= map->capacity) {
            target_index -= map->capacity;
        }

        // found an empty bucket, so key doesn't exist
        if ((map->buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET) {
            return false;
        }

        // check for match, and return value if found
        if (map->buckets[target_index] == (hash | FULL_BUCKET)) {
            if (map->keys[target_index] == key) {
                void *dest = buf;
                void *src = ((uint8_t *)map->values) + target_index * map->stride;
                errno_t err = memcpy_s(dest, map->capacity * map->stride, src, map->stride);
                CHECK(err == 0, "memcpy_s failed");
                return true;
            }
        }
    }

    return false;
}

bool remove_inline_map(struct inline_map *map, uint64_t key)
{
    // create hash and find the start bucket
    uint64_t hash = murmur_hash_aligned_2(&key, sizeof(uint64_t), 0) & HASH_MASK;
    uint32_t start = hash % map->capacity;

    for (uint32_t i = 0; i < map->capacity; i++) {
        uint32_t target_index = i + start;

        // wrap index
        if (target_index >= map->capacity)
            target_index -= map->capacity;

        // if we find an empty bucket, then search failed
        if ((map->buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET)
            return false;

        // check for match and remove
        if (map->buckets[target_index] == (hash | FULL_BUCKET)) {
            if (map->keys[target_index] == key) {
                map->buckets[target_index] = 0;
                map->count--;
                return true;
            }
        }
    }

    return false;
}

void rehash_inline_map(struct inline_map *map, uint32_t capacity)
{
    ASSERT(capacity > map->capacity && capacity % 2 == 0, "invalid capacity");

    // allocate new mem
    size_t bucket_sz = capacity * sizeof(uint64_t);
    size_t key_sz = capacity * sizeof(uint64_t);
    size_t value_sz = capacity * map->stride;
    size_t total = bucket_sz + key_sz + value_sz;

    void *ptr = mem_alloc(HEAP_MEM, total, 0, "inline_map rehash");

    uint64_t *buckets = ptr;
    uint64_t *keys = (uint64_t *)((uint8_t *)ptr + bucket_sz);
    uint8_t *values = (uint8_t *)ptr + bucket_sz + key_sz;

    // Walk through all the current elements and insert them into the new buckets
    for (uint32_t i = 0; i < map->capacity; i++) {
        if ((map->buckets[i] & BUCKET_STATE_MASK) == EMPTY_BUCKET)
            continue;

        // Find the starting bucket. Have to remove the bucket state flag
        // from the hash first, before we index
        uint32_t start = (map->buckets[i] & HASH_MASK) % capacity;
        uint32_t target_index = 0;

        for (uint32_t j = 0; j < capacity; j++) {
            target_index = j + start;

            // wrap index
            if (target_index >= map->capacity)
                target_index -= map->capacity;

            // put item in first empty bucket
            if ((buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET)
                break;
        }

        // mapay is full, which should never happen
        bool full = (buckets[target_index] & BUCKET_STATE_MASK) == EMPTY_BUCKET;
        CHECK(full, "inline map full");

        // insert
        void *dest = values + target_index * map->stride;
        void *src = ((uint8_t *)map->values) + i * map->stride;
        errno_t err = memcpy_s(dest, map->capacity * map->stride, src, map->stride);
        CHECK(err == 0, "memcpy_s failed");

        buckets[target_index] = map->buckets[i] | FULL_BUCKET;
        keys[target_index] = map->keys[i];
    }

    void *old_mem = map->buckets;

    // update map
    map->buckets = buckets;
    map->keys = keys;
    map->values = values;
    map->capacity = capacity;

    // delete old memory
    mem_free(HEAP_MEM, old_mem, "inline_map rehash");
}
