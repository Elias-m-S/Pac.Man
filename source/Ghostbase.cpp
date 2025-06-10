
#include "Ghostbase.h"
#include <iostream>
#include <algorithm> //für algorythmen
#include <random> //für zufällige Komponenten
#include <cfloat> //für FLT_MAX, um unendlich roße werte zu haben

Ghostbase::Ghostbase(const Map& map, int startX, int startY, float speed)
    : Entity(startX, startY, speed),
      frightenedColor(BLUE),
      radius(14), // Feste Radius für Geister
      state(GhostState::IN_BASE),
      stateTimer(2.0f), // 2 Sekunden warten bevor sie die Basis verlassen
      moveTimer(0.0f),
      moveInterval(1.0f / speed), // Bewegungsintervall: 1/speed Sekunden pro Tile
      mapRef(map),
      rng(std::random_device{}())
{

}

Ghostbase::~Ghostbase() {
    //Destruktor, nicht umbedingt nötig, aber gut für debugging
}

void Ghostbase::update(float deltaTime, const Vector2& pacmanPos, const Map& map) {

    stateTimer -= deltaTime;
    moveTimer -= deltaTime;

    // Automatischer State-Wechsel
    if (stateTimer <= 0.0f) {
        if (state == GhostState::IN_BASE) {
            changeState(GhostState::SCATTER);
            stateTimer = 7.0f; // 7 Sekunden im Scatter-Modus
        } else if (state == GhostState::SCATTER && state != GhostState::FRIGHTENED) {
            changeState(GhostState::CHASE);
            stateTimer = 20.0f; // 20 Sekunden im Chase-Modus
        } else if (state == GhostState::CHASE && state != GhostState::FRIGHTENED) {
            changeState(GhostState::SCATTER);
            stateTimer = 7.0f; // 7 Sekunden im Scatter-Modus
        }
    }

    // Nur bewegen wenn Move-Timer abgelaufen ist (diskrete Tile-Bewegung)
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
    Color drawColor = (state == GhostState::FRIGHTENED)
                      ? frightenedColor
                      : normalColor;

    //Die Mitte der tiles festlegen als "Anker"
    int centerX = x * tileSize + tileSize/2;
    int centerY = y * tileSize + tileSize/2;
    //Basierend auf TileSize den Radius der geister bestimmen
    int ghostRadius = tileSize/2 - 2;
    
    // Kreis(Kopfrundung)der Geister
    DrawCircle(centerX, centerY - ghostRadius/4, ghostRadius, drawColor);
    
    // rechteckiger Körper (aka das Betttuch was runterhängt)
    DrawRectangle(centerX - ghostRadius, centerY - ghostRadius/4, 
                  ghostRadius * 2, ghostRadius + ghostRadius/2, drawColor);
    
    
    // Augen Zeichnen, aber anhängig von den zuständen
    if (state != GhostState::FRIGHTENED) {
        int eyeSize = ghostRadius / 4;
        int eyeOffsetX = ghostRadius / 3;
        int eyeOffsetY = ghostRadius / 3;
        
        // Linkes Auge
        DrawCircle(centerX - eyeOffsetX, centerY - eyeOffsetY, eyeSize, WHITE);
        DrawCircle(centerX - eyeOffsetX + eyeSize/3, centerY - eyeOffsetY, eyeSize/2, BLACK);
        
        // Rechtes Auge
        DrawCircle(centerX + eyeOffsetX, centerY - eyeOffsetY, eyeSize, WHITE);
        DrawCircle(centerX + eyeOffsetX + eyeSize/3, centerY - eyeOffsetY, eyeSize/2, BLACK);
    } else {
        // Wenn Geister verängstigt: Zeichne X-Augen oder andere verängstigt aussehende Augen
        int eyeSize = ghostRadius / 4;
        int eyeOffsetX = ghostRadius / 3;
        int eyeOffsetY = ghostRadius / 3;
        
        DrawCircle(centerX - eyeOffsetX, centerY - eyeOffsetY, eyeSize, WHITE);
        DrawCircle(centerX + eyeOffsetX, centerY - eyeOffsetY, eyeSize, WHITE);
        
        // Zeichne "ängstliche" Punkte in den Augen
        DrawCircle(centerX + eyeOffsetX, centerY - eyeOffsetY, eyeSize/3, BLACK);
        DrawCircle(centerX - eyeOffsetX, centerY - eyeOffsetY, eyeSize/3, BLACK);
    }
};

void Ghostbase::setFrightened(bool on) {
    if (on) {
        changeState(GhostState::FRIGHTENED);
        stateTimer = 10.0f; // 10 Sekunden verängstigt
    } else {
        changeState(GhostState::CHASE);
        stateTimer = 20.0f; // Nach Frightened wieder zum Chase-Modus
    }
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
    // Richtungen welche möglich sind, static um nicht bei jedem Aufruf neu zu erstellen
    static const Vector2 options[4] = {
        {  1,  0 },  // rechts
        { -1,  0 },  // links
        {  0,  1 },  // runter
        {  0, -1 }   // rauf
    };

    //Zurücklaufen auschließen
    float oppX = -dirX;//setzt entgegengesetzte Richtung
    float oppY = -dirY;

    
    float bestDist = FLT_MAX; //größtmöglicher floatwert, um beste Distanz finden zu können
    Vector2 bestMove{0,0}; //standardwert, falls keine Richtung gefunden

    for (auto& opt : options) {
        // Zurückgehen überspringen
        if ((int)opt.x == oppX && (int)opt.y == oppY) continue;

        int nx = x + (int)opt.x;
        int ny = y + (int)opt.y;
        //checken ob das nächste Feld überhupt begehbar ist
        if (!mapRef.isWalkable(nx, ny)) continue;

        // Kürzeste Distanz (Fachbegriff Manhatten- Distanz) berechnen
        float dist = fabsf((nx - target.x)) + fabsf((ny - target.y));// fabsf aus header, um negative Werte zu vermeiden
        if (dist < bestDist) {
            bestDist = dist;//gleichsetzen mit der berechneten distanz
            bestMove = opt;//beste richtung setzen
        }
    }

    //Wenn keine Richtung gefunden wurde, soll der Geist umkehren
    if (bestDist == FLT_MAX) {
        // Fall wie z.B. Sackgassen(selten)
        return Vector2{ oppX, oppY };
    }
    return bestMove;
}

// Private-Helfer
Vector2 Ghostbase::randomTile() const {
    // Wähle zufällige begehbare Kachel auf mapRef (für z.B. Scatter-Zustand in Frightend)"noch zu erledigen"
    return Vector2{ (float)x, (float)y };//umwandlung zu float für Vector
}

