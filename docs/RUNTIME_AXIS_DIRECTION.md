# Runtime Axis Direction Implementation

## Summary
This implementation adds the ability to change axis direction inversion at runtime via G-code, rather than requiring recompilation. The compile-time `INVERT_*_DIR` settings serve as defaults that can be overridden during operation.

## Files Modified

### 1. Configuration_adv.h
- Added `RUNTIME_AXIS_DIRECTION` configuration option
- Located after E_DUAL_STEPPER_DRIVERS section
- Includes comprehensive documentation about the feature

### 2. motion.h / motion.cpp
- Added `AxisFlags axis_inverted` static member to Motion class
- Added `reset_axis_direction()` function to initialize with compile-time defaults
- Initializes from INVERT_X_DIR, INVERT_Y_DIR, etc. defines

### 3. indirection.h
- Modified `INVERT_DIR` macro to check runtime flags when `RUNTIME_AXIS_DIRECTION` is enabled
- Falls back to compile-time defines when feature is disabled
- Added include for motion.h when feature is enabled

### 4. M670.cpp (new file)
- Created G-code command handler for axis direction control
- Supports setting individual axes: M670 X<0|1> Y<0|1> Z<0|1> etc.
- Supports reset to defaults: M670 R
- Reports current settings when called without parameters
- Works with all axes dynamically (X, Y, Z, I, J, K, U, V, W, E)

### 5. gcode.h
- Added M670 handler declarations
- Added M670 to command documentation
- Located after M666 (endstop adjustments)

### 6. gcode.cpp
- Added M670 case to command dispatcher
- Case 670 routes to M670() handler

### 7. settings.cpp
- Added axis_inverted to SettingsDataStruct
- Integrated EEPROM_WRITE in save section
- Integrated EEPROM_READ in load section
- Added reset_axis_direction() call in reset section
- Added M670_report() to settings report output

## Usage

### Enable the Feature
Uncomment in Configuration_adv.h:
```cpp
#define RUNTIME_AXIS_DIRECTION
```

### G-code Commands

**Report current settings:**
```gcode
M670
```
Output example:
```
M670 X S0 Y S1 Z S0
M670 E0 S0
M670 E1 S1
```

**Set axis directions:**
```gcode
M670 X S1      ; Invert X axis direction
M670 Y S0      ; Set Y to normal direction
M670 Z S1      ; Invert Z axis direction
M670 E0 S1     ; Invert extruder 0
M670 E1 S0     ; Set extruder 1 to normal direction
```

**Set secondary stepper relative directions:**
```gcode
M670 X2 S1     ; X2 motor runs opposite to X
M670 Y2 S0     ; Y2 motor runs same direction as X
M670 Z2 S1     ; Z2 motor runs opposite to Z
M670 Z3 S0     ; Z3 motor runs same direction as Z
M670 Z4 S1     ; Z4 motor runs opposite to Z
M670 H S1      ; E1 motor runs opposite to E0 (dual E steppers only)
```

**Set multiple axes at once:**
```gcode
M670 X S1 Y S0 Z S1     ; Set X inverted, Y normal, Z inverted
M670 X S0 Y S0 E0 S1    ; Set X and Y normal, E0 inverted
```

**Per-extruder control:**
Each extruder (E0, E1, E2, etc.) has its own independent direction flag. Specify the extruder number with the E parameter:
```gcode
M670 E0 S1     ; Invert E0 only
M670 E1 S0     ; Normal direction for E1
M670 E2 S1     ; Invert E2
```

**Secondary steppers (relative inversions):**
Secondary stepper motors have **relative** direction flags that determine if they run in the same or opposite direction as their primary axis:

- **X2, Y2** - For dual X/Y motor configurations (e.g., IDEX or ganged motors)
- **Z2, Z3, Z4** - For multi-Z motor configurations (triple/quad Z steppers)
- **H** - For dual E steppers (E1 relative to E0, when both motors are mechanically linked)

