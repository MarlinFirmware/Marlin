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

#include "../../inc/MarlinConfig.h"

#if ENABLED(MPCTEMP)

#include "../gcode.h"
#include "../../module/temperature.h"

/**
 * M306: MPC settings and autotune
 *
 *  A<watts/kelvin>           Ambient heat transfer coefficient (no fan).
 *  C<joules/kelvin>          Block heat capacity.
 *  E<extruder>               Extruder number to set. (Default: E0)
 *  F<watts/kelvin>           Ambient heat transfer coefficient (fan on full).
 *  P<watts>                  Heater power.
 *  R<kelvin/second/kelvin>   Sensor responsiveness (= transfer coefficient / heat capcity).
 *  T                         Autotune the active extruder.
 */

void GcodeSuite::M306() {
  if (parser.seen_test('T')) {
    thermalManager.MPC_autotune();
    return;
  }

  else if (parser.seen("ACFPR")) {
    const heater_id_t hid = (heater_id_t)parser.intval('E', 0);
    MPC_t& constants = thermalManager.temp_hotend[hid].constants;
    constants.heater_power = parser.floatval('P', constants.heater_power);
    constants.block_heat_capacity = parser.floatval('C', constants.block_heat_capacity);
    constants.sensor_responsiveness = parser.floatval('R', constants.sensor_responsiveness);
    constants.ambient_xfer_coeff_fan0 = parser.floatval('A', constants.ambient_xfer_coeff_fan0);
    #if ENABLED(MPC_INCLUDE_FAN)
      constants.fan255_adjustment = parser.floatval('F', constants.ambient_xfer_coeff_fan0 + constants.fan255_adjustment) - constants.ambient_xfer_coeff_fan0;
    #endif
  }

  HOTEND_LOOP() {
    SERIAL_ECHOLNPGM("MPC constants for hotend ", e);
    MPC_t& constants = thermalManager.temp_hotend[e].constants;
    SERIAL_ECHOLNPGM("Heater power: ", constants.heater_power);
    SERIAL_ECHOLNPGM("Heatblock heat capacity: ", constants.block_heat_capacity);
    SERIAL_ECHOLNPAIR_F("Sensor responsivness: ", constants.sensor_responsiveness, 4);
    SERIAL_ECHOLNPAIR_F("Ambient heat transfer coefficient (no fan): ", constants.ambient_xfer_coeff_fan0, 4);
    #if ENABLED(MPC_INCLUDE_FAN)
      SERIAL_ECHOLNPAIR_F("Ambient heat transfer coefficient (full fan): ", constants.ambient_xfer_coeff_fan0 + constants.fan255_adjustment, 4);
    #endif
  }
}

#endif // MPCTEMP
