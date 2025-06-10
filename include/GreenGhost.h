#pragma once

#include "Ghostbase.h"

class GreenGhost : public Ghostbase {
public:    GreenGhost(const Map& map, int startX, int startY, float speed);

    Vector2 getTargetTile(const Vector2& pacmanPos) const override;
    Vector2 getScatterTarget() const override;

private:
    // spezifische Variablen für RedGhost (z.B. Zielverhalten)


};