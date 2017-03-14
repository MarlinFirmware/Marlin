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
 * AZTEEG_X3 Arduino Mega with RAMPS v1.4 pin assignments
 */

#if HOTENDS > 2
  #error "Azteeg X3 supports up to 2 hotends. Comment this line to keep going."
#endif

#define BOARD_NAME "Azteeg X3"

#include "pins_RAMPS_13.h"

#if ENABLED(VIKI2) || ENABLED(miniVIKI)

  #undef DOGLCD_A0
  #undef DOGLCD_CS
  #undef BTN_ENC
  #define DOGLCD_A0         31
  #define DOGLCD_CS         32
  #define BTN_ENC           12

  #if ENABLED(TEMP_STAT_LEDS)
    #undef STAT_LED_RED
    #undef STAT_LED_BLUE
    #define STAT_LED_RED    64
    #define STAT_LED_BLUE   63
  #endif

#elif ENABLED(TEMP_STAT_LEDS)

  #define STAT_LED_RED       6
  #define STAT_LED_BLUE     11

#endif
