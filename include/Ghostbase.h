#pragma once
#include "Entity.h"
#include "Map.h"
#include "Pacman.h"


class Ghostbase : public Entity{
    
protected:
    //gemeinsame hilfsfunktionen einfügen, wie z.B.blinken, collisionsüberprüfung etc 
    //outsourcen der kollisionsprüfung der einzelnen geister
    bool canMove(Map& map, int dx, int dy);
    //Attribut der eigenen Farbe
    Color color;
public:

    //Startposition (in Geisterklassen tatsächlichen startort)
    Ghostbase(int startX, int startY);
    //ermöglichen dass Farbe der Abgeleiteten Geister individuell sein können
    Ghostbase(int sx, int sy, Color col)
        : Entity(sx, sy), color(col){}

    //nur virtuell, weil jeder geist später sein eigenes Verhalten bekommt
    virtual void update(class Map& map, const PacMan& pacman) = 0;

    //draw wird übershrieben, eventuell gemeinsame Grundzeichnung der Geister 
    void draw(int tileSize) const override;


};