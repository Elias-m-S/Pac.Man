#pragma once
#include <vector>
#include <string>

class Leaderboard {
public:
    Leaderboard(const std::string& filename);
    ~Leaderboard();

    void addEntry(const std::string& name, int score);
    void draw() const;  // draw full leaderboard screen

private:
    std::string filename;
    std::vector<std::pair<std::string, int>> entries;
};