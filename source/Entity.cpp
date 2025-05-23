#include "Entity.h"

Entity::Entity(Vector2 pos, float spd)
    : position(pos), speed(spd) {}

Vector2 Entity::getPosition() const {
    return position;
}

void Entity::setPosition(Vector2 pos) {
    position = pos;
}
