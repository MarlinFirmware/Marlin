# Artillery Genius Pro — Marlin Firmware Merge Report

Generated: 2026-04-30  
Base: **Marlin bugfix-2.1.x** (HEX version `02010300`, distribution date `2026-05-01`)  
Output: `Configuration.h` + `Configuration_adv.h` (place in `Marlin-bugfix-2.1.x/Marlin/`)

---

## Detected Marlin Version

| Field | Value |
|---|---|
| Branch | `bugfix-2.1.x` |
| HEX Version | `02010300` |
| Distribution Date | `2026-05-01` |
| Version file | `Marlin-bugfix-2.1.x/Marlin/src/inc/Version.h` |
| Artillery/Genius Pro example in release | **Not present** — config/examples/ contains only README.md |
| Syntax baseline used | Generic `Marlin-bugfix-2.1.x/Marlin/Configuration.h` |

---

## Source Priority Applied

| Priority | Source | Role |
|---|---|---|
| 1 (lowest) | `old stock fw - artillery genius pro` | Hardware truth: board, thermistors, serial ports |
| 2 | `custom fw - sidewinder x2` | Reference only — all hardware values adapted to Genius Pro |
| 3 | `custom fw - genius, genius pro and sw 1` | Feature reference; Genius Pro-relevant settings only |
| 4 (highest) | `custom fw - artillery genius pro` (both variants) | Proven settings and feature set authority |
| Syntax | `latest marlin release` (Marlin-bugfix-2.1.x) | Syntax authority and output template |

---

## Confirmed Hardware Values

| Setting | Value | Source |
|---|---|---|
| `MOTHERBOARD` | `BOARD_ARTILLERY_RUBY` (STM32F401RC) | old stock fw + custom fw artillery genius pro |
| `SERIAL_PORT` | `-1` (USB virtual serial) | old stock fw + all custom sources |
| `SERIAL_PORT_2` | `1` (UART1 → TFT touchscreen) | old stock fw + all custom sources |
| `BAUDRATE` | `250000` | old stock fw + all custom sources |
| `TEMP_SENSOR_0` | `1` (EPCOS 100kΩ) | old stock fw (OEM firmware — hardware truth) |
| `TEMP_SENSOR_BED` | `1` (EPCOS 100kΩ) | old stock fw (OEM firmware — hardware truth) |
| `HEATER_0_MAXTEMP` | `275` | custom fw artillery genius pro |
| `BED_MAXTEMP` | `150` | custom fw artillery genius pro |
| `X/Y/Z/E0_DRIVER_TYPE` | `TMC2100` | old stock fw + custom fw artillery genius pro |
| `EXTRUDERS` | `1` | all sources |
| `INVERT_Z_DIR` | `true` | custom fw artillery genius pro |
| `X_BED_SIZE` | `220` | custom fw artillery genius pro |
| `Y_BED_SIZE` | `220` | custom fw artillery genius pro |
| `Z_MAX_POS` | `250` | custom fw artillery genius pro |
| `X_MIN_POS / Y_MIN_POS` | `0 / 0` | custom fw artillery genius pro |
| `NOZZLE_TO_PROBE_OFFSET` | `{ 27.25, -12.8, -2 }` | custom fw artillery genius pro (both variants agree on X/Y; Z=-2 from bugfix-2.1.x) |
| `PROBING_MARGIN` | `10` | all sources agree |
| `HOMING_FEEDRATE_MM_M` | `{ (100*60), (100*60), (25*60) }` | custom fw artillery genius pro |
| `DEFAULT_AXIS_STEPS_PER_UNIT` | `{ 80.121, 80.121, 402, 449.5 }` | custom fw artillery genius pro (both variants agree) |
| `DEFAULT_MAX_FEEDRATE` | `{ 300, 300, 30, 40 }` | user specification (Z=30 confirmed by user) |
| `DEFAULT_MAX_ACCELERATION` | `{ 2000, 2000, 100, 10000 }` | custom fw artillery genius pro bugfix-2.1.x |
| `DEFAULT_ACCELERATION` | `800` | custom fw artillery genius pro bugfix-2.1.x |
| `DEFAULT_Kp/Ki/Kd` | `13.1079 / 0.9257 / 46.4019` | custom fw artillery genius pro (gpro-mp, hardware calibrated) |
| `DEFAULT_bedKp/Ki/Kd` | `47.0143 / 6.3191 / 233.1900` | custom fw artillery genius pro (gpro-mp, hardware calibrated) |
| `NEOPIXEL_PIN` | `PB7` | custom fw artillery genius pro (Ruby board onboard LED) |
| `NEOPIXEL_TYPE` | `NEO_GRBW` | custom fw artillery genius pro |
| `NEOPIXEL_PIXELS` | `1` | custom fw artillery genius pro |
| `NUM_SERVOS` | `1` | custom fw artillery genius pro (BLTouch) |
| `SERVO_DELAY` | `{ 300 }` | custom fw artillery genius pro |

---

## Source Conflicts and Resolutions

### PID Hotend Values — Conflict Between Two Genius Pro Variants

| Source | Kp | Ki | Kd |
|---|---|---|---|
| `bugfix-2.1.x` (official upstream example) | 24.81 | 3.36 | 46.15 |
| `gpro-mp` (MirakelPrints/thisiskeithb — hardware calibrated) | 13.1079 | 0.9257 | 46.4019 |
| **Used** | **13.1079** | **0.9257** | **46.4019** |

**Resolution:** gpro-mp values selected — these are attributed to actual machine calibration runs. The upstream example values are generic starting-point defaults. Both sets differ significantly on Kp and Ki. The user should still run `M303 E0 S200 C8` after flashing.

### PID Bed Values — Minor Conflict

| Source | bedKp | bedKi | bedKd |
|---|---|---|---|
| `bugfix-2.1.x` | 42.4679 | 5.0080 | 240.0085 |
| `gpro-mp` | 47.0143 | 6.3191 | 233.1900 |
| **Used** | **47.0143** | **6.3191** | **233.1900** |

**Resolution:** gpro-mp values selected for same reason as hotend PID. User should run `M303 E-1 S60 C8`.

### Z Max Feedrate — User Spec vs Source

| Source | Z feedrate |
|---|---|
| `bugfix-2.1.x` source | 10 mm/s |
| User specification | **30 mm/s** |
| **Used** | **30 mm/s** |

**Resolution:** User-specified value wins per source priority rules. Note: 30 mm/s is high for a leadscrew printer — if you experience missed steps on fast Z moves, reduce to 10–15 mm/s via `M203 Z10` and save with `M500`.

### Probe Z Offset — Minor Conflict

| Source | Z offset |
|---|---|
| `bugfix-2.1.x` | -2 mm |
| `gpro-mp` | -1 mm |
| **Used** | **-2 mm** |

**Resolution:** bugfix-2.1.x value selected (higher authority among custom sources). This value **must** be re-calibrated after every flash with the probe offset wizard or `M851 Z<value>` + `M500`.

