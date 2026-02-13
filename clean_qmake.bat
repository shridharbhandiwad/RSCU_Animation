@echo off
REM Clean qmake build artifacts

echo Cleaning qmake build files...

if exist build_qmake (
    echo Removing build_qmake directory...
    rmdir /s /q build_qmake
)

if exist bin (
    echo Removing bin directory...
    rmdir /s /q bin
)

if exist build (
    echo Removing build directory...
    rmdir /s /q build
)

REM Remove qmake generated files
if exist Makefile (
    echo Removing Makefile...
    del /f /q Makefile
)

if exist Makefile.Debug (
    del /f /q Makefile.Debug
)

if exist Makefile.Release (
    del /f /q Makefile.Release
)

if exist .qmake.stash (
    del /f /q .qmake.stash
)

echo.
echo Clean completed!
pause
