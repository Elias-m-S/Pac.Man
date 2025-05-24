#include "PacMan.h"
#include "Map.h"
#include "raylib.h"

PacMan::PacMan(int startX, int startY)
    : Entity(startX, startY), dx(0), dy(0), desiredDx(0), desiredDy(0), score(0) {}

void PacMan::update(Map& map) {
    // Richtungswechsel, falls neue Richtung möglich ist
    int testX = x + desiredDx;
    int testY = y + desiredDy;

    if (map.isWalkable(testX, testY)) {
        dx = desiredDx;
        dy = desiredDy;
    }

    int nextX = x + dx;
    int nextY = y + dy;

    if (map.isWalkable(nextX, nextY)) {
        move(dx, dy);

        if (map.hasCoin(x, y)) {
            map.collectCoin(x, y);
            addScore(10);
        }
    }
}

void PacMan::draw(int tileSize) const {
    DrawCircle(x * tileSize + tileSize / 2, y * tileSize + tileSize / 2, tileSize / 2 - 2, YELLOW);
}

void PacMan::setDesiredDirection(int dx, int dy) {
    desiredDx = dx;
    desiredDy = dy;
}

void PacMan::addScore(int points) {
    score += points;
}

int PacMan::getScore() const {
    return score;
}
