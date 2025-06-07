#pragma once
#include "Entity.h"
#include "Map.h"
#include "Pacman.h"


// Beispiel: Basisklasse Ghost
class Ghostbase : public Entity {
    public:
        enum class State { Chase, Scatter, Frightened };
        protected:
        State currentState; // einführen einer Variable umd Zustände der Geister zu dokumenteiren/überprüfen
        Color normalColor; //Normalfarbe des Geistes, nonstatic um instanz für jeden geist zu kopieren
        const Color frightenedColor;// Farbe im "Frightened" Zustand, nonstatic um instanz für jeden geist zu kopieren

    public:
        Ghostbase(Vector2 pos, Color color) : Entity(pos.x, pos.y), normalColor(color), frightenedColor(BLUE), currentState(State::Scatter) {}
        
        virtual void update(float dt, const PacMan &pacman);
        virtual void draw();
        virtual Vector2 getTarget(const PacMan &pacman) = 0; // von Unterklassen zu implementieren
        void setState(State s) { currentState = s; };
    
    protected:
        virtual void handleStateduration(float dt);
};