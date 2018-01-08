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

#include "../gcode.h"

/**
 * M118: Display a message in the host console.
 *
 *  A1  Append '// ' for an action command, as in OctoPrint
 *  E1  Have the host 'echo:' the text
 */
void GcodeSuite::M118() {
  if (parser.boolval('E')) SERIAL_ECHO_START();
  if (parser.boolval('A')) SERIAL_ECHOPGM("// ");
  SERIAL_ECHOLN(parser.string_arg);
}
