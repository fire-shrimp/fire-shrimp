#pragma once

#include <stdint.h>
#include <stdbool.h>

#include "data-type.h"

enum sys_events {
    SYS_QUIT,
    SYS_LOAD_LEVEL,
    /* end of native handlers */
    SYS_FRAME,
    NUM_SYS_EVENTS
};

typedef struct event {
    uint32_t id;
    data_buf arg;
} event;

/* number of sys events not handled by an event handler instance*/
#define NUM_NATIVE_HANLDERS (2)
