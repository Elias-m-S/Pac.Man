#pragma once
#include "Entity.h"

class PacMan : public Entity {
public:
    PacMan(int startX, int startY, float speed = 1.0);

    void update(class Map& map, float deltaTime);  // Bewegung mit Kollision
    void draw(int tileSize) const override;    void setDesiredDirection(int dx, int dy);
    void addScore(int points);
    int getScore() const;
    bool justAteFruit() const; // Check if fruit was eaten this frame
    bool justAtePowerUp() const; // Check if power-up was eaten this frame

private:
    int desiredDirX, desiredDirY; // gewünschte Richtung
    int score;
    float moveTimer;
    float moveInterval;
    bool fruitEaten; // Flag to track if fruit was eaten this frame
    bool powerUpEaten; // Flag to track if power-up was eaten this frame
};