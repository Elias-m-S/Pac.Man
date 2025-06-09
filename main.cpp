
#include <raylib.h>
#include "Game.h"

const int tileSize = 32;
const int mapWidth = 21;
const int mapHeight = 21;

int main() {
    Game game(mapWidth, mapHeight, tileSize);
    game.run();
    return 0;
}
