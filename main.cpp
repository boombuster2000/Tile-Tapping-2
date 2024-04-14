#include "raylib.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
#define WINDOW_TITLE "Tile Tapping 2"
 
int main(int argc, const char **argv) { 
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
 
    // Main loop
    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(WINDOW_TITLE, SCREEN_WIDTH/2, 100, 50, PURPLE);

        EndDrawing();
    }
    // Cleanup
    CloseWindow();
    return 0;
}