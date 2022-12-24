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

#if ENABLED(POWER_LOSS_RECOVERY)

#include "../../gcode.h"
#include "../../../feature/powerloss.h"
#include "../../../module/motion.h"
#include "../../../lcd/marlinui.h"

/**
 * M413: Enable / Disable power-loss recovery
 *
 * Parameters
 *   S[bool] - Flag to enable / disable.
 *             If omitted, report current state.
 */
void GcodeSuite::M413() {

  if (parser.seen('S'))
    recovery.enable(parser.value_bool());
  else
    M413_report();

  #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
    if (parser.seen("RL")) recovery.load();
    if (parser.seen_test('W')) recovery.save(true);
    if (parser.seen_test('P')) recovery.purge();
    if (parser.seen_test('D')) recovery.debug(F("M413"));
    if (parser.seen_test('O')) recovery._outage(true);
    if (parser.seen_test('C')) (void)recovery.check();
    if (parser.seen_test('E')) SERIAL_ECHOF(recovery.exists() ? F("PLR Exists\n") : F("No PLR\n"));
    if (parser.seen_test('V')) SERIAL_ECHOF(recovery.valid() ? F("Valid\n") : F("Invalid\n"));
  #endif
}

void GcodeSuite::M413_report(const bool forReplay/*=true*/) {
  report_heading_etc(forReplay, F(STR_POWER_LOSS_RECOVERY));
  SERIAL_ECHOPGM("  M413 S", AS_DIGIT(recovery.enabled), " ; ");
  serialprintln_onoff(recovery.enabled);
}

#endif // POWER_LOSS_RECOVERY
