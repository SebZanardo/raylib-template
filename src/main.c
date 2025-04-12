#include "raylib.h"
#include "core.h"


#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080
#define WINDOW_CAPTION "raylib template"
#define FPS 0

#define NODES 8
#define NODE_SIZE 128
#define NODE_OFFSET 16

#define DATA_STRUCTURES 2

typedef enum {
    STACK,
    QUEUE,
} DataStructures;


DEFINE_TYPED_QUEUE(int8_t, Queue)
DEFINE_TYPED_STACK(int8_t, Stack)


int rx(int i);
void render_stack(void);
void render_queue(void);


int x;
int y = (WINDOW_HEIGHT - (NODE_SIZE + NODE_OFFSET)) / 2;
DataStructures selected;


int8_t value;
Stack stack;
Queue queue;


int main(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CAPTION);
    SetTargetFPS(FPS);

    Stack_init(&stack, NODES);
    Queue_init(&queue, NODES);

    while (!WindowShouldClose()) {
        // INPUT
        bool append = IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
        bool pop = IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);
        bool next = IsKeyPressed(KEY_SPACE);

        // UPDATE
        if (next) {
            selected = (selected + 1) % DATA_STRUCTURES;
        }

        switch (selected) {
            case STACK:
                if (append) {
                    Stack_append(&stack, GetRandomValue(INT8_MIN, INT8_MAX));
                }
                if (pop) {
                    Stack_pop(&stack, &value);
                }
                break;
            case QUEUE:
                if (append) {
                    Queue_append(&queue, GetRandomValue(INT8_MIN, INT8_MAX));
                }
                if (pop) {
                    Queue_pop(&queue, &value);
                }
                break;
        }

        // RENDER
        BeginDrawing();
        ClearBackground(BLACK);

        switch (selected) {
            case STACK:
                render_stack();
                break;
            case QUEUE:
                render_queue();
                break;
        }

        DrawFPS(8, 8);
        EndDrawing();
    }

    CloseWindow();

    // DEINITIALISE
    Stack_free(&stack);
    Queue_free(&queue);
}

int rx(int i) {
    return (NODE_SIZE + NODE_OFFSET) * i + (WINDOW_WIDTH - NODES * (NODE_SIZE + NODE_OFFSET)) / 2;
}

void render_stack(void) {
    DrawText("STACK", 8, 64, 40, WHITE);
    DrawText(TextFormat("head: %d", stack.head), 8, 120, 20, RED);
    DrawText(TextFormat("capacity: %d", stack.capacity), 8, 140, 20, WHITE);

    for (int i = 0; i < stack.capacity; i++) {
        x = rx(i);
        DrawRectangle(x, y, NODE_SIZE, NODE_SIZE, WHITE);

        Color colour = i < stack.head ? BLACK : LIGHTGRAY;
        DrawText(TextFormat("%d", stack.nodes[i]), x + NODE_OFFSET, y + NODE_OFFSET, 40, colour);
    }

    x = rx(stack.head) + NODE_SIZE / 2;
    DrawTriangle(
        (Vector2){x, y - NODE_OFFSET},
        (Vector2){x + NODE_OFFSET, y - NODE_OFFSET * 2},
        (Vector2){x - NODE_OFFSET, y - NODE_OFFSET * 2},
        RED
    );

    x = rx(0);
    DrawText(TextFormat("length: %d", Stack_length(&stack)), x, y - NODE_SIZE, 20, MAGENTA);
}

void render_queue(void) {
    DrawText("QUEUE", 8, 64, 40, WHITE);
    DrawText(TextFormat("head: %d", queue.head), 8, 120, 20, RED);
    DrawText(TextFormat("tail: %d", queue.tail), 8, 140, 20, GREEN);
    DrawText(TextFormat("capacity: %d", queue.capacity), 8, 160, 20, WHITE);

    for (int i = 0; i < queue.capacity; i++) {
        x = rx(i);
        DrawRectangle(x, y, NODE_SIZE, NODE_SIZE, WHITE);

        Color colour;
        if (queue.tail < queue.head) {
            colour = i >= queue.head || i < queue.tail ? BLACK : LIGHTGRAY;
        } else {
            colour = i >= queue.head && i < queue.tail ? BLACK : LIGHTGRAY;
        }
        DrawText(TextFormat("%d", queue.nodes[i]), x + NODE_OFFSET, y + NODE_OFFSET, 40, colour);
    }

    x = rx(queue.head) + NODE_SIZE / 2;
    DrawTriangle(
        (Vector2){x, y - NODE_OFFSET},
        (Vector2){x + NODE_OFFSET, y - NODE_OFFSET * 2},
        (Vector2){x - NODE_OFFSET, y - NODE_OFFSET * 2},
        RED
    );

    x = rx(queue.tail) + NODE_SIZE / 2;
    DrawTriangle(
        (Vector2){x - NODE_OFFSET, y + NODE_SIZE + NODE_OFFSET * 2},
        (Vector2){x + NODE_OFFSET, y + NODE_SIZE + NODE_OFFSET * 2},
        (Vector2){x, y + NODE_SIZE + NODE_OFFSET},
        GREEN
    );

    x = rx(0);
    DrawText(TextFormat("length: %d", Queue_length(&queue)), x, y - NODE_SIZE, 20, MAGENTA);
}
