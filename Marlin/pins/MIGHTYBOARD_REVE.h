/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * Mightyboard Rev.E pin assignments
 */

/**
 *
 * This is a starting-point to support the Makerbot Replicator series of 3D printers.
 * It's not functional because Marlin has limited support for some features.
 * Marlin will need the following augmentations before it will be supportable:
 *
 *   - Support for two or more MAX6675 thermocouples
 *   - Support for multiple i2c buses to control the MCP4018 digital pots
 *   - Support for one additional unidirectional SPI bus, to read the thermocouples
 *   - Support for an RGB LED that may work differently from BLINKM
 *
 * The MCP4018 requires separate I2C buses because it has a fixed address (0x2F << 1 = 0x5E)
 * The thermocouples share the same SCK and DO pins, with their own CS pins.
 * The controller interface port connects to a 3-wire shift-register display controller
 *
 */

#if !defined(__AVR_ATmega1280__) && !defined(__AVR_ATmega2560__)
  #error "Oops! Make sure you have 'Arduino Mega' selected from the 'Tools -> Boards' menu."
#endif

#define DEFAULT_MACHINE_NAME    "MB Replicator"
#define BOARD_NAME              "Mightyboard"

#define LARGE_FLASH true

//
// Servos
//
#define SERVO0_PIN         36 // C1 (1280-EX1)
#define SERVO1_PIN         37 // C0 (1280-EX2)
#define SERVO2_PIN         40 // G1 (1280-EX3)
#define SERVO3_PIN         41 // G0 (1280-EX4)

//
// Limit Switches
//
#define X_MIN_PIN          49 // L0
#define X_MAX_PIN          48 // L1
#define Y_MIN_PIN          47 // L2
#define Y_MAX_PIN          46 // L3
#define Z_MIN_PIN          43 // L6
#define Z_MAX_PIN          42 // L7

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  42
#endif

//
// Steppers
//
#define X_STEP_PIN         55 // F1
#define X_DIR_PIN          54 // F0
#define X_ENABLE_PIN       56 // F2

#define Y_STEP_PIN         59 // F5
#define Y_DIR_PIN          58 // F4
#define Y_ENABLE_PIN       60 // F6

#define Z_STEP_PIN         63 // K1
#define Z_DIR_PIN          62 // K0
#define Z_ENABLE_PIN       64 // K2

#define E0_STEP_PIN        25 // A3
#define E0_DIR_PIN         24 // A2
#define E0_ENABLE_PIN      26 // A4

#define E1_STEP_PIN        29 // A7
#define E1_DIR_PIN         28 // A6
#define E1_ENABLE_PIN      39 // G2

//
// I2C Digipots - MCP4018
// Address 5E (2F << 1)
// Set from 0 - 127 with stop bit.
// (Ex. 3F << 1 | 1)
//
#define DIGIPOTS_I2C_SCL    76 // J5
#define DIGIPOTS_I2C_SDA_X  57 // F3
#define DIGIPOTS_I2C_SDA_Y  61 // F7
#define DIGIPOTS_I2C_SDA_Z  65 // K3
#define DIGIPOTS_I2C_SDA_E0 27 // A5
#define DIGIPOTS_I2C_SDA_E1 77 // J6

//
// Temperature Sensors
//
#define TEMP_BED_PIN       69 // K7

// SPI for Max6675 or Max31855 Thermocouple
// Uses a separate SPI bus
//
//  3 E5 DO (SO)
//  5 E3 CS1
//  2 E4 CS2
// 78 E2 SCK
//
#define THERMO_SCK_PIN     78 // E2
#define THERMO_DO_PIN       3 // E5
#define THERMO_CS1          5 // E3
#define THERMO_CS2          2 // E4

//
// Augmentation for auto-assigning plugs
//
// Two thermocouple connectors allows for either
// 2 extruders or 1 extruder and a heated bed.
// With no heated bed, an additional 24V fan is possible.
//
#define MOSFET_A_PIN     6 // H3
#define MOSFET_B_PIN     9 // B5
#define MOSFET_C_PIN    45 // L4
#define MOSFET_D_PIN    44 // L5

#if HOTENDS > 1
  #if TEMP_SENSOR_BED
    #define IS_EEB
  #else
    #define IS_EEF
  #endif
#elif TEMP_SENSOR_BED
  #define IS_EFB
#else
  #define IS_EFF
#endif

//
// Heaters / Fans (24V)
//
#define HEATER_0_PIN     MOSFET_A_PIN

#if ENABLED(IS_EFB)                            // Hotend, Fan, Bed
  #define FAN_PIN        MOSFET_B_PIN
  #define HEATER_BED_PIN MOSFET_C_PIN
#elif ENABLED(IS_EEF)                          // Hotend, Hotend, Fan
  #define HEATER_1_PIN   MOSFET_B_PIN
  #define FAN_PIN        MOSFET_C_PIN
#elif ENABLED(IS_EEB)                          // Hotend, Hotend, Bed
  #define HEATER_1_PIN   MOSFET_B_PIN
  #define HEATER_BED_PIN MOSFET_C_PIN
#elif ENABLED(IS_EFF)                          // Hotend, Fan, Fan
  #define FAN_PIN        MOSFET_B_PIN
  #define FAN1_PIN       MOSFET_C_PIN
#elif ENABLED(IS_SF)                           // Spindle, Fan
  #define FAN_PIN        MOSFET_C_PIN
#endif

#ifndef FAN_PIN
  #define FAN_PIN MOSFET_D_PIN
#endif

//
// Extruder Auto Fan Pins
//
#define EXTRUDER_0_AUTO_FAN  7 // H4
#define EXTRUDER_1_AUTO_FAN 12 // B6

//
// Misc. Functions
//
#define LED_PIN             13 // B7
#define CUTOFF_RESET_PIN    16 // H1
#define CUTOFF_TEST_PIN     17 // H0

//
// LCD / Controller
//
// Replicator uses a 3-wire SR controller with HD44780
// For now, pretend it's the SAV
//
#define SAV_3DLCD
#define SR_DATA_PIN         34 // C3
#define SR_CLK_PIN          35 // C2
#define SR_STROBE_PIN       33 // C4

#define BTN_UP              75 // J4
#define BTN_DOWN            73 // J3
#define BTN_LEFT            72 // J2
#define BTN_RIGHT           14 // J1
#define BTN_CENTER          15 // J0
#define BTN_ENC             BTN_CENTER

#define BEEPER_PIN           4 // G5

#define STAT_LED_RED        32 // C5
#define STAT_LED_BLUE       31 // C6 (Actually green)

//
// SD Card
//
#define SDSS                53 // B0
#define SD_DETECT_PIN       30 // C7

