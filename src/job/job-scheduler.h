#pragma once

#include <stdatomic.h>
#include <stdint.h>

#include "allocator.h"
#include "config.h"
#include "job-queue.h"
#include "tinycthread.h"

/* forward declarations */

typedef struct job_scheduler job_scheduler;
typedef struct job_batch job_batch;
typedef struct thrd_start_args thrd_start_args;

typedef struct job_scheduler {
    thrd_t threads[MAX_WORKER_THREADS];
    uint32_t num_threads;
    thrd_start_args *args;
    job_queue queues[MAX_WORKER_THREADS + 1];
    atomic_bool join_workers;
} job_scheduler;

void init_scheduler(job_scheduler *scheduler, uint32_t num_threads);

void submit_batch(job_scheduler *scheduler, job_batch *batch);

void wait_for_batch(job_scheduler *scheduler, job_batch *batch);

void join_threads(job_scheduler *scheduler);
