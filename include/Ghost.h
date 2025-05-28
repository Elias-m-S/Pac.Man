#pragma once
#include "Entity.h"
#include "Map.h"
#include "PacMan.h"
#include <raylib.h>

enum class GhostState { SCATTER, CHASE };

class Ghost : public Entity {
public:
    Ghost(int startX, int startY, int speed, Color color, Vector2 scatterTarget);
    void update(Map& map, const PacMan& pacman, float deltaTime);
    void draw(int tileSize) const override;

private:
    Color color;
    Vector2 scatterTarget;
    GhostState state;
    float modeTimer;
    const float scatterDuration = 7.0f;
    const float chaseDuration = 20.0f;

    void switchMode();
    Vector2 getTarget(const PacMan& pacman) const;
    void chooseNextDirection(const Map& map, Vector2 target);
};
