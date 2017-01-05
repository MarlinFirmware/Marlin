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
 * MKS BASE 1.0 – Arduino Mega2560 with RAMPS v1.4 pin assignments
 */

#if HOTENDS > 2
  #error "MKS BASE 1.0 supports up to 2 hotends. Comment this line to keep going."
#endif

#define BOARD_NAME "MKS BASE 1.0"

//
// Heaters / Fans
//
// Power outputs EFBF or EFBE
#define MOSFET_D_PIN 7

#if !PIN_EXISTS(CASE_LIGHT)         // doesn't already exist so OK to change the definition coming
  #define OK_TO_CHANGE_CASE_LIGHT   // in from from the include file
#endif

#include "pins_RAMPS.h"

// added 1 DEC
#undef  SPINDLE_SPEED_PIN      //definitions in pins_ramps.h are not valid with this board
#undef  SPINDLE_ENABLE_PIN
#undef  SPINDLE_DIR_PIN

#if ENABLED(SPINDLE_ENABLE)  // use max endstops
  #undef  X_MAX_PIN
  #undef  Y_MAX_PIN
  #undef  Z_MAX_PIN
  #define SPINDLE_SPEED_PIN      2
  #define SPINDLE_ENABLE_PIN    15  // should have a pullup resistor on this pin
  #define SPINDLE_DIR_PIN       19
#endif

// misc
#if ENABLED(OK_TO_CHANGE_CASE_LIGHT)
  #undef CASE_LIGHT_PIN     // needs a hardware PWM
  #if !ENABLED(SPINDLE_ENABLE)  // use max endstops
    #undef  X_MAX_PIN
    #define CASE_LIGHT_PIN         2
  #endif
#endif
