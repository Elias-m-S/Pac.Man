#include "raylib.h"

int main() {
    // Fenster erstellen
    InitWindow(800, 600, "Raylib Testgame");
    SetTargetFPS(60);

    // Spieler-Position
    Vector2 playerPos = { 400, 300 };
    float speed = 5.0f;

    // Game Loop
    while (!WindowShouldClose()) {
        // Eingabe
        if (IsKeyDown(KEY_RIGHT)) playerPos.x += speed;
        if (IsKeyDown(KEY_LEFT))  playerPos.x -= speed;
        if (IsKeyDown(KEY_UP))    playerPos.y -= speed;
        if (IsKeyDown(KEY_DOWN))  playerPos.y += speed;

        // Zeichnen
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Benutze Pfeiltasten!", 10, 10, 20, DARKGRAY);
        DrawRectangleV(playerPos, {50, 50}, BLUE);
        EndDrawing();
    }

    // Aufräumen
    CloseWindow();
    return 0;
}
