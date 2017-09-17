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

#if ENABLED(BARICUDA)

#include "../../gcode.h"
#include "../../../feature/baricuda.h"

#if HAS_HEATER_1

  /**
   * M126: Heater 1 valve open
   */
  void GcodeSuite::M126() { baricuda_valve_pressure = parser.byteval('S', 255); }

  /**
   * M127: Heater 1 valve close
   */
  void GcodeSuite::M127() { baricuda_valve_pressure = 0; }

#endif // HAS_HEATER_1

#if HAS_HEATER_2

  /**
   * M128: Heater 2 valve open
   */
  void GcodeSuite::M128() { baricuda_e_to_p_pressure = parser.byteval('S', 255); }

  /**
   * M129: Heater 2 valve close
   */
  void GcodeSuite::M129() { baricuda_e_to_p_pressure = 0; }

#endif // HAS_HEATER_2

#endif // BARICUDA
