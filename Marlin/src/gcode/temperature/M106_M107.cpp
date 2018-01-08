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

#if FAN_COUNT > 0

#include "../gcode.h"
#include "../../Marlin.h" // for fanSpeeds — should move those to Planner

/**
 * M106: Set Fan Speed
 *
 *  S<int>   Speed between 0-255
 *  P<index> Fan index, if more than one fan
 *
 * With EXTRA_FAN_SPEED enabled:
 *
 *  T<int>   Restore/Use/Set Temporary Speed:
 *           1     = Restore previous speed after T2
 *           2     = Use temporary speed set with T3-255
 *           3-255 = Set the speed for use with T2
 */
void GcodeSuite::M106() {
  const uint8_t p = parser.byteval('P');
  if (p < FAN_COUNT) {
    #if ENABLED(EXTRA_FAN_SPEED)
      const int16_t t = parser.intval('T');
      if (t > 0) {
        switch (t) {
          case 1:
            fanSpeeds[p] = old_fanSpeeds[p];
            break;
          case 2:
            old_fanSpeeds[p] = fanSpeeds[p];
            fanSpeeds[p] = new_fanSpeeds[p];
            break;
          default:
            new_fanSpeeds[p] = min(t, 255);
            break;
        }
        return;
      }
    #endif // EXTRA_FAN_SPEED
    const uint16_t s = parser.ushortval('S', 255);
    fanSpeeds[p] = min(s, 255);
  }
}

/**
 * M107: Fan Off
 */
void GcodeSuite::M107() {
  const uint16_t p = parser.ushortval('P');
  if (p < FAN_COUNT) fanSpeeds[p] = 0;
}

#endif // FAN_COUNT > 0
