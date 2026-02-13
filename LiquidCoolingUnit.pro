#-------------------------------------------------
#
# Project: Liquid Cooling Unit Simulator
# File: LiquidCoolingUnit.pro
# Description: Qt project file for qmake build system
#
#-------------------------------------------------

QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

TARGET = LiquidCoolingUnit
TEMPLATE = app

# Output directories
DESTDIR = bin
OBJECTS_DIR = build/obj
MOC_DIR = build/moc
RCC_DIR = build/rcc
UI_DIR = build/ui

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# Application icon (uncomment and add icon if needed)
# win32:RC_ICONS += resources/app.ico

# Application information
VERSION = 1.0.0
QMAKE_TARGET_COMPANY = "LCU Simulator"
QMAKE_TARGET_PRODUCT = "Liquid Cooling Unit Simulator"
QMAKE_TARGET_DESCRIPTION = "Real-time liquid cooling unit visualization and simulation"
QMAKE_TARGET_COPYRIGHT = "Copyright 2026"

# Source files
SOURCES += \
    src/main.cpp \
    src/mainwindow.cpp \
    src/lcuscene.cpp \
    src/datamodel.cpp \
    src/animationcontroller.cpp \
    src/components/basecomponent.cpp \
    src/components/pump.cpp \
    src/components/valve.cpp \
    src/components/tank.cpp \
    src/components/heater.cpp \
    src/components/heatexchanger.cpp \
    src/components/condenser.cpp \
    src/components/blower.cpp \
    src/components/pipe.cpp \
    src/components/solenoidvalve.cpp

# Header files
HEADERS += \
    src/mainwindow.h \
    src/lcuscene.h \
    src/datamodel.h \
    src/animationcontroller.h \
    src/components/basecomponent.h \
    src/components/pump.h \
    src/components/valve.h \
    src/components/tank.h \
    src/components/heater.h \
    src/components/heatexchanger.h \
    src/components/condenser.h \
    src/components/blower.h \
    src/components/pipe.h \
    src/components/solenoidvalve.h

# Include paths
INCLUDEPATH += \
    src \
    src/components

# Forms (if any UI files are added in the future)
# FORMS += \
#     src/mainwindow.ui

# Resources (if any QRC files are added in the future)
# RESOURCES += \
#     resources/resources.qrc

# Platform-specific settings
win32 {
    # Windows specific settings
    CONFIG += windows
    
    # Remove console window for release builds
    CONFIG(release, debug|release) {
        CONFIG += windows
        CONFIG -= console
    }
    
    # Keep console for debug builds
    CONFIG(debug, debug|release) {
        CONFIG += console
    }
    
    # MSVC compiler flags
    msvc {
        QMAKE_CXXFLAGS += /std:c++17
        QMAKE_CXXFLAGS_WARN_ON += -W3
    }
    
    # MinGW compiler flags
    mingw {
        QMAKE_CXXFLAGS += -std=c++17
        QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra
    }
}

unix:!macx {
    # Linux specific settings
    CONFIG += c++17
    QMAKE_CXXFLAGS += -std=c++17
    QMAKE_CXXFLAGS_WARN_ON += -Wall -Wextra
}

macx {
    # macOS specific settings
    CONFIG += c++17
    QMAKE_CXXFLAGS += -std=c++17
    QMAKE_MACOSX_DEPLOYMENT_TARGET = 10.15
}

# Default rules for deployment
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# Additional clean targets
QMAKE_CLEAN += -r $$DESTDIR $$OBJECTS_DIR $$MOC_DIR $$RCC_DIR $$UI_DIR

# Optimization flags
CONFIG(release, debug|release) {
    DEFINES += QT_NO_DEBUG_OUTPUT
    
    # Enable optimization
    msvc {
        QMAKE_CXXFLAGS_RELEASE += /O2 /Ot
    }
    unix {
        QMAKE_CXXFLAGS_RELEASE += -O3
    }
}

CONFIG(debug, debug|release) {
    DEFINES += DEBUG
}

# Print configuration info
message("Building Liquid Cooling Unit Simulator")
message("Qt version: $$[QT_VERSION]")
message("Build mode: $$CONFIG")
message("Target: $$TARGET")
message("Output directory: $$DESTDIR")
