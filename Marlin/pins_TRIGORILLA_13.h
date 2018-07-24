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
 * Arduino Mega with RAMPS v1.3 for Anycubic
 */

#ifndef BOARD_NAME
  #define BOARD_NAME "Anycubic RAMPS 1.3"
#endif

#define IS_RAMPS_EFB

// FAN0 / D9  - Typically used for the part fan on Anycubic Delta devices
#define FAN_PIN 9

// FAN1 / D7  - Typically unused, can be allocated as Case Fan

// FAN2 / D44 - Typical Extruder Fan on Anycubic Delta devices
#define ORIG_E0_AUTO_FAN_PIN  44

#include "pins_RAMPS_13.h"

#undef E1_STEP_PIN
#undef E1_DIR_PIN
#undef E1_ENABLE_PIN
#undef E1_CS_PIN


// TODO
// when running a comparison between 1.1.0-RC8 and the Anycubic patched sources
// The following differences were highlighted
// However I don't currently have the correct display's to test these changes.

//#if ENABLED(ULTRA_LCD)
//
//  // LCD Display output pins
//  #if ENABLED(NEWPANEL) && ENABLED(PANEL_ONE)
//    #undef LCD_PINS_D6
//    #define LCD_PINS_D6 57
//  #endif

//  // LCD Display input pins
//  #if ENABLED(NEWPANEL)
//    #if ENABLED(VIKI2) || ENABLED(miniVIKI)
//      #undef DOGLCD_A0
//      #define DOGLCD_A0        23
//    #elif ENABLED(ELB_FULL_GRAPHIC_CONTROLLER)
//      #undef BEEPER_PIN
//	    #define BEEPER_PIN 33
//      #undef LCD_BACKLIGHT_PIN
//      #define LCD_BACKLIGHT_PIN 67
//    #endif
//  #elif ENABLED(MINIPANEL)
//    #undef BEEPER_PIN
//    #define BEEPER_PIN 33
//    #undef DOGLCD_A0
//    #define DOGLCD_A0  42
//  #endif

//#endif // ULTRA_LCD
