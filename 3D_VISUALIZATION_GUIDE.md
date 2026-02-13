# 3D Visualization Guide for LCU Simulator

## Overview

The Liquid Cooling Unit (LCU) Simulator now includes a fully interactive 3D visualization mode using Qt3D framework. This guide explains how to use the 3D features and what to expect.

## Features

### 3D Components

All major LCU components are represented in 3D:

1. **Coolant Tank**
   - Vertical cylindrical tank
   - Blue material representing coolant
   - Located at the left side of the system

2. **Heater**
   - Red box below the tank
   - Indicates heating element

3. **Coolant Pumps (2x)**
   - Cylindrical housing with internal impeller
   - Rotating animation when active
   - Rotation speed: 180°/second

4. **Channel Valves (4x)**
   - Gray cylindrical bodies
   - Orange spherical handles
   - Positioned along the distribution manifold

5. **Heat Exchangers (3x)**
   - Flat rectangular boxes
   - Gray metallic finish
   - One per refrigerant loop

6. **Solenoid Valves (3x)**
   - Small blue-gray cylinders
   - Control refrigerant flow

7. **Condensers (3x)**
   - Large boxes with visible fins
   - Fin structure for heat dissipation
   - Gray aluminum-like material

8. **Blowers (3x)**
   - Cylindrical housing with 6-blade fans
   - Rotating animation when active
   - Rotation speed: 360°/second
   - Blue-gray housing

9. **Piping System**
   - Blue pipes: Coolant system
   - Red pipes: Refrigerant system
   - Realistic 3D pipe routing

10. **Floor**
    - Gray platform for spatial reference

## User Controls

### Switching Between Views

- **Button**: "Switch to 3D View" / "Switch to 2D View"
- **Location**: Control Panel (right side)
- **Function**: Toggle between 2D schematic and 3D model instantly

### 3D Camera Controls

When in 3D mode, you can interact with the view using your mouse:

#### Rotation
- **Action**: Left mouse button + drag
- **Effect**: Rotate the camera around the model
- **Use**: View the system from different angles

#### Pan
- **Action**: Right mouse button + drag
- **Effect**: Move the camera left/right/up/down
- **Use**: Focus on specific areas

#### Zoom
- **Action**: Mouse scroll wheel
- **Effect**: Move closer or farther from the model
- **Use**: Inspect details or see full system

### Camera Position

- **Default Position**: (0, 20, 40) looking at center
- **Default View**: Slight overhead angle for optimal viewing
- **Up Vector**: Y-axis (standard orientation)

## Technical Details

### Rendering

- **Framework**: Qt3D (OpenGL-based)
- **Shading**: Phong material model
- **Lighting**: 
  - Main point light at (0, 30, 20)
  - Fill light at (-20, 15, -10)
- **Anti-aliasing**: Enabled
- **Background**: Light blue (200, 220, 240)

### Performance

- **Target Frame Rate**: 30 FPS
- **Update Rate**: 33ms per frame
- **CPU Usage**: Similar to 2D mode (~5-15%)
- **GPU Usage**: Moderate (depends on graphics card)

### Materials

All components use Phong materials with:
- **Diffuse**: Component-specific colors
- **Ambient**: Slightly darker shade (120% darker)
- **Specular**: White highlights
- **Shininess**: 30-50 (varies by material)

## Animation Behavior

### Synchronized with Data Model

All 3D animations are synchronized with the system data model:

- **Pumps**: Only rotate when pump state is ON
- **Blowers**: Only rotate when blower state is ON
- **Valves**: State reflected (though rotation animation not yet implemented)
- **System State**: All animations stop when system is stopped

### Animation Speeds

- **Coolant Pumps**: 180° per second (30 RPM)
- **Blowers**: 360° per second (60 RPM)
- **Smooth Updates**: Frame-rate independent using delta time

## Building the Application

### Requirements

The 3D visualization requires additional Qt modules:

```bash
# Qt6 modules needed
- Qt6::3DCore
- Qt6::3DRender
- Qt6::3DInput
- Qt6::3DExtras
```

### CMake Build

The CMakeLists.txt has been updated to include Qt3D:

```bash
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets 3DCore 3DRender 3DInput 3DExtras)
```

Build as usual:
```bash
build.bat  # Windows
# or
cmake --build build  # Linux/Mac
```

### qmake Build

The .pro file has been updated:

```bash
QT += core gui widgets 3dcore 3drender 3dinput 3dextras
```

Build as usual:
```bash
build_qmake.bat  # Windows
# or
qmake && make  # Linux/Mac
```

## Troubleshooting

