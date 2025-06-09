#include "RedGhost.h"
#include "Map.h"

RedGhost::RedGhost(const Map& map, int startX, int startY)
    : Ghostbase(map, startX, startY, 0.5) // Speed = 0.5 leicht langsamer wie andere Entitäten
{
    normalColor = RED; // Setzt die normale Farbe für den RedGhost
    frightenedColor = BLUE; // Farbe wenn verängstigt
}

void RedGhost::update(float lastTime, const Vector2& pacmanPos) {
    // Rufe die Basisklassen-Update Methode auf
    Ghostbase::update(lastTime, pacmanPos);
}

Vector2 RedGhost::getTargetTile(const Vector2& pacmanPos) const {
    //Gibt die Position von Pacman zurück, um ihn zu verfolgen
    return pacmanPos;
}