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

//
// Serial aliases for debugging.
// Include this header after defining DEBUG_OUT
//  (or not) in a given .cpp file
//

#undef DEBUG_SECTION
#undef DEBUG_ECHO_START
#undef DEBUG_ERROR_START
#undef DEBUG_WARN_START
#undef DEBUG_CHAR
#undef DEBUG_ECHO
#undef DEBUG_ECHOLN
#undef DEBUG_ECHOPGM
#undef DEBUG_ECHOLNPGM
#undef DEBUG_ECHOPGM_P
#undef DEBUG_ECHOLNPGM_P
#undef DEBUG_ECHO_MSG
#undef DEBUG_ERROR_MSG
#undef DEBUG_WARN_MSG
#undef DEBUG_ECHO_TERNARY
#undef DEBUG_EOL
#undef DEBUG_FLUSH
#undef DEBUG_POS
#undef DEBUG_XYZ
#undef DEBUG_DELAY
#undef DEBUG_SYNCHRONIZE

#if DEBUG_OUT

  #include "debug_section.h"
  #define DEBUG_SECTION(N,S,D)    SectionLog N(F(S),D)

  #define DEBUG_ECHO_START        SERIAL_ECHO_START
  #define DEBUG_ERROR_START       SERIAL_ERROR_START
  #define DEBUG_WARN_START        SERIAL_WARN_START
  #define DEBUG_CHAR              SERIAL_CHAR
  #define DEBUG_ECHO              SERIAL_ECHO
  #define DEBUG_ECHOLN            SERIAL_ECHOLN
  #define DEBUG_ECHOPGM           SERIAL_ECHOPGM
  #define DEBUG_ECHOLNPGM         SERIAL_ECHOLNPGM
  #define DEBUG_ECHOPGM           SERIAL_ECHOPGM
  #define DEBUG_ECHOPGM_P         SERIAL_ECHOPGM_P
  #define DEBUG_ECHOLNPGM         SERIAL_ECHOLNPGM
  #define DEBUG_ECHOLNPGM_P       SERIAL_ECHOLNPGM_P
  #define DEBUG_ECHO_MSG          SERIAL_ECHO_MSG
  #define DEBUG_ERROR_MSG         SERIAL_ERROR_MSG
  #define DEBUG_WARN_MSG          SERIAL_WARN_MSG
  #define DEBUG_ECHO_TERNARY      SERIAL_ECHO_TERNARY
  #define DEBUG_EOL               SERIAL_EOL
  #define DEBUG_FLUSH             SERIAL_FLUSH
  #define DEBUG_POS               SERIAL_POS
  #define DEBUG_XYZ               SERIAL_XYZ
  #define DEBUG_DELAY(ms)         serial_delay(ms)
  #define DEBUG_SYNCHRONIZE()     planner.synchronize()

#else

  #define DEBUG_SECTION(...)        NOOP
  #define DEBUG_ECHO_START()        NOOP
  #define DEBUG_ERROR_START()       NOOP
  #define DEBUG_WARN_START()        NOOP
  #define DEBUG_CHAR(...)           NOOP
  #define DEBUG_ECHO(...)           NOOP
  #define DEBUG_ECHOLN(...)         NOOP
  #define DEBUG_ECHOPGM(...)        NOOP
  #define DEBUG_ECHOLNPGM(...)      NOOP
  #define DEBUG_ECHOPGM_P(...)      NOOP
  #define DEBUG_ECHOLNPGM_P(...)    NOOP
  #define DEBUG_ECHO_MSG(...)       NOOP
  #define DEBUG_ERROR_MSG(...)      NOOP
  #define DEBUG_WARN_MSG(...)       NOOP
  #define DEBUG_ECHO_TERNARY(...)   NOOP
  #define DEBUG_EOL()               NOOP
  #define DEBUG_FLUSH()             NOOP
  #define DEBUG_POS(...)            NOOP
  #define DEBUG_XYZ(...)            NOOP
  #define DEBUG_DELAY(...)          NOOP
  #define DEBUG_SYNCHRONIZE()       NOOP

#endif

#undef DEBUG_OUT
