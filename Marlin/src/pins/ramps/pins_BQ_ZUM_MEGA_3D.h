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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * bq ZUM Mega 3D board definition
 */

#ifndef __AVR_ATmega2560__
  #error "Oops! Select 'Arduino/Genuino Mega or Mega 2560' in 'Tools > Board.'"
#endif

#define BOARD_INFO_NAME "ZUM Mega 3D"

//
// Heaters / Fans
//
#define RAMPS_D8_PIN                          10
#define RAMPS_D9_PIN                          12
#define RAMPS_D10_PIN                          9
#define MOSFET_D_PIN                           7

//
// Auto fans
//
#ifndef E0_AUTO_FAN_PIN
  #define E0_AUTO_FAN_PIN                     11
#endif
#ifndef E1_AUTO_FAN_PIN
  #define E1_AUTO_FAN_PIN                      6
#endif
#ifndef E2_AUTO_FAN_PIN
  #define E2_AUTO_FAN_PIN                      6
#endif
#ifndef E3_AUTO_FAN_PIN
  #define E3_AUTO_FAN_PIN                      6
#endif

//
// M3/M4/M5 - Spindle/Laser Control
//
#define SPINDLE_LASER_ENA_PIN                 40  // Pullup or pulldown!
#define SPINDLE_LASER_PWM_PIN                 44  // Hardware PWM
#define SPINDLE_DIR_PIN                       42

//
// Limit Switches
//
#define X_MAX_PIN                             79  // 2

//
// Import RAMPS 1.3 pins
//
#include "pins_RAMPS_13.h"

//
// Z Probe (when not Z_MIN_PIN)
//
#undef Z_MIN_PROBE_PIN
#define Z_MIN_PROBE_PIN                       19  // IND_S_5V

#undef Z_ENABLE_PIN
#define Z_ENABLE_PIN                          77  // 62

//
// Steppers
//
#define DIGIPOTSS_PIN                         22
#define DIGIPOT_CHANNELS   { 4, 5, 3, 0, 1 }

//
// Temperature Sensors
//
#undef TEMP_1_PIN
#define TEMP_1_PIN                            14  // Analog Input (15)

#undef TEMP_BED_PIN
#define TEMP_BED_PIN                          15  // Analog Input (14)

//
// Misc. Functions
//
#undef PS_ON_PIN                                  // 12
#define PS_ON_PIN                             81  // External Power Supply

#ifndef CASE_LIGHT_PIN
  #define CASE_LIGHT_PIN                      44  // Hardware PWM
#endif

// This board has headers for Z-min, Z-max and IND_S_5V *but* as the bq team
// decided to ship the printer only with the probe and no additional Z-min
// endstop and the instruction manual advises the user to connect the probe to
// IND_S_5V the option Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN will not work.
#ifdef Z_MIN_PROBE_USES_Z_MIN_ENDSTOP_PIN
  #undef Z_MIN_PIN
  #undef Z_MAX_PIN
  #define Z_MIN_PIN                           19  // IND_S_5V
  #define Z_MAX_PIN                           18  // Z-MIN Label
#endif

//
// Used by the Hephestos 2 heated bed upgrade kit
//
#if ENABLED(HEPHESTOS2_HEATED_BED_KIT)
  #undef HEATER_BED_PIN
  #define HEATER_BED_PIN                       8
#endif
