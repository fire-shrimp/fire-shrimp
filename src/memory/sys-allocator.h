
#pragma once

#include "allocator.h"

typedef struct sys_allocator {
    allocator_interface interface;
} sys_allocator;

void init_sys_allocator(sys_allocator *allocator);

