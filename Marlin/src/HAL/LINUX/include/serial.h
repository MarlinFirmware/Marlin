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
#pragma once

#include "../../../inc/MarlinConfigPre.h"
#if ENABLED(EMERGENCY_PARSER)
  #include "../../../feature/e_parser.h"
#endif

#include <stdarg.h>
#include <stdio.h>

/**
 * Generic RingBuffer
 * T type of the buffer array
 * S size of the buffer (must be power of 2)
 */
template <typename T, uint32_t S> class RingBuffer {
public:
  RingBuffer() { index_read = index_write = 0; }
  uint32_t available() volatile { return index_write - index_read; }
  uint32_t free() volatile      { return buffer_size - available(); }
  bool empty() volatile         { return index_read == index_write; }
  bool full() volatile          { return available() == buffer_size; }
  void clear() volatile         { index_read = index_write = 0; }

  bool peek(T *value) volatile {
    if (value == 0 || available() == 0)
      return false;
    *value = buffer[mask(index_read)];
    return true;
  }

  int read() volatile {
    if (empty()) return -1;
    return buffer[mask(index_read++)];
  }

  bool write(T value) volatile {
    if (full()) return false;
    buffer[mask(index_write++)] = value;
    return true;
  }

private:
  uint32_t mask(uint32_t val) volatile {
    return buffer_mask & val;
  }

  static const uint32_t buffer_size = S;
  static const uint32_t buffer_mask = buffer_size - 1;
  volatile T buffer[buffer_size];
  volatile uint32_t index_write;
  volatile uint32_t index_read;
};

class HalSerial {
public:

  #if ENABLED(EMERGENCY_PARSER)
    EmergencyParser::State emergency_state;
    static inline bool emergency_parser_enabled() { return true; }
  #endif

  HalSerial() { host_connected = true; }

  void begin(int32_t) {}

  void end() {}

  int peek() {
    uint8_t value;
    return receive_buffer.peek(&value) ? value : -1;
  }

  int read() { return receive_buffer.read(); }

  size_t write(char c) {
    if (!host_connected) return 0;
    while (!transmit_buffer.free());
    return transmit_buffer.write(c);
  }

  operator bool() { return host_connected; }

  uint16_t available() {
    return (uint16_t)receive_buffer.available();
  }

  void flush() { receive_buffer.clear(); }

  uint8_t availableForWrite() {
    return transmit_buffer.free() > 255 ? 255 : (uint8_t)transmit_buffer.free();
  }

  void flushTX() {
    if (host_connected)
      while (transmit_buffer.available()) { /* nada */ }
  }

  void printf(const char *format, ...) {
    static char buffer[256];
    va_list vArgs;
    va_start(vArgs, format);
    int length = vsnprintf((char *) buffer, 256, (char const *) format, vArgs);
    va_end(vArgs);
    if (length > 0 && length < 256) {
      if (host_connected) {
        for (int i = 0; i < length;) {
          if (transmit_buffer.write(buffer[i])) {
            ++i;
          }
        }
      }
    }
  }

  #define DEC 10
  #define HEX 16
  #define OCT 8
  #define BIN 2

  void print_bin(uint32_t value, uint8_t num_digits) {
    uint32_t mask = 1 << (num_digits -1);
    for (uint8_t i = 0; i < num_digits; i++) {
      if (!(i %  4) && i) write(' ');
      if (!(i % 16) && i) write(' ');
      if (value & mask)   write('1');
      else                write('0');
      value <<= 1;
    }
  }

  void print(const char value[]) { printf("%s" , value); }
  void print(char value, int nbase = 0) {
    if (nbase == BIN) print_bin(value, 8);
    else if (nbase == OCT) printf("%3o", value);
    else if (nbase == HEX) printf("%2X", value);
    else if (nbase == DEC ) printf("%d", value);
    else printf("%c" , value);
  }
  void print(unsigned char value, int nbase = 0) {
    if (nbase == BIN) print_bin(value, 8);
    else if (nbase == OCT) printf("%3o", value);
    else if (nbase == HEX) printf("%2X", value);
    else printf("%u" , value);
  }
  void print(int value, int nbase = 0) {
    if (nbase == BIN) print_bin(value, 16);
    else if (nbase == OCT) printf("%6o", value);
    else if (nbase == HEX) printf("%4X", value);
    else printf("%d", value);
  }
  void print(unsigned int value, int nbase = 0) {
    if (nbase == BIN) print_bin(value, 16);
    else if (nbase == OCT) printf("%6o", value);
    else if (nbase == HEX) printf("%4X", value);
    else printf("%u" , value);
  }
  void print(long value, int nbase = 0) {
    if (nbase == BIN) print_bin(value, 32);
    else if (nbase == OCT) printf("%11o", value);
    else if (nbase == HEX) printf("%8X", value);
    else printf("%ld" , value);
  }
  void print(unsigned long value, int nbase = 0) {
    if (nbase == BIN) print_bin(value, 32);
    else if (nbase == OCT) printf("%11o", value);
    else if (nbase == HEX) printf("%8X", value);
    else printf("%lu" , value);
  }
  void print(float value, int round = 6)  { printf("%f" , value); }
  void print(double value, int round = 6) { printf("%f" , value); }

  void println(const char value[]) { printf("%s\n" , value); }
  void println(char value, int nbase = 0) { print(value, nbase); println(); }
  void println(unsigned char value, int nbase = 0) { print(value, nbase); println(); }
  void println(int value, int nbase = 0) { print(value, nbase); println(); }
  void println(unsigned int value, int nbase = 0) { print(value, nbase); println(); }
  void println(long value, int nbase = 0) { print(value, nbase); println(); }
  void println(unsigned long value, int nbase = 0) { print(value, nbase); println(); }
  void println(float value, int round = 6) { printf("%f\n" , value); }
  void println(double value, int round = 6) { printf("%f\n" , value); }
  void println() { print('\n'); }

  volatile RingBuffer<uint8_t, 128> receive_buffer;
  volatile RingBuffer<uint8_t, 128> transmit_buffer;
  volatile bool host_connected;
};
