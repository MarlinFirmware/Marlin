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

static FSTR_P get_trajectory_type_name() {
  switch (ftMotion.getTrajectoryType()) {
    default:
    case TrajectoryType::TRAPEZOIDAL: return GET_TEXT_F(MSG_FTM_TRAPEZOIDAL);
    case TrajectoryType::POLY5:       return GET_TEXT_F(MSG_FTM_POLY5);
    case TrajectoryType::POLY6:       return GET_TEXT_F(MSG_FTM_POLY6);
  }
}

void say_ftm_settings() {
  SERIAL_ECHOLN(F("  Trajectory: "), get_trajectory_type_name(), C('('), (uint8_t)ftMotion.getTrajectoryType(), C(')'));

  const ft_config_t &c = ftMotion.cfg;

  if (ftMotion.getTrajectoryType() == TrajectoryType::POLY6)
    SERIAL_ECHOLNPGM("  Poly6 Overshoot: ", p_float_t(c.poly6_acceleration_overshoot, 3));

  #if ENABLED(FTM_SMOOTHING)
    #define _SMOO_REPORT(A) SERIAL_ECHOLN(F("  "), C(IAXIS_CHAR(_AXIS(A))), F(" smoothing time: "), p_float_t(c.smoothingTime.A, 3), C('s'));
    CARTES_MAP(_SMOO_REPORT);
  #endif
}

void GcodeSuite::M494_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  const ft_config_t &c = ftMotion.cfg;

  report_heading_etc(forReplay, F("FT Motion"));
  SERIAL_ECHOPGM("  M494 T", (uint8_t)ftMotion.getTrajectoryType());

  #if ENABLED(FTM_SMOOTHING)
    SERIAL_ECHOPGM(
      CARTES_PAIRED_LIST(
        " X", c.smoothingTime.X, " Y", c.smoothingTime.Y,
        " Z", c.smoothingTime.Z, " E", c.smoothingTime.E
      )
    );
  #endif

  if (ftMotion.getTrajectoryType() == TrajectoryType::POLY6)
    SERIAL_ECHOPGM(" O", c.poly6_acceleration_overshoot);

  SERIAL_EOL();
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

  #if ENABLED(FTM_SMOOTHING)

    #define SMOOTH_SET(A,N) \
      if (parser.seenval(CHARIFY(A))) { \
        const float val = parser.value_float(); \
        if (WITHIN(val, 0.0f, FTM_MAX_SMOOTHING_TIME)) { \
          ftMotion.set_smoothing_time(_AXIS(A), val); \
          report = true; \
        } \
        else \
          SERIAL_ECHOLNPGM("?Invalid ", C(N), " smoothing time [", C(CHARIFY(A)), "] value."); \
      }

    CARTES_GANG(
      SMOOTH_SET(X, STEPPER_A_NAME), SMOOTH_SET(Y, STEPPER_B_NAME),
      SMOOTH_SET(Z, STEPPER_C_NAME), SMOOTH_SET(E, 'E')
    );

  #endif // FTM_SMOOTHING

  if (report) say_ftm_settings();
}

#endif // FT_MOTION
