# Scoundrel TUI - Project Context

Scoundrel TUI is a terminal-based frontend for the **Scoundrel** card game. It provides both a rich Terminal User Interface (TUI) built with [FTXUI](https://github.com/ArthurSonzogni/ftxui) and a simple Command Line Interface (CLI) fallback.

## 🏛 Architecture Overview

The project is structured to separate game logic from presentation:

-   **`scoundrel-core` (Sibling Dependency):** Contains the core game logic, card definitions, and abstract interfaces. This project depends on `scoundrel-core` being present in a sibling directory.
-   **`src/main.cpp`:** The application entry point. Handles command-line arguments, internationalization (i18n), and orchestrates the game loop.
-   **`src/ui/`:** Contains the UI implementations.
    -   `cli/`: `CLIUserInterface` - A basic text-based interface.
    -   `tui/`: `TUIUserInterface` - An interactive, color-coded terminal interface using FTXUI.
-   **`src/ui/tui/Theme.h` & `ThemeLoader.cpp`:** A customizable theme system for the TUI.
-   **`assets/themes/`:** `.ini` files defining color schemes (e.g., Dracula, Catppuccin Mocha, Gruvbox).

## 🛠 Building and Running

### Prerequisites

-   C++17 compatible compiler.
-   CMake 3.16+.
-   `scoundrel-core` must exist at `../scoundrel-core`.

### Build Commands

```bash
# Configure the project
cmake -B build -S .

# Build the executable
cmake --build build --config Release
```

### Running

The binary is located in `build/Release/Scoundrel` (Windows) or `build/Scoundrel` (Linux).

```bash
# Start in CLI mode (default)
./build/Scoundrel

# Start in TUI mode
./build/Scoundrel --tui

# Start with a specific theme
./build/Scoundrel --tui --theme assets/themes/dracula.ini
```

## 🧪 Development Conventions

-   **Separation of Concerns:** Keep UI logic in `src/ui` and game logic in `scoundrel-core`.
-   **Interface-Based Design:** UI classes must implement the `UserInterface` abstract class defined in `scoundrel-core`.
-   **Modern C++:** Use smart pointers (`std::unique_ptr`, `std::shared_ptr`) and modern STL containers.
-   **Internationalization:** Use `LocalizationManager` for all user-facing strings.
-   **Theming:** When adding new UI elements, ensure they respect the `Theme` structure to maintain visual consistency across themes.

## 📁 Key Files

-   `CMakeLists.txt`: Project configuration and dependency management (FetchContent for FTXUI).
-   `BUILD.md`: Detailed build instructions and dependency notes.
-   `src/ui/tui/TUIUserInterface.cpp`: The primary rendering and interaction logic for the TUI.
-   `assets/themes/*.ini`: Theme configuration files.
