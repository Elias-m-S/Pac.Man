#pragma once
#include <vector>
#include <string>

class Menu {
public:
    Menu(const std::vector<std::string>& items);
    void update();                        // Input verarbeiten
    void draw(int screenWidth, int screenHeight) const; // Menü zeichnen
    bool isSelected() const;              // true, wenn ENTER gedrückt wurde
    int getSelectedIndex() const;         // index des gewählten Eintrags
    void reset();                         // Auswahl zurücksetzen

private:
    std::vector<std::string> items;
    int selectedIndex;
    bool selectedFlag;
};