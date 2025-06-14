#pragma once

#include "Ghostbase.h"

// Das ist der rote Geist (Blinky). Er erbt alles von Ghostbase und macht nur sein Zielverhalten anders.
class RedGhost : public Ghostbase {
public:
    // Konstruktor: Hier wird der rote Geist erstellt
    RedGhost(const Map& map, int startX, int startY, float speed);
    // Gibt das Ziel zurück, das der Geist im Chase-Modus ansteuert (bei Blinky: immer PacMan)
    Vector2 getTargetTile(const Vector2& pacmanPos) const override;
    // Gibt die Ecke zurück, in die der Geist im Scatter-Modus läuft
    Vector2 getScatterTarget() const override;
};