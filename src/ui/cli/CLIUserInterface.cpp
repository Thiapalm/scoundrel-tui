#include "CLIUserInterface.h"
#include <gamecontext.h>
#include <actions/action.h>
#include <iostream>
#include <string>
#include <sstream>

namespace
{
    std::optional<int> read_input(const std::string &prompt)
    {
        std::string input;
        std::cout << prompt;
        std::cin >> input;
        if (input == "q" || input == "Q")
            return std::nullopt;
        try
        {
            return std::stoi(input);
        }
        catch (...)
        {
            return -1; // Invalid numeric input
        }
    }

    std::string damage_to_face_string(int damage)
    {
        int face_int = damage - 2;
        if (face_int < 0 || face_int > 12)
            return std::to_string(damage);
        face f = static_cast<face>(face_int);
        return transform_face(f);
    }

    std::string format_damage_for_display(GameType type, int damage)
    {
        return type == GameType::DEFAULT ? damage_to_face_string(damage) : std::to_string(damage);
    }
}

std::optional<int> CLIUserInterface::select_game_mode()
{
    std::cout << "Choose Game Mode (press 'q' to quit):\n";
    std::cout << "(1) STANDARD\n";
    std::cout << "(2) EXTENDED\n";
    return read_input("Choose: ");
}

std::optional<int> CLIUserInterface::select_player_class()
{
    std::cout << "Choose Player Class (press 'q' to quit):\n";
    std::cout << "(1) Peasant\n";
    std::cout << "(2) Healer\n";
    std::cout << "(3) Warrior\n";
    auto value = read_input("Choose: ");
    if (!value)
        return std::nullopt;
    if (*value < 1 || *value > 3)
        return std::nullopt;
    return *value - 1;
}

void CLIUserInterface::display_game_state(const GameContext &ctx)
{
    std::cout << "================================================================================" << std::endl;
    std::cout << " Dungeon Rooms: " << (ctx.get_dungeon()->dungeon_size() / 4) << "(" << ctx.get_dungeon()->dungeon_size() << ")\n";
    std::cout << " Cemetery: " << ctx.get_graveyard()->graveyard_size() << "\n";

    // Extended info if available
    if (!ctx.get_player()->getName().empty() && ctx.get_player()->getName() != " ")
    {
        std::cout << "Class: " << ctx.get_player()->getName() << "\n";
        std::cout << "   Buffs:   [";
        for (const auto &buff : ctx.get_player()->getBuffs())
        {
            std::cout << buff << ", ";
        }
        std::cout << "]\n";
        std::cout << "   DeBuffs: [";
        for (const auto &debuff : ctx.get_player()->getDeBuffs())
        {
            std::cout << debuff << ", ";
        }
        std::cout << "]\n";
        std::cout << "   Passives: [";
        for (const auto &passive : ctx.get_player()->getPassiveDescriptions())
        {
            std::cout << passive << ", ";
        }
        std::cout << "]\n";
    }

    std::cout << "\n\nLife: " << ctx.get_player()->current_life() << "  { ";
    for (int i = 0; i < ctx.get_room()->cards_in_room(); ++i)
    {
        std::cout << "(" << (i + 1) << ")->" << ctx.get_room()->look_card(i).to_string() << " ";
    }

    int damage = ctx.get_weapon() == nullptr ? ctx.get_player()->modify_outgoing_damage(0) : ctx.get_player()->modify_outgoing_damage(ctx.get_weapon()->weapon_damage());

    int last_monster = (ctx.get_weapon() != nullptr && ctx.get_weapon()->has_monster()) ? ctx.get_weapon()->last_monster_damage() : 0;

    std::string damage_str = format_damage_for_display(ctx.get_type(), damage);
    std::string monster_str = (ctx.get_weapon() != nullptr && ctx.get_weapon()->has_monster()) ? format_damage_for_display(ctx.get_type(), last_monster) : "None";

    std::cout << "}  Weapon Damage: " << damage_str << " Last Monster: " << monster_str << "\n";
}

void CLIUserInterface::display_valid_actions(std::span<Action *const> actions)
{
    std::cout << "\nValid Actions (press 'q' at any prompt to quit): " << std::endl;
    for (size_t i = 0; i < actions.size(); ++i)
    {
        std::cout << "( " << i << " ) - " << actions[i]->name() << "\n";
    }
}

std::optional<int> CLIUserInterface::request_action_index(int max_index)
{
    while (true)
    {
        auto val = read_input("Choose action -> ");
        if (!val)
            return std::nullopt;
        if (*val < 0 || *val >= max_index)
            std::cout << "Invalid Action, try again" << std::endl;
        else
            return val;
    }
}

std::optional<int> CLIUserInterface::request_card_index(int max_cards)
{
    return read_input("Choose card -> ");
}

void CLIUserInterface::show_message(const std::string &message)
{
    std::cout << message << std::endl;
}

void CLIUserInterface::show_error(const std::string &error)
{
    std::cerr << "Error: " << error << std::endl;
}

void CLIUserInterface::show_victory(int score)
{
    std::cout << "Congratulations you survived the Dungeon !!" << std::endl;
    std::cout << "Score: " << score << std::endl;
}

void CLIUserInterface::show_defeat(int score, const std::string &reason)
{
    std::cout << "You Died !! " << reason << std::endl;
    std::cout << "Score: " << score << std::endl;
}

bool CLIUserInterface::ask_restart()
{
    std::string choice;
    std::cout << "Do you want to play again? (y/n): ";
    std::cin >> choice;
    return (choice == "y" || choice == "Y");
}
