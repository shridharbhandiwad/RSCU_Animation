# Build Guide - Multiple Build Systems

This project supports two build systems: **CMake** (recommended) and **qmake** (Qt native).

## Build System Comparison

| Feature | CMake | qmake |
|---------|-------|-------|
| Cross-platform | ✅ Excellent | ✅ Good |
| Qt Creator Integration | ✅ Yes | ✅ Native |
| Visual Studio Integration | ✅ Yes | ⚠️ Limited |
| Command Line | ✅ Yes | ✅ Yes |
| Configuration | More flexible | Simpler |
| Recommended for | Modern projects | Qt-only projects |

## Option 1: Build with CMake (Recommended)

CMake is the modern, cross-platform build system with excellent IDE support.

### Prerequisites
- Qt 6.x installed
- CMake 3.16 or later
- C++17 compatible compiler (MSVC 2019+, GCC 8+, Clang 7+)

### Windows with Visual Studio

```batch
REM Open Visual Studio Command Prompt or set environment
REM Set Qt path
set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\msvc2019_64

REM Build
build.bat

REM Run
run.bat
```

### Windows with MinGW

```batch
REM Set Qt path
set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\mingw_64

REM Build manually
mkdir build
cd build
cmake .. -G "MinGW Makefiles"
cmake --build .

REM Run
..\run.bat
```

### Linux

```bash
# Install Qt6
sudo apt install qt6-base-dev cmake build-essential

# Build
mkdir build && cd build
cmake ..
cmake --build .

# Run
./bin/LiquidCoolingUnit
```

### macOS

```bash
# Install Qt6
brew install qt@6 cmake

# Build
mkdir build && cd build
cmake .. -DCMAKE_PREFIX_PATH=/usr/local/opt/qt@6
cmake --build .

# Run
./bin/LiquidCoolingUnit
```

### Qt Creator with CMake

1. Open `CMakeLists.txt` in Qt Creator
2. Configure project with your Qt Kit
3. Build: Ctrl+B (Windows/Linux) or Cmd+B (macOS)
4. Run: Ctrl+R (Windows/Linux) or Cmd+R (macOS)

## Option 2: Build with qmake (Qt Native)

qmake is Qt's native build system, simpler for Qt-only projects.

### Prerequisites
- Qt 6.x installed (includes qmake)
- C++17 compatible compiler

### Windows - Qt Command Prompt (Easiest)

1. Start Menu → Qt → Qt 6.x → MSVC/MinGW → Qt Command Prompt
2. Navigate to project directory
3. Run:

```batch
build_qmake.bat
run_qmake.bat
```

### Windows - Manual Build

#### With MSVC:

```batch
REM Open Visual Studio Command Prompt
REM Add Qt to PATH
set PATH=C:\Qt\6.5.0\msvc2019_64\bin;%PATH%

REM Build
mkdir build_qmake
cd build_qmake
qmake ..
nmake

REM Run
cd ..
run_qmake.bat
```

#### With MinGW:

```batch
REM Add Qt MinGW to PATH
set PATH=C:\Qt\6.5.0\mingw_64\bin;%PATH%
set PATH=C:\Qt\Tools\mingw1120_64\bin;%PATH%

REM Build
mkdir build_qmake
cd build_qmake
qmake ..
mingw32-make

REM Run
cd ..
run_qmake.bat
```

### Linux with qmake

```bash
# Install Qt6
sudo apt install qt6-base-dev

# Build
mkdir build_qmake && cd build_qmake
qmake ..
make

# Run
../bin/LiquidCoolingUnit
```

### macOS with qmake

```bash
# Install Qt6
brew install qt@6

# Add qmake to PATH
export PATH="/usr/local/opt/qt@6/bin:$PATH"

# Build
mkdir build_qmake && cd build_qmake
qmake ..
make

# Run
../bin/LiquidCoolingUnit
```

### Qt Creator with qmake

1. Open `LiquidCoolingUnit.pro` in Qt Creator
2. Configure project with your Qt Kit
3. Build: Ctrl+B (Windows/Linux) or Cmd+B (macOS)
4. Run: Ctrl+R (Windows/Linux) or Cmd+R (macOS)

## Choosing the Right Build System

### Use CMake if:
- ✅ You want modern, industry-standard build system
- ✅ You need Visual Studio project generation
- ✅ You plan to integrate non-Qt libraries
- ✅ You prefer cross-platform build consistency
- ✅ You're new to Qt (learning modern practices)

### Use qmake if:
- ✅ You primarily use Qt Creator
- ✅ You want simpler Qt-focused builds
- ✅ You're familiar with Qt's traditional workflow
- ✅ You have existing qmake projects
- ✅ You want quick Qt-native builds

## Build System Files

### CMake Files
- `CMakeLists.txt` - Main build configuration
- `build.bat` - Windows build script
- `run.bat` - Windows run script
- `build/` - Build output directory (gitignored)

