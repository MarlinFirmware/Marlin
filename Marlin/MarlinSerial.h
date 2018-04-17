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
 * MarlinSerial.h - Hardware serial library for Wiring
 * Copyright (c) 2006 Nicholas Zambetti.  All right reserved.
 *
 * Modified 28 September 2010 by Mark Sproul
 * Modified 14 February 2016 by Andreas Hardtung (added tx buffer)
 * Modified 15 April 2018 by Stephan Veigl (multi instance version)
 */

#ifndef MARLINSERIAL_H
#define MARLINSERIAL_H

#include "Stream.h"

#include "MarlinConfig.h"

#ifndef SERIAL_PORT
  #define SERIAL_PORT 0
#endif

// The presence of the UBRRH register is used to detect a UART.
#define UART_PRESENT(port) ((port == 0 && (defined(UBRRH) || defined(UBRR0H))) || \
                            (port == 1 && defined(UBRR1H)) || (port == 2 && defined(UBRR2H)) || \
                            (port == 3 && defined(UBRR3H)))

// These are macros to build serial port register names for the selected SERIAL_PORT (C preprocessor
// requires two levels of indirection to expand macro values properly)
#define SERIAL_REGNAME(registerbase,number,suffix) SERIAL_REGNAME_INTERNAL(registerbase,number,suffix)
#if SERIAL_PORT == 0 && (!defined(UBRR0H) || !defined(UDR0)) // use un-numbered registers if necessary
  #define SERIAL_REGNAME_INTERNAL(registerbase,number,suffix) registerbase##suffix
#else
  #define SERIAL_REGNAME_INTERNAL(registerbase,number,suffix) registerbase##number##suffix
#endif

// Define constants and variables for buffering serial data.
// Use only 0 or powers of 2 greater than 1
// : [0, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, ...]
#ifndef RX_BUFFER_SIZE
  #define RX_BUFFER_SIZE 128
#endif
// 256 is the max TX buffer limit due to uint8_t head and tail.
#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 32
#endif

#if !(defined(__AVR__) && defined(USBCON))
  
  #if RX_BUFFER_SIZE > 256
    typedef uint16_t ring_buffer_pos_t;
  #else
    typedef uint8_t ring_buffer_pos_t;
  #endif

  typedef struct {
    unsigned char buffer[RX_BUFFER_SIZE];
    volatile ring_buffer_pos_t head, tail;
  } ring_buffer_r;

  typedef struct {
    unsigned char buffer[TX_BUFFER_SIZE];
    volatile uint8_t head, tail;
  } ring_buffer_t;

  #if ENABLED(EMERGENCY_PARSER)
    extern bool killed_by_M112;
  #endif

  // include declaration of MarlinSerial for SERIAL_PORT
  #define T_PORT SERIAL_PORT
  #include "MarlinSerial_Template.h"
  #undef T_PORT
  #ifdef SEC_SERIAL_PORT
    #define T_PORT SEC_SERIAL_PORT
    #include "MarlinSerial_Template.h"
    #undef T_PORT
  #endif

#endif // !(__AVR__ && USBCON)

// Use the UART for Bluetooth in AT90USB configurations
#if defined(__AVR__) && defined(USBCON) && ENABLED(BLUETOOTH)
  extern HardwareSerial bluetoothSerial;
#endif

#endif // MARLINSERIAL_H
