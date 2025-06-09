#include "Entity.h"
#include "map.h"
#include "constants.h"

Entity::Entity(int x, int y, float speed, float dirX, float dirY)
    : x(x), y(y), speed(speed), dirX(dirX), dirY(dirY) {}

void Entity::setDirection(int dx, int dy) {
        dirX = dx;
        dirY = dy;
    }

void Entity::move() {
    x += dirX * speed;
    y += dirY * speed;
}


void Entity::setSpeed(int s) {
    speed = s;
}

void Entity::handleTunnelWrap(int mapWidth) {
    int tileX = static_cast<int>(x / TILE_SIZE);
    if (tileX < 0) {
        x = (mapWidth - 1) * TILE_SIZE; // Wrap to the right side
    } else if (tileX >= mapWidth) {
        x = 0;
    }
}