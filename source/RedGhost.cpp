#include "RedGhost.h"
#include "Map.h"

RedGhost::RedGhost(const Map& map, int startX, int startY, float speed)
    : Ghostbase(map, startX, startY, speed)
{
    normalColor = RED;
    frightenedColor = BLUE;
}

Vector2 RedGhost::getTargetTile(const Vector2& pacmanPos) const {
    //Gibt die Position von Pacman zurück, um ihn zu verfolgen
    return pacmanPos;
}

Vector2 RedGhost::getScatterTarget() const {
    // RedGhost (Blinky) geht zur oberen rechten Ecke im Scatter-Modus
    return Vector2{18, 1};
}