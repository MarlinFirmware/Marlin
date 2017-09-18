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
  if (parser.seen('W')) {
    filament_width_nominal = parser.value_linear_units();
  }
  else {
    SERIAL_PROTOCOLPGM("Filament dia (nominal mm):");
    SERIAL_PROTOCOLLN(filament_width_nominal);
  }
}

/**
 * M405: Turn on filament sensor for control
 */
void GcodeSuite::M405() {
  // This is technically a linear measurement, but since it's quantized to centimeters and is a different
  // unit than everything else, it uses parser.value_byte() instead of parser.value_linear_units().
  if (parser.seen('D')) {
    meas_delay_cm = parser.value_byte();
    NOMORE(meas_delay_cm, MAX_MEASUREMENT_DELAY);
  }

  if (filwidth_delay_index[1] == -1) { // Initialize the ring buffer if not done since startup
    const uint8_t temp_ratio = thermalManager.widthFil_to_size_ratio() - 100; // -100 to scale within a signed byte

    for (uint8_t i = 0; i < COUNT(measurement_delay); ++i)
      measurement_delay[i] = temp_ratio;

    filwidth_delay_index[0] = filwidth_delay_index[1] = 0;
  }

  filament_sensor = true;

  //SERIAL_PROTOCOLPGM("Filament dia (measured mm):");
  //SERIAL_PROTOCOL(filament_width_meas);
  //SERIAL_PROTOCOLPGM("Extrusion ratio(%):");
  //SERIAL_PROTOCOL(planner.flow_percentage[active_extruder]);
}

/**
 * M406: Turn off filament sensor for control
 */
void GcodeSuite::M406() {
  filament_sensor = false;
  planner.calculate_volumetric_multipliers();   // Restore correct 'volumetric_multiplier' value
}

/**
 * M407: Get measured filament diameter on serial output
 */
void GcodeSuite::M407() {
  SERIAL_PROTOCOLPGM("Filament dia (measured mm):");
  SERIAL_PROTOCOLLN(filament_width_meas);
}

#endif // FILAMENT_WIDTH_SENSOR
