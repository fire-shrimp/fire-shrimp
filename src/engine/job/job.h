#pragma once

#include <stdint.h>
#include <stdatomic.h>

#include "config.h"

struct job_scheduler;
struct job_item;

typedef void(*job_routine_fn)(struct job_scheduler *scheduler, struct job_item *job);

struct job_item
{
	uint64_t id;
	job_routine_fn job_fn;
	struct job_item* parent;
	void *data;
	atomic_int open_work_items;
	struct job_item *continuations [MAX_CONTINUATIONS];
	atomic_int num_continuations;
	//TODO add padding to round to 168 bytes?
};

struct job_item* job_create(uint64_t id, job_routine_fn job_fn, struct job_item *parent, void *data);
void job_add_continuation(struct job_item *job, struct job_item *dependency);
