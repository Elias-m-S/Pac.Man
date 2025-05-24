#pragma once
#include <vector>
#include <raylib.h>

class Map {
public:
    Map(int width, int height);
    void loadDefaultLayout(); // Erstmal harter Map-Entwurf
    void draw() const;

    bool isWalkable(int x, int y) const;
    bool hasCoin(int x, int y) const;
    void collectCoin(int x, int y);

    bool allCoinsCollected() const;

    int getWidth() const;
    int getHeight() const;

private:
    enum TileType {
        EMPTY,
        WALL,
        COIN
    };

    std::vector<std::vector<TileType>> grid;
    int width;
    int height;
    int tileSize = 32; // Pixelgröße pro Tile
};
