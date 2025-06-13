
#include "Ghostbase.h"
#include <iostream>
#include <algorithm> //für algorythmen
#include <random> //für zufällige Komponenten
#include <cfloat> //für FLT_MAX, um unendlich roße werte zu haben
#include <cstdlib> //für rand()

Ghostbase::Ghostbase(const Map& map, int startX, int startY, float speed)
    : Entity(startX, startY, speed),
      frightenedColor(BLUE),
      radius(14), // Feste Radius für Geister
      state(GhostState::IN_BASE),
      stateTimer(2.0f), // 2 Sekunden warten bevor sie die Basis verlassen
      moveTimer(0.0f),
      moveInterval(1.0f / speed), // Bewegungsintervall: 1/speed Sekunden pro Tile
      mapRef(map),
      spawnX(startX), spawnY(startY), // Spawn-Position merken
      rng(std::random_device{}())
{

}


void Ghostbase::update(float deltaTime, const Vector2& pacmanPos, const Map& map) {
    moveTimer -= deltaTime;
    stateTimer -= deltaTime;

    // Simplified state transitions
        if (stateTimer <= 0.0f) {
        if (state == GhostState::IN_BASE) {
            // Leave base after initial wait
            changeState(GhostState::SCATTER);
            stateTimer = 7.0f; // 7 seconds in scatter mode
        } 
        else if (state == GhostState::SCATTER) {
            // After scatter, permanently go to chase
            changeState(GhostState::CHASE);
            stateTimer = FLT_MAX; // Stay in chase mode indefinitely
        }
        else if (state == GhostState::FRIGHTENED) {
            // After frightened wears off, return to chase
            changeState(GhostState::CHASE);
            stateTimer = FLT_MAX; // Stay in chase mode indefinitely
            moveInterval = 1.0f / speed; // Restore normal speed
        }
        // No more cycling between chase and scatter
    }

    // Rest of update method remains unchanged
    if (moveTimer <= 0.0f) {
        moveTimer = moveInterval; // Timer zurücksetzen
        Vector2 target;
        if (state == GhostState::IN_BASE) {
            // Ziel: Basis verlassen (gehe nach oben zur Tür bei Position 10,7)
            target = Vector2{10, 7};
        } else if (state == GhostState::FRIGHTENED) {
            target = randomTile();
        } else if (state == GhostState::SCATTER) {
            target = getScatterTarget(); // Zur eigenen Ecke gehen
        } else { // CHASE Modus
            target = getTargetTile(pacmanPos); // Pacman verfolgen
        }

        // Bewegung in Richtung ausgewählten Ziels
        Vector2 dir = chooseDirectionTowards(target);
        setDirection((int)dir.x, (int)dir.y);
        
        // Diskrete Tile-Bewegung: immer genau 1 Tile bewegen
        int nextX = x + getDirX();
        int nextY = y + getDirY();
        
        // Prüfe ob Bewegung möglich ist
        if (mapRef.isWalkable(nextX, nextY)) {
            x = nextX;
            y = nextY;
            // Tunnel benutzen
            handleTunnelWrap(map.getWidth());
        }
    }
}

// Generalisiertes zeichnen, so dass einzelne Geister nur noch Farb überschrieben müssen
void Ghostbase::draw(int tileSize) const {
    Color drawColor;
    if (state == GhostState::FRIGHTENED) {
        drawColor = frightenedColor;
    } else {
        drawColor = normalColor;
    }

    //Die Mitte der tiles festlegen als "Anker"
    int centerX = x * tileSize + tileSize/2;
    int centerY = y * tileSize + tileSize/2;
    //Basierend auf TileSize den Radius der geister bestimmen
    int ghostRadius = tileSize/2 - 2;
    
    // Körper nur zeichnen wenn NICHT im Frightened Modus
    if (state != GhostState::FRIGHTENED) {
        // Kreis(Kopfrundung)der Geister
        DrawCircle(centerX, centerY - ghostRadius/4, ghostRadius, drawColor);
        
        // rechteckiger Körper (aka das Betttuch was runterhängt)
        DrawRectangle(centerX - ghostRadius, centerY - ghostRadius/4, 
                      ghostRadius * 2, ghostRadius + ghostRadius/2, drawColor);
    }    
    // Augen Zeichnenvorbereiten, damit abhängig von den Zuständen
    int eyeSize = ghostRadius / 4;
    int eyeOffsetX = ghostRadius / 3;
    int eyeOffsetY = ghostRadius / 3;
    
    if (state == GhostState::FRIGHTENED) {
        // Nur Augen für frightened Geister (größer und auffälliger)
        eyeSize = ghostRadius / 2;
        DrawCircle(centerX - eyeOffsetX, centerY - eyeOffsetY, eyeSize, WHITE);
        DrawCircle(centerX + eyeOffsetX, centerY - eyeOffsetY, eyeSize, WHITE);
        DrawCircle(centerX - eyeOffsetX, centerY - eyeOffsetY, eyeSize/2, BLACK);
        DrawCircle(centerX + eyeOffsetX, centerY - eyeOffsetY, eyeSize/2, BLACK);
    } else {
        // Normale Augen für normale Geister
        DrawCircle(centerX - eyeOffsetX, centerY - eyeOffsetY, eyeSize, WHITE);
        DrawCircle(centerX - eyeOffsetX + eyeSize/3, centerY - eyeOffsetY, eyeSize/2, BLACK);
        
        DrawCircle(centerX + eyeOffsetX, centerY - eyeOffsetY, eyeSize, WHITE);
        DrawCircle(centerX + eyeOffsetX + eyeSize/3, centerY - eyeOffsetY, eyeSize/2, BLACK);
    }
};

