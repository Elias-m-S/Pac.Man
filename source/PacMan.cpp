
#include "PacMan.h"
#include "Map.h"
#include "raylib.h"

PacMan::PacMan(int startX, int startY, float speed)
    : Entity(startX, startY, speed), desiredDirX(0), desiredDirY(0), score(0),
      moveTimer(0.0f), moveInterval(1.0f / speed) {}

void PacMan::update(Map& map, float deltaTime) {
    moveTimer -= deltaTime;
    
    // Only move when timer expires (discrete tile movement)
    if (moveTimer <= 0.0f) {
        moveTimer = moveInterval; // Reset timer
        
        // Richtungswechsel, falls neue Richtung möglich ist
        int testX = x + desiredDirX;
        int testY = y + desiredDirY;
        if (map.isWalkable(testX, testY)) {
            setDirection(desiredDirX, desiredDirY);
        }
        
        // Bewegung
        int nextX = x + getDirX();
        int nextY = y + getDirY();
        if (map.isWalkable(nextX, nextY)) {
            move();
            // Handle tunnel wrapping using inherited method
            handleTunnelWrap(map.getWidth());
            // Sammle Item (Coin, Fruit, PowerUp)
            if (map.hasItem(x, y)) {
                int pts = map.collectItem(x, y);
                addScore(pts);
            }
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