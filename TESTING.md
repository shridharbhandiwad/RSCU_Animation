# Testing Guide for Liquid Cooling Unit Simulator

## Prerequisites

Before testing, ensure you have:
- Built the project successfully (see README.md)
- Qt6 installed and configured
- Windows environment (for full testing)

## Manual Testing Procedures

### 1. Application Startup Test

**Steps:**
1. Run `LiquidCoolingUnit.exe`
2. Verify main window opens with dimensions ~1400x900
3. Check that all UI elements are visible:
   - Graphics view showing LCU system diagram
   - Control panel on the right
   - Sensor data panel on the right
   - Menu bar (File, View)
   - Status bar at bottom

**Expected Result:**
- Application starts without errors
- All components are visible in their default (stopped) state
- Status bar shows "Ready"

### 2. System Startup Test

**Steps:**
1. Click "Start System" button
2. Observe the system response

**Expected Results:**
- Pump 1 starts rotating (green indicator, spinning impeller)
- Heater activates (glowing effect)
- Channels 1 and 2 valves open (green indicator)
- Refrigerant loop 1 activates:
  - Compressor starts (status shows "Running")
  - Solenoid valve opens (energized, yellow coil)
  - Blower starts rotating
- Pipes show flowing animation with moving dashes
- Flow direction arrows appear in active pipes
- Status bar shows "System Running"

### 3. Sensor Data Verification

**Steps:**
1. With system running, observe the Sensor Data panel
2. Monitor values for 30 seconds

**Expected Results:**
- Supply Temp: ~20-23°C (oscillating)
- System Pressure: ~2.2-2.8 Bar (oscillating)
- Return Temp: ~25-30°C (oscillating)
- Return Pressure: ~1.8-2.2 Bar
- Flow Rate: ~50 lpm (with 1 pump running)
- All values update smoothly every 100ms

### 4. Component Animation Test

**Component** | **Animation** | **Verification**
---|---|---
Pump | Rotating impeller blades | Visual rotation, smooth and continuous
Blower | Rotating fan blades | Faster rotation than pump
Heater | Pulsing glow effect | Orange/red glow oscillates
Valve | Open/close movement | Position changes smoothly
Solenoid Valve | Energized pulse | Yellow coil pulses when open
Pipe | Flow animation | Dashed lines move in correct direction
Tank | Liquid level | Blue liquid at correct level
Condenser | Color change | Color indicates active state
Heat Exchanger | Color change | Hot/cold sides show different colors

### 5. Multi-System Operation Test

**Steps:**
1. Start system (already tested above)
2. Observe 1 pump, 2 channels, 1 refrigerant loop active
3. Monitor for 1 minute

**Expected Results:**
- System operates stably
- Animations are smooth (30 FPS target)
- Temperature values oscillate realistically
- Channel flow indicators show proper distribution
- No crashes or freezes

### 6. System Shutdown Test

**Steps:**
1. Click "Stop System" button
2. Observe the shutdown sequence

**Expected Results:**
- All pumps stop rotating immediately
- Valves close (indicators turn red)
- Heater deactivates (glow stops)
- All pipe flows stop
- Refrigerant loops shut down
- Status bar shows "System Stopped"
- All sensor values stabilize to ambient conditions

### 7. View Controls Test

**Steps:**
1. Menu → View → Zoom In (multiple times)
2. Menu → View → Zoom Out
3. Menu → View → Reset Zoom

**Expected Results:**
- Zoom in: Scene enlarges smoothly
- Zoom out: Scene reduces smoothly
- Reset: Scene returns to default 1:1 zoom

### 8. Cooling Capacity Adjustment

**Steps:**
1. Start system
2. Adjust "Cooling Capacity" slider to different values (0, 30, 60, 100 kW)
3. Observe system response

**Expected Results:**
- Heater power adjusts proportionally
- Temperature readings may change
- System remains stable at all capacity levels

## Automated Test Scenarios

### Test Scenario 1: Normal Operation (test_data.json)

**Configuration:**
- 1 pump running
- 2 channels open (CH1, CH2)
- 1 refrigerant loop active
- 30 kW cooling capacity

**Duration:** 60 seconds

**Pass Criteria:**
- Supply temp: 20-25°C
- Return temp: 26-30°C
- System pressure: 2.0-3.0 Bar
- Flow rate: ~100 lpm
- No component failures
- Smooth animations

### Test Scenario 2: High Load Operation

**Configuration:**
- 2 pumps running
- 4 channels open (all)
- 3 refrigerant loops active
- 65 kW cooling capacity

**Duration:** 120 seconds

