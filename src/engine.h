#pragma once

#include <stdbool.h>
#include <stdint.h>

/****** FORWARD DECLARATIONS *******/

typedef struct event event;

enum log_channel {
    CORE_LOG = 0,
    MEMORY_LOG = 1,
    RENDERER_LOG = 2,
    ANIMATION_LOG = 3,
    PHYSICS_LOG = 4,
    USER_LOG = 5
};

event *raise_event();

void debug_log(enum log_channel channel, const char *format, ...);
