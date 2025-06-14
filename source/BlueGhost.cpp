#include "BlueGhost.h"
#include "Map.h"
#include <cmath>

BlueGhost::BlueGhost(const Map &map, int startX, int startY, float speed)
    : Ghostbase(map, startX, startY, speed)
{
    normalColor = BLUE; // Setzt die normale Farbe für den BlueGhost
}

Vector2 BlueGhost::getTargetTile(const Vector2 &pacmanPos) const
{
    //Anstelle des original Algorythmus von Inky, der sehr komplex ist,
    //wird hier ein vereinfachter Algorithmus verwendet, der Pacman basiert auf die Distanz
    //entweder verfolgt, oder in eine ecke flieht
    // Distanz zu Pacman berechnen
    float distance = sqrt(pow(pacmanPos.x - x, 2) + pow(pacmanPos.y - y, 2));

    // Map Größe für zielsetzung
    int mapWidth = mapRef.getWidth();
    int mapHeight = mapRef.getHeight();
    if (distance > 8)
    {
        // Wenn weit weg: PacMan verfolgen
        return pacmanPos;
    }
    else if (distance > 4)
    {
        // Wenn pacmans Position in der Nähe ist, gehe zu der oberen Linken ecke
        return {2, 2};
    }
    else
    {
        return { //sonst untere rechte ecke
            (float)(mapWidth - 1 - x),
            (float)(mapHeight - 1 - y)};
    }
}

Vector2 BlueGhost::getScatterTarget() const
{
    // BlueGhost (Inky) geht zur unteren rechten Ecke im Scatter-Modus

    return {18, 19};
}