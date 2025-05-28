#include "Leaderboard.h"
#include <fstream>
#include <raylib.h>

Leaderboard::Leaderboard(const std::string& filename)
    : filename(filename) {
    std::ifstream file(filename);
    std::string name;
    int score;
    while (file >> name >> score) {
        entries.emplace_back(name, score);
    }
    file.close();
}

Leaderboard::~Leaderboard() {
    std::ofstream file(filename);
    for (auto& e : entries) {
        file << e.first << " " << e.second << "\n";
    }
}

void Leaderboard::addEntry(const std::string& name, int score) {
    entries.emplace_back(name, score);
}

void Leaderboard::draw() const {
    ClearBackground(BLACK);
    DrawText("Leaderboard", 100, 50, 40, WHITE);
    int y = 120;
    for (auto& e : entries) {
        DrawText(TextFormat("%s: %i", e.first.c_str(), e.second), 100, y, 20, GOLD);
        y += 30;
    }
}