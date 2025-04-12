#include "core.h"

// CIRCULAR QUEUE /////////////////////////////////////////////////////////////

bool queue_init(Queue *queue, uint32_t capacity) {
    queue_free(queue);

    queue->items = (int *) malloc(capacity * sizeof(int));

    // Is queue at specified capacity unable to be allocated on the heap
    if (queue->items == NULL) {
        return false;
    }

    queue->capacity = capacity;
    queue_clear(queue);

    return true;
}

bool queue_pop(Queue *queue, int *value) {
    // Is queue empty
    if (queue->end == queue->start) {
        return false;
    }

    *value = queue->items[queue->start];
    queue->start = (queue->start + 1) % queue->capacity;

    return true;
}

bool enqueue(Queue *queue, int value) {
    // Is queue full
    if ((queue->end + 1) % queue->capacity == queue->start) {
        return false;
    }

    queue->items[queue->end] = value;
    queue->end = (queue->end + 1) % queue->capacity;

    return true;
}

bool queue_front(Queue *queue, int *value) {
    // Is queue empty
    if (queue->end == queue->start) {
        return false;
    }

    *value = queue->items[queue->start];

    return true;
}

uint32_t queue_length(Queue *queue) {
    if (queue->end == queue->start) {
        return 0;
    } else if (queue->end > queue->start) {
        return queue->end - queue->start;
    } else {
        return queue->capacity - queue->start + queue->end;
    }
};

void queue_clear(Queue *queue) {
    queue->start = 0;
    queue->end = 0;
}

void queue_free(Queue *queue) {
    free(queue->items);
}
