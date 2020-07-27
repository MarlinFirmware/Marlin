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

#if ENABLED(FWRETRACT)

#include "../../../feature/fwretract.h"
#include "../../gcode.h"
#include "../../../module/motion.h"

/**
 * G10 - Retract filament according to settings of M207
 *       TODO: Handle 'G10 P' for tool settings and 'G10 L' for workspace settings
 */
void GcodeSuite::G10() {
  #if EXTRUDERS > 1
    const bool rs = parser.boolval('S');
  #endif
  fwretract.retract(true
    #if EXTRUDERS > 1
      , rs
    #endif
  );
}

/**
 * G11 - Recover filament according to settings of M208
 */
void GcodeSuite::G11() { fwretract.retract(false); }

#endif // FWRETRACT
