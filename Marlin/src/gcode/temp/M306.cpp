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
#include "../../lcd/marlinui.h"
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
  if (parser.seen_test('T')) {
    LCD_MESSAGE(MSG_MPC_AUTOTUNE);
    thermalManager.MPC_autotune();
    ui.reset_status();
    return;
  }

  if (parser.seen("ACFPRH")) {
    const heater_id_t hid = (heater_id_t)parser.intval('E', 0);
    MPC_t &mpc = thermalManager.temp_hotend[hid].mpc;
    if (parser.seenval('P')) mpc.heater_power = parser.value_float();
    if (parser.seenval('C')) mpc.block_heat_capacity = parser.value_float();
    if (parser.seenval('R')) mpc.sensor_responsiveness = parser.value_float();
    if (parser.seenval('A')) mpc.ambient_xfer_coeff_fan0 = parser.value_float();
    #if ENABLED(MPC_INCLUDE_FAN)
      if (parser.seenval('F')) mpc.fan255_adjustment = parser.value_float() - mpc.ambient_xfer_coeff_fan0;
    #endif
    if (parser.seenval('H')) mpc.filament_heat_capacity_permm = parser.value_float();
    return;
  }

  M306_report(true);
}

void GcodeSuite::M306_report(const bool forReplay/*=true*/) {
  report_heading(forReplay, F("Model predictive control"));
  HOTEND_LOOP() {
    report_echo_start(forReplay);
    MPC_t& mpc = thermalManager.temp_hotend[e].mpc;
    SERIAL_ECHOPGM("  M306 E", e);
    SERIAL_ECHOPAIR_F(" P", mpc.heater_power, 2);
    SERIAL_ECHOPAIR_F(" C", mpc.block_heat_capacity, 2);
    SERIAL_ECHOPAIR_F(" R", mpc.sensor_responsiveness, 4);
    SERIAL_ECHOPAIR_F(" A", mpc.ambient_xfer_coeff_fan0, 4);
    #if ENABLED(MPC_INCLUDE_FAN)
      SERIAL_ECHOPAIR_F(" F", mpc.ambient_xfer_coeff_fan0 + mpc.fan255_adjustment, 4);
    #endif
    SERIAL_ECHOPAIR_F(" H", mpc.filament_heat_capacity_permm, 4);
    SERIAL_EOL();
  }
}

#endif // MPCTEMP