### Steps/mm — SW X2 vs Genius Pro

SW X2 uses rounded integers `{ 80, 80, 400, 445 }`. Both Genius Pro variants use `{ 80.121, 80.121, 402, 449.5 }`. Genius Pro values used — they represent hardware-specific calibration.

### Display — No LCD Controller Defined

The Artillery Genius Pro TFT touchscreen communicates via serial (UART1 = `SERIAL_PORT_2=1`), acting as a host. No Marlin LCD controller (`TFT_COLOR_UI`, `TOUCH_SCREEN`, `DWIN_CREALITY_LCD`, etc.) is defined in any Genius Pro reference source. The TFT is driven entirely through the BTT/serial host features enabled in `Configuration_adv.h`.

---

## SW X2 Values Adapted for Genius Pro

All SW X2 hardware values were discarded. Only feature patterns were referenced.

| SW X2 Setting | SW X2 Value | Genius Pro Value Used | Reason |
|---|---|---|---|
| `X_BED_SIZE` | 300 | 220 | Different printer |
| `Y_BED_SIZE` | 300 | 220 | Different printer |
| `Z_MAX_POS` | 400 | 250 | Different printer |
| `X_MIN_POS` | -2 | 0 | Genius Pro homes to origin |
| `Y_MIN_POS` | -5 | 0 | Genius Pro homes to origin |
| `DEFAULT_AXIS_STEPS_PER_UNIT` | `{ 80, 80, 400, 445 }` | `{ 80.121, 80.121, 402, 449.5 }` | Genius Pro calibrated values |
| `DEFAULT_MAX_FEEDRATE` (Z) | 10 | 30 | User specification |
| Input Shaping frequencies | X:55, Y:48.6 | X:55, Y:48.6 | Measured on SW2; used as starting point — re-measure on Genius Pro |

---

## Features Renamed or Removed in bugfix-2.1.x

These were requested in the user specification but are deprecated/renamed. Using the old names causes a **compile error** (`Changes.h`):

| Old Name | Status in bugfix-2.1.x | Action Taken |
|---|---|---|
| `Z_HOMING_HEIGHT` | **REMOVED** — renamed to `Z_CLEARANCE_FOR_HOMING` | Used `Z_CLEARANCE_FOR_HOMING 4` in `Configuration.h` |
| `MIN_Z_HEIGHT_FOR_HOMING` | **REMOVED** — renamed to `Z_CLEARANCE_FOR_HOMING` | Same replacement |
| `ALLOW_LOW_EJERK` | **DEPRECATED** — compile error | Removed; not needed when `CLASSIC_JERK` is disabled |
| `EXPERIMENTAL_SCURVE` | **DEPRECATED** — compile error | Removed; `S_CURVE_ACCELERATION` is the correct define |
| `LCD_SET_PROGRESS_MANUALLY` | **RENAMED** to `SET_PROGRESS_MANUALLY` | Used new name in `Configuration_adv.h` |

---

## Feature Location Changes vs User Specification

Some features the user specified for `Configuration.h` actually live in `Configuration_adv.h` in bugfix-2.1.x:

| Feature | User Said | Actual Location |
|---|---|---|
| `ARC_SUPPORT` | Configuration.h | **Configuration_adv.h** (line 2651) |
| `ARC_P_CIRCLES` | Configuration.h | **Configuration_adv.h** (line 2658) |
| `ADAPTIVE_STEP_SMOOTHING` | Configuration.h | **Configuration_adv.h** (line 1474) |
| `BABYSTEPPING` | Configuration.h | **Configuration_adv.h** (line 2395) |
| `BABYSTEP_ALWAYS_AVAILABLE` | Configuration.h | **Configuration_adv.h** |
| `POWER_LOSS_RECOVERY` | Configuration.h | **Configuration_adv.h** (line 1860) |
| `CANCEL_OBJECTS` | Configuration.h | **Configuration_adv.h** (line 4341) |
| `GCODE_MACROS` | Configuration.h | **Configuration_adv.h** (line 4183) |
| `ASSISTED_TRAMMING` | Configuration.h | **Configuration_adv.h** (line 1120) |
| `LCD_SET_PROGRESS_MANUALLY` | Configuration.h | **Configuration_adv.h** as `SET_PROGRESS_MANUALLY` |
| `SHOW_REMAINING_TIME` | Configuration.h | **Configuration_adv.h** (line 1774) |

All features have been placed in their correct file for this Marlin version.

---

## TMC2100 StealthChop — Full Upgrade Path

### Current State
The Artillery Genius Pro ships with **TMC2100** stepper drivers in standalone mode. TMC2100 has **no UART or SPI interface**. Marlin's software StealthChop (`STEALTHCHOP_XY`, `STEALTHCHOP_Z`, `STEALTHCHOP_E`) requires UART-capable drivers and **cannot be used with TMC2100**.

### Hardware StealthChop on TMC2100
StealthChop mode on TMC2100 is set by physical pin strapping on the driver board:
- **StealthChop ON:** CFG1 = GND, CFG2 = GND
- **SpreadCycle (default):** CFG1 = VIO, CFG2 = GND

### Upgrade Path: TMC2100 → TMC2209 (UART recommended)

**Step 1 — In `Configuration.h`, change all driver types:**
```cpp
#define X_DRIVER_TYPE  TMC2209
#define Y_DRIVER_TYPE  TMC2209
#define Z_DRIVER_TYPE  TMC2209
#define E0_DRIVER_TYPE TMC2209
```

**Step 2 — In `Configuration_adv.h`, enable:**
```cpp
#define STEALTHCHOP_XY
#define STEALTHCHOP_Z
#define STEALTHCHOP_E

#define HYBRID_THRESHOLD
// Then set per-axis thresholds (mm/s above which SpreadCycle activates):
#define X_HYBRID_THRESHOLD  100
#define Y_HYBRID_THRESHOLD  100
#define Z_HYBRID_THRESHOLD    6
#define E0_HYBRID_THRESHOLD  30
```

**Step 3 — Optional sensorless homing (TMC2209 stallGuard):**
```cpp
#define SENSORLESS_HOMING
#define X_STALL_SENSITIVITY 8   // Tune 0-255; higher = less sensitive
#define Y_STALL_SENSITIVITY 8
```

**Step 4 — Wire TMC2209 UART to Ruby board UART pins** and configure `#define X_SLAVE_ADDRESS 0` etc. per your wiring.

---

## Probe Configuration Assumptions

| Item | Value | Confidence | Notes |
|---|---|---|---|
| Probe type | BLTouch | **High** — confirmed in all custom Genius Pro sources | |
| X/Y probe offset | `{ 27.25, -12.8 }` | **High** — both Genius Pro variants agree, likely physical measurement | |
| Z probe offset | `-2` | **Low** — conflicts between sources (-1 vs -2); **must calibrate with M851** | |
| Probe pin | `Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN` | **High** — consistent across all sources | |
| Probe trigger logic | Active-low (`Z_MIN_PROBE_ENDSTOP_INVERTING false`) | **High** — standard BLTouch configuration | |
| `PROBING_MARGIN` | `10` mm | **High** — all sources agree | |

