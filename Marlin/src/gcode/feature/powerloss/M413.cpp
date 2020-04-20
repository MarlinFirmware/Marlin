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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../../inc/MarlinConfig.h"

#if ENABLED(POWER_LOSS_RECOVERY)

#include "../../gcode.h"
#include "../../../feature/powerloss.h"
#include "../../../module/motion.h"
#include "../../../lcd/ultralcd.h"

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
  else {
    SERIAL_ECHO_START();
    SERIAL_ECHOPGM("Power-loss recovery ");
    serialprintln_onoff(recovery.enabled);
  }

  #if ENABLED(DEBUG_POWER_LOSS_RECOVERY)
    if (parser.seen("RL")) recovery.load();
    if (parser.seen('W')) recovery.save(true);
    if (parser.seen('P')) recovery.purge();
    if (parser.seen('E')) serialprintPGM(recovery.exists() ? PSTR("PLR Exists\n") : PSTR("No PLR\n"));
    if (parser.seen('V')) serialprintPGM(recovery.valid() ? PSTR("Valid\n") : PSTR("Invalid\n"));
  #endif
}

#endif // POWER_LOSS_RECOVERY
