# Runtime Homing Direction Implementation

## Overview

The `RUNTIME_HOMING_DIRECTION` feature allows changing the homing direction of axes at runtime via G-code commands. Normally, homing direction is set at compile time via `X_HOME_DIR`, `Y_HOME_DIR`, `Z_HOME_DIR`, etc. in `Configuration.h`. With this feature enabled, these can be changed dynamically and saved to EEPROM.

## Configuration

Enable in `Configuration_adv.h`:

```cpp
#define RUNTIME_HOMING_DIRECTION
```

## G-code Command: M671

### Syntax

```gcode
M671 [X<dir>] [Y<dir>] [Z<dir>] [I<dir>] [J<dir>] [K<dir>] [U<dir>] [V<dir>] [W<dir>] [R]
```

### Parameters

- **Axis letters (X, Y, Z, I, J, K, U, V, W)**: Specify which axis to configure, followed by `S<dir>` where:
  - `S-1` = Home to MIN endstop (negative direction)
  - `S1` = Home to MAX endstop (positive direction)
- **R**: Reset all axes to compile-time defaults
- **No parameters**: Report current homing direction settings

**Note:** This feature does NOT apply to extruder axes (E). Extruders do not home.

### Examples

**Set homing directions:**

```gcode
M671 X S-1      ; X homes to MIN endstop
M671 Y S-1      ; Y homes to MIN endstop
M671 Z S1       ; Z homes to MAX endstop (uncommon, for printers with top Z endstop)
```

**Set multiple axes at once:**

```gcode
M671 X S-1 Y S-1 Z S-1    ; All axes home to MIN
```

**Reset to defaults:**

```gcode
M671 R          ; Reset all axes to compile-time X_HOME_DIR, Y_HOME_DIR, Z_HOME_DIR values
```

**Report current settings:**

```gcode
M671            ; Display current homing directions
```

Example output:

```
Homing Direction:
  M671 X S-1 Y S-1 Z S-1 ; -1=MIN, 1=MAX
```

## Persistence

Homing direction settings can be saved to EEPROM:

```gcode
M671 X S1 Y S-1 Z S-1    ; Set homing directions
M500                      ; Save to EEPROM
```

To restore from EEPROM after power cycle:

```gcode
M501                      ; Load settings from EEPROM
```

To reset all settings (including homing direction) to defaults:

```gcode
M502                      ; Reset all settings to firmware defaults
M500                      ; Save defaults to EEPROM
```

## Use Cases

### 1. Testing Endstop Configurations

When building or troubleshooting a printer, you can test different homing directions without recompiling firmware:

```gcode
M671 X S1       ; Try homing X to MAX
G28 X           ; Test X homing
M671 X S-1      ; Switch back to MIN
G28 X           ; Test again
```

### 2. Switchable Printer Configurations

For machines that can physically swap endstop locations or operate in different modes:

```gcode
; Configuration A: Standard MIN homing
M671 X S-1 Y S-1 Z S-1
M500

; Configuration B: MAX homing on Z (e.g., for top-mounted Z probe)
M671 Z S1
M500
```

### 3. Delta Printers

Some delta printers home to MAX endstops at the top of the towers:

```gcode
M671 X S1 Y S1 Z S1    ; Home all towers to MAX
```

### 4. Safety Stops

If your printer has endstops on both ends of an axis (defined by `X_SAFETY_STOP`, etc.), you can switch which end is used for homing:

```gcode
M671 X S-1      ; Home to MIN end
; or
M671 X S1       ; Home to MAX end (safety stop)
```

## Memory Impact

When enabled:

- Adds LOGICAL_AXES bytes (typically 3-9 bytes) for axis_home_dir array to Motion class
- Adds same to EEPROM settings
- Adds ~1KB for M671 handler code

Example memory usage:

- Standard 3-axis (X, Y, Z): 3 bytes RAM + 3 bytes EEPROM
- 5-axis (X, Y, Z, A, B): 5 bytes RAM + 5 bytes EEPROM
- 9-axis (X, Y, Z, I, J, K, U, V, W): 9 bytes RAM + 9 bytes EEPROM

## Technical Details

- Uses `int8_t axis_home_dir[LOGICAL_AXES]` array for runtime storage
- The `home_dir(axis)` function becomes a runtime lookup instead of compile-time macro
- All homing logic in G28 and related code automatically uses the runtime values
- Compatible with all Marlin kinematics (Cartesian, Delta, CoreXY, SCARA, Polargraph, etc.)
- Does not affect `tool_x_home_dir()` for DUAL_X_CARRIAGE (that remains controlled by active tool)