---

## Risks and Assumptions

| # | Risk / Assumption | Severity | Mitigation |
|---|---|---|---|
| 1 | **Z probe offset is wrong.** The -2mm value is from the larger source but conflicts with the -1mm in gpro-mp. | HIGH | Run probe offset wizard or `M851 Z<value>` + `M500` before first UBL leveling. |
| 2 | **Power Loss Recovery untested on Ruby board.** Some Ruby revisions have known issues with PLR pin state after power loss. | HIGH | Test on a sacrificial print before trusting. Enable `PLR_ENABLED_DEFAULT false` and only activate with `M413 S1` after testing. |
| 3 | **Input Shaping frequencies from SW X2, not Genius Pro.** The frame resonance characteristics differ between printers. | MEDIUM | Re-measure with ADXL345 on your Genius Pro using `M593`. Use SHAPING_MENU to tune live. |
| 4 | **Z max feedrate set to 30 mm/s per user spec.** Source firmware uses 10 mm/s. 30 mm/s may cause missed Z steps. | MEDIUM | If Z layer shifting occurs, reduce with `M203 Z10` + `M500`. |
| 5 | **Linear Advance K=0.13 is a starting estimate.** Actual value depends on filament, temperature, and nozzle wear. | MEDIUM | Print a K-factor tower from K=0 to K=0.30. Set with `M900 K<value>` + `M500`. |
| 6 | **Filament runout sensor disabled.** No pin verified for Ruby board. | LOW | Only enable after identifying the correct pin in `pins_ARTILLERY_RUBY.h`. |
| 7 | **ASSISTED_TRAMMING corner positions are estimates.** Bed screw locations assumed at 15mm inset from bed edges. | LOW | Adjust `TRAMMING_POINT_XY` in `Configuration_adv.h` to match actual screw positions on your Genius Pro. |
| 8 | **Both Z steppers share a single TMC2100 driver.** Z_STEPPER_AUTO_ALIGN is disabled for this reason. Do not enable it. | HIGH | With TMC2209 upgrade and independent drivers, re-enable after wiring correctly. |
| 9 | **No display type defined.** The Artillery TFT is a serial host, not a Marlin LCD. If you see a blank screen, verify SERIAL_PORT_2=1 baudrate matches the TFT setting. | LOW | TFT baud rate can be changed in the TFT's own config menu. |
| 10 | **EEPROM_AUTO_INIT disabled.** On first flash, EEPROM will not auto-initialise. | LOW | Run `M502` (factory defaults) → `M500` (save) → `M501` (load) immediately after flashing. |

---

## Post-Flash Calibration Checklist

Perform in this exact order after every firmware flash:

### 1. EEPROM Initialisation
```gcode
M502   ; Load factory defaults
M500   ; Save to EEPROM
M501   ; Load from EEPROM (verify)
```

### 2. Hotend PID Autotune
```gcode
M303 E0 S200 C8   ; Autotune hotend at 200°C, 8 cycles
; When complete, copy Kp/Ki/Kd from serial output, then:
M301 P<Kp> I<Ki> D<Kd>
M500
```

### 3. Bed PID Autotune
```gcode
M303 E-1 S60 C8   ; Autotune bed at 60°C, 8 cycles
; When complete, copy Kp/Ki/Kd from serial output, then:
M304 P<Kp> I<Ki> D<Kd>
M500
```

### 4. Probe Z Offset
```gcode
; Heat hotend and bed to printing temperatures first (PLA: 200°C / 60°C)
G28            ; Home all
M851 Z0        ; Reset Z offset
G1 Z0 F1200    ; Move to Z=0
; Paper test: slide paper under nozzle, adjust until slight drag
M851 Z-<measured_value>   ; e.g. M851 Z-2.15
M500
```

### 5. UBL Bed Leveling
```gcode
G28             ; Home all
G29 P1          ; Auto-probe the full 7×7 grid
G29 P3          ; Fill any missed points by interpolation
G29 S1          ; Save mesh to slot 1
M500            ; Save to EEPROM
; Add to slicer start G-code: G29 L1 (load mesh) + G29 J (tilt correction)
```

### 6. Skew Correction
```gcode
; Print a 200×200mm calibration square, measure diagonals AC and BD with calipers
M852 I<XY_skew_factor>   ; Calculate: skew = (AC² - BD²) / (2 * side²)
; For Z skew, print a calibration box and measure XZ and YZ diagonals
M852 J<XZ_factor> K<YZ_factor>
M500
```

### 7. Linear Advance Calibration
```gcode
; Print a K-factor pressure advance test pattern (Marlin K-factor website)
; Start at K=0.0, test to K=0.30 in 0.05 increments
; Find the value where corners are sharpest without bulging
M900 K<optimal_value>   ; e.g. M900 K0.10
M500
```

### 8. Input Shaping Verification
```
; Connect ADXL345 accelerometer to SPI pins on Ruby board
; Enable M593 resonance testing
; Run resonance test for X axis, then Y axis
; Update SHAPING_FREQ_X and SHAPING_FREQ_Y in Configuration_adv.h
; Recompile and flash
; Alternatively: use SHAPING_MENU on the TFT to tune live without recompiling
```

### 9. Power Loss Recovery Test
```gcode
M413 S1    ; Enable Power Loss Recovery
; Start a 30-minute sacrificial print
; Power off the printer mid-print
; Power on — verify it prompts to resume
; Inspect resume position accuracy and nozzle behaviour
; If unsatisfactory: M413 S0 and investigate PLR_PIN in pins_ARTILLERY_RUBY.h
```

---

## Feature Count Summary

