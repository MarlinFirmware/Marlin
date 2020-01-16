/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

//
// Serial aliases for debugging.
// Include this header after defining DEBUG_OUT
//  (or not) in a given .cpp file
//

#undef DEBUG_PRINT_P
#undef DEBUG_ECHO_START
#undef DEBUG_ERROR_START
#undef DEBUG_CHAR
#undef DEBUG_ECHO
#undef DEBUG_ECHO_F
#undef DEBUG_ECHOLN
#undef DEBUG_ECHOPGM
#undef DEBUG_ECHOLNPGM
#undef DEBUG_ECHOPAIR
#undef DEBUG_ECHOPAIR_P
#undef DEBUG_ECHOPAIR_F
#undef DEBUG_ECHOPAIR_F_P
#undef DEBUG_ECHOLNPAIR
#undef DEBUG_ECHOLNPAIR_P
#undef DEBUG_ECHOLNPAIR_F
#undef DEBUG_ECHOLNPAIR_F_P
#undef DEBUG_ECHO_MSG
#undef DEBUG_ERROR_MSG
#undef DEBUG_EOL
#undef DEBUG_POS
#undef DEBUG_XYZ
#undef DEBUG_DELAY

#if DEBUG_OUT
  #define DEBUG_PRINT_P(P)        serialprintPGM(P)
  #define DEBUG_ECHO_START        SERIAL_ECHO_START
  #define DEBUG_ERROR_START       SERIAL_ERROR_START
  #define DEBUG_CHAR              SERIAL_CHAR
  #define DEBUG_ECHO              SERIAL_ECHO
  #define DEBUG_ECHO_F            SERIAL_ECHO_F
  #define DEBUG_ECHOLN            SERIAL_ECHOLN
  #define DEBUG_ECHOPGM           SERIAL_ECHOPGM
  #define DEBUG_ECHOLNPGM         SERIAL_ECHOLNPGM
  #define DEBUG_ECHOPAIR          SERIAL_ECHOPAIR
  #define DEBUG_ECHOPAIR_P        SERIAL_ECHOPAIR_P
  #define DEBUG_ECHOPAIR_F        SERIAL_ECHOPAIR_F
  #define DEBUG_ECHOPAIR_F_P      SERIAL_ECHOPAIR_F_P
  #define DEBUG_ECHOLNPAIR        SERIAL_ECHOLNPAIR
  #define DEBUG_ECHOLNPAIR_P      SERIAL_ECHOLNPAIR_P
  #define DEBUG_ECHOLNPAIR_F      SERIAL_ECHOLNPAIR_F
  #define DEBUG_ECHOLNPAIR_F_P    SERIAL_ECHOLNPAIR_F_P
  #define DEBUG_ECHO_MSG          SERIAL_ECHO_MSG
  #define DEBUG_ERROR_MSG         SERIAL_ERROR_MSG
  #define DEBUG_EOL               SERIAL_EOL
  #define DEBUG_POS               SERIAL_POS
  #define DEBUG_XYZ               SERIAL_XYZ
  #define DEBUG_DELAY(ms)         serial_delay(ms)
#else
  #define DEBUG_PRINT_P(P)          NOOP
  #define DEBUG_ECHO_START()        NOOP
  #define DEBUG_ERROR_START()       NOOP
  #define DEBUG_CHAR(...)           NOOP
  #define DEBUG_ECHO(...)           NOOP
  #define DEBUG_ECHO_F(...)         NOOP
  #define DEBUG_ECHOLN(...)         NOOP
  #define DEBUG_ECHOPGM(...)        NOOP
  #define DEBUG_ECHOLNPGM(...)      NOOP
  #define DEBUG_ECHOPAIR(...)       NOOP
  #define DEBUG_ECHOPAIR_P(...)     NOOP
  #define DEBUG_ECHOPAIR_F(...)     NOOP
  #define DEBUG_ECHOPAIR_F_P(...)   NOOP
  #define DEBUG_ECHOLNPAIR(...)     NOOP
  #define DEBUG_ECHOLNPAIR_P(...)   NOOP
  #define DEBUG_ECHOLNPAIR_F(...)   NOOP
  #define DEBUG_ECHOLNPAIR_F_P(...) NOOP
  #define DEBUG_ECHO_MSG(...)       NOOP
  #define DEBUG_ERROR_MSG(...)      NOOP
  #define DEBUG_EOL()               NOOP
  #define DEBUG_POS(...)            NOOP
  #define DEBUG_XYZ(...)            NOOP
  #define DEBUG_DELAY(...)          NOOP
#endif

#undef DEBUG_OUT
