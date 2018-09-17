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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(MIXING_EXTRUDER)

#include "../../gcode.h"
#include "../../../feature/mixing.h"

/**
 * M163: Set a single mix factor for a mixing extruder
 *       This is called "weight" by some systems.
 *       The 'P' values must sum to 1.0 or must be followed by M164 to normalize them.
 *
 *   S[index]   The channel index to set
 *   P[float]   The mix value
 */
void GcodeSuite::M163() {
  const int mix_index = parser.intval('S');
  if (mix_index < MIXING_STEPPERS)
    mixing_factor[mix_index] = MAX(parser.floatval('P'), 0.0);
}

/**
 * M164: Normalize and commit the mix.
 *       If 'S' is given store as a virtual tool. (Requires MIXING_VIRTUAL_TOOLS > 1)
 *
 *   S[index]   The virtual tool to store
 */
void GcodeSuite::M164() {
  normalize_mix();
  #if MIXING_VIRTUAL_TOOLS > 1
    const int tool_index = parser.intval('S', -1);
    if (WITHIN(tool_index, 0, MIXING_VIRTUAL_TOOLS - 1)) {
      for (uint8_t i = 0; i < MIXING_STEPPERS; i++)
        mixing_virtual_tool_mix[tool_index][i] = mixing_factor[i];
    }
  #endif
}

#if ENABLED(DIRECT_MIXING_IN_G1)

  /**
   * M165: Set multiple mix factors for a mixing extruder.
   *       Factors that are left out will be set to 0.
   *       All factors should sum to 1.0, but they will be normalized regardless.
   *
   *   A[factor] Mix factor for extruder stepper 1
   *   B[factor] Mix factor for extruder stepper 2
   *   C[factor] Mix factor for extruder stepper 3
   *   D[factor] Mix factor for extruder stepper 4
   *   H[factor] Mix factor for extruder stepper 5
   *   I[factor] Mix factor for extruder stepper 6
   */
  void GcodeSuite::M165() { gcode_get_mix(); }

#endif // DIRECT_MIXING_IN_G1

#endif // MIXING_EXTRUDER
