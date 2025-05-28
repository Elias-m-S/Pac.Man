#include "BlueGhost.h"

//Dem Geist seine Farbe zuordnen

BlueGhost::BlueGhost(int startX, int startY)
    : Ghostbase(startX, startY, BLUE) {}

//einfache erste Verfolgung
void BlueGhost::update(Map& map, const PacMan& pacman) {
    // Richtung in X bestimmen
    int dx = (pacman.getX() > getX()) ? 1 : (pacman.getX() < getX()) ? -1 : 0;
    // Richtung in Y bestimmen
    int dy = (pacman.getY() > getY()) ? 1 : (pacman.getY() < getY()) ? -1 : 0;

    // Erst versuche horizontale Bewegung, dann vertikale
    if (!canMove(map, dx, 0)) {
        canMove(map, 0, dy);
    }
}
