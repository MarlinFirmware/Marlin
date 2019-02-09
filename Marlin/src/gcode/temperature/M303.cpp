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

#if HAS_PID_HEATING

#include "../gcode.h"
#include "../../module/temperature.h"

/**
 * M303: PID relay autotune
 *
 *       S<temperature> sets the target temperature. (default 150C / 70C)
 *       E<extruder> (-1 for the bed) (default 0)
 *       C<cycles> Minimum 3. Default 5.
 *       U<bool> with a non-zero value will apply the result to current settings
 */
void GcodeSuite::M303() {

  const int8_t e = parser.intval('E');

  if (!WITHIN(e, 0
    #if ENABLED(PIDTEMPBED)
      -1
    #endif
    ,
    #if ENABLED(PIDTEMP)
      HOTENDS
    #endif
    -1
  )) {
    SERIAL_ECHOLNPGM(MSG_PID_BAD_EXTRUDER_NUM);
    return;
  }

  const int c = parser.intval('C', 5);
  const bool u = parser.boolval('U');
  const int16_t temp = parser.celsiusval('S', e < 0 ? 70 : 150);

  #if DISABLED(BUSY_WHILE_HEATING)
    KEEPALIVE_STATE(NOT_BUSY);
  #endif

  thermalManager.PID_autotune(temp, e, c, u);

  #if DISABLED(BUSY_WHILE_HEATING)
    KEEPALIVE_STATE(IN_HANDLER);
  #endif
}

#endif // HAS_PID_HEATING
