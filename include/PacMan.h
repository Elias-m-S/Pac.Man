#pragma once
#include "Entity.h"

class PacMan : public Entity {
public:
    PacMan(int startX, int startY);

    void update(class Map& map);  // Bewegung mit Kollision
    void draw(int tileSize) const override;

    void setDesiredDirection(int dx, int dy);
    void addScore(int points);
    int getScore() const;

private:
    int dx, dy;                   // aktuelle Richtung
    int desiredDx, desiredDy;     // gewünschte Richtung
    int score;
};
