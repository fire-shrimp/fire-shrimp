#pragma once

#include <stdint.h>
#include <stdlib.h>

#include "config.h"
#include "log.h"

#define MAKEFOURCC(ch0, ch1, ch2, ch3) (((uint32_t)(ch0) | ((uint32_t)(ch1) << 8) | ((uint32_t)(ch2) << 16) | ((uint32_t)(ch3) << 24)))

#ifdef ASSERTIONS
#define ASSERT(condition)                                                                          \
    {                                                                                              \
        if (!(!!(condition))) {                                                                    \
            abort();                                                                               \
        }                                                                                          \
    }

#define LOG_ASSERT(condition, format, ...)                                                         \
    {                                                                                              \
        if (!(!!(condition))) {                                                                    \
            LOG_FATAL(CORE_LOG, format, __VA_ARGS__);                                              \
            abort();                                                                               \
        }                                                                                          \
    }
#else
#define ASSERT(condition) ;
#define LOG_ASSERT(condition, format, ...) ;
#endif

#ifdef ERROR_CHECKS
#define CHECK(expression)                                                                          \
    {                                                                                              \
        if (!(!!(expression))) {                                                                   \
            abort();                                                                               \
        }                                                                                          \
    }

#define LOG_CHECK(expression, format, ...)                                                         \
    {                                                                                              \
        if (!(!!(expression))) {                                                                   \
            LOG_FATAL(CORE_LOG, format, __VA_ARGS__);                                              \
            abort();                                                                               \
        }                                                                                          \
    }
#else
#define CHECK(expression) ;
#define LOG_CHECK(expression, format, ...) ;
#endif

