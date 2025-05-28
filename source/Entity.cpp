#include "Entity.h"

Entity::Entity(int x, int y, int speed, int dirX, int dirY)
    : x(x), y(y), speed(speed), dirX(dirX), dirY(dirY) {}

void Entity::move() {
    x += dirX * speed;
    y += dirY * speed;
}

void Entity::setDirection(int dx, int dy) {
    dirX = dx;
    dirY = dy;
}

void Entity::setSpeed(int s) {
    speed = s;
}