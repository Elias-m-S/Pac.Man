#include "BlueGhost.h"
#include "Map.h"
#include <cmath>

BlueGhost::BlueGhost(const Map& map, int startX, int startY, float speed)
    : Ghostbase(map, startX, startY, speed)
{
    normalColor = BLUE; // Setzt die normale Farbe für den BlueGhost
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
    if (distance > 8) {
        // Wenn weit weg: PacMan verfolgen
        return pacmanPos;
    }
}
Vector2 BlueGhost::getScatterTarget() const {
    // BlueGhost (Inky) geht zur unteren rechten Ecke im Scatter-Modus

    return Vector2{18, 19};
}