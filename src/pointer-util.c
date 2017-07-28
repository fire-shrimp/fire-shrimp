#include "pointer-util.h"

#include "error.h"
#include "math-util.h"

void *align_pointer(void *ptr, size_t align)
{
    ASSERT(ptr, "ptr is null");
    ASSERT(align <= 128, "aling > 128");
    ASSERT((align & (align - 1)) == 0, "align not power of 2");

    //0 alignment not supported. Need the extra byte for storing adjustment.
    size_t actual_align = max_size(align, 1);

    uintptr_t address = (uintptr_t)ptr;
    uintptr_t misalign = address & (actual_align - 1);
    ptrdiff_t adjustment = actual_align - misalign;
    uintptr_t aligned_address = address + adjustment;

    ASSERT(adjustment > 0, "adjustment == 0");
    ASSERT(adjustment < 256, "adjustment >= 256");

    //store adjustment for use when freeing memory
    uint8_t *aligned_ptr = (void *)aligned_address;
    aligned_ptr[-1] = adjustment;

    return aligned_ptr;
}

void *unalign_pointer(void *ptr)
{
    //retrieve the adjustment from the preceding byte
    uint8_t *aligned_ptr = ptr;
    ptrdiff_t adjustment = aligned_ptr[-1];

    //adjust address
    uintptr_t aligned_address = (uintptr_t)ptr;
    uintptr_t address = aligned_address - adjustment;

    void *unaligned_ptr = (void *)address;

    return unaligned_ptr;
}

bool is_aligned(void *ptr, size_t align)
{
    return (((uintptr_t)ptr) & (align - 1)) == 0;
}
