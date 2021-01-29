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

#include "../inc/MarlinConfigPre.h"

#if ENABLED(EMERGENCY_PARSER)
  #include "../feature/e_parser.h"
#endif

#ifndef DEC
  #define DEC 10
  #define HEX 16
  #define OCT 8
  #define BIN 2
#endif

// flushTX is not implemented in all HAL, so use SFINAE to call the method where it is.
CALL_IF_EXISTS_IMPL(void, flushTX );
CALL_IF_EXISTS_IMPL(bool, connected, true);

// Using Curiously Recurring Template Pattern here to avoid virtual table cost when compiling.
// Since the real serial class is known at compile time, this results in compiler writing a completely
// efficient code
template <class Child>
struct SerialBase {
  #if ENABLED(EMERGENCY_PARSER)
    const bool ep_enabled;
    EmergencyParser::State emergency_state;
    inline bool emergency_parser_enabled() { return ep_enabled; }
    SerialBase(bool ep_capable) : ep_enabled(ep_capable), emergency_state(EmergencyParser::State::EP_RESET) {}
  #else
    SerialBase(const bool) {}
  #endif

  // Static dispatch methods below:
  // The most important method here is where it all ends to:
  size_t write(uint8_t c)           { return static_cast<Child*>(this)->write(c); }
  // Called when the parser finished processing an instruction, usually build to nothing
  void msgDone()                    { static_cast<Child*>(this)->msgDone(); }
  // Called upon initialization
  void begin(const long baudRate)   { static_cast<Child*>(this)->begin(baudRate); }
  // Called upon destruction
  void end()                        { static_cast<Child*>(this)->end(); }
  /** Check for available data from the port
      @param index  The port index, usually 0 */
  bool available(uint8_t index = 0) { return static_cast<Child*>(this)->available(index); }
  /** Read a value from the port
      @param index  The port index, usually 0 */
  int  read(uint8_t index = 0)      { return static_cast<Child*>(this)->read(index); }
  // Check if the serial port is connected (usually bypassed)
  bool connected()                  { return static_cast<Child*>(this)->connected(); }
  // Redirect flush
  void flush()                      { static_cast<Child*>(this)->flush(); }
  // Not all implementation have a flushTX, so let's call them only if the child has the implementation
  void flushTX()                    { CALL_IF_EXISTS(void, static_cast<Child*>(this), flushTX); }

  // Glue code here
  FORCE_INLINE void write(const char* str)                    { while (*str) write(*str++); }
  FORCE_INLINE void write(const uint8_t* buffer, size_t size) { while (size--) write(*buffer++); }
  FORCE_INLINE void print(const char* str)                    { write(str); }
  NO_INLINE void print(char c, int base = 0)               { print((long)c, base); }
  NO_INLINE void print(unsigned char c, int base = 0)      { print((unsigned long)c, base); }
  NO_INLINE void print(int c, int base = DEC)              { print((long)c, base); }
  NO_INLINE void print(unsigned int c, int base = DEC)     { print((unsigned long)c, base); }
  void print(long c, int base = DEC)            { if (!base) write(c); write((const uint8_t*)"-", c < 0); printNumber(c < 0 ? -c : c, base); }
  void print(unsigned long c, int base = DEC)   { printNumber(c, base); }
  void print(double c, int digits = 2)          { printFloat(c, digits); }

  NO_INLINE void println(const char s[])                  { print(s); println(); }
  NO_INLINE void println(char c, int base = 0)            { print(c, base); println(); }
  NO_INLINE void println(unsigned char c, int base = 0)   { print(c, base); println(); }
  NO_INLINE void println(int c, int base = DEC)           { print(c, base); println(); }
  NO_INLINE void println(unsigned int c, int base = DEC)  { print(c, base); println(); }
  NO_INLINE void println(long c, int base = DEC)          { print(c, base); println(); }
  NO_INLINE void println(unsigned long c, int base = DEC) { print(c, base); println(); }
  NO_INLINE void println(double c, int digits = 2)        { print(c, digits); println(); }
  NO_INLINE void println()                                { write('\r'); write('\n'); }

  // Print a number with the given base
  void printNumber(unsigned long n, const uint8_t base) {
    if (n) {
      unsigned char buf[8 * sizeof(long)]; // Enough space for base 2
      int8_t i = 0;
      while (n) {
        buf[i++] = n % base;
        n /= base;
      }
      while (i--) write((char)(buf[i] + (buf[i] < 10 ? '0' : 'A' - 10)));
    }
    else write('0');
  }

  // Print a decimal number
  void printFloat(double number, uint8_t digits) {
    // Handle negative numbers
    if (number < 0.0) {
      write('-');
      number = -number;
    }

    // Round correctly so that print(1.999, 2) prints as "2.00"
    double rounding = 0.5;
    LOOP_L_N(i, digits) rounding *= 0.1;
    number += rounding;

    // Extract the integer part of the number and print it
    unsigned long int_part = (unsigned long)number;
    double remainder = number - (double)int_part;
    printNumber(int_part, 10);

    // Print the decimal point, but only if there are digits beyond
    if (digits) {
      write('.');
      // Extract digits from the remainder one at a time
      while (digits--) {
        remainder *= 10.0;
        int toPrint = int(remainder);
        printNumber(toPrint, 10);
        remainder -= toPrint;
      }
    }
  }
};

// All serial instances will be built by chaining the features required for the function in a form of a template
// type definition
