
#pragma once

#include <stdatomic.h>
#include <stdint.h>

#include "config.h"
#include "tinycthread.h"
#include "parallel-job-queue.h"

struct job_item;

struct job_scheduler
{
	thrd_t threads[MAX_WORKER_THREADS];
	uint32_t num_threads;
	struct parallel_job_queue queues[MAX_WORKER_THREADS + 1];
	atomic_bool join_workers;
};

void scheduler_init(struct job_scheduler *scheduler, uint32_t num_threads);
void scheduler_submit(struct job_scheduler *scheduler, struct job_item *job);
void scheduler_wait(struct job_scheduler *scheduler, struct job_item *job);
void scheduler_sync(struct job_scheduler *scheduler);
void scheduler_join(struct job_scheduler *scheduler);
