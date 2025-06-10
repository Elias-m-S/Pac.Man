#include "Menu.h"
#include "Game.h"
#include <raylib.h>
#include <algorithm>

Game::Game(int width, int height, int tileSize)
    : mapWidth(width), mapHeight(height), tileSize(tileSize),

      pacmanSpeed(8.0f), // <--- Set your default PacMan speed here
      ghostSpeed(4.0f),  // <--- Set your default Ghost speed here
      map(width, height, tileSize),
      pacman(10, 15, pacmanSpeed),
      redGhost(new RedGhost(map, 10, 7, ghostSpeed)),
      pinkGhost(new PinkGhost(map, 10, 9, ghostSpeed)),
      greenGhost(new GreenGhost(map, 9, 9, ghostSpeed)),
      blueGhost(new BlueGhost(map, 11, 9, ghostSpeed)),
      leaderboard(),
      state(GameState::MENU),
      menu({"Start Game", "How to Play", "Leaderboard", "Exit"})
{}

Game::~Game() {
    delete redGhost;
    delete pinkGhost;
    delete greenGhost;
    delete blueGhost;
}

void Game::run() {
    InitWindow(mapWidth * tileSize, mapHeight * tileSize, "Pac-Man");
    SetTargetFPS(60);//rück zu 10 FPS wie ursprünglich

    // ─── Fenstersymbol setzen ───────────────────────────────────────────────
    // Icon-Datei liegt im Root/assets/icon.png
    Image icon = LoadImage("assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
    // ─────────────────────────────────────────────────────────────────────────

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        handleInput();
        update();
        draw(dt);
    }

    CloseWindow();
}

void Game::handleInput() {
    if (state == GameState::MENU) {
        menu.update();
        if (menu.isSelected()) {
            switch (menu.getSelectedIndex()) {
                case 0:
                    map         = Map(mapWidth, mapHeight, tileSize);
                    pacman      = PacMan(10, 15, pacmanSpeed);
                    delete redGhost;
                    delete pinkGhost;
                    delete greenGhost;
                    delete blueGhost;
                    redGhost    = new RedGhost(map, 10, 7, ghostSpeed);
                    pinkGhost   = new PinkGhost(map, 10, 9, ghostSpeed);
                    greenGhost  = new GreenGhost(map, 9, 9, ghostSpeed);
                    blueGhost   = new BlueGhost(map, 11, 9, ghostSpeed);
                    state = GameState::PLAYING;
                    break;
                case 1: state = GameState::HOWTO; break;
                case 2: state = GameState::LEADERBOARD; break;
                case 3:
                    leaderboard.save();
                    CloseWindow();
                    return; // Exit
            }
            menu.reset();
        }
    }
    else if (state == GameState::PLAYING || state == GameState::ENDLESSGAME) {
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) pacman.setDesiredDirection(0, -1);
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) pacman.setDesiredDirection(0, 1);
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) pacman.setDesiredDirection(-1, 0);
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) pacman.setDesiredDirection(1, 0);
    }
    else if (state == GameState::GAMEOVER) {
        if (IsKeyPressed(KEY_ENTER)) {
            state = GameState::ENTERNAME;
            playerName.clear();
        }    
    }
    else if (state == GameState::ENTERNAME) {
        // Buchstaben einsammeln
        int c = GetCharPressed();
        while (c > 0) {
            if (playerName.size() < maxNameLength && c >= 32 && c <= 125)
                playerName.push_back((char)c);
            c = GetCharPressed();
        }
        // Backspace
        if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty()){
            playerName.pop_back();
        // Enter bestätigt Namen
        }
        if (IsKeyPressed(KEY_ENTER)) {
            if (playerName.empty()) playerName = "PLAYER";
            leaderboard.addEntry(playerName, pacman.getScore());
            state = GameState::LEADERBOARD;
        }
    }
    else if (state == GameState::LEADERBOARD) {
        if (IsKeyPressed(KEY_ENTER)) {
            // restart
            state = GameState::MENU;
            playerName.clear();
        }
    }
    else if (state == GameState::HOWTO) {
        if (IsKeyPressed(KEY_ENTER)) {
            state = GameState::MENU;
        }
    }
}

void Game::update() {
    if (state == GameState::PLAYING || state == GameState::ENDLESSGAME) {
        float deltaTime = GetFrameTime();
        map.update(deltaTime);
        pacman.update(map, deltaTime);
        redGhost->update(deltaTime, {(float)pacman.getX(), (float)pacman.getY()}, map);
        pinkGhost->update(deltaTime, {(float)pacman.getX(), (float)pacman.getY()}, map);
        greenGhost->update(deltaTime, {(float)pacman.getX(), (float)pacman.getY()}, map);
        blueGhost->update(deltaTime, {(float)pacman.getX(), (float)pacman.getY()}, map);
        if (map.allCoinsCollected()) state = GameState::ENDLESSGAME;
        ghostCollision();
    }
}

