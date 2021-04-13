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

#if ENABLED(PIDTEMP)

#include "../gcode.h"
#include "../../module/temperature.h"

/**
 * M301: Set PID parameters P I D (and optionally C, L)
 *
 *   E[extruder] Default: 0
 *
 *   P[float] Kp term
 *   I[float] Ki term (unscaled)
 *   D[float] Kd term (unscaled)
 *
 * With PID_EXTRUSION_SCALING:
 *
 *   C[float] Kc term
 *   L[int] LPQ length
 *
 * With PID_FAN_SCALING:
 *
 *   F[float] Kf term
 */
void GcodeSuite::M301() {

  // multi-extruder PID patch: M301 updates or prints a single extruder's PID values
  // default behavior (omitting E parameter) is to update for extruder 0 only
  const uint8_t e = parser.byteval('E'); // extruder being updated

  if (e < HOTENDS) { // catch bad input value
    Heater & heater = Temperature::get_heater(HotEndPos0 + e);
    if (parser.seen('P')) heater.pid.setKp(parser.value_float());
    if (parser.seen('I')) heater.pid.setKi(scalePID_i(parser.value_float()));
    if (parser.seen('D')) heater.pid.setKd(scalePID_d(parser.value_float()));
    #if ENABLED(PID_EXTRUSION_SCALING)
      if (parser.seen('C')) heater.pid.setKc(parser.value_float());
      if (parser.seenval('L')) thermalManager.lpq_len = parser.value_int();
      NOMORE(thermalManager.lpq_len, LPQ_MAX_LEN);
      NOLESS(thermalManager.lpq_len, 0);
    #endif

    #if ENABLED(PID_FAN_SCALING)
      if (parser.seen('F')) heater.pid.setKf(parser.value_float());
    #endif

    thermalManager.updatePID();

    SERIAL_ECHO_START();
    #if ENABLED(PID_PARAMS_PER_HOTEND)
      SERIAL_ECHOPAIR(" e:", e); // specify extruder in serial output
    #endif
    SERIAL_ECHOPAIR(" p:", heater.pid.getKp(),
                    " i:", unscalePID_i(heater.pid.getKi()),
                    " d:", unscalePID_d(heater.pid.getKd()));
    #if ENABLED(PID_EXTRUSION_SCALING)
      SERIAL_ECHOPAIR(" c:", heater.pid.getKc());
    #endif
    #if ENABLED(PID_FAN_SCALING)
      SERIAL_ECHOPAIR(" f:", heater.pid.getKf());
    #endif

    SERIAL_EOL();
  }
  else
    SERIAL_ERROR_MSG(STR_INVALID_EXTRUDER);
}

#endif // PIDTEMP
