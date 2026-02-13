@echo off
REM Build script for Windows

echo Building Liquid Cooling Unit Simulator...

REM Create build directory
if not exist build mkdir build
cd build

REM Configure CMake - Adjust Qt path as needed
REM Common Qt installation paths:
REM   C:\Qt\6.5.0\msvc2019_64
REM   C:\Qt\6.6.0\mingw_64
REM   %USERPROFILE%\Qt\6.5.0\msvc2019_64

if defined CMAKE_PREFIX_PATH (
    echo Using CMAKE_PREFIX_PATH: %CMAKE_PREFIX_PATH%
    cmake .. -DCMAKE_PREFIX_PATH=%CMAKE_PREFIX_PATH%
) else (
    echo WARNING: CMAKE_PREFIX_PATH not set
    echo Please set it to your Qt installation directory, for example:
    echo set CMAKE_PREFIX_PATH=C:\Qt\6.5.0\msvc2019_64
    echo.
    echo Attempting build anyway...
    cmake ..
)

if errorlevel 1 (
    echo CMake configuration failed!
    cd ..
    exit /b 1
)

REM Build
cmake --build . --config Release

if errorlevel 1 (
    echo Build failed!
    cd ..
    exit /b 1
)

echo.
echo Build successful!
echo Executable location: build\bin\Release\LiquidCoolingUnit.exe
echo.

cd ..
