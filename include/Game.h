#pragma once
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

enum class GameState { MENU, HOWTO, PLAYING, ENDLESSGAME, GAMEOVER, ENTERNAME, LEADERBOARD };

class Game {
public:
    Game(int width, int height, int tileSize);
    void run();
    void ghostCollision();//funktion ankündigen, um in der cpp kollision zu überprüfen

private:
    int mapWidth, mapHeight, tileSize;
    Menu menu;
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
    void draw(float dt);
};