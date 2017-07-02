#pragma once

#include "config.h"
#include "tinycthread.h"

typedef struct job_item job_item;

typedef struct job_queue {
    job_item **jobs;
    uint32_t capacity;
    uint64_t bottom;
    uint64_t top;
    mtx_t mutex;
} job_queue;

void init_job_queue(job_queue *queue, uint32_t capacity);

void reset_job_queue(job_queue *queue);

void enqueue_jobs(job_queue *queue, job_item **jobs, uint32_t num_jobs);

job_item *dequeue_job(job_queue *queue);

job_item *steal_job(job_queue *queue);
