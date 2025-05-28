#pragma once
#include "PacMan.h"
#include "Ghostbase.h"
#include "raylib.h"

class PinkGhost : public Ghostbase{

    public:
    // Konstruktor für Position und Farbe
    PinkGhost(int startX, int startY);

    //bewegung überschreiben
    void update(Map& map, const PacMan& pacman)override;


};