void Game::ghostCollision() {
    if (state != GameState::PLAYING && state !=GameState::ENDLESSGAME) return;
    int px = pacman.getX();
    int py = pacman.getY();
    if ((redGhost->getX() == px && redGhost->getY() == py && !redGhost->isFrightened() ) ||
        (pinkGhost->getX() == px && pinkGhost->getY() == py && !pinkGhost->isFrightened()) ||
        (greenGhost->getX() == px && greenGhost->getY() == py && !greenGhost->isFrightened()) ||
        (blueGhost->getX() == px && blueGhost->getY() == py && !blueGhost->isFrightened())) {
        state = GameState::GAMEOVER;
    }
}

void Game::draw(float dt) {
    BeginDrawing();
    ClearBackground(BLACK);
    if (state == GameState::MENU) {
        menu.draw(mapWidth*tileSize, mapHeight*tileSize);
    }else if (state == GameState::HOWTO) {
        DrawText("How to Play:", 50, 50, 30, WHITE);
        DrawText("- Use Arrow keys or WASD to move Pac-Man.", 50, 100, 20, WHITE);
        DrawText("- Eat all coins (10 pts).", 50, 130, 20, WHITE);
        DrawText("- Fruits appear randomly (100 pts).", 50, 160, 20, WHITE);
        DrawText("- Avoid ghosts unless they're frightened.", 50, 190, 20, WHITE);
        DrawText("Press ENTER to return to Menu", 50, 240, 20, SKYBLUE);
    }else if (state == GameState::PLAYING) {
        map.draw();
        pacman.draw(tileSize);
        redGhost->draw(tileSize);
        pinkGhost->draw(tileSize);
        greenGhost->draw(tileSize);
        blueGhost->draw(tileSize);
        DrawText(TextFormat("Score: %i", pacman.getScore()), 10, 10, 20, GOLD);
    }else if (state == GameState::ENDLESSGAME) {
        map.draw();
        pacman.draw(tileSize);
        redGhost->draw(tileSize);
        pinkGhost->draw(tileSize);
        greenGhost->draw(tileSize);
        blueGhost->draw(tileSize);
        DrawText(TextFormat("Score: %i", pacman.getScore()), 10, 10, 20, GOLD);
        DrawText("Endless Mode! Avoid Ghosts!", 50, mapHeight * tileSize - 40, 20, WHITE);
    }else if (state == GameState::ENTERNAME) {
        DrawText("Enter your name:", 50, 50, 20, WHITE);
        DrawText(playerName.c_str(), 50, 80, 30, YELLOW);
        int cx = 50 + MeasureText(playerName.c_str(), 30);
        if ( ((int)(GetTime()*2) % 2) == 0 )
            DrawLine(cx, 80, cx, 110, YELLOW);
    }else if (state == GameState::GAMEOVER) {
        static float fadeTimer = 0.0f;
        fadeTimer += dt;
        int  w        = GetScreenWidth();
        int  h        = GetScreenHeight();
        float maxInset = std::min(w, h) * 0.5f;
        float speed    = 300.0f;
        float inset    = std::min(maxInset, fadeTimer * speed);
        map.draw();
        pacman.draw(tileSize);
        redGhost->draw(tileSize);
        pinkGhost->draw(tileSize);
        greenGhost->draw(tileSize);
        blueGhost->draw(tileSize);
        DrawRectangle(    0, 0,                (int)inset, h,      BLACK);
        DrawRectangle(w-(int)inset, 0,         (int)inset, h,      BLACK);
        DrawRectangle((int)inset, 0,           w-2*(int)inset, (int)inset, BLACK);
        DrawRectangle((int)inset, h-(int)inset, w-2*(int)inset, (int)inset, BLACK);
        if (inset >= maxInset) {
            const char* msg = "GAME OVER";
            int fontSize    = 48;
            int tw          = MeasureText(msg, fontSize);
            DrawText(msg,
                     w/2 - tw/2,
                     h/2 - fontSize/2,
                     fontSize,
                     RED);
        }
    }else if (state == GameState::LEADERBOARD) {
        leaderboard.draw();
        DrawText("Press Enter to Restart", 50, mapHeight * tileSize - 40, 20, WHITE);
    }
    EndDrawing();
}