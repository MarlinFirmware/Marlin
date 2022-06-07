/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if ENABLED(BAUD_RATE_GCODE)

#include "../gcode.h"

/**
 * M575 - Change serial baud rate
 *
 *   P<index>    - Serial port index. Omit for all.
 *   B<baudrate> - Baud rate (bits per second)
 */
void GcodeSuite::M575() {
  int32_t baud = parser.ulongval('B');
  switch (baud) {
    case   24:
    case   96:
    case  192:
    case  384:
    case  576:
    case 1152: baud *= 100; break;
    case  250:
    case  500: baud *= 1000; break;
    case   19: baud = 19200; break;
    case   38: baud = 38400; break;
    case   57: baud = 57600; break;
    case  115: baud = 115200; break;
  }
  switch (baud) {
    case 2400: case 9600: case 19200: case 38400: case 57600:
    case 115200: case 250000: case 500000: case 1000000: {
      const int8_t port = parser.intval('P', -99);
      const bool set1 = (port == -99 || port == 0);
      if (set1) SERIAL_ECHO_MSG(" Serial ", AS_DIGIT(0), " baud rate set to ", baud);
      #if HAS_MULTI_SERIAL
        const bool set2 = (port == -99 || port == 1);
        if (set2) SERIAL_ECHO_MSG(" Serial ", AS_DIGIT(1), " baud rate set to ", baud);
        #ifdef SERIAL_PORT_3
          const bool set3 = (port == -99 || port == 2);
          if (set3) SERIAL_ECHO_MSG(" Serial ", AS_DIGIT(2), " baud rate set to ", baud);
        #endif
      #endif

      SERIAL_FLUSH();

      if (set1) { MYSERIAL1.end(); MYSERIAL1.begin(baud); }
      #if HAS_MULTI_SERIAL
        if (set2) { MYSERIAL2.end(); MYSERIAL2.begin(baud); }
        #ifdef SERIAL_PORT_3
          if (set3) { MYSERIAL3.end(); MYSERIAL3.begin(baud); }
        #endif
      #endif

    } break;
    default: SERIAL_ECHO_MSG("?(B)aud rate implausible.");
  }
}

#endif // BAUD_RATE_GCODE
