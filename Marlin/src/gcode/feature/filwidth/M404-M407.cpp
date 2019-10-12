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

#if ENABLED(FILAMENT_WIDTH_SENSOR)

#include "../../../feature/filwidth.h"
#include "../../../module/planner.h"
#include "../../../module/temperature.h"
#include "../../../Marlin.h"
#include "../../gcode.h"

/**
 * M404: Display or set (in current units) the nominal filament width (3mm, 1.75mm ) W<3.0>
 */
void GcodeSuite::M404() {
  if (parser.seenval('W')) {
    filwidth.nominal_mm = parser.value_linear_units();
    planner.volumetric_area_nominal = CIRCLE_AREA(filwidth.nominal_mm * 0.5);
  }
  else
    SERIAL_ECHOLNPAIR("Filament dia (nominal mm):", filwidth.nominal_mm);
}

/**
 * M405: Turn on filament sensor for control
 */
void GcodeSuite::M405() {
  // This is technically a linear measurement, but since it's quantized to centimeters and is a different
  // unit than everything else, it uses parser.value_byte() instead of parser.value_linear_units().
  if (parser.seenval('D'))
    filwidth.set_delay_cm(parser.value_byte());

  filwidth.enable(true);
}

/**
 * M406: Turn off filament sensor for control
 */
void GcodeSuite::M406() {
  filwidth.enable(false);
  planner.calculate_volumetric_multipliers();   // Restore correct 'volumetric_multiplier' value
}

/**
 * M407: Get measured filament diameter on serial output
 */
void GcodeSuite::M407() {
  SERIAL_ECHOLNPAIR("Filament dia (measured mm):", filwidth.measured_mm);
}

#endif // FILAMENT_WIDTH_SENSOR
