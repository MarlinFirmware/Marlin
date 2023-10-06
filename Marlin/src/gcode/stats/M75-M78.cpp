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

#if ENABLED(DWIN_LCD_PROUI)
  #include "../../lcd/e3v2/proui/dwin.h"
#endif

/**
 * M75: Start print timer
 */
void GcodeSuite::M75() {
  startOrResumeJob();
  #if ENABLED(DWIN_LCD_PROUI)
    if (!IS_SD_PRINTING()) DWIN_Print_Header(parser.string_arg && parser.string_arg[0] ? parser.string_arg : GET_TEXT(MSG_HOST_START_PRINT));
  #endif
}

/**
 * M76: Pause print timer
 */
void GcodeSuite::M76() {
  TERN(DWIN_LCD_PROUI, ui.pause_print(), print_job_timer.pause());
  TERN_(HOST_PAUSE_M76, hostui.pause());
}

/**
 * M77: Stop print timer
 */
void GcodeSuite::M77() {
  print_job_timer.stop();
}

#if ENABLED(PRINTCOUNTER)

  /**
   * M78: Show print statistics
   */
  void GcodeSuite::M78() {
    if (parser.intval('S') == 78) {  // "M78 S78" will reset the statistics
      print_job_timer.initStats();
      ui.reset_status();
      return;
    }

    #if HAS_SERVICE_INTERVALS
      if (parser.seenval('R')) {
        print_job_timer.resetServiceInterval(parser.value_int());
        ui.reset_status();
        return;
      }
    #endif

    print_job_timer.showStats();
  }

#endif // PRINTCOUNTER
