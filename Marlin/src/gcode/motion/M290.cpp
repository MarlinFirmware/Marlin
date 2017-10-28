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

#include "../../inc/MarlinConfig.h"

#if ENABLED(BABYSTEPPING)

#include "../gcode.h"
#include "../../module/probe.h"
#include "../../module/temperature.h"
#include "../../module/planner.h"

/**
 * M290: Babystepping
 */
void GcodeSuite::M290() {
  #if ENABLED(BABYSTEP_XY)
    for (uint8_t a = X_AXIS; a <= Z_AXIS; a++)
      if (parser.seenval(axis_codes[a]) || (a == Z_AXIS && parser.seenval('S'))) {
        float offs = constrain(parser.value_axis_units(a), -2, 2);
        #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
          if (a == Z_AXIS) {
            zprobe_zoffset += offs;
            refresh_zprobe_zoffset(true); // 'true' to not babystep
          }
        #endif
        thermalManager.babystep_axis(a, offs * planner.axis_steps_per_mm[a]);
      }
  #else
    if (parser.seenval('Z') || parser.seenval('S')) {
      float offs = constrain(parser.value_axis_units(Z_AXIS), -2, 2);
      #if ENABLED(BABYSTEP_ZPROBE_OFFSET)
        zprobe_zoffset += offs;
        refresh_zprobe_zoffset(); // This will babystep the axis
      #else
        thermalManager.babystep_axis(Z_AXIS, offs * planner.axis_steps_per_mm[Z_AXIS]);
      #endif
    }
  #endif
}

#endif // BABYSTEPPING
