/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(BTT_TFT)

#include <inttypes.h>
#include "Stream.h"
#include "uart.h"
#include "btt_tft_api.h"

class BttSerial {
  protected:
    // Has any byte been written to the UART since begin()
    bool _written;
    serial_t _serial;
  public:
    uint8_t bttRxBuf[BTT_TFT_RX_BUF_SIZE];
    uint8_t bttTxBuf[BTT_TFT_TX_BUF_SIZE];
    BttSerial(void *peripheral);

    // Set up / tear down
    void begin(uint32_t baud);
    void begin(uint32_t baud, uint8_t config);
    void end();
    int available(void);
    int read(void);
    int write(uint8_t);
    void write(const char *str)                    { while (*str) write(*str++); }
    void write(const uint8_t *buffer, size_t size) { while (size--) write(*buffer++); }
    void print(char *str)                          { write(str); }
    void print(const char *str)                    { write(str); }

  // Prints are performed with a single size, to avoid needing multiple print functions.
  // The fixed integer size used for prints will be the larger of long or a pointer.
  #if __LONG_WIDTH__ >= __INTPTR_WIDTH__
    typedef long int_fixed_print_t;
    typedef unsigned long uint_fixed_print_t;
  #else
    typedef intptr_t int_fixed_print_t;
    typedef uintptr_t uint_fixed_print_t;

    FORCE_INLINE void print(intptr_t c, PrintBase base)         { printNumber_signed(c, base); }
    FORCE_INLINE void print(uintptr_t c, PrintBase base)        { printNumber_unsigned(c, base); }
  #endif


  FORCE_INLINE void print(char c, PrintBase base)               { printNumber_signed(c, base); }
  FORCE_INLINE void print(short c, PrintBase base)              { printNumber_signed(c, base); }
  FORCE_INLINE void print(int c, PrintBase base)                { printNumber_signed(c, base); }
  FORCE_INLINE void print(long c, PrintBase base)               { printNumber_signed(c, base); }
  FORCE_INLINE void print(unsigned char c, PrintBase base)      { printNumber_unsigned(c, base); }
  FORCE_INLINE void print(unsigned short c, PrintBase base)     { printNumber_unsigned(c, base); }
  FORCE_INLINE void print(unsigned int c, PrintBase base)       { printNumber_unsigned(c, base); }
  FORCE_INLINE void print(unsigned long c, PrintBase base)      { printNumber_unsigned(c, base); }


  void print(EnsureDouble c, int digits)           { printFloat(c, digits); }

  // Forward the call to the former's method

  // Default implementation for anything without a specialization
  // This handles integers since they are the most common
  template <typename T>
  void print(T c)    { print(c, PrintBase::Dec); }

  void print(float c)    { print(c, 2); }
  void print(double c)    { print(c, 2); }

  void println(char *s)               { print(s); println(); }
  void println(const char *s)         { print(s); println(); }
  void println(float c, int digits)   { print(c, digits); println(); }
  void println(double c, int digits)  { print(c, digits); println(); }
  void println()                      { write('\r'); write('\n'); }

  // Default implementations for types without a specialization. Handles integers.
  template <typename T>
  void println(T c, PrintBase base)   { print(c, base); println(); }

  template <typename T>
  void println(T c)                   { println(c, PrintBase::Dec); }

  // Forward the call to the former's method
  void println(float c)               { println(c, 2); }
  void println(double c)              { println(c, 2); }

  // Print a number with the given base
  NO_INLINE void printNumber_unsigned(uint_fixed_print_t n, PrintBase base) {
    if (n) {
      unsigned char buf[8 * sizeof(long)]; // Enough space for base 2
      int8_t i = 0;
      while (n) {
        buf[i++] = n % (uint_fixed_print_t)base;
        n /= (uint_fixed_print_t)base;
      }
      while (i--) write((char)(buf[i] + (buf[i] < 10 ? '0' : 'A' - 10)));
    }
    else write('0');
  }

  NO_INLINE void printNumber_signed(int_fixed_print_t n, PrintBase base) {
    if (base == PrintBase::Dec && n < 0) {
      n = -n; // This works because all platforms Marlin's builds on are using 2-complement encoding for negative number
              // On such CPU, changing the sign of a number is done by inverting the bits and adding one, so if n = 0x80000000 = -2147483648 then
              // -n = 0x7FFFFFFF + 1 => 0x80000000 = 2147483648 (if interpreted as unsigned) or -2147483648 if interpreted as signed.
              // On non 2-complement CPU, there would be no possible representation for 2147483648.
      write('-');
    }
    printNumber_unsigned((uint_fixed_print_t)n , base);
  }

  // Print a decimal number
  NO_INLINE void printFloat(double number, uint8_t digits) {
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
    printNumber_unsigned(int_part, PrintBase::Dec);

    // Print the decimal point, but only if there are digits beyond
    if (digits) {
      write('.');
      // Extract digits from the remainder one at a time
      while (digits--) {
        remainder *= 10.0;
        unsigned long toPrint = (unsigned long)remainder;
        printNumber_unsigned(toPrint, PrintBase::Dec);
        remainder -= toPrint;
      }
    }
  }





    // Interrupt handlers
    static int _tx_complete_irq(serial_t *obj);
    static void _rx_complete_irq(serial_t *obj);

    void flush(void);
    bool isHalfDuplex(void) const;
    void enableHalfDuplexRx(void);

  private:
    void setRx(uint32_t _rx);
    void setTx(uint32_t _tx);
    void setRx(PinName _rx);
    void setTx(PinName _tx);
    void init(PinName _rx, PinName _tx);
    bool _rx_enabled;
    uint8_t _config;
    unsigned long _baud;
  

  
};

#endif