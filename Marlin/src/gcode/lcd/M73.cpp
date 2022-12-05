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

#if ENABLED(SET_PROGRESS_MANUALLY)

#include "../gcode.h"
#include "../../lcd/marlinui.h"
#include "../../sd/cardreader.h"
#include "../../libs/numtostr.h"

#if ENABLED(DWIN_LCD_PROUI)
  #include "../../lcd/e3v2/proui/dwin.h"
#endif

#if ENABLED(M73_REPORT)
  #define M73_REPORT_PRUSA
#endif

/**
 * M73: Set percentage complete (for display on LCD)
 *
 * Example:
 *   M73 P25.63 ; Set progress to 25.63%
 *   M73 R456   ; Set remaining time to 456 minutes
 *   M73 C12    ; Set next interaction countdown to 12 minutes
 *   M73        ; Report current values
 *
 * Use a shorter-than-Průša report format:
 * M73 Percent done: ---%; Time left: -----m; Change: -----m;
 *
 * When PRINT_PROGRESS_SHOW_DECIMALS is enabled - reports percent with 100 / 23.4 / 3.45 format
 *
 */
void GcodeSuite::M73() {

  #if ENABLED(DWIN_LCD_PROUI)

    DWIN_M73();

  #else

    #if ENABLED(SET_PROGRESS_PERCENT)
      if (parser.seenval('P'))
        ui.set_progress((PROGRESS_SCALE) > 1
          ? parser.value_float() * (PROGRESS_SCALE)
          : parser.value_byte()
        );
    #endif

    #if ENABLED(SET_REMAINING_TIME)
      if (parser.seenval('R')) ui.set_remaining_time(60 * parser.value_ulong());
    #endif

    #if ENABLED(SET_INTERACTION_TIME)
      if (parser.seenval('C')) ui.set_interaction_time(60 * parser.value_ulong());
    #endif

  #endif

  #if ENABLED(M73_REPORT)
  {
    SERIAL_ECHO_MSG(
        TERN(M73_REPORT_PRUSA, "M73 Percent done: ", "Progress: ")
      , TERN(PRINT_PROGRESS_SHOW_DECIMALS, permyriadtostr4(ui.get_progress_permyriad()), ui.get_progress_percent())
      #if ENABLED(SET_REMAINING_TIME)
        , TERN(M73_REPORT_PRUSA, "; Print time remaining in mins: ", "%; Time left: "), ui.remaining_time / 60
      #endif
      #if ENABLED(SET_INTERACTION_TIME)
        , TERN(M73_REPORT_PRUSA, "; Change in mins: ", "m; Change: "), ui.interaction_time / 60
      #endif
      , TERN(M73_REPORT_PRUSA, ";", "m")
    );
  }
  #endif
}

#endif // SET_PROGRESS_MANUALLY
