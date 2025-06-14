#include "PinkGhost.h"
#include "Map.h"
#include <cmath>

// Konstruktor: Hier wird der pinke Geist erstellt und bekommt seine Farbe
PinkGhost::PinkGhost(const Map& map, int startX, int startY, float speed)
    : Ghostbase(map, startX, startY, speed)
{
    normalColor = PINK; // Setzt die normale Farbe für den RedGhost
}

Vector2 PinkGhost::getTargetTile(const Vector2& pacmanPos) const {
    //der Pinke Geist will eigentlich 4 Felder vor pacman angreifen, da wir aber der einfachheit halber
    //keine richtung sondern nur die position übergeben, wird er einfach 4 Felder vor pacman angreifen, aber nicht basierend auf richtung,
    //sondern von pacman in richtung mitte 4 Felder
    Vector2 target = pacmanPos;
    
    // Mitte der Map
    float mapCenterX = mapRef.getWidth() / 2.0f;
    float mapCenterY = mapRef.getHeight() / 2.0f;
    
    // Berechnung richtung mitte 
    float dirX = (mapCenterX - pacmanPos.x);
    float dirY = (mapCenterY - pacmanPos.y);
    
    // Normalize and extend 3 tiles ahead
    float length = sqrt(dirX * dirX + dirY * dirY);
    if (length > 0.1f) {
        dirX /= length;
        dirY /= length;
        target.x = pacmanPos.x + dirX * 3.0f;
        target.y = pacmanPos.y + dirY * 3.0f;
    }
      // Clamp to map bounds
    target.x = fmax(0, fmin(target.x, mapRef.getWidth() - 1));
    target.y = fmax(0, fmin(target.y, mapRef.getHeight() - 1));
    
    return target;
}

Vector2 PinkGhost::getScatterTarget() const {
    // PinkGhost (Pinky) geht zur oberen linken Ecke im Scatter-Modus
    //Wenn keine Richtung gefunden wurde, soll der Geist umkehren
    return Vector2{2, 2};
}