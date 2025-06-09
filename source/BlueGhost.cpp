#include "BlueGhost.h"
#include "Map.h"
#include <cmath>

BlueGhost::BlueGhost(const Map& map, int startX, int startY)
    : Ghostbase(map, startX, startY, 1) // Speed = 1 wie andere Entitäten
{
    normalColor = BLUE; // Setzt die normale Farbe für den BlueGhost
}

void BlueGhost::update(float lastTime, const Vector2& pacmanPos, const Map& map) {
    // Rufe die Basisklassen-Update Methode auf
    Ghostbase::update(lastTime, pacmanPos, map);
}

Vector2 BlueGhost::getTargetTile(const Vector2& pacmanPos) const {
    // BlueGhost (Inky) has a complex algorithm based on both PacMan and RedGhost position
    // Since we don't have access to RedGhost position here, we'll implement
    // an alternative complex behavior: patrol between corners and chase
    
    // Calculate distance to PacMan
    float distance = sqrt(pow(pacmanPos.x - x, 2) + pow(pacmanPos.y - y, 2));
    
    // Get map dimensions for corner targeting
    int mapWidth = mapRef.getWidth();
    int mapHeight = mapRef.getHeight();
    
    // Complex behavior: different strategies based on distance and position
    if (distance > 8) {
        // Far away: patrol corners
        Vector2 corners[4] = {
            {0, 0},                           // Top-left
            {(float)(mapWidth-1), 0},         // Top-right
            {0, (float)(mapHeight-1)},        // Bottom-left
            {(float)(mapWidth-1), (float)(mapHeight-1)} // Bottom-right
        };
        
        // Choose corner based on PacMan's quadrant
        int quadrant = 0;
        if (pacmanPos.x > mapWidth/2) quadrant += 1;
        if (pacmanPos.y > mapHeight/2) quadrant += 2;
        
        return corners[quadrant];
    } else if (distance > 4) {
        // Medium distance: try to intercept by targeting ahead of PacMan
        Vector2 target = pacmanPos;
        // Target 2 tiles ahead in a strategic direction
        target.x += (pacmanPos.x > mapWidth/2) ? -2 : 2;
        target.y += (pacmanPos.y > mapHeight/2) ? -2 : 2;
        
        // Clamp to map bounds
        target.x = fmax(0, fmin(target.x, mapWidth - 1));
        target.y = fmax(0, fmin(target.y, mapHeight - 1));
        
        return target;
    } else {
        // Close distance: direct chase
        return pacmanPos;
    }
}