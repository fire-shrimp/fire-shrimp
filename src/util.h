#pragma once

#include <stdint.h>

#define least_significant_bit(x) _Generic((x),                                                     \
    uint32_t : u32_least_significant_bit,                                                          \
    uint64_t : u64_least_significant_bit)                                                          \
    (x)

int32_t u32_least_significant_bit(uint32_t num);

int32_t u64_least_significant_bit(uint64_t num);

size_t max_size(size_t a, size_t b);

size_t min_size(size_t a, size_t b);

