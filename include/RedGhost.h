#pragma once

#include "Ghostbase.h"

class RedGhost : public Ghostbase {
public:
    RedGhost(const Map& map, int startX, int startY);

    // Hier kannst du später den Bewegungsalgorithmus überschreiben
    void update(float lastTime, const Vector2& pacmanPos) override;
    Vector2 getTargetTile(const Vector2& pacmanPos) const override;

private:
    // spezifische Variablen für RedGhost (z. B. Zielverhalten)


};