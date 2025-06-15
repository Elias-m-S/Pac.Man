#include "Menu.h"
#include "Game.h"
#include <raylib.h>
#include <algorithm>
#include "constants.h"

Game::Game(int width, int height, int tileSize)
    : mapWidth(width)
    , mapHeight(height)
    , tileSize(tileSize)
    , menu({"Start Game", "How to Play", "Leaderboard", "Exit"})
    , map(width, height, tileSize)
    , pacman(PACMAN_SPAWN_X, PACMAN_SPAWN_Y, PACMAN_SPEED) // Konstanten aus constants.h
    , redGhost(new RedGhost(map, RED_GHOST_SPAWN_X, RED_GHOST_SPAWN_Y, GHOST_SPEED))
    , pinkGhost(new PinkGhost(map, PINK_GHOST_SPAWN_X, PINK_GHOST_SPAWN_Y, GHOST_SPEED))
    , greenGhost(new GreenGhost(map, GREEN_GHOST_SPAWN_X, GREEN_GHOST_SPAWN_Y, GHOST_SPEED))
    , blueGhost(new BlueGhost(map, BLUE_GHOST_SPAWN_X, BLUE_GHOST_SPAWN_Y, GHOST_SPEED))
    , leaderboard()
    , state(GameState::MENU)
    , pacmanSpeed(PACMAN_SPEED)
    , ghostSpeed(GHOST_SPEED)
    , ghostEatenCount(0)
{
    // Audio wird später in run() initialisiert
}

Game::~Game() {
    delete redGhost;
    delete pinkGhost;
    delete greenGhost;
    delete blueGhost;
    
    // Sounds entladen und Audio schließen
    UnloadSound(deathSound);
    UnloadSound(eatFruitSound);
    UnloadSound(eatGhostSound);
    CloseAudioDevice();
}

void Game::run() {
    InitWindow(mapWidth * tileSize, mapHeight * tileSize, "Pac-Man");
    SetTargetFPS(60);

    // Audio initialisieren nach Fenster-Initialisierung
    initializeAudio();
    
    // Fenstersymbol setzen
    setWindowIcon();

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        handleInput();
        update();
        draw(dt);
    }

    CloseWindow();
}

void Game::initializeAudio() {
    InitAudioDevice();
    
    // Prüfe ob Audio-Device initialisiert wurde
    if (!IsAudioDeviceReady()) {
        // Fallback: versuche Audio nochmal zu initialisieren
        CloseAudioDevice();
        InitAudioDevice();
    }
    
    // Lade Sound-Dateien
    deathSound = LoadSound("../assets/pacman_death.wav");
    eatFruitSound = LoadSound("../assets/pacman_eatfruit.wav");
    eatGhostSound = LoadSound("../assets/pacman_eatghost.wav");
    
    // Setze Lautstärke
    SetSoundVolume(deathSound, 1.0f);
    SetSoundVolume(eatFruitSound, 1.0f);
    SetSoundVolume(eatGhostSound, 1.0f);
    
}

void Game::setWindowIcon() {
    Image icon = LoadImage("../assets/icon.png");
    SetWindowIcon(icon);
    UnloadImage(icon);
}

void Game::handleInput() {
    switch (state) {
        case GameState::MENU:
            handleMenuInput();
            break;
        case GameState::PLAYING:
            handlePlayingInput();
            break;
        case GameState::GAMEOVERL:
            handleGameOverInput();
            break;
        case GameState::GAMEOVERW:
            handleGameOverInput();
            break;
        case GameState::ENTERNAME:
            handleEnterNameInput();
            break;
        case GameState::LEADERBOARD:
            handleLeaderboardInput();
            break;
        case GameState::HOWTO:
            handleHowToInput();
            break;
    }
}

void Game::handleMenuInput() {
    menu.update();
    if (menu.isSelected()) {
        switch (menu.getSelectedIndex()) {
            case 0: // Start Game
                //wechsel zu Spielen
                resetGame();
                state = GameState::PLAYING;
                // Test-Sound beim Spielstart
                if (eatFruitSound.frameCount > 0) {
                    PlaySound(eatFruitSound);
                }
                break;
            case 1: // How to Play
                state = GameState::HOWTO;
                break;
            case 2: // Leaderboard
                state = GameState::LEADERBOARD;
                break;
            case 3: // Exit
                leaderboard.save();
                CloseWindow();
                return;
        }
        menu.reset();
    }
}

