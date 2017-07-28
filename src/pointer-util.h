#pragma once

#include <stdbool.h>
#include <stdint.h>

//aligns ptr to next boundary. Stores adjusment in byte before adjusted pointer
void *align_pointer(void *ptr, size_t align);

//assumes that ptr was first aligned with align_pointer
void *unalign_pointer(void *ptr);

bool is_aligned(void *ptr, size_t align);
