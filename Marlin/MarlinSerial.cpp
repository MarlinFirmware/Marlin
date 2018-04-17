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

/**
 * MarlinSerial.cpp - Hardware serial library for Wiring
 * Copyright (c) 2006 Nicholas Zambetti.  All right reserved.
 *
 * Modified 23 November 2006 by David A. Mellis
 * Modified 28 September 2010 by Mark Sproul
 * Modified 14 February 2016 by Andreas Hardtung (added tx buffer)
 * Modified 01 October 2017 by Eduardo José Tagle (added XON/XOFF)
 * Modified 15 April 2018 by Stephan Veigl (multi instance version)
 */

// Disable HardwareSerial.cpp to support chips without a UART (Attiny, etc.)

#include "MarlinConfig.h"

#include "MarlinSerial.h"
#include "Marlin.h"

// myserial is the currently active serial port
// For the beginning (and for single port systems) we use the primary serial port.
Stream *myserial = &PRIM_SERIAL;

#if !(defined(__AVR__) && defined(USBCON)) && (defined(UBRRH) || defined(UBRR0H) || defined(UBRR1H) || defined(UBRR2H) || defined(UBRR3H))

  #include "MarlinSerial.h"
  #include "Marlin.h"

  #if ENABLED(SERIAL_XON_XOFF)
    constexpr uint8_t XON_XOFF_CHAR_SENT = 0x80;  // XON / XOFF Character was sent
    constexpr uint8_t XON_XOFF_CHAR_MASK = 0x1F;  // XON / XOFF character to send
    // XON / XOFF character definitions
    constexpr uint8_t XON_CHAR  = 17;
    constexpr uint8_t XOFF_CHAR = 19;
  #endif

  #if ENABLED(EMERGENCY_PARSER)
    bool killed_by_M112 = false;
  #endif

  // include actual implementation of MarlinSerial for SERIAL_PORT
  #define T_PORT SERIAL_PORT
  #include "MarlinSerial_Template.cpp"
  #undef T_PORT
  #ifdef SEC_SERIAL_PORT
    #define T_PORT SEC_SERIAL_PORT
    #include "MarlinSerial_Template.cpp"
    #undef T_PORT
  #endif
#endif // !(__AVR__ && USBCON) && (UBRRH || UBRR0H || UBRR1H || UBRR2H || UBRR3H)

// For AT90USB targets use the UART for BT interfacing
#if defined(__AVR__) && defined(USBCON) && ENABLED(BLUETOOTH)
  HardwareSerial bluetoothSerial;
#endif