void Game::handlePlayingInput() {
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) 
        pacman.setDesiredDirection(0, -1);
    if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) 
        pacman.setDesiredDirection(0, 1);
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) 
        pacman.setDesiredDirection(-1, 0);
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) 
        pacman.setDesiredDirection(1, 0);
}

void Game::handleGameOverInput() {
    if (IsKeyPressed(KEY_ENTER)) {
        state = GameState::ENTERNAME;
        playerName.clear();
    }
}


void Game::handleEnterNameInput() {
    // Buchstaben einsammeln
    int c = GetCharPressed();
    while (c > 0) {
        if (playerName.size() < maxNameLength && c >= 32 && c <= 125)
            playerName.push_back((char)c);
        c = GetCharPressed();
    }
    
    // Backspace
    if (IsKeyPressed(KEY_BACKSPACE) && !playerName.empty()) {
        playerName.pop_back();
    }
    
    // Enter bestätigt Namen
    if (IsKeyPressed(KEY_ENTER)) {
        if (playerName.empty()) playerName = "PLAYER";
        leaderboard.addEntry(playerName, pacman.getScore());
        state = GameState::LEADERBOARD;
    }
}

void Game::handleLeaderboardInput() {
    if (IsKeyPressed(KEY_ENTER)) {
        state = GameState::MENU;
        playerName.clear();
    }
}

void Game::handleHowToInput() {
    if (IsKeyPressed(KEY_ENTER)) {
        state = GameState::MENU;
    }
}

void Game::resetGame() {
    map = Map(mapWidth, mapHeight, tileSize);
    pacman = PacMan(PACMAN_SPAWN_X, PACMAN_SPAWN_Y, PACMAN_SPEED);
    delete redGhost;
    delete pinkGhost;
    delete greenGhost;
    delete blueGhost;
    redGhost = new RedGhost(map, RED_GHOST_SPAWN_X, RED_GHOST_SPAWN_Y, ghostSpeed);
    pinkGhost = new PinkGhost(map, PINK_GHOST_SPAWN_X, PINK_GHOST_SPAWN_Y, ghostSpeed);
    greenGhost = new GreenGhost(map, GREEN_GHOST_SPAWN_X, GREEN_GHOST_SPAWN_Y, ghostSpeed);
    blueGhost = new BlueGhost(map, BLUE_GHOST_SPAWN_X, BLUE_GHOST_SPAWN_Y, ghostSpeed);
    ghostEatenCount = 0;
}

void Game::update() {
    if (state == GameState::PLAYING) {
        float deltaTime = GetFrameTime();
        
        // Update game objects
        map.update(deltaTime);
        pacman.update(map, deltaTime);
        
        // Handle power-up and fruit eating
        handlePowerUpAndFruit();
        
        // Update all ghosts
        updateGhosts(deltaTime);
        
        // Check game end conditions
        if (map.allCoinsCollected()) {
            state = GameState::GAMEOVERW;
        }
        
        ghostCollision();
    }
}

void Game::handlePowerUpAndFruit() {
    // Check if pacman just ate a power-up
    if (pacman.justAtePowerUp()) {
        // Set all ghosts to frightened mode for 10 seconds
        redGhost->setFrightened(true);
        pinkGhost->setFrightened(true);
        greenGhost->setFrightened(true);
        blueGhost->setFrightened(true);
        ghostEatenCount = 0; // Reset ghost eaten counter for new frightened phase
        
        // Play eat fruit sound for power-up
        if (eatFruitSound.frameCount > 0) {
            PlaySound(eatFruitSound);
        }
    }
    
    // Check if pacman just ate a fruit
    if (pacman.justAteFruit()) {
        // Play eat fruit sound
        if (eatFruitSound.frameCount > 0) {
            PlaySound(eatFruitSound);
        }
    }
}

void Game::updateGhosts(float deltaTime) {
    Vector2 pacmanPos = {(float)pacman.getX(), (float)pacman.getY()};
    
    redGhost->update(deltaTime, pacmanPos, map);
    pinkGhost->update(deltaTime, pacmanPos, map);
    greenGhost->update(deltaTime, pacmanPos, map);
    blueGhost->update(deltaTime, pacmanPos, map);
}

