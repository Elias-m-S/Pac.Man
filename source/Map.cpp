
#include "Map.h"
#include <raylib.h>
#include <algorithm>

// Layout: W=wall, C=coin, P=power-up, E=empty tunnel
static const char* level[] = {
    " ################### ",
    " #........#........# ",
    " #o##.###.#.###.##o# ",
    " #.................# ",
    " #.##.#.#####.#.##.# ",
    " #....#...#...#....# ",
    " ####.### # ###.#### ",
    "    #.#   0   #.#    ",
    "#####.# ##=## #.#####",
    "     .  #123#  .     ",
    "#####.# ##### #.#####",
    "    #.#       #.#    ",
    " ####.# ##### #.#### ",
    " #........#........# ",
    " #.##.###.#.###.##.# ",
    " #o.#.....P.....#.o# ",
    " ##.#.#.#####.#.#.## ",
    " #....#...#...#....# ",
    " #.######.#.######.# ",
    " #.................# ",
    " ################### "
};

Map::Map(int width, int height, int tileSize)
    : width(width), height(height), tileSize(tileSize),
      grid(height, std::vector<TileType>(width, COIN)),
      fruitTimer(0.0f), fruitSpawnInterval(15.0f), rng(std::random_device{}())
{
    loadLayout();
}

void Map::loadLayout() {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            char ch = level[y][x];
            switch (ch) {
                case '#': grid[y][x] = WALL;    break;
                case '.': grid[y][x] = COIN;    break;
                case 'o': grid[y][x] = POWERUP; break;
                // tunnels, ghosts, pacman, numbers -> empty
                default:  grid[y][x] = EMPTY;   break;
            }
        }
    }
}

void Map::update(float deltaTime) {
    // Ensure always one fruit on map
    bool fruitExists = false;
    for (int y = 0; y < height && !fruitExists; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == FRUIT) { fruitExists = true; break; }
        }
    }
    if (!fruitExists) {
        spawnFruit();
    }
}

void Map::spawnFruit() {
    std::vector<std::pair<int,int>> coins;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (grid[y][x] == COIN) coins.emplace_back(x,y);
        }
    }
    if (coins.empty()) return;
    std::uniform_int_distribution<size_t> dist(0, coins.size()-1);
    auto [fx, fy] = coins[dist(rng)];
    grid[fy][fx] = FRUIT;
}

void Map::draw() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Vector2 pos{ float(x*tileSize), float(y*tileSize) };
            switch (grid[y][x]) {
                case WALL:
                    DrawRectangle(pos.x, pos.y, tileSize, tileSize, DARKGRAY);
                    break;
                case COIN:
                    DrawCircle(pos.x + tileSize/2, pos.y + tileSize/2, 4, GOLD);
                    break;
                case POWERUP:
                    DrawCircle(pos.x + tileSize/2, pos.y + tileSize/2, 8, SKYBLUE);
                    break;
                case FRUIT:
                    DrawRectangle(pos.x+tileSize/4, pos.y+tileSize/4, tileSize/2, tileSize/2, RED);
                    break;
                default:
                    break;
            }
        }
    }
}

bool Map::isWalkable(int x, int y) const {
    // tunnel wrap
    if (y >= 0 && y < height) {
        if (x < 0) x = width - 1;
        else if (x >= width) x = 0;
    }
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    return grid[y][x] != WALL;
}

bool Map::hasItem(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    return (grid[y][x] == COIN || grid[y][x] == POWERUP || grid[y][x] == FRUIT);
}

int Map::collectItem(int x, int y) {
    if (x < 0 || y < 0 || x >= width || y >= height) return 0;
    TileType& t = grid[y][x];
    int points = 0;
    if (t == COIN) points = 10;
    else if (t == FRUIT) points = 100;
    // POWERUP points = 0; // handled externally
    if (t == COIN || t == FRUIT || t == POWERUP) {
        t = EMPTY;
    }
    return points;
}

bool Map::allCoinsCollected() const {
    for (const auto& row : grid) {
        for (auto t : row) {
            if (t == COIN) return false;
        }
    }
    return true;
}

int Map::getWidth() const { return width; }
int Map::getHeight() const { return height; }
