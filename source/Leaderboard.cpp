#include "Leaderboard.h"
#include <fstream>
#include <algorithm>
#include <raylib.h>

Leaderboard::Leaderboard(const std::string& filename)
    : filename(filename)
{
    load();
}

Leaderboard::~Leaderboard() {
    save();
}

void Leaderboard::load() {
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
    for (size_t i = 0; i < entries.size(); ++i) {
        const auto& e = entries[i];
        // 1. Name 1234
        DrawText(
            TextFormat("%zu. %s %d", i + 1, e.first.c_str(), e.second),
            x, y, 20, GOLD
        );
        y += 30;
    }
}