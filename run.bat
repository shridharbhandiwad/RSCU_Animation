@echo off
REM Run script for Liquid Cooling Unit Simulator

if exist build\bin\Release\LiquidCoolingUnit.exe (
    echo Starting Liquid Cooling Unit Simulator...
    build\bin\Release\LiquidCoolingUnit.exe
) else if exist build\bin\Debug\LiquidCoolingUnit.exe (
    echo Starting Liquid Cooling Unit Simulator (Debug)...
    build\bin\Debug\LiquidCoolingUnit.exe
) else (
    echo ERROR: Executable not found!
    echo Please build the project first using build.bat
    exit /b 1
)
