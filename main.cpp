#include "raylib.h"
#include "Map.h"
#include "PacMan.h"

const int tileSize = 32;
const int mapWidth = 20;
const int mapHeight = 15;

int main() {
    InitWindow(mapWidth * tileSize, mapHeight * tileSize, "PacMan Test");
    SetTargetFPS(60);

    Map map(mapWidth, mapHeight);
    PacMan pacman(1, 1); // Start bei Feld (1,1)

    while (!WindowShouldClose()) {
        // ===== INPUT / UPDATE =====
        int dx = 0, dy = 0;

        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) dy = -1;
        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) dy = 1;
        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) dx = -1;
        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) dx = 1;

        int newX = pacman.getX() + dx;
        int newY = pacman.getY() + dy;

        if (map.isWalkable(newX, newY)) {
            pacman.move(dx, dy);
            if (map.hasCoin(newX, newY)) {
                map.collectCoin(newX, newY);
                pacman.addScore(10);
            }
        }

        // ===== DRAWING =====
        BeginDrawing();
        ClearBackground(BLACK);

        map.draw();
        pacman.draw(tileSize);

        DrawText(TextFormat("Score: %i", pacman.getScore()), 10, 10, 20, YELLOW);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
