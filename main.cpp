#include "raylib.h"
#include <deque>

const int screenWidth = 800;
const int screenHeight = 600;
const int cellSize = 20;
const int gridWidth = screenWidth / cellSize;
const int gridHeight = screenHeight / cellSize;

enum Direction { UP, DOWN, LEFT, RIGHT };

struct Segment {
    int x, y;
};

int main() {
    InitWindow(screenWidth, screenHeight, "Snake Game - Raylib");
    SetTargetFPS(10);  // Controls snake speed

    std::deque<Segment> snake = { {gridWidth / 2, gridHeight / 2} };
    Direction dir = RIGHT;
    Segment food = { GetRandomValue(0, gridWidth - 1), GetRandomValue(0, gridHeight - 1) };

    bool gameOver = false;

    while (!WindowShouldClose()) {
        // Input
        if (IsKeyPressed(KEY_UP) && dir != DOWN) dir = UP;
        if (IsKeyPressed(KEY_DOWN) && dir != UP) dir = DOWN;
        if (IsKeyPressed(KEY_LEFT) && dir != RIGHT) dir = LEFT;
        if (IsKeyPressed(KEY_RIGHT) && dir != LEFT) dir = RIGHT;

        // Movement
        if (!gameOver) {
            Segment head = snake.front();
            switch (dir) {
                case UP: head.y--; break;
                case DOWN: head.y++; break;
                case LEFT: head.x--; break;
                case RIGHT: head.x++; break;
            }

            // Collision check
            if (head.x < 0 || head.y < 0 || head.x >= gridWidth || head.y >= gridHeight) gameOver = true;
            for (const auto& s : snake) {
                if (s.x == head.x && s.y == head.y) gameOver = true;
            }

            snake.push_front(head);

            if (head.x == food.x && head.y == food.y) {
                food = { GetRandomValue(0, gridWidth - 1), GetRandomValue(0, gridHeight - 1) };
            } else {
                snake.pop_back();
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(DARKGREEN);

        if (gameOver) {
            DrawText("Game Over! Press R to Restart", 200, screenHeight / 2, 20, RED);
            if (IsKeyPressed(KEY_R)) {
                snake = { {gridWidth / 2, gridHeight / 2} };
                dir = RIGHT;
                food = { GetRandomValue(0, gridWidth - 1), GetRandomValue(0, gridHeight - 1) };
                gameOver = false;
            }
        }

        // Draw grid
        for (int x = 0; x < gridWidth; x++)
            for (int y = 0; y < gridHeight; y++)
                DrawRectangleLines(x * cellSize, y * cellSize, cellSize, cellSize, DARKGRAY);

        // Draw snake
        for (const auto& s : snake)
            DrawRectangle(s.x * cellSize, s.y * cellSize, cellSize, cellSize, GREEN);

        // Draw food
        DrawRectangle(food.x * cellSize, food.y * cellSize, cellSize, cellSize, RED);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
