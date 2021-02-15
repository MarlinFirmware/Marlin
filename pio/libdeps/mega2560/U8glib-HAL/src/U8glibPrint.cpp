/*
  Print.cpp - Base class that provides print() and println()
  Copyright (c) 2008 David A. Mellis.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 23 November 2006 by David A. Mellis
  Modified 03 August 2015 by Chuck Todd
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <inttypes.h>
#include "U8glibPrint.h"
#include <stdarg.h>

#define PrintfEnable  0
// Public Methods //////////////////////////////////////////////////////////////

/* default implementation: may be overridden */
size_t U8glibPrint::write(const uint8_t *buffer, size_t size)
{

  size_t n = 0;
  while (size--) {
    if (write(*buffer++)) n++;
    else break;
  }
  return n;
}


size_t U8glibPrint::print(const char str[])
{
  //while(1);
  return write(str);
}

size_t U8glibPrint::print(char c)
{
  return write(c);
}

size_t U8glibPrint::print(unsigned char b, int base)
{
  return print((unsigned long) b, base);
}

size_t U8glibPrint::print(int n, int base)
{
  return print((long) n, base);
}

size_t U8glibPrint::print(unsigned int n, int base)
{
  return print((unsigned long) n, base);
}

size_t U8glibPrint::print(long n, int base)
{
  if (base == 0) {
    return write(n);
  } else if (base == 10) {
    if (n < 0) {
      int t = print('-');
      n = -n;
      return printNumber(n, 10) + t;
    }
    return printNumber(n, 10);
  } else {
    return printNumber(n, base);
  }
}

size_t U8glibPrint::print(unsigned long n, int base)
{
  if (base == 0) return write(n);
  else return printNumber(n, base);
}

size_t U8glibPrint::print(double n, int digits)
{
  return printFloat(n, digits);
}

size_t U8glibPrint::print(const U8glibPrintable& x)
{
  return x.printTo(*this);
}

size_t U8glibPrint::println(void)
{
  return write("\r\n");
}

size_t U8glibPrint::println(const char c[])
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t U8glibPrint::println(char c)
{
  size_t n = print(c);
  n += println();
  return n;
}

size_t U8glibPrint::println(unsigned char b, int base)
{
  size_t n = print(b, base);
  n += println();
  return n;
}

size_t U8glibPrint::println(int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t U8glibPrint::println(unsigned int num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t U8glibPrint::println(long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t U8glibPrint::println(unsigned long num, int base)
{
  size_t n = print(num, base);
  n += println();
  return n;
}

size_t U8glibPrint::println(double num, int digits)
{
  size_t n = print(num, digits);
  n += println();
  return n;
}

size_t U8glibPrint::println(const U8glibPrintable& x)
{
  size_t n = print(x);
  n += println();
  return n;
}

// Private Methods /////////////////////////////////////////////////////////////

size_t U8glibPrint::printNumber(unsigned long n, uint8_t base) {
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

size_t U8glibPrint::printFloat(double number, uint8_t digits)
{
  size_t n = 0;

  if (isnan(number)) return print("nan");
  if (isinf(number)) return print("inf");
  if (number > 4294967040.0) return print ("ovf");  // constant determined empirically
  if (number <-4294967040.0) return print ("ovf");  // constant determined empirically

  // Handle negative numbers
  if (number < 0.0)
  {
     n += print('-');
     number = -number;
  }

  // Round correctly so that print(1.999, 2) prints as "2.00"
  double rounding = 0.5;
  for (uint8_t i=0; i<digits; ++i)
    rounding /= 10.0;

  number += rounding;

  // Extract the integer part of the number and print it
  unsigned long int_part = (unsigned long)number;
  double remainder = number - (double)int_part;
  n += print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0) {
    n += print(".");
  }

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    n += print(toPrint);
    remainder -= toPrint;
  }

  return n;
}


#if (PrintfEnable == 1)
size_t U8glibPrint::printf(const char *argList, ...)
{
    const char *ptr;
    double floatNum_f32;
    va_list argp;
    sint16_t num_s16;
    sint32_t num_s32;
    uint16_t num_u16;
    uint32_t num_u32;
    char *str;
    char  ch;
    uint8_t numOfDigits;

    va_start(argp, argList);

    /* Loop through the list to extract all the input arguments */
    for(ptr = argList; *ptr != '\0'; ptr++)
    {

        ch= *ptr;
        if(ch == '%')         /*Check for '%' as there will be format specifier after it */
        {
            ptr++;
            ch = *ptr;
            if((ch>=0x30) && (ch<=0x39))
            {
                numOfDigits = 0;
                while((ch>=0x30) && (ch<=0x39))
                {
                    numOfDigits = (numOfDigits * 10) + (ch-0x30);
                    ptr++;
                    ch = *ptr;
                }
            }
            else
            {
                numOfDigits = 0xff;
            }


            switch(ch)       /* Decode the type of the argument */
            {

            case 'C':
            case 'c':     /* Argument type is of char, hence read char data from the argp */
                ch = va_arg(argp, int);
                print(ch);
                break;



            case 'd':    /* Argument type is of signed integer, hence read 16bit data from the argp */
            case 'D':
                num_s32 = va_arg(argp, int);
                print(num_s32, 10);
                break;


            case 'u':
            case 'U':    /* Argument type is of integer, hence read 32bit unsigend data */
                num_u32 = va_arg(argp, uint32_t);
                print(num_u32, 10);
                break;




            case 'x':
            case 'X':  /* Argument type is of hex, hence hexadecimal data from the argp */
                num_u32 = va_arg(argp, uint32_t);
                print(num_u32, 16);
                break;


            case 'b':
            case 'B':  /* Argument type is of binary,Read int and convert to binary */
                num_u32 = va_arg(argp, uint32_t);
                print(num_u32, 2);
                break;



            case 'F':
            case 'f': /* Argument type is of float, hence read double data from the argp */
                floatNum_f32 = va_arg(argp, double);
                printFloat(floatNum_f32,10);
                break;



            case 'S':
            case 's': /* Argument type is of string, hence get the pointer to sting passed */
                str = va_arg(argp, char *);
                print(str);
                break;



            case '%':
                print('%');
                break;
            }
        }
        else
        {
            /* As '%' is not detected transmit the char passed */
            print(ch);
        }
    }

    va_end(argp);
}

#endif
