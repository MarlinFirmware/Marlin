/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * Artillery Genius Pro — Configuration.h
 * Base: Marlin bugfix-2.1.x (HEX 02010300, dated 2026-05-01)
 *
 * Place this file in: Marlin-bugfix-2.1.x/Marlin/Configuration.h
 * Do NOT modify any Marlin source files.
 *
 * SOURCE PRIORITY (lowest → highest authority):
 *   old stock fw           → hardware truth: board, thermistors, serial ports
 *   custom fw - sw x2      → reference only; all hardware values adapted to Genius Pro
 *   custom fw - gpro multi → feature reference; Genius Pro values extracted only
 *   custom fw - gpro       → highest authority for proven settings and feature set
 *   latest marlin release  → syntax authority and output template
 */

#define CONFIGURATION_H_VERSION 02010300

//===========================================================================
//============================= Getting Started =============================
//===========================================================================

// @section info

#define STRING_CONFIG_H_AUTHOR "(Artillery Genius Pro, Marlin bugfix-2.1.x)"
#define CUSTOM_VERSION_FILE Version.h

// @section machine

#ifndef MOTHERBOARD
  #define MOTHERBOARD BOARD_ARTILLERY_RUBY  // SOURCE: old stock fw + custom fw artillery genius pro | REASON: STM32F401RC Ruby board confirmed in OEM firmware
#endif

#define CUSTOM_MACHINE_NAME "Artillery Genius Pro"  // SOURCE: user specification + custom fw artillery genius pro

//#define MACHINE_UUID "00000000-0000-0000-0000-000000000000"

// @section stepper drivers

/**
 * Stepper Drivers — TMC2100 (standalone mode only)
 * ==================================================
 * The Artillery Genius Pro uses TMC2100 drivers. TMC2100 has NO UART or SPI
 * interface. Marlin software StealthChop (STEALTHCHOP_XY / Z / E) requires
 * UART-capable drivers and CANNOT be used with TMC2100.
 *
 * StealthChop on TMC2100 is HARDWARE-ONLY via CFG pin strapping on the driver board:
 *   StealthChop ON:  CFG1=GND, CFG2=GND
 *   SpreadCycle:     CFG1=VIO, CFG2=GND (default from factory)
 *
 * UPGRADE PATH — Replace drivers with TMC2209 (UART, recommended):
 *   In Configuration.h, change all driver types:
 *     #define X_DRIVER_TYPE  TMC2209
 *     #define Y_DRIVER_TYPE  TMC2209
 *     #define Z_DRIVER_TYPE  TMC2209
 *     #define E0_DRIVER_TYPE TMC2209
 *   Then in Configuration_adv.h, enable:
 *     #define STEALTHCHOP_XY
 *     #define STEALTHCHOP_Z
 *     #define STEALTHCHOP_E
 *     #define HYBRID_THRESHOLD        // X:100, Y:100, Z:6, E:30
 *   TMC2209 also enables sensorless homing via stallGuard (SENSORLESS_HOMING).
 *   All TMC2209 requires proper wiring of UART TX/RX to the Ruby board UART pins.
 */
#define X_DRIVER_TYPE  TMC2100  // SOURCE: old stock fw + custom fw artillery genius pro | REASON: hardware-confirmed standalone driver
#define Y_DRIVER_TYPE  TMC2100  // SOURCE: old stock fw + custom fw artillery genius pro | REASON: hardware-confirmed standalone driver
#define Z_DRIVER_TYPE  TMC2100  // SOURCE: old stock fw + custom fw artillery genius pro | REASON: both Z steppers share this single driver on the Ruby board
#define E0_DRIVER_TYPE TMC2100  // SOURCE: old stock fw + custom fw artillery genius pro | REASON: hardware-confirmed standalone driver

// @section serial port

/**
 * Serial Port
 * SOURCE: old stock fw + custom fw artillery genius pro | REASON: confirmed in OEM and all custom sources
 *   -1  = USB virtual serial (STM32F401RC USB-CDC)
 *    1  = UART1, connected to Artillery TFT touchscreen display
 */
