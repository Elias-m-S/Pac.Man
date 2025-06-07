
#include "Game.h"
#include <raylib.h>
#include <algorithm>

Game::Game(int width, int height, int tileSize)
    : mapWidth(width), mapHeight(height), tileSize(tileSize),
      map(width, height, tileSize),
      pacman(1, 1, 1),
      redGhost(1, 1),
      leaderboard("assets/Scoreboard.txt"),
      state(GameState::START), playerName("")
{}

void Game::run() {
    InitWindow(mapWidth * tileSize, mapHeight * tileSize, "Pac-Man");
    SetTargetFPS(60);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        handleInput();
        update();
        draw();
    }

    CloseWindow();
}

void Game::handleInput() {
    if (state == GameState::START) {
        int key = GetKeyPressed();
        if (key > 0) {
            if (key == KEY_ENTER && !playerName.empty()) {
                state = GameState::PLAYING;
            } else if (key == KEY_BACKSPACE && !playerName.empty()) {
                playerName.pop_back();
            } else if (playerName.size() < maxNameLength) {
                char c = static_cast<char>(key);
                if (isprint(c)) playerName.push_back(c);
            }
        }
    } else if (state == GameState::PLAYING) {
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) pacman.setDesiredDirection(0, -1);
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) pacman.setDesiredDirection(0, 1);
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) pacman.setDesiredDirection(-1, 0);
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) pacman.setDesiredDirection(1, 0);
    } else if (state == GameState::GAMEOVER) {
        if (IsKeyPressed(KEY_ENTER)) {
            leaderboard.addEntry(playerName, pacman.getScore());
            state = GameState::LEADERBOARD;
        }
    } else if (state == GameState::LEADERBOARD) {
        if (IsKeyPressed(KEY_ENTER)) {
            // restart
            state = GameState::START;
            // reset game
            map = Map(mapWidth, mapHeight, tileSize);
            pacman = PacMan(1, 1, 1);
            playerName.clear();
        }
    }
}

void Game::update() {
    if (state == GameState::PLAYING) {
        map.update(GetFrameTime());
        pacman.update(map);
        if (map.allCoinsCollected()) state = GameState::GAMEOVER;
    }
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (state == GameState::START) {
        DrawText("Enter Name:", 50, 50, 20, WHITE);
        DrawText(playerName.c_str(), 50, 80, 20, GOLD);
    } else if (state == GameState::PLAYING) {
        map.draw();
        pacman.draw(tileSize);
        redGhost.draw(tileSize);
        DrawText(TextFormat("Score: %i", pacman.getScore()), 10, 10, 20, GOLD);
    } else if (state == GameState::GAMEOVER) {
        DrawText("Game Over! Press Enter.", 50, 50, 20, RED);
        DrawText(TextFormat("Final Score: %i", pacman.getScore()), 50, 80, 20, GOLD);
    } else if (state == GameState::LEADERBOARD) {
        leaderboard.draw();
        DrawText("Press Enter to Restart", 50, mapHeight * tileSize - 40, 20, WHITE);
    }

    EndDrawing();
}