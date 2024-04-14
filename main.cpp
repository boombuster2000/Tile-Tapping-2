#include "raylib.h"
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720
 
int main(int argc, const char **argv) {
    // Initialization
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const char *windowTitle = "Tile Tapping 2";
 
    InitWindow(screenWidth, screenHeight, windowTitle);
    SetTargetFPS(60);
 
    // Main loop
    while(!WindowShouldClose()) {
 

    }
    // Cleanup
    CloseWindow();
     
    return 0;
}