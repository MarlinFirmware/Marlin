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
 *  F         - Fill the matrix (set all bits)
 *  P         - Dump the led_line[] array values
 *  C<column> - Set a column to the 8-bit value V
 *  R<row>    - Set a row to the 8-bit value V
 *  X<pos>    - X position of an LED to set or toggle
 *  Y<pos>    - Y position of an LED to set or toggle
 *  V<value>  - The potentially 32-bit value or on/off state to set
 *              (for example: a chain of 4 Max7219 devices can have 32 bit
 *               rows or columns depending upon rotation)
 */
void GcodeSuite::M7219() {
  if (parser.seen('I')) {
    max7219.register_setup();
    max7219.clear();
  }

  if (parser.seen('F')) max7219.fill();

  const uint32_t v = parser.ulongval('V');

  if (parser.seenval('R')) {
    const uint8_t r = parser.value_byte();
    max7219.set_row(r, v);
  }
  else if (parser.seenval('C')) {
    const uint8_t c = parser.value_byte();
    max7219.set_column(c, v);
  }
  else if (parser.seenval('X') || parser.seenval('Y')) {
    const uint8_t x = parser.byteval('X'), y = parser.byteval('Y');
    if (parser.seenval('V'))
      max7219.led_set(x, y, parser.boolval('V'));
    else
      max7219.led_toggle(x, y);
  }
  else if (parser.seen('D')) {
    const uint8_t line = parser.byteval('D') + (parser.byteval('U') << 3);
    if (line < MAX7219_LINES) {
      max7219.led_line[line] = v;
      return max7219.refresh_line(line);
    }
  }

  if (parser.seen('P')) {
    for (uint8_t r = 0; r < MAX7219_LINES; r++) {
      SERIAL_ECHOPGM("led_line[");
      if (r < 10) SERIAL_CHAR(' ');
      SERIAL_ECHO(int(r));
      SERIAL_ECHO("]=");
      for (uint8_t b = 8; b--;) SERIAL_CHAR('0' + TEST(max7219.led_line[r], b));
      SERIAL_EOL();
    }
  }
}

#endif // MAX7219_GCODE
