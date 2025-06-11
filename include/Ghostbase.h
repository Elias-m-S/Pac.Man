#pragma once
#include "Entity.h"
#include "Map.h"
#include "Pacman.h"


class Ghostbase : public Entity{
      // Zustände welche die Geister annehmen können
    enum class GhostState {
        IN_BASE,     // Geister sind noch in der Basis und müssen zuerst hinaus
        SCATTER,     // Geister bewegen sich weg vom "Spawn" zu ihren Ecken
        CHASE,       // geister vefolgen/jagen pacman
        FRIGHTENED,  // Geister haben angst und vergessen ihr Jagdmuster in bezug auf Pacman
        EATEN        // Geister wurden gefressen, kehren zum Spawnpunkt zurück und fangen erst dort an den kreislauf von vore zu beginnen
    };
    
    public: // sind für das ganze Programm sichtbar

        //Konstroktor | Map für Kollision und items | start X&Y für Spawnpoint | speed für speed
        Ghostbase (const Map& map, int startX, int startY, float speed = 1);
        virtual ~Ghostbase(); //Destrukor, virtual, damit die abgeieteten klassen auch destruktet werden
        // Update Funktion, wird von abgleitenden Klassen überschrieben| lastTime: Zeit letztes Update, für z.B. Timer der Zustände| PacmanPos für Position Pacman -> Zielberechnung
        virtual void update(float lastTime, const Vector2& pacmanPos, const Map& map);

        //überschreibt draw funktion von Entity | tileSize: größe einer kachel
        void draw(int tileSize) const override;

          //versetzt nach bedingung alle Geister in Angst
        void setFrightened(bool on);
          // Wird gefressen von PacMan (nur im Frightened-Modus möglich)
        void getEaten();
        
        // Prüft ob der Geist von PacMan gefressen werden kann
        bool canBeEaten() const;
        
        // Prüft ob der Geist "tot" ist und zur Basis zurückkehrt
        bool isEaten() const;

        //Lässt geister 
        void reset();

        //Pure virtuelle Funktion, denn, jeder Geist überschreibt mit eigenen werten -> andere Algorythmen. 
        virtual Vector2 getTargetTile(const Vector2& pacmanPos) const = 0;
        
        // Pure virtuelle Funktion für Scatter-Ziel (jeder Geist hat seine eigene Ecke)
        virtual Vector2 getScatterTarget() const = 0;

        bool isFrightened() const;

        void resetPosition(int x, int y) {//einfach hier ganz rein weil eh winzig
            this->x = x;
            this->y = y;
        }
    

    protected:// können/sollen von allen abgeleiteten Klassen gesehen und verwendet werden, aber nicht von extern

        //wechselt Zustand der geister, erstellt timer für den Zustand
        void changeState(GhostState newState);

        //berechnet welch richtung gewählt werden muss um Target zu erreichen
        Vector2 chooseDirectionTowards(const Vector2& target) const;        Color normalColor; // normale Farbe welche jeder geist hat(verschiedene deswegen nicht festgelegt)
        Color frightenedColor = BLUE; // farbe welche die geister haben wenn sie angst haben
        Color eatenColor = DARKGRAY; // farbe für gefressene Geister (nur Augen sichtbar)
        float radius; //radius der Geister, bestimmt die form der geister mit
        GhostState state; //Tatsächlicher Zustand des Geistes: SCATTER, CHASE, FRIGHTENED, EATEN
        float stateTimer; //timer um ablauf der Zustände zu verwalten
        float moveTimer; // Timer für diskrete Tile-basierte Bewegung 
        float moveInterval; // Bewegungsintervall basierend auf Geschwindigkeit
        const Map& mapRef; //referenz auf die Map, um Kollisionen zu prüfen
        
        // Spawn-Position für EATEN Zustand
        int spawnX, spawnY;
        

    private: //sind nur in Ghostbase sichtbar, wie z.B. Form


        // Hilfsfunktionen
        std::mt19937 rng; //Zufallszahlengenerator
        //Zufälige Tile (z.B. für Scatter Zustand)
        Vector2 randomTile() const;


};
