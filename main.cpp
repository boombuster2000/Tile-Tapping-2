#include "raylib.h"
#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Tile Tapping 2"

#define DrawTextWithStuct(textToDraw) DrawText(textToDraw.text, textToDraw.position.x, textToDraw.position.y, textToDraw.fontSize, textToDraw.colour);

struct Text
{
    const char* text;
    int fontSize;
    Color colour;
    Vector2 position;
    bool visible = true;
};

struct Button
{
    Text text;
    bool selected = false;
};

/**
 * Calculates the centre position on the screen from the given text and font size.
 * 
 * @param text The text struct that will be measured.
 * 
 * @return Vector2 of x and y positions.
 * 
 * @warning This function does not calculate true centre for y.
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

    // Define Texts
    Text menuTitle = {WINDOW_TITLE, 100, PURPLE};
    menuTitle.position.x = GetTextCenterPositionOnScreen(menuTitle).x;
    menuTitle.position.y = 50;

    Button playButton = {"Play", 50, PINK};
    playButton.text.position.y = 300;
    playButton.selected = true;

    Button exitButton = {"Exit", 50, PINK};
    exitButton.text.position.y = playButton.text.position.y + 100;


    // Main loop
    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Menu Input Handling
        if (IsKeyPressed(KEY_DOWN))
        {
            if (playButton.selected) {
                playButton.selected = false;
                playButton.text.fontSize = 50;

                exitButton.selected = true;
                exitButton.text.fontSize = 70;
            }
            else 
            {
                exitButton.selected = false;
                exitButton.text.fontSize = 50;

                playButton.selected = true;
                playButton.text.fontSize = 70;
            }
            
        }

        playButton.text.position.x = GetTextCenterPositionOnScreen(playButton.text).x;
        exitButton.text.position.x = GetTextCenterPositionOnScreen(exitButton.text).x;

        //Draw Text
        DrawTextWithStuct(menuTitle);
        DrawTextWithStuct(playButton.text);
        DrawTextWithStuct(exitButton.text);
        EndDrawing();
    }
    // Cleanup
    CloseWindow();
    return 0;
}