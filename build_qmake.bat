@echo off
REM Build script for Windows using qmake
REM This is an alternative to build.bat which uses CMake

echo ========================================
echo Building with qmake (Qt native build)
echo ========================================
echo.

REM Check if qmake is in PATH
where qmake >nul 2>&1
if %errorlevel% neq 0 (
    echo ERROR: qmake not found in PATH!
    echo.
    echo Please ensure Qt is installed and one of these is in your PATH:
    echo   - C:\Qt\6.x\msvc2019_64\bin
    echo   - C:\Qt\6.x\mingw_64\bin
    echo.
    echo Or run this from Qt command prompt.
    pause
    exit /b 1
)

REM Show qmake version
echo Using qmake:
qmake -v
echo.

REM Create build directory
if not exist build_qmake mkdir build_qmake
cd build_qmake

REM Run qmake
echo Running qmake...
qmake ..

if %errorlevel% neq 0 (
    echo.
    echo ERROR: qmake failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo qmake completed successfully.
echo.

REM Build using nmake (MSVC) or mingw32-make (MinGW)
echo Building project...

where nmake >nul 2>&1
if %errorlevel% equ 0 (
    echo Using nmake (MSVC)...
    nmake
) else (
    where mingw32-make >nul 2>&1
    if %errorlevel% equ 0 (
        echo Using mingw32-make (MinGW)...
        mingw32-make
    ) else (
        echo ERROR: Neither nmake nor mingw32-make found!
        echo Please run from Qt command prompt or MSVC/MinGW command prompt.
        cd ..
        pause
        exit /b 1
    )
)

if %errorlevel% neq 0 (
    echo.
    echo ERROR: Build failed!
    cd ..
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build successful!
echo ========================================
echo.
echo Executable location: build_qmake\bin\LiquidCoolingUnit.exe
echo.
echo To run the application, execute: run_qmake.bat
echo.

cd ..
pause
