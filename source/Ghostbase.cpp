#include "Ghostbase.h"
#include "raylib.h"


Ghostbase::Ghostbase(int startX, int startY)
    : Entity(startX, startY){}


//alle geister in gleiche Form bringen
void Ghostbase::draw(int tileSize) const{

    //kreis in der mitte der zelle
    int cx = x * tileSize + tileSize /2;
    int cy = y * tileSize + tileSize /2;
    DrawCircle(
        cx, cy + tileSize/8, tileSize/2 -2,
        color
    );

    int rectY = y* tileSize + tileSize/4;
    DrawRectangle(
        x*tileSize + 4, rectY,
        tileSize - 8, tileSize/2,
        color
    );
};

bool Ghostbase::canMove(Map& map, int dx, int dy){

    int nx = x + dx, ny = y + dy;
    if (map.isWalkable(nx, ny)) {
        // move(dx, dy);
        return true;
    }
    return false;
};