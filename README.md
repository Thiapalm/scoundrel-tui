# Scoundrel TUI

A modern, terminal-based frontend for the **Scoundrel** card game.

Scoundrel is a dungeon-crawling card game where you explore a deck of 52 cards, facing monsters, using weapons, and consuming potions to survive the depths. This repository provides a rich Terminal User Interface (TUI) and a classic Command Line Interface (CLI) for the game.

## ✨ Features

-   **Interactive TUI:** A full-featured terminal interface built with [FTXUI](https://github.com/ArthurSonzogni/ftxui).
-   **Theming Support:** Multiple color schemes included (Dracula, Gruvbox, One Dark, etc.).
-   **CLI Fallback:** A simple, lightweight text interface for quick sessions.
-   **Internationalization:** Multi-language support (English included by default).
-   **Cross-Platform:** Built with CMake for compatibility across Windows and Linux.

## 🚀 Quick Start

### Prerequisites

-   C++17 compatible compiler
-   CMake 3.16+
-   **Scoundrel Core:** This project requires the [scoundrel-core](https://github.com/your-repo/scoundrel-core) library in a sibling directory (`../scoundrel-core`).

### Installation

1.  Clone the repository and its core dependency:
    ```bash
    git clone https://github.com/your-username/scoundrel-tui.git
    git clone https://github.com/your-username/scoundrel-core.git
    ```
2.  Navigate to the TUI directory:
    ```bash
    cd scoundrel-tui
    ```
3.  Build the project:
    ```bash
    cmake -B build -S .
    cmake --build build --config Release
    ```

### Usage

Run the game from the build directory:

```bash
# Start in TUI mode (Recommended)
./build/Scoundrel --tui

# Start with a specific theme
./build/Scoundrel --tui --theme assets/themes/tokyo_night.ini

# Start in classic CLI mode
./build/Scoundrel
```

## 🎨 Themes

Themes are located in `assets/themes/`. You can customize your experience by passing the `--theme` flag. Available themes include:
-   `dracula.ini`
-   `gruvbox.ini`
-   `one_dark.ini`
-   `tokyo_night.ini`
-   `synthwave84.ini`
-   ...and more!

## 🛠 Project Structure

-   `src/ui/tui/`: FTXUI-based interactive interface.
-   `src/ui/cli/`: Standard text-based interface.
-   `assets/themes/`: Theme configuration files.
-   `GEMINI.md`: Context and instructions for AI-assisted development.

## 🤝 Contributing

Contributions are welcome! Please feel free to submit Pull Requests or open Issues for bugs and feature requests.

## 📄 License

This project is licensed under the [MIT License](LICENSE) (if applicable).
