# OpenGL Template VS Code

Cross-platform OpenGL starter for Windows (MSYS2/MinGW-w64) and macOS.

## Prerequisites

### Windows

- [MSYS2](https://www.msys2.org/) with MinGW-w64 toolchain:
  ```
  pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-cmake mingw-w64-x86_64-make mingw-w64-x86_64-gdb
  ```
- [Python 3](https://www.python.org/) (for GLAD generation)
- If your MSYS2 is not at `C:/msys64`, update the path in:
  - `cmake/toolchain-mingw64.cmake` → `MSYS2_ROOT`
  - `.vscode/settings.json` → `grafika.msys2Root`

### macOS

- Xcode Command Line Tools: `xcode-select --install`
- Homebrew packages:
  ```
  brew install cmake glfw
  ```
- Python 3 (usually preinstalled or `brew install python`)

## Setup dependencies

The `deps/` folder is not tracked by git. Run these commands once after cloning:

### GLAD (OpenGL loader)

```bash
pip install glad2
glad --out-path deps/glad --api gl:core=4.6 c
```

### GLM (math library, header-only)

```bash
curl -L https://github.com/g-truc/glm/releases/download/1.0.1/glm-1.0.1-light.zip -o glm.zip
mkdir -p deps
unzip -o glm.zip -d deps/
rm glm.zip
```

### GLFW (Windows only — prebuilt binaries)

```bash
curl -L https://github.com/glfw/glfw/releases/download/3.4/glfw-3.4.bin.WIN64.zip -o glfw.zip
unzip -o glfw.zip -d glfw-temp
mkdir -p deps/glfw/include deps/glfw/lib
cp -r glfw-temp/glfw-3.4.bin.WIN64/include/GLFW deps/glfw/include/
cp glfw-temp/glfw-3.4.bin.WIN64/lib-mingw-w64/* deps/glfw/lib/
rm -rf glfw-temp glfw.zip
```

On macOS, GLFW is installed via Homebrew — no manual download needed.

## Build

```bash
# Windows (from MSYS2 MinGW64 shell)
cmake --preset mingw64-debug
cmake --build --preset debug

# macOS
cmake --preset macos-debug
cmake --build --preset debug-mac
```

The executable is placed in `build/`.

## VS Code

Open the project in VS Code. Pre-configured tasks and launch configs are included:

- **Ctrl+Shift+B** — Build Debug
- **F5** — Debug with GDB (Windows) or LLDB (macOS)

## Project structure

```
src/main.cpp          — application entry point
deps/                 — dependencies (not tracked, see setup above)
  glad/               — OpenGL loader (generated, GL 4.6 core)
  glfw/               — GLFW 3.4 prebuilt (Windows only)
  glm/                — GLM math library (header-only)
cmake/                — toolchain files
.vscode/              — editor tasks, launch configs, settings
```