void Ghostbase::setFrightened(bool on) {
    if (on) {
        changeState(GhostState::FRIGHTENED);
        stateTimer = 10.0f; // 10 Sekunden verängstigt
        moveInterval = 1.0f / (speed * 0.5f); // Langsamere Bewegung im Frightened-Modus
    } else if (!on && state == GhostState::FRIGHTENED) {
        changeState(GhostState::CHASE);
        stateTimer = 10.0f; // Nach Frightened wieder zum Chase-Modus
        moveInterval = 1.0f / speed; // Normale Geschwindigkeit wiederherstellen
    }
}

void Ghostbase::getEaten() {
    if (state == GhostState::FRIGHTENED) {
        // Geist wird sofort zur Spawn-Position teleportiert
        x = spawnX;
        y = spawnY;
        // Direkt zu inbase-Modus wechseln (nicht EATEN)
        changeState(GhostState::IN_BASE);
        stateTimer = 2.0f; // 7 Sekunden im Scatter-Modus
        moveInterval = 1.0f / speed; // Normale Geschwindigkeit wiederherstellen
    }
}

bool Ghostbase::canBeEaten() const {
    return state == GhostState::FRIGHTENED;
}

bool Ghostbase::isFrightened() const {
    return state == GhostState::FRIGHTENED;
}

void Ghostbase::reset() {
    // Zurück auf Startposition, State zurücksetzen
    state = GhostState::IN_BASE;
    stateTimer = 2.0f; // 2 Sekunden warten bevor sie die Basis verlassen
    moveTimer = 0.0f;
}

// Protected-Helfer
void Ghostbase::changeState(GhostState newState) {
    state = newState;
    // State-abhängige Timer setzen
}

Vector2 Ghostbase::chooseDirectionTowards(const Vector2& target) const {
    static const Vector2 options[4] = {
        {  1,  0 },
        { -1,  0 },
        {  0,  1 },
        {  0, -1 }
    };

    float oppX = -dirX;
    float oppY = -dirY;

    float bestDist = FLT_MAX;
    Vector2 bestMove{0, 0};

    for (auto& opt : options) {
        if ((int)opt.x == oppX && (int)opt.y == oppY) continue;

        int nx = x + (int)opt.x;
        int ny = y + (int)opt.y;

        if (!mapRef.isWalkable(nx, ny)) continue;

        //Sackgassen vermeiden: Prüfen, wie viele Wege es vom nächsten Feld gibt
        int exits = 0;

        for (auto& nextOpt : options) {
            int ex = nx + (int)nextOpt.x;
            int ey = ny + (int)nextOpt.y;

            // nicht Rückrichtung zählen
            if ((int)nextOpt.x == -opt.x && (int)nextOpt.y == -opt.y) continue;
            if (mapRef.isWalkable(ex, ey)) exits++;
        }

        if (exits == 0) continue; // Sackgasse → überspringen

        // normale Distanzberechnung
        float dist = fabsf((nx - target.x)) + fabsf((ny - target.y));
        if (dist < bestDist) {
            bestDist = dist;
            bestMove = opt;
        }
    }

    if (bestDist == FLT_MAX) {
        return Vector2{ oppX, oppY };
    }
    return bestMove;
}

// Private-Helfer
Vector2 Ghostbase::randomTile() const {
    // Improved random movement for frightened mode
    // Try to move away from current position in a random direction
    static const Vector2 directions[4] = {
        {  1,  0 },  // right
        { -1,  0 },  // left
        {  0,  1 },  // down
        {  0, -1 }   // up
    };
    
    std::vector<Vector2> validMoves;
    
    // Find all valid directions
    for (int i = 0; i < 4; i++) {
        int testX = x + (int)directions[i].x;
        int testY = y + (int)directions[i].y;
        
        if (mapRef.isWalkable(testX, testY)) {
            validMoves.push_back(directions[i]);
        }
    }
    
    if (validMoves.empty()) {
        return Vector2{ (float)x, (float)y }; // Stay in place if no valid moves
    }
    
    // Choose random valid direction
    int randomIndex = rand() % validMoves.size();
    Vector2 chosenDir = validMoves[randomIndex];
    
    return Vector2{ (float)(x + (int)chosenDir.x), (float)(y + (int)chosenDir.y) };
}