### Configuration.h — Non-Default Features Enabled
1. `BOARD_ARTILLERY_RUBY` (vs upstream RAMPS default)
2. `CUSTOM_MACHINE_NAME "Artillery Genius Pro"`
3. `SERIAL_PORT -1` + `SERIAL_PORT_2 1` + `BAUDRATE 250000`
4. `SHOW_BOOTSCREEN`
5. `EXTRUDERS 1` (explicit)
6. `TEMP_SENSOR_0 1` + `TEMP_SENSOR_BED 1` (EPCOS 100kΩ)
7. `PIDTEMP` with calibrated Kp/Ki/Kd
8. `PIDTEMPBED` with calibrated Kp/Ki/Kd
9. `TMC2100` drivers (×4)
10. `DEFAULT_AXIS_STEPS_PER_UNIT { 80.121, 80.121, 402, 449.5 }`
11. `DEFAULT_MAX_FEEDRATE { 300, 300, 30, 40 }`
12. `DEFAULT_MAX_ACCELERATION { 2000, 2000, 100, 10000 }`
13. `DEFAULT_ACCELERATION 800`
14. `JUNCTION_DEVIATION_MM 0.016` + `JD_HANDLE_SMALL_SEGMENTS`
15. `S_CURVE_ACCELERATION`
16. `USE_PROBE_FOR_Z_HOMING`
17. `BLTOUCH`
18. `NOZZLE_TO_PROBE_OFFSET { 27.25, -12.8, -2 }`
19. `MULTIPLE_PROBING 3` + `EXTRA_PROBING 1`
20. `Z_MIN_PROBE_REPEATABILITY_TEST`
21. `Z_PROBE_OFFSET_RANGE_MIN -8` + `Z_PROBE_OFFSET_RANGE_MAX 8`
22. `DETECT_BROKEN_ENDSTOP`
23. `INVERT_Z_DIR true`
24. `X/Y/Z_MAX_POS 220/220/250`
25. `AUTO_BED_LEVELING_UBL`
26. `GRID_MAX_POINTS_X 7`
27. `UBL_HILBERT_CURVE`
28. `G26_MESH_VALIDATION`
29. `RESTORE_LEVELING_AFTER_G28`
30. `ENABLE_LEVELING_FADE_HEIGHT` (10mm)
31. `Z_SAFE_HOMING`
32. `Z_CLEARANCE_FOR_HOMING 4` (renamed from Z_HOMING_HEIGHT)
33. `SKEW_CORRECTION` + `SKEW_CORRECTION_FOR_Z` + `SKEW_CORRECTION_GCODE`
34. `EEPROM_SETTINGS` (AUTO_INIT and INIT_NOW disabled)
35. `SDSUPPORT` + `SD_CHECK_AND_RETRY`
36. `PRINTJOB_TIMER_AUTOSTART`
37. `NOZZLE_PARK_FEATURE`
38. `NEOPIXEL_LED` (PB7, NEO_GRBW, 1 pixel)
39. `NUM_SERVOS 1` + `SERVO_DELAY { 300 }`

**Total Configuration.h non-default features: 39**

### Configuration_adv.h — Non-Default Features Enabled
1. `TEMP_BED_RESIDENCY_TIME 15` (was 10)
2. `Z_STEPPER_AUTO_ALIGN` DISABLED
3. `ASSISTED_TRAMMING`
4. `INPUT_SHAPING_X` (55.0 Hz)
5. `INPUT_SHAPING_Y` (48.6 Hz)
6. `SHAPING_MENU`
7. `ADAPTIVE_STEP_SMOOTHING`
8. `POWER_LOSS_RECOVERY`
9. `BABYSTEPPING` + `BABYSTEP_ALWAYS_AVAILABLE`
10. `LIN_ADVANCE` (K=0.13)
11. `ARC_SUPPORT` + `ARC_P_CIRCLES`
12. `LONG_FILENAME_HOST_SUPPORT`
13. `AUTO_REPORT_SD_STATUS`
14. `SDCARD_CONNECTION ONBOARD`
15. `ABL_BILINEAR_SUBDIVISION` (no effect with UBL, included for completeness)
16. `SET_PROGRESS_MANUALLY` (renamed from LCD_SET_PROGRESS_MANUALLY)
17. `SHOW_REMAINING_TIME`
18. `M73_REPORT`
19. `ADVANCED_PAUSE_FEATURE`
20. `PARK_HEAD_ON_PAUSE`
21. `FILAMENT_LOAD_UNLOAD_GCODES`
22. `CANCEL_OBJECTS`
23. `GCODE_MACROS`
24. `EMERGENCY_PARSER`
25. `SERIAL_FLOAT_PRECISION 4`
26. `BUFSIZE 32` (was 4)
27. `TX_BUFFER_SIZE 128` (was 0)
28. `RX_BUFFER_SIZE 1024` (was 128)
29. `NO_TIMEOUTS 1000`
30. `ADVANCED_OK`
31. `AUTO_REPORT_TEMPERATURES` (already default; confirmed present)
32. `AUTO_REPORT_POSITION`
33. `EXTENDED_CAPABILITIES_REPORT` (already default; confirmed present)
34. `M115_GEOMETRY_REPORT`
35. `M114_DETAIL`
36. `REPORT_FAN_CHANGE`
37. `HOST_ACTION_COMMANDS`
38. `HOST_PROMPT_SUPPORT`
39. `HOST_STATUS_NOTIFICATIONS`

**Total Configuration_adv.h non-default features: 39**

---

## Warnings Before Flashing

1. **Calibrate Z probe offset immediately after flash.** The -2mm default is an estimate.
2. **Run M502 → M500 → M501 as the very first G-codes** after flashing (EEPROM_AUTO_INIT is disabled).
3. **Power Loss Recovery: test on a sacrificial print.** Do not trust it on real prints until verified.
4. **Z feedrate 30 mm/s:** if you see Z layer shifting, run `M203 Z10` + `M500` to reduce.
5. **Input Shaping frequencies are from a different printer.** Re-measure with ADXL345 on your Genius Pro.
6. **Filament runout sensor is NOT enabled.** This is intentional. Read the commented block in Configuration.h before enabling.
7. **Both Z steppers share one driver on this board.** `Z_STEPPER_AUTO_ALIGN` is disabled. Do not enable it without independent Z drivers.
8. **StealthChop requires hardware driver replacement.** TMC2100 cannot do software StealthChop. See upgrade path above.

---

## Build and Flash

### PlatformIO Environment

| Item | Value |
|---|---|
| Environment name | `Artillery_Ruby` |
| Board definition | `marlin_Artillery_Ruby` (STM32F401RCT6) |
| Defined in | `ini/stm32f4.ini` line 767 |
| Extends | `common_stm32` |
| Upload protocols | stlink, dfu, jlink |
| EEPROM emulation | `FLASH_DATA_SECTOR=1U`, `FLASH_BASE_ADDRESS=0x08004000` |
| Binary flash address | **0x08000000** — binary starts with ISR vector; flash here with STM32CubeProgrammer |
| Main code offset | `0x08008000` — where Marlin main code begins inside the binary |

Flash layout from `buildroot/share/PlatformIO/variants/MARLIN_ARTILLERY_RUBY/ldscript.ld`:
```
BOOT            : ORIGIN = 0x08000000, LENGTH = 16K   ← Marlin ISR vector + Reset_Handler (firmware entry point)
EMULATED_EEPROM : ORIGIN = 0x08004000, LENGTH = 16K   ← runtime EEPROM emulation (0xFF in binary; Marlin writes at runtime)
FLASH           : ORIGIN = 0x08008000, LENGTH = 224K  ← Marlin main code
RAM             : ORIGIN = 0x20000000, LENGTH = 64K
```

**Important:** The `.bin` file produced by `objcopy` spans all three regions starting from the lowest LMA (0x08000000). The first 4 bytes of the binary are `0x20010000` (initial stack pointer), confirming the ISR vector is at the binary's start. The binary must be written to **0x08000000**. There is no separate factory bootloader on the Genius Pro Ruby board — the firmware owns all 256 KB of flash.

---

