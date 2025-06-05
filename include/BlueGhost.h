#pragma once
#include "PacMan.h"
#include "Ghostbase.h"
#include "raylib.h"

class BlueGhost : public Ghostbase{

    public:
    // Konstruktor für Position und Farbe
    BlueGhost(int startX, int startY);

    //bewegung überschreiben
    void update(Map& map, const PacMan& pacman)override;


};