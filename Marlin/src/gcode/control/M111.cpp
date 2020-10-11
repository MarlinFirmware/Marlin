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

/**
 * M111: Set the debug level
 */
void GcodeSuite::M111() {
  if (parser.seen('S')) marlin_debug_flags = parser.byteval('S');

  static PGMSTR(str_debug_1, STR_DEBUG_ECHO);
  static PGMSTR(str_debug_2, STR_DEBUG_INFO);
  static PGMSTR(str_debug_4, STR_DEBUG_ERRORS);
  static PGMSTR(str_debug_8, STR_DEBUG_DRYRUN);
  static PGMSTR(str_debug_16, STR_DEBUG_COMMUNICATION);
  #if ENABLED(DEBUG_LEVELING_FEATURE)
    static PGMSTR(str_debug_lvl, STR_DEBUG_LEVELING);
  #endif

  static PGM_P const debug_strings[] PROGMEM = {
    str_debug_1, str_debug_2, str_debug_4, str_debug_8, str_debug_16,
    TERN_(DEBUG_LEVELING_FEATURE, str_debug_lvl)
  };

  SERIAL_ECHO_START();
  SERIAL_ECHOPGM(STR_DEBUG_PREFIX);
  if (marlin_debug_flags) {
    uint8_t comma = 0;
    LOOP_L_N(i, COUNT(debug_strings)) {
      if (TEST(marlin_debug_flags, i)) {
        if (comma++) SERIAL_CHAR(',');
        serialprintPGM((char*)pgm_read_ptr(&debug_strings[i]));
      }
    }
  }
  else {
    SERIAL_ECHOPGM(STR_DEBUG_OFF);
    #if !IS_AT90USB
      #if ENABLED(SERIAL_STATS_RX_BUFFER_OVERRUNS)
        SERIAL_ECHOPAIR("\nBuffer Overruns: ", MYSERIAL0.buffer_overruns());
      #endif

      #if ENABLED(SERIAL_STATS_RX_FRAMING_ERRORS)
        SERIAL_ECHOPAIR("\nFraming Errors: ", MYSERIAL0.framing_errors());
      #endif

      #if ENABLED(SERIAL_STATS_DROPPED_RX)
        SERIAL_ECHOPAIR("\nDropped bytes: ", MYSERIAL0.dropped());
      #endif

      #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
        SERIAL_ECHOPAIR("\nMax RX Queue Size: ", MYSERIAL0.rxMaxEnqueued());
      #endif
    #endif // !IS_AT90USB
  }
  SERIAL_EOL();
}
