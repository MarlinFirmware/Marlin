/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * M192.cpp - Wait for probe to reach temperature
 */

#include "../../inc/MarlinConfig.h"

#if HAS_TEMP_PROBE

#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../lcd/marlinui.h"

/**
 * M192: Wait for probe temperature sensor to reach a target
 *
 * Select only one of these flags:
 *    R - Wait for heating or cooling
 *    S - Wait only for heating
 */
void GcodeSuite::M192() {
  if (DEBUGGING(DRYRUN)) return;

  const bool no_wait_for_cooling = parser.seenval('S');
  if (!no_wait_for_cooling && !parser.seenval('R')) {
    SERIAL_ERROR_MSG("No target temperature set.");
    return;
  }

  const celsius_t target_temp = parser.value_celsius();
  ui.set_status(thermalManager.isProbeBelowTemp(target_temp) ? GET_TEXT_F(MSG_PROBE_HEATING) : GET_TEXT_F(MSG_PROBE_COOLING));
  thermalManager.wait_for_probe(target_temp, no_wait_for_cooling);
}

#endif // HAS_TEMP_PROBE
