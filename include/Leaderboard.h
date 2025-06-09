#pragma once
#include <string>
#include <vector>
#include <utility>

class Leaderboard {
public:
    // Standard auf assets/Leaderboard.txt
    explicit Leaderboard(const std::string& filename = "assets/Scoreboard.txt");
    ~Leaderboard();

    void addEntry(const std::string& name, int score);
    void draw(int x = 100, int yStart = 120) const;
    void save() const;

private:
    std::string filename;
    std::vector<std::pair<std::string,int>> entries;

    void load();
};