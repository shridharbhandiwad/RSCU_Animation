# Quick Start Guide - qmake Build

The simplest way to build and run the Liquid Cooling Unit Simulator using Qt's native build system.

## For Qt Creator Users (Easiest!)

### 3 Steps to Run:

1. **Open Project**
   - Launch Qt Creator
   - File → Open File or Project
   - Select `LiquidCoolingUnit.pro`

2. **Configure**
   - Select your Qt Kit (Qt 6.x MSVC or MinGW)
   - Click "Configure Project"

3. **Run**
   - Press **Ctrl+R** (or click the green play button)
   - Application starts immediately!

That's it! Qt Creator handles everything automatically.

## For Command Line Users

### Windows - Qt Command Prompt (Recommended)

1. **Open Qt Command Prompt**
   - Start Menu → Qt → Qt 6.x.x for Desktop (MSVC 2019 64-bit)
   - OR: Qt 6.x.x for Desktop (MinGW 64-bit)

2. **Navigate to Project**
   ```batch
   cd C:\path\to\LiquidCoolingUnit
   ```

3. **Build**
   ```batch
   build_qmake.bat
   ```

4. **Run**
   ```batch
   run_qmake.bat
   ```

### Windows - Regular Command Prompt

If Qt is in your system PATH:

```batch
# Build
mkdir build_qmake
cd build_qmake
qmake ..
nmake          # For MSVC
mingw32-make   # For MinGW
cd ..

# Run
run_qmake.bat
```

### Linux

```bash
# Install Qt6 if not already installed
sudo apt install qt6-base-dev

# Build
mkdir build_qmake && cd build_qmake
qmake ..
make -j4

# Run
cd ..
./bin/LiquidCoolingUnit
```

### macOS

```bash
# Install Qt6 if not already installed
brew install qt@6

# Add Qt to PATH
export PATH="/usr/local/opt/qt@6/bin:$PATH"

# Build
mkdir build_qmake && cd build_qmake
qmake ..
make -j4

# Run
cd ..
./bin/LiquidCoolingUnit
```

## What Does the .pro File Do?

The `LiquidCoolingUnit.pro` file tells Qt's build system:
- Which source files to compile
- Which Qt modules to link (Core, Gui, Widgets)
- Where to put the executable (bin/ folder)
- C++17 standard settings
- Platform-specific configurations

## Build Outputs

After building, you'll find:
- **Executable**: `build_qmake/bin/LiquidCoolingUnit.exe` (Windows)
- **Object files**: `build_qmake/build/obj/`
- **Generated files**: `build_qmake/build/moc/`

## Clean Build

To start fresh:

```batch
# Windows
clean_qmake.bat

# Linux/macOS
rm -rf build_qmake bin build
```

## Switching Between Debug and Release

### Debug Build (with debug symbols):
```batch
qmake CONFIG+=debug
make
```

### Release Build (optimized):
```batch
qmake CONFIG+=release
make
```

## Troubleshooting

### "qmake: command not found"

**Solution:** Add Qt to your PATH or use Qt Command Prompt

Windows:
```batch
set PATH=C:\Qt\6.5.0\msvc2019_64\bin;%PATH%
```

Linux:
```bash
export PATH="/usr/lib/qt6/bin:$PATH"
```

### "nmake: command not found" (Windows)

**Solution:** Use Visual Studio Command Prompt or Qt Command Prompt (MSVC version)

### "mingw32-make: command not found" (Windows)

**Solution:** Use Qt Command Prompt (MinGW version) or add MinGW to PATH:
```batch
set PATH=C:\Qt\Tools\mingw1120_64\bin;%PATH%
```

### Build errors about C++17

**Solution:** Your Qt kit is configured correctly. The .pro file already sets C++17.

### Can't find Qt modules

**Solution:** Make sure you have Qt 6.x installed (not Qt 5.x)
```batch
qmake -v
```
Should show: "Using Qt version 6.x.x"

## Features of This .pro File

✅ **Automatic Qt module detection** (Core, Gui, Widgets)  
✅ **C++17 standard** enabled  
✅ **Organized output directories** (bin/, build/)  
✅ **Platform-specific optimizations** (Windows/Linux/macOS)  
✅ **Debug and Release configurations**  
✅ **All source files included automatically**  
✅ **Component directory structure preserved**  

## Comparison: qmake vs CMake

| Feature | qmake | CMake |
|---------|-------|-------|
| Setup difficulty | ⭐ Easy | ⭐⭐ Moderate |
| Qt Creator integration | ⭐⭐⭐ Native | ⭐⭐ Good |
| Command line | ⭐⭐⭐ Simple | ⭐⭐ More complex |
| Cross-platform | ⭐⭐ Good | ⭐⭐⭐ Excellent |
| Non-Qt libraries | ⭐ Limited | ⭐⭐⭐ Excellent |

**Recommendation:** Use **qmake** for simplest Qt-focused development!

## Next Steps

1. ✅ Build the project (using steps above)
2. ✅ Run the application
3. ✅ Click "Start System" button
4. ✅ Watch the animations!
5. 📖 Read TESTING.md for testing procedures
6. 🔌 Read INTEGRATION_GUIDE.md for connecting external data

## Additional Resources

- **Full build guide**: See [BUILD_GUIDE.md](BUILD_GUIDE.md)
- **User manual**: See [README.md](README.md)
- **Testing**: See [TESTING.md](TESTING.md)
- **Qt documentation**: https://doc.qt.io

---

**Quick Command Reference:**

```batch
# Build
build_qmake.bat

# Run
run_qmake.bat

# Clean
clean_qmake.bat

# That's all you need to know! 🚀
```
