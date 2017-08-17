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
 * 3DRAG (and K8200) Arduino Mega with RAMPS v1.4 pin assignments
 */

#include "pins_RAMPS_14.h"

#define DEFAULT_MACHINE_NAME    "3Drag"
#define DEFAULT_SOURCE_CODE_URL "http://3dprint.elettronicain.it/"

#undef Z_ENABLE_PIN
#define Z_ENABLE_PIN       63

#undef X_MAX_PIN
#undef Y_MAX_PIN
#undef Z_MAX_PIN
#define X_MAX_PIN          2
#define Y_MAX_PIN          15
#define Z_MAX_PIN          -1

#undef SDSS
#define SDSS               25//53

#undef FAN_PIN
#define FAN_PIN            8

#undef HEATER_1_PIN
#undef HEATER_2_PIN
#undef HEATER_BED_PIN
#define HEATER_0_PIN       10
#define HEATER_1_PIN       12
#define HEATER_2_PIN       6

#define HEATER_BED_PIN     9    // BED

#if ENABLED(ULTRA_LCD) && ENABLED(NEWPANEL)
  #undef BEEPER_PIN
  #define BEEPER_PIN -1

  #undef LCD_PINS_RS
  #undef LCD_PINS_ENABLE
  #undef LCD_PINS_D4
  #undef LCD_PINS_D5
  #undef LCD_PINS_D6
  #undef LCD_PINS_D7
  #define LCD_PINS_RS 27
  #define LCD_PINS_ENABLE 29
  #define LCD_PINS_D4 37
  #define LCD_PINS_D5 35
  #define LCD_PINS_D6 33
  #define LCD_PINS_D7 31

  // Buttons
  #undef BTN_EN1
  #undef BTN_EN2
  #undef BTN_ENC
  #define BTN_EN1 16
  #define BTN_EN2 17
  #define BTN_ENC 23 //the click

#else

  #define BEEPER_PIN 33

#endif // ULTRA_LCD && NEWPANEL
