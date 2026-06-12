#pragma once
#include <ftxui/screen/color.hpp>
#include <string>
#include <map>

struct Theme {
    // Card Colors
    ftxui::Color monster = ftxui::Color::RGB(243, 139, 168); // #f38ba8
    ftxui::Color weapon = ftxui::Color::RGB(137, 220, 235);  // #89dceb
    ftxui::Color potion = ftxui::Color::RGB(245, 194, 231);  // #f5c2e7
    
    // UI Elements
    ftxui::Color health_high = ftxui::Color::RGB(166, 227, 161); // #a6e3a1
    ftxui::Color health_mid = ftxui::Color::RGB(249, 226, 175);  // #f9e2af
    ftxui::Color health_low = ftxui::Color::RGB(243, 139, 168);  // #f38ba8
    
    ftxui::Color text = ftxui::Color::RGB(205, 214, 244);        // #cdd6f4
    ftxui::Color border = ftxui::Color::RGB(49, 50, 68);         // #313244
    ftxui::Color selected_bg = ftxui::Color::RGB(69, 71, 90);    // #45475a
    ftxui::Color action_label = ftxui::Color::RGB(250, 179, 135); // #fab387 (Peach)

    static Theme LoadFromFile(const std::string& path);
};