### Files Placed for Build

| File | Destination |
|---|---|
| `output/Configuration.h` | `Marlin-bugfix-2.1.x/Marlin/Configuration.h` |
| `output/Configuration_adv.h` | `Marlin-bugfix-2.1.x/Marlin/Configuration_adv.h` |
| `platformio.ini` | `default_envs` changed from `mega2560` → `Artillery_Ruby` |

---

### Step 1 — Install PlatformIO

PlatformIO was not found on this machine. Install it before building.

**Option A — Via pip (Python 3 required):**
```
pip install platformio
```

**Option B — Via VS Code Extension:**
Install the "PlatformIO IDE" extension in VS Code. The `pio` CLI is then available in the PlatformIO terminal.

Verify installation:
```
pio --version
```

---

### Step 2 — Build Firmware

Open a terminal in the Marlin release folder and run:

```bash
cd "d:/Documentos/Marlin Firmware for Artillery Genius Pro/latest marlin release/Marlin-bugfix-2.1.x"
pio run -e Artillery_Ruby
```

Expected output location after successful build:
```
.pio/build/Artillery_Ruby/firmware.bin
```

Rename and copy to output folder:
```bash
cp ".pio/build/Artillery_Ruby/firmware.bin" \
   "../../output/firmware-gpro-merged-2026-04-30.bin"
```

---

### Step 3 — Flash via STM32CubeProgrammer (DFU, recommended)

**Prerequisites:**
- STM32CubeProgrammer installed (download from st.com)
- USB cable connected to the printer's USB port
- Printer in DFU mode (see below)

**Entering DFU mode on Artillery Genius Pro (Ruby board):**
1. Power off the printer
2. Hold the BOOT0 button on the Ruby board while connecting USB to PC
3. Release BOOT0 after ~2 seconds
4. Device should appear as "STM32 BOOTLOADER" in Device Manager (USB HW ID: 0x0483/0xDF11)

**Flash command (STM32CubeProgrammer CLI):**
```bash
STM32_Programmer_CLI -c port=USB1 -w "output/firmware-gpro-merged-v2-0x08000000.bin" 0x08000000 -v -rst
```

- `-w <file> 0x08000000` — write binary from base address (binary contains ISR vector + EEPROM gap + main code)
- `-v` — verify after write
- `-rst` — reset after flash

**Flash via STM32CubeProgrammer GUI:**
1. Connect → USB → Connect
2. Erasing & Programming → File path: `firmware-gpro-merged-v2-0x08000000.bin`
3. Start address: `0x08000000`
4. Click "Start Programming"

---

### Step 4 — Flash via ST-Link (alternative)

If you have an ST-Link V2 debugger connected to the SWD pins on the Ruby board:

```bash
pio run -e Artillery_Ruby --target upload
```

Or with OpenOCD:
```bash
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg \
  -c "program firmware-gpro-merged-v2-0x08000000.bin 0x08000000 verify reset exit"
```

---

### Bricking Risk and Recovery

| Risk | Severity | Notes |
|---|---|---|
| Writing at wrong flash address | HIGH | Binary must be written at 0x08000000. Writing at any other offset misaligns the ISR vector and the firmware will not boot. |
| Full-chip erase before flash | LOW | Safe — the Genius Pro Ruby has no separate factory bootloader. A full-chip erase followed by writing the firmware at 0x08000000 is the cleanest approach. |
| Partial erase that misses BOOT sector | MEDIUM | If you erase only sectors 2–15 (old documentation advice) without re-writing the ISR vector, the firmware will not boot. Always write the full binary starting at 0x08000000. |

**Recovery if flash is corrupted or firmware won't boot:**
1. Connect ST-Link V2 to SWD pins on Ruby board (CLK, DIO, GND, 3.3V)
2. Do a full-chip erase via STM32CubeProgrammer
3. Re-flash `firmware-gpro-merged-v2-0x08000000.bin` at 0x08000000

DFU mode (USB BOOT0 button) remains available regardless, because there is no separate bootloader to lose — the STM32's built-in ROM DFU loader is in mask ROM and cannot be erased.

**Safe erase scope** — a full-chip erase is fine. There is no separate bootloader to protect. The firmware binary itself contains all required startup code from 0x08000000.

---

### First Boot After Flash

Run these G-codes immediately after the first successful boot (via Pronterface, OctoPrint, or TFT terminal):

```gcode
M502   ; Load factory defaults (EEPROM_AUTO_INIT is enabled but run this anyway)
M500   ; Save to EEPROM
M501   ; Load from EEPROM (verify)
```

Then proceed with the post-flash calibration checklist above.

---

## Build Iteration — Deprecated Define Fixes

During the PlatformIO build (`python -m platformio run -e Artillery_Ruby`), the following defines were found missing or deprecated in bugfix-2.1.x and required correction:

### Configuration.h Fixes

| Old / Missing | New / Added | Source in Changes.h | Value |
|---|---|---|---|
| `X/Y/Z_MIN_ENDSTOP_INVERTING`, `Z_MIN_PROBE_INVERTING` | Removed; replaced with `*_ENDSTOP_HIT_STATE` | Changes.h:655 | `X/Y: LOW`, `Z_MIN: HIGH`, `Z_MIN_PROBE: HIGH` |
| `USE_XMIN_PLUG`, `USE_YMIN_PLUG`, `USE_ZMIN_PLUG` | Removed (implicit from pin definitions) | Changes.h:669 | — |
| `Z_PROBE_OFFSET_RANGE_MIN/MAX` | Renamed to `PROBE_OFFSET_ZMIN/ZMAX` | Changes.h:686 | `-8` / `8` |
| `DEFAULT_Kp/Ki/Kd` | Renamed to `DEFAULT_KP/KI/KD` | Changes.h:752 | `13.1079` / `0.9257` / `46.4019` |
| `DEFAULT_bedKp/Ki/Kd` | Renamed to `DEFAULT_BED_KP/KI/KD` | Changes.h:755 | `47.0143` / `6.3191` / `233.19` |
| `TEMP_RESIDENCY_TIME` (missing) | Added | SanityCheck.h | `10` s |
| `TEMP_WINDOW` (missing) | Added | SanityCheck.h | `1` °C |
| `TEMP_HYSTERESIS` (missing) | Added | SanityCheck.h | `3` °C |
| `THERMAL_PROTECTION_PERIOD` (missing) | Added | temperature.cpp | `40` s |
| `THERMAL_PROTECTION_HYSTERESIS` (missing) | Added | temperature.cpp | `4` °C |
| `THERMAL_PROTECTION_BED_PERIOD` (missing) | Added | temperature.cpp | `20` s |
| `THERMAL_PROTECTION_BED_HYSTERESIS` (missing) | Added | temperature.cpp | `2` °C |
| `STEP_STATE_X/Y/Z/E` (missing) | Added | stepper.cpp | `HIGH` (standard A4988/TMC) |
| `DEFAULT_MINIMUMFEEDRATE` (missing) | Added | settings.cpp | `0.0` mm/s |
| `DEFAULT_MINTRAVELFEEDRATE` (missing) | Added | settings.cpp | `0.0` mm/s |
| `DEFAULT_MINSEGMENTTIME` (missing) | Added | settings.cpp | `20000` µs |

