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

#if ENABLED(BACKLASH_GCODE)

#include "../../module/planner.h"

float backlash_distance_mm[XYZ] = BACKLASH_DISTANCE_MM,
      backlash_correction = BACKLASH_CORRECTION;

#ifdef BACKLASH_SMOOTHING_MM
  float backlash_smoothing_mm = BACKLASH_SMOOTHING_MM;
#endif

#if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
  float backlash_measured_mm[XYZ] = { 0 };
  uint8_t backlash_measured_num[XYZ] = { 0 };
#endif

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
      const float measured_backlash = (
        #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
          backlash_measured_num[i] > 0 ? backlash_measured_mm[i] / backlash_measured_num[i] : 0
        #else
          0
        #endif
      );
      backlash_distance_mm[i] = parser.has_value() ? parser.value_linear_units() : measured_backlash;
      noArgs = false;
    }
  }

  if (parser.seen('F')) {
    planner.synchronize();
    backlash_correction = max(0, min(1.0, parser.value_linear_units()));
    noArgs = false;
  }

  #ifdef BACKLASH_SMOOTHING_MM
    if (parser.seen('S')) {
      planner.synchronize();
      backlash_smoothing_mm = parser.value_linear_units();
      noArgs = false;
    }
  #endif

  if (noArgs) {
    SERIAL_ECHOPGM("Backlash correction is ");
    if (!backlash_correction) SERIAL_ECHOPGM("in");
    SERIAL_ECHOLNPGM("active:");
    SERIAL_ECHOPAIR("  Correction Amount/Fade-out:     F", backlash_correction);
    SERIAL_ECHOLNPGM("     (F1.0 = full, F0.0 = none)");
    SERIAL_ECHOPGM("  Backlash Distance (mm):        ");
    LOOP_XYZ(a) {
      SERIAL_CHAR(' ');
      SERIAL_CHAR(axis_codes[a]);
      SERIAL_ECHO(backlash_distance_mm[a]);
      SERIAL_EOL();
    }

    #ifdef BACKLASH_SMOOTHING_MM
      SERIAL_ECHOLNPAIR("  Smoothing (mm):                 S", backlash_smoothing_mm);
    #endif

    #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
      SERIAL_ECHOPGM("  Average measured backlash (mm):");
      LOOP_XYZ(a) {
        if (backlash_measured_num[a] > 0) {
          SERIAL_CHAR(' ');
          SERIAL_CHAR(axis_codes[a]);
          SERIAL_ECHO(backlash_measured_mm[a] / backlash_measured_num[a]);
        }
      }
      if (!backlash_measured_num[X_AXIS] && !backlash_measured_num[Y_AXIS] && !backlash_measured_num[Z_AXIS])
        SERIAL_ECHOPGM(" (Not yet measured)");
      SERIAL_EOL();
    #endif
  }
}

#endif // BACKLASH_GCODE
