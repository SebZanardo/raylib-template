#ifndef CORE_H
#define CORE_H


#include <stdbool.h>  // Redundant for C23+, but including for compatability
#include <stdint.h>
#include <stdlib.h>


////////////////////////////////////////////////////////////////////////////////
// STACK ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DEFINE_TYPED_STACK(type, name)                                         \
    typedef struct {                                                           \
        uint32_t head;                                                         \
        uint32_t capacity;                                                     \
        type *nodes;                                                           \
    } name;                                                                    \
                                                                               \
    bool name##_init(name *stack, uint32_t capacity);                          \
    void name##_free(name *stack);                                             \
    bool name##_append(name *stack, type node);                                \
    bool name##_pop(name *stack, type *node);                                  \
    uint32_t name##_length(name *stack);                                       \
    void name##_clear(name *stack);                                            \
                                                                               \
    bool name##_init(name *stack, uint32_t capacity) {                         \
        name##_free(stack);                                                    \
                                                                               \
        stack->nodes = malloc(capacity * sizeof(type));                        \
                                                                               \
        if (stack->nodes == NULL) return false;                                \
                                                                               \
        name##_clear(stack);                                                   \
        stack->capacity = capacity;                                            \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    void name##_free(name *stack) {                                            \
        free(stack->nodes);                                                    \
        stack->nodes = NULL;                                                   \
    }                                                                          \
                                                                               \
    bool name##_append(name *stack, type node) {                               \
        if (stack->head >= stack->capacity) return false;                      \
                                                                               \
        stack->nodes[stack->head++] = node;                                    \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool name##_pop(name *stack, type *node) {                                 \
        if (stack->head == 0) return false;                                    \
                                                                               \
        *node = stack->nodes[stack->head--];                                   \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    uint32_t name##_length(name *stack) {                                      \
        return stack->head;                                                    \
    }                                                                          \
                                                                               \
    void name##_clear(name *stack) {                                           \
        stack->head = 0;                                                       \
    }                                                                          \


////////////////////////////////////////////////////////////////////////////////
// QUEUE ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DEFINE_TYPED_QUEUE(type, name)                                         \
    typedef struct {                                                           \
        uint32_t head;                                                         \
        uint32_t tail;                                                         \
        uint32_t capacity;                                                     \
        type *nodes;                                                           \
    } name;                                                                    \
                                                                               \
    bool name##_init(name *queue, uint32_t capacity);                          \
    void name##_free(name *queue);                                             \
    bool name##_append(name *queue, type node);                                \
    bool name##_pop(name *queue, type *node);                                  \
    uint32_t name##_length(name *queue);                                       \
    void name##_clear(name *queue);                                            \
                                                                               \
    bool name##_init(name *queue, uint32_t capacity) {                         \
        name##_free(queue);                                                    \
                                                                               \
        queue->nodes = malloc(capacity * sizeof(type));                        \
                                                                               \
        if (queue->nodes == NULL) return false;                                \
                                                                               \
        name##_clear(queue);                                                   \
        queue->capacity = capacity;                                            \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    void name##_free(name *queue) {                                            \
        free(queue->nodes);                                                    \
        queue->nodes = NULL;                                                   \
    }                                                                          \
                                                                               \
    bool name##_append(name *queue, type node) {                               \
        uint32_t next = (queue->tail + 1) % queue->capacity;                   \
                                                                               \
        if (next == queue->head) return false;                                 \
                                                                               \
        queue->nodes[queue->tail] = node;                                      \
        queue->tail = next;                                                    \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool name##_pop(name *queue, type *node) {                                 \
        if (queue->tail == queue->head) return false;                          \
                                                                               \
        *node = queue->nodes[queue->head];                                     \
        queue->head = (queue->head + 1) % queue->capacity;                     \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    uint32_t name##_length(name *queue) {                                      \
        return (queue->tail + queue->capacity - queue->head) % queue->capacity;\
    }                                                                          \
                                                                               \
    void name##_clear(name *queue) {                                           \
        queue->head = 0;                                                       \
        queue->tail = 0;                                                       \
    }                                                                          \


#endif  /* CORE_H */
