
#include <raylib.h>
#include "Game.h"
#include "constants.h"

int main() {
    Game game(MAP_WIDTH, MAP_HEIGHT, TILE_SIZE);
    game.run();
    return 0;
}
