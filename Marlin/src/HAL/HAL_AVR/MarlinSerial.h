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
 * Modified 01 October 2017 by Eduardo José Tagle (added XON/XOFF)
 */

#ifndef _MARLINSERIAL_H_
#define _MARLINSERIAL_H_

#include "../../inc/MarlinConfigPre.h"

#include <WString.h>

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

// Registers used by MarlinSerial class (expanded depending on selected serial port)
#define M_UCSRxA           SERIAL_REGNAME(UCSR,SERIAL_PORT,A) // defines M_UCSRxA to be UCSRnA where n is the serial port number
#define M_UCSRxB           SERIAL_REGNAME(UCSR,SERIAL_PORT,B)
#define M_RXENx            SERIAL_REGNAME(RXEN,SERIAL_PORT,)
#define M_TXENx            SERIAL_REGNAME(TXEN,SERIAL_PORT,)
#define M_TXCx             SERIAL_REGNAME(TXC,SERIAL_PORT,)
#define M_RXCIEx           SERIAL_REGNAME(RXCIE,SERIAL_PORT,)
#define M_UDREx            SERIAL_REGNAME(UDRE,SERIAL_PORT,)
#define M_FEx              SERIAL_REGNAME(FE,SERIAL_PORT,)
#define M_DORx             SERIAL_REGNAME(DOR,SERIAL_PORT,)
#define M_UPEx             SERIAL_REGNAME(UPE,SERIAL_PORT,)
#define M_UDRIEx           SERIAL_REGNAME(UDRIE,SERIAL_PORT,)
#define M_UDRx             SERIAL_REGNAME(UDR,SERIAL_PORT,)
#define M_UBRRxH           SERIAL_REGNAME(UBRR,SERIAL_PORT,H)
#define M_UBRRxL           SERIAL_REGNAME(UBRR,SERIAL_PORT,L)
#define M_RXCx             SERIAL_REGNAME(RXC,SERIAL_PORT,)
#define M_USARTx_RX_vect   SERIAL_REGNAME(USART,SERIAL_PORT,_RX_vect)
#define M_U2Xx             SERIAL_REGNAME(U2X,SERIAL_PORT,)
#define M_USARTx_UDRE_vect SERIAL_REGNAME(USART,SERIAL_PORT,_UDRE_vect)

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

#ifndef USBCON
  // We're using a ring buffer (I think), in which rx_buffer_head is the index of the
  // location to which to write the next incoming character and rx_buffer_tail is the
  // index of the location from which to read.
  #if RX_BUFFER_SIZE > 256
    typedef uint16_t ring_buffer_pos_t;
  #else
    typedef uint8_t ring_buffer_pos_t;
  #endif

  #if ENABLED(SERIAL_STATS_DROPPED_RX)
    extern uint8_t rx_dropped_bytes;
  #endif

  #if ENABLED(SERIAL_STATS_RX_BUFFER_OVERRUNS)
    extern uint8_t rx_buffer_overruns;
  #endif

  #if ENABLED(SERIAL_STATS_RX_FRAMING_ERRORS)
    extern uint8_t rx_framing_errors;
  #endif

  #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
    extern ring_buffer_pos_t rx_max_enqueued;
  #endif

  class MarlinSerial {

    public:
      MarlinSerial() {};
      static void begin(const long);
      static void end();
      static int peek(void);
      static int read(void);
      static void flush(void);
      static ring_buffer_pos_t available(void);
      static void write(const uint8_t c);
      static void flushTX(void);

      #if ENABLED(SERIAL_STATS_DROPPED_RX)
        FORCE_INLINE static uint32_t dropped() { return rx_dropped_bytes; }
      #endif

      #if ENABLED(SERIAL_STATS_RX_BUFFER_OVERRUNS)
        FORCE_INLINE static uint32_t buffer_overruns() { return rx_buffer_overruns; }
      #endif

      #if ENABLED(SERIAL_STATS_RX_FRAMING_ERRORS)
        FORCE_INLINE static uint32_t framing_errors() { return rx_framing_errors; }
      #endif

      #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
        FORCE_INLINE static ring_buffer_pos_t rxMaxEnqueued() { return rx_max_enqueued; }
      #endif

      FORCE_INLINE static void write(const char* str) { while (*str) write(*str++); }
      FORCE_INLINE static void write(const uint8_t* buffer, size_t size) { while (size--) write(*buffer++); }
      FORCE_INLINE static void print(const String& s) { for (int i = 0; i < (int)s.length(); i++) write(s[i]); }
      FORCE_INLINE static void print(const char* str) { write(str); }

      static void print(char, int = BYTE);
      static void print(unsigned char, int = BYTE);
      static void print(int, int = DEC);
      static void print(unsigned int, int = DEC);
      static void print(long, int = DEC);
      static void print(unsigned long, int = DEC);
      static void print(double, int = 2);

      static void println(const String& s);
      static void println(const char[]);
      static void println(char, int = BYTE);
      static void println(unsigned char, int = BYTE);
      static void println(int, int = DEC);
      static void println(unsigned int, int = DEC);
      static void println(long, int = DEC);
      static void println(unsigned long, int = DEC);
      static void println(double, int = 2);
      static void println(void);
      operator bool() { return true; }

    private:
      static void printNumber(unsigned long, const uint8_t);
      static void printFloat(double, uint8_t);
  };

  extern MarlinSerial customizedSerial;

#endif // !USBCON

// Use the UART for Bluetooth in AT90USB configurations
#if defined(USBCON) && ENABLED(BLUETOOTH)
  extern HardwareSerial bluetoothSerial;
#endif

#endif // _MARLINSERIAL_H_
