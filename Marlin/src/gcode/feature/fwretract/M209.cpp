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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(FWRETRACT)

#include "../../../feature/fwretract.h"
#include "../../gcode.h"

/**
 * M209: Enable automatic retract (M209 S1)
 *   For slicers that don't support G10/11, reversed extrude-only
 *   moves will be classified as retraction.
 */
void GcodeSuite::M209() {
  if (MIN_AUTORETRACT <= MAX_AUTORETRACT) {
    if (parser.seen('S')) {
      fwretract.autoretract_enabled = parser.value_bool();
      for (uint8_t i = 0; i < EXTRUDERS; i++) fwretract.retracted[i] = false;
    }
  }
}

#endif // FWRETRACT
