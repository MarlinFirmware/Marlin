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

#include "../../inc/MarlinConfig.h"

#if ENABLED(BACKLASH_GCODE)

#include "../../feature/backlash.h"
#include "../../module/planner.h"

#include "../gcode.h"

/**
 * M425: Enable and tune backlash correction.
 *
 *   F<fraction>     Enable/disable/fade-out backlash correction (0.0 to 1.0)
 *   S<smoothing_mm> Distance over which backlash correction is spread
 *   X<distance_mm>  Set the backlash distance on X (0 to disable)
 *   Y<distance_mm>                        ... on Y
 *   Z<distance_mm>                        ... on Z
 *   X               If a backlash measurement was done on X, copy that value
 *   Y                                              ... on Y
 *   Z                                              ... on Z
 *
 * Type M425 without any arguments to show active values.
 */
void GcodeSuite::M425() {
  bool noArgs = true;

  LOOP_XYZ(i) {
    if (parser.seen(axis_codes[i])) {
      planner.synchronize();
      backlash.distance_mm[i] = parser.has_value() ? parser.value_linear_units() : backlash.get_measurement(i);
      noArgs = false;
    }
  }

  if (parser.seen('F')) {
    planner.synchronize();
    backlash.set_correction(parser.value_float());
    noArgs = false;
  }

  #ifdef BACKLASH_SMOOTHING_MM
    if (parser.seen('S')) {
      planner.synchronize();
      backlash.smoothing_mm = parser.value_linear_units();
      noArgs = false;
    }
  #endif

  if (noArgs) {
    SERIAL_ECHOPGM("Backlash Correction ");
    if (!backlash.correction) SERIAL_ECHOPGM("in");
    SERIAL_ECHOLNPGM("active:");
    SERIAL_ECHOLNPAIR("  Correction Amount/Fade-out:     F", backlash.get_correction(), " (F1.0 = full, F0.0 = none)");
    SERIAL_ECHOPGM("  Backlash Distance (mm):        ");
    LOOP_XYZ(a) {
      SERIAL_CHAR(' ');
      SERIAL_CHAR(axis_codes[a]);
      SERIAL_ECHO(backlash.distance_mm[a]);
      SERIAL_EOL();
    }

    #ifdef BACKLASH_SMOOTHING_MM
      SERIAL_ECHOLNPAIR("  Smoothing (mm):                 S", backlash.smoothing_mm);
    #endif

    #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
      SERIAL_ECHOPGM("  Average measured backlash (mm):");
      if (backlash.has_any_measurement()) {
        LOOP_XYZ(a) if (backlash.has_measurement(a)) {
          SERIAL_CHAR(' ');
          SERIAL_CHAR(axis_codes[a]);
          SERIAL_ECHO(backlash.get_measurement(a));
        }
      }
      else
        SERIAL_ECHOPGM(" (Not yet measured)");
      SERIAL_EOL();
    #endif
  }
}

#endif // BACKLASH_GCODE
