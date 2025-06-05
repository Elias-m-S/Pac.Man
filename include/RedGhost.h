#pragma once
#include "PacMan.h"
#include "Ghostbase.h"
#include "raylib.h"

class RedGhost : public Ghostbase{

    public:
    // Konstruktor für Position und Farbe
    RedGhost(int startX, int startY);

    //bewegung überschreiben
    void update(Map& map, const PacMan& pacman)override;


};