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

#include "../../inc/MarlinConfigPre.h"

#if HAS_RESUME_CONTINUE

#include "../gcode.h"
#include "../../module/planner.h"

#include "../../inc/MarlinConfig.h"

#if HAS_LCD_MENU
  #include "../../lcd/ultralcd.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extensible_ui/ui_api.h"
#endif

#if HAS_LEDS_OFF_FLAG
  #include "../../feature/leds/printer_event_leds.h"
#endif

#if ENABLED(HOST_PROMPT_SUPPORT)
  #include "../../feature/host_actions.h"
#endif

/**
 * M0: Unconditional stop - Wait for user button press on LCD
 * M1: Conditional stop   - Wait for user button press on LCD
 */
void GcodeSuite::M0_M1() {
  const char * const args = parser.string_arg;

  millis_t ms = 0;
  bool hasP = false, hasS = false;
  if (parser.seenval('P')) {
    ms = parser.value_millis(); // milliseconds to wait
    hasP = ms > 0;
  }
  if (parser.seenval('S')) {
    ms = parser.value_millis_from_seconds(); // seconds to wait
    hasS = ms > 0;
  }

  const bool has_message = !hasP && !hasS && args && *args;

  planner.synchronize();

  #if HAS_LCD_MENU

    if (has_message)
      ui.set_status(args, true);
    else if (!parser.seenval('Q')) {
      LCD_MESSAGEPGM(MSG_USERWAIT);
      #if ENABLED(LCD_PROGRESS_BAR) && PROGRESS_MSG_EXPIRE > 0
        ui.reset_progress_bar_timeout();
      #endif
    }

  #elif ENABLED(EXTENSIBLE_UI)

    if (has_message)
      ExtUI::onUserConfirmRequired(args); // Can this take an SRAM string??
    else
      ExtUI::onUserConfirmRequired_P(GET_TEXT(MSG_USERWAIT));

  #else

    if (has_message) {
      SERIAL_ECHO_START();
      SERIAL_ECHOLN(args);
    }

  #endif

  KEEPALIVE_STATE(PAUSED_FOR_USER);
  wait_for_user = true;

  #if ENABLED(HOST_PROMPT_SUPPORT)
    host_prompt_do(PROMPT_USER_CONTINUE, PSTR("M0/1 Break Called"), CONTINUE_STR);
  #endif

  if (ms > 0) ms += millis();  // wait until this time for a click
  while (wait_for_user && (ms > 0 || PENDING(millis(), ms))) idle();

  #if HAS_LEDS_OFF_FLAG
    printerEventLEDs.onResumeAfterWait();
  #endif

  #if HAS_LCD_MENU
    ui.reset_status();
  #endif

  wait_for_user = false;
}

#endif // HAS_RESUME_CONTINUE
