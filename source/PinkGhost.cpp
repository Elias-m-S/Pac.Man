#include "PinkGhost.h"
#include "Map.h"
#include <cmath>

PinkGhost::PinkGhost(const Map& map, int startX, int startY)
    : Ghostbase(map, startX, startY, 1) // Speed = 1 wie andere Entitäten
{
    normalColor = PINK; // Setzt die normale Farbe für den RedGhost
    frightenedColor = BLUE; // Farbe wenn verängstigt
}

void PinkGhost::update(float lastTime, const Vector2& pacmanPos, const Map& map) {
    // Rufe die Basisklassen-Update Methode auf
    Ghostbase::update(lastTime, pacmanPos, map);
}

Vector2 PinkGhost::getTargetTile(const Vector2& pacmanPos) const {
    // PinkGhost (Speedy) ambushes 4 tiles ahead of PacMan's current direction
    // We need to get PacMan's direction, but since we only have position,
    // we'll implement a simpler ambush strategy targeting a point ahead of PacMan
    
    // Try to target 4 tiles ahead in the direction PacMan is likely moving
    // Since we don't have direct access to PacMan's direction, we'll target
    // slightly ahead based on the map center direction
    Vector2 target = pacmanPos;
    
    // Simple ambush: target 3 tiles ahead towards map center
    float mapCenterX = mapRef.getWidth() / 2.0f;
    float mapCenterY = mapRef.getHeight() / 2.0f;
    
    // Calculate direction from pacman towards center
    float dirX = (mapCenterX - pacmanPos.x);
    float dirY = (mapCenterY - pacmanPos.y);
    
    // Normalize and extend 3 tiles ahead
    float length = sqrt(dirX * dirX + dirY * dirY);
    if (length > 0.1f) {
        dirX /= length;
        dirY /= length;
        target.x = pacmanPos.x + dirX * 3.0f;
        target.y = pacmanPos.y + dirY * 3.0f;
    }
    
    // Clamp to map bounds
    target.x = fmax(0, fmin(target.x, mapRef.getWidth() - 1));
    target.y = fmax(0, fmin(target.y, mapRef.getHeight() - 1));
    
    return target;
}