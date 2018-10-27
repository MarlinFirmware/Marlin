/**
 * Print.cpp - Base class that provides print() and println()
 * Copyright (c) 2008 David A. Mellis.  All right reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Modified 23 November 2006 by David A. Mellis
 * Modified 03 August 2015 by Chuck Todd
 * Modified 03 February 2018 by Scott Lahteine
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include "Print.h"
#include <stdarg.h>

#define ENABLE_PRINTF

// Public Methods //////////////////////////////////////////////////////////////

/* default implementation: may be overridden */
size_t Print::write(const uint8_t *buffer, size_t size) {
  size_t n = 0;
  while (size--) {
    if (write(*buffer++)) n++;
    else break;
  }
  return n;
}

size_t Print::print(const char str[])           { return write(str); }
size_t Print::print(char c)                     { return write(c); }
size_t Print::print(unsigned char b, int base)  { return print((unsigned long) b, base); }
size_t Print::print(int n, int base)            { return print((long) n, base); }
size_t Print::print(unsigned int n, int base)   { return print((unsigned long) n, base); }
size_t Print::print(double n, int digits)       { return printFloat(n, digits); }
size_t Print::print(const Printable& x)         { return x.printTo(*this); }
size_t Print::println(void)                     { return write("\r\n"); }

size_t Print::print(long n, int base) {
  if (base == 0) return write(n);
  if (base == 10) {
    if (n < 0) {
      const int t = print('-');
      return printNumber(-n, 10) + t;
    }
  }
  return printNumber(n, base);
}

size_t Print::print(unsigned long n, int base) {
  if (base == 0) return write(n);
  return printNumber(n, base);
}

#define PRINTLN(...) do{ \
  size_t n = print(__VA_ARGS__); \
  n += println(); \
  return n; \
}while(0)

size_t Print::println(const char c[])              { PRINTLN(c); }
size_t Print::println(char c)                      { PRINTLN(c); }
size_t Print::println(unsigned char b, int base)   { PRINTLN(b, base); }
size_t Print::println(int num, int base)           { PRINTLN(num, base); }
size_t Print::println(unsigned int num, int base)  { PRINTLN(num, base); }
size_t Print::println(long num, int base)          { PRINTLN(num, base); }
size_t Print::println(unsigned long num, int base) { PRINTLN(num, base); }
size_t Print::println(double num, int digits)      { PRINTLN(num, digits); }
size_t Print::println(const Printable& x)          { PRINTLN(x); }

// Private Methods /////////////////////////////////////////////////////////////

size_t Print::printNumber(unsigned long n, uint8_t base) {
  char buf[8 * sizeof(long) + 1]; // Assumes 8-bit chars plus zero byte.
  char *str = &buf[sizeof(buf) - 1];

  *str = '\0';

  // prevent crash if called with base == 1
  if (base < 2) base = 10;

  do {
    unsigned long m = n;
    n /= base;
    char c = m - base * n;
    *--str = c < 10 ? c + '0' : c + 'A' - 10;
  } while(n);

  return write(str);
}

size_t Print::printFloat(double number, uint8_t digits) {
  size_t n = 0;

  if (isnan(number)) return print("nan");
  if (isinf(number)) return print("inf");
  if (number > 4294967040.0) return print ("ovf");  // constant determined empirically
  if (number <-4294967040.0) return print ("ovf");  // constant determined empirically

  // Handle negative numbers
  if (number < 0.0) {
    n += print('-');
    number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i = 0; i < digits; ++i) rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  uint32_t int_part = (uint32_t)number;
  double remainder = number - (double)int_part;
  n += print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) n += print('.');

  // Extract digits from the remainder one at a time
  while (digits-- > 0) {
    remainder *= 10.0;
    const int toPrint = int(remainder);
    n += print(toPrint);
    remainder -= toPrint;
  }

  return n;
}

#ifdef ENABLE_PRINTF

  size_t Print::printf(const char *argList, ...) {
    const char *ptr;
    double floatNum_f32;
    va_list argp;
    int16_t num_s16;
    int32_t num_s32;
    uint16_t num_u16;
    uint32_t num_u32;
    char *str;
    char  ch;
    uint8_t numOfDigits;

    va_start(argp, argList);

    // Loop through the list to extract all the input arguments
    for (ptr = argList; (ch = *ptr); ptr++) {
      if (ch == '%') {       //Check for '%' as there will be format specifier after it
        ptr++;
        ch = *ptr;
        if (ch >= '0' && ch <= '9') {
          numOfDigits = 0;
          while (ch >= '0' && ch <= '9') {
            numOfDigits = numOfDigits * 10 + ch - '0';
            ptr++;
            ch = *ptr;
          }
        }
        else
         numOfDigits = 0xFF;

        switch(ch) {     // Decode the type of the argument

        case 'C':
        case 'c':     // Argument type is of char, hence read char data from the argp
            ch = va_arg(argp, int);
            print(ch);
            break;

        case 'd':    // Argument type is of signed integer, hence read 16bit data from the argp
        case 'D':
            num_s32 = va_arg(argp, int);
            print(num_s32, 10);
            break;

        case 'u':
        case 'U':    // Argument type is of integer, hence read 32bit unsigend data
            num_u32 = va_arg(argp, uint32_t);
            print(num_u32, 10);
            break;

        case 'x':
        case 'X':  // Argument type is of hex, hence hexadecimal data from the argp
            num_u32 = va_arg(argp, uint32_t);
            print(num_u32, 16);
            break;

        case 'b':
        case 'B':  // Argument type is of binary,Read int and convert to binary
            num_u32 = va_arg(argp, uint32_t);
            print(num_u32, 2);
            break;

        case 'F':
        case 'f': // Argument type is of float, hence read double data from the argp
            floatNum_f32 = va_arg(argp, double);
            printFloat(floatNum_f32, 10);
            break;

        case 'S':
        case 's': // Argument type is of string, hence get the pointer to sting passed
            str = va_arg(argp, char *);
            print(str);
            break;

        case '%':
          print('%');
          break;
        }
      }
      else
        print(ch); // As '%' is not detected transmit the char passed
    }

    va_end(argp);
  }

#endif // ENABLE_PRINTF
