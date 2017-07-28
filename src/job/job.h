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
    atomic_int open_batches;
    job_batch *parent;
    job_batch *continuation;
} job_batch;

typedef struct job_item {
    job_batch *batch;
    uint64_t id;
    job_routine_fn job_fn;
    void *data;
} job_item;

job_batch *create_job_batch(uint32_t capacity, job_batch *parent);

void add_job(job_batch *batch, uint64_t id, job_routine_fn job_fn, void *data);

void add_continuation(job_batch *batch, job_batch *continuation);
