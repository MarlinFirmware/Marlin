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

#if ENABLED(FTM_SMOOTHING)

#include "../../gcode.h"
#include "../../../module/ft_motion.h"
#include "../../../module/stepper.h"

void say_smoothing() {
  #if HAS_X_AXIS
    SERIAL_ECHOLN(F("  "), C('X'), F(" smoothing time: "), p_float_t(ftMotion.cfg.smoothingTime.x, 3), C('s'));
  #endif
  #if HAS_Y_AXIS
    SERIAL_ECHOLN(F("  "), C('Y'), F(" smoothing time: "), p_float_t(ftMotion.cfg.smoothingTime.y, 3), C('s'));
  #endif
  #if HAS_Z_AXIS
    SERIAL_ECHOLN(F("  "), C('Z'), F(" smoothing time: "), p_float_t(ftMotion.cfg.smoothingTime.z, 3), C('s'));
  #endif
  #if HAS_EXTRUDERS
    SERIAL_ECHOLN(F("  "), C('E'), F(" smoothing time: "), p_float_t(ftMotion.cfg.smoothingTime.e, 3), C('s'));
  #endif
}

void GcodeSuite::M494_report(const bool forReplay/*=true*/) {
  TERN_(MARLIN_SMALL_BUILD, return);

  report_heading_etc(forReplay, F("FTM Smoothing"));
  const ft_config_t &c = ftMotion.cfg;
  SERIAL_ECHOLN(F("  M494")
    #if HAS_X_AXIS
      , F(" X"), c.smoothingTime.x
    #endif
    #if HAS_Y_AXIS
      , F(" Y"), c.smoothingTime.y
    #endif
    #if HAS_Z_AXIS
      , F(" Z"), c.smoothingTime.z
    #endif
    #if HAS_EXTRUDERS
      , F(" E"), c.smoothingTime.e
    #endif
  );
}

/**
 * M494: Set Fixed-time Motion Control Smoothing parameters
 *
 * Parameters:
 *    X<time> Set smoothing time for the X axis
 *    Y<time> Set smoothing time for the Y axis
 *    Z<time> Set smoothing time for the Z axis
 *    E<time> Set smoothing time for the E axis
 */
void GcodeSuite::M494() {
  bool report = !parser.seen_any();

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

  if (report) say_smoothing();
}

#endif // FTM_SMOOTHING
