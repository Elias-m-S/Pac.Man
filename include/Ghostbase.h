#pragma once
#include "Entity.h"
#include "Map.h"
#include "Pacman.h"
#include <random>

class Ghostbase : public Entity {
public:    // Zustände welche die Geister annehmen können
    enum class GhostState {
        IN_BASE,     // Geister sind noch in der Basis und müssen zuerst hinaus
        SCATTER,     // Geister bewegen sich weg vom "Spawn" zu ihren Ecken
        CHASE,       // Geister verfolgen/jagen Pacman
        FRIGHTENED   // Geister haben Angst und vergessen ihr Jagdmuster in Bezug auf Pacman
    };
    
    // Konstruktor | Map für Kollision und Items | start X&Y für Spawnpoint | speed für Geschwindigkeit
    Ghostbase(const Map& map, int startX, int startY, float speed = 1);
    
    // Update Funktion, wird von abgeleiteten Klassen überschrieben
    // lastTime: Zeit letztes Update, für z.B. Timer der Zustände
    // pacmanPos für Position Pacman -> Zielberechnung
    virtual void update(float lastTime, const Vector2& pacmanPos, const Map& map);

    // Überschreibt draw Funktion von Entity | tileSize: Größe einer Kachel
    void draw(int tileSize) const override;    // Versetzt nach Bedingung alle Geister in Angst
    void setFrightened(bool on);
    
    // Wird gefressen von PacMan (nur im Frightened-Modus möglich)
    void getEaten();
    
    // Prüft ob der Geist von PacMan gefressen werden kann
    bool canBeEaten() const;

    // Setzt Geister zurück
    void reset();

    // Pure virtuelle Funktion, jeder Geist überschreibt mit eigenen Werten -> andere Algorithmen
    virtual Vector2 getTargetTile(const Vector2& pacmanPos) const = 0;
    
    // Pure virtuelle Funktion für Scatter-Ziel (jeder Geist hat seine eigene Ecke)
    virtual Vector2 getScatterTarget() const = 0;

    // Prüft ob der Geist im Frightened-Zustand ist
    bool isFrightened() const;

    // Setzt Position des Geistes zurück
    void resetPosition(int x, int y) {
        this->x = x;
        this->y = y;
    }

protected:
    // Wechselt Zustand der Geister, erstellt Timer für den Zustand
    void changeState(GhostState newState);

    // Berechnet welche Richtung gewählt werden muss um Target zu erreichen
    Vector2 chooseDirectionTowards(const Vector2& target) const;
      // Farben
    Color normalColor;         // Normale Farbe welche jeder Geist hat (verschiedene deswegen nicht festgelegt)
    Color frightenedColor = BLUE; // Farbe welche die Geister haben wenn sie Angst haben
    
    // Eigenschaften
    float radius;              // Radius der Geister, bestimmt die Form der Geister mit
    GhostState state;          // Tatsächlicher Zustand des Geistes: SCATTER, CHASE, FRIGHTENED
    float stateTimer;          // Timer um Ablauf der Zustände zu verwalten
    float moveTimer;           // Timer für diskrete Tile-basierte Bewegung 
    float moveInterval;        // Bewegungsintervall basierend auf Geschwindigkeit
    const Map& mapRef;         // Referenz auf die Map, um Kollisionen zu prüfen
    
    // Spawn-Position für Reset und Teleport
    int spawnX, spawnY;

private:
    // Hilfsfunktionen
    std::mt19937 rng;          // Zufallszahlengenerator
    
    // Zufällige Tile (z.B. für Scatter Zustand)
    Vector2 randomTile() const;
};
