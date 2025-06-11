# Wissenschaftliche Dokumentation: Ghost-System und Kollisionserkennung

## Einleitung

Diese Dokumentation beschreibt die Entwicklung und Implementierung des Ghost-Systems sowie der Kollisionserkennung im Pac-Man Spiel. Der Fokus liegt auf der objektorientierten Architektur der Geister-Entitäten, deren Verhaltensmuster und der Implementierung eines robusten Kollisionssystems zwischen Pac-Man und den Geistern.

## Architektur und Design-Entscheidungen

### 2.1 Übergeordnete Architektur

Die Ghost-Implementierung folgt einer hierarchischen objektorientierten Struktur, die auf dem **Template Method Pattern** und **Strategy Pattern** basiert. Die Architektur wurde gewählt, um maximale Wiederverwendbarkeit von Code zu gewährleisten und gleichzeitig spezifische Verhaltensweisen für jeden Geist zu ermöglichen.

```cpp
Entity (Basisklasse)
└── Ghostbase (Abstrakte Basisklasse)
    ├── RedGhost (Konkrete Implementierung)
    ├── PinkGhost (Konkrete Implementierung)
    ├── GreenGhost (Konkrete Implementierung)
    └── BlueGhost (Konkrete Implementierung)
```

### 2.2 Begründung der Architekturentscheidungen

**Abstrakte Basisklasse (Ghostbase):** Die Entscheidung für eine abstrakte Basisklasse ermöglicht die Definition gemeinsamer Eigenschaften und Verhaltensweisen aller Geister, während spezifische Algorithmen durch reine virtuelle Funktionen in den abgeleiteten Klassen implementiert werden.

**State Machine Pattern:** Jeder Geist implementiert eine Zustandsmaschine mit fünf definierten Zuständen:
- `IN_BASE`: Initialzustand in der Geisterbasis
- `SCATTER`: Bewegung zu spezifischen Eckpositionen
- `CHASE`: Verfolgung von Pac-Man
- `FRIGHTENED`: Verwundbarer Zustand nach Power-Up Konsum
- `EATEN`: Rückkehr zur Basis nach dem Gefressen werden

## Objektorientierte Konzepte und deren Nutzen

### 3.1 Vererbung (Inheritance)

Die Vererbungshierarchie wurde implementiert, um Code-Duplikation zu vermeiden und eine konsistente Schnittstelle zu gewährleisten:

```cpp
class Ghostbase : public Entity {
    // Gemeinsame Geister-Funktionalität
public:
    virtual Vector2 getTargetTile(const Vector2& pacmanPos) const = 0;
    virtual Vector2 getScatterTarget() const = 0;
protected:
    GhostState state;
    float stateTimer;
    Color normalColor;
    // ...weitere gemeinsame Attribute
};
```

**Nutzen:** Diese Struktur ermöglicht es, gemeinsame Funktionalitäten wie Bewegung, State Management und Rendering in der Basisklasse zu implementieren, während spezifische Verhaltensweisen in den abgeleiteten Klassen definiert werden.

### 3.2 Polymorphismus

Pure virtuelle Funktionen ermöglichen unterschiedliche Implementierungen der Target-Algorithmen:

```cpp
// RedGhost: Direkte Verfolgung
Vector2 RedGhost::getTargetTile(const Vector2& pacmanPos) const {
    return pacmanPos;
}

// PinkGhost: Ambush-Verhalten (4 Tiles voraus)
Vector2 PinkGhost::getTargetTile(const Vector2& pacmanPos) const {
    // Komplexerer Algorithmus für Ambush-Verhalten
}
```

**Nutzen:** Polymorphismus ermöglicht es, zur Laufzeit die korrekte Implementierung aufzurufen, ohne dass die aufrufende Code-Stelle wissen muss, welcher spezifische Geist-Typ verwendet wird.

### 3.3 Kapselung (Encapsulation)

Die Kapselung wurde durch die Definition von `public`, `protected` und `private` Bereichen realisiert:

```cpp
public:    // Schnittstelle für externe Interaktion
    void setFrightened(bool on);
    bool canBeEaten() const;
    
protected: // Für abgeleitete Klassen zugänglich
    GhostState state;
    void changeState(GhostState newState);
    
private:   // Interne Implementierungsdetails
    std::mt19937 rng;
    Vector2 randomTile() const;
```

**Nutzen:** Kapselung schützt die interne Zustandsrepräsentation vor ungewollten Änderungen und definiert klare Schnittstellen für die Interaktion mit anderen Systemkomponenten.

## State Machine Implementation

### 4.1 Zustandsmanagement

Das State Machine Pattern wurde implementiert, um komplexe Geisterverhalten strukturiert zu verwalten:

```cpp
enum class GhostState {
    IN_BASE,     // Geister verlassen die Basis
    SCATTER,     // Bewegung zu Eckpositionen
    CHASE,       // Verfolgung von Pac-Man
    FRIGHTENED   // Verwundbarer Zustand
};
```

