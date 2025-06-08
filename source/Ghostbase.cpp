
#include "Ghostbase.h"
#include <iostream>
#include <algorithm> //für algorythmen
#include <random> //für zufällige Komponenten

Ghostbase::Ghostbase(const Map& map, int startX, int startY, int speed)
    : Entity(startX, startY, speed),
      frightenedColor(BLUE),
      radius(14), // Feste Radius für Geister
      state(GhostState::SCATTER),
      stateTimer(0.0f),
      mapRef(map),
      rng(std::random_device{}())
{

}

Ghostbase::~Ghostbase() {
    //Destruktor, nicht umbedingt nötig, aber gut für debugging
}

void Ghostbase::update(float deltaTime, const Vector2& pacmanPos) {

    stateTimer -= deltaTime;

    // (State-Wechsellogik hier einfügen)
    //z.B. Wenn powerup gegessen -> Geister in FRIGHTENED Zustand
    // (Hier State-Wechsel-Logik einbauen)

    // 2) Ziel berechnen (Polymorphie)

    Vector2 target = (state == GhostState::FRIGHTENED) ? randomTile() : getTargetTile(pacmanPos);


    // 3) Bewegung in Richtung Ziel
    Vector2 dir = chooseDirectionTowards(target);
    setDirection((int)dir.x, (int)dir.y);
    Entity::move();

    // 4) Tunnel-Logik (wie bei PacMan)
    //    …  

    // 5) Sammle keine Items, aber handle Kollision mit PacMan extern
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
    changeState(on ? GhostState::FRIGHTENED : GhostState::CHASE);
    // ggf. Timer neu setzen
}

void Ghostbase::reset() {
    // Zurück auf Startposition, State zurücksetzen
}

// Protected-Helfer
void Ghostbase::changeState(GhostState newState) {
    state = newState;
    // State-abhängige Timer setzen
}

Vector2 Ghostbase::chooseDirectionTowards(const Vector2& target) const {
    // Simple Heuristik: versuche die Nachbarkachel mit geringster Distanz
    // (Implementierung hier)
    return Vector2{ dirX, dirY };
}

// Private-Helfer
Vector2 Ghostbase::randomTile() const {
    // Wähle zufällige begehbare Kachel auf mapRef
    return Vector2{ x, y };

}

