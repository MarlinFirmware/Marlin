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
#pragma once

#include "../../inc/MarlinConfig.h"

#include <WString.h>

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2

#ifndef RX_BUFFER_SIZE
  #define RX_BUFFER_SIZE 128
#endif
#ifndef TX_BUFFER_SIZE
  #define TX_BUFFER_SIZE 32
#endif
#if TX_BUFFER_SIZE <= 0
  #error "TX_BUFFER_SIZE is required for the WebSocket."
#endif

#if RX_BUFFER_SIZE > 256
  typedef uint16_t ring_buffer_pos_t;
#else
  typedef uint8_t ring_buffer_pos_t;
#endif

class WebSocketSerial {
public:
  WebSocketSerial() {};
  static void begin(const long);
  static void end();
  static int peek(void);
  static int read(void);
  static void flush(void);
  static void flushTx(void);
  static bool available(void);
  static void write(const uint8_t c);

  #if ENABLED(SERIAL_STATS_DROPPED_RX)
    FORCE_INLINE static uint32_t dropped() { return 0; }
  #endif

  #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
    FORCE_INLINE static int rxMaxEnqueued() { return 0; }
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

extern WebSocketSerial webSocketSerial;
