#pragma once
#include "Entity.h"
#include "Map.h"
#include "Pacman.h"


class Ghostbase : public Entity{
    
    // Zustände welche die Geister annehmen können
    enum class GhostState {
        SCATTER, //Geister bewegen sich weg vom "Spawn" zu ihren Ecken
        CHASE, //geister vefolgen/jagen pacman
        FRIGHTENED, //Geister haben angst und vergessen ihr Jagdmuster in bezug auf Pacman
        EATEN //Geister wurden gefressen, kehren zum Spawnpunkt zurück und fangen erst dort an den kreislauf von vore zu beginnen
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

        //Lässt geister 
        void reset();

        //Pure virtuelle Funktion, denn, jeder Geist überschreibt mit eigenen werten -> andere Algorythmen. 
        virtual Vector2 getTargetTile(const Vector2& pacmanPos) const = 0;

        bool isFrightened() const { return state == GhostState::FRIGHTENED; }

        void resetPosition(int x, int y) {//einfach hier ganz rein weil eh winzig
            this->x = x;
            this->y = y;
        }
    

    protected:// können/sollen von allen abgeleiteten Klassen gesehen und verwendet werden, aber nicht von extern

        //wechselt Zustand der geister, erstellt timer für den Zustand
        void changeState(GhostState newState);

        //berechnet welch richtung gewählt werden muss um Target zu erreichen
        Vector2 chooseDirectionTowards(const Vector2& target) const;

        Color normalColor; // normale Farbe welche jeder geist hat(verschiedene deswegen nicht festgelegt)
        Color frightenedColor = BLUE; // farbe welche die geister haben wenn sie angst haben
        float radius; //radius der Geister, bestimmt die form der geister mit
        GhostState state; //Tatsächlicher Zustand des Geistes: SCATTER, CHASE, FRIGHTENED, EATEN
        float stateTimer; //timer um ablauf der Zustände zu verwalten
        const Map& mapRef; //referenz auf die Map, um Kollisionen zu prüfen
        

    private: //sind nur in Ghostbase sichtbar, wie z.B. Form


        // Hilfsfunktionen
        std::mt19937 rng; //Zufallszahlengenerator
        //Zufälige Tile (z.B. für Scatter Zustand)
        Vector2 randomTile() const;


};
