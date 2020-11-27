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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * VERTEX NANO Arduino Mega with RAMPS EFB v1.4 pin assignments.
 */

#if HOTENDS > 1
  #error "Only 1 hotend is supported for Vertex Nano."
#endif

#define BOARD_INFO_NAME      "K8600"
#define DEFAULT_MACHINE_NAME "Vertex Nano"

//
// Limit Switches
//
#define X_MIN_PIN                              3
#define Y_MAX_PIN                             14
#define Z_MAX_PIN                             18
#define Z_MIN_PIN                             -1

//
// Heaters / Fans
//
#define FAN_PIN                                8

//
// Misc. Functions
//
#define CASE_LIGHT_PIN                         7

//
// Other RAMPS pins
//
#define IS_RAMPS_EFB                              // Override autodetection. Bed will be undefined.
#include "pins_RAMPS.h"

//
// Steppers
//
#undef X_STEP_PIN
#undef X_DIR_PIN
#undef X_ENABLE_PIN
#define X_STEP_PIN                            54
#define X_DIR_PIN                             55
#define X_ENABLE_PIN                          38

#undef Y_STEP_PIN
#undef Y_DIR_PIN
#undef Y_ENABLE_PIN
#define Y_STEP_PIN                            60
#define Y_DIR_PIN                             61
#define Y_ENABLE_PIN                          56

#undef Z_ENABLE_PIN
#define Z_ENABLE_PIN                          63

#undef E0_STEP_PIN
#undef E0_DIR_PIN
#undef E0_ENABLE_PIN
#define E0_STEP_PIN                           26
#define E0_DIR_PIN                            28
#define E0_ENABLE_PIN                         24

//
// Heaters / Fans
//
#undef HEATER_BED_PIN

//
// Misc. Functions
//
#undef SDSS
#define SDSS                                  25  // 53

//
// LCD / Controller
//
#if IS_ULTRA_LCD && IS_NEWPANEL
  #undef BEEPER_PIN

  #undef LCD_PINS_RS
  #undef LCD_PINS_ENABLE
  #undef LCD_PINS_D4
  #undef LCD_PINS_D5
  #undef LCD_PINS_D6
  #undef LCD_PINS_D7
  #define LCD_PINS_RS                         27
  #define LCD_PINS_ENABLE                     29
  #define LCD_PINS_D4                         37
  #define LCD_PINS_D5                         35
  #define LCD_PINS_D6                         33
  #define LCD_PINS_D7                         31

  // Buttons
  #undef BTN_EN1
  #undef BTN_EN2
  #undef BTN_ENC
  #define BTN_EN1                             17
  #define BTN_EN2                             16
  #define BTN_ENC                             23

#else

  #define BEEPER_PIN                          33

#endif