void Game::ghostCollision() {
    if (state != GameState::PLAYING) return;
    
    int pacmanX = pacman.getX();
    int pacmanY = pacman.getY();
    
    // Array von Geister Pointern
    Ghostbase* ghosts[] = {redGhost, pinkGhost, greenGhost, blueGhost};
    
    for (int i = 0; i < 4; i++) {
        Ghostbase* ghost = ghosts[i];
        
        // Ist pacman == ghost?
        if (ghost->getX() == pacmanX && ghost->getY() == pacmanY) {
            if (ghost->canBeEaten()) {
                handleGhostEaten(ghost);
            } else if (!ghost->isFrightened()) {
                handlePacmanDeath();
                return;
            }
        }
    }
}

void Game::handleGhostEaten(Ghostbase* ghost) {
    // Ghost is frightened and can be eaten
    ghost->getEaten();
    
    // Award points for eating ghost (200, 400, 800, 1600 points)
    int points = 200 * (1 << ghostEatenCount); // 200, 400, 800, 1600
    ghostEatenCount = (ghostEatenCount + 1) % 4; // Reset after 4 ghosts
    pacman.addScore(points);
    
    // Play eat ghost sound
    if (eatGhostSound.frameCount > 0) {
        PlaySound(eatGhostSound);
    }
}

void Game::handlePacmanDeath() {
    // Ghost is not frightened - game over
    if (deathSound.frameCount > 0) {
        PlaySound(deathSound);
    }
    state = GameState::GAMEOVERL;
}

void Game::draw(float dt) {
    BeginDrawing();
    ClearBackground(BLACK);
    
    switch (state) {
        case GameState::MENU:
            drawMenu();
            break;
        case GameState::HOWTO:
            drawHowTo();
            break;
        case GameState::PLAYING:
            drawPlaying();
            break;
        case GameState::ENTERNAME:
            drawEnterName();
            break;
        case GameState::GAMEOVERL:
            drawGameOverL(dt);
            break;
        case GameState::GAMEOVERW:
            drawGameOverW(dt);
            break;
        case GameState::LEADERBOARD:
            drawLeaderboard();
            break;
    }
    
    EndDrawing();
}

void Game::drawMenu() {
    menu.draw(mapWidth * tileSize, mapHeight * tileSize);
}

void Game::drawHowTo() {
    DrawText("How to Play:", 50, 50, 30, WHITE);
    DrawText("- Use Arrow keys or WASD to move Pac-Man.", 50, 100, 20, WHITE);
    DrawText("- Eat all coins (10 pts).", 50, 130, 20, WHITE);
    DrawText("- Fruits appear randomly (100 pts).", 50, 160, 20, WHITE);
    DrawText("- Power-ups (yellow) make ghosts frightened!", 50, 190, 20, WHITE);
    DrawText("- Frightened ghosts show only eyes and can be eaten.", 50, 220, 20, WHITE);
    DrawText("- Eaten ghosts respawn at center and restart.", 50, 250, 20, WHITE);
    DrawText("- Avoid normal ghosts or it's game over.", 50, 280, 20, WHITE);
    DrawText("Press ENTER to return to Menu", 50, 320, 20, SKYBLUE);
}

void Game::drawPlaying() {
    map.draw();
    pacman.draw(tileSize);
    redGhost->draw(tileSize);
    pinkGhost->draw(tileSize);
    greenGhost->draw(tileSize);
    blueGhost->draw(tileSize);

    // Debug: Scatter-Ziele der Geister einzeichnen
    /*
    int scatterRadius = tileSize / 6;
    DrawCircle(redGhost->getScatterTarget().x * tileSize + tileSize/2,
               redGhost->getScatterTarget().y * tileSize + tileSize/2,
               scatterRadius, RED);
    DrawCircle(pinkGhost->getScatterTarget().x * tileSize + tileSize/2,
               pinkGhost->getScatterTarget().y * tileSize + tileSize/2,
               scatterRadius, PINK);
    DrawCircle(greenGhost->getScatterTarget().x * tileSize + tileSize/2,
               greenGhost->getScatterTarget().y * tileSize + tileSize/2,
               scatterRadius, GREEN);
    DrawCircle(blueGhost->getScatterTarget().x * tileSize + tileSize/2,
               blueGhost->getScatterTarget().y * tileSize + tileSize/2,
               scatterRadius, BLUE);
    */

    DrawText(TextFormat("Score: %i", pacman.getScore()), 10, 10, 20, GOLD);
}

