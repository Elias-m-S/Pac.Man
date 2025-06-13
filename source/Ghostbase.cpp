#include "Ghostbase.h"
#include <iostream>
#include <algorithm> //für algorythmen
#include <random> //für zufällige Komponenten
#include <cfloat> //für FLT_MAX, um unendlich roße werte zu haben
#include <cstdlib> //für rand()

bool movingAllowed = false;

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

    // Timer Debugging
    //std::cout << "State Timer: " << stateTimer << std::endl;
    //std::cout << "Move Timer: " << moveTimer << std::endl;

    // Simplified state transitions
    if (stateTimer <= 0.0f) {        if (state == GhostState::IN_BASE) {
            // Leave base after initial wait
            changeState(GhostState::SCATTER);
            movingAllowed = true; // Allow movement after leaving base
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
    if (moveTimer <= 0.0f && movingAllowed) {
        moveTimer = moveInterval; // Timer zurücksetzen
        Vector2 target;
        if (state == GhostState::IN_BASE) {
            // Ziel: Basis verlassen (gehe nach oben zur Tür bei Position 10,7)
            target = Vector2{10, 7};
        } else if (state == GhostState::FRIGHTENED) {
            target = randomTile();        } else if (state == GhostState::SCATTER) {
            target = getScatterTarget(); // Zur eigenen Ecke gehen
            
            // Prüfe ob bereits am Ziel angekommen
            if (x == (int)target.x && y == (int)target.y) {
                // Am Scatter-Ziel angekommen - nicht mehr bewegen
                return; // Überspringt die Bewegungslogik
            }
            
            // Wenn Geist im Tunnel ist, sofort raus in Richtung Mitte
            if (x <= 0 || x >= mapRef.getWidth() - 1) {
                if (x <= 0) {
                    target = Vector2{mapRef.getWidth() / 2.0f, (float)y}; // Nach rechts zur Mitte
                } else {
                    target = Vector2{mapRef.getWidth() / 2.0f, (float)y}; // Nach links zur Mitte
                }
            }

            // Debug-Ausgabe für Scatter-Ziel
            //std::cout << "Scatter Target fuer " << typeid(*this).name() << ": " << target.x << ", " << target.y << std::endl;

        } else { // CHASE Modus
            target = getTargetTile(pacmanPos); // Pacman verfolgen
        }        // Bewegung in Richtung ausgewählten Ziels
        Vector2 dir = chooseDirectionTowards(target);
        setDirection((int)dir.x, (int)dir.y);
        
        // Debug-Ausgabe für direction
        //std::cout << "Direction fuer " << typeid(*this).name() << ": " << dir.x << ", " << dir.y << std::endl;        // Diskrete Tile-Bewegung: immer genau 1 Tile bewegen
        int nextX = x + getDirX();
        int nextY = y + getDirY();
        
        // Zusätzliche Tunnel-Prüfung vor der Bewegung (im Scatter-Modus)
        if (state == GhostState::SCATTER) {
            if (nextX <= 0 || nextX >= mapRef.getWidth() - 1) {
                // Tunnel erkannt! Bewegung verhindern
                return;
            }
        }
        
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
        stateTimer = 6.0f; // 6 Sekunden verängstigt
        moveInterval = 1.0f / (speed * 0.5f); // Langsamere Bewegung im Frightened-Modus
    } else if (!on && state == GhostState::FRIGHTENED) {
        changeState(GhostState::CHASE);
        stateTimer = 6.0f; // Nach Frightened wieder zum Chase-Modus
        moveInterval = 1.0f / speed; // Normale Geschwindigkeit wiederherstellen
    }
}

void Ghostbase::getEaten() {
    if (state == GhostState::FRIGHTENED) {
        // Geist wird sofort zur Spawn-Position teleportiert
        x = spawnX;
        y = spawnY;
        // Direkt zu Chase-Modus wechseln (kein Scatter nach dem Tod)
        changeState(GhostState::CHASE);
        stateTimer = FLT_MAX; // Unbegrenzt im Chase-Modus bleiben
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
    static const Vector2 directions[4] = {
        {  1,  0 },  // rechts
        { -1,  0 },  // links
        {  0,  1 },  // runter
        {  0, -1 }   // rauf
    };    // Aktuelle Richtung (entgegengesetzt = Rückwärts)
    Vector2 opposite = { (float)(-dirX), (float)(-dirY) };
    
    struct Option {
        Vector2 direction;
        float distance;
        int exits;
        bool isReverse;
    };
    
    std::vector<Option> validOptions;
      // Alle möglichen Richtungen prüfen
    for (const auto& dir : directions) {
        int nextX = x + (int)dir.x;
        int nextY = y + (int)dir.y;        // Nicht begehbar? Überspringen
        if (!mapRef.isWalkable(nextX, nextY)) continue;
        
        // Tunnel vermeiden im Scatter-Modus (linke und rechte Ränder)
        if (state == GhostState::SCATTER) {
            if (nextX <= 0 || nextX >= mapRef.getWidth() - 1) {
                continue; // Tunnel-Position überspringen
            }
        }
        
        // Ist das Rückwärts?
        bool isReverse = ((int)dir.x == (int)opposite.x && (int)dir.y == (int)opposite.y);
        
        // Distanz zum Ziel berechnen (Manhattan-Distanz)
        float distance = fabsf(nextX - target.x) + fabsf(nextY - target.y);
        
        // Anzahl Ausgänge vom nächsten Feld zählen (Sackgassen-Vermeidung)
        int exits = 0;
        for (const auto& checkDir : directions) {
            int checkX = nextX + (int)checkDir.x;
            int checkY = nextY + (int)checkDir.y;
            if (mapRef.isWalkable(checkX, checkY)) {
                exits++;
            }
        }
        
        validOptions.push_back({dir, distance, exits, isReverse});
    }
    
    // Keine gültigen Optionen? Notfall: Rückwärts gehen
    if (validOptions.empty()) {
        return opposite;
    }
    
    // Sortierung nach Priorität:
    // 1. Nicht rückwärts gehen (außer es ist die einzige Option)
    // 2. Sackgassen vermeiden (exits > 1)
    // 3. Kürzeste Distanz zum Ziel
    std::sort(validOptions.begin(), validOptions.end(), 
        [](const Option& a, const Option& b) {
            // Rückwärts ist immer schlechter (außer beide sind rückwärts)
            if (a.isReverse != b.isReverse) {
                return !a.isReverse;  // a ist besser wenn es nicht rückwärts ist
            }
            
            // Sackgassen vermeiden (mehr Ausgänge = besser)
            if (a.exits != b.exits) {
                return a.exits > b.exits;
            }
            
            // Bei gleichen Ausgängen: kürzeste Distanz
            return a.distance < b.distance;
        });
    
    // Beste Option zurückgeben
    return validOptions[0].direction;
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