### qmake Files
- `LiquidCoolingUnit.pro` - Qt project file
- `build_qmake.bat` - Windows build script
- `run_qmake.bat` - Windows run script
- `clean_qmake.bat` - Clean build artifacts
- `build_qmake/` - Build output directory (gitignored)

## Common Build Issues

### Issue: Qt not found

**CMake:**
```batch
REM Solution: Set CMAKE_PREFIX_PATH
set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\msvc2019_64
```

**qmake:**
```batch
REM Solution: Add Qt bin to PATH
set PATH=C:\Qt\6.5.0\msvc2019_64\bin;%PATH%
```

### Issue: Compiler not found

**Solution:** Run from appropriate command prompt:
- MSVC: Visual Studio Developer Command Prompt
- MinGW: Add MinGW bin to PATH
- Linux: `sudo apt install build-essential`

### Issue: Wrong Qt version

**Solution:** Check Qt version:
```batch
qmake -v
cmake --version
```

Ensure Qt 6.x is in PATH (not Qt 5.x).

### Issue: Permission denied (Linux/macOS)

**Solution:**
```bash
chmod +x build.sh
chmod +x bin/LiquidCoolingUnit
```

### Issue: Missing dependencies (Linux)

**Solution:**
```bash
# Ubuntu/Debian
sudo apt install qt6-base-dev libqt6core6 libqt6gui6 libqt6widgets6

# Fedora
sudo dnf install qt6-qtbase-devel

# Arch
sudo pacman -S qt6-base
```

## Clean Build

### CMake:
```batch
REM Windows
rmdir /s /q build

# Linux/macOS
rm -rf build
```

### qmake:
```batch
REM Windows
clean_qmake.bat

# Linux/macOS
rm -rf build_qmake bin build
rm -f Makefile Makefile.Debug Makefile.Release .qmake.stash
```

## Build Configurations

### Debug Build

**CMake:**
```batch
cmake --build . --config Debug
```

**qmake:**
```batch
qmake CONFIG+=debug
make
```

### Release Build (Optimized)

**CMake:**
```batch
cmake --build . --config Release
```

**qmake:**
```batch
qmake CONFIG+=release
make
```

## Build Performance Tips

1. **Parallel builds**:
   - CMake: `cmake --build . -j 4`
   - qmake: `make -j4`

2. **Incremental builds**: Only changed files are rebuilt

3. **Clean rebuild when**:
   - Switching between Debug/Release
   - After modifying CMakeLists.txt or .pro file
   - After updating Qt version

## IDE Support

### Visual Studio 2019/2022
- Open folder containing `CMakeLists.txt`
- Visual Studio auto-detects CMake
- Configure and build from IDE

### Qt Creator
- **CMake**: Open `CMakeLists.txt`
- **qmake**: Open `LiquidCoolingUnit.pro`
- Both work seamlessly

### VS Code
- Install CMake Tools extension
- Open folder
- Configure and build via command palette

### CLion
- Open folder containing `CMakeLists.txt`
- JetBrains CLion has excellent CMake support

## Deployment

### Windows

After building, copy Qt DLLs alongside executable:

```batch
REM Navigate to output directory
cd build\bin\Release

REM Run Qt deployment tool
windeployqt LiquidCoolingUnit.exe
```

This copies all required Qt DLLs.

### Linux

```bash
# Check dependencies
ldd bin/LiquidCoolingUnit

# Create AppImage (optional)
# Use linuxdeployqt tool
```

### macOS

```bash
# Create app bundle
macdeployqt LiquidCoolingUnit.app
```

## Continuous Integration

### GitHub Actions Example

```yaml
name: Build

on: [push, pull_request]

jobs:
  build-windows:
    runs-on: windows-latest
    steps:
      - uses: actions/checkout@v3
      - uses: jurplel/install-qt-action@v3
        with:
          version: '6.5.0'
      - name: Build with CMake
        run: |
          mkdir build
          cd build
          cmake ..
          cmake --build . --config Release
```

## Troubleshooting

### Check Build Environment

```batch
REM Windows
where qmake
where cmake
where cl.exe

# Linux/macOS
which qmake
which cmake
which g++
```

### Verbose Build Output

**CMake:**
```batch
cmake --build . --config Release --verbose
```

**qmake:**
```batch
make VERBOSE=1
```

### Get Help

- Qt documentation: https://doc.qt.io
- CMake documentation: https://cmake.org/documentation/
- Project issues: Check PROJECT_SUMMARY.md

## Quick Reference

| Task | CMake | qmake |
|------|-------|-------|
| Generate build files | `cmake ..` | `qmake ..` |
| Build | `cmake --build .` | `nmake` or `make` |
| Clean | `cmake --build . --target clean` | `make clean` |
| Rebuild | `cmake --build . --clean-first` | `make clean && make` |
| Release build | `cmake --build . --config Release` | `qmake CONFIG+=release && make` |

---

**Recommendation**: Start with **qmake** if you're using Qt Creator for simplest setup. Use **CMake** for professional projects and better IDE flexibility.
