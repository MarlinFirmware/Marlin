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
 *  Sainsmart 2-in-1 pin assignments
 */

#if HOTENDS > 2
  #error "Sainsmart 2-in-1 supports up to 2 hotends. Comment this line to keep going."
#endif

#define BOARD_NAME "Sainsmart"

#define RAMPS_D10_PIN 9 // E
#define RAMPS_D9_PIN  7 // F PART FAN in front of board next to Extruder heat
     // RAMPS_D8_PIN  8 // B
#define MOSFET_D_PIN 10 // F / E

#include "pins_RAMPS.h"
