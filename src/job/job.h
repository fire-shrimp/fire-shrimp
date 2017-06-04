#pragma once

#include <stdint.h>
#include <stdatomic.h>

#include "config.h"

typedef struct job_scheduler job_scheduler;

typedef struct job_item job_item;

typedef void(*job_routine_fn)(job_scheduler *scheduler, job_item *job);

typedef struct job_item
{
	uint64_t id;
	job_routine_fn job_fn;
	job_item* parent;
	void *data;
	atomic_int open_work_items;
	job_item *continuations [MAX_CONTINUATIONS];
	atomic_int num_continuations;
	//TODO add padding to round to 168 bytes?

} job_item;

job_item* create_job(uint64_t id, job_routine_fn job_fn, job_item *parent, void *data);

void add_continuation(job_item *job, job_item *dependency);
