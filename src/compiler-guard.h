
#include <climits>
#include <stdint.h>

#ifdef __STDC_NO_THREADS__
#error "C11 threads not supported"
#endif

#ifdef __STDC_NO_ATOMICS__
#error "C11 atomics not supported"
#endif

#ifdef __STDC_LIB_EXT1__
#error "C11 bounds checking functions not supported"
#endif

_Static_assert(CHAR_BIT == 8, "8 bit bytes required");

_Static_assert(sizeof(uint8_t) * CHAR_BIT, "8 bit uint8_t required");

_Static_assert(sizeof(int8_t) * CHAR_BIT, "8 bit int8_t required");

_Static_assert(sizeof(uint32_t) * CHAR_BIT, "32 bit uint32_t required");

_Static_assert(sizeof(int32_t) * CHAR_BIT, "32 bit int32_t required");

_Static_assert(sizeof(uint64_t) * CHAR_BIT, "64 bit uint64_t required");

_Static_assert(sizeof(int64_t) * CHAR_BIT, "64 bit int64_t required");

_Static_assert(sizeof(float) * CHAR_BIT == 32, "size of float required to be 32 bits");

_Static_assert(sizeof(double) * CHAR_BIT == 64, "size of double required to be 64 bits");

