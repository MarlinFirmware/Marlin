/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(NOZZLE_CLEAN_FEATURE)

#include "../../../libs/nozzle.h"

#include "../../gcode.h"
#include "../../parser.h"
#include "../../../module/motion.h"

#if HAS_LEVELING
  #include "../../../module/planner.h"
  #include "../../../feature/bedlevel/bedlevel.h"
#endif

/**
 * G12: Clean the nozzle
 */
void GcodeSuite::G12() {
  // Don't allow nozzle cleaning without homing first
  if (axis_unhomed_error()) return;

  const uint8_t pattern = parser.ushortval('P', 0),
                strokes = parser.ushortval('S', NOZZLE_CLEAN_STROKES),
                objects = parser.ushortval('T', NOZZLE_CLEAN_TRIANGLES);
  const float radius = parser.floatval('R', NOZZLE_CLEAN_CIRCLE_RADIUS);

  const bool seenxyz = parser.seen("XYZ");
  const uint8_t cleans =  (!seenxyz || parser.boolval('X') ? _BV(X_AXIS) : 0)
                        | (!seenxyz || parser.boolval('Y') ? _BV(Y_AXIS) : 0)
                        #if DISABLED(NOZZLE_CLEAN_NO_Z)
                          | (!seenxyz || parser.boolval('Z') ? _BV(Z_AXIS) : 0)
                        #endif
                      ;

  #if HAS_LEVELING
    // Disable bed leveling if cleaning Z
    TEMPORARY_BED_LEVELING_STATE(!TEST(cleans, Z_AXIS) && planner.leveling_active);
  #endif

  nozzle.clean(pattern, strokes, radius, objects, cleans);
}

#endif // NOZZLE_CLEAN_FEATURE