These flags are **relative** inversions. For example:
- If X is inverted and X2_vs_X is normal (S0): Both X and X2 run inverted
- If X is inverted and X2_vs_X is inverted (S1): X runs inverted, X2 runs normal
- If X is normal and X2_vs_X is inverted (S1): X runs normal, X2 runs inverted

This allows you to:
1. Change the absolute direction of an axis (affects all motors)
2. Change the relative direction between primary and secondary motors

**Reset to defaults:**
```gcode
M670 R         ; Reset all axes to compile-time defaults
```

**Save to EEPROM:**
```gcode
M500           ; Save all settings including axis directions
```

**Load from EEPROM:**
```gcode
M501           ; Load all settings including axis directions
```

## Features

1. **Dynamic Axes Support**: Automatically handles the number of axes configured (X, Y, Z, I, J, K, U, V, W)

2. **Per-Extruder Control**: Each extruder (E0-E7) has independent direction control matching the compile-time INVERT_E0_DIR through INVERT_E7_DIR defines

3. **EEPROM Integration**: Settings are saved/loaded with M500/M501

4. **Default Initialization**: Uses compile-time INVERT_*_DIR defines as defaults

5. **Settings Report**: M503 includes current axis direction settings

6. **Zero Runtime Overhead When Disabled**: When feature is not enabled, the original compile-time macro is used

## Technical Details

- Uses `AxisFlags` structure for efficient boolean storage for main axes (X, Y, Z, I, J, K, U, V, W)
- Uses `bool extruder_inverted[E_STEPPERS]` array for per-extruder direction flags
- Uses individual bool flags for secondary stepper relative inversions (x2_vs_x_inverted, y2_vs_y_inverted, z2_vs_z_inverted, z3_vs_z_inverted, z4_vs_z_inverted, e1_vs_e0_inverted)
- Macro check happens at compile time to select runtime vs compile-time path
- All axis direction changes require no recompilation or firmware reflash
- Compatible with all Marlin motion systems (Cartesian, Delta, CoreXY, SCARA, etc.)
- Secondary steppers use relative inversion flags that work in combination with primary axis direction:
  - **Final direction = Primary axis direction XOR Secondary relative inversion**
  - Example: If X is inverted (1) and X2_vs_X is inverted (1), X2 runs normal (1 XOR 1 = 0)
  - Example: If X is normal (0) and X2_vs_X is inverted (1), X2 runs inverted (0 XOR 1 = 1)

## Use Cases

1. **Hardware Testing**: Quickly test motor wiring without recompiling
2. **Machine Variants**: Support different hardware configurations with same firmware
3. **Field Service**: Change motor directions without access to development tools
4. **Prototyping**: Rapid iteration on new machine designs
5. **Machine Migration**: Reuse firmware across different printer builds

## Backward Compatibility

When `RUNTIME_AXIS_DIRECTION` is not defined:
- Zero impact on code size
- Zero impact on runtime performance
- Original compile-time behavior preserved
- No changes to existing configurations

## Memory Impact

When enabled:
- Adds 1 byte (AxisFlags) for main axes to Motion class
- Adds E_STEPPERS bytes (1 byte per extruder) for extruder directions to Motion class
- Adds up to 6 additional bytes for secondary stepper relative inversions (X2, Y2, Z2, Z3, Z4, E1vsE0)
- Adds same amounts to EEPROM settings
- Adds ~700 bytes for M670 handler code

Example memory usage:
- Basic (3 axes, 1 extruder, no secondary steppers): 2 bytes RAM + 2 bytes EEPROM
- Standard (3 axes, 2 extruders, no secondary steppers): 3 bytes RAM + 3 bytes EEPROM
- Advanced (3 axes, 2 extruders, triple Z): 5 bytes RAM + 5 bytes EEPROM (adds Z2, Z3 flags)
- Complex (3 axes, 4 extruders, quad Z, dual X): 8 bytes RAM + 8 bytes EEPROM
