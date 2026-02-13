@echo off
REM Run script for qmake build

if exist build_qmake\bin\LiquidCoolingUnit.exe (
    echo Starting Liquid Cooling Unit Simulator...
    echo.
    build_qmake\bin\LiquidCoolingUnit.exe
) else if exist build_qmake\bin\release\LiquidCoolingUnit.exe (
    echo Starting Liquid Cooling Unit Simulator (Release)...
    echo.
    build_qmake\bin\release\LiquidCoolingUnit.exe
) else if exist build_qmake\bin\debug\LiquidCoolingUnit.exe (
    echo Starting Liquid Cooling Unit Simulator (Debug)...
    echo.
    build_qmake\bin\debug\LiquidCoolingUnit.exe
) else (
    echo ERROR: Executable not found!
    echo.
    echo Please build the project first using build_qmake.bat
    pause
    exit /b 1
)
