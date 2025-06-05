#pragma once
#include "PacMan.h"
#include "Ghostbase.h"
#include "raylib.h"

class GreenGhost : public Ghostbase{

    public:
    // Konstruktor für Position und Farbe
    GreenGhost(int startX, int startY);

    //bewegung überschreiben
    void update(Map& map, const PacMan& pacman)override;


};