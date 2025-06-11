#pragma once
#include <vector>
#include <random>
#include <raylib.h>

class Map {
public:
    enum TileType { WALL, EMPTY, COIN, POWERUP, FRUIT };

    // Construct map with given dimensions and tile size
    Map(int width, int height, int tileSize);

    // Update map state (e.g., fruit spawning)
    void update(float deltaTime);
    // Draw the map (walls, coins, power-ups, fruit)
    void draw() const;

    // Check if a tile is walkable (includes tunnel wrap-around)
    bool isWalkable(int x, int y) const;

    // Check for items on a tile (coin, power-up, fruit)
    bool hasItem(int x, int y) const;
    // Collect item at tile; returns points earned (10 coin, 100 fruit, 0 power-up)
    int collectItem(int x, int y);

    // Check if all coins have been collected
    bool allCoinsCollected() const;    // Check if a tile contains a fruit
    bool hasFruit(int x, int y) const;
    
    // Check if a tile contains a power-up
    bool hasPowerUp(int x, int y) const;
    
    int getWidth() const;
    int getHeight() const;

private:
    int width;
    int height;
    int tileSize;
    std::vector<std::vector<TileType>> grid;
    std::vector<std::pair<int,int>> validFruitPositions;

    // Timing for fruit spawn
    float fruitTimer;
    float fruitSpawnInterval;
    std::mt19937 rng;

    void loadLayout();
    void spawnFruit();
};