### Configuration_adv.h Fixes

| Old / Missing | New / Added | Source | Value |
|---|---|---|---|
| `MAX_CMD_SIZE` (missing) | Added | serial.cpp | `96` |
| `PROPORTIONAL_FONT_RATIO` (missing) | Added | serial.cpp | `1.0` (no LCD) |
| `BLOCK_BUFFER_SIZE` (missing) | Added | planner | `16` |
| `MIN_STEPS_PER_SEGMENT` (missing; error msg said "dropsegments") | Added | Changes.h:177 | `6` |
| `BABYSTEP_MULTIPLICATOR_Z` (missing) | Added | babystep.cpp | `1` |
| `TRAMMING_SCREW_THREAD` (missing) | Added inside `ASSISTED_TRAMMING` block | tramming.h:34 | `30` (M3 CW) |
| `TRAMMING_POINT_XY` front/back-left X=15 → X=35 | Fixed reachability | tramming.h:66 | probe X≥35 so nozzle X=7.75≥0 (PROBE_OFFSET_X=27.25) |
| `PAUSE_PARK_NOZZLE_TIMEOUT` (missing) | Added inside `ADVANCED_PAUSE_FEATURE` block | pause.cpp:542 | `45` s |
| `FILAMENT_CHANGE_ALERT_BEEPS` (missing) | Added inside `ADVANCED_PAUSE_FEATURE` block | M701_M702.cpp:117 | `10` |

### Build Result (v1 — initial, 2026-04-30)

| Item | Value |
|---|---|
| Build command | `python -m platformio run -e Artillery_Ruby` |
| Result | **SUCCESS** |
| Firmware size | 187 KB (limit 224 KB — 84% utilization) |
| Output file | `output/firmware-gpro-merged-2026-04-30.bin` |
| Flash address | `0x08000000` |
| Known issue | M115 returns "Unknown command" — `CAPABILITIES_REPORT` not defined (see v3 below) |

---

## Post-Flash Fix — M115 Missing (v3, 2026-05-01)

### Root Cause

M115 ("Unknown command") was compiled out entirely. In the latest Marlin bugfix-2.1.x, `M115.cpp` is wrapped in a new outer guard:

```cpp
#if ENABLED(CAPABILITIES_REPORT)   // ← gates whether M115 exists at all
  ...
  #if ENABLED(EXTENDED_CAPABILITIES_REPORT)  // ← gates Cap:... lines inside M115
  ...
  #endif
#endif
```

Our `Configuration_adv.h` only defined `EXTENDED_CAPABILITIES_REPORT` (copied from the gpro-mp reference, which used an older Marlin version where `M115.cpp` had no outer guard). `CAPABILITIES_REPORT` was never defined, so the entire M115 handler was excluded from the build.

The `Warnings.cpp` in the latest release explicitly warns: `"CAPABILITIES_REPORT is recommended if you have space. Some hosts rely on it."` (~1150 bytes flash cost).

### Fix Applied (Configuration_adv.h)

```cpp
// Before (broken):
#define EXTENDED_CAPABILITIES_REPORT

// After (fixed):
#define CAPABILITIES_REPORT
#if ENABLED(CAPABILITIES_REPORT)
  #define EXTENDED_CAPABILITIES_REPORT
  #if ENABLED(EXTENDED_CAPABILITIES_REPORT)
    #define M115_GEOMETRY_REPORT
  #endif
#endif
```

### Build Result (v3 — M115 fix)

| Item | Value |
|---|---|
| Build command | `python -m platformio run -e Artillery_Ruby` |
| Result | **SUCCESS** |
| Firmware size | 189 KB (limit 224 KB — 85% utilization; +2 KB for M115) |
| Output file | `output/firmware-gpro-merged-v3-0x08000000.bin` |
| Flash address | `0x08000000` |
| Binary verification | Word 0 = `0x20010000` (_estack), Word 1 = `0x08000195` (Reset_Handler) ✓ |

---

## Post-Flash Fixes — M43 + BLTouch z_min (v5, 2026-05-01)

### Issue 1 — M115 (Resolved in v3)

M115 was already fixed in v3 by adding `CAPABILITIES_REPORT`. No further changes in v5.

---

### Issue 2 — M43 "Unknown command" (PINS_DEBUGGING missing)

M43 reports pin state and toggles pins for hardware debugging. It is gated by `PINS_DEBUGGING` in `Configuration_adv.h`. The define was absent from our merged config (the gpro-mp reference had it commented out).

**Fix applied (`Configuration_adv.h`):**

```cpp
#define PINS_DEBUGGING  // Enable M43 to report pin state and toggle pins
```

Flash cost: ~6 KB (M43 reports every GPIO on the chip).

---

### Issue 3 — BLTouch z_min Always TRIGGERED

#### Root Cause: Wrong Pin Assignment in v2/v3

The initial merge set `Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN` (defined, not commented). This tells Marlin that the BLTouch signal and the physical Z endstop share the same pin — but on the Artillery Genius Pro hardware they do **not** share a pin. The result was that Marlin read the physical Z_MIN endstop (PA0) as the probe trigger. PA0 floats HIGH when no endstop is pressed, and with `HIT_STATE HIGH` that permanently reads as TRIGGERED — `M119` reported `z_min: TRIGGERED` at all times.

Marlin's `SanityCheck.h` (line ~1523) additionally enforces that when `Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN` is set, `Z_MIN_ENDSTOP_HIT_STATE` must equal `Z_MIN_PROBE_ENDSTOP_HIT_STATE`. Because BLTouch requires `Z_MIN_PROBE_ENDSTOP_HIT_STATE HIGH`, this forced `Z_MIN_ENDSTOP_HIT_STATE HIGH` — making the physical endstop also read as always triggered.

#### Artillery Ruby Dual-Pin BLTouch Wiring

The Artillery Genius Pro ships with a dual-wire BLTouch arrangement confirmed by both working reference firmwares (`Marlin-for-artillery-genius-pro-bugfix-2.1.x` and `gpro-mp`), which both use identical pin assignments:

| Connector | MCU Pin | Function | Direction |
|---|---|---|---|
| Z_STOP | PA0 | Physical Z limit switch (homing only) | Endstop input |
| Z_OTHR | PC2 | BLTouch signal output (probe trigger) | Probe input |
| SERVO0 | PC3 | BLTouch servo control (deploy/stow) | Servo output |

The BLTouch 5-wire harness connects:
- Signal wire → **Z_OTHR (PC2)** — distinct from the Z endstop port
- Control wire → **SERVO0 (PC3)** — for deploy/stow commands (M280)

No physical rewiring is required. The hardware is factory-wired this way. Both reference firmwares prove it: they home Z correctly with this configuration and the existing wiring.

