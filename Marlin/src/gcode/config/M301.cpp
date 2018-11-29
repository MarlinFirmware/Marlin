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

#if ENABLED(PIDTEMP)

#include "../gcode.h"
#include "../../module/temperature.h"

/**
 * M301: Set PID parameters P I D (and optionally C, L)
 *
 *   P[float] Kp term
 *   I[float] Ki term (unscaled)
 *   D[float] Kd term (unscaled)
 *
 * With PID_EXTRUSION_SCALING:
 *
 *   C[float] Kc term
 *   L[int] LPQ length
 */
void GcodeSuite::M301() {

  // multi-extruder PID patch: M301 updates or prints a single extruder's PID values
  // default behaviour (omitting E parameter) is to update for extruder 0 only
  const uint8_t e = parser.byteval('E'); // extruder being updated

  if (e < HOTENDS) { // catch bad input value
    if (parser.seen('P')) PID_PARAM(Kp, e) = parser.value_float();
    if (parser.seen('I')) PID_PARAM(Ki, e) = scalePID_i(parser.value_float());
    if (parser.seen('D')) PID_PARAM(Kd, e) = scalePID_d(parser.value_float());
    #if ENABLED(PID_EXTRUSION_SCALING)
      if (parser.seen('C')) PID_PARAM(Kc, e) = parser.value_float();
      if (parser.seenval('L')) thermalManager.lpq_len = parser.value_int();
      NOMORE(thermalManager.lpq_len, LPQ_MAX_LEN);
      NOLESS(thermalManager.lpq_len, 0);
    #endif

    thermalManager.updatePID();
    SERIAL_ECHO_START();
    #if ENABLED(PID_PARAMS_PER_HOTEND)
      SERIAL_ECHOPAIR(" e:", e); // specify extruder in serial output
    #endif // PID_PARAMS_PER_HOTEND
    SERIAL_ECHOPAIR(" p:", PID_PARAM(Kp, e));
    SERIAL_ECHOPAIR(" i:", unscalePID_i(PID_PARAM(Ki, e)));
    SERIAL_ECHOPAIR(" d:", unscalePID_d(PID_PARAM(Kd, e)));
    #if ENABLED(PID_EXTRUSION_SCALING)
      //Kc does not have scaling applied above, or in resetting defaults
      SERIAL_ECHOPAIR(" c:", PID_PARAM(Kc, e));
    #endif
    SERIAL_EOL();
  }
  else
    SERIAL_ERROR_MSG(MSG_INVALID_EXTRUDER);
}

#endif // PIDTEMP
