/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

typedef uint16_t ring_buffer_pos_t;

struct ring_buffer_t {
  unsigned char *data;
  volatile uint8_t head, tail;
};

class WebSocketSerial {
  ring_buffer_t rx_buffer;
  ring_buffer_t tx_buffer;

  void printNumber(unsigned long, const uint8_t);
  void printFloat(double, uint8_t);

public:
  WebSocketSerial();
  ~WebSocketSerial();

  void begin(const long);
  void end();
  int peek(void);
  int read(void);
  void flush(void);
  void flushTX(void);
  bool available(void);
  void write(const uint8_t c);

  #if ENABLED(SERIAL_STATS_DROPPED_RX)
    FORCE_INLINE uint32_t dropped() { return 0; }
  #endif

  #if ENABLED(SERIAL_STATS_MAX_RX_QUEUED)
    FORCE_INLINE int rxMaxEnqueued() { return 0; }
  #endif

  FORCE_INLINE void write(const char* str) { while (*str) write(*str++); }
  FORCE_INLINE void write(const uint8_t* buffer, size_t size) { while (size--) write(*buffer++); }
  FORCE_INLINE void print(const String& s) { for (int i = 0; i < (int)s.length(); i++) write(s[i]); }
  FORCE_INLINE void print(const char* str) { write(str); }

  void print(char, int = 0);
  void print(unsigned char, int = 0);
  void print(int, int = DEC);
  void print(unsigned int, int = DEC);
  void print(long, int = DEC);
  void print(unsigned long, int = DEC);
  void print(double, int = 2);

  void println(const String& s);
  void println(const char[]);
  void println(char, int = 0);
  void println(unsigned char, int = 0);
  void println(int, int = DEC);
  void println(unsigned int, int = DEC);
  void println(long, int = DEC);
  void println(unsigned long, int = DEC);
  void println(double, int = 2);
  void println(void);
  operator bool() { return true; }
};

extern WebSocketSerial webSocketSerial;
