#include "GreenGhost.h"
#include "Map.h"
#include <cmath>

GreenGhost::GreenGhost(const Map& map, int startX, int startY, float speed)
    : Ghostbase(map, startX, startY, speed)
{
    normalColor = GREEN; //Setzt normale Farbe
}

Vector2 GreenGhost::getTargetTile(const Vector2& pacmanPos) const { // einfacher Algorytmus
    //Verfolgt Pacman nur, solang dieser mehr als 8 tiles entfernt ist,ansonsten flieht er zur Ecke (1, 1)
    float distance = sqrt(pow(pacmanPos.x - x, 2) + pow(pacmanPos.y - y, 2));
    
    if (distance > 5) {
        return pacmanPos; //Verfolgung
    } else {
        return {2, 2}; //Flieht
    }
}

Vector2 GreenGhost::getScatterTarget() const {
    // GreenGhost (Clyde) geht zur unteren linken Ecke im Scatter-Modus
    return Vector2{2, 20};
}