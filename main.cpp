#include "raylib.h"
#include "PacMan.h"

int main() {
    InitWindow(800, 600, "Test PacMan");
    SetTargetFPS(60);

    PacMan pac({400, 300}, 20.0f);

    while (!WindowShouldClose()) {
        pac.update();

        BeginDrawing();
        ClearBackground(BLACK);
        pac.draw();
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
