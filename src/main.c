#include <assert.h>
#include "raylib.h"
#include "core.h"


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_CAPTION "raylib template"
#define FPS 0

DEFINE_TYPED_QUEUE(int8_t, Queue)
DEFINE_TYPED_STACK(int8_t, Stack)


const uint8_t NODES = 255;

uint8_t i;
int8_t value;
Stack stack;
Queue queue;


int main(void) {
    Stack_init(&stack, NODES);
    Queue_init(&queue, NODES + 1);

////////////////////////////////////////////////////////////////////////////////
// BASIC UNIT TESTS ////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
    assert(MIN('a', 'b') == 'a');
    assert(MIN(8.0f, -8) == -8);

    assert(MAX('a', 'b') == 'b');
    assert(MAX(8.0f, -8) == 8.0f);

    for (i = 0; i < NODES; i++) {
        assert(Stack_append(&stack, GetRandomValue(INT8_MIN, INT8_MAX)));
        assert(Queue_append(&queue, GetRandomValue(INT8_MIN, INT8_MAX)));
    }
    assert(Stack_length(&stack) == NODES);
    assert(Queue_length(&queue) == NODES);
    for (i = 0; i < NODES; i++) {
        assert(Stack_pop(&stack, &value));
        assert(Queue_pop(&queue, &value));
    }
    assert(Stack_length(&stack) == 0);
    assert(Queue_length(&queue) == 0);

////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CAPTION);
    SetTargetFPS(FPS);
    while (!WindowShouldClose()) {
        // INPUT
        ;

        // UPDATE
        ;

        // RENDER
        BeginDrawing();
        ClearBackground(BLACK);
        DrawFPS(8, 8);
        EndDrawing();
    }

    CloseWindow();

    // DEINITIALISE
    Stack_free(&stack);
    Queue_free(&queue);
}
