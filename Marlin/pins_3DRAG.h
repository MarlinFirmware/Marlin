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
 * 3DRAG (and K8200 / K8400) Arduino Mega with RAMPS v1.4 pin assignments
 */

#ifndef BOARD_NAME
  #define BOARD_NAME "3Drag"
#endif

#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "3Drag"
#endif

#ifndef DEFAULT_SOURCE_CODE_URL
  #define DEFAULT_SOURCE_CODE_URL "http://3dprint.elettronicain.it/"
#endif

//
// Heaters / Fans
//
#define RAMPS_D8_PIN 9
#define RAMPS_D9_PIN 8
#define MOSFET_D_PIN 12

#include "pins_RAMPS.h"

//
// Limit Switches
//
#undef Z_MAX_PIN
#define Z_MAX_PIN          -1

//
// Steppers
//
#undef Z_ENABLE_PIN
#define Z_ENABLE_PIN       63

//
// Heaters / Fans
//
#define HEATER_2_PIN        6

//
// Misc. Functions
//
#undef SDSS
#define SDSS               25

#undef SD_DETECT_PIN
#define SD_DETECT_PIN      53

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)
  #undef BEEPER_PIN
  #define BEEPER_PIN       -1

  #undef LCD_PINS_RS
  #undef LCD_PINS_ENABLE
  #undef LCD_PINS_D4
  #undef LCD_PINS_D5
  #undef LCD_PINS_D6
  #undef LCD_PINS_D7
  #define LCD_PINS_RS      27
  #define LCD_PINS_ENABLE  29
  #define LCD_PINS_D4      37
  #define LCD_PINS_D5      35
  #define LCD_PINS_D6      33
  #define LCD_PINS_D7      31

  // Buttons
  #undef BTN_EN1
  #undef BTN_EN2
  #undef BTN_ENC
  #define BTN_EN1          16
  #define BTN_EN2          17
  #define BTN_ENC          23

#else

  #define BEEPER_PIN       33

#endif // ULTRA_LCD && NEWPANEL