### 3D View Not Appearing

**Problem**: Click "Switch to 3D View" but nothing happens

**Solutions**:
1. Check that Qt3D modules are installed with your Qt distribution
2. Verify OpenGL drivers are up to date
3. Check console for Qt3D error messages

### Poor Performance in 3D Mode

**Problem**: Low frame rate or stuttering

**Solutions**:
1. Update graphics drivers
2. Reduce window size
3. Close other GPU-intensive applications
4. Switch back to 2D mode for better performance

### Black or White Screen

**Problem**: 3D view shows but only black or white

**Solutions**:
1. Verify OpenGL support (Qt3D requires OpenGL 3.0+)
2. Check graphics driver compatibility
3. Try updating Qt to latest version

### Components Not Visible

**Problem**: Some 3D components are missing

**Solutions**:
1. Use camera controls to navigate - components might be outside view
2. Reset zoom by scrolling out
3. Restart application

## Tips for Best Experience

1. **Start in 2D**: Get familiar with the system layout first
2. **Switch During Operation**: Try switching views while system is running to see animations
3. **Explore Angles**: Use rotation to find the best viewing angle
4. **Zoom In**: Inspect individual components up close
5. **Pan for Focus**: Use right-click drag to center specific areas

## Known Limitations

Current version limitations:

1. **Valve Animation**: Valves don't open/close visually (planned for future)
2. **Fluid Animation**: Pipe flow not animated in 3D (shown in 2D only)
3. **Text Labels**: No 3D labels for components (use 2D view for labels)
4. **Tank Level**: Tank liquid level not animated (planned for future)
5. **Lighting Controls**: Light positions are fixed (not adjustable)

## Future Enhancements

Planned improvements for 3D visualization:

1. **Animated Valves**: Visual opening/closing of valve handles
2. **Particle Effects**: Fluid flow particles in pipes
3. **Dynamic Tank Level**: Animated coolant level in tank
4. **Component Labels**: Floating 3D text labels
5. **Multiple Camera Presets**: Quick views (front, top, side, iso)
6. **Lighting Controls**: User-adjustable lighting
7. **Color Coding**: Temperature-based component coloring
8. **Transparency**: See-through components for internal views
9. **Exploded View**: Component separation for maintenance training
10. **VR Support**: Virtual reality headset compatibility

## File Structure

New files added for 3D support:

```
src/
├── lcuscene3d.h           # 3D scene class header
├── lcuscene3d.cpp         # 3D scene implementation
├── animationcontroller3d.h    # 3D animation controller header
└── animationcontroller3d.cpp  # 3D animation controller implementation
```

## API Reference

### LCUScene3D Class

Main 3D scene class that creates and manages all 3D entities.

**Constructor**:
```cpp
LCUScene3D(DataModel *dataModel, Qt3DCore::QEntity *parent = nullptr)
```

**Methods**:
```cpp
void setupCamera(Qt3DRender::QCamera *camera);  // Configure camera
void updateAnimations(double deltaTime);         // Update frame
```

### AnimationController3D Class

Manages animation updates for 3D scene.

**Constructor**:
```cpp
AnimationController3D(LCUScene3D *scene, DataModel *dataModel, QObject *parent = nullptr)
```

**Methods**:
```cpp
void start();               // Start animations
void stop();                // Stop animations
void setFrameRate(int fps); // Set target FPS (default 30)
```

## Coordinates System

The 3D scene uses a right-handed coordinate system:

- **X-axis**: Left (-) to Right (+)
- **Y-axis**: Down (-) to Up (+)
- **Z-axis**: Back (-) to Front (+)

**Component Layout**:
- Tank/Heater: X=-15, Y=0-10, Z=0
- Pumps: X=-15 to -9, Y=1.5, Z=-6
- Valves: X=-10 to 5, Y=2, Z=5
- Heat Exchangers: X=5, Y=3, Z=-5 to 7
- Condensers: X=15, Y=3, Z=-5 to 7
- Blowers: X=20, Y=3, Z=-5 to 7

## Additional Resources

- **Qt3D Documentation**: https://doc.qt.io/qt-6/qt3d-index.html
- **Main README**: See README.md for general usage
- **Project Summary**: See PROJECT_SUMMARY.md for implementation details
- **Integration Guide**: See INTEGRATION_GUIDE.md for external data

## Support

For issues or questions about 3D visualization:
1. Check this guide first
2. Review Qt3D documentation
3. Verify OpenGL compatibility
4. Check GitHub issues

---

**Document Version**: 1.0
**Last Updated**: February 13, 2026
**Compatible with**: LCU Simulator v1.0.0+
