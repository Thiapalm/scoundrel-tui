#pragma once
#include <UserInterface.h>
#include <iostream>

class CLIUserInterface : public UserInterface
{
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
