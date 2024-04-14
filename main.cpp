#include "raylib.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
 
int main(int argc, const char **argv) {
    // Initialization
    const char *windowTitle = "Tile Tapping 2";
 
    InitWindow(SCREEN_HEIGHT, SCREEN_WIDTH, windowTitle);
    SetTargetFPS(60);
 
    // Main loop
    while(!WindowShouldClose()) {
 

    }
    // Cleanup
    CloseWindow();
     
    return 0;
}