Die Zustandsübergänge werden durch Timer gesteuert:
- IN_BASE → SCATTER (2 Sekunden)
- SCATTER ↔ CHASE (7/20 Sekunden Zyklen)
- FRIGHTENED → SCATTER (10 Sekunden oder bei Kollision mit sofortigem Teleport)

### 4.2 Algorithmusvielfalt durch Strategy Pattern

Jeder Geist implementiert individuelle Target-Algorithmen:

**RedGhost (Blinky):** Direkter Verfolger
```cpp
Vector2 getTargetTile(const Vector2& pacmanPos) const {
    return pacmanPos; // Direkte Verfolgung
}
```

**Spezifische Eckziele im Scatter-Modus:**
- RedGhost: Obere rechte Ecke (18,2)
- PinkGhost: Obere linke Ecke (2,2)
- GreenGhost: Untere linke Ecke (2,18)
- BlueGhost: Untere rechte Ecke (18,18)

## Kollisionssystem

### 5.1 Kollisionserkennung

Die Kollisionserkennung basiert auf tile-basierter Positionsprüfung:

```cpp
void Game::ghostCollision() {
    int px = pacman.getX();
    int py = pacman.getY();
    
    Ghostbase* ghosts[] = {redGhost, pinkGhost, greenGhost, blueGhost};
    
    for (int i = 0; i < 4; i++) {
        if (ghosts[i]->getX() == px && ghosts[i]->getY() == py) {
            if (ghosts[i]->canBeEaten()) {
                // Geist kann gefressen werden
                ghosts[i]->getEaten();
                // Progressives Punktesystem: 200, 400, 800, 1600
                int points = 200 * (1 << ghostEatenCount);
                pacman.addScore(points);
            } else if (!ghosts[i]->isFrightened()) {
                // Game Over bei normaler Kollision
                state = GameState::GAMEOVER;
            }
        }
    }
}
```

### 5.2 Power-Up Frightened Mode

Das System implementiert einen ereignisgesteuerten Frightened Mode:

1. **Aktivierung:** Power-Up Konsum durch Pac-Man
2. **Zustandsänderung:** Alle Geister wechseln zu FRIGHTENED
3. **Verhaltensänderung:** Zufällige, verlangsamte Bewegung
4. **Visuelle Änderung:** Nur Augen werden dargestellt
5. **Kollisionslogik:** Geister können gefressen werden

```cpp
if (pacman.justAtePowerUp()) {
    redGhost->setFrightened(true);
    pinkGhost->setFrightened(true);
    greenGhost->setFrightened(true);
    blueGhost->setFrightened(true);
    ghostEatenCount = 0; // Reset für progressive Punktevergabe
}
```

## Performance-Optimierungen

### 6.1 Diskrete Bewegung

Die Implementierung verwendet ein timer-basiertes System für diskrete Tile-Bewegung:

```cpp
if (moveTimer <= 0.0f) {
    moveTimer = moveInterval; // Timer zurücksetzen
    // Bewegungslogik nur bei Timer-Ablauf ausführen
}
```

**Nutzen:** Reduziert Rechenaufwand und gewährleistet gleichmäßige Bewegungsgeschwindigkeiten unabhängig von der Framerate.

### 6.2 Efficient Pathfinding

Die Richtungsberechnung verwendet eine optimierte Distanzberechnung:

```cpp
Vector2 Ghostbase::chooseDirectionTowards(const Vector2& target) const {
    Vector2 directions[4] = {{1,0}, {-1,0}, {0,1}, {0,-1}};
    Vector2 bestDirection = {0, 0};
    float minDistance = FLT_MAX;
    
    for (const auto& dir : directions) {
        // Berechne Distanz und wähle optimale Richtung
        float distance = sqrt(pow(testX - target.x, 2) + pow(testY - target.y, 2));
        if (distance < minDistance && mapRef.isWalkable(testX, testY)) {
            minDistance = distance;
            bestDirection = dir;
        }
    }
    return bestDirection;
}
```

## Fazit

Die implementierte Ghost-Architektur demonstriert erfolgreiche Anwendung objektorientierten Designs. Die Kombination aus Vererbung, Polymorphismus und Kapselung ermöglicht eine saubere Trennung von gemeinsamer Funktionalität und spezifischen Verhaltensweisen. Das State Machine Pattern gewährleistet robuste Zustandsverwaltung, während das Strategy Pattern flexible Algorithmusimplementierung ermöglicht.

Die tile-basierte Kollisionserkennung in Verbindung mit dem ereignisgesteuerten Frightened Mode schafft ein reaktionsschnelles und erweiterbares Spielsystem. Die Architektur unterstützt einfache Erweiterungen um zusätzliche Geistertypen oder Verhaltensmuster ohne Modifikation bestehenden Codes.

Diese Lösung zeigt, wie objektorientierte Prinzipien zur Entwicklung wartbarer, erweiterbarer und effizienter Spielsysteme eingesetzt werden können.
