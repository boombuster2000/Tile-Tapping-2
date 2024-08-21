#include "raylib.h"
#include "vector"
#include <random>
#include <set>
#include <stdexcept>
#include <string>

#define WINDOW_WIDTH 1280
#define WINDOW_HEIGHT 720
#define WINDOW_TITLE "Tile Tapping 2"

#define DrawTextWithStuct(textToDraw) DrawText(textToDraw.text.c_str(), textToDraw.position.x, textToDraw.position.y, textToDraw.fontSize, textToDraw.colour);

struct Text
{
    std::string text;
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
 * @warning This function does not calculate true centre for y as can not determine text height.
*/
Vector2 GetTextCenterPositionOnScreen(const Text text)
{
    const int textWidth = MeasureText(text.text.c_str(), text.fontSize);
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
    
    struct m_countdown
{
    private:
    float m_timeLeft = 0;

    public:
    Text text = {"Time Left: ", 50, PINK, {150, 200}, false};

    public: 
    void Set(float duration) //seconds
    {
        m_timeLeft = duration;
    }

    void Update()
    {
        m_timeLeft -= GetFrameTime() * !IsCountdownDone();

        text.text = "Time Left: " + std::to_string((int)m_timeLeft);
    }

    bool IsCountdownDone()
    {
        return m_timeLeft <= 0;
    }
};

    Text m_tilesTappedText = {
        "0",
        75,
        PINK,
        {900, 200},
        true
    };

    Text m_gameOverMessage = {
        "Game Over! Your Score was",
        75,
        PINK,
        GetTextCenterPositionOnScreen(m_gameOverMessage),
        true
    };

    std::vector<std::vector<m_tile>> m_tiles;
    bool m_isGameRunning = false;
    bool m_missedTileThisFrame = false;
    bool m_gameOver = false;
    bool m_gameFrozen = true;
    int m_tilesTapped = 0;
    const int m_gameDuration = 10; // seconds
    const int m_numberOfInvisibleTiles = 3;
    const int m_tilesTall = 3;
    const int m_tilesWide = 3;
    std::vector<Vector2> m_invisibleTilesIndexes;
    m_countdown m_timer;
    

    public:
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

    std::vector<Vector2> GetInvisbleTilesIndexes(const int numberOfInvisibleTiles)
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
            int index = dis(gen);
            randomIndexes.insert(index);
        }

        std::vector<Vector2> indexes;

        for (int randomIndex: randomIndexes) indexes.push_back(Get2DIndexFrom1DIndex(randomIndex));
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

    /**
     * Creates a 2D vector grid of tiles.
     * 
     * @param rows Determines height of grid.
     * 
     * @param columns Determines width of grid.
     * 
     * @param invisibleTiles The indexes of tiles to be invisible.
     * 
     * @return Returns a 2D vector populated with m_tile.
     * 
    */
    std::vector<std::vector<m_tile>> InitialiseTiles(int rows, int columns, std::vector<Vector2> invisibleTiles = {})
    {
        std::vector<std::vector<m_tile>> tiles;
        for (int y = 0; y<rows; y++)
        {
            std::vector<m_tile> row;
            for (int x = 0; x<columns; x++) row.push_back(m_tile{100, 100, 120, 120, PURPLE});
            tiles.push_back(row);
        }

        for (Vector2 index:invisibleTiles) tiles[index.y][index.x].visible = false;
  
        return tiles;
    }


    public:

    /**
     * Initialises all the tiles and sets m_isGameRunning to true which allows the game to run.
     * @note Game must be ended to start a new one.
    */
    void Start()
    {
        if (m_isGameRunning) return;

        m_invisibleTilesIndexes = GetInvisbleTilesIndexes(m_numberOfInvisibleTiles);
        m_tiles = InitialiseTiles(m_tilesTall, m_tilesWide, m_invisibleTilesIndexes);
        
        m_timer.Set(m_gameDuration);
        m_isGameRunning = true;
    }

    void End()
    {
        m_gameOver = true;
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

        if (m_gameOver) return; // Keys won't be processed if game is over

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

        Vector2 newVisibleTileIndexes = m_invisibleTilesIndexes[indexToSwap];
        m_tiles[newVisibleTileIndexes.y][newVisibleTileIndexes.x].visible = true;
        m_invisibleTilesIndexes[indexToSwap] = tilePressedCoords;

    }

    /**
     * Processes game actions.
    */
    void tick()
    {
        if (IsKeyPressed(KEY_ENTER) && m_gameOver) // Ends Game, resets variables.
        {
            m_isGameRunning = false; 
            m_missedTileThisFrame = false;
            m_gameOver = false;
            m_tilesTapped = 0;
            m_invisibleTilesIndexes.clear();
            m_tiles.clear();
            return;
        }

        if (m_gameOver) return;

        ProcessKeyPressed();
        m_timer.Update();

        if (m_timer.IsCountdownDone()) End();
    }

    void Render()
    {
        if (!m_isGameRunning) return;

        if (m_gameOver){
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
            m_tilesTappedText.position = {900, 200};
            DrawTextWithStuct(m_tilesTappedText);
        }

        DrawTextWithStuct(m_timer.text);
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
        
        if (!game.IsGameRunning())
        {
            mainMenu.Render();
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
        }
        else 
        {
            if (game.MissedTileThisFrame()) 
            {
                game.End();
            }
            game.tick();
            game.Render();
        }

        DrawTextWithStuct(menuTitle);
        EndDrawing();
    }

    // Cleanup
    CloseWindow();
    return 0;
}