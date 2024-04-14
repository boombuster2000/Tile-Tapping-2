#include "raylib.h"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Tile Tapping 2"
 
struct Vector2
{
    int x = 0;
    int y = 0;
};


int main(int argc, const char **argv) 
{ 
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
 
    // Main loop
    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText(WINDOW_TITLE, WINDOW_WIDTH/2, 100, 50, PURPLE);

        EndDrawing();
    }
    // Cleanup
    CloseWindow();
    return 0;
}