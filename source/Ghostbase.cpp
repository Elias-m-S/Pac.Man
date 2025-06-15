#include "Ghostbase.h"
#include <iostream>
#include <algorithm> // für Algorithmen
#include <random> // für Zufall
#include <cfloat> // für FLT_MAX, damit ich quasi "unendlich" große Werte habe
#include <cstdlib> // für rand()

bool movingAllowed = false;

Ghostbase::Ghostbase(const Map& map, int startX, int startY, float speed)
    : Entity(startX, startY, speed),
      radius(14), // Der Geist ist immer 14 Pixel groß (Radius)
      state(GhostState::IN_BASE),
      stateTimer(2.0f), // Am Anfang wartet der Geist 2 Sekunden in der Base
      moveTimer(0.0f),
      moveInterval(1.0f / speed), // Wie oft darf sich der Geist bewegen? (je schneller, desto kleiner das Intervall)
      mapRef(map),
      spawnX(startX), spawnY(startY), // Ich merke mir, wo der Geist spawnt
      rng(std::random_device{}())
{

}

void Ghostbase::update(float deltaTime, const Vector2& pacmanPos, const Map& map) {
    moveTimer -= deltaTime;
    stateTimer -= deltaTime;

    // Debugging: Hier könnte ich mir die Timer ausgeben lassen, falls was nicht klappt
    //std::cout << "State Timer: " << stateTimer << std::endl;
    //std::cout << "Move Timer: " << moveTimer << std::endl;

    // Hier wird geprüft, ob der aktuelle Zustand abgelaufen ist (z.B. Base-Zeit, Scatter-Zeit, Frightened-Zeit)
    if (stateTimer <= 0.0f) {
        if (state == GhostState::IN_BASE) {
            // Wenn die Wartezeit in der Base vorbei ist, gehe in den Scatter-Modus
            changeState(GhostState::SCATTER);
            movingAllowed = true; // Ab jetzt darf sich der Geist bewegen
            stateTimer = 7.0f; // Im Scatter-Modus bleibt der Geist 7 Sekunden
        }
        else if (state == GhostState::SCATTER) {
            // Nach dem Scatter-Modus kommt der Chase-Modus (dauerhaft)
            changeState(GhostState::CHASE);
            stateTimer = FLT_MAX; // Der Timer läuft quasi nie ab, Geist bleibt im Chase
        }
        else if (state == GhostState::FRIGHTENED) {
            // Wenn der Frightened-Modus vorbei ist, wieder auf Chase umschalten
            changeState(GhostState::CHASE);
            stateTimer = FLT_MAX; // Auch hier: Chase bleibt für immer
            moveInterval = 1.0f / speed; // Geschwindigkeit wieder normal machen
        }
        // Es gibt keinen Wechsel mehr zurück zu Scatter
    }

    // Ab hier kommt die eigentliche Bewegungslogik
    if (moveTimer <= 0.0f && movingAllowed) {
        moveTimer = moveInterval; // Timer für die nächste Bewegung zurücksetzen
        Vector2 target;
        if (state == GhostState::IN_BASE) {
            // Wenn ich noch in der Base bin, will ich zur Tür (Position 10,7)
            target = Vector2{10, 7};
        } else if (state == GhostState::FRIGHTENED) {
            // Wenn ich erschrocken bin, laufe ich einfach irgendwohin (Zufall)
            target = randomTile();
        } else if (state == GhostState::SCATTER) {
            // Im Scatter-Modus will ich in meine Ecke
            target = getScatterTarget();
            // Bin ich schon in der Ecke angekommen?
            if (x == (int)target.x && y == (int)target.y) {
                // Dann bleibe ich einfach stehen und mache nichts mehr
                return; // Ich überspringe die Bewegung
            }
            // Falls ich im Tunnel bin, gehe ich sofort wieder raus Richtung Mitte
            if (x <= 0 || x >= mapRef.getWidth() - 1) {
                target = Vector2{mapRef.getWidth() / 2.0f, (float)y}; // Ziel ist die Mitte
            }
            // Hier könnte ich mir das Ziel zum Debuggen ausgeben lassen
            //std::cout << "Scatter Target fuer " << typeid(*this).name() << ": " << target.x << ", " << target.y << std::endl;
        } else { // CHASE Modus
            // Im Chase-Modus will ich PacMan jagen
            target = getTargetTile(pacmanPos);
        }
        // Jetzt suche ich mir die beste Richtung zum Ziel
        Vector2 dir = chooseDirectionTowards(target);
        setDirection((int)dir.x, (int)dir.y);
        // Hier könnte ich mir die Richtung zum Debuggen ausgeben lassen
        //std::cout << "Direction fuer " << typeid(*this).name() << ": " << dir.x << ", " << dir.y << std::endl;
        // Ich bewege mich immer genau ein Feld weiter
        int nextX = x + getDirX();
        int nextY = y + getDirY();
        // Im Scatter-Modus darf ich nicht in den Tunnel laufen
        if (state == GhostState::SCATTER) {
            if (nextX <= 0 || nextX >= mapRef.getWidth() - 1) {
                // Tunnel erkannt! Ich bleibe stehen
                return;
            }
        }
        // Prüfen, ob das nächste Feld begehbar ist
        if (mapRef.isWalkable(nextX, nextY)) {
            x = nextX;
            y = nextY;
            // Falls ich am Rand bin, Tunnel benutzen
            handleTunnelWrap(map.getWidth());
        }
    }
}

