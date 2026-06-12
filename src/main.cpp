#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include "GameContextFactory.h"
#include "DefaultGameContextFactory.h"
#include "GameLogic.h"
#include "i18n/LocalizationManager.h"
#include "ui/cli/CLIUserInterface.h"
#include "ui/tui/TUIUserInterface.h"
#include "ui/tui/Theme.h"

int main(int argc, char** argv)
{
    std::vector<std::string> args(argv + 1, argv + argc);
    bool use_tui = false;
    std::string theme_path = "";
    
    auto& lm = LocalizationManager::instance();
    if (!lm.load_locale("en", "locales")) {
        if (!lm.load_locale("en", "../locales")) {
            if (!lm.load_locale("en", "scoundrel-core/assets/locales")) {
                lm.load_locale("en", "../scoundrel-core/assets/locales");
            }
        }
    }
    lm.set_locale("en");

    for (size_t i = 0; i < args.size(); ++i) {
        if (args[i] == "--tui") {
            use_tui = true;
        } else if (args[i] == "--theme" && i + 1 < args.size()) {
            theme_path = args[i+1];
        }
    }

    std::unique_ptr<UserInterface> ui;
    if (use_tui) {
        auto tui = std::make_unique<TUIUserInterface>();
        if (!theme_path.empty()) {
            tui->set_theme(Theme::LoadFromFile(theme_path));
        }
        ui = std::move(tui);
    } else {
        ui = std::make_unique<CLIUserInterface>();
    }

    // Initialize core managers with defaults for desktop
    GameLogic::set_auth_manager(std::make_shared<NullAuthManager>());
    
    bool play_again = true;
    while (play_again) {
        ui->reset();
        auto mode_opt = ui->select_game_mode();
        if (!mode_opt) break;
        int mode = *mode_opt;

        auto game = GameLogic::create(mode == 1 ? GameType::DEFAULT : GameType::EXTENDED);
        game->set_interface(ui.get());
        game->run_game_loop();

        play_again = ui->ask_restart();
    }

    return 0;
}
