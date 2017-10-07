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

#ifndef HAL_SERIAL_H_
#define HAL_SERIAL_H_

#include <stdarg.h>
#include <stdio.h>

extern "C" {
#include <debug_frmwrk.h>
}

/*
 * Generic RingBuffer
 * T type of the buffer array
 * S size of the buffer (must be power of 2)
 *
 * todo: optimise
 */
template <typename T, uint32_t S> class RingBuffer {
public:
  RingBuffer() {
    index_read = 0;
    index_write = 0;
  }
  bool peek(T *value) volatile {
    if(value == 0 || available() == 0)
      return false;
    *value = buffer[buffer_mask & index_read];
    return true;
  }
  uint32_t available() volatile {
    return buffer_mask & (index_write - index_read);
  }
  uint32_t free() volatile {
    return buffer_size - available();
  }
  T read() volatile {
    if((buffer_mask & index_read) == (buffer_mask & index_write)) return T(-1);
    T val = buffer[buffer_mask & index_read];
    ++index_read;
    return val;
  }
  bool write( T value) volatile {
    uint32_t next_head = buffer_mask & (index_write + 1);
    if(next_head != index_read) {
      buffer[buffer_mask & index_write] = value;
      index_write = next_head;
      return true;
    }
    return false;
  }
  bool empty() volatile {
    return (buffer_mask & index_read) == (buffer_mask & index_write);
  }
  bool full() volatile {
    return index_read == buffer_mask & (index_write + 1);
  }
  void clear() volatile {
    index_read = index_write = 0;
  }

private:
  static const uint32_t buffer_size = S;
  static const uint32_t buffer_mask = buffer_size - 1;
  volatile T buffer[buffer_size];
  volatile uint32_t index_write;
  volatile uint32_t index_read;
};

class HalSerial {
public:
  HalSerial() {
    host_connected = false;
  }

  void begin(int32_t baud) {
  }

  char read() {
    return (char)receive_buffer.read();
  }

  void write(char c) {
    _DBC(c); //Duplicate output over uart0
    if(host_connected) transmit_buffer.write((uint8_t)c);
  }

  operator bool() {
    return host_connected;
  }

  uint16_t available() {
    return (uint16_t)receive_buffer.available();
  }

  void flush() {
    receive_buffer.clear();
  }

  uint8_t availableForWrite(void){
    return transmit_buffer.free() > 255 ? 255 : (uint8_t)transmit_buffer.free();
  }

  void flushTX(void){
    if(host_connected) {
      while(transmit_buffer.available());
    }
  }

  void printf(const char *format, ...) {
    static char buffer[256];
    va_list vArgs;
    va_start(vArgs, format);
    int length = vsnprintf((char *) buffer, 256, (char const *) format, vArgs);
    va_end(vArgs);
    if (length > 0 && length < 256) {
      _DBG(buffer); //Duplicate output over uart0
      if (host_connected) {
        for (int i = 0; i < length;) {
          if (transmit_buffer.write(buffer[i])) {
            ++i;
          }
        }
      }
    }
  }

  void print(const char value[]) {
    printf("%s" , value);
  }
  void print(char value, int = 0) {
    printf("%c" , value);
  }
  void print(unsigned char value, int = 0) {
    printf("%u" , value);
  }
  void print(int value, int = 0) {
    printf("%d" , value);
  }
  void print(unsigned int value, int = 0) {
    printf("%u" , value);
  }
  void print(long value, int = 0) {
    printf("%ld" , value);
  }
  void print(unsigned long value, int = 0) {
    printf("%lu" , value);
  }

  void print(float value, int round = 6) {
    printf("%f" , value);
  }
  void print(double value, int round = 6) {
    printf("%f" , value );
  }

  void println(const char value[]) {
    printf("%s\n" , value);
  }
  void println(char value, int = 0) {
    printf("%c\n" , value);
  }
  void println(unsigned char value, int = 0) {
    printf("%u\r\n" , value);
  }
  void println(int value, int = 0) {
    printf("%d\n" , value);
  }
  void println(unsigned int value, int = 0) {
    printf("%u\n" , value);
  }
  void println(long value, int = 0) {
    printf("%ld\n" , value);
  }
  void println(unsigned long value, int = 0) {
    printf("%lu\n" , value);
  }
  void println(float value, int round = 6) {
    printf("%f\n" , value );
  }
  void println(double value, int round = 6) {
    printf("%f\n" , value );
  }
  void println(void) {
    print('\n');
  }
  volatile RingBuffer<uint8_t, 128> receive_buffer;
  volatile RingBuffer<uint8_t, 128> transmit_buffer;
  volatile bool host_connected;
};


#endif /* MARLIN_SRC_HAL_HAL_SERIAL_H_ */
