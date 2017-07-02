#include <stdint.h>

#include "config.h"
#include "linear-allocator.h"
#include "macros.h"
#include "mem.h"
#include "sys-allocator.h"

linear_allocator static_allocator;
linear_allocator frame_allocator;
sys_allocator sys;

static allocator_interface* glb_allocators[NUM_ARENAS] = {
    &static_allocator.interface,
    &frame_allocator.interface,
    &sys.interface,
#ifdef DEBUG_BUILD
    &sys.interface,
    &sys.interface,
    &sys.interface
#endif
};

void mem_init()
{
    init_sys_allocator(&sys);

    void *static_mem = sys.interface.alloc_fn(&sys, STATIC_MEM_SZ, 0, "static mem");
    init_linear_allocator(&static_allocator, static_mem,  STATIC_MEM_SZ);

    void *frame_mem = sys.interface.alloc_fn(&sys, FRAME_MEM_SZ, 0, "frame mem");
    init_linear_allocator(&frame_allocator, frame_mem, FRAME_MEM_SZ);
}

void* mem_alloc(enum mem_arena arena, size_t sz, size_t align, const char *tag)
{
    ASSERT(arena >= 0 && arena < NUM_ARENAS, "invalid arena");

    return glb_allocators[arena]->alloc_fn(glb_allocators[arena], sz, align, tag);
}

void mem_free(enum mem_arena arena, void *ptr, size_t align,  const char *tag)
{
    ASSERT(arena >= 0 && arena < NUM_ARENAS, "invalid arena");

    glb_allocators[arena]->free_fn(glb_allocators[arena], ptr, align, tag);
}

void mem_dump()
{
    sys.interface.free_fn(&sys, static_allocator.buf, 0, "static mem" );
    sys.interface.free_fn(&sys, frame_allocator.buf, 0, "frame mem" );
}
