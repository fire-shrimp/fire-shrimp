
#pragma once

#include <stdbool.h>
#include <stdint.h>

// Associative array using open addressing and linear probing.
// Uses 64bit hash, first bit for full / empty bucket, other 63 bits for hash.
// Stores buckets in seperate array to optimise look up operations.
// Duplicate keys not allowed.
// Stores values in place.
// Probably suitable only for small types ( < 128 bytes )
// as anything bigger may get expensive to move.

// REMOVING A DUPLICATE WILL MAKE A HOLE WHICH WILL CAUSE SUBSEQUENT QUERIES TO FAIL!!!!!!!!!!!!!!!
// Solution is to have a REMOVED flag, which is different from FULL or EMPTY
// When searching for, ignore removed buckets. Simples

typedef struct inline_map {
    uint64_t *buckets;
    uint64_t *keys;
    void *values;
    uint32_t capacity;
    uint32_t count;
    size_t stride;
} inline_map;

void init_inline_map(inline_map *map, uint32_t capacity, size_t stride);

bool insert_inline_map(inline_map *map, uint64_t key, void *buf);

bool look_up_inline_map(inline_map *map, uint64_t key, void *buf);

bool remove_inline_map(inline_map *map, uint64_t key);

void rehash_inline_map(inline_map *map, uint32_t capacity);
