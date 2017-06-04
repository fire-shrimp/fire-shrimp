
#include "util.h"

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

size_t max_size(size_t a, size_t b)
{
    return a > b ? a : b;
}

size_t min_size(size_t a, size_t b)
{
    return a < b ? a : b;
}
