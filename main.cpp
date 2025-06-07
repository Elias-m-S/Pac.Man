
#include "raylib.h"
#include "Map.h"
#include "PacMan.h"

const int tileSize = 32;
const int mapWidth = 21;
const int mapHeight = 21;

int main() {
    InitWindow(mapWidth * tileSize, mapHeight * tileSize, "Pac-Man");
    SetTargetFPS(10);

    Map map(mapWidth, mapHeight, tileSize);
    PacMan pacman(10, 10, 1);

    while (!WindowShouldClose()) {
        // Input
        if (IsKeyPressed(KEY_UP)) pacman.setDesiredDirection(0, -1);
        if (IsKeyPressed(KEY_DOWN)) pacman.setDesiredDirection(0, 1);
        if (IsKeyPressed(KEY_LEFT)) pacman.setDesiredDirection(-1, 0);
        if (IsKeyPressed(KEY_RIGHT)) pacman.setDesiredDirection(1, 0);

        // Update
        map.update(1.0f / 10.0f);
        pacman.update(map);

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        map.draw();
        pacman.draw(tileSize);
        DrawText(TextFormat("Score: %i", pacman.getScore()), 10, 10, 20, GOLD);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