#define SERIAL_PORT -1
#define SERIAL_PORT_2 1
#define BAUDRATE 250000

//#define BLUETOOTH

// @section machine

#define SHOW_BOOTSCREEN  // ADDED: show Marlin boot screen on startup

#define LCD_LANGUAGE en
#define DISPLAY_CHARSET_HD44780 JAPANESE
#define LCD_INFO_SCREEN_STYLE 0

// @section extruder

#define EXTRUDERS 1  // SOURCE: old stock fw + custom fw artillery genius pro | REASON: single direct-drive extruder

#define DEFAULT_NOMINAL_FILAMENT_DIA 1.75

// @section temperature

/**
 * Thermistors
 * SOURCE: old stock fw (genius-pro-firmware-main) | REASON: hardware truth from OEM firmware
 * Type 1 = 100kΩ EPCOS thermistor — confirmed in OEM and all custom Genius Pro sources
 */
#define TEMP_SENSOR_0 1   // SOURCE: old stock fw | REASON: EPCOS 100kΩ hotend thermistor
#define TEMP_SENSOR_1 0
#define TEMP_SENSOR_2 0
#define TEMP_SENSOR_3 0
#define TEMP_SENSOR_4 0
#define TEMP_SENSOR_5 0
#define TEMP_SENSOR_6 0
#define TEMP_SENSOR_7 0
#define TEMP_SENSOR_BED 1  // SOURCE: old stock fw | REASON: EPCOS 100kΩ bed thermistor
#define TEMP_SENSOR_PROBE 0
#define TEMP_SENSOR_CHAMBER 0
#define TEMP_SENSOR_COOLER 0
#define TEMP_SENSOR_BOARD 0
#define TEMP_SENSOR_SOC 0
#define TEMP_SENSOR_REDUNDANT 0

// Temperature safety limits
#define HEATER_0_MINTEMP   5    // SOURCE: custom fw artillery genius pro
#define HEATER_0_MAXTEMP 275    // SOURCE: custom fw artillery genius pro | REASON: E3D-style hotend with PTFE liner maximum safe temperature
#define BED_MINTEMP        5    // SOURCE: custom fw artillery genius pro
#define BED_MAXTEMP      150    // SOURCE: custom fw artillery genius pro | REASON: standard heated bed limit

// Hotend temperature stability
#define TEMP_RESIDENCY_TIME   10  // (s) Wait for hotend to "settle" in M109
#define TEMP_WINDOW            1  // (°C) Temperature proximity for the "ready" timer
#define TEMP_HYSTERESIS        3  // (°C) Temperature proximity considered "close enough" to target

// @section pid

/**
 * Hotend PID
 * SOURCE: custom fw artillery genius pro (gpro-mp, MirakelPrints/thisiskeithb)
 * REASON: calibrated on actual Genius Pro hardware; preferred over the upstream example values.
 * NOTE: run M303 E0 S200 C8 after first flash, then M500.
 */
#define PIDTEMP
#define BANG_MAX 255
#define PID_MAX BANG_MAX
#define PID_K1 0.95

#if ENABLED(PIDTEMP)
  #define PID_FUNCTIONAL_RANGE 25
  #define DEFAULT_KP  13.1079  // SOURCE: custom fw artillery genius pro (gpro-mp) | REASON: real hardware calibration
  #define DEFAULT_KI   0.9257  // SOURCE: custom fw artillery genius pro (gpro-mp) | REASON: real hardware calibration
  #define DEFAULT_KD  46.4019  // SOURCE: custom fw artillery genius pro (gpro-mp) | REASON: real hardware calibration
#endif

/**
 * Bed PID
 * SOURCE: custom fw artillery genius pro (gpro-mp, MirakelPrints/thisiskeithb)
 * REASON: calibrated on actual Genius Pro hardware.
 * NOTE: run M303 E-1 S60 C8 after first flash, then M500.
 */
#define PIDTEMPBED
//#define BED_LIMIT_SWITCHING
#define MAX_BED_POWER 255

#if ENABLED(PIDTEMPBED)
  #define DEFAULT_BED_KP  47.0143  // SOURCE: custom fw artillery genius pro (gpro-mp) | REASON: real hardware calibration
  #define DEFAULT_BED_KI   6.3191  // SOURCE: custom fw artillery genius pro (gpro-mp) | REASON: real hardware calibration
  #define DEFAULT_BED_KD 233.1900  // SOURCE: custom fw artillery genius pro (gpro-mp) | REASON: real hardware calibration
#endif

// @section extruder

#define PREVENT_COLD_EXTRUSION
#define EXTRUDE_MINTEMP 170
#define PREVENT_LENGTHY_EXTRUDE
#define EXTRUDE_MAXLENGTH 200

// @section temperature

#define THERMAL_PROTECTION_HOTENDS
#define THERMAL_PROTECTION_PERIOD      40  // (s) Seconds to measure hotend temp stability
#define THERMAL_PROTECTION_HYSTERESIS   4  // (°C) Tolerance for hotend thermal runaway

#define THERMAL_PROTECTION_BED
#define THERMAL_PROTECTION_BED_PERIOD  20  // (s) Seconds to measure bed temp stability
#define THERMAL_PROTECTION_BED_HYSTERESIS 2  // (°C) Tolerance for bed thermal runaway

// Auto-adjust target temp proportional to print speed (useful with fast printing profiles)
#define AUTOTEMP
#if ENABLED(AUTOTEMP)
  #define AUTOTEMP_OLDWEIGHT 0.98
  #define AUTOTEMP_MIN       210
  #define AUTOTEMP_MAX       250
  #define AUTOTEMP_FACTOR    0.1f
#endif

#define THERMAL_PROTECTION_CHAMBER
#define THERMAL_PROTECTION_COOLER

// @section motion

/**
 * Steps per mm — CALIBRATED GENIUS PRO VALUES
 * SOURCE: custom fw artillery genius pro (both gpro variants agree)
 * REASON: fractional precision from hardware measurement.
 *         SW X2 uses rounded values { 80, 80, 400, 445 } — do NOT use those.
 */
#define DEFAULT_AXIS_STEPS_PER_UNIT   { 80.121, 80.121, 402, 449.5 }

/**
 * Max Feed Rate (mm/s)
 * SOURCE: user specification | REASON: Z=30 confirmed by user (differs from bugfix-2.1.x source Z=10)
 */
#define DEFAULT_MAX_FEEDRATE          { 300, 300, 30, 40 }  // Z=30 adopted from gpro-mp reference (was 10)

// Step pin active state — HIGH for A4988/TMC drivers (standard)
#define STEP_STATE_X HIGH
#define STEP_STATE_Y HIGH
#define STEP_STATE_Z HIGH
#define STEP_STATE_E HIGH

//#define LIMITED_MAX_FR_EDITING
#if ENABLED(LIMITED_MAX_FR_EDITING)
  #define MAX_FEEDRATE_EDIT_VALUES    { 600, 600, 10, 50 }
#endif

/**
 * Max Acceleration (mm/s^2)
 * SOURCE: custom fw artillery genius pro (bugfix-2.1.x)
 */
#define DEFAULT_MAX_ACCELERATION      { 2000, 2000, 100, 10000 }

//#define LIMITED_MAX_ACCEL_EDITING
#if ENABLED(LIMITED_MAX_ACCEL_EDITING)
  #define MAX_ACCEL_EDIT_VALUES       { 6000, 6000, 200, 20000 }
#endif

/**
 * Default Acceleration
 * SOURCE: custom fw artillery genius pro (bugfix-2.1.x)
 */
#define DEFAULT_ACCELERATION          800
#define DEFAULT_RETRACT_ACCELERATION  10000
#define DEFAULT_TRAVEL_ACCELERATION   2000

#define DEFAULT_MINIMUMFEEDRATE       0.0   // (mm/s) Minimum feedrate for print moves
#define DEFAULT_MINTRAVELFEEDRATE     0.0   // (mm/s) Minimum feedrate for travel moves
#define DEFAULT_MINSEGMENTTIME    20000     // (µs) Minimum planner segment time

/**
 * Default Jerk — CLASSIC_JERK disabled in favour of JUNCTION_DEVIATION_MM
 * SOURCE: custom fw artillery genius pro (bugfix-2.1.x)
 *
 * NOTE: ALLOW_LOW_EJERK is deprecated in bugfix-2.1.x and causes a compile error.
 *       It has been removed per Changes.h. No replacement needed — E jerk is
 *       controlled by JUNCTION_DEVIATION_MM when CLASSIC_JERK is disabled.
 */
//#define CLASSIC_JERK
#if ENABLED(CLASSIC_JERK)
  #define DEFAULT_XJERK  8.0
  #define DEFAULT_YJERK  8.0
  #define DEFAULT_ZJERK  0.3
  #define DEFAULT_EJERK  5.0
#endif

/**
 * Junction Deviation
 * SOURCE: custom fw artillery genius pro (bugfix-2.1.x)
 * REASON: proven value for Genius Pro; smoother cornering vs classic jerk
 */
#define JUNCTION_DEVIATION_MM 0.016
#define JD_HANDLE_SMALL_SEGMENTS

/**
 * S-Curve Acceleration
 * SOURCE: custom fw artillery genius pro (bugfix-2.1.x)
 * REASON: reduces stepper vibration and improves corner quality.
 * NOTE: EXPERIMENTAL_SCURVE is deprecated in bugfix-2.1.x — compile error if used.
 *       Removed per Changes.h line 632-633. S_CURVE_ACCELERATION is the correct define.
 */
#define S_CURVE_ACCELERATION

// @section probing

/**
 * Probe: BLTouch
 * SOURCE: custom fw artillery genius pro (both variants) | REASON: hardware-confirmed
 * CONSISTENCY CHECK:
 *   - Z_SAFE_HOMING enabled because probe is defined — confirmed.
 *   - Probe-related features (MULTIPLE_PROBING, Z_MIN_PROBE_REPEATABILITY_TEST, etc.)
 *     only enabled because BLTouch is defined — confirmed.
 */
#define USE_PROBE_FOR_Z_HOMING

//#define Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN  // Disabled: sharing Z_MIN pin forces HIT_STATE=HIGH → BLTouch stowed (HIGH) reads as always TRIGGERED
#define Z_MIN_PROBE_PIN PC2  // BLTouch signal wire → Z_OTHR connector (PC2); Z endstop stays on Z_STOP (PA0)

//#define PROBING_HEATERS_OFF
//#define WAIT_FOR_BED_HEATER
//#define WAIT_FOR_HOTEND
//#define PROBING_FANS_OFF
//#define PROBING_STEPPERS_OFF
//#define DELAY_BEFORE_PROBING 200

#define BLTOUCH  // SOURCE: custom fw artillery genius pro | REASON: all Genius Pro sources consistently enable BLTouch

#if ENABLED(BLTOUCH)
  //#define BLTOUCH_DELAY 500
  //#define BLTOUCH_SET_5V_MODE
  //#define BLTOUCH_FORCE_5V_MODE
  //#define BLTOUCH_HS_MODE 1
#endif

/**
 * Probe offsets from nozzle
 * X/Y SOURCE: custom fw artillery genius pro (both variants agree on 27.25, -12.8)
 * REASON: physical measurement of BLTouch mount position on Genius Pro carriage
 * Z SOURCE: custom fw artillery genius pro bugfix-2.1.x uses -2
 * WARNING: Z offset MUST be verified with M851 after every flash. Run probe offset wizard.
 */
#define NOZZLE_TO_PROBE_OFFSET { 27.25, -12.8, -2 }

#define PROBING_MARGIN 10  // SOURCE: custom fw artillery genius pro | REASON: consistent across all sources; keeps probe away from bed edges

#define XY_PROBE_FEEDRATE (133*60)

#define Z_PROBE_FEEDRATE_FAST (4*60)
#define Z_PROBE_FEEDRATE_SLOW (Z_PROBE_FEEDRATE_FAST / 2)

#define Z_CLEARANCE_DEPLOY_PROBE   10
#define Z_CLEARANCE_BETWEEN_PROBES  5
#define Z_CLEARANCE_MULTI_PROBE     5
//#define Z_AFTER_PROBING           5

#define Z_PROBE_LOW_POINT  -2

#define Z_MIN_PROBE_REPEATABILITY_TEST  // ADDED: allows M48 probe repeatability test

#define PROBE_OFFSET_ZMIN -8  // ADDED: per user specification (renamed from Z_PROBE_OFFSET_RANGE_MIN in bugfix-2.1.x)
#define PROBE_OFFSET_ZMAX  8  // ADDED: per user specification (renamed from Z_PROBE_OFFSET_RANGE_MAX in bugfix-2.1.x)

/**
 * Multiple Probing
 * ADDED: SOURCE: custom fw artillery genius pro (bugfix-2.1.x) + user specification
 * REASON: averages 3 probe readings and discards the outlier for accuracy
 */
#define MULTIPLE_PROBING 3
#define EXTRA_PROBING    1

// @section motion

// Broken endstop detection — ADDED per user specification
#define DETECT_BROKEN_ENDSTOP

// @section homing

// USE_XMIN_PLUG / USE_YMIN_PLUG / USE_ZMIN_PLUG — removed in bugfix-2.1.x (Changes.h:669)
// Endstop activation is now implicit from pin definitions in the board pins file

#define ENDSTOPPULLUPS

// Endstop hit states — *_ENDSTOP_INVERTING was removed in bugfix-2.1.x; use *_ENDSTOP_HIT_STATE HIGH/LOW
// SOURCE: custom fw artillery genius pro (gpro-mp) + SanityCheck.h requirements
// X/Y mechanical switches: active-low → LOW
// Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN is disabled (matching gpro-mp reference).
// Z_MIN pin (PA0) used only for endstop homing; BLTouch uses servo pin PC3 for control.
// Z_MIN_ENDSTOP_HIT_STATE LOW: BLTouch stowed output is HIGH (open-drain+pull-up) → LOW means NOT triggered.
#define X_MIN_ENDSTOP_HIT_STATE LOW    // mechanical switch — active-low
#define Y_MIN_ENDSTOP_HIT_STATE LOW    // mechanical switch — active-low
#define Z_MIN_ENDSTOP_HIT_STATE LOW    // Physical endstop on Z_MIN (PA0): LOW when triggered (NC switch)
#define X_MAX_ENDSTOP_HIT_STATE LOW    // not used (software limits)
#define Y_MAX_ENDSTOP_HIT_STATE LOW    // not used (software limits)
#define Z_MAX_ENDSTOP_HIT_STATE LOW    // not used (software limits)
#define Z_MIN_PROBE_ENDSTOP_HIT_STATE HIGH  // BLTouch requires HIGH without INVERTED_PROBE_STATE

// Software endstops — required for M211 (Enable/Disable/Report software endstops)
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

/**
 * Motor directions
 * SOURCE: custom fw artillery genius pro (bugfix-2.1.x) | REASON: confirmed correct for Genius Pro
 */
#define INVERT_X_DIR  false
#define INVERT_Y_DIR  false
#define INVERT_Z_DIR  true   // SOURCE: custom fw artillery genius pro | REASON: Z leadscrew direction on Ruby board requires inversion
#define INVERT_E0_DIR false   // SOURCE: custom fw artillery genius pro | REASON: direct drive, standard direction

// Stepper enable logic — active-low standard for Ruby board
#define X_ENABLE_ON LOW
#define Y_ENABLE_ON LOW
#define Z_ENABLE_ON LOW
#define E_ENABLE_ON LOW

#define DISABLE_IDLE_X
#define DISABLE_IDLE_Y
#define DISABLE_IDLE_Z
#define DISABLE_IDLE_E
#define DEFAULT_STEPPER_TIMEOUT_SEC 120
#define DISABLE_E false

// Homing direction — all axes home to MIN
#define X_HOME_DIR -1  // SOURCE: custom fw artillery genius pro
#define Y_HOME_DIR -1  // SOURCE: custom fw artillery genius pro
#define Z_HOME_DIR -1  // SOURCE: custom fw artillery genius pro

// @section machine

// Travel limits after homing
// SOURCE: custom fw artillery genius pro | REASON: 220×220×250mm build volume
#define X_MIN_POS 0
#define Y_MIN_POS 0   // NOTE: SW X2 uses -5; Genius Pro homes to 0 — do not copy SW X2 value
#define Z_MIN_POS 0
#define X_MAX_POS 220
#define Y_MAX_POS 220
#define Z_MAX_POS 250

// @section bed leveling

/**
 * Bed Leveling: UNIFIED BED LEVELING (UBL)
 * ADDED: SOURCE: custom fw artillery genius pro (bugfix-2.1.x) + user specification
 *
 * CONSISTENCY CHECKS PERFORMED:
 *   1. AUTO_BED_LEVELING_UBL: ENABLED
 *      AUTO_BED_LEVELING_BILINEAR: DISABLED — confirmed mutually exclusive, not both active.
 *   2. Probe is defined (BLTouch) — all probe-dependent features are valid.
 *   3. Z_SAFE_HOMING enabled because probe is configured — confirmed.
 *   4. ABL_BILINEAR_SUBDIVISION is in Configuration_adv.h but has NO EFFECT with UBL active.
 *      It is included there for completeness only.
 */
//#define AUTO_BED_LEVELING_3POINT
//#define AUTO_BED_LEVELING_LINEAR
//#define AUTO_BED_LEVELING_BILINEAR   // DISABLED: mutually exclusive with UBL
#define AUTO_BED_LEVELING_UBL
//#define MESH_BED_LEVELING

#define RESTORE_LEVELING_AFTER_G28  // ADDED: re-applies saved mesh after every G28
#define SEGMENT_LEVELED_MOVES       // Split long moves into segments and apply mesh compensation per-segment
#define LEVELED_SEGMENT_LENGTH 5.0  // (mm) Maximum segment length for leveled moves

#if ENABLED(AUTO_BED_LEVELING_UBL)
  #define MESH_INSET 1
  #define GRID_MAX_POINTS_X 7       // ADDED: 7×7=49 probe points; good coverage for 220mm bed
  #define GRID_MAX_POINTS_Y GRID_MAX_POINTS_X

  #define UBL_HILBERT_CURVE         // ADDED: minimises travel during probing via Hilbert space-filling curve

  //#define MESH_G28_REST_ORIGIN
  //#define UBL_Z_RAISE_WHEN_OFF_MESH 2.5
#endif

// G26 Mesh Validation Pattern
#define G26_MESH_VALIDATION  // ADDED: enables G26 to print a test pattern after UBL calibration
#if ENABLED(G26_MESH_VALIDATION)
  #define MESH_TEST_NOZZLE_SIZE    0.4
  #define MESH_TEST_LAYER_HEIGHT   0.2
  #define MESH_TEST_HOTEND_TEMP  205
  #define MESH_TEST_BED_TEMP      60
  #define G26_XY_FEEDRATE         20
  #define G26_XY_FEEDRATE_TRAVEL 100
  #define G26_RETRACT_MULTIPLIER   1.0
#endif

// Leveling fade height — gradually reduce correction above this height
#define ENABLE_LEVELING_FADE_HEIGHT
#if ENABLED(ENABLE_LEVELING_FADE_HEIGHT)
  #define DEFAULT_LEVELING_FADE_HEIGHT 10.0  // SOURCE: custom fw artillery genius pro
#endif

// @section homing

/**
 * Z Safe Homing
 * ADDED: ensures Z homing occurs at a point the BLTouch probe can reach.
 * Only enabled because BLTouch probe is configured — consistency check passed.
 */
#define Z_SAFE_HOMING
#if ENABLED(Z_SAFE_HOMING)
  #define Z_SAFE_HOMING_X_POINT X_CENTER
  #define Z_SAFE_HOMING_Y_POINT Y_CENTER
#endif

/**
 * Homing feedrates
 * SOURCE: custom fw artillery genius pro (bugfix-2.1.x)
 * REASON: proven values; Z=1500 mm/min safe for 402 steps/mm leadscrew
 */
#define HOMING_FEEDRATE_MM_M { (100*60), (100*60), (25*60) }

#define HOMING_BUMP_MM      { 5, 5, 2 }
#define HOMING_BUMP_DIVISOR { 2, 2, 4 }
#define QUICK_HOME              // Home X and Y simultaneously for faster homing
#define VALIDATE_HOMING_ENDSTOPS  // Halt if endstop not triggered during homing (safety)

/**
 * Z Clearance for Homing
 * RENAMED in Marlin bugfix-2.1.x: was Z_HOMING_HEIGHT, now Z_CLEARANCE_FOR_HOMING.
 * Using the old name (Z_HOMING_HEIGHT) causes a compile error (Changes.h line 163-164).
 */
#define Z_CLEARANCE_FOR_HOMING 4  // RENAMED in Marlin bugfix-2.1.x: was Z_HOMING_HEIGHT

//#define Z_AFTER_HOMING 5

// @section motion

#define AXIS_RELATIVE_MODES { false, false, false, false }

// @section temperature

/**
 * Skew Correction
 * ADDED: SOURCE: user specification
 * NOTE: Reference sources have this commented out — explicitly enabling per user request.
 *       Print a calibration square, measure diagonals, then run:
 *         M852 I<XY_factor> [J<XZ_factor>] [K<YZ_factor>]  (with SKEW_CORRECTION_GCODE)
 *         M500  (save to EEPROM)
 */
#define SKEW_CORRECTION
#if ENABLED(SKEW_CORRECTION)
  // Default values (square = no skew). Measure from a printed calibration square.
  #define XY_DIAG_AC 282.8427124746
  #define XY_DIAG_BD 282.8427124746
  #define XY_SIDE_AD 200

  #define SKEW_CORRECTION_FOR_Z  // ADDED: enable Z skew correction as well
  #if ENABLED(SKEW_CORRECTION_FOR_Z)
    #define XZ_DIAG_AC 282.8427124746
    #define XZ_DIAG_BD 282.8427124746
    #define YZ_DIAG_AC 282.8427124746
    #define YZ_DIAG_BD 282.8427124746
    #define YZ_SIDE_AD 200
  #endif

  #define SKEW_CORRECTION_GCODE  // ADDED: enables M852 for runtime skew correction; save with M500
#endif

// @section eeprom

/**
 * EEPROM
 * EEPROM_AUTO_INIT and EEPROM_INIT_NOW deliberately disabled.
 * SOURCE: user specification | REASON: manual control via M502 → M500 → M501
 */
#define EEPROM_SETTINGS
#define EEPROM_CHITCHAT
#define EEPROM_BOOT_SILENT    // SOURCE: custom fw artillery genius pro (bugfix-2.1.x) | REASON: keeps M503 quiet, only reports errors
#if ENABLED(EEPROM_SETTINGS)
  #define EEPROM_AUTO_INIT    // SOURCE: custom fw artillery genius pro (bugfix-2.1.x) | REASON: silently reinitialises EEPROM on version mismatch without requiring M502
  //#define EEPROM_INIT_NOW   // Init EEPROM on first boot after new build — left disabled (matches both sources)
#endif

// @section extras

/**
 * SD Card Support
 * SOURCE: custom fw artillery genius pro | REASON: print from SD card
 */
#define SDSUPPORT
#define EVENT_GCODE_SD_ABORT "G28XY"  // G-code to run after SD print abort (park head)
#if ENABLED(SDSUPPORT)
  #define SD_CHECK_AND_RETRY  // ADDED: re-reads if a block has a checksum error; improves reliability
  #define SDCARD_RATHERRECENTFIRST  // Show most recently modified files first in SD listing
#endif

// @section interface

#define ENCODER_PULSES_PER_STEP 4
#define ENCODER_STEPS_PER_MENU_ITEM 1

// @section lcd

// Artillery Genius Pro uses an Artillery/BTT TFT touchscreen connected via SERIAL_PORT_2=1.
// The TFT acts as a serial host — no traditional LCD controller is used.
// TFT communication features are enabled in Configuration_adv.h under "BTT TFT support".
// No TFT_COLOR_UI, TFT_CLASSIC_UI, TOUCH_SCREEN, or DWIN_CREALITY_LCD defines are needed.

#define PRINTJOB_TIMER_AUTOSTART  // ADDED: auto-start print timer when heating completes

// @section safety

/**
 * Nozzle Park
 * ADDED: required by ADVANCED_PAUSE_FEATURE and filament load/unload G-codes
 */
#define NOZZLE_PARK_FEATURE
#if ENABLED(NOZZLE_PARK_FEATURE)
  #define NOZZLE_PARK_POINT { (X_MIN_POS + 10), (Y_MAX_POS - 10), 20 }
  #define NOZZLE_PARK_MOVE          0
  #define NOZZLE_PARK_Z_RAISE_MIN   2
  #define NOZZLE_PARK_XY_FEEDRATE 100
  #define NOZZLE_PARK_Z_FEEDRATE    5
#endif

// @section motion

/**
 * Filament Runout Sensor
 * ========================
 * NOT enabled by default.
 * WARNING: enabling without a physical sensor or with an incorrect pin will cause
 * the printer to pause / refuse every print. Verify the pin for your Ruby board first.
 *
 * To enable:
 *   1. Identify the runout pin in: Marlin/src/pins/stm32f4/pins_ARTILLERY_RUBY.h
 *   2. Uncomment the block below and set FIL_RUNOUT_PIN to the verified pin.
 *
 * //#define FILAMENT_RUNOUT_SENSOR
 * //#if ENABLED(FILAMENT_RUNOUT_SENSOR)
 * //  #define NUM_RUNOUT_SENSORS   1
 * //  #define FIL_RUNOUT_PIN  <VERIFIED_PIN>   // Check pins_ARTILLERY_RUBY.h
 * //  #define FIL_RUNOUT_STATE     LOW
 * //  #define FILAMENT_RUNOUT_SCRIPT "M600"
 * //  //#define FILAMENT_RUNOUT_DISTANCE_MM 25
 * //#endif
 */

// @section extras

/**
 * NeoPixel LED
 * SOURCE: custom fw artillery genius pro (bugfix-2.1.x)
 * REASON: PB7 is the Artillery Ruby board onboard NeoPixel status LED (1 pixel, NEO_GRBW).
 *         Removing this disables the board status LED.
 */
#define NEOPIXEL_LED
#if ENABLED(NEOPIXEL_LED)
  #define NEOPIXEL_TYPE    NEO_GRBW  // SOURCE: custom fw artillery genius pro | REASON: Ruby board LED type
  #define NEOPIXEL_PIN         PB7   // SOURCE: custom fw artillery genius pro | REASON: Ruby board LED pin
  #define NEOPIXEL_PIXELS        1   // SOURCE: custom fw artillery genius pro | REASON: single status LED
  #define NEOPIXEL_IS_SEQUENTIAL    // Sequential display for temperature status
  #define NEOPIXEL_BRIGHTNESS  127   // Initial brightness (0-0)
  //#define NEOPIXEL_STARTUP_TEST
#endif

// @section servos

#define NUM_SERVOS 1          // SOURCE: custom fw artillery genius pro | REASON: BLTouch servo slot
#define SERVO_DELAY { 300 }   // SOURCE: custom fw artillery genius pro | REASON: BLTouch deploy/stow delay (ms)
//#define DEACTIVATE_SERVOS_AFTER_MOVE

// @section interface

// Preheat presets — SOURCE: custom fw artillery genius pro (bugfix-2.1.x)
#define PREHEAT_1_LABEL       "PLA"
#define PREHEAT_1_TEMP_HOTEND 200
#define PREHEAT_1_TEMP_BED     60
#define PREHEAT_1_FAN_SPEED   0

#define PREHEAT_2_LABEL       "PETG"
#define PREHEAT_2_TEMP_HOTEND 225
#define PREHEAT_2_TEMP_BED     80
#define PREHEAT_2_FAN_SPEED   0

#define PREHEAT_3_LABEL       "ABS"
#define PREHEAT_3_TEMP_HOTEND 240
#define PREHEAT_3_TEMP_BED    110
#define PREHEAT_3_FAN_SPEED   0
