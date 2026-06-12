#include "TUIUserInterface.h"
#include <gamecontext.h>
#include <actions/action.h>
#include <i18n/LocalizationManager.h>
#include <ftxui/dom/elements.hpp>
#include <ftxui/component/component.hpp>
#include <ftxui/component/screen_interactive.hpp>
#include <string>
#include <sstream>
#include <map>

using namespace ftxui;

namespace
{
    template <typename T>
    std::string to_string_with_precision(const T a_value, const int n = 6)
    {
        std::ostringstream out;
        out.precision(n);
        out << std::fixed << a_value;
        return out.str();
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

void TUIUserInterface::add_log(const std::string &msg)
{
    event_logs.push_back(msg);
    if (event_logs.size() > max_logs)
    {
        event_logs.erase(event_logs.begin());
    }
}

Element TUIUserInterface::RenderCard(const Card *card, bool selected)
{
    if (!card)
        return text("         ") | border | color(current_theme.border) | dim;

    std::string name = transform_face(card->get_face());
    std::string suit_symbol = transform_suit(card->get_suit());

    Color c = current_theme.text;
    std::string type_label = "CARD";

    switch (card->getType())
    {
    case CardType::Monster:
        c = current_theme.monster;
        type_label = "MONST";
        break;
    case CardType::Weapon:
        c = current_theme.weapon;
        type_label = "WEAPON";
        break;
    case CardType::Potion:
        c = current_theme.potion;
        type_label = "POTION";
        break;
    }

    auto content = vbox({hbox({text(name + suit_symbol) | bold, filler()}),
                         filler(),
                         text(type_label) | hcenter | dim,
                         filler(),
                         hbox({filler(), text(name + suit_symbol) | bold})}) |
                   size(WIDTH, EQUAL, 11) | size(HEIGHT, EQUAL, 8);

    if (selected)
    {
        return content | borderDouble | ftxui::color(c) | bgcolor(current_theme.selected_bg);
    }
    return content | border | ftxui::color(c);
}

Element TUIUserInterface::RenderActionButton(const std::string &name, bool selected)
{
    Color c = current_theme.action_label;
    if (name == "Hand Combat" || name == "Attack Monster")
        c = current_theme.monster;
    else if (name == "Drink Potion")
        c = current_theme.potion;
    else if (name == "Attach Weapon")
        c = current_theme.weapon;
    else if (name == "Skip Room" || name == "New Room")
        c = current_theme.health_mid;

    auto btn = text(" " + name + " ") | center;

    if (selected)
    {
        return btn | borderDouble | ftxui::color(c) | bold;
    }
    return btn | border | ftxui::color(c) | dim;
}

Element TUIUserInterface::RenderPlayerStats(const GameContext &ctx)
{
    auto player = ctx.get_player();
    float health_ratio = (float)player->current_life() / player->get_max_health();

    Color health_color = current_theme.health_high;
    if (health_ratio < 0.3)
        health_color = current_theme.health_low;
    else if (health_ratio < 0.6)
        health_color = current_theme.health_mid;

    Elements buffs;
    for (auto &b : player->getBuffs())
        buffs.push_back(text(" + " + b) | color(current_theme.health_high));
    for (auto &d : player->getDeBuffs())
        buffs.push_back(text(" - " + d) | color(current_theme.health_low));
    for (auto &p : player->getPassiveDescriptions())
        buffs.push_back(text(" * " + p) | color(current_theme.weapon));

    auto weapon_info = text("No Weapon") | dim;
    Element last_monster_info = text("");
    if (ctx.get_weapon())
    {
        int effective_atk = player->modify_outgoing_damage(ctx.get_weapon()->weapon_damage());
        std::string power_str = format_damage_for_display(ctx.get_type(), effective_atk);
        weapon_info = text("Equipped: " + ctx.get_weapon()->to_string() + " (Atk: " + power_str + ")") | color(current_theme.weapon);

        if (ctx.get_weapon()->has_monster())
        {
            std::string monster_str = format_damage_for_display(ctx.get_type(), ctx.get_weapon()->last_monster_damage());
            last_monster_info = text("Last Slayed: [Power " + monster_str + "]") | color(current_theme.action_label);
        }
    }

    return vbox({text(" PLAYER STATUS ") | bold | hcenter | color(current_theme.action_label),
                 separator() | color(current_theme.border),
                 hbox({text("Life: ") | color(current_theme.text),
                       gauge(health_ratio) | color(health_color) | flex,
                       text(" " + std::to_string(player->current_life()) + "/" + std::to_string(player->get_max_health())) | color(current_theme.text)}),
                 separator() | color(current_theme.border),
                 weapon_info,
                 last_monster_info,
                 text("Kills attached: " + std::to_string(ctx.get_weapon() ? ctx.get_weapon()->number_of_monsters() : 0)) | dim,
                 separator() | color(current_theme.border),
                 text("Abilities:") | color(current_theme.text),
                 vbox(std::move(buffs)) | flex}) |
           border | color(current_theme.border) | size(WIDTH, EQUAL, 30);
}

Element TUIUserInterface::RenderDungeonStats(const GameContext &ctx)
{
    return vbox({text(" DUNGEON ") | bold | hcenter | color(current_theme.action_label),
                 separator() | color(current_theme.border),
                 text("Rooms left: " + std::to_string(ctx.get_dungeon()->dungeon_size() / 4)) | hcenter | color(current_theme.text),
                 text("Cards left: " + std::to_string(ctx.get_dungeon()->dungeon_size())) | hcenter | color(current_theme.text),
                 text("Cemetery: " + std::to_string(ctx.get_graveyard()->graveyard_size())) | color(current_theme.text) | hcenter | dim,
                 separator() | color(current_theme.border),
                 text(ctx.is_room_skipped() ? "ROOM SKIPPED" : "NEW ROOM") | color(ctx.is_room_skipped() ? current_theme.health_mid : current_theme.health_high) | hcenter}) |
           border | color(current_theme.border) | size(WIDTH, EQUAL, 20);
}

Element TUIUserInterface::RenderLogs()
{
    Elements logs;
    if (event_logs.empty())
    {
        logs.push_back(text("No events yet...") | dim);
    }
    else
    {
        for (auto &l : event_logs)
            logs.push_back(text(l) | color(current_theme.text));
    }
    return vbox(std::move(logs)) | borderRounded | color(current_theme.border) | flex;
}

std::optional<int> TUIUserInterface::select_game_mode()
{
    auto screen = ScreenInteractive::Fullscreen();
    int selected = 0;
    std::vector<std::string> options = {"STANDARD", "EXTENDED", "QUIT"};

    auto menu = Menu(&options, &selected);
    bool confirmed = false;
    bool quit = false;

    auto component = Renderer(menu, [&]
                              { return vbox({text(" SCOUNDREL ") | bold | center | border | color(current_theme.action_label),
                                             text("Select Game Mode:") | hcenter | color(current_theme.text),
                                             menu->Render() | border | hcenter | color(current_theme.weapon),
                                             text(" Use arrows to navigate, Enter to select. ") | dim | hcenter | color(current_theme.text)}) |
                                       center; });

    auto event_handler = CatchEvent(component, [&](Event event)
                                    {
        if (event == Event::Character('q') || event == Event::Escape) {
            quit = true;
            screen.ExitLoopClosure()();
            return true;
        }
        if (event == Event::Return) {
            confirmed = true;
            screen.ExitLoopClosure()();
            return true;
        }
        return false; });

    screen.Loop(event_handler);
    if (quit || (confirmed && selected == 2))
        return std::nullopt;
    return selected + 1;
}

std::optional<int> TUIUserInterface::select_player_class()
{
    auto screen = ScreenInteractive::Fullscreen();
    int selected = 0;
    std::vector<std::string> options = {"Peasant", "Healer", "Warrior", "QUIT"};

    auto &lm = LocalizationManager::instance();
    std::map<int, std::vector<std::string>> class_perks = {
        {0, {lm.get_string("ui.class.peasant_perk")}},
        {1, {lm.get_string("ui.class.healer_perk2"), lm.get_string("ui.class.healer_perk1")}},
        {2, {lm.get_string("ui.class.warrior_perk1"), lm.get_string("ui.class.warrior_perk2")}},
        {3, {lm.get_string("ui.mode.quit")}}};

    auto menu = Menu(&options, &selected);
    bool confirmed = false;
    bool quit = false;

    auto renderer = Renderer(menu, [&]
                             {
        Elements perk_elements;
        for (const auto& perk : class_perks[selected]) {
            perk_elements.push_back(text(" • " + perk) | color(current_theme.text));
        }

        return vbox({
            text(" CHOOSE YOUR CLASS ") | bold | center | border | color(current_theme.action_label) | size(WIDTH, EQUAL, 60),
            menu->Render() | border | hcenter | color(current_theme.health_high) | size(HEIGHT, EQUAL, 6) | size(WIDTH, EQUAL, 60),
            separator() | size(WIDTH, EQUAL, 60),
            vbox({
                text(" Selected Class Perks: ") | bold | color(current_theme.action_label),
                vbox(std::move(perk_elements)) | size(HEIGHT, EQUAL, 3),
            }) | size(WIDTH, EQUAL, 60),
            filler(),
            text(" Use arrows to navigate, Enter to select. ") | dim | hcenter | color(current_theme.text) | size(WIDTH, EQUAL, 60)
        }) | center; });

    auto event_handler = CatchEvent(renderer, [&](Event event)
                                    {
        if (event == Event::Character('q') || event == Event::Escape) {
            quit = true;
            screen.ExitLoopClosure()();
            return true;
        }
        if (event == Event::Return) {
            confirmed = true;
            screen.ExitLoopClosure()();
            return true;
        }
        return false; });

    screen.Loop(event_handler);
    if (quit || (confirmed && selected == 3))
        return std::nullopt;
    return selected;
}

void TUIUserInterface::display_game_state(const GameContext &ctx)
{
    current_ctx = &ctx;
}

void TUIUserInterface::display_valid_actions(std::span<Action *const> actions)
{
    current_actions = actions;
}

std::optional<int> TUIUserInterface::request_action_index(int max_index)
{
    auto screen = ScreenInteractive::Fullscreen();
    int selected = 0;

    std::map<std::string, std::string> descriptions = {
        {"Hand Combat", "Attack a monster using your bare hands (full damage taken)."},
        {"Attack Monster", "Use your equipped weapon to slay a monster."},
        {"Drink Potion", "Consume a potion to restore life (max 20)."},
        {"Attach Weapon", "Equip a new weapon. Replaces current weapon."},
        {"Skip Room", "Return current cards to the dungeon and draw 4 new ones."},
        {"New Room", "The room is cleared. Proceed to the next floor."}};

    std::vector<std::string> options;
    for (auto *a : current_actions)
        options.push_back(a->name());

    auto menu = Menu(&options, &selected, MenuOption::Horizontal());

    auto component = Renderer(menu, [&]
                              {
        Elements buttons;
        for (size_t i = 0; i < current_actions.size(); ++i) {
            buttons.push_back(RenderActionButton(current_actions[i]->name(), selected == (int)i));
        }

        std::string current_name = current_actions[selected]->name();
        std::string desc = descriptions.count(current_name) ? descriptions[current_name] : "";

        Elements cards;
        auto room = current_ctx->get_room();
        for (int i = 0; i < room->cards_in_room(); ++i) {
            cards.push_back(RenderCard(&room->look_card(i)));
        }

        return vbox({
            hbox({
                RenderPlayerStats(*current_ctx),
                vbox({
                    text(" THE ROOM ") | bold | hcenter | color(current_theme.action_label),
                    hbox(std::move(cards)) | hcenter | flex
                }) | flex,
                RenderDungeonStats(*current_ctx)
            }) | flex,
            hbox({
                vbox({
                    text(" ACTIONS ") | bold | ftxui::color(Color::Yellow),
                    hbox(std::move(buttons)),
                    text(desc) | ftxui::color(Color::Grey37),
                    filler(),
                    text("[Arrows]: Navigate | [Enter]: Select | [q]: Quit") | dim
                }) | flex,
                RenderLogs() | size(WIDTH, EQUAL, 40)
            }) | size(HEIGHT, EQUAL, 10)
        }); });

    bool quit = false;
    bool confirmed = false;
    auto event_handler = CatchEvent(component, [&](Event event)
                                    {
        if (event == Event::Character('q')) { quit = true; screen.ExitLoopClosure()(); return true; }
        if (event == Event::Return) { confirmed = true; screen.ExitLoopClosure()(); return true; }
        return false; });

    screen.Loop(event_handler);
    if (quit)
        return std::nullopt;

    add_log("Action: " + current_actions[selected]->name());
    return selected;
}

std::optional<int> TUIUserInterface::request_card_index(int max_cards)
{
    auto screen = ScreenInteractive::Fullscreen();
    int selected = 0;
    bool quit = false;

    auto component = Renderer([&]
                              {
        Elements cards;
        auto room = current_ctx->get_room();
        for (int i = 0; i < room->cards_in_room(); ++i) {
            cards.push_back(RenderCard(&room->look_card(i), selected == i));
        }

        return vbox({
            hbox({
                RenderPlayerStats(*current_ctx),
                vbox({
                    text(" SELECT A CARD ") | bold | hcenter | color(current_theme.action_label),
                    hbox(std::move(cards)) | hcenter | flex
                }) | flex,
                RenderDungeonStats(*current_ctx)
            }) | flex,
            hbox({
                vbox({
                    text(" TARGET SELECTION ") | bold | ftxui::color(Color::Cyan),
                    filler(),
                    text(" Use Arrows to pick a card, Enter to confirm. ") | bold | hcenter | color(current_theme.text),
                    filler(),
                    text("[Arrows]: Change Target | [Enter]: Confirm | [q]: Quit") | color(current_theme.border)
                }) | flex,
                RenderLogs() | size(WIDTH, EQUAL, 40)
            }) | size(HEIGHT, EQUAL, 10)
        }); });

    auto event_handler = CatchEvent(component, [&](Event event)
                                    {
        if (event == Event::Character('q')) { quit = true; screen.ExitLoopClosure()(); return true; }
        if (event == Event::ArrowLeft) { selected = std::max(0, selected - 1); return true; }
        if (event == Event::ArrowRight) { selected = std::min(max_cards - 1, selected + 1); return true; }
        if (event == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false; });

    screen.Loop(event_handler);
    if (quit)
        return std::nullopt;

    add_log("Target: " + current_ctx->get_room()->look_card(selected).to_string());
    return selected + 1;
}

void TUIUserInterface::show_message(const std::string &message) { add_log(message); }
void TUIUserInterface::show_error(const std::string &error) { add_log("ERROR: " + error); }

void TUIUserInterface::show_victory(int score)
{
    auto screen = ScreenInteractive::Fullscreen();
    auto component = Renderer([&]
                              { return vbox({text(" CONGRATULATIONS! ") | bold | center | border | ftxui::color(Color::Green),
                                             text("You survived the dungeon!") | hcenter | color(current_theme.text),
                                             text("Final Score: " + std::to_string(score)) | hcenter | bold | color(current_theme.action_label),
                                             separator() | color(current_theme.border),
                                             text(" Press Enter to continue. ") | dim | hcenter | color(current_theme.text)}) |
                                       center; });

    auto event_handler = CatchEvent(component, [&](Event event)
                                    {
        if (event == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false; });

    screen.Loop(event_handler);
}

void TUIUserInterface::show_defeat(int score, const std::string &reason)
{
    auto screen = ScreenInteractive::Fullscreen();
    auto component = Renderer([&]
                              { return vbox({text(" YOU DIED ") | bold | center | border | ftxui::color(Color::Red),
                                             text(reason) | hcenter | color(current_theme.text),
                                             text("Final Score: " + std::to_string(score)) | hcenter | bold | color(current_theme.action_label),
                                             separator() | color(current_theme.border),
                                             text(" Press Enter to continue. ") | dim | hcenter | color(current_theme.text)}) |
                                       center; });

    auto event_handler = CatchEvent(component, [&](Event event)
                                    {
        if (event == Event::Return) { screen.ExitLoopClosure()(); return true; }
        return false; });

    screen.Loop(event_handler);
}

bool TUIUserInterface::ask_restart()
{
    auto screen = ScreenInteractive::Fullscreen();
    int selected = 0;
    std::vector<std::string> options = {"PLAY AGAIN", "EXIT"};
    auto menu = Menu(&options, &selected);

    auto component = Renderer(menu, [&]
                              { return vbox({text(" GAME OVER ") | bold | center | border | ftxui::color(current_theme.action_label),
                                             text("Do you want to embark on another adventure?") | hcenter | ftxui::color(current_theme.text),
                                             menu->Render() | border | hcenter | ftxui::color(current_theme.health_high),
                                             text(" Use arrows to navigate, Enter to select. ") | dim | hcenter | ftxui::color(current_theme.text)}) |
                                       center; });

    bool confirmed = false;
    auto event_handler = CatchEvent(component, [&](Event event)
                                    {
        if (event == Event::Return) { confirmed = true; screen.ExitLoopClosure()(); return true; }
        return false; });

    screen.Loop(event_handler);
    return (confirmed && selected == 0);
}
