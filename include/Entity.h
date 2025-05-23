#pragma once
#include "raylib.h"

class Entity {
protected:
    Vector2 position;
    float speed;

public:
    Entity(Vector2 pos, float spd);
    virtual ~Entity() = default;

    virtual void update() = 0;       // abstrakt – jede Entity implementiert es selbst
    virtual void draw() const = 0;   // abstrakt – für spätere Darstellung

    Vector2 getPosition() const;
    void setPosition(Vector2 pos);
};
