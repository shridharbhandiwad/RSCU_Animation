cmake_minimum_required(VERSION 3.16)
project(LiquidCoolingUnit VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# Find Qt6 packages
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# Source files
set(SOURCES
    src/main.cpp
    src/mainwindow.cpp
    src/lcuscene.cpp
    src/components/basecomponent.cpp
    src/components/pump.cpp
    src/components/valve.cpp
    src/components/tank.cpp
    src/components/heater.cpp
    src/components/heatexchanger.cpp
    src/components/condenser.cpp
    src/components/blower.cpp
    src/components/pipe.cpp
    src/components/solenoidvalve.cpp
    src/datamodel.cpp
    src/animationcontroller.cpp
)

# Header files
set(HEADERS
    src/mainwindow.h
    src/lcuscene.h
    src/components/basecomponent.h
    src/components/pump.h
    src/components/valve.h
    src/components/tank.h
    src/components/heater.h
    src/components/heatexchanger.h
    src/components/condenser.h
    src/components/blower.h
    src/components/pipe.h
    src/components/solenoidvalve.h
    src/datamodel.h
    src/animationcontroller.h
)

# Create executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS})

# Link Qt libraries
target_link_libraries(${PROJECT_NAME}
    Qt6::Core
    Qt6::Gui
    Qt6::Widgets
)

# Include directories
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/components
)

# Set output directory
set_target_properties(${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

# Windows specific settings
if(WIN32)
    set_target_properties(${PROJECT_NAME} PROPERTIES
        WIN32_EXECUTABLE TRUE
    )
endif()
