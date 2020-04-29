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
 * Ortur 4 Arduino Mega based on RAMPS v1.4 pin assignments
 */

#ifndef BOARD_INFO_NAME
  #define BOARD_INFO_NAME "Ortur 4.3"
#endif

#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME BOARD_INFO_NAME
#endif

#ifndef DEFAULT_SOURCE_CODE_URL
  #define DEFAULT_SOURCE_CODE_URL "https://github.com/MarlinFirmware/"
#endif


#include "pins_RAMPS.h"

//
// Limit Switches
//

#undef X_MAX_PIN
#define X_MAX_PIN         18

#undef Z_MIN_PIN
#define Z_MIN_PIN          63

//
// Steppers
//

#undef E0_STEP_PIN
#define E0_STEP_PIN        36
#undef E0_DIR_PIN
#define E0_DIR_PIN         34
#undef E0_ENABLE_PIN
#define E0_ENABLE_PIN      30
#undef E0_CS_PIN
#define E0_CS_PIN        44

#undef E1_STEP_PIN
#define E1_STEP_PIN        26
#undef E1_DIR_PIN
#define E1_DIR_PIN         28
#undef E1_ENABLE_PIN
#define E1_ENABLE_PIN      24
#undef E1_CS_PIN
#define E1_CS_PIN        42

//
// Temperature Sensors
//

#undef TEMP_0_PIN
#define TEMP_0_PIN         15   // Analog Input
#undef TEMP_1_PIN
#define TEMP_1_PIN         13   // Analog Input

//
// Software serial
//

#undef X_SERIAL_TX_PIN
#define X_SERIAL_TX_PIN    59
#undef SERVO0_PIN
#define X_SERIAL_RX_PIN    63
#undef X_SERIAL_RX_PIN

#undef Y_SERIAL_TX_PIN
#define Y_SERIAL_TX_PIN    64
#undef Y_SERIAL_RX_PIN
#define Y_SERIAL_RX_PIN    40

#undef Z_SERIAL_TX_PIN
#define Z_SERIAL_TX_PIN    44
#undef Z_SERIAL_RX_PIN
#define Z_SERIAL_RX_PIN    42

#undef E0_SERIAL_TX_PIN
#define E0_SERIAL_TX_PIN   66
#undef E0_SERIAL_RX_PIN
#define E0_SERIAL_RX_PIN   65

//
// Misc. Functions
//

#undef SERVO0_PIN
#define SERVO0_PIN       29

#undef Z_MIN_PROBE_PIN
#define Z_MIN_PROBE_PIN  2

#undef FIL_RUNOUT_PIN
#define FIL_RUNOUT_PIN    59

//
// LCD / Controller
//

#if ENABLED(REPRAP_DISCOUNT_FULL_GRAPHIC_SMART_CONTROLLER)

  #undef BEEPER_PIN
  #define BEEPER_PIN                           35

  #undef LCD_PINS_RS
  #undef LCD_PINS_ENABLE
  #undef LCD_PINS_D4
  #define LCD_PINS_RS                         27
  #define LCD_PINS_ENABLE                     23
  #define LCD_PINS_D4                         37

  #undef LCD_SDSS
  #undef SD_DETECT_PIN
  #define LCD_SDSS                        53
  #define SD_DETECT_PIN	                  49

  #undef BTN_EN1
  #undef BTN_EN2
  #undef BTN_ENC
  #define BTN_EN1                       29
  #define BTN_EN2                       25
  #define BTN_ENC                       16

#endif