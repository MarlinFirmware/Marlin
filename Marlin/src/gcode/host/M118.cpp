/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../gcode.h"

/**
 * M118: Display a message in the host console.
 *
 *  A1  Prepend '// ' for an action command, as in OctoPrint
 *  E1  Have the host 'echo:' the text
 */
void GcodeSuite::M118() {
  bool hasE = false, hasA = false;
  char *p = parser.string_arg;
  for (uint8_t i = 2; i--;)
    if ((p[0] == 'A' || p[0] == 'E') && p[1] == '1') {
      if (p[0] == 'A') hasA = true;
      if (p[0] == 'E') hasE = true;
      p += 2;
      while (*p == ' ') ++p;
    }
  if (hasE) SERIAL_ECHO_START();
  if (hasA) SERIAL_ECHOPGM("// ");
  SERIAL_ECHOLN(p);
}
