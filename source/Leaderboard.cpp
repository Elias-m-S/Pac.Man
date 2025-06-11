#include "Leaderboard.h"
#include <fstream>
#include <algorithm>
#include <raylib.h>
#include <iostream>

Leaderboard::Leaderboard(const std::string& filename)
    : filename("assets/Scoreboard.txt") // immer fester Pfad
{
    load();
}

Leaderboard::~Leaderboard() {
    save();
}

void Leaderboard::load() {
    std::cout << "Loading from: " << filename << std::endl;
    entries.clear();
    std::ifstream file(filename);
    if (!file.is_open()) return;
    std::string name;
    int score;
    while (file >> name >> score) {
        entries.emplace_back(name, score);
    }
    file.close();

    // Bereits sortieren, falls die Datei unsortiert war
    std::sort(entries.begin(), entries.end(),
              [](auto &a, auto &b){ return a.second > b.second; });
}

void Leaderboard::save() const {
    std::cout << "Saving to: " << filename << std::endl;
    std::ofstream file(filename, std::ios::trunc);
    if (!file.is_open()) return;
    for (auto &e : entries) {
        file << e.first << " " << e.second << "\n";
    }
    file.close();
}

void Leaderboard::addEntry(const std::string& name, int score) {
    entries.emplace_back(name, score);
    std::sort(entries.begin(), entries.end(),
              [](auto &a, auto &b){ return a.second > b.second; });
}

void Leaderboard::draw(int x, int yStart) const {
    ClearBackground(BLACK);
    DrawText("Leaderboard", x, yStart - 40, 40, WHITE);
    int y = yStart;
    
    // Nur Top 10 anzeigen
    size_t maxEntries = std::min(entries.size(), (size_t)10);
    
    for (size_t i = 0; i < maxEntries; ++i) {
        const auto& e = entries[i];
        
        // Farbe basierend auf Platzierung
        Color color;
        if (i == 0) {
            color = GOLD;           // 1. Platz: Gold
        } else if (i == 1) {
            color = Color{192, 192, 192, 255};  // 2. Platz: Silber
        } else if (i == 2) {
            color = Color{205, 127, 50, 255};   // 3. Platz: Bronze/Kupfer
        } else {
            color = GRAY;           // Rest: Grau
        }
        
        DrawText(
            TextFormat("%zu. %s %d", i + 1, e.first.c_str(), e.second),
            x, y, 20, color
        );
        y += 30;
    }
}