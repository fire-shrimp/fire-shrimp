
#pragma once

#include "config.h"
#include "tinycthread.h"

struct job_item;

struct parallel_job_queue
{
	struct job_item **jobs;
	uint32_t max_jobs, bottom, top;
	mtx_t mutex;
};

void job_queue_init(struct parallel_job_queue *queue, uint32_t max_jobs);
void job_queue_reset(struct parallel_job_queue *queue);
void job_queue_push(struct parallel_job_queue *queue, struct job_item *job);
struct job_item *job_queue_pop(struct parallel_job_queue *queue);
struct job_item *job_queue_steal(struct parallel_job_queue *queue);
