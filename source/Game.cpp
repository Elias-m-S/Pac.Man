#include "Menu.h"
#include "Game.h"
#include <raylib.h>
#include <algorithm>

Game::Game(int width, int height, int tileSize)
    : mapWidth(width), mapHeight(height), tileSize(tileSize),
      map(width, height, tileSize),
      pacman(10, 15, 1),
      redGhost(map, 10, 7), //Spawnpoint (über der Tür)
      pinkGhost(map, 10, 9), //Spawnpoint (im Käfig)
      greenGhost(map, 9, 9), //Spawnpoint (im Käfig)
      blueGhost(map, 11, 9), //Spawnpoint (im Käfig)
      leaderboard("assets/Scoreboard.txt"),
      state(GameState::MENU),
      menu({"Start Game", "How to Play", "Leaderboard", "Exit"})
{}

void Game::run() {
    InitWindow(mapWidth * tileSize, mapHeight * tileSize, "Pac-Man");
    SetTargetFPS(10); // Zurück zu 10 FPS wie ursprünglich

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        handleInput();
        update();
        draw();
    }

    CloseWindow();
}

void Game::handleInput() {
    if (state == GameState::MENU) {
        menu.update();
        if (menu.isSelected()) {
            switch (menu.getSelectedIndex()) {
                case 0: state = GameState::PLAYING;    break;
                case 1: state = GameState::HOWTO; break;
                case 2: state = GameState::LEADERBOARD;       break;
                case 3: CloseWindow(); return;          // Exit
            }
            menu.reset();
        }
    } else if (state == GameState::PLAYING || state == GameState::ENDLESSGAME) {
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
            state = GameState::MENU;
            // reset game
            map = Map(mapWidth, mapHeight, tileSize);
            pacman = PacMan(10, 15, 1); // Startposition Pacman
            //eventuell fehlen Geister?
            playerName.clear();
        }
    }
}

void Game::update() {
    if (state == GameState::PLAYING || state == GameState::ENDLESSGAME) {
        map.update(GetFrameTime());
        pacman.update(map);
        redGhost.update(GetFrameTime(), {(float)pacman.getX(), (float)pacman.getY()}); // RedGhost aktivieren
        pinkGhost.update(GetFrameTime(), {(float)pacman.getX(), (float)pacman.getY()}); // PinkGhost aktivieren
        greenGhost.update(GetFrameTime(), {(float)pacman.getX(), (float)pacman.getY()}); // GreenGhost aktivieren
        blueGhost.update(GetFrameTime(), {(float)pacman.getX(), (float)pacman.getY()}); // BlueGhost aktivieren
        if (map.allCoinsCollected()) state = GameState::ENDLESSGAME; //unendliches Spiel(bis pacman von geist erwischt)
        ghostCollision(); // Touchen mit Geistern überprüfen
    }
}

void Game::ghostCollision() {
    // Wird nur verwendet, wenn das spiel läuft
    if (state != GameState::PLAYING && state !=GameState::ENDLESSGAME) return;

    // benutzen der aus enity gegebenen getX und getY Funktionen, um Position von Pacman zu speichern
    int px = pacman.getX();
    int py = pacman.getY();

    // Jetzt gucken ob die Geister pacman touchen, und ob sie nicht in FRIGHTENED sind (alternativ ist überprüfung auch mit Vector der geister möglcih, aber so ist es einfacher), ist aber zu aufwendig für so wenige geister
    if ((redGhost.getX() == px && redGhost.getY() == py && !redGhost.isFrightened() ) ||
        (pinkGhost.getX() == px && pinkGhost.getY() == py && !pinkGhost.isFrightened()) ||
        (greenGhost.getX() == px && greenGhost.getY() == py && !greenGhost.isFrightened()) ||
        (blueGhost.getX() == px && blueGhost.getY() == py && !blueGhost.isFrightened())) {
        state = GameState::GAMEOVER;
    }
}

void Game::draw() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (state == GameState::MENU) {
        menu.draw(mapWidth*tileSize, mapHeight*tileSize);
        
    } else if (state == GameState::HOWTO) {
        DrawText("How to Play:", 50, 50, 30, WHITE);
        DrawText("- Use Arrow keys or WASD to move Pac-Man.", 50, 100, 20, WHITE);
        DrawText("- Eat all coins (10 pts).", 50, 130, 20, WHITE);
        DrawText("- Fruits appear randomly (100 pts).", 50, 160, 20, WHITE);
        DrawText("- Avoid ghosts unless they're frightened.", 50, 190, 20, WHITE);
        DrawText("Press ESC to return to Menu", 50, 240, 20, SKYBLUE);
    } else if (state == GameState::PLAYING) {
        map.draw();
        pacman.draw(tileSize);
        redGhost.draw(tileSize);
        pinkGhost.draw(tileSize); 
        greenGhost.draw(tileSize); 
        blueGhost.draw(tileSize); 
        DrawText(TextFormat("Score: %i", pacman.getScore()), 10, 10, 20, GOLD);
        
    } else if (state == GameState::ENDLESSGAME) {
        map.draw();
        pacman.draw(tileSize);
        redGhost.draw(tileSize);
        pinkGhost.draw(tileSize); 
        greenGhost.draw(tileSize); 
        blueGhost.draw(tileSize); 
        DrawText(TextFormat("Score: %i", pacman.getScore()), 10, 10, 20, GOLD);
        DrawText("Endless Mode! Avoid Ghosts!", 50, mapHeight * tileSize - 40, 20, WHITE);

    }else if (state == GameState::GAMEOVER) {
        DrawText("Game Over! Press Enter.", 50, 50, 20, RED);
        DrawText(TextFormat("Final Score: %i", pacman.getScore()), 50, 80, 20, GOLD);
    } else if (state == GameState::LEADERBOARD) {
        leaderboard.draw();
        DrawText("Press Enter to Restart", 50, mapHeight * tileSize - 40, 20, WHITE);
    }

    EndDrawing();
}