#include "PacMan.h"
#include "raylib.h"

PacMan::PacMan(Vector2 pos, float spd) : Entity(pos, spd) {}

void PacMan::update() {
    if (IsKeyDown(KEY_RIGHT)) position.x += speed;
    if (IsKeyDown(KEY_LEFT))  position.x -= speed;
    if (IsKeyDown(KEY_UP))    position.y -= speed;
    if (IsKeyDown(KEY_DOWN))  position.y += speed;
}

void PacMan::draw() const {
    DrawCircleV(position, 10, YELLOW);
}
