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
 * bq ZUM Mega 3D board definition
 */

#ifndef __AVR_ATmega2560__
  #error "Oops! Make sure you have 'Arduino Mega 2560' selected from the 'Tools -> Boards' menu."
#endif

#define BOARD_NAME "ZUM Mega 3D"

#define RAMPS_D8_PIN  10
#define RAMPS_D9_PIN  12
#define RAMPS_D10_PIN  9
#define MOSFET_D_PIN   7

#include "pins_RAMPS_13.h"

#undef X_MAX_PIN
#define X_MAX_PIN         79 //  2

#undef Z_MIN_PROBE_PIN
#define Z_MIN_PROBE_PIN   19 // IND_S_5V

#undef Z_ENABLE_PIN
#define Z_ENABLE_PIN      77 // 62

#undef TEMP_1_PIN
#define TEMP_1_PIN        14 // 15

#undef TEMP_BED_PIN
#define TEMP_BED_PIN      15 // 14

#define DIGIPOTSS_PIN     22
#define DIGIPOT_CHANNELS  { 4, 5, 3, 0, 1 }

#undef PS_ON_PIN             // 12
#define PS_ON_PIN         81 // External Power Supply