**Pass Criteria:**
- Supply temp: 18-22°C
- Return temp: 30-35°C
- System pressure: 2.8-3.5 Bar
- Flow rate: ~150 lpm
- All blowers at high speed
- All components animated correctly

### Test Scenario 3: Startup Sequence

**Configuration:**
- Gradual system activation
- 1 pump → 1 channel → 1 loop

**Duration:** 30 seconds

**Pass Criteria:**
- Components activate in sequence
- No sudden jumps in values
- Smooth transitions
- Proper startup animations

### Test Scenario 4: Shutdown

**Configuration:**
- Running system → complete stop

**Duration:** 15 seconds

**Pass Criteria:**
- Clean shutdown sequence
- All animations stop properly
- Values return to ambient
- No residual flows

## Performance Testing

### Frame Rate Test

**Steps:**
1. Start system with maximum load
2. Observe animation smoothness
3. Monitor CPU usage (Task Manager)

**Expected Results:**
- Animations at ~30 FPS (smooth)
- CPU usage < 10% (on modern hardware)
- No stuttering or frame drops

### Memory Test

**Steps:**
1. Start application
2. Note initial memory usage
3. Run system for 10 minutes
4. Check memory usage

**Expected Results:**
- Initial memory: < 100 MB
- After 10 minutes: No significant increase
- No memory leaks

## Data Model API Testing

### Manual API Test

Create a test function to verify the data model API:

```cpp
// Test code (add to main.cpp temporarily)
void testDataModel() {
    DataModel model;
    
    // Test system state
    model.setSystemRunning(true);
    assert(model.isSystemRunning() == true);
    
    // Test coolant parameters
    model.setSupplyTemp(22.5);
    assert(model.getSupplyTemp() == 22.5);
    
    model.setFlowRate(100.0);
    assert(model.getFlowRate() == 100.0);
    
    // Test channel control
    model.setChannelState(0, true);
    assert(model.getChannelState(0) == true);
    
    model.setChannelFlowRate(0, 50.0);
    assert(model.getChannelFlowRate(0) == 50.0);
    
    // Test pump control
    model.setPumpState(0, true);
    assert(model.getPumpState(0) == true);
    
    // Test refrigerant system
    model.setCompressorState(0, true);
    assert(model.getCompressorState(0) == true);
    
    model.setBlowerState(0, true);
    assert(model.getBlowerState(0) == true);
    
    qDebug() << "All DataModel API tests passed!";
}
```

## Known Issues and Limitations

### Current Version Limitations:
1. No external data input (uses built-in simulation)
2. No data logging to file
3. No alarm/trip visualization (UI ready, logic minimal)
4. Animation frame rate not configurable

### Future Enhancements:
1. TCP/IP or serial communication for external data
2. Historical data plotting
3. Configurable alarm thresholds
4. Multiple LCU units in single view
5. Export system state to JSON

## Troubleshooting

### Issue: Application doesn't start
- **Solution:** Verify Qt DLLs are accessible (add Qt bin directory to PATH)

### Issue: Animations are choppy
- **Solution:** Check CPU load, close other applications, verify graphics drivers

### Issue: Components not visible
- **Solution:** Try View → Reset Zoom, maximize window

### Issue: Sensor values don't update
- **Solution:** Verify system is running (click Start System)

### Issue: No flow animation in pipes
- **Solution:** Ensure pumps are running and channels are open

## Test Results Template

```
Test Date: _______________
Tester: _______________
Build Version: _______________

| Test Case | Status | Notes |
|-----------|--------|-------|
| Application Startup | PASS/FAIL | |
| System Startup | PASS/FAIL | |
| Sensor Data | PASS/FAIL | |
| Component Animations | PASS/FAIL | |
| Multi-System Operation | PASS/FAIL | |
| System Shutdown | PASS/FAIL | |
| View Controls | PASS/FAIL | |
| Cooling Capacity | PASS/FAIL | |
| Performance | PASS/FAIL | |

Overall Result: PASS / FAIL
Additional Comments:
_________________________________
_________________________________
```

## Reporting Issues

When reporting issues, include:
1. Windows version
2. Qt version
3. Compiler used (MSVC/MinGW)
4. Steps to reproduce
5. Expected vs actual behavior
6. Screenshots/video if applicable
7. Console output (if any errors)

## Success Criteria

The application passes testing if:
1. ✅ All manual tests pass
2. ✅ All automated scenarios run without crashes
3. ✅ Animations are smooth (≥25 FPS)
4. ✅ No memory leaks over 10 minutes
5. ✅ All sensor values are realistic and update correctly
6. ✅ All components respond to data model changes
7. ✅ UI remains responsive during operation
8. ✅ System can start/stop cleanly multiple times
