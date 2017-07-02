#pragma once

#include <stdatomic.h>
#include <stdint.h>

#include "config.h"

typedef struct job_scheduler job_scheduler;

typedef struct job_item job_item;

typedef void (*job_routine_fn)(job_scheduler *scheduler, job_item *job);

typedef struct job_batch job_batch;

typedef struct job_batch {
    job_item **jobs;
    uint32_t capacity;
    atomic_int open_jobs;
    job_batch *continuations[MAX_CONTINUATIONS];
    atomic_int num_continuations;
} job_batch;

typedef struct job_item {
    job_batch *batch;
    uint64_t id;
    job_routine_fn job_fn;
    void *data;
    //TODO add padding to round to 168 bytes?
} job_item;

job_batch *create_job_batch(uint32_t capacity);

void add_job(job_batch *batch, uint64_t id, job_routine_fn job_fn, void *data);

void add_continuation(job_batch *batch, job_batch *continuation);
