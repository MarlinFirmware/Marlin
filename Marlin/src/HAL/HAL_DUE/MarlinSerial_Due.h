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
 * MarlinSerial_Due.h - Hardware serial library for Arduino DUE
 * Copyright (c) 2017 Eduardo José Tagle. All right reserved
 * Based on MarlinSerial for AVR, copyright (c) 2006 Nicholas Zambetti.  All right reserved.
 */

#ifndef MARLINSERIAL_DUE_H
#define MARLINSERIAL_DUE_H

#include "../../inc/MarlinConfig.h"

#if SERIAL_PORT >= 0

#include <WString.h>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

// Define constants and variables for buffering incoming serial data.  We're
// using a ring buffer (I think), in which rx_buffer_head is the index of the
// location to which to write the next incoming character and rx_buffer_tail
// is the index of the location from which to read.
// 256 is the max limit due to uint8_t head and tail. Use only powers of 2. (...,16,32,64,128,256)
#ifndef RX_BUFFER_SIZE
  #define RX_BUFFER_SIZE 128
#endif
#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 32
#endif

//#if ENABLED(SERIAL_XON_XOFF) && RX_BUFFER_SIZE < 1024
//  #error "SERIAL_XON_XOFF requires RX_BUFFER_SIZE >= 1024 for reliable transfers without drops."
//#elif RX_BUFFER_SIZE && (RX_BUFFER_SIZE < 2 || !IS_POWER_OF_2(RX_BUFFER_SIZE))
//  #error "RX_BUFFER_SIZE must be a power of 2 greater than 1."
//#elif TX_BUFFER_SIZE && (TX_BUFFER_SIZE < 2 || TX_BUFFER_SIZE > 256 || !IS_POWER_OF_2(TX_BUFFER_SIZE))
//  #error "TX_BUFFER_SIZE must be 0, a power of 2 greater than 1, and no greater than 256."
//#endif

#if RX_BUFFER_SIZE > 256
  typedef uint16_t ring_buffer_pos_t;
#else
  typedef uint8_t ring_buffer_pos_t;
#endif

#if ENABLED(SERIAL_STATS_DROPPED_RX)
  extern uint8_t rx_dropped_bytes;
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
  static void checkRx(void);
  static void write(const uint8_t c);
  #if TX_BUFFER_SIZE > 0
    static uint8_t availableForWrite(void);
    static void flushTX(void);
  #endif
  static void writeNoHandshake(const uint8_t c);

  #if ENABLED(SERIAL_STATS_DROPPED_RX)
    FORCE_INLINE static uint32_t dropped() { return rx_dropped_bytes; }
  #endif

  #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
    FORCE_INLINE static ring_buffer_pos_t rxMaxEnqueued() { return rx_max_enqueued; }
  #endif

  FORCE_INLINE static void write(const char* str) { while (*str) write(*str++); }
  FORCE_INLINE static void write(const uint8_t* buffer, size_t size) { while (size--) write(*buffer++); }
  FORCE_INLINE static void print(const String& s) { for (int i = 0; i < (int)s.length(); i++) write(s[i]); }
  FORCE_INLINE static void print(const char* str) { write(str); }

  static void print(char, int = 0);
  static void print(unsigned char, int = 0);
  static void print(int, int = DEC);
  static void print(unsigned int, int = DEC);
  static void print(long, int = DEC);
  static void print(unsigned long, int = DEC);
  static void print(double, int = 2);

  static void println(const String& s);
  static void println(const char[]);
  static void println(char, int = 0);
  static void println(unsigned char, int = 0);
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

#endif // SERIAL_PORT >= 0

#endif // MARLINSERIAL_DUE_H
