#pragma once

#include "Ghostbase.h"

// Das ist der blaue Geist (Inky). Er erbt alles von Ghostbase und macht nur sein Zielverhalten anders.
class BlueGhost : public Ghostbase {
public:
    // Konstruktor: Hier wird der blaue Geist erstellt
    BlueGhost(const Map& map, int startX, int startY, float speed);
    // Gibt das Ziel zurück, das der Geist im Chase-Modus ansteuert (bei Inky: komplexes Verhalten)
    Vector2 getTargetTile(const Vector2& pacmanPos) const override;
    // Gibt die Ecke zurück, in die der Geist im Scatter-Modus läuft
    Vector2 getScatterTarget() const override;
};