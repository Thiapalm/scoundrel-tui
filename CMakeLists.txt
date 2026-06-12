cmake_minimum_required(VERSION 3.16)
project(ScoundrelTUI)

include(FetchContent)
FetchContent_Declare(ftxui
  GIT_REPOSITORY https://github.com/ArthurSonzogni/ftxui
  GIT_TAG v5.0.0
)
FetchContent_MakeAvailable(ftxui)

# Assuming ScoundrelCore is in the parent directory for isolated development
set(BUILD_AS_SUBMODULE ON)
add_subdirectory(${CMAKE_SOURCE_DIR}/../scoundrel-core scoundrel-core)

# UI Source Files
set(UI_SOURCES
    src/ui/cli/CLIUserInterface.cpp
    src/ui/tui/TUIUserInterface.cpp
    src/ui/tui/ThemeLoader.cpp
)

# 1. Create the ScoundrelUI Library (Console/TUI)
add_library(ScoundrelUI STATIC ${UI_SOURCES})

# ScoundrelUI needs ScoundrelCore and FTXUI
target_link_libraries(ScoundrelUI PUBLIC 
    ScoundrelCore
    ftxui::screen 
    ftxui::dom 
    ftxui::component
)

# Include directories for the library
target_include_directories(ScoundrelUI PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/ui/cli>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/src/ui/tui>
)

# 2. Create the main executable
add_executable(Scoundrel src/main.cpp)
target_link_libraries(Scoundrel PRIVATE ScoundrelUI)
