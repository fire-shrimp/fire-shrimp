//-----------------------------------------------------------------------------
// MurmurHash2 was written by Austin Appleby, and is placed in the public
// domain. The author hereby disclaims copyright to this source code.

#pragma once

#include <stdint.h>

uint32_t murmur_hash_aligned_2(const void * key, uint32_t len, uint32_t seed);
