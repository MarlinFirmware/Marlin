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
 * VERTEX NANO Arduino Mega with RAMPS EFB v1.4 pin assignments.
 * ATmega2560, ATmega1280
 */

#if HAS_MULTI_HOTEND
  #error "K8600 only supports 1 hotend / E stepper."
#endif

#define BOARD_INFO_NAME      "K8600"
#define DEFAULT_MACHINE_NAME "Vertex Nano"

//
// Limit Switches
//
#define X_MIN_PIN                             PinE5
#define Y_MAX_PIN                             PinJ1
#define Z_MAX_PIN                             PinD3
#define Z_MIN_PIN                             -1

//
// Steppers
//
#define Z_ENABLE_PIN                          PinK1

//
// Heaters / Fans
//
#define FAN_PIN                               PinH5

//
// Misc. Functions
//
#define SDSS                                  PinA3
#define CASE_LIGHT_PIN                        PinH4

//
// Other RAMPS pins
//
#include "pins_RAMPS.h"

//
// Heaters / Fans
//
#undef HEATER_BED_PIN

//
// LCD / Controller
//
#if HAS_WIRED_LCD && IS_NEWPANEL
  #undef BEEPER_PIN

  #undef LCD_PINS_RS
  #undef LCD_PINS_ENABLE
  #undef LCD_PINS_D4
  #undef LCD_PINS_D5
  #undef LCD_PINS_D6
  #undef LCD_PINS_D7
  #define LCD_PINS_RS                         PinA5
  #define LCD_PINS_ENABLE                     PinA7
  #define LCD_PINS_D4                         PinC0
  #define LCD_PINS_D5                         PinC2
  #define LCD_PINS_D6                         PinC4
  #define LCD_PINS_D7                         PinC6

  // Buttons
  #undef BTN_EN1
  #undef BTN_EN2
  #undef BTN_ENC
  #define BTN_EN1                             PinH0
  #define BTN_EN2                             PinH1
  #define BTN_ENC                             PinA1

#else

  #define BEEPER_PIN                          PinC4

#endif
