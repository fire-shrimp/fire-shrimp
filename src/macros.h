#pragma once

#include <stdint.h>

#include "config.h"
#include "error.h"

#define ZP_MAKEFOURCC(ch0, ch1, ch2, ch3) (((uint32_t)(ch0) | ((uint32_t)(ch1) << 8) | ((uint32_t)(ch2) << 16) | ((uint32_t)(ch3) << 24)))

#ifdef ASSERTIONS
#define ASSERT(condition, format, ...)                                                             \
    {                                                                                              \
        if (!(!!(condition))) {                                                                    \
            fatal_error(format, __VA_ARGS__);                                                      \
        }                                                                                          \
    }
#else
#define ASSERT(condition) ;
#endif

#ifdef ERROR_CHECKS
#define CHECK(expression, format, ...)                                                             \
    {                                                                                              \
        if (!(!!(expression))) {                                                                   \
            fatal_error(format, __VA_ARGS__);                                                      \
        }                                                                                          \
    }
#else
#define CHECK(expression) ;
#endif

