#pragma once

#include <stdbool.h>
#include <stdint.h>

enum log_channel {
    CORE_LOG = 0,
    MEMORY_LOG = 1,
    RENDERER_LOG = 2,
    ANIMATION_LOG = 3,
    PHYSICS_LOG = 4,
    USER_LOG = 5
};

//"sys:boot"
//"sys:load-level"
//"sys:load-level"
//"sys:quit"

void raise_event(uint64_t evt_id, void *args, size_t args_sz);

void debug_log(enum log_channel channel, const char* format, ...);
