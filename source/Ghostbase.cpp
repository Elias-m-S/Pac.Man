
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


void Ghostbase::draw(int tileSize) const {
    Color drawColor = (state == GhostState::FRIGHTENED)
                      ? frightenedColor
                      : normalColor;
    DrawCircle(x * tileSize + tileSize/2,
               y * tileSize + tileSize/2,
               radius,
               drawColor);
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

