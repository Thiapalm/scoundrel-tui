# scoundrel-tui Build Instructions

This project is a TUI frontend that depends on `scoundrel-core` for game logic.

## Build with CMake

### Windows

1. Open a PowerShell terminal and navigate to the folder:
   ```powershell
   cd \path\to\scoundrelProj\scoundrel-tui
   ```
2. Create a build directory and configure CMake:
   ```powershell
   cmake -B build -S .
   ```
3. Build the TUI executable:
   ```powershell
   cmake --build build --config Release
   ```
4. Run the executable (located in `build/Release/`):
   ```powershell
   .\build\Release\Scoundrel.exe
   ```
   *(To start in TUI mode add `--tui`, for example: `.\build\Release\Scoundrel.exe --tui`)*

### Linux

1. Open a terminal and navigate to the folder:
   ```bash
   cd /path/to/scoundrelProj/scoundrel-tui
   ```
2. Create a build directory and configure CMake:
   ```bash
   cmake -B build -S .
   ```
3. Build the TUI executable:
   ```bash
   cmake --build build --config Release
   ```
4. Run the executable (located in `build/`):
   ```bash
   ./build/Scoundrel
   ```
   *(To start in TUI mode add `--tui`, for example: `./build/Scoundrel --tui`)*

## How it links against `scoundrel-core`

The `CMakeLists.txt` in this folder includes `scoundrel-core` as a subdirectory:

```cmake
add_subdirectory(${CMAKE_SOURCE_DIR}/../scoundrel-core scoundrel-core)
```

This means the runtime and build must preserve the sibling structure:

- `scoundrelProj/scoundrel-tui`
- `scoundrelProj/scoundrel-core`

The TUI target links against `ScoundrelCore` via:

```cmake
target_link_libraries(ScoundrelUI PUBLIC ScoundrelCore ...)
```

## Required files when `scoundrel-core` is not present

This project currently depends on the `scoundrel-core` source tree or an equivalent prebuilt library and headers. The easiest working setup is to keep the sibling source tree in place and preserve the relative path used by CMake.

Required items:

- `scoundrel-core/CMakeLists.txt`
- `scoundrel-core/include/...` public `scoundrel` headers
- `scoundrel-core/src/...` core sources and implementation files
- any additional `scoundrel-core` resources required by the library

If `scoundrel-core` is missing, CMake will fail during configuration because this project currently uses `add_subdirectory(...)`. To build without the source tree, you would need a compatible built `ScoundrelCore` static library plus matching headers, and you would also need to update the CMakeLists to find and link that library instead of using `add_subdirectory(...)`.

## Notes

- `scoundrel-core` is built automatically by this project.
- If you move the folders, update the path used in `add_subdirectory(...)`.
