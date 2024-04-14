#include "raylib.h"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Tile Tapping 2"

/**
 * Calculates the centre position on the screen from the given text and font size.
 * 
 * @param text The text that will be displayed.
 * @param fontSize The fontSize being used for the text.
 * 
 * @return Vector2 of x and y positions.
*/
Vector2 GetTextCenterPositionOnScreen(const char* text, const int fontSize)
{
    const int textWidth = MeasureText(text, fontSize);
    const float offset = textWidth/2;

    return {((float)GetScreenWidth()/2) - offset, (float)GetScreenHeight()/2};
}

int main(int argc, const char **argv) 
{ 
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);
 
    // Main loop
    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        Vector2 TextPosition = GetTextCenterPositionOnScreen(WINDOW_TITLE, 50);
        DrawText(WINDOW_TITLE, TextPosition.x, 0, 50, PURPLE);

        EndDrawing();
    }
    // Cleanup
    CloseWindow();
    return 0;
}