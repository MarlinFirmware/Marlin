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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(GRADIENT_MIX)

#include "../../gcode.h"
#include "../../../module/motion.h"
#include "../../../module/planner.h"
#include "../../../feature/mixing.h"

inline void echo_mix() {
  SERIAL_ECHOPAIR(" (", int(mixer.mix[0]), "%|", int(mixer.mix[1]), "%)");
}

inline void echo_zt(const int t, const float &z) {
  mixer.update_mix_from_vtool(t);
  SERIAL_ECHOPAIR_P(SP_Z_STR, z, SP_T_STR, t);
  echo_mix();
}

/**
 * M166: Set a simple gradient mix for a two-component mixer
 *       based on the Geeetech A10M implementation by Jone Liu.
 *
 *   S[bool]  - Enable / disable gradients
 *   A[float] - Starting Z for the gradient
 *   Z[float] - Ending Z for the gradient. (Must be greater than the starting Z.)
 *   I[index] - V-Tool to use as the starting mix.
 *   J[index] - V-Tool to use as the ending mix.
 *
 *   T[index] - A V-Tool index to use as an alias for the Gradient (Requires GRADIENT_VTOOL)
 *              T with no index clears the setting. Note: This can match the I or J value.
 *
 * Example: M166 S1 A0 Z20 I0 J1
 */
void GcodeSuite::M166() {
  if (parser.seenval('A')) mixer.gradient.start_z = parser.value_float();
  if (parser.seenval('Z')) mixer.gradient.end_z = parser.value_float();
  if (parser.seenval('I')) mixer.gradient.start_vtool = (uint8_t)constrain(parser.value_int(), 0, MIXING_VIRTUAL_TOOLS);
  if (parser.seenval('J')) mixer.gradient.end_vtool = (uint8_t)constrain(parser.value_int(), 0, MIXING_VIRTUAL_TOOLS);

  #if ENABLED(GRADIENT_VTOOL)
    if (parser.seen('T')) mixer.gradient.vtool_index = parser.byteval('T', -1);
  #endif

  if (parser.seen('S')) mixer.gradient.enabled = parser.value_bool();

  mixer.refresh_gradient();

  SERIAL_ECHOPGM("Gradient Mix ");
  serialprint_onoff(mixer.gradient.enabled);
  if (mixer.gradient.enabled) {

    #if ENABLED(GRADIENT_VTOOL)
      if (mixer.gradient.vtool_index >= 0) {
        SERIAL_ECHOPAIR(" (T", int(mixer.gradient.vtool_index));
        SERIAL_CHAR(')');
      }
    #endif

    SERIAL_ECHOPGM(" ; Start");
    echo_zt(mixer.gradient.start_vtool, mixer.gradient.start_z);

    SERIAL_ECHOPGM(" ; End");
    echo_zt(mixer.gradient.end_vtool, mixer.gradient.end_z);

    mixer.update_mix_from_gradient();

    SERIAL_ECHOPGM(" ; Current Z");
    #if ENABLED(DELTA)
      get_cartesian_from_steppers();
      SERIAL_ECHO(cartes.z);
    #else
      SERIAL_ECHO(planner.get_axis_position_mm(Z_AXIS));
    #endif
    echo_mix();
  }

  SERIAL_EOL();
}

#endif // GRADIENT_MIX
