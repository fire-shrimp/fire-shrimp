#pragma once

#include "config.h"
#include "pointer-util.h"

#ifdef ASSERTIONS
#define ASSERT(condition, format, ...)                                                             \
    {                                                                                              \
        if (!(!!(condition))) {                                                                    \
            fatal_error(format, __VA_ARGS__);                                                      \
        }                                                                                          \
    }
#else
#define ASSERT(condition)
#endif

#ifdef ERROR_CHECKS
#define CHECK(expression, format, ...)                                                             \
    {                                                                                              \
        if (!(!!(expression))) {                                                                   \
            fatal_error(format, __VA_ARGS__);                                                      \
        }                                                                                          \
    }
#else
#define CHECK(expression)
#endif

#ifdef RESTRICT_UNALIGNED_IO
#define CHECK_ALIGNMENT(ptr, align)                                                                \
    {                                                                                              \
        if(!ptr || !is_aligned(ptr, align)){                                                       \
            fatal_error("unaligned io");                                                           \
        }                                                                                          \
    }
#else
#define CHECK_ALIGNMENT(ptr, align)
#endif

void fatal_error(const char* format, ...);
