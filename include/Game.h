#pragma once
#include "Map.h"
#include "PacMan.h"
#include "Leaderboard.h"
#include "RedGhost.h"
#include "PinkGhost.h"
#include "GreenGhost.h"
#include "BlueGhost.h"
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
    RedGhost redGhost;
    PinkGhost pinkGhost;
    GreenGhost greenGhost;
    BlueGhost blueGhost;

    Leaderboard leaderboard;
    GameState state;
    float deltaTime;
    std::string playerName;
    const int maxNameLength = 10;

    void handleInput();
    void update();
    void draw();
};