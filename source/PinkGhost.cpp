#include "PinkGhost.h"

//Dem Geist seine Farbe zuordnen

PinkGhost::PinkGhost(int startX, int startY)
    : Ghostbase(startX, startY, PINK) {}

//einfache erste Verfolgung
void PinkGhost::update(Map& map, const PacMan& pacman) {
    // Richtung in X bestimmen
    int dx = (pacman.getX() > getX()) ? 1 : (pacman.getX() < getX()) ? -1 : 0;
    // Richtung in Y bestimmen
    int dy = (pacman.getY() > getY()) ? 1 : (pacman.getY() < getY()) ? -1 : 0;

    // Erst versuche horizontale Bewegung, dann vertikale
    if (!canMove(map, dx, 0)) {
        canMove(map, 0, dy);
    }
}