// Zeichnen: Hier wird der Geist gemalt. Die Farbe kann jede Geisterklasse selbst setzen.
void Ghostbase::draw(int tileSize) const {
    Color drawColor;
    if (state != GhostState::FRIGHTENED) {
        drawColor = normalColor;
    }
    // Die Mitte des Feldes berechnen, damit der Geist immer schön mittig sitzt
    int centerX = x * tileSize + tileSize/2;
    int centerY = y * tileSize + tileSize/2;
    //Basierend auf TileSize den Radius der geister bestimmen
    int ghostRadius = tileSize/2 - 2;
    
    // Körper nur zeichnen wenn NICHT im Frightened Modus
    if (state != GhostState::FRIGHTENED) {
        // Kopf vom Geist malen (Kreis)
        DrawCircle(centerX, centerY - ghostRadius/4, ghostRadius, drawColor);
        // Körper vom Geist malen (Rechteck, sieht aus wie ein Laken)
        DrawRectangle(centerX - ghostRadius, centerY - ghostRadius/4, 
                      ghostRadius * 2, ghostRadius + ghostRadius/2, drawColor);
    }    
    // Jetzt kommen die Augen. Die sind je nach Zustand unterschiedlich groß
    int eyeSize = ghostRadius / 4;
    int eyeOffsetX = ghostRadius / 3;
    int eyeOffsetY = ghostRadius / 3;
    if (state == GhostState::FRIGHTENED) {
        // Wenn der Geist Angst hat, sind die Augen größer und auffälliger
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
        // Wenn PacMan ein PowerUp frisst, werden alle Geister erschrocken
        changeState(GhostState::FRIGHTENED);
        stateTimer = 6.0f; // So lange bleibt der Geist erschrocken
        moveInterval = 1.0f / (speed * 0.5f); // Geist wird langsamer
    } else if (!on && state == GhostState::FRIGHTENED) {
        // Wenn die Angst vorbei ist, wieder normal jagen
        changeState(GhostState::CHASE);
        stateTimer = 6.0f; // Timer für Chase-Modus (eigentlich egal, weil FLT_MAX)
        moveInterval = 1.0f / speed; // Geschwindigkeit wieder normal
    }
}

void Ghostbase::getEaten() {
    if (state == GhostState::FRIGHTENED) {
        // Wenn PacMan mich frisst, werde ich sofort zur Startposition teleportiert
        x = spawnX;
        y = spawnY;
        // Danach jage ich sofort wieder PacMan (kein Scatter mehr)
        changeState(GhostState::CHASE);
        stateTimer = FLT_MAX; // Bleibe für immer im Chase-Modus
        moveInterval = 1.0f / speed; // Geschwindigkeit wieder normal
    }
}

//zwei Funktionen gleiche Funktionalität, aber für unterschiedliche Zwecke übersichtlich eigene Namen
bool Ghostbase::canBeEaten() const {
    // Ich kann nur gefressen werden, wenn ich Angst habe
    return state == GhostState::FRIGHTENED;
}
bool Ghostbase::isFrightened() const {
    // Bin ich gerade im Frightened-Modus?
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


//Zufällige Felder suchen, welche die Geister in Frightened Zustand ansteuern können
Vector2 Ghostbase::randomTile() const {
    static const Vector2 directions[4] = {
        {  1,  0 },  // right
        { -1,  0 },  // left
        {  0,  1 },  // down
        {  0, -1 }   // up
    };
    
    std::vector<Vector2> validMoves;
    
    //alle möglichen richtungen prüfen
    for (int i = 0; i < 4; i++) {
        int testX = x + (int)directions[i].x;
        int testY = y + (int)directions[i].y;
        
        if (mapRef.isWalkable(testX, testY)) {
            validMoves.push_back(directions[i]);
        }
    }
    
    if (validMoves.empty()) {
        return Vector2{ (float)x, (float)y }; //stehenbleiben wenn keine Richutng möglich
    }
    
    //zufällige mögliche Richtung auswählen
    int randomIndex = rand() % validMoves.size();
    Vector2 chosenDir = validMoves[randomIndex];
    
    return Vector2{ (float)(x + (int)chosenDir.x), (float)(y + (int)chosenDir.y) };
}