## Interaction with Other Features

### DELTA Printers

**Important limitation**: On delta printers, Z homing direction cannot be changed from the default (MAX/top). Delta kinematics require all three towers to home to the top endstops.

If you attempt to change Z direction on a delta printer:

```gcode
M671 Z S-1     ; ERROR: "Delta Z must home to MAX (use S1 only)"
M671 Z S1      ; OK (but unnecessary - already the default)
```

X and Y axes on delta printers (tower A, B, C) also cannot change homing direction. The M671 command on delta should generally not be used.

### DUAL_X_CARRIAGE

The `tool_x_home_dir()` function still controls homing direction based on active tool. The M671 setting for X axis is separate and does not override tool-specific homing.

### Z_SAFE_HOMING

Works normally. The Z_HOME_DIR (or runtime axis_home_dir[Z_AXIS]) still controls which direction Z moves during homing.

### Bed Probing

Z homing direction affects probe-based homing. If you change Z_HOME_DIR at runtime, ensure your probe configuration is compatible with the new direction.

## Runtime Endstop Pin Swapping

When both MIN and MAX endstop pins are defined for an axis (e.g., both `X_MIN_PIN` and `X_MAX_PIN` exist on the board), the RUNTIME_HOMING_DIRECTION feature automatically handles physical pin swapping:

### How It Works

1. **Compile-time storage**: Physical pin numbers for both MIN and MAX endstops are stored as `constexpr` values:

   ```cpp
   constexpr pin_t X_MIN_ENDSTOP_PIN = X_MIN_PIN;
   constexpr pin_t X_MAX_ENDSTOP_PIN = X_MAX_PIN;
   ```

2. **Runtime accessor functions**: FORCE_INLINE accessor functions return the appropriate pin based on current homing direction:

   ```cpp
   pin_t X_HOME_ENDSTOP_PIN() {
     return (motion.home_dir(X_AXIS) < 0) ? X_MIN_ENDSTOP_PIN : X_MAX_ENDSTOP_PIN;
   }
   ```

3. **Automatic swapping**: When you change homing direction via M671, the firmware automatically reads from the correct physical pin:
   ```gcode
   M671 X S-1    ; Firmware now reads X_MIN_PIN during X homing
   M671 X S1     ; Firmware now reads X_MAX_PIN during X homing
   ```

### Board Compatibility

**Pin swapping works when:**

- Both MIN and MAX pins are defined for an axis (e.g., `#define X_MIN_PIN 3` and `#define X_MAX_PIN 2`)
- Common on modern boards that support safety stops or reversible configurations

**Pin swapping is NOT needed when:**

- Only one endstop pin is defined (e.g., only `X_MIN_PIN` or only `X_MAX_PIN`)
- The accessor falls back to the single available pin
- Common on budget boards or single-endstop configurations

### Example: Reversible Printer

For a printer with endstops at both ends of the X axis:

```gcode
; Initial configuration - home to left side
M671 X S-1
M500           ; Save to EEPROM
G28 X          ; Homes to X_MIN_PIN (left endstop)

; Reconfigure - home to right side
M671 X S1
M500           ; Save to EEPROM
G28 X          ; Homes to X_MAX_PIN (right endstop)
```

The physical wiring doesn't change - only which pin is read during homing.

### Safety Stops vs Home Endstops

**Important distinction:**

- **Home endstop**: The endstop used during G28 homing. This is the one affected by M671 and runtime pin swapping.
- **Safety stop**: Additional endstops monitored during normal movement to prevent crashes. These always monitor their physical pins (e.g., X_MAX as safety stop is always read from X_MAX_PIN).

If both `X_MIN_PIN` and `X_MAX_PIN` are defined:

- Runtime direction controls which is the "home" endstop (used during G28)
- Both pins are still monitored during moves for safety (if safety stops are enabled)

### Implementation Files

The runtime pin swapping is implemented in:

- **Header**: `/Marlin/src/pins/runtime_endstop_pins.h`
  - Defines `HAS_*_RUNTIME_ENDSTOP_SWAP` flags when both MIN and MAX pins exist
  - Provides `*_HOME_ENDSTOP_PIN()` accessor functions
  - Provides `READ_*_HOME_ENDSTOP()` and `*_HOME_ENDSTOP_HIT_STATE()` macros
- **Integration**: `/Marlin/src/module/endstops.cpp`
  - Includes runtime_endstop_pins.h when RUNTIME_HOMING_DIRECTION enabled
  - Endstop checking uses runtime accessors where appropriate

### Sensorless Homing

Works with TMC stallGuard-based sensorless homing. The runtime direction changes which direction triggers the stall detection.

