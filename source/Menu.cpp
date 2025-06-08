#include "Menu.h"
#include <raylib.h>

Menu::Menu(const std::vector<std::string>& items)
  : items(items), selectedIndex(0), selectedFlag(false)
{}

void Menu::update() {
    selectedFlag = false;
    if (IsKeyPressed(KEY_DOWN)) {
        selectedIndex = (selectedIndex + 1) % items.size();
    }
    if (IsKeyPressed(KEY_UP)) {
        selectedIndex = (selectedIndex - 1 + items.size()) % items.size();
    }
    if (IsKeyPressed(KEY_ENTER)) {
        selectedFlag = true;
    }
}

void Menu::draw(int screenWidth, int screenHeight) const {
    const int fontSize = 30;
    int y = screenHeight / 3;
    for (int i = 0; i < items.size(); ++i) {
        Color col = (i == selectedIndex ? YELLOW : WHITE);
        int textWidth = MeasureText(items[i].c_str(), fontSize);
        DrawText(items[i].c_str(),
                 (screenWidth - textWidth)/2,
                 y + i * (fontSize + 10),
                 fontSize,
                 col);
    }
}

bool Menu::isSelected() const { return selectedFlag; }
int  Menu::getSelectedIndex() const { return selectedIndex; }
void Menu::reset()     { selectedFlag = false; }