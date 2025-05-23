#pragma once
#include "Entity.h"

class PacMan : public Entity {
public:
    PacMan(Vector2 pos, float spd);
    void update() override;
    void draw() const override;
};
