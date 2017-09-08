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

#include "../gcode.h"
#include "../parser.h"
#include "../../Marlin.h"

/**
 * M200: Set filament diameter and set E axis units to cubic units
 *
 *    T<extruder> - Optional extruder number. Current extruder if omitted.
 *    D<linear> - Diameter of the filament. Use "D0" to switch back to linear units on the E axis.
 */
void GcodeSuite::M200() {

  if (get_target_extruder_from_command()) return;

  if (parser.seen('D')) {
    // setting any extruder filament size disables volumetric on the assumption that
    // slicers either generate in extruder values as cubic mm or as as filament feeds
    // for all extruders
    volumetric_enabled = (parser.value_linear_units() != 0.0);
    if (volumetric_enabled) {
      filament_size[gcode.target_extruder] = parser.value_linear_units();
      // make sure all extruders have some sane value for the filament size
      for (uint8_t i = 0; i < COUNT(filament_size); i++)
        if (! filament_size[i]) filament_size[i] = DEFAULT_NOMINAL_FILAMENT_DIA;
    }
  }
  calculate_volumetric_multipliers();
}
