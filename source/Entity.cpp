#include "Entity.h"
#include "Map.h"
#include "constants.h"

Entity::Entity(int x, int y, float speed, float dirX, float dirY)
    : x(x), y(y), speed(speed), dirX((int)dirX), dirY((int)dirY) {}

void Entity::setDirection(int dx, int dy) {
        dirX = dx;
        dirY = dy;
    }

void Entity::move() {
    x += dirX;
    y += dirY;
}


void Entity::setSpeed(int s) {
    speed = s;
}

void Entity::handleTunnelWrap(int mapWidth) {
    // Entity coordinates are in tiles, handle integer coordinates
    if (x < 0) {
        x = mapWidth - 1;
    } else if (x >= mapWidth) {
        x = 0;
    }
}