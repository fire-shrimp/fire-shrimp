

#pragma once

#define DEBUG_BUILD
#define ASSERTIONS
#define ERROR_CHECKS

// file config
#define MAX_FILE_PATH 4096u
#define MAX_FILE_NAME 256u
#define MAX_FILE_EXTENSION 32u
#define MAX_TEMPLATE_FILE_SIZE (1 << 20) // 1MB

// memory config
#define STATIC_MEM_SZ (1 << 20) // 1MB
#define HEAP_MEM_SZ (1 << 23)   // 8MB
#define FRAME_MEM_SZ (1 << 20)  // 1M

// job config
#define MAX_WORKER_THREADS 31u
#define WORKER_THREADS 7u
#define MAX_JOBS_PER_QUEUE 4096u
#define MAX_CONTINUATIONS 8u
#define JOB_NAME_SIZE 32u

//event config
#define MAX_EVENTS 8192u

// data config
#define UTF8_CHAR_SZ 4u
#define MAX_TITLE_LENGTH 128u
#define MAX_AUTHOR_LENGTH 64u
#define MAX_PROPERTY_LENGTH 64u
#define MAX_SCRIPT_NAME_LENGTH 64u
#define MAX_PORT_LENGTH 32u
#define MAX_FIELD_LENGTH 32u

// logging config
#define MAX_LOGGERS 32u
#define MAX_LOG_FILE_NAME_SIZE 64u
#define MAX_LOG_ENTRY_SIZE 2048u
#define LOG_LEVEL DEBUG_LEVEL

// debug server
#define DEFAULT_DEBUG_PORT 31415
