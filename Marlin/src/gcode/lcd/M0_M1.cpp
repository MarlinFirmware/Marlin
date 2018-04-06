/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../../inc/MarlinConfig.h"

#if HAS_RESUME_CONTINUE

#include "../gcode.h"
#include "../../module/stepper.h"

#if ENABLED(ULTIPANEL)
  #include "../../lcd/ultralcd.h"
#endif

#include "../../sd/cardreader.h"

#if ENABLED(PRINTER_EVENT_LEDS) && ENABLED(SDSUPPORT)
  bool GcodeSuite::lights_off_after_print;
  #include "../../feature/leds/leds.h"
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

  #if ENABLED(ULTIPANEL)

    if (has_message)
      lcd_setstatus(args, true);
    else {
      LCD_MESSAGEPGM(MSG_USERWAIT);
      #if ENABLED(LCD_PROGRESS_BAR) && PROGRESS_MSG_EXPIRE > 0
        dontExpireStatus();
      #endif
    }

  #else

    if (has_message) {
      SERIAL_ECHO_START();
      SERIAL_ECHOLN(args);
    }

  #endif

  KEEPALIVE_STATE(PAUSED_FOR_USER);
  wait_for_user = true;

  stepper.synchronize();

  if (ms > 0) {
    ms += millis();  // wait until this time for a click
    while (PENDING(millis(), ms) && wait_for_user) idle();
  }
  else {
    #if ENABLED(ULTIPANEL)
      if (lcd_detected())
    #endif
        while (wait_for_user) idle();
  }

  #if ENABLED(PRINTER_EVENT_LEDS) && ENABLED(SDSUPPORT)
    if (lights_off_after_print) {
      leds.set_off();
      lights_off_after_print = false;
    }
  #endif

  #if ENABLED(ULTIPANEL)
    if (lcd_detected()) {
      IS_SD_PRINTING ? LCD_MESSAGEPGM(MSG_RESUMING) : LCD_MESSAGEPGM(WELCOME_MSG);
    }
  #endif

  wait_for_user = false;
  KEEPALIVE_STATE(IN_HANDLER);
}

#endif // HAS_RESUME_CONTINUE
