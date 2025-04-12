#ifndef CORE_H
#define CORE_H


#include <stdbool.h>  // Redundant in C23+
#include <stdint.h>
#include <stdlib.h>


typedef struct {
    uint32_t start;
    uint32_t end;
    uint32_t capacity;
    int *items;
} Queue;


bool queue_init(Queue *queue, uint32_t capacity);
bool queue_pop(Queue *queue, int *value);
bool enqueue(Queue *queue, int value);
bool queue_front(Queue *queue, int *value);
uint32_t queue_length(Queue *queue);
void queue_clear(Queue *queue);
void queue_free(Queue *queue);


#endif  /* CORE_H */
