#include "raylib.h"
#include "vector"
#include <random>
#include <set>
#include <stdexcept>

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
        bool visible = true;
    };
    
    Text m_tilesTappedText = {
        "0",
        75,
        PURPLE,
        {900, 200},
        true
    };

    Text m_gameOverMessage = {
        "Game Over! Your Score was",
        75,
        PURPLE,
        GetTextCenterPositionOnScreen(m_gameOverMessage),
        true
    };

    std::vector<std::vector<m_tile>> m_tiles;
    bool m_isGameRunning = false;
    bool m_missedTileThisFrame = false;
    bool m_endGame = false;
    int m_tilesTapped = 0;
    const int m_tilesTall = 3;
    const int m_tilesWide = 3;
    std::vector<int> m_invisibleTilesIndexes;
    
    public:
    // Constructor
    Game()
    {
        
    }


    private:
    Vector2 GetGridSize(std::vector<std::vector<m_tile>> m_tiles)
    {
        Vector2 gridSize = {0,0};

        gridSize.x += (m_tiles[0][0].padding_x + m_tiles[0][0].width) * m_tilesWide;
        gridSize.y += (m_tiles[0][0].padding_y + m_tiles[0][0].height) * m_tilesTall;
        
        return gridSize;
    }

    std::vector<int> GetInvisbleTilesIndexes(const int numberOfInvisibleTiles)
    {
        int numberOfTiles = m_tilesWide * m_tilesTall;
        
        if (numberOfTiles <= numberOfInvisibleTiles) throw std::invalid_argument("Number of invisible Tiles can not be larger than number of tiles.");

         // Random number generation setup
        std::random_device rd; // Seed for the random number engine
        std::mt19937 gen(rd()); // Mersenne Twister engine
        std::uniform_int_distribution<> dis(0, numberOfTiles-1); //range, inclusive

        // Set to store unique numbers
        std::set<int> randomIndexes;

        // Generate unique random numbers
        while (randomIndexes.size() < numberOfInvisibleTiles) {
            int num = dis(gen);
            randomIndexes.insert(num);
        }

        std::vector<int> indexes(randomIndexes.begin(), randomIndexes.end());
        return indexes;
    }

    Vector2 Get2DIndexFrom1DIndex(int index)
    {  
        Vector2 indexes;

        indexes.y = (int)index/3;
        indexes.x = index%3;

        return indexes;
    }

    int Get1DIndexFrom2DIndex(Vector2 indexes)
    {
        return (indexes.y*3)+indexes.x;
    }

    public:
    void Start()
    {
        if (m_isGameRunning) return;
        m_invisibleTilesIndexes = GetInvisbleTilesIndexes(3);
        m_endGame = false;

        int iterations = 0;
        for (int y = 0; y<m_tilesTall; y++)
        {
            std::vector<m_tile> row;
            for (int x = 0; x<m_tilesWide; x++)
            {
                row.push_back(m_tile{100, 100, 120, 120, PURPLE});
                iterations++;
            }
            m_tiles.push_back(row);
        }

        for (int index:m_invisibleTilesIndexes)
        {
            Vector2 indexes = Get2DIndexFrom1DIndex(index);
            m_tiles[indexes.y][indexes.x].visible = false;
        }

        m_isGameRunning = true;
    }

    void End()
    {
        m_endGame = true;
    }

    bool IsGameRunning()
    {
        return m_isGameRunning;
    }

    bool MissedTileThisFrame()
    {
        return m_missedTileThisFrame;
    }

    void ProcessKeyPressed()
    {   
        
        if (IsKeyPressed(KEY_ENTER) && m_endGame) m_isGameRunning = false;
        if (m_endGame) return;

        Vector2 tilePressedCoords;
                                                            //x,y
        if (IsKeyPressed(KEY_KP_7)) tilePressedCoords = {0,0};
        else if (IsKeyPressed(KEY_KP_8)) tilePressedCoords = {1,0};
        else if (IsKeyPressed(KEY_KP_9)) tilePressedCoords = {2,0};
        else if (IsKeyPressed(KEY_KP_4)) tilePressedCoords = {0,1};
        else if (IsKeyPressed(KEY_KP_5)) tilePressedCoords = {1,1};
        else if (IsKeyPressed(KEY_KP_6)) tilePressedCoords = {2,1};
        else if (IsKeyPressed(KEY_KP_1)) tilePressedCoords = {0,2};
        else if (IsKeyPressed(KEY_KP_2)) tilePressedCoords = {1,2};
        else if (IsKeyPressed(KEY_KP_3)) tilePressedCoords = {2,2};
        else return; // A key must have been pressed to continue

        if (!m_tiles[tilePressedCoords.y][tilePressedCoords.x].visible)
        {
            m_missedTileThisFrame = true;
            return;
        };

        m_missedTileThisFrame = false;
        m_tiles[tilePressedCoords.y][tilePressedCoords.x].visible = false;
        m_tilesTapped++;
    
        // Random number generation setup
        std::random_device rd; // Seed for the random number engine
        std::mt19937 gen(rd()); // Mersenne Twister engine
        std::uniform_int_distribution<> dis(0, m_invisibleTilesIndexes.size()-1); //range, inclusive
        int indexToSwap = dis(gen);

        Vector2 newVisibleTileIndexes = Get2DIndexFrom1DIndex(m_invisibleTilesIndexes[indexToSwap]);
        m_tiles[newVisibleTileIndexes.y][newVisibleTileIndexes.x].visible = true;
        m_invisibleTilesIndexes[indexToSwap] = Get1DIndexFrom2DIndex(tilePressedCoords);

    }

    void Render()
    {
        if (m_endGame){
            DrawTextWithStuct(m_gameOverMessage);

            m_tilesTappedText.position = {GetTextCenterPositionOnScreen(m_tilesTappedText).x, m_gameOverMessage.position.y + 65};
            m_tilesTappedText.text = std::to_string(m_tilesTapped).c_str();
            DrawTextWithStuct(m_tilesTappedText);
        }
        else
        {       
            Vector2 grid_size = GetGridSize(m_tiles);
            int offset_x = WINDOW_WIDTH/2 - (grid_size.x/4);
            int offset_y = WINDOW_HEIGHT/2 - (grid_size.y/4);

            for (int y = 0; y<m_tiles.size(); y++)
            {
                for (int x = 0; x<m_tiles[y].size(); x++)
                {
                    m_tile tile = m_tiles[y][x];
                    if (!tile.visible) continue;

                    int x_coord = tile.padding_x*x + offset_x;
                    int y_coord = tile.padding_y*y + offset_y;

                    DrawRectangle(x_coord, y_coord, tile.width, tile.height, tile.colour);
                }
            }

            m_tilesTappedText.text = std::to_string(m_tilesTapped).c_str();
            DrawTextWithStuct(m_tilesTappedText);
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
        
        if (!game.IsGameRunning()) mainMenu.Render();
        else 
        {
            if (game.MissedTileThisFrame()) game.End();
            game.ProcessKeyPressed();
            game.Render();
        }

        DrawTextWithStuct(menuTitle);
        EndDrawing();
    }

    // Cleanup
    CloseWindow();
    return 0;
}