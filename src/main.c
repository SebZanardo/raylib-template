#include "raylib.h"
#include "constants.h"
#include "core.h"


void handle_resize();


WindowParameters window;
bool handheld;


int main(void) {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_CAPTION);
    SetWindowMinSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    SetTargetFPS(WINDOW_FPS);

    InitAudioDevice();
    SetAudioStreamBufferSizeDefault(WINDOW_AUDIO_SAMPLES);

    SetRandomSeed(0);

    #ifdef WEB
        handheld = IsMobile();
    #endif
    handle_resize();

    Image icon = LoadImage("src/resources/icon.png");
    SetWindowIcon(icon);

    RenderTexture2D target = LoadRenderTexture(WINDOW_WIDTH, WINDOW_HEIGHT);

    while (!WindowShouldClose()) {
        /*UpdateMusicStream(music);*/
        if (IsWindowResized()) handle_resize();

        Vector2 mouse_pos = GetMousePosition();

        #ifdef WEB
            if (!handheld) {
                mouse_pos.x *= window.screen_width / WINDOW_WIDTH;
                mouse_pos.y *= window.screen_height / WINDOW_HEIGHT;
            }
        #endif

        BeginTextureMode(target);
        ClearBackground(WHITE);
        EndTextureMode();

        BeginDrawing();
        ClearBackground(DARKGRAY);
        DrawTexturePro(
            target.texture,
            (Rectangle){
                0,
                0,
                (float)target.texture.width,
                (float)-target.texture.height
            },
            (Rectangle){
                window.offset_x,
                window.offset_y,
                (float)target.texture.width * window.scale,
                (float)-target.texture.height * window.scale
            },
            (Vector2){0, 0},
            0.0f,
            WHITE
        );
        DrawCircleV(mouse_pos, 16, MAGENTA);
        EndDrawing();
    }

    UnloadRenderTexture(target);

    CloseAudioDevice();
    CloseWindow();
}


void handle_resize() {
    window.screen_width = GetScreenWidth();
    window.screen_height = GetScreenHeight();
    window.scale = MIN(
        window.screen_width / WINDOW_WIDTH,
        window.screen_height / WINDOW_HEIGHT
    );
    window.offset_x = (window.screen_width - window.scale * WINDOW_WIDTH) / 2;
    window.offset_y = (window.screen_height - window.scale * WINDOW_HEIGHT) / 2;
}
