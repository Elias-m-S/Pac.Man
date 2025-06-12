#pragma once

#include "Ghostbase.h"

class RedGhost : public Ghostbase {
public:

    RedGhost(const Map& map, int startX, int startY, float speed);
    Vector2 getTargetTile(const Vector2& pacmanPos) const override;
    Vector2 getScatterTarget() const override;
    
    
private:

    
};