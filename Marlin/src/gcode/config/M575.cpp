/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../../inc/MarlinConfig.h"
#include "../gcode.h"

/**
 * M575 - set hotend offset (in linear units)
 *
 *   P<Serial channel number>
 *   B<baudrate>
 */
void GcodeSuite::M575() {
  long baudrate = 0;
  int serialN = -99;
  if (parser.seenval('P')) serialN = parser.value_int();
  if (parser.seenval('B')) baudrate = parser.value_long();
  if (baudrate != 0) {
    #if NUM_SERIAL > 0
      SERIAL_ECHO_START();
      SERIAL_ECHOPAIR("New baudrate for serial: ", serialN);
      SERIAL_ECHOLNPAIR(" is: ", baudrate);
      if ( (serialN == -99) || (serialN == 0)) {
        MYSERIAL0.end();
        MYSERIAL0.begin(baudrate);
      }
      #if NUM_SERIAL > 1
        if ((serialN == -99) || (serialN == 1)) {
          MYSERIAL1.end();
          MYSERIAL1.begin(baudrate);
        }
      #endif
    #endif
  }
}
