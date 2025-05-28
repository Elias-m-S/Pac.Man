#include "Ghost.h"
#include <cmath>
#include <algorithm>
#include <cfloat>  // for FLT_MAX

Ghost::Ghost(int startX, int startY, int speed, Color color, Vector2 scatterTarget)
    : Entity(startX, startY, speed), color(color), scatterTarget(scatterTarget), state(GhostState::SCATTER), modeTimer(0.0f) {}

void Ghost::update(Map& map, const PacMan& pacman, float deltaTime) {
    modeTimer += deltaTime;
    float modeDuration = (state == GhostState::SCATTER ? scatterDuration : chaseDuration);
    if (modeTimer >= modeDuration) {
        switchMode();
        modeTimer = 0;
    }
    Vector2 target = getTarget(pacman);
    chooseNextDirection(map, target);
    move();
    // Tunnel wrap horizontally for ghosts
    if (x < 0) x = map.getWidth() - 1;
    else if (x >= map.getWidth()) x = 0;
}

void Ghost::switchMode() {
    state = (state == GhostState::SCATTER ? GhostState::CHASE : GhostState::SCATTER);
}

Vector2 Ghost::getTarget(const PacMan& pacman) const {
    if (state == GhostState::SCATTER) return scatterTarget;
    // CHASE: target PacMan's current tile
    return { static_cast<float>(pacman.getX()), static_cast<float>(pacman.getY()) };
}

void Ghost::chooseNextDirection(const Map& map, Vector2 target) {
    struct Dir { int dx, dy; };
    const std::vector<Dir> dirs = {{1,0},{-1,0},{0,1},{0,-1}};
    int bestDx = getDirX(), bestDy = getDirY();
    float bestDist = FLT_MAX;
    // avoid reversing
    int revDx = -getDirX(), revDy = -getDirY();
    for (auto d : dirs) {
        if (d.dx == revDx && d.dy == revDy) continue;
        int nx = getX() + d.dx;
        int ny = getY() + d.dy;
        if (!map.isWalkable(nx, ny)) continue;
        float dist = fabs(nx - target.x) + fabs(ny - target.y);
        if (dist < bestDist) {
            bestDist = dist;
            bestDx = d.dx; bestDy = d.dy;
        }
    }
    setDirection(bestDx, bestDy);
}

void Ghost::draw(int tileSize) const {
    Vector2 pos{ float(getX()*tileSize), float(getY()*tileSize) };
    DrawCircle(pos.x + tileSize/2, pos.y + tileSize/2, tileSize/2 - 2, color);
}