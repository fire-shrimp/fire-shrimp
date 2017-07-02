
#include "script-interface.h"
#include "duktape.h"
#include "mem.h"
#include "allocator.h"

_Thread_local duk_context *glb_ctx;

static void *alloc_script(void *user_data, duk_size_t sz)
{
    return mem_alloc(HEAP_MEM, sz, 0, "script");
}

static void *realloc_script(void *user_data, void *ptr, duk_size_t sz)
{
    return mem_alloc(HEAP_MEM, sz, 0, "script");
}

static void free_script(void *user_data, void *ptr)
{
    //TODO
}

static void script_fatal(void *user_data, const char *msg)
{
    //TODO
}

bool open_script_context()
{
    void *user_data = NULL;
    duk_context *ctx = duk_create_heap_default();
    return (ctx != NULL);
}

bool close_script_context()
{
    duk_destroy_heap(glb_ctx);
    return true;
}
