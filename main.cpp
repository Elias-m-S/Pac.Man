#include "raylib.h"

int main() {
    // Fenster initialisieren (Breite, Höhe, Titel)
    InitWindow(800, 600, "Pac-Man Fenster");

    // Framerate setzen (optional)
    SetTargetFPS(60);

    // Haupt-Game-Loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("Hallo Till", 350, 280, 20, DARKGRAY);

        EndDrawing();
    }

    // Fenster und Ressourcen freigeben
    CloseWindow();

    return 0;
}
