#pragma once
#include <UserInterface.h>
#include <ftxui/component/screen_interactive.hpp>
#include <ftxui/dom/elements.hpp>
#include "Theme.h"
#include <string>
#include <vector>
#include <optional>

class Card;

class TUIUserInterface : public UserInterface
{
public:
    TUIUserInterface()
    {
        add_log("Welcome to Scoundrel!");
    }

    void set_theme(const Theme &theme)
    {
        current_theme = theme;
    }

private:
    Theme current_theme;
    std::vector<std::string> event_logs;
    const size_t max_logs = 5;

    // Rendering Helpers
    ftxui::Element RenderCard(const Card *card, bool selected = false);
    ftxui::Element RenderPlayerStats(const GameContext &ctx);
    ftxui::Element RenderDungeonStats(const GameContext &ctx);
    ftxui::Element RenderLogs();
    ftxui::Element RenderActionButton(const std::string &name, bool selected);

    void add_log(const std::string &msg);

    // Interaction State
    const GameContext *current_ctx = nullptr;
    std::span<Action *const> current_actions;

public:
    std::optional<int> select_game_mode() override;
    std::optional<int> select_player_class() override;

    void display_game_state(const GameContext &ctx) override;
    void display_valid_actions(std::span<Action *const> actions) override;

    std::optional<int> request_action_index(int max_index) override;
    std::optional<int> request_card_index(int max_cards) override;

    void show_message(const std::string &message) override;
    void show_error(const std::string &error) override;

    void show_victory(int score) override;
    void show_defeat(int score, const std::string &reason) override;
    bool ask_restart() override;
};
