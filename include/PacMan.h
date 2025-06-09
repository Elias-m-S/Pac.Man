#pragma once
#include "Entity.h"

class PacMan : public Entity {
public:
    PacMan(int startX, int startY, float speed = 1.0);

    void update(class Map& map, float deltaTime);  // Bewegung mit Kollision
    void draw(int tileSize) const override;

    void setDesiredDirection(int dx, int dy);
    void addScore(int points);
    int getScore() const;

private:
    int desiredDirX, desiredDirY; // gewünschte Richtung
    int score;
    float moveTimer;
    float moveInterval;
};