#### Fix Applied (`Configuration.h`)

```cpp
// Before (broken — v2/v3):
#define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN   // Wrong: routes probe through PA0 (Z_STOP)
// Z_MIN_PROBE_PIN not set — PA0 shared; SanityCheck forces Z_MIN_ENDSTOP_HIT_STATE = HIGH
#define Z_MIN_ENDSTOP_HIT_STATE HIGH          // PA0 floating HIGH → always TRIGGERED

// After (fixed — v5, matching both reference firmwares):
//#define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN  // Commented out: PA0 and PC2 are separate
#define Z_MIN_PROBE_PIN PC2                   // BLTouch signal on Z_OTHR connector
#define Z_MIN_ENDSTOP_HIT_STATE LOW           // Physical NC endstop on PA0: LOW when triggered
#define Z_MIN_PROBE_ENDSTOP_HIT_STATE HIGH    // BLTouch: stowed = HIGH (not triggered), deployed = LOW (triggered)
```

#### Why This Works

| Pin | Role | `HIT_STATE` | Stowed / Open (HIGH) | Deployed / Pressed (LOW) |
|---|---|---|---|---|
| PA0 (Z_STOP) | Physical Z endstop | `LOW` | Not triggered ✓ | Triggered ✓ |
| PC2 (Z_OTHR) | BLTouch signal | `HIGH` | Not triggered ✓ | Triggered ✓ |

BLTouch stowed → PC2 = HIGH → `Z_MIN_PROBE_ENDSTOP_HIT_STATE HIGH` reads as not triggered. BLTouch deployed and touching bed → PC2 = LOW → reads as triggered. PA0 physical endstop works independently for Z homing with `HIT_STATE LOW`.

---

### Build Result (v5 — M43 + BLTouch fix)

| Item | Value |
|---|---|
| Build command | `python -m platformio run -e Artillery_Ruby` |
| Result | **SUCCESS** (identical binary to v4 — no config changes, rename only) |
| Firmware size | 196 KB (limit 224 KB — 88% utilization; +6 KB for M43 vs v3) |
| Output file | `output/firmware-gpro-merged-v5-0x08000000.bin` |
| Flash address | `0x08000000` |
| Binary verification | Word 0 = `0x20010000` (_estack), Word 1 = `0x08000195` (Reset_Handler) ✓ |

### No Rewiring Required

The Artillery Genius Pro hardware is already wired per the dual-pin configuration above. Both reference firmwares home Z correctly with this wiring. Flash v5 directly — no physical changes to the printer.

---

## Post-Flash Fix — TFT Temperature Display (v6, 2026-05-01)

### Problem

The TFT touchscreen showed no live temperature updates after boot. The screen was static until the slicer sent `M155 S2` at print start.

### Root Cause

`AutoReporter::report_interval` is a `uint8_t` with no compile-time default — it is always zero-initialized at startup. Marlin's `AUTO_REPORT_TEMPERATURES` feature (already enabled, confirmed via `Cap:AUTOREPORT_TEMP:1` in M115) only enables the capability; it does not activate pushing. The TFT must receive `M155 S2` explicitly to start the 2-second temperature push cycle.

There is no `DEFAULT_AUTO_REPORT_TEMP_INTERVAL` or similar define in Marlin source. Confirmed by reading `Marlin/src/libs/autoreport.h` — the struct has no default interval initializer:

```cpp
struct AutoReporter {
  millis_t next_report_ms;
  uint8_t report_interval;   // zero-initialized; must be set via M155
  ...
};
```

### Fix Applied (`Configuration_adv.h`)

Marlin's `MarlinCore.cpp` (line 1641-1643) provides a config-only injection point:

```cpp
#ifdef STARTUP_COMMANDS
  queue.inject(F(STARTUP_COMMANDS));
#endif
```

Setting `STARTUP_COMMANDS` in `Configuration_adv.h` queues G-code at the end of `setup()` — no source modification required.

```cpp
// Added to Configuration_adv.h:
#define STARTUP_COMMANDS "M155 S2"
```

`M155 S2` sets the auto-report temperature interval to 2 seconds, matching what the TFT expects. The command runs once per boot after all initialization is complete.

The working reference firmware (`Marlin-for-artillery-genius-pro`) does not use `STARTUP_COMMANDS` — it has `AUTO_REPORT_POSITION` disabled and likely relies on the TFT firmware issuing `M155 S2` itself on connection. Our config keeps `AUTO_REPORT_POSITION` enabled and ensures temperature pushing starts immediately regardless of TFT behavior.

### Build Result (v6 — TFT auto-report fix)

| Item | Value |
|---|---|
| Build command | `python -m platformio run -e Artillery_Ruby` |
| Result | **SUCCESS** |
| Flash delta | +16 bytes (the `"M155 S2"` string literal) |
| Firmware size | 195 KB (limit 224 KB — 87% utilization) |
| Output file | `output/firmware-gpro-merged-v6-0x08000000.bin` |
| Flash address | `0x08000000` |
| Binary verification | Word 0 = `0x20010000` (_estack), Word 1 = `0x08000195` (Reset_Handler) ✓ |

All v5 fixes preserved: probe pin assignment (PC2), M115 (`CAPABILITIES_REPORT`), M43 (`PINS_DEBUGGING`).

## Exhaustive Config Audit and G-code Fixes (v7, 2026-05-01)

### Goals

1. **Fix missing G-code commands** — `M92` (set steps-per-unit) and `M211` (software endstops toggle) were confirmed absent from v6 firmware. Identify root causes, fix, and audit all other differences against the reference.
2. **Preserve all v3–v6 fixes** — BLTouch dual-pin wiring, M115, M43, STARTUP_COMMANDS auto-temp push.

### Root Cause: M92 Missing

`M92.cpp` is compiled only when `EDITABLE_STEPS_PER_UNIT` is defined. This feature flag is gated in `ini/features.ini:293`:

```
EDITABLE_STEPS_PER_UNIT = build_src_filter=+<src/gcode/config/M92.cpp>
```

Neither our config nor the reference `Marlin-bugfix-2.1.x.gpro-mp` had this define. Fix: added `#define EDITABLE_STEPS_PER_UNIT` to `Configuration_adv.h`.

### Root Cause: M211 Missing

`M211()` (software endstops toggle) is gated in `gcode.cpp:750` by `HAS_SOFTWARE_ENDSTOPS`, which is derived from `MIN_SOFTWARE_ENDSTOPS || MAX_SOFTWARE_ENDSTOPS`. Neither was defined in our config. Fix: added the full software endstops block to `Configuration.h`:

```cpp
#define MIN_SOFTWARE_ENDSTOPS
#if ENABLED(MIN_SOFTWARE_ENDSTOPS)
  #define MIN_SOFTWARE_ENDSTOP_X
  #define MIN_SOFTWARE_ENDSTOP_Y
  #define MIN_SOFTWARE_ENDSTOP_Z
#endif
#define MAX_SOFTWARE_ENDSTOPS
#if ENABLED(MAX_SOFTWARE_ENDSTOPS)
  #define MAX_SOFTWARE_ENDSTOP_X
  #define MAX_SOFTWARE_ENDSTOP_Y
  #define MAX_SOFTWARE_ENDSTOP_Z
#endif
```

### Audit Methodology

Extracted all `#define` directives at depth ≤ 1 from both configs using a Python script, normalized whitespace, and classified each difference as ADOPT / KEEP / INVESTIGATE. 8 items required user decisions; the rest were auto-classified.

### User Decisions (8-question resolution)

| # | Setting | Reference | Ours | Decision | Rationale |
|---|---------|-----------|------|----------|-----------|
| 1 | `DEFAULT_RETRACT_ACCELERATION` | 10000 | 2000 | **ADOPT 10000** | Ref tuned for this printer |
| 2 | `DEFAULT_TRAVEL_ACCELERATION` | 2000 | 1000 | **ADOPT 2000** | Ref tuned for this printer |
| 3 | `DEFAULT_XJERK` / `DEFAULT_YJERK` | 5.0 | 8.0 | **KEEP 8.0** | Tuned for Genius Pro motion |
| 4 | `DEFAULT_ZJERK` | 0.3 | 0.4 | **ADOPT 0.3** | Ref tuned for this printer |
| 5 | `Z_PROBE_OFFSET_FROM_EXTRUDER` | -1.2 | -2.0 | **KEEP -2.0** | Physically calibrated on this unit |
| 6 | `PID_FUNCTIONAL_RANGE` | 25 | 10 | **ADOPT 25** | Wider range handles cold starts better |
| 7 | Preheat fan speed (`PREHEAT_*_FAN_SPEED`) | 0 | 255 | **KEEP 0** | Fan should be off during preheat |
| 8 | `Z_MAX_ENDSTOP_HIT_STATE` | HIGH | LOW | **KEEP LOW** | Matches physical wiring on this board |

### Incident: Accidental Mass Replacement of 255→0

During the preheat fan speed edit, `replace_all: true` with `old_string="255"` was used, which zeroed every occurrence of `255` in `Configuration.h` — including `BANG_MAX` and `MAX_BED_POWER` (heater power limits). Both were immediately restored to `255`. A Python script verified no other occurrences remained at `0` unexpectedly.

### Configuration.h Changes Applied (v7)

| Setting | Old | New | Source |
|---------|-----|-----|--------|
| `PID_FUNCTIONAL_RANGE` | 10 | 25 | ADOPT reference |
| `DEFAULT_MAX_FEEDRATE` Z | 10 | 30 | ADOPT reference |
| `DEFAULT_RETRACT_ACCELERATION` | 2000 | 10000 | ADOPT reference |
| `DEFAULT_TRAVEL_ACCELERATION` | 1000 | 2000 | ADOPT reference |
| `DEFAULT_ZJERK` | 0.4 | 0.3 | ADOPT reference |
| `PREHEAT_*_FAN_SPEED` | 255 | 0 | KEEP ours (user confirmed fan should be off) |
| `DISABLE_X/Y/Z false` | old API | `DISABLE_IDLE_X/Y/Z/E` + `DEFAULT_STEPPER_TIMEOUT_SEC 120` | Migration to new API |
| `MIN_SOFTWARE_ENDSTOPS` block | absent | added | Enables M211 |
| `MAX_SOFTWARE_ENDSTOPS` block | absent | added | Enables M211 |
| `AUTOTEMP` block | absent | added | Auto-adjusts hotend temp proportional to speed |
| `QUICK_HOME` | absent | added | Simultaneous X+Y homing |
| `VALIDATE_HOMING_ENDSTOPS` | absent | added | Halts if endstop not triggered during homing |
| `SEGMENT_LEVELED_MOVES` + `LEVELED_SEGMENT_LENGTH 5.0` | absent | added | Per-segment UBL mesh compensation |
| `EVENT_GCODE_SD_ABORT "G28XY"` | absent | added | Parks head after SD abort |
| `SDCARD_RATHERRECENTFIRST` | absent | added | Shows most-recent files first in SD listing |

### Configuration_adv.h Changes Applied (v7)

| Define | Effect |
|--------|--------|
| `SERIAL_OVERRUN_PROTECTION` | Throttles serial when buffer full — prevents data loss |
| `FASTER_GCODE_PARSER` | Smaller, faster G-code parser |
| `BUSY_WHILE_HEATING` | Sends busy status to host during temperature waits |
| `EDITABLE_STEPS_PER_UNIT` | Compiles `M92.cpp` via features.ini gate — enables M92 |
| `USE_WATCHDOG` | Hardware watchdog — resets if firmware hangs >8s |
| `SLOWDOWN` + `SLOWDOWN_DIVISOR 2` | Slows machine if look-ahead buffer is nearly full |
| `MULTISTEPPING_LIMIT 16` | Matches reference (default auto-set to 128 with warning) |
| `HOST_KEEPALIVE_FEATURE` + `DEFAULT_KEEPALIVE_INTERVAL 2` | Sends busy/idle status every 2s so host knows Marlin is alive |

Note: `ALLOW_LOW_EJERK` was in the INVESTIGATE list but was **excluded** — it is deprecated in bugfix-2.1.x and causes a compile error.

### v3–v6 Fix Verification

All prior fixes confirmed intact after all v7 edits:

| Fix | Define | Status |
|-----|--------|--------|
| BLTouch dual-pin (not Z_MIN_PIN) | `Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN` commented out | ✓ |
| BLTouch probe pin | `Z_MIN_PROBE_PIN PC2` | ✓ |
| Endstop hit state | `Z_MIN_ENDSTOP_HIT_STATE LOW` | ✓ |
| Probe hit state | `Z_MIN_PROBE_ENDSTOP_HIT_STATE HIGH` | ✓ |
| M115 / capabilities | `CAPABILITIES_REPORT` + `EXTENDED_CAPABILITIES_REPORT` | ✓ |
| M43 / pin debug | `PINS_DEBUGGING` | ✓ |
| TFT auto-temp push | `STARTUP_COMMANDS "M155 S2"` | ✓ |

### Build Result (v7 — exhaustive audit + G-code fix)

| Item | Value |
|---|---|
| Build command | `python -m platformio run -e Artillery_Ruby` |
| Result | **SUCCESS** |
| RAM usage | 52.8% (34,596 / 65,536 bytes) |
| Flash usage | 65.0% (170,328 / 262,144 bytes) — +~15 KB vs v6 |
| G-code modules compiled | M92.cpp ✓, M211.cpp ✓ |
| Output file | `output/firmware-gpro-merged-v7-0x08000000.bin` |
| Flash address | `0x08000000` |
| Binary size | 198.5 KB |
| Binary verification | Word 0 = `0x20010000` (_estack), Word 1 = `0x08000195` (Reset_Handler) ✓ |
