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

#ifndef _HAL_SERIAL_H_
#define _HAL_SERIAL_H_

#include "../../../inc/MarlinConfigPre.h"
#if ENABLED(EMERGENCY_PARSER)
  #include "../../../feature/emergency_parser.h"
#endif

#include <stdarg.h>
#include <stdio.h>
#include <Print.h>

/**
 * Generic RingBuffer
 * T type of the buffer array
 * S size of the buffer (must be power of 2)
 */

template <typename T, uint32_t S> class RingBuffer {
public:
  RingBuffer() {index_read = index_write = 0;}

  uint32_t available() {return mask(index_write - index_read);}
  uint32_t free() {return buffer_size - available();}
  bool empty() {return index_read == index_write;}
  bool full() {return next(index_write) == index_read;}
  void clear() {index_read = index_write = 0;}

  bool peek(T *const value) {
    if (value == nullptr || empty()) return false;
    *value = buffer[index_read];
    return true;
  }

  uint32_t read(T *const value) {
    if (value == nullptr || empty()) return 0;
    *value = buffer[index_read];
    index_read = next(index_read);
    return 1;
  }

  uint32_t write(T value) {
    uint32_t next_head = next(index_write);
    if (next_head == index_read) return 0;     // buffer full
    buffer[index_write] = value;
    index_write = next_head;
    return 1;
  }

private:
  inline uint32_t mask(uint32_t val) {
    return val & buffer_mask;
  }

  inline uint32_t next(uint32_t val) {
    return mask(val + 1);
  }

  static const uint32_t buffer_size = S;
  static const uint32_t buffer_mask = buffer_size - 1;
  T buffer[buffer_size];
  volatile uint32_t index_write;
  volatile uint32_t index_read;
};

/**
 *  Serial Interface Class
 *  Data is injected directly into, and consumed from, the fifo buffers
 */

class HalSerial: public Print {
public:

  #if ENABLED(EMERGENCY_PARSER)
    EmergencyParser::State emergency_state;
  #endif

  HalSerial() : host_connected(false) { }
  virtual ~HalSerial() { }

  operator bool() { return host_connected; }

  void begin(int32_t baud) { }

  int16_t peek() {
    uint8_t value;
    return receive_buffer.peek(&value) ? value : -1;
  }

  int16_t read() {
    uint8_t value;
    return receive_buffer.read(&value) ? value : -1;
  }

  size_t write(const uint8_t c) {
    if (!host_connected) return 0;          // Do not fill buffer when host disconnected
    while (transmit_buffer.write(c) == 0) { // Block until there is free room in buffer
      if (!host_connected) return 0;        // Break infinite loop on host disconect
    }
    return 1;
  }

  size_t available() {
    return (size_t)receive_buffer.available();
  }

  void flush() {
    receive_buffer.clear();
  }

  uint8_t availableForWrite(void) {
    return transmit_buffer.free() > 255 ? 255 : (uint8_t)transmit_buffer.free();
  }

  void flushTX(void) {
    while (transmit_buffer.available() && host_connected) { /* nada */}
  }

  size_t printf(const char *format, ...) {
    static char buffer[256];
    va_list vArgs;
    va_start(vArgs, format);
    int length = vsnprintf((char *) buffer, 256, (char const *) format, vArgs);
    va_end(vArgs);
    size_t i = 0;
    if (length > 0 && length < 256) {
      while (i < (size_t)length && host_connected) {
        i += transmit_buffer.write(buffer[i]);
      }
    }
    return i;
  }

  RingBuffer<uint8_t, 128> receive_buffer;
  RingBuffer<uint8_t, 128> transmit_buffer;
  volatile bool host_connected;
};

#endif // _HAL_SERIAL_H_
