#include "PacMan.h"
#include "raylib.h"

PacMan::PacMan(int startX, int startY) : Entity(startX, startY), score(0) {}

void PacMan::draw(int tileSize) const {
    DrawCircle(x * tileSize + tileSize / 2, y * tileSize + tileSize / 2, tileSize / 2 - 2, YELLOW);
}

void PacMan::addScore(int amount) {
    score += amount;
}

int PacMan::getScore() const {
    return score;
}
