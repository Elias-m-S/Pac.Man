#include "Entity.h"

Entity::Entity(int x, int y) : x(x), y(y) {}

void Entity::move(int dx, int dy) {
    x += dx;
    y += dy;
}

int Entity::getX() const { return x; }
int Entity::getY() const { return y; }
