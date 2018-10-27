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
 * Arduino Mega with RAMPS v1.4 for Anycubic
 */

#ifndef BOARD_NAME
  #define BOARD_NAME "Anycubic RAMPS 1.4"
#endif

// Remap MOSFET pins to common usages
#if HOTENDS > 1
  #define RAMPS_D9_PIN     45   // EEB, EEF
  #if !TEMP_SENSOR_BED
    #define RAMPS_D8_PIN    9   // EEF
  #endif
#elif TEMP_SENSOR_BED
  #define FAN1_PIN          7   // EFB
#else
  #define FAN2_PIN         44   // EFF
#endif

// D44 - Typical Extruder Fan on Anycubic Delta devices
#define ORIG_E0_AUTO_FAN_PIN 44

#include "pins_RAMPS.h"

//
// AnyCubic made the following changes to 1.1.0-RC8
// If these are appropriate for your LCD let us know.
//
#if 0 && ENABLED(ULTRA_LCD)

  // LCD Display output pins
  #if ENABLED(NEWPANEL) && ENABLED(PANEL_ONE)
    #undef LCD_PINS_D6
    #define LCD_PINS_D6    57
  #endif

  // LCD Display input pins
  #if ENABLED(NEWPANEL)
    #if ENABLED(VIKI2) || ENABLED(miniVIKI)
      #undef DOGLCD_A0
      #define DOGLCD_A0    23
    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
      #undef BEEPER_PIN
      #define BEEPER_PIN   33
      #undef LCD_BACKLIGHT_PIN
      #define LCD_BACKLIGHT_PIN 67
    #endif
  #elif ENABLED(MINIPANEL)
    #undef BEEPER_PIN
    #define BEEPER_PIN     33
    #undef DOGLCD_A0
    #define DOGLCD_A0      42
  #endif

#endif // ULTRA_LCD
