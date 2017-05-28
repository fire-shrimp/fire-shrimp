#pragma once

#include <stdlib.h>

#include "config.h"
#include "log.h"

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
