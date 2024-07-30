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
    int m_fontEnlargment = 0;
    std::vector<Text> m_options;

    public:
    int m_currentOptionIndex = 0;

    // Constructor
    Menu(std::vector<Text> options, int fontEnlargment) 
    {   
        m_fontEnlargment = fontEnlargment;
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

class Game
{
    private:
    struct m_tile
    {
        int width, height;
        int padding_x, padding_y;
        Color colour;
    };
    
    std::vector<std::vector<m_tile>> m_tiles;
    bool m_isGameRunning = false;

    public:

    // Constructor
    Game()
    {
        for (int y = 0; y<3; y++)
        {
            std::vector<m_tile> row;
            for (int x = 0; x<3; x++)
            {
                row.push_back(m_tile{100, 100, 20, 20, PURPLE});
            }
            m_tiles.push_back(row);
        }
    }

    void Start()
    {
        m_isGameRunning = true;
    }

    bool isGameRunning()
    {
        return m_isGameRunning;
    }


    void Render()
    {
        int offset_x = 475;
        int offset_y = 250;

        for (int y = 0; y<m_tiles.size(); y++)
        {
            for (int x = 0; x<m_tiles[y].size(); x++)
            {
                m_tile tile = m_tiles[y][x];
                int x_coord = 100*x + tile.padding_x*x + offset_x;
                int y_coord = 100*y + tile.padding_y*y + offset_y;

                DrawRectangle(x_coord, y_coord, tile.width, tile.height, tile.colour);
            }
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

    // Define Classes
    Menu mainMenu({playButton, exitButton}, 20);
    Game game;

    // Main loop
    while(!WindowShouldClose()) 
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        
        // Menu Input Handling

        if (IsKeyPressed(KEY_DOWN)) mainMenu.ChangeToNextOption();
        else if (IsKeyPressed(KEY_UP)) mainMenu.ChangeToPreviousOption();
        else if (IsKeyPressed(KEY_ENTER))
        {
            Text currentOption = mainMenu.GetCurrentOption();
            if (currentOption.text == "Exit") break;
            else if (currentOption.text == playButton.text) {
                game.Start();
            }
        }
        
        if (!game.isGameRunning()) mainMenu.Render();
        else game.Render();


        DrawTextWithStuct(menuTitle);
        EndDrawing();
    }
    // Cleanup
    CloseWindow();
    return 0;
}