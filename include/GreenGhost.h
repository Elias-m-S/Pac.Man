#pragma once

#include "Ghostbase.h"

// Das ist der grüne Geist (Clyde). Er erbt alles von Ghostbase und macht nur sein Zielverhalten anders.
class GreenGhost : public Ghostbase {
public:
    // Konstruktor: Hier wird der grüne Geist erstellt
    GreenGhost(const Map& map, int startX, int startY, float speed);
    // Gibt das Ziel zurück, das der Geist im Chase-Modus ansteuert (bei Clyde: PacMan oder Ecke, je nach Abstand)
    Vector2 getTargetTile(const Vector2& pacmanPos) const override;
    // Gibt die Ecke zurück, in die der Geist im Scatter-Modus läuft
    Vector2 getScatterTarget() const override;
};