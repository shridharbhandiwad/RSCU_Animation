# Liquid Cooling Unit (LCU) Simulator

A comprehensive Qt C++ application that simulates a liquid cooling unit system with real-time animations and data visualization.

## Features

- **Complete System Simulation**: Coolant system with tank, heater, and pumps
- **Multi-Channel Support**: 4 independent cooling channels with controllable valves
- **Refrigerant System**: 3 independent refrigerant loops with PHE, solenoid valves, condensers, and blowers
- **Real-time Animation**: Fluid flow visualization, rotating pumps and blowers, valve operations
- **Live Data Display**: Temperature, pressure, flow rate sensors updated in real-time
- **Interactive Controls**: Start/stop system, adjust cooling capacity, monitor component status
- **3D Visualization**: Toggle between 2D schematic and interactive 3D model views with orbit camera controls

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

This project supports two build systems: **CMake** and **qmake**. Choose the one you prefer.

### Prerequisites

1. **Qt 6.x** - Download and install from https://www.qt.io/download
   - Make sure Qt6 Widgets module is installed
   - **Qt3D modules required** (Qt3DCore, Qt3DRender, Qt3DInput, Qt3DExtras)
   - Add Qt to your system PATH (optional for CMake, required for qmake)

2. **Compiler**: Visual Studio 2019/2022 or MinGW (included with Qt)

3. **CMake 3.16+** (only if using CMake build) - Download from https://cmake.org/download/

### Build Instructions

#### Option 1: Using qmake (Simplest - Recommended for Qt Users)

```bash
# Open Qt Command Prompt (from Start Menu → Qt → Qt 6.x)
# Navigate to project directory

# Build
build_qmake.bat

# Run
run_qmake.bat
```

#### Option 2: Using CMake

```bash
# Set Qt path
set CMAKE_PREFIX_PATH=C:/Qt/6.x/msvc2019_64

# Build
build.bat

# Run
run.bat
```

#### Using Qt Creator (Easiest)

**With qmake:**
1. Open `LiquidCoolingUnit.pro` in Qt Creator
2. Configure the project with your Qt kit
3. Build and run (Ctrl+R)

**With CMake:**
1. Open `CMakeLists.txt` in Qt Creator
2. Configure the project with your Qt kit
3. Build and run (Ctrl+R)

#### Using Visual Studio

1. Open the folder in Visual Studio 2019/2022
2. Visual Studio will auto-detect CMakeLists.txt
3. Select the build configuration
4. Build and run (F5)

**For detailed build instructions, see [BUILD_GUIDE.md](BUILD_GUIDE.md)**

## Usage

### Starting the System

1. Click **"Start System"** to begin operation
2. The coolant pumps will activate
3. Channels 1 and 2 will open automatically
4. Refrigerant loop 1 will start

### View Modes

The application supports both 2D and 3D visualization modes:

#### 2D View (Default)
- Traditional schematic diagram view
- Clear representation of all components and connections
- Optimal for understanding system layout and monitoring

#### 3D View
- Fully interactive 3D model of the LCU system
- **Mouse Controls**:
  - Left-click + drag: Rotate view
  - Right-click + drag: Pan view
  - Scroll wheel: Zoom in/out
- Realistic representation of physical components
- Toggle between views using the **"Switch to 3D/2D View"** button

### Monitoring

- **Left Panel**: System control buttons and cooling capacity adjustment
- **Right Panel**: Real-time sensor readings and component status
- **Main View**: Animated system diagram (2D) or interactive 3D model with flowing coolant and active components

### Controls

- **Start System**: Activates the cooling system
- **Stop System**: Safely shuts down all components
- **Reset All Trips**: Clears any alarm conditions
- **Switch View**: Toggle between 2D schematic and 3D model views
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
├── CMakeLists.txt              # CMake build configuration
├── LiquidCoolingUnit.pro       # qmake project file
├── README.md                   # This file
├── BUILD_GUIDE.md             # Detailed build instructions
├── TESTING.md                 # Testing procedures
├── INTEGRATION_GUIDE.md       # External data integration
├── build.bat                  # CMake build script (Windows)
├── build_qmake.bat           # qmake build script (Windows)
├── run.bat                    # Run script for CMake build
├── run_qmake.bat             # Run script for qmake build
├── clean_qmake.bat           # Clean qmake build artifacts
├── test_data.json            # Sample test scenarios
└── src/
    ├── main.cpp
    ├── mainwindow.h/cpp
    ├── lcuscene.h/cpp           # 2D scene
    ├── lcuscene3d.h/cpp         # 3D scene (NEW)
    ├── datamodel.h/cpp
    ├── animationcontroller.h/cpp      # 2D animations
    ├── animationcontroller3d.h/cpp    # 3D animations (NEW)
    └── components/
        ├── basecomponent.h/cpp
        ├── pump.h/cpp
        ├── valve.h/cpp
        ├── tank.h/cpp
        ├── heater.h/cpp
        ├── heatexchanger.h/cpp
        ├── condenser.h/cpp
        ├── blower.h/cpp
        ├── solenoidvalve.h/cpp
        └── pipe.h/cpp
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
