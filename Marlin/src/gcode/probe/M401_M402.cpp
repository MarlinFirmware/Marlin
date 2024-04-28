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

#if HAS_BED_PROBE

#include "../gcode.h"
#include "../../module/motion.h"
#include "../../module/probe.h"

#if HAS_BLTOUCH_HS_MODE
  #include "../../feature/bltouch.h"
#endif

/**
 * @brief M401: Deploy and activate the Z probe
 *
 * Usage:
 *   M401 [H] [S<bool>] [R<bool>]
 *
 * Parameters:
 * @param  R  : Remain in place after deploying (and before activating) the probe
 *
 * With BLTOUCH_HS_MODE:
 * @param  H  : Report the current BLTouch HS mode state and exit
 * @param  S  : Set High Speed (HS) Mode and exit without deploy
 *
 */
void GcodeSuite::M401() {
  #if HAS_BLTOUCH_HS_MODE
    const bool seenH = parser.seen_test('H'),
               seenS = parser.seen('S');
    if (seenH || seenS) {
      if (seenS) bltouch.high_speed_mode = parser.value_bool();
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM("BLTouch HS mode ");
      serialprintln_onoff(bltouch.high_speed_mode);
      return;
    }
  #endif

  probe.deploy(parser.boolval('R'));
  TERN_(PROBE_TARE, probe.tare());
  report_current_position();
}

/**
 * @brief M402: Deactivate and stow the Z probe
 *
 * Usage:
 *   M402 [R<bool>]
 *
 * Parameters:
 * @param  R  : Remain in place after stowing (and before deactivating) the probe
 */
void GcodeSuite::M402() {
  probe.stow(parser.boolval('R'));
  #ifdef Z_AFTER_PROBING
    do_z_clearance(Z_AFTER_PROBING);
  #endif
  report_current_position();
}

#endif // HAS_BED_PROBE
