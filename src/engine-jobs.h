
#pragma once

typedef struct job_scheduler job_scheduler;
typedef struct job_item job_item;

void game_routine(job_scheduler *scheduler, job_item *job);

void render_routine(job_scheduler *scheduler, job_item *job);

void device_routine(job_scheduler *scheduler, job_item *job);
