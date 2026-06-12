#include "Theme.h"
#include <fstream>
#include <sstream>
#include <iostream>

namespace {
    ftxui::Color HexToColor(const std::string& hex) {
        if (hex.empty() || hex[0] != '#') return ftxui::Color::White;
        
        try {
            std::string r_str = hex.substr(1, 2);
            std::string g_str = hex.substr(3, 2);
            std::string b_str = hex.substr(5, 2);
            
            int r = std::stoi(r_str, nullptr, 16);
            int g = std::stoi(g_str, nullptr, 16);
            int b = std::stoi(b_str, nullptr, 16);
            
            return ftxui::Color::RGB(r, g, b);
        } catch (...) {
            return ftxui::Color::White;
        }
    }

    std::string trim(const std::string& s) {
        size_t first = s.find_first_not_of(" \t\r\n");
        if (std::string::npos == first) return s;
        size_t last = s.find_last_not_of(" \t\r\n");
        return s.substr(first, (last - first + 1));
    }
}

Theme Theme::LoadFromFile(const std::string& path) {
    Theme theme; // Default Catppuccin
    std::ifstream file(path);
    if (!file.is_open()) return theme;

    std::string line;
    while (std::getline(file, line)) {
        line = trim(line);
        if (line.empty() || line[0] == '#' || line[0] == ';') continue;

        size_t sep = line.find('=');
        if (sep == std::string::npos) continue;

        std::string key = trim(line.substr(0, sep));
        std::string val = trim(line.substr(sep + 1));

        if (key == "COLOR_MONSTER") theme.monster = HexToColor(val);
        else if (key == "COLOR_WEAPON") theme.weapon = HexToColor(val);
        else if (key == "COLOR_POTION") theme.potion = HexToColor(val);
        else if (key == "COLOR_HEALTH_HIGH") theme.health_high = HexToColor(val);
        else if (key == "COLOR_HEALTH_MID") theme.health_mid = HexToColor(val);
        else if (key == "COLOR_HEALTH_LOW") theme.health_low = HexToColor(val);
        else if (key == "COLOR_TEXT") theme.text = HexToColor(val);
        else if (key == "COLOR_BORDER") theme.border = HexToColor(val);
        else if (key == "COLOR_SELECTED_BG") theme.selected_bg = HexToColor(val);
        else if (key == "COLOR_ACTION_LABEL") theme.action_label = HexToColor(val);
    }

    return theme;
}
