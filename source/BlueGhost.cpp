#include "BlueGhost.h"
#include "Map.h"

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
    // RedGhost verfolgt PacMan direkt
    return pacmanPos;
}