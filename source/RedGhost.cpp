#include "RedGhost.h"
#include "Map.h"

// Konstruktor: Hier wird der rote Geist erstellt und bekommt seine Farbe
RedGhost::RedGhost(const Map& map, int startX, int startY, float speed)
    : Ghostbase(map, startX, startY, speed)
{
    normalColor = RED;
}

// Im Chase-Modus verfolgt der rote Geist immer direkt PacMan
Vector2 RedGhost::getTargetTile(const Vector2& pacmanPos) const {
    // Ziel ist einfach die aktuelle Position von PacMan
    return pacmanPos;
}

// Im Scatter-Modus will der rote Geist in die obere rechte Ecke
Vector2 RedGhost::getScatterTarget() const {
    return Vector2{18, 1};
}