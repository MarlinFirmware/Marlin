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

#if BOTH(SDCARD_SORT_ALPHA, SDSORT_GCODE)

#include "../gcode.h"
#include "../../sd/cardreader.h"

/**
 * M34: Set SD Card Sorting Options
 */
void GcodeSuite::M34() {
  if (parser.seen('S')) card.setSortOn(parser.value_bool());
  if (parser.seenval('F')) {
    const int v = parser.value_long();
    card.setSortFolders(v < 0 ? -1 : v > 0 ? 1 : 0);
  }
  //if (parser.seen('R')) card.setSortReverse(parser.value_bool());
}

#endif // SDCARD_SORT_ALPHA && SDSORT_GCODE
