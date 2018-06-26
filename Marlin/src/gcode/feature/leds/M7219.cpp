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

#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(MAX7219_GCODE)

#include "../../gcode.h"
#include "../../../feature/Max7219_Debug_LEDs.h"

/**
 * M7219: Control the Max7219 LED matrix
 * 
 *  I         - Initialize (clear) the matrix
 *  C<column> - Set a column to the 8-bit value V
 *  R<row>    - Set a row to the 8-bit value V
 *  X<pos>    - X position of an LED to set or toggle
 *  Y<pos>    - Y position of an LED to set or toggle
 *  V<value>  - The 8-bit value or on/off state to set
 */
void GcodeSuite::M7219() {
  if (parser.seen('I'))
    Max7219_Clear();
  else if (parser.seenval('R')) {
    const uint8_t r = parser.value_int();
    Max7219_Set_Row(r, parser.byteval('V'));
  }
  else if (parser.seenval('C')) {
    const uint8_t c = parser.value_int();
    Max7219_Set_Column(c, parser.byteval('V'));
  }
  else if (parser.seenval('X') || parser.seenval('Y')) {
    const uint8_t x = parser.byteval('X'), y = parser.byteval('Y');
    if (parser.seenval('V'))
      Max7219_LED_Set(x, y, parser.boolval('V'));
    else
      Max7219_LED_Toggle(x, y);
  }
}

#endif // MAX7219_GCODE
