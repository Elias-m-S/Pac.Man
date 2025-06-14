#include "GreenGhost.h"
#include "Map.h"
#include <cmath>

// Konstruktor: Hier wird der grüne Geist erstellt und bekommt seine Farbe
GreenGhost::GreenGhost(const Map& map, int startX, int startY, float speed)
    : Ghostbase(map, startX, startY, speed)
{
    normalColor = GREEN; // Setzt die normale Farbe für Clyde
}

// Clyde verfolgt PacMan nur, wenn er weit weg ist. Sonst läuft er in seine Ecke.
Vector2 GreenGhost::getTargetTile(const Vector2& pacmanPos) const {
    // Abstand zu PacMan berechnen
    float distance = sqrt(pow(pacmanPos.x - x, 2) + pow(pacmanPos.y - y, 2));
    if (distance > 5) {
        // Wenn weit weg: PacMan verfolgen
        return pacmanPos;
    } else {
        // Wenn nah dran: in rechte obere Ecke fliehen
        return {18, 1};
    }
}

// Im Scatter-Modus will Clyde in die untere linke Ecke
Vector2 GreenGhost::getScatterTarget() const {
    return Vector2{2, 19};
}