/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * AZTEEG_X3_PRO (Arduino Mega) pin assignments
 */

#ifndef __AVR_ATmega2560__
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

#if HOTENDS > 5 || E_STEPPERS > 5
  #error "Azteeg X3 Pro supports up to 5 hotends / E-steppers. Comment out this line to continue."
#endif

#define BOARD_NAME "Azteeg X3 Pro"

//
// RAMPS pins overrides
//

//
// Servos
//
// Tested this pin with bed leveling on a Delta with 1 servo.
// Physical wire attachment on EXT1: GND, 5V, D47.
//
#define SERVO0_PIN         47

//
// Limit Switches
//
#define X_STOP_PIN          3
#define Y_STOP_PIN         14
#define Z_STOP_PIN         18

#ifndef FAN_PIN
  #define FAN_PIN           6
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !PIN_EXISTS(CASE_LIGHT)
  #define CASE_LIGHT_PIN   44
#endif

//
// Import RAMPS 1.4 pins
//
#include "pins_RAMPS.h"

// DIGIPOT slave addresses
#ifndef DIGIPOT_I2C_ADDRESS_A
  #define DIGIPOT_I2C_ADDRESS_A 0x2C   // unshifted slave address for first DIGIPOT 0x2C (0x58 <- 0x2C << 1)
#endif
#ifndef DIGIPOT_I2C_ADDRESS_B
  #define DIGIPOT_I2C_ADDRESS_B 0x2E   // unshifted slave address for second DIGIPOT 0x2E (0x5C <- 0x2E << 1)
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN  18
#endif

//
// Steppers
//
#define E2_STEP_PIN        23
#define E2_DIR_PIN         25
#define E2_ENABLE_PIN      40

#define E3_STEP_PIN        27
#define E3_DIR_PIN         29
#define E3_ENABLE_PIN      41

#define E4_STEP_PIN        43
#define E4_DIR_PIN         37
#define E4_ENABLE_PIN      42

//
// Temperature Sensors
//
#define TEMP_2_PIN         12   // Analog Input
#define TEMP_3_PIN         11   // Analog Input
#define TEMP_4_PIN         10   // Analog Input
#define TC1                 4   // Analog Input (Thermo couple on Azteeg X3Pro)
#define TC2                 5   // Analog Input (Thermo couple on Azteeg X3Pro)

//
// Heaters / Fans
//
#define HEATER_2_PIN       16
#define HEATER_3_PIN       17
#define HEATER_4_PIN        4
#define HEATER_5_PIN        5
#define HEATER_6_PIN        6
#define HEATER_7_PIN       11

#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN 4   // Pin used for the fan to cool motherboard (-1 to disable)
#endif

// Fans/Water Pump to cool the hotend cool side.
#define ORIG_E0_AUTO_FAN_PIN 5
#define ORIG_E1_AUTO_FAN_PIN 5
#define ORIG_E2_AUTO_FAN_PIN 5
#define ORIG_E3_AUTO_FAN_PIN 5

//
// LCD / Controller
//
#undef BEEPER_PIN
#define BEEPER_PIN         33

#if ANY(VIKI2, miniVIKI)
  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN    49   // For easy adapter board
  #undef BEEPER_PIN
  #define  BEEPER_PIN      12   // 33 isn't physically available to the LCD display
#else
  #define STAT_LED_RED_PIN  32
  #define STAT_LED_BLUE_PIN 35
#endif

//
// Misc. Functions
//
#if ENABLED(CASE_LIGHT_ENABLE) && PIN_EXISTS(CASE_LIGHT) && defined(DOGLCD_A0) && DOGLCD_A0 == CASE_LIGHT_PIN
  #undef DOGLCD_A0              // Steal pin 44 for the case light; if you have a Viki2 and have connected it
  #define DOGLCD_A0        57   // following the Panucatt wiring diagram, you may need to tweak these pin assignments
                                // as the wiring diagram uses pin 44 for DOGLCD_A0
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#undef SPINDLE_LASER_PWM_PIN    // Definitions in pins_RAMPS.h are no good with the AzteegX3pro board
#undef SPINDLE_LASER_ENA_PIN
#undef SPINDLE_DIR_PIN

#if ENABLED(SPINDLE_LASER_ENABLE)   // EXP2 header
  #if ANY(VIKI2, miniVIKI)
    #undef BTN_EN2
    #define BTN_EN2             31   // need 7 for the spindle speed PWM
  #endif
  #define SPINDLE_LASER_PWM_PIN     7   // must have a hardware PWM
  #define SPINDLE_LASER_ENA_PIN    20   // Pin should have a pullup!
  #define SPINDLE_DIR_PIN          21
#endif
