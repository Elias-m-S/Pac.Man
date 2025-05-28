#pragma once
#include "Map.h"
#include "PacMan.h"
#include "Ghost.h"
#include "Leaderboard.h"
#include <vector>
#include <string>

enum class GameState { START, PLAYING, GAMEOVER, LEADERBOARD };

class Game {
public:
    Game(int width, int height, int tileSize);
    void run();

private:
    int mapWidth, mapHeight, tileSize;
    Map map;
    PacMan pacman;
    std::vector<Ghost> ghosts;
    Leaderboard leaderboard;
    GameState state;
    float deltaTime;
    std::string playerName;
    const int maxNameLength = 10;

    void handleInput();
    void update();
    void draw();
};