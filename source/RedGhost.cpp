#include "RedGhost.h"
#include "Map.h" // nur wenn du die Map direkt brauchst

RedGhost::RedGhost(/*const Map& map,*/ int startX, int startY)
    : Ghostbase(/*map,*/ startX, startY, speed) // Übergibt Parameter an Ghostbase-Konstruktor
{
    // Falls du weitere Initialisierung brauchst, kommt sie hier hin
}

void RedGhost::update(float lastTime, const Vector2& pacmanPos) {
    // Beispiel: einfacher Bewegungsalgorithmus (z. B. immer nach rechts)
    //tryMove(Direction::Right); // Funktion aus Ghostbase
}

Vector2 RedGhost::getTargetTile(const Vector2& pacmanPos) const {
    // Beispiel: Ziel ist die aktuelle Spielerposition
    // Achtung: Du brauchst eine Referenz auf PacMan im Konstruktor oder eine andere globale Zugriffsmöglichkeit
    return {5.0f, 5.0f};
}

void RedGhost::draw(int tileSize) const {
    DrawCircle(x * tileSize + tileSize / 2, y * tileSize + tileSize / 2, tileSize / 2 - 2, RED);
}