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

  auto axis_can_calibrate = [](const uint8_t a) {
    #define _CAN_CASE(N) case N##_AXIS: return AXIS_CAN_CALIBRATE(N);
    switch (a) {
      default: return false;
      MAIN_AXIS_MAP(_CAN_CASE)
    }
  };

  LOOP_NUM_AXES(a) {
    if (axis_can_calibrate(a) && parser.seen(AXIS_CHAR(a))) {
      planner.synchronize();
      backlash.set_distance_mm((AxisEnum)a, parser.has_value() ? parser.value_axis_units((AxisEnum)a) : backlash.get_measurement((AxisEnum)a));
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
      backlash.set_smoothing_mm(parser.value_linear_units());
      noArgs = false;
    }
  #endif

  if (noArgs) {
    SERIAL_ECHOPGM("Backlash Correction ");
    if (!backlash.get_correction_uint8()) SERIAL_ECHOPGM("in");
    SERIAL_ECHOLNPGM("active:");
    SERIAL_ECHOLNPGM("  Correction Amount/Fade-out:     F", backlash.get_correction(), " (F1.0 = full, F0.0 = none)");
    SERIAL_ECHOPGM("  Backlash Distance (mm):        ");
    LOOP_NUM_AXES(a) if (axis_can_calibrate(a)) {
      SERIAL_ECHOLNPGM_P((PGM_P)pgm_read_ptr(&SP_AXIS_STR[a]), backlash.get_distance_mm((AxisEnum)a));
    }

    #ifdef BACKLASH_SMOOTHING_MM
      SERIAL_ECHOLNPGM("  Smoothing (mm):                 S", backlash.get_smoothing_mm());
    #endif

    #if ENABLED(MEASURE_BACKLASH_WHEN_PROBING)
      SERIAL_ECHOPGM("  Average measured backlash (mm):");
      if (backlash.has_any_measurement()) {
        LOOP_NUM_AXES(a) if (axis_can_calibrate(a) && backlash.has_measurement(AxisEnum(a))) {
          SERIAL_ECHOPGM_P((PGM_P)pgm_read_ptr(&SP_AXIS_STR[a]), backlash.get_measurement((AxisEnum)a));
        }
      }
      else
        SERIAL_ECHOPGM(" (Not yet measured)");
      SERIAL_EOL();
    #endif
  }
}

void GcodeSuite::M425_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_BACKLASH_COMPENSATION));
  SERIAL_ECHOLNPGM_P(
    PSTR("  M425 F"), backlash.get_correction()
    #ifdef BACKLASH_SMOOTHING_MM
      , PSTR(" S"), LINEAR_UNIT(backlash.get_smoothing_mm())
    #endif
    , LIST_N(DOUBLE(NUM_AXES),
        SP_X_STR, LINEAR_UNIT(backlash.get_distance_mm(X_AXIS)),
        SP_Y_STR, LINEAR_UNIT(backlash.get_distance_mm(Y_AXIS)),
        SP_Z_STR, LINEAR_UNIT(backlash.get_distance_mm(Z_AXIS)),
        SP_I_STR, I_AXIS_UNIT(backlash.get_distance_mm(I_AXIS)),
        SP_J_STR, J_AXIS_UNIT(backlash.get_distance_mm(J_AXIS)),
        SP_K_STR, K_AXIS_UNIT(backlash.get_distance_mm(K_AXIS)),
        SP_U_STR, U_AXIS_UNIT(backlash.get_distance_mm(U_AXIS)),
        SP_V_STR, V_AXIS_UNIT(backlash.get_distance_mm(V_AXIS)),
        SP_W_STR, W_AXIS_UNIT(backlash.get_distance_mm(W_AXIS))
      )
  );
}

#endif // BACKLASH_GCODE