void Game::drawEnterName() {
    DrawText("Enter your name:", 50, 50, 20, WHITE);
    DrawText(playerName.c_str(), 50, 80, 30, YELLOW);
    int cx = 50 + MeasureText(playerName.c_str(), 30);
    if (((int)(GetTime() * 2) % 2) == 0) {
        DrawLine(cx, 80, cx, 110, YELLOW);
    }
}

void Game::drawGameOverL(float dt) {
    static float fadeTimer = 0.0f;
    fadeTimer += dt;
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    float maxInset = std::min(w, h) * 0.5f;
    float speed = 300.0f;
    float inset = std::min(maxInset, fadeTimer * speed);
    
    // Draw game state behind fade effect
    map.draw();
    pacman.draw(tileSize);
    redGhost->draw(tileSize);
    pinkGhost->draw(tileSize);
    greenGhost->draw(tileSize);
    blueGhost->draw(tileSize);
    
    // Draw fade effect
    DrawRectangle(0, 0, (int)inset, h, BLACK);
    DrawRectangle(w - (int)inset, 0, (int)inset, h, BLACK);
    DrawRectangle((int)inset, 0, w - 2 * (int)inset, (int)inset, BLACK);
    DrawRectangle((int)inset, h - (int)inset, w - 2 * (int)inset, (int)inset, BLACK);
    
    if (inset >= maxInset) {
        const char* msg1 = "GAME OVER";
        int fontSize1 = 54;
        int tw1 = MeasureText(msg1, fontSize1);
    
        // Position of main message
        int y1 = h / 2 - fontSize1 / 2;
        DrawText(msg1, w / 2 - tw1 / 2, y1, fontSize1, RED);

        const char* msg2 = "ENTER to continue";
        int fontSize2 = 20;
        int tw2 = MeasureText(msg2, fontSize2);
    
        // Position second message below the first with some spacing
        int y2 = y1 + fontSize1 + 10; // 10 pixels of space between the texts
        DrawText(msg2, w / 2 - tw2 / 2, y2, fontSize2, RED);
    }    
}

void Game::drawGameOverW(float dt) {
    static float fadeTimer = 0.0f;
    fadeTimer += dt;
    int w = GetScreenWidth();
    int h = GetScreenHeight();
    float maxInset = std::min(w, h) * 0.5f;
    float speed = 300.0f;
    float inset = std::min(maxInset, fadeTimer * speed);
    
    // Draw game state behind fade effect
    map.draw();
    pacman.draw(tileSize);
    redGhost->draw(tileSize);
    pinkGhost->draw(tileSize);
    greenGhost->draw(tileSize);
    blueGhost->draw(tileSize);
    
    // Draw fade effect
    DrawRectangle(0, 0, (int)inset, h, BLACK);
    DrawRectangle(w - (int)inset, 0, (int)inset, h, BLACK);
    DrawRectangle((int)inset, 0, w - 2 * (int)inset, (int)inset, BLACK);
    DrawRectangle((int)inset, h - (int)inset, w - 2 * (int)inset, (int)inset, BLACK);
    
    if (inset >= maxInset) {
        const char* msg1 = "YOU WIN!";
        int fontSize1 = 54;
        int tw1 = MeasureText(msg1, fontSize1);
    
        // Position of main message
        int y1 = h / 2 - fontSize1 / 2;
        DrawText(msg1, w / 2 - tw1 / 2, y1, fontSize1, GOLD);

        const char* msg2 = "ENTER to continue";
        int fontSize2 = 20;
        int tw2 = MeasureText(msg2, fontSize2);
    
        // Position second message below the first with some spacing
        int y2 = y1 + fontSize1 + 10; // 10 pixels of space between the texts
        DrawText(msg2, w / 2 - tw2 / 2, y2, fontSize2, GOLD);

    }
}

void Game::drawLeaderboard() {
    leaderboard.draw();
    DrawText("Press Enter to Restart", 50, mapHeight * tileSize - 40, 20, WHITE);
}