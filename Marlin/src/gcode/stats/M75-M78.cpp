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

#include "../gcode.h"
#include "../../module/printcounter.h"
#include "../../lcd/marlinui.h"
#if ENABLED(HOST_PAUSE_M76)
  #include "../../feature/host_actions.h"
#endif

#include "../../MarlinCore.h" // for startOrResumeJob

#if ENABLED(DWIN_CREALITY_LCD)
  #include "../../lcd/e3v2/creality/dwin.h"
#endif

/**
 * M75: Start print timer
 */
void GcodeSuite::M75() {
  startOrResumeJob();
  #if ENABLED(DWIN_CREALITY_LCD)
    DWIN_Print_Header(parser.string_arg && parser.string_arg[0] ? parser.string_arg : GET_TEXT(MSG_HOST_START_PRINT));
    DWIN_Print_Started(false);
  #endif
}

/**
 * M76: Pause print timer
 */
void GcodeSuite::M76() {
  print_job_timer.pause();
  TERN_(HOST_PAUSE_M76, host_action_pause());
}

/**
 * M77: Stop print timer
 */
void GcodeSuite::M77() {
  print_job_timer.stop();
  TERN_(DWIN_CREALITY_LCD, DWIN_Print_Finished());
}

#if ENABLED(PRINTCOUNTER)

  #if ENABLED(PASSWORD_FEATURE)
    #include "../../feature/password/password.h"
  #endif

  /**
   * M78: Show print statistics
   *
   * Parameters:
   *
   *  S78      Use M78 S78 to reset all statistics
   *
   * With SERVICE_INTERVALS:
   *
   *  R<index> Reset service interval 1, 2, or 3 so warnings won't
   *           appear until the next service interval expires.
   *
   * With PASSWORD_FEATURE:
   *
   *  P<passcode> - The correct passcode is required for reset.
   */
  void GcodeSuite::M78() {
    const bool reset_all = parser.intval('S') == 78,
               reset_some = TERN0(HAS_SERVICE_INTERVALS, parser.seenval('R'));

    #if ENABLED(PASSWORD_FEATURE)
      bool authorized = false;
      if ((reset_all || reset_some) && password.is_set) {
        if (!parser.seenval('P'))
          SERIAL_ECHOLNPGM(STR_PASSWORD_REQUIRED);
        else if (parser.value_ulong() != password.value)
          SERIAL_ECHOLNPGM(STR_WRONG_PASSWORD);
        else
          authorized = true;
      }
    #else
      constexpr bool authorized = true;
    #endif

    if (authorized && reset_all) {  // "M78 S78" will reset the statistics
      print_job_timer.initStats();
      ui.reset_status();
    }

    #if HAS_SERVICE_INTERVALS
      if (authorized && parser.seenval('R')) {
        print_job_timer.resetServiceInterval(parser.value_int());
        ui.reset_status();
      }
    #endif

    print_job_timer.showStats();
  }

#endif // PRINTCOUNTER
