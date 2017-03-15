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
 * BT7272A  3DYMY RAMPS 1.4Plus with LCD2400
 */

#ifndef BOARD_NAME
  #define BOARD_NAME "BT7272A"
#endif

#ifndef DEFAULT_MACHINE_NAME
  #define DEFAULT_MACHINE_NAME "flsun i3"
#endif

#ifndef DEFAULT_SOURCE_CODE_URL
  #define DEFAULT_SOURCE_CODE_URL "https://github.com/RobAltenburg/Marlin"
#endif

//
// Heaters (Swap bed and extruder)
//
#define RAMPS_D8_PIN 10
#define RAMPS_D10_PIN 8

#include "pins_RAMPS.h"

//
// EXTRUDERS
//
#define E1_STEP_PIN        26
#define E1_DIR_PIN         28
#define E1_ENABLE_PIN      24
#define E1_CS_PIN          42
 
#define E0_STEP_PIN        36
#define E0_DIR_PIN         34
#define E0_ENABLE_PIN      30
#define E0_CS_PIN          44

//
// LCD / Controller
//
#if ENABLED(ULTRA_LCD) 

#define LCD_PINS_RS 41
#define LCD_PINS_ENABLE 31
#define LCD_PINS_D4 33
#define LCD_PINS_D5 23
#define LCD_PINS_D6 42
#define LCD_PINS_D7 44

  #if ENABLED(NEWPANEL)

    #if ENABLED(REPRAP_DISCOUNT_SMART_CONTROLLER)

      #define BTN_EN1 25 
      #define BTN_EN2 29 

      #define KILL_PIN 27 
    
    #endif
  #endif // NEWPANEL
#endif // ULTRA_LCD 

