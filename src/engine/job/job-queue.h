#pragma once

#include "config.h"
#include "tinycthread.h"

typedef struct job_item job_item;

typedef struct job_queue {
    job_item **jobs;
    uint32_t max_jobs, bottom, top;
    mtx_t mutex;
} job_queue;

void init_job_queue(job_queue *queue, uint32_t max_jobs);

void reset_job_queue(job_queue *queue);

void enqueue_job(job_queue *queue, job_item *job);

job_item *dequeue_job(job_queue *queue);

job_item *steal_job(job_queue *queue);