## Safety Considerations

1. **Verify endstop configuration**: Before changing homing direction, ensure endstops exist at the target end of travel
2. **Test cautiously**: After changing direction, use `G28` with individual axes first (e.g., `G28 X`) before homing all axes
3. **Check MIN/MAX positions**: Changing homing direction may affect coordinate system; verify your MIN_POS and MAX_POS settings are correct
4. **Save after testing**: Only save to EEPROM (`M500`) after verifying the new direction works correctly
5. **Multi-endstop configurations**: If using dual X/Y/Z steppers with separate endstops, the hardware endstop pins are configured at compile-time based on the default `X_HOME_DIR`, `Y_HOME_DIR`, `Z_HOME_DIR` settings. Runtime direction changes work best with single-endstop configurations or when using Safety Stops (endstops at both ends).

## Limitations

### Compile-Time vs Runtime Behavior

The `X_HOME_TO_MIN`/`X_HOME_TO_MAX` flags (and equivalent for Y, Z, etc.) are compile-time defines based on `X_HOME_DIR`, `Y_HOME_DIR`, etc. These flags are used throughout the codebase for:

1. **Hardware pin configuration** (pins_postprocess.h) - Determines which physical pins to use for endstops
2. **Sanity checks** (SanityCheck.h) - Validates hardware configuration at compile time
3. **Endstop enumeration** (endstops.h) - Compile-time mapping of logical to physical endstops
4. **Multi-endstop stepper ISR** (stepper.cpp) - Hardware-specific endstop pin selection

With `RUNTIME_HOMING_DIRECTION` enabled:

- These flags still exist and reflect **compile-time defaults** (needed for pin config, sanity checks, etc.)
- The actual **runtime homing logic** uses `motion.home_dir(axis)` to respect M671 changes
- Runtime code (like G28 homing and parking extruder logic) dynamically checks `motion.home_dir(axis)`

### Multi-Endstop Configurations

When using dual/triple/quad stepper configurations with separate endstops for each motor (e.g., `X_DUAL_ENDSTOPS`, `TRIPLE_Z_ENDSTOPS`), the stepper ISR code uses compile-time `X_HOME_TO_MIN`/`X_HOME_TO_MAX` flags to determine which physical endstop pins to monitor. Changing homing direction at runtime does not reconfigure these hardware pin assignments.

**Recommendation**: For multi-endstop setups, set the desired homing direction at compile-time in `Configuration.h`. Use runtime changes only for testing or single-endstop configurations.

### Safety Stops

If your printer has endstops at **both ends** of an axis (configured with `X_SAFETY_STOP`, `Y_SAFETY_STOP`, `Z_SAFETY_STOP`), runtime homing direction changes work as expected, as both endstops are always monitored.

## Troubleshooting

**Printer homes in wrong direction:**

- Check current setting with `M671`
- Verify endstop is present at target end
- Try the opposite direction: `M671 X S-1` vs `M671 X S1`

**Changes don't persist:**

- Ensure you ran `M500` to save to EEPROM
- Check that EEPROM is not corrupted (try `M502` then `M500`)

**Homing still uses compile-time direction:**

- Verify `RUNTIME_HOMING_DIRECTION` is defined in `Configuration_adv.h`
- Rebuild firmware completely (`platformio run --target clean` then `platformio run`)

## Code Implementation

### Files Modified

- `Configuration_adv.h` - Feature enable flag
- `module/motion.h` - Runtime storage and function declaration
- `module/motion.cpp` - Storage and reset function implementation
- `gcode/config/M671.cpp` - G-code handler (new file)
- `gcode/gcode.h` - M671 declaration
- `gcode/gcode.cpp` - M671 case registration
- `module/settings.cpp` - EEPROM save/load/reset integration
- `ini/features.ini` - Conditional compilation entry

### Key Functions

- `motion.reset_axis_home_dir()` - Initialize from compile-time defaults
- `motion.home_dir(axis)` - Runtime lookup instead of XYZ_DEFS macro
- `M671()` - G-code command handler
- `M671_report()` - Settings report for M503

## Related Features

- **RUNTIME_AXIS_DIRECTION** - Runtime control of motor direction (separate from homing direction)
- **X_SAFETY_STOP / Y_SAFETY_STOP / Z_SAFETY_STOP** - Endstops at opposite end from homing direction
- **SENSORLESS_HOMING** - TMC stallGuard-based homing (works with runtime direction)
- **Z_SAFE_HOMING** - Move to safe XY position before homing Z (uses runtime Z direction)

## License

This feature is part of Marlin Firmware and licensed under GPLv3.0.
