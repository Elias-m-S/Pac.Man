#include "RedGhost.h"
#include "Map.h"

RedGhost::RedGhost(const Map& map, int startX, int startY)
    : Ghostbase(map, startX, startY, 4.0) //geister schneller für besseres gameplay
{
    normalColor = RED; // Setzt die normale Farbe für den RedGhost
    frightenedColor = BLUE; // Farbe wenn verängstigt
}

void RedGhost::update(float lastTime, const Vector2& pacmanPos, const Map& map) {
    // Rufe die Basisklassen-Update Methode auf
    Ghostbase::update(lastTime, pacmanPos, map);
}

Vector2 RedGhost::getTargetTile(const Vector2& pacmanPos) const {
    //Gibt die Position von Pacman zurück, um ihn zu verfolgen
    return pacmanPos;
}