# Liquid Cooling Unit (LCU) Simulator

A comprehensive Qt C++ application that simulates a liquid cooling unit system with real-time animations and data visualization.

## Features

- **Complete System Simulation**: Coolant system with tank, heater, and pumps
- **Multi-Channel Support**: 4 independent cooling channels with controllable valves
- **Refrigerant System**: 3 independent refrigerant loops with PHE, solenoid valves, condensers, and blowers
- **Real-time Animation**: Fluid flow visualization, rotating pumps and blowers, valve operations
- **Live Data Display**: Temperature, pressure, flow rate sensors updated in real-time
- **Interactive Controls**: Start/stop system, adjust cooling capacity, monitor component status

## System Components

### Coolant System
- Coolant tank with level indicator
- Electric heater with visual feedback
- Dual redundant pumps
- Temperature and pressure sensors

### Channel System (CH1-4)
- 4 parallel cooling channels
- Individual flow control valves
- Flow rate monitoring per channel

### Refrigerant System
- 3 independent cooling loops
- Plate Heat Exchangers (PHE)
- Solenoid valves with energization animation
- Air-cooled condensers
- Variable speed blowers

## Building on Windows

### Prerequisites

1. **Qt 6.x** - Download and install from https://www.qt.io/download
   - Make sure Qt6 Widgets module is installed
   - Add Qt to your system PATH

2. **CMake 3.16+** - Download from https://cmake.org/download/

3. **Visual Studio 2019/2022** or **MinGW** compiler

### Build Instructions

#### Using Command Line (CMake + Visual Studio)

```bash
# Create build directory
mkdir build
cd build

# Configure with CMake (adjust Qt path as needed)
cmake .. -DCMAKE_PREFIX_PATH="C:/Qt/6.x/msvc2019_64"

# Build
cmake --build . --config Release

# Run
./bin/Release/LiquidCoolingUnit.exe
```

#### Using Qt Creator

1. Open `CMakeLists.txt` in Qt Creator
2. Configure the project with your Qt kit
3. Build and run (Ctrl+R)

#### Using Visual Studio

1. Open the folder in Visual Studio 2019/2022
2. Visual Studio will auto-detect CMakeLists.txt
3. Select the build configuration
4. Build and run (F5)

## Usage

### Starting the System

1. Click **"Start System"** to begin operation
2. The coolant pumps will activate
3. Channels 1 and 2 will open automatically
4. Refrigerant loop 1 will start

### Monitoring

- **Left Panel**: System control buttons and cooling capacity adjustment
- **Right Panel**: Real-time sensor readings and component status
- **Main View**: Animated system diagram with flowing coolant and active components

### Controls

- **Start System**: Activates the cooling system
- **Stop System**: Safely shuts down all components
- **Reset All Trips**: Clears any alarm conditions
- **Cooling Capacity**: Adjust between 0-100 kW

## Data Model API

The system can receive external data through the `DataModel` class:

```cpp
// System control
dataModel->setSystemRunning(true/false);

// Coolant parameters
dataModel->setSupplyTemp(25.5);        // °C
dataModel->setReturnTemp(30.2);        // °C
dataModel->setSystemPressure(2.5);     // Bar
dataModel->setFlowRate(100.0);         // lpm

// Channel control (0-3)
dataModel->setChannelState(0, true);   // Open channel 0
dataModel->setChannelFlowRate(0, 25.0); // lpm

// Pump control (0-1)
dataModel->setPumpState(0, true);      // Start pump 0

// Refrigerant system (0-2)
dataModel->setCompressorState(0, true);     // Start compressor 0
dataModel->setBlowerState(0, true);         // Start blower 0
dataModel->setSolenoidValveState(0, true);  // Open solenoid valve 0
```

## Project Structure

```
LiquidCoolingUnit/
├── CMakeLists.txt
├── README.md
├── src/
│   ├── main.cpp
│   ├── mainwindow.h/cpp
│   ├── lcuscene.h/cpp
│   ├── datamodel.h/cpp
│   ├── animationcontroller.h/cpp
│   └── components/
│       ├── basecomponent.h/cpp
│       ├── pump.h/cpp
│       ├── valve.h/cpp
│       ├── tank.h/cpp
│       ├── heater.h/cpp
│       ├── heatexchanger.h/cpp
│       ├── condenser.h/cpp
│       ├── blower.h/cpp
│       ├── solenoidvalve.h/cpp
│       └── pipe.h/cpp
```

## Sample Data

The application includes built-in simulation that generates realistic sample data:
- Oscillating temperatures (realistic thermal dynamics)
- Pressure variations based on flow
- Dynamic flow rates based on pump and valve states
- Component state transitions with animations

## Troubleshooting

### Qt not found
- Ensure Qt6 is properly installed
- Set `CMAKE_PREFIX_PATH` to your Qt installation directory
- Example: `-DCMAKE_PREFIX_PATH=C:/Qt/6.5.0/msvc2019_64`

### Build errors
- Verify all source files are present
- Check that Qt6 Widgets module is installed
- Ensure C++17 compiler support

### Runtime issues
- Verify Qt DLLs are in PATH or copied to executable directory
- Check that all required Qt modules (Core, Gui, Widgets) are available

## License

This project is provided as-is for educational and demonstration purposes.

## Author

Created for liquid cooling system simulation and monitoring applications.
