#pragma once
#include "constants.h"
#include "Map.h"
#include "Menu.h"
#include "PacMan.h"
#include "Leaderboard.h"
#include "RedGhost.h"
#include "PinkGhost.h"
#include "GreenGhost.h"
#include "BlueGhost.h"
#include <vector>
#include <string>

enum class GameState { 
    MENU, 
    HOWTO, 
    PLAYING, 
    GAMEOVERL,
    GAMEOVERW, 
    ENTERNAME, 
    LEADERBOARD 
};

class Game {
public:
    Game(int width, int height, int tileSize);
    ~Game();
    void run();

private:
    // Game dimensions and settings
    int mapWidth, mapHeight, tileSize;
    const int maxNameLength = 10;
    
    // Game objects
    Menu menu;
    Map map;
    PacMan pacman;
    RedGhost* redGhost;
    PinkGhost* pinkGhost;
    GreenGhost* greenGhost;
    BlueGhost* blueGhost;
    Leaderboard leaderboard;
    
    // Game state
    GameState state;
    std::string playerName;
    
    // Speed settings
    float pacmanSpeed;
    float ghostSpeed;
    int ghostEatenCount; // Track ghosts eaten in current frightened phase
    
    // Sound effects
    Sound deathSound;
    Sound eatFruitSound;
    Sound eatGhostSound;    // Private methods

    void handleInput();
    void handleMenuInput();
    void handlePlayingInput();
    void handleGameOverInput();
    void handleEnterNameInput();
    void handleLeaderboardInput();
    void handleHowToInput();    void resetGame();
    void update();
    void handlePowerUpAndFruit();    void updateGhosts(float deltaTime);
    void draw(float dt);
    void drawMenu();
    void drawHowTo();
    void drawPlaying();
    void drawEnterName();
    void drawGameOverL(float dt); 
    void drawGameOverW(float dt);   
    void drawLeaderboard();
    void ghostCollision(); // Collision detection between Pac-Man and ghosts
    void handleGhostEaten(Ghostbase* ghost);
    void handlePacmanDeath();
    void initializeAudio(); // Initialize audio system and load sounds
    void setWindowIcon(); // Set window icon
};