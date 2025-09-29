/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../../inc/MarlinConfigPre.h"

#if ENABLED(FT_MOTION)

#include "../../gcode.h"
#include "../../../module/ft_motion.h"
#include "../../../module/stepper.h"
#include "../../../module/planner.h"

FSTR_P get_trajectory_type_name() {
  switch (ftMotion.getTrajectoryType()) {
    default:
    case TrajectoryType::TRAPEZOIDAL: return GET_TEXT_F(MSG_FTM_TRAPEZOIDAL);
    case TrajectoryType::POLY5:       return GET_TEXT_F(MSG_FTM_POLY5);
    case TrajectoryType::POLY6:       return GET_TEXT_F(MSG_FTM_POLY6);
  }
}

void say_ftm_settings() {
  const ft_config_t &c = ftMotion.cfg;
  SERIAL_ECHOLN(F("  Trajectory: "), (uint8_t)ftMotion.getTrajectoryType(), F("("), get_trajectory_type_name(), F(")"));
  if (ftMotion.getTrajectoryType() == TrajectoryType::POLY6) {
    SERIAL_ECHOLN(F("  Poly6 Overshoot: "), p_float_t(c.poly6_acceleration_overshoot, 3));
  }
  #define _SMOO_REPORT(A) SERIAL_ECHOLN(F("  "), C(IAXIS_CHAR(_AXIS(A))), F(" smoothing time: "), p_float_t(c.smoothingTime.A, 3), C('s'));
  CARTES_MAP(_SMOO_REPORT);
}

void GcodeSuite::M494_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F("FT Motion"));
  const ft_config_t &c = ftMotion.cfg;
  SERIAL_ECHOLN(F("  M494 T"), (uint8_t)ftMotion.getTrajectoryType()
    CARTES_COMMA CARTES_PAIRED_LIST(
      F(" X"), c.smoothingTime.X, F(" Y"), c.smoothingTime.Y,
      F(" Z"), c.smoothingTime.Z, F(" E"), c.smoothingTime.E
    )
  );
  if (ftMotion.getTrajectoryType() == TrajectoryType::POLY6) {
    SERIAL_ECHOLN(F("  M494 O"), c.poly6_acceleration_overshoot);
  }
}

/**
 * M494: Set Fixed-time Motion Control parameters
 *
 * Parameters:
 *    T<type> Set trajectory generator type (0=TRAPEZOIDAL, 1=POLY5, 2=POLY6)
 *    O<overshoot> Set acceleration overshoot for POLY6 (1.25-1.875)
 *    X<time> Set smoothing time for the X axis
 *    Y<time> Set smoothing time for the Y axis
 *    Z<time> Set smoothing time for the Z axis
 *    E<time> Set smoothing time for the E axis
 */
void GcodeSuite::M494() {
  bool report = !parser.seen_any();

  // Parse trajectory type parameter.
  if (parser.seenval('T')) {
    const int val = parser.value_int();
    if (WITHIN(val, 0, 2)) {
      planner.synchronize();
      ftMotion.setTrajectoryType((TrajectoryType)val);
      report = true;
    }
    else
      SERIAL_ECHOLNPGM("?Invalid trajectory type [T] value. Use 0=TRAPEZOIDAL, 1=POLY5, 2=POLY6");
  }

  // Parse overshoot parameter.
  if (parser.seenval('O')) {
    const float val = parser.value_float();
    if (WITHIN(val, 1.25f, 1.875f)) {
      ftMotion.cfg.poly6_acceleration_overshoot = val;
      report = true;
    }
    else
      SERIAL_ECHOLNPGM("?Invalid overshoot [O] value. Range 1.25-1.875");
  }

  #if HAS_X_AXIS
    // Parse X axis smoothing time parameter.
    if (parser.seenval('X')) {
      const float val = parser.value_float();
      if (WITHIN(val, 0.0f, FTM_MAX_SMOOTHING_TIME)) {
        ftMotion.set_smoothing_time(X_AXIS, val);
        report = true;
      }
      else
        SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_A_NAME), " smoothing time [", C('X'), "] value.");
    }
  #endif

  #if HAS_Y_AXIS
    // Parse Y axis smoothing time parameter.
    if (parser.seenval('Y')) {
      const float val = parser.value_float();
      if (WITHIN(val, 0.0f, FTM_MAX_SMOOTHING_TIME)) {
        ftMotion.set_smoothing_time(Y_AXIS, val);
        report = true;
      }
      else
        SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_B_NAME), " smoothing time [", C('Y'), "] value.");
    }
  #endif

  #if HAS_Z_AXIS
    // Parse Z axis smoothing time parameter.
    if (parser.seenval('Z')) {
      const float val = parser.value_float();
      if (WITHIN(val, 0.0f, FTM_MAX_SMOOTHING_TIME)) {
        ftMotion.set_smoothing_time(Z_AXIS, val);
        report = true;
      }
      else
        SERIAL_ECHOLNPGM("?Invalid ", C(STEPPER_C_NAME), " smoothing time [", C('Z'), "] value.");
    }
  #endif

  #if HAS_EXTRUDERS
    // Parse E axis smoothing time parameter.
    if (parser.seenval('E')) {
      const float val = parser.value_float();
      if (WITHIN(val, 0.0f, FTM_MAX_SMOOTHING_TIME)) {
        ftMotion.set_smoothing_time(E_AXIS, val);
        report = true;
      }
      else
        SERIAL_ECHOLNPGM("?Invalid ", C('E'), " smoothing time [", C('E'), "] value.");
    }
  #endif

  if (report) say_ftm_settings();
}

#endif // FT_MOTION
