#ifndef CORE_H
#define CORE_H

#include <stdbool.h>  // Redundant for C23+, but included for compatability
#include <stdint.h>
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////
// TYPEDEFS ////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
typedef int8_t   i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;

typedef uint8_t  u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

////////////////////////////////////////////////////////////////////////////////
// HELPER FUNCTIONS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))


#ifdef WEB
#include <emscripten/emscripten.h>

EM_JS(bool, IsMobile, (), {
    return (
        typeof navigator !== 'undefined' &&
        /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i
            .test(navigator.userAgent)
    );
});
#endif


// https://create.stephan-brumme.com/fnv-hash/
#define FNV1_SEED_32  0x811C9DC5;  // 2166136261
#define FNV1_PRIME_32 0x01000193;  //   16777619

static inline u32 FNV1a_u32a(u32 data) {
    u32 hash = FNV1_SEED_32;

    hash = (((data >>  0) & 0xFF) ^ hash) * FNV1_PRIME_32;
    hash = (((data >>  8) & 0xFF) ^ hash) * FNV1_PRIME_32;
    hash = (((data >> 16) & 0xFF) ^ hash) * FNV1_PRIME_32;
    hash = (((data >> 24) & 0xFF) ^ hash) * FNV1_PRIME_32;

    return hash;
}

////////////////////////////////////////////////////////////////////////////////
// STACK ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DEFINE_TYPED_STACK(type, name)                                         \
    typedef struct {                                                           \
        type *items;                                                           \
        u32 head;                                                              \
        u32 capacity;                                                          \
    } name;                                                                    \
                                                                               \
    static inline bool name##_init(name *stack, u32 capacity);                 \
    static inline void name##_free(name *stack);                               \
    static inline bool name##_append(name *stack, type item);                  \
    static inline bool name##_pop(name *stack, type *item);                    \
    static inline u32 name##_length(name *stack);                              \
    static inline void name##_clear(name *stack);                              \
                                                                               \
    static inline bool name##_init(name *stack, u32 capacity) {                \
        stack->items = (type *) malloc(capacity * sizeof(type));               \
                                                                               \
        if (stack->items == NULL) return false;                                \
                                                                               \
        stack->capacity = capacity;                                            \
        name##_clear(stack);                                                   \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    static inline void name##_free(name *stack) {                              \
        free(stack->items);                                                    \
        stack->items = NULL;                                                   \
    }                                                                          \
                                                                               \
    static inline bool name##_append(name *stack, type item) {                 \
        if (stack->head >= stack->capacity) return false;                      \
                                                                               \
        stack->items[stack->head++] = item;                                    \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    static inline bool name##_pop(name *stack, type *item) {                   \
        if (stack->head == 0) return false;                                    \
                                                                               \
        *item = stack->items[stack->head--];                                   \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    static inline u32 name##_length(name *stack) {                             \
        return stack->head;                                                    \
    }                                                                          \
                                                                               \
    static inline void name##_clear(name *stack) {                             \
        stack->head = 0;                                                       \
    }                                                                          \

////////////////////////////////////////////////////////////////////////////////
// QUEUE ///////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DEFINE_TYPED_QUEUE(type, name)                                         \
    typedef struct {                                                           \
        type *items;                                                           \
        u32 head;                                                              \
        u32 tail;                                                              \
        u32 capacity;                                                          \
    } name;                                                                    \
                                                                               \
    static inline bool name##_init(name *queue, u32 capacity);                 \
    static inline void name##_free(name *queue);                               \
    static inline bool name##_append(name *queue, type item);                  \
    static inline bool name##_pop(name *queue, type *item);                    \
    static inline u32 name##_length(name *queue);                              \
    static inline void name##_clear(name *queue);                              \
                                                                               \
    static inline bool name##_init(name *queue, u32 capacity) {                \
        queue->items = (type *) malloc(capacity * sizeof(type));               \
                                                                               \
        if (queue->items == NULL) return false;                                \
                                                                               \
        queue->capacity = capacity;                                            \
        name##_clear(queue);                                                   \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    static inline void name##_free(name *queue) {                              \
        free(queue->items);                                                    \
        queue->items = NULL;                                                   \
    }                                                                          \
                                                                               \
    static inline bool name##_append(name *queue, type item) {                 \
        u32 next = (queue->tail + 1) % queue->capacity;                        \
                                                                               \
        if (next == queue->head) return false;                                 \
                                                                               \
        queue->items[queue->tail] = item;                                      \
        queue->tail = next;                                                    \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    static inline bool name##_pop(name *queue, type *item) {                   \
        if (queue->tail == queue->head) return false;                          \
                                                                               \
        *item = queue->items[queue->head];                                     \
        queue->head = (queue->head + 1) % queue->capacity;                     \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    static inline u32 name##_length(name *queue) {                             \
        return (queue->tail + queue->capacity - queue->head) % queue->capacity;\
    }                                                                          \
                                                                               \
    static inline void name##_clear(name *queue) {                             \
        queue->head = 0;                                                       \
        queue->tail = 0;                                                       \
    }                                                                          \

////////////////////////////////////////////////////////////////////////////////
// GRID ////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#define DEFINE_TYPED_GRID(type, name)                                          \
    typedef struct {                                                           \
        type *cells;                                                           \
        u64 capacity;                                                          \
        u32 height;                                                            \
        u32 width;                                                             \
    } name;                                                                    \
                                                                               \
    static inline bool name##_init(name *grid, u32 width, u32 height);         \
    static inline void name##_free(name *grid);                                \
    static inline type name##_get(name *grid, u32 x, u32 y);                   \
    static inline void name##_set(name *grid, u32 x, u32 y, type cell);        \
    static inline void name##_fill(name *grid, type cell);                     \
                                                                               \
    static inline bool name##_init(name *grid, u32 width, u32 height) {        \
        u64 capacity = width * height;                                         \
        grid->cells = (type *) malloc(capacity * sizeof(type));                \
                                                                               \
        if (grid->cells == NULL) return false;                                 \
                                                                               \
        grid->capacity = capacity;                                             \
        grid->width = width;                                                   \
        grid->height = height;                                                 \
                                                                               \
        name##_fill(grid, 0);                                                  \
                                                                               \
        return true;                                                           \
    }                                                                          \
                                                                               \
    static inline void name##_free(name *grid) {                               \
        free(grid->cells);                                                     \
        grid->cells = NULL;                                                    \
    }                                                                          \
                                                                               \
    static inline type name##_get(name *grid, u32 x, u32 y) {                  \
        return grid->cells[y * grid->width + x];                               \
    }                                                                          \
                                                                               \
    static inline void name##_set(name *grid, u32 x, u32 y, type cell) {       \
        grid->cells[y * grid->width + x] = cell;                               \
    }                                                                          \
                                                                               \
    static inline void name##_fill(name *grid, type cell) {                    \
        for (u64 i = 0; i < grid->capacity; i++) grid->cells[i] = cell;        \
    }                                                                          \

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

#endif  /* CORE_H */
