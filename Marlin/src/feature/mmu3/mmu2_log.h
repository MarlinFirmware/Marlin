/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2024 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * mmu2_log.h
 */

#pragma once
#ifdef __AVR__
  #include <avr/pgmspace.h>
#endif

// Beware - before changing this prefix, think twice
// you'd need to change appmain.cpp app_marlin_serial_output_write_hook
// and MMU2::ReportError + MMU2::ReportProgress
static constexpr char mmu2Magic[] PROGMEM = "MMU2:";

#if HAS_PRUSA_MMU3
  namespace MMU2 {

  /// Report the msg into the general logging subsystem (through Marlin's SERIAL_ECHO stuff)
  /// @param msg pointer to a string in PROGMEM
  /// On the AVR platform this variant reads the input string from PROGMEM.
  /// On the ARM platform it calls LogErrorEvent directly (silently expecting the compiler to optimize it away)
  void LogErrorEvent_P(const char *msg_P);

  /// Report the msg into the general logging subsystem (through Marlin's SERIAL_ECHO stuff)
  /// @param msg pointer to a string in PROGMEM
  /// On the AVR platform this variant reads the input string from PROGMEM.
  /// On the ARM platform it calls LogErrorEvent directly (silently expecting the compiler to optimize it away)
  void LogEchoEvent_P(const char *msg_P);

  } // namespace MMU2

  #ifndef UNITTEST
    #ifdef __AVR__
      #include "src/MarlinCore.h"
    #else
      #include "src/core/serial.h"
    #endif
    #define SERIAL_MMU2() \
            { SERIAL_ECHO_P(mmu2Magic); }

    #define MMU2_ECHO_MSGLN(S)   \
            do {                     \
              SERIAL_ECHO_START;   \
              SERIAL_MMU2();       \
              SERIAL_ECHOLN(S);    \
            } while (0)
    #define MMU2_ERROR_MSGLN(S) MMU2_ECHO_MSGLN(S) //! @todo Decide MMU errors  on serial line
    #define MMU2_ECHO_MSGRPGM(S) \
            do {                     \
              SERIAL_ECHO_START;   \
              SERIAL_MMU2();       \
              SERIAL_ECHO_P(S);  \
            } while (0)
    #define MMU2_ERROR_MSGRPGM(S) MMU2_ECHO_MSGRPGM(S) //! @todo Decide MMU errors  on serial line
    #define MMU2_ECHO_MSG(S)     \
            do {                     \
              SERIAL_ECHO_START;   \
              SERIAL_MMU2();       \
              SERIAL_ECHO(S);      \
            } while (0)
    #define MMU2_ERROR_MSG(S) MMU2_ECHO_MSG(S) //! @todo Decide MMU errors  on serial line

  #else                                          // #ifndef UNITTEST
    #include "stubs/stub_interfaces.h"
    #define MMU2_ECHO_MSGLN(S)    marlinLogSim.AppendLine(S)
    #define MMU2_ERROR_MSGLN(S)   marlinLogSim.AppendLine(S)
    #define MMU2_ECHO_MSGRPGM(S) /*marlinLogSim.AppendLine(S)*/
    #define MMU2_ERROR_MSGRPGM(S) /*marlinLogSim.AppendLine(S)*/
    #define SERIAL_ECHOLNPGM(S) /*marlinLogSim.AppendLine(S)*/
    #define SERIAL_ECHOPGM(S)   /* */
    #define SERIAL_ECHOLN(S)    /*marlinLogSim.AppendLine(S)*/

  #endif                            // #ifndef UNITTEST
#endif // HAS_PRUSA_MMU3
