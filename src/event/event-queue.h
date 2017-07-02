
#pragma once

#include <stdint.h>

typedef struct event {
    uint32_t id;
    void *args;
    size_t args_sz;
} event;

typedef struct event_queue {
    // event *evts;
    // uint32_t capacity;
    // //head
    // //tail
} event_queue;

void init_event_queue(event_queue *queue, uint32_t capacity);

void queue_event(event_queue *queue, uint64_t id, void *args, size_t args_sz);

event *dequeue_event(event_queue *queue);

void close_frame(event_queue *queue);
