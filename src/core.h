#ifndef CORE_H
#define CORE_H


#include <stdbool.h>  // Redundant for C23+, but included for compatability
#include <stdint.h>
#include <stdlib.h>


////////////////////////////////////////////////////////////////////////////////
// MACRO FUNCTIONS /////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


////////////////////////////////////////////////////////////////////////////////
// STACK ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DEFINE_TYPED_STACK(type, name)                                         \
    typedef struct {                                                           \
        uint32_t head;                                                         \
        uint32_t capacity;                                                     \
        type *items;                                                           \
    } name;                                                                    \
                                                                               \
    bool name##_init(name *stack, uint32_t capacity);                          \
    void name##_free(name *stack);                                             \
    bool name##_append(name *stack, type item);                                \
    bool name##_pop(name *stack, type *item);                                  \
    uint32_t name##_length(name *stack);                                       \
    void name##_clear(name *stack);                                            \
                                                                               \
    bool name##_init(name *stack, uint32_t capacity) {                         \
        name##_free(stack);                                                    \
                                                                               \
        stack->items = malloc(capacity * sizeof(type));                        \
                                                                               \
        if (stack->items == NULL) return false;                                \
                                                                               \
        name##_clear(stack);                                                   \
        stack->capacity = capacity;                                            \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    void name##_free(name *stack) {                                            \
        free(stack->items);                                                    \
        stack->items = NULL;                                                   \
    }                                                                          \
                                                                               \
    bool name##_append(name *stack, type item) {                               \
        if (stack->head >= stack->capacity) return false;                      \
                                                                               \
        stack->items[stack->head++] = item;                                    \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool name##_pop(name *stack, type *item) {                                 \
        if (stack->head == 0) return false;                                    \
                                                                               \
        *item = stack->items[stack->head--];                                   \
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
        type *items;                                                           \
    } name;                                                                    \
                                                                               \
    bool name##_init(name *queue, uint32_t capacity);                          \
    void name##_free(name *queue);                                             \
    bool name##_append(name *queue, type item);                                \
    bool name##_pop(name *queue, type *item);                                  \
    uint32_t name##_length(name *queue);                                       \
    void name##_clear(name *queue);                                            \
                                                                               \
    bool name##_init(name *queue, uint32_t capacity) {                         \
        name##_free(queue);                                                    \
                                                                               \
        queue->items = malloc(capacity * sizeof(type));                        \
                                                                               \
        if (queue->items == NULL) return false;                                \
                                                                               \
        name##_clear(queue);                                                   \
        queue->capacity = capacity;                                            \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    void name##_free(name *queue) {                                            \
        free(queue->items);                                                    \
        queue->items = NULL;                                                   \
    }                                                                          \
                                                                               \
    bool name##_append(name *queue, type item) {                               \
        uint32_t next = (queue->tail + 1) % queue->capacity;                   \
                                                                               \
        if (next == queue->head) return false;                                 \
                                                                               \
        queue->items[queue->tail] = item;                                      \
        queue->tail = next;                                                    \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    bool name##_pop(name *queue, type *item) {                                 \
        if (queue->tail == queue->head) return false;                          \
                                                                               \
        *item = queue->items[queue->head];                                     \
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


////////////////////////////////////////////////////////////////////////////////
// GRID ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DEFINE_TYPED_GRID(type, name)                                          \
    typedef struct {                                                           \
        uint64_t capacity;                                                     \
        uint32_t height;                                                       \
        uint32_t width;                                                        \
        type *cells;                                                           \
    } name;                                                                    \
                                                                               \
    bool name##_init(name *grid, uint32_t width, uint32_t height);             \
    void name##_free(name *grid);                                              \
    type name##_get(name *grid, uint32_t x, uint32_t y);                       \
    void name##_set(name *grid, uint32_t x, uint32_t y, type cell);            \
    void name##_clear(name *grid);                                             \
                                                                               \
    bool name##_init(name *grid, uint32_t width, uint32_t height) {            \
        name##_free(grid);                                                     \
                                                                               \
        uint64_t capacity = width * height;                                    \
        grid->cells = malloc(capacity * sizeof(type));                         \
                                                                               \
        if (grid->cells == NULL) return false;                                 \
                                                                               \
        grid->capacity = capacity;                                             \
        grid->width = width;                                                   \
        grid->height = height;                                                 \
                                                                               \
        name##_clear(grid);                                                    \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    void name##_free(name *grid) {                                             \
        free(grid->cells);                                                     \
        grid->cells = NULL;                                                    \
    }                                                                          \
                                                                               \
    type name##_get(name *grid, uint32_t x, uint32_t y) {                      \
        return grid->cells[y * grid->width + x];                               \
    }                                                                          \
                                                                               \
    void name##_set(name *grid, uint32_t x, uint32_t y, type cell) {           \
        grid->cells[y * grid->width + x] = cell;                               \
    }                                                                          \
                                                                               \
    void name##_clear(name *grid) {                                            \
        for (uint64_t i = 0; i < grid->capacity; i++) grid->cells[i] = 0;      \
    }                                                                          \


#endif  /* CORE_H */
