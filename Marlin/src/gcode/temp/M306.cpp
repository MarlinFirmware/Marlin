/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 *  T                         Autotune the active extruder.
 *
 *  A<watts/kelvin>           Ambient heat transfer coefficient (no fan).
 *  C<joules/kelvin>          Block heat capacity.
 *  E<extruder>               Extruder number to set. (Default: E0)
 *  F<watts/kelvin>           Ambient heat transfer coefficient (fan on full).
 *  H<joules/kelvin/mm>       Filament heat capacity per mm.
 *  P<watts>                  Heater power.
 *  R<kelvin/second/kelvin>   Sensor responsiveness (= transfer coefficient / heat capcity).
 */

void GcodeSuite::M306() {
  if (parser.seen_test('T')) { thermalManager.MPC_autotune(); return; }

  if (parser.seen("ACFPRH")) {
    const heater_id_t hid = (heater_id_t)parser.intval('E', 0);
    MPC_t &constants = thermalManager.temp_hotend[hid].constants;
    if (parser.seenval('P')) constants.heater_power = parser.value_float();
    if (parser.seenval('C')) constants.block_heat_capacity = parser.value_float();
    if (parser.seenval('R')) constants.sensor_responsiveness = parser.value_float();
    if (parser.seenval('A')) constants.ambient_xfer_coeff_fan0 = parser.value_float();
    #if ENABLED(MPC_INCLUDE_FAN)
      if (parser.seenval('F')) constants.fan255_adjustment = parser.value_float() - constants.ambient_xfer_coeff_fan0;
    #endif
    if (parser.seenval('H')) constants.filament_heat_capacity_permm = parser.value_float();
    return;
  }

  M306_report(true);
}

void GcodeSuite::M306_report(const bool forReplay/*=true*/) {
  report_heading(forReplay, F("Model predictive control"));
  HOTEND_LOOP() {
    report_echo_start(forReplay);
    MPC_t& constants = thermalManager.temp_hotend[e].constants;
    SERIAL_ECHOPGM("  M306 E", e);
    SERIAL_ECHOPAIR_F(" P", constants.heater_power, 2);
    SERIAL_ECHOPAIR_F(" C", constants.block_heat_capacity, 2);
    SERIAL_ECHOPAIR_F(" R", constants.sensor_responsiveness, 4);
    SERIAL_ECHOPAIR_F(" A", constants.ambient_xfer_coeff_fan0, 4);
    #if ENABLED(MPC_INCLUDE_FAN)
      SERIAL_ECHOPAIR_F(" F", constants.ambient_xfer_coeff_fan0 + constants.fan255_adjustment, 4);
    #endif
    SERIAL_ECHOPAIR_F(" M", constants.filament_heat_capacity_permm, 4);
    SERIAL_EOL();
  }
}

#endif // MPCTEMP
