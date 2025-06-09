#include "PinkGhost.h"
#include "Map.h"

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
    // RedGhost verfolgt PacMan direkt
    return pacmanPos;
}