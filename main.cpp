#include "raylib.h"
#include "vector"
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
        TextListNode* next;
        TextListNode* before;
        TextListNode() : text(nullptr), next(nullptr), before(nullptr) {}
        TextListNode(Text* text) : text(text), next(nullptr), before(nullptr) {}
        TextListNode(Text* text, TextListNode *next) : text(text), next(next), before(nullptr) {}
        TextListNode(Text* text, TextListNode *next, TextListNode* before) : text(text), next(next), before(before) {}
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

class Menu
{
    private:
    int m_currentOptionIndex = 0;
    const int m_fontEnlargment = 0;
    std::vector<Text> m_options;

    public:
    // Constructor
    Menu(std::vector<Text> options, int fontEnlargment) 
    {   
        m_options = options;
        m_options[m_currentOptionIndex].fontSize += fontEnlargment;
    }

    Text GetCurrentOption()
    {
        return m_options[m_currentOptionIndex];
    }

    void ChangeToNextOption()
    {
        m_options[m_currentOptionIndex].fontSize -= m_fontEnlargment;

        if (m_options.size() != m_currentOptionIndex+1) m_currentOptionIndex++;
        else m_currentOptionIndex = 0;
        
        m_options[m_currentOptionIndex].fontSize += m_fontEnlargment;

    }

    void ChangeToPreviousOption()
    {
        m_options[m_currentOptionIndex].fontSize -= m_fontEnlargment;

        if (m_currentOptionIndex-1 < 0) m_currentOptionIndex = m_options.size() - 1;
        else m_currentOptionIndex--;

        m_options[m_currentOptionIndex].fontSize += m_fontEnlargment;
    }

    void Render()
    {
        for (auto option:m_options)
        {
            option.position.x = GetTextCenterPositionOnScreen(option).x;
            DrawTextWithStuct(option);
        }
    }
};


int main(int argc, const char **argv) 
{ 
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
    SetTargetFPS(60);

    // Define Texts
    Text menuTitle = {WINDOW_TITLE, 100, PURPLE};
    menuTitle.position.x = GetTextCenterPositionOnScreen(menuTitle).x;
    menuTitle.position.y = 50;

    Text playButton = {"Play", 50, PINK};
    playButton.position.x = GetTextCenterPositionOnScreen(playButton).x;
    playButton.position.y = 300;

    Text exitButton = {"Exit", 50, PINK};
    exitButton.position.x = GetTextCenterPositionOnScreen(exitButton).x;
    exitButton.position.y = playButton.position.y + 100;

    Menu mainMenu({playButton, exitButton}, 20);
    
    // Main loop
    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Menu Input Handling

        if (IsKeyPressed(KEY_DOWN)) mainMenu.ChangeToNextOption();
        else if (IsKeyPressed(KEY_UP)) mainMenu.ChangeToPreviousOption();
        mainMenu.Render();

        EndDrawing();
    }
    // Cleanup
    CloseWindow();
    return 0;
}