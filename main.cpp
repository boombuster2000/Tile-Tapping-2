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

struct TextListNode 
{
	Text* text;
	TextListNode *next;
	TextListNode() : text(nullptr), next(nullptr) {}
	TextListNode(Text* x) : text(x), next(nullptr) {}
	TextListNode(Text* x, TextListNode *next) : text(x), next(next) {}
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

    Text playButton = {"Play", 70, PINK};
    playButton.position.x = GetTextCenterPositionOnScreen(playButton).x;
    playButton.position.y = 300;

    Text exitButton = {"Exit", 50, PINK};
    exitButton.position.x = GetTextCenterPositionOnScreen(exitButton).x;
    exitButton.position.y = playButton.position.y + 100;

    // Circular Linked List
    TextListNode playButtonNode = {&playButton};
    TextListNode exitButtonNode = {&exitButton, &playButtonNode};
    playButtonNode.next = &exitButtonNode;
    
    TextListNode* menuHead = &playButtonNode;

    // Main loop
    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Menu Input Handling
        if (IsKeyPressed(KEY_DOWN))
        {
            menuHead->text->fontSize = 50;
            menuHead->text->position.x = GetTextCenterPositionOnScreen(*(menuHead->text)).x;

            menuHead = menuHead->next;
            
            menuHead->text->fontSize = 70;
            menuHead->text->position.x = GetTextCenterPositionOnScreen(*(menuHead->text)).x;
        }

        //Draw Text
        DrawTextWithStuct(menuTitle);
        DrawTextWithStuct(playButton);
        DrawTextWithStuct(exitButton);
        EndDrawing();
    }
    // Cleanup
    CloseWindow();
    return 0;
}