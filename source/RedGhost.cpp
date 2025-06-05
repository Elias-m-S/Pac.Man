#include "RedGhost.h"

//Dem Geist seine Farbe zuordnen

RedGhost::RedGhost(int startX, int startY)
    : Ghostbase(startX, startY, RED) {}

//einfache erste Verfolgung
void RedGhost::update(Map& map, const PacMan& pacman) {
    // Richtung in X bestimmen
    int dx = (pacman.getX() > getX()) ? 1 : (pacman.getX() < getX()) ? -1 : 0;
    // Richtung in Y bestimmen
    int dy = (pacman.getY() > getY()) ? 1 : (pacman.getY() < getY()) ? -1 : 0;

    // Erst versuche horizontale Bewegung, dann vertikale
    if (!canMove(map, dx, 0)) {
        canMove(map, 0, dy);
    }
}
