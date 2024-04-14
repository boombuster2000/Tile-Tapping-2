#include "raylib.h"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Tile Tapping 2"

struct Text
{
    const char* text;
    int fontSize;
    Color colour;
    Vector2 position;
};


/**
 * Calculates the centre position on the screen from the given text and font size.
 * 
 * @param text The text struct that will be measured.
 * 
 * @return Vector2 of x and y positions.
*/
Vector2 GetTextCenterPositionOnScreen(const Text text)
{
    const int textWidth = MeasureText(text.text, text.fontSize);
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

        Text menuTitle = {WINDOW_TITLE, 50, PURPLE};
        menuTitle.position.x = GetTextCenterPositionOnScreen(menuTitle).x;
        menuTitle.position.y = 10;

        DrawText(menuTitle.text, menuTitle.position.x, menuTitle.position.y, menuTitle.fontSize, menuTitle.colour);

        EndDrawing();
    }
    // Cleanup
    CloseWindow();
    return 0;
}