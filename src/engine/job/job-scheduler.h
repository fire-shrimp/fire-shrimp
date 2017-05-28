#pragma once

#include <stdatomic.h>
#include <stdint.h>

#include "config.h"
#include "job-queue.h"
#include "tinycthread.h"

typedef struct job_item job_item;

typedef struct job_scheduler {
    thrd_t threads[MAX_WORKER_THREADS];
    uint32_t num_threads;
    job_queue queues[MAX_WORKER_THREADS + 1];
    atomic_bool join_workers;
} job_scheduler;

void init_scheduler(job_scheduler *scheduler, uint32_t num_threads);

void submit_job(job_scheduler *scheduler, job_item *job);

void wait_for_job(job_scheduler *scheduler, job_item *job);

void sync_threads(job_scheduler *scheduler);

void join_threads(job_scheduler *scheduler);
