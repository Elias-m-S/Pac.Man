# Code-Analyse: Objektorientierte Konzepte im Ghost-System

## Übersicht der implementierten OOP-Konzepte

| **Konzept** | **Implementierung** | **Datei/Klasse** | **Nutzen** |
|-------------|-------------------|------------------|------------|
| **Vererbung** | `Entity` → `Ghostbase` → `RedGhost/PinkGhost/etc.` | `Entity.h`, `Ghostbase.h`, `RedGhost.h` | Code-Wiederverwendung, gemeinsame Schnittstelle |
| **Abstrakte Klassen** | `Ghostbase` mit pure virtual functions | `Ghostbase.h` | Erzwingt Implementierung spezifischer Methoden |
| **Polymorphismus** | `getTargetTile()`, `getScatterTarget()` virtuell | Alle Ghost-Klassen | Unterschiedliche Algorithmen zur Laufzeit |
| **Kapselung** | `private`, `protected`, `public` Bereiche | `Ghostbase.h` | Datenintegrität, klare Schnittstellen |
| **Template Method** | `update()` in Basisklasse, Steps überschreibbar | `Ghostbase.cpp` | Algorithmus-Skelett mit anpassbaren Teilen |
| **State Machine** | `GhostState` enum mit Zustandslogik | `Ghostbase.h/.cpp` | Strukturierte Verhaltensverwaltung |
| **Strategy Pattern** | Verschiedene Targeting-Algorithmen | Ghost-Implementierungen | Austauschbare Strategien |

## Klassen-Verantwortlichkeiten

### **Ghostbase (Abstrakte Basisklasse)**
```cpp
// Template Method Pattern: Definiert Algorithmus-Struktur
void update(float deltaTime, const Vector2& pacmanPos, const Map& map) {
    // 1. Timer-Management (gemeinsam)
    stateTimer -= deltaTime;
    
    // 2. State-Transitions (gemeinsam)
    if (stateTimer <= 0.0f) { /* ... */ }
    
    // 3. Target-Berechnung (spezialisiert)
    Vector2 target = getTargetTile(pacmanPos); // PURE VIRTUAL
    
    // 4. Bewegung (gemeinsam)
    Vector2 dir = chooseDirectionTowards(target);
}
```

### **RedGhost (Konkrete Implementierung)**
```cpp
// Strategy Pattern: Spezifische Targeting-Strategie
Vector2 getTargetTile(const Vector2& pacmanPos) const override {
    return pacmanPos; // Direkte Verfolgung
}

Vector2 getScatterTarget() const override {
    return Vector2{18, 2}; // Obere rechte Ecke
}
```

## State Machine Implementierung

### **Zustandsübergänge**
```cpp
// Timer-basierte Zustandsübergänge
if (state == GhostState::IN_BASE) {
    changeState(GhostState::SCATTER);
    stateTimer = 7.0f;
} else if (state == GhostState::SCATTER) {
    changeState(GhostState::CHASE);
    stateTimer = 20.0f;
}

// Event-basierte Übergänge
void setFrightened(bool on) {
    if (on) {
        previousState = state;
        changeState(GhostState::FRIGHTENED);
        stateTimer = 10.0f;
        moveInterval = 2.0f / speed; // Verlangsamung
    }
}
```

## Kollisionssystem

### **Tile-basierte Kollisionserkennung**
```cpp
void Game::ghostCollision() {
    // Effizienter Array-Zugriff statt einzelne Abfragen
    Ghostbase* ghosts[] = {redGhost, pinkGhost, greenGhost, blueGhost};
    
    for (int i = 0; i < 4; i++) {
        if (ghosts[i]->getX() == px && ghosts[i]->getY() == py) {
            // Zustandsabhängige Kollisionsbehandlung
            if (ghosts[i]->canBeEaten()) {
                handleGhostEaten(ghosts[i]);
            } else {
                handleGameOver();
            }
        }
    }
}
```

## Design Pattern Vorteile

### **1. Template Method Pattern**
- **Vorteil:** Gemeinsame Algorithmus-Struktur in `update()`
- **Flexibilität:** Spezifische Steps (`getTargetTile`) überschreibbar
- **Wartbarkeit:** Änderungen an Timer-Logic nur in Basisklasse

### **2. Strategy Pattern**
- **Vorteil:** Austauschbare Targeting-Algorithmen
- **Erweiterbarkeit:** Neue Ghost-Typen durch neue Strategien
- **Testbarkeit:** Isolierte Testung einzelner Algorithmen

### **3. State Machine Pattern**
- **Vorteil:** Klare Trennung der Verhaltenszustände
- **Robustheit:** Definierte Übergangsbedingungen
- **Debuggbarkeit:** Zustandsverfolgung vereinfacht

## Performance-Aspekte

### **Optimierungen**
```cpp
// 1. Diskrete Bewegung reduziert Berechnungen
if (moveTimer <= 0.0f) {
    moveTimer = moveInterval;
    // Bewegungslogik nur bei Timer-Ablauf
}

// 2. Referenzen statt Kopien für Map-Zugriff
const Map& mapRef; // Keine Kopie der Map-Daten

// 3. Inline-Funktionen für häufige Zugriffe
inline int getX() const { return x; }
inline int getY() const { return y; }
```

## Erweiterbarkeit

Das System unterstützt einfache Erweiterungen:

1. **Neue Ghost-Typen:** Ableitung von `Ghostbase` mit neuen Strategien
2. **Neue Zustände:** Erweiterung des `GhostState` Enums
3. **Neue Verhaltensweisen:** Überschreibung virtueller Methoden
4. **Neue Kollisionstypen:** Erweiterung der `ghostCollision()` Logik

Das objektorientierte Design ermöglicht Modifikationen ohne Breaking Changes am bestehenden Code.
