

#pragma once

#define DEBUG_BUILD
#define ASSERTIONS
#define ERROR_CHECKS

// file config
#define MAX_FILE_PATH 4096
#define MAX_FILE_NAME 256
#define MAX_FILE_EXTENSION 32
#define MAX_TEMPLATE_FILE_SIZE (1 << 20) // 1MB

// memory config
#define STATIC_MEM (1 << 20) // 1MB
#define HEAP_MEM (1 << 23)   // 8MB
#define FRAME_MEM (1 << 20)  // 1M

// job config
#define MAX_WORKER_THREADS 31
#define MAX_JOBS_PER_QUEUE 4096
#define MAX_CONTINUATIONS 8
#define JOB_NAME_SIZE 32

// property config
#define PROPERTY_NAME_SIZE 64
#define FIELD_NAME_SIZE 64

// logging config
#define MAX_LOGGERS 32
#define MAX_LOG_FILE_NAME_SIZE 64
#define MAX_LOG_ENTRY_SIZE 2048
#define LOG_LEVEL DEBUG_LEVEL
