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
 * AZTEEG_X3_PRO (Arduino Mega) pin assignments
 * ATmega2560
 */

#define REQUIRE_MEGA2560
#include "env_validate.h"

#if HOTENDS > 5 || E_STEPPERS > 5
  #error "Azteeg X3 Pro supports up to 5 hotends / E steppers."
#endif

#define BOARD_INFO_NAME "Azteeg X3 Pro"

//
// RAMPS pins overrides
//

//
// Servos
//
// Tested this pin with bed leveling on a Delta with 1 servo.
// Physical wire attachment on EXT1: GND, 5V, D47.
//
#define SERVO0_PIN                            PinL2

//
// Limit Switches
//
#define X_STOP_PIN                            PinE5
#define Y_STOP_PIN                            PinJ1
#define Z_STOP_PIN                            PinD3

#ifndef FAN_PIN
  #define FAN_PIN                             PinH3
#endif

#if ENABLED(CASE_LIGHT_ENABLE) && !PIN_EXISTS(CASE_LIGHT)
  #define CASE_LIGHT_PIN                      PinL5
#endif

//
// Import RAMPS 1.4 pins
//
#include "pins_RAMPS.h"

// DIGIPOT slave addresses
#ifndef DIGIPOT_I2C_ADDRESS_A
  #define DIGIPOT_I2C_ADDRESS_A             0x2C  // unshifted slave address for first DIGIPOT 0x2C (0x58 <- 0x2C << 1)
#endif
#ifndef DIGIPOT_I2C_ADDRESS_B
  #define DIGIPOT_I2C_ADDRESS_B             0x2E  // unshifted slave address for second DIGIPOT 0x2E (0x5C <- 0x2E << 1)
#endif

//
// Z Probe (when not Z_MIN_PIN)
//
#ifndef Z_MIN_PROBE_PIN
  #define Z_MIN_PROBE_PIN                     PinD3
#endif

//
// Steppers
//
#define E2_STEP_PIN                           PinA1
#define E2_DIR_PIN                            PinA3
#define E2_ENABLE_PIN                         PinG1

#define E3_STEP_PIN                           PinA5
#define E3_DIR_PIN                            PinA7
#define E3_ENABLE_PIN                         PinG0

#define E4_STEP_PIN                           PinL6
#define E4_DIR_PIN                            PinC0
#define E4_ENABLE_PIN                         PinL7

//
// Temperature Sensors
//
#define TEMP_2_PIN                            PinB6  // Analog Input
#define TEMP_3_PIN                            PinB5  // Analog Input
#define TEMP_4_PIN                            PinB4  // Analog Input
#define TC1                                   PinG5  // Analog Input (Thermo couple on Azteeg X3Pro)
#define TC2                                   PinE3  // Analog Input (Thermo couple on Azteeg X3Pro)

//
// Heaters / Fans
//
#define HEATER_2_PIN                          PinH1
#define HEATER_3_PIN                          PinH0
#define HEATER_4_PIN                          PinG5
#define HEATER_5_PIN                          PinE3
#define HEATER_6_PIN                          PinH3
#define HEATER_7_PIN                          PinB5

#ifndef CONTROLLER_FAN_PIN
  #define CONTROLLER_FAN_PIN                  PinG5  // Pin used for the fan to cool motherboard (-1 to disable)
#endif

//
// Auto fans
//
#define AUTO_FAN_PIN                          PinE3
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E2_AUTO_FAN_PIN
  #define E2_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif
#ifndef E3_AUTO_FAN_PIN
  #define E3_AUTO_FAN_PIN           AUTO_FAN_PIN
#endif

//
// LCD / Controller
//
#undef BEEPER_PIN
#define BEEPER_PIN                            PinC4

#if EITHER(VIKI2, miniVIKI)
  #undef SD_DETECT_PIN
  #define SD_DETECT_PIN                       PinL0  // For easy adapter board
  #undef BEEPER_PIN
  #define BEEPER_PIN                          PinB6  // 33 isn't physically available to the LCD display
#else
  #define STAT_LED_RED_PIN                    PinC5
  #define STAT_LED_BLUE_PIN                   PinC2
#endif

//
// Misc. Functions
//
#if ENABLED(CASE_LIGHT_ENABLE) && PIN_EXISTS(CASE_LIGHT) && defined(DOGLCD_A0) && DOGLCD_A0 == CASE_LIGHT_PIN
  #undef DOGLCD_A0                                // Steal pin 44 for the case light; if you have a Viki2 and have connected it
  #define DOGLCD_A0                           PinF3  // following the Panucatt wiring diagram, you may need to tweak these pin assignments
                                                  // as the wiring diagram uses pin 44 for DOGLCD_A0.
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#undef SPINDLE_LASER_PWM_PIN                      // Definitions in pins_RAMPS.h are no good with the AzteegX3pro board
#undef SPINDLE_LASER_ENA_PIN
#undef SPINDLE_DIR_PIN

#if HAS_CUTTER                                    // EXP2 header
  #if EITHER(VIKI2, miniVIKI)
    #define BTN_EN2                           PinC6  // Pin 7 needed for Spindle PWM
  #endif
  #define SPINDLE_LASER_PWM_PIN               PinH4  // Hardware PWM
  #define SPINDLE_LASER_ENA_PIN               PinD1  // Pullup!
  #define SPINDLE_DIR_PIN                     PinD0
#endif
