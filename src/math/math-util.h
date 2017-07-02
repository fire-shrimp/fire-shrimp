
#pragma once

#include <stdint.h>

#define least_significant_bit(x) _Generic((x),                                                     \
    uint32_t : u32_least_significant_bit,                                                          \
    uint64_t : u64_least_significant_bit)                                                          \
    (x)

#define round_up_multiple(x, mod) _Generic((x),                                                    \
    uint32_t : u32_round_up_multiple,                                                              \
    int32_t :  i32_round_up_multiple,                                                              \
    uint64_t : u64_round_up_multiple,                                                              \
    int64_t : i64_round_up_multiple)                                                               \
    (x, mod)

#define round_down_multiple(x, mod) _Generic((x),                                                  \
    uint32_t : u32_round_down_multiple,                                                            \
    int32_t : i32_round_down_multiple,                                                             \
    uint64_t : u64_round_down_multiple,                                                            \
    int64_t : i64_round_down_multiple)                                                             \
    (x, mod)

int32_t u32_least_significant_bit(uint32_t num);

int32_t u64_least_significant_bit(uint64_t num);

uint32_t u32_round_up_multiple(uint32_t x, uint32_t mod);

int32_t i32_round_up_multiple(int32_t x, int32_t mod);

uint64_t u64_round_up_multiple(uint64_t x, uint64_t mod);

int64_t i64_round_up_multiple(int64_t x, int64_t mod);

uint32_t u32_round_down_multiple(uint32_t x, uint32_t mod);

int32_t i32_round_down_multiple(int32_t x, int32_t mod);

uint64_t u64_round_down_multiple(uint64_t x, uint64_t mod);

int64_t i64_round_down_multiple(int64_t x, int64_t mod);

size_t max_size(size_t a, size_t b);

size_t min_size(size_t a, size_t b);

