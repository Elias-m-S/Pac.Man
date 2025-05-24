#include "Map.h"

Map::Map(int width, int height) : width(width), height(height) {
    grid.resize(height, std::vector<TileType>(width, COIN));
    loadDefaultLayout();
}

void Map::loadDefaultLayout() {
    // Wände außenrum
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            if (x == 0 || x == width - 1 || y == 0 || y == height - 1) {
                grid[y][x] = WALL;
            }
        }
    }

    // Beispiel-Innenwände
    grid[5][5] = WALL;
    grid[5][6] = WALL;
    grid[6][5] = WALL;

    // Coins bleiben automatisch durch Konstruktor
}

void Map::draw() const {
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Vector2 pos = { x * tileSize, y * tileSize };
            switch (grid[y][x]) {
                case WALL:
                    DrawRectangle(pos.x, pos.y, tileSize, tileSize, DARKGRAY);
                    break;
                case COIN:
                    DrawCircle(pos.x + tileSize / 2, pos.y + tileSize / 2, 4, GOLD);
                    break;
                default:
                    break;
            }
        }
    }
}

bool Map::isWalkable(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    return grid[y][x] != WALL;
}

bool Map::hasCoin(int x, int y) const {
    if (x < 0 || y < 0 || x >= width || y >= height) return false;
    return grid[y][x] == COIN;
}

void Map::collectCoin(int x, int y) {
    if (hasCoin(x, y)) {
        grid[y][x] = EMPTY;
    }
}

bool Map::allCoinsCollected() const {
    for (const auto& row : grid) {
        for (const auto& tile : row) {
            if (tile == COIN) return false;
        }
    }
    return true;
}

int Map::getWidth() const { return width; }
int Map::getHeight() const { return height; }
