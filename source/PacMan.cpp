
#include "PacMan.h"
#include "Map.h"
#include "raylib.h"

PacMan::PacMan(int startX, int startY, float speed)
    : Entity(startX, startY, speed), desiredDirX(0), desiredDirY(0), score(0) {}

void PacMan::update(Map& map) {
    // Richtungswechsel, falls neue Richtung möglich ist
    int testX = x + desiredDirX;
    int testY = y + desiredDirY;
    if (map.isWalkable(testX, testY)) {
        setDirection(desiredDirX, desiredDirY);
    }

    // Bewegung
    int nextX = x + getDirX() * getSpeed();
    int nextY = y + getDirY() * getSpeed();
    if (map.isWalkable(nextX, nextY)) {
        move();
        // Tunnel wrap horizontally
        if (x < 0) x = map.getWidth() - 1;
        else if (x >= map.getWidth()) x = 0;
        // Sammle Item (Coin, Fruit, PowerUp)
        if (map.hasItem(x, y)) {
            int pts = map.collectItem(x, y);
            addScore(pts);
        }
    }
}

void PacMan::draw(int tileSize) const {
    DrawCircle(x * tileSize + tileSize / 2, y * tileSize + tileSize / 2, tileSize / 2 - 2, YELLOW);
}

void PacMan::setDesiredDirection(int dx, int dy) {
    desiredDirX = dx;
    desiredDirY = dy;
}

void PacMan::addScore(int points) {
    score += points;
}

int PacMan::getScore() const {
    return score;
}