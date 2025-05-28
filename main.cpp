#include "raylib.h"
#include "Map.h"
#include "PacMan.h"
#include "RedGhost.h"

const int tileSize = 32;
const int mapWidth = 20;
const int mapHeight = 15;

int main() {
    InitWindow(mapWidth * tileSize, mapHeight * tileSize, "Pac-Man: Autobewegung");
    SetTargetFPS(10); // 10 FPS = langsame Bewegung wie im Original

    Map map(mapWidth, mapHeight);
    PacMan pacman(1, 1);
    RedGhost redghost(8,8);

    while (!WindowShouldClose()) {
        // Eingabe für Richtungsänderung
        if (IsKeyPressed(KEY_UP)) pacman.setDesiredDirection(0, -1);
        if (IsKeyPressed(KEY_DOWN)) pacman.setDesiredDirection(0, 1);
        if (IsKeyPressed(KEY_LEFT)) pacman.setDesiredDirection(-1, 0);
        if (IsKeyPressed(KEY_RIGHT)) pacman.setDesiredDirection(1, 0);

        // Bewegung
        pacman.update(map);
        redghost.update(map, pacman);

        // Zeichnen
        BeginDrawing();
        ClearBackground(BLACK);

        map.draw();
        pacman.draw(tileSize);
        redghost.draw(tileSize);

        DrawText(TextFormat("Score: %i", pacman.getScore()), 10, 10, 20, GOLD);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
