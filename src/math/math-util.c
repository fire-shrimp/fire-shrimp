
#include "math-util.h"

int32_t u32_least_significant_bit(uint32_t num)
{
    for (uint8_t i = 0; i < 32; i++) {
        if ((1 << i) & num) {
            return i;
        }
    }

    return -1;
}

int32_t u64_least_significant_bit(uint64_t num)
{
    for (uint8_t i = 0; i < 64; i++) {
        if ((1 << i) & num) {
            return i;
        }
    }

    return -1;
}

uint32_t u32_round_up_multiple(uint32_t x, uint32_t mod)
{
    return ((x + mod - 1) / mod) * mod;
}

int32_t i32_round_up_multiple(int32_t x, int32_t mod)
{
    return x >= 0 ? ((x + mod - 1) / mod) * mod : (x / mod) * mod;
}

uint64_t u64_round_up_multiple(uint64_t x, uint64_t mod)
{
    return ((x + mod - 1) / mod) * mod;
}

int64_t i64_round_up_multiple(int64_t x, int64_t mod)
{
    return x >= 0 ? ((x + mod - 1) / mod) * mod : (x / mod) * mod;
}

uint32_t u32_round_down_multiple(uint32_t x, uint32_t mod)
{
    return (x / mod) * mod;
}

int32_t i32_round_down_multiple(int32_t x, int32_t mod)
{
    return x >= 0 ? (x / mod) * mod : ((x - mod + 1) / mod) * mod;
}

uint64_t u64_round_down_multiple(uint64_t x, uint64_t mod)
{
    return (x / mod) * mod;
}

int64_t i64_round_down_multiple(int64_t x, int64_t mod)
{
    return x >= 0 ? (x / mod) * mod : ((x - mod + 1) / mod) * mod;
}

size_t max_size(size_t a, size_t b)
{
    return a > b ? a : b;
}

size_t min_size(size_t a, size_t b)
{
    return a < b ? a : b;
}
