#include "GreenGhost.h"
#include "Map.h"

GreenGhost::GreenGhost(const Map& map, int startX, int startY)
    : Ghostbase(map, startX, startY, 1)
{
    normalColor = GREEN; //Setzt normale Farbe
}

void GreenGhost::update(float lastTime, const Vector2& pacmanPos) {
    Ghostbase::update(lastTime, pacmanPos);
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