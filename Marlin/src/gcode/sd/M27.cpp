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

#if HAS_MEDIA

#include "../gcode.h"
#include "../../sd/cardreader.h"

/**
 * M27: Get SD Card status
 *
 * Parameters:
 *   None  Report the current SD read position
 *   C     Report the filename and long filename of the current file
 *
 *   With AUTO_REPORT_SD_STATUS:
 *     S<seconds>  Interval between auto-reports. S0 to disable
 */
void GcodeSuite::M27() {
  if (parser.seen_test('C')) {
    SERIAL_ECHOPGM("Current file: ");
    card.printSelectedFilename();
    return;
  }

  #if ENABLED(AUTO_REPORT_SD_STATUS)
    if (parser.seenval('S')) {
      card.auto_reporter.set_interval(parser.value_byte());
      return;
    }
  #endif

  card.report_status();
}

#endif // HAS_MEDIA
