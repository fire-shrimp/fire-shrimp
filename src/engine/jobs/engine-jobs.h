
#pragma once

struct job_scheduler;
struct job_item;

void engine_game_logic(struct job_scheduler *scheduler, struct job_item *job);

void engine_render_front_end(struct job_scheduler *scheduler, struct job_item *job);

void engine_render_back_end(struct job_scheduler *scheduler, struct job_item *job);
