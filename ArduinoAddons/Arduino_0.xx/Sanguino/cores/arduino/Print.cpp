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
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "wiring.h"

#include "Print.h"

// Public Methods //////////////////////////////////////////////////////////////

/* default implementation: may be overridden */
void Print::write(const char *str)
{
  while (*str)
    write(*str++);
}

/* default implementation: may be overridden */
void Print::write(const uint8_t *buffer, size_t size)
{
  while (size--)
    write(*buffer++);
}

void Print::print(const String &s)
{
  for (int i = 0; i < s.length(); i++) {
    write(s[i]);
  }
}

void Print::print(const char str[])
{
  write(str);
}

void Print::print(char c, int base)
{
  print((long) c, base);
}

void Print::print(unsigned char b, int base)
{
  print((unsigned long) b, base);
}

void Print::print(int n, int base)
{
  print((long) n, base);
}

void Print::print(unsigned int n, int base)
{
  print((unsigned long) n, base);
}

void Print::print(long n, int base)
{
  if (base == 0) {
    write(n);
  } else if (base == 10) {
    if (n < 0) {
      print('-');
      n = -n;
    }
    printNumber(n, 10);
  } else {
    printNumber(n, base);
  }
}

void Print::print(unsigned long n, int base)
{
  if (base == 0) write(n);
  else printNumber(n, base);
}

void Print::print(double n, int digits)
{
  printFloat(n, digits);
}

void Print::println(void)
{
  print('\r');
  print('\n');  
}

void Print::println(const String &s)
{
  print(s);
  println();
}

void Print::println(const char c[])
{
  print(c);
  println();
}

void Print::println(char c, int base)
{
  print(c, base);
  println();
}

void Print::println(unsigned char b, int base)
{
  print(b, base);
  println();
}

void Print::println(int n, int base)
{
  print(n, base);
  println();
}

void Print::println(unsigned int n, int base)
{
  print(n, base);
  println();
}

void Print::println(long n, int base)
{
  print(n, base);
  println();
}

void Print::println(unsigned long n, int base)
{
  print(n, base);
  println();
}

void Print::println(double n, int digits)
{
  print(n, digits);
  println();
}

// Private Methods /////////////////////////////////////////////////////////////

void Print::printNumber(unsigned long n, uint8_t base)
{
  unsigned char buf[8 * sizeof(long)]; // Assumes 8-bit chars. 
  unsigned long i = 0;

  if (n == 0) {
    print('0');
    return;
  } 

  while (n > 0) {
    buf[i++] = n % base;
    n /= base;
  }

  for (; i > 0; i--)
    print((char) (buf[i - 1] < 10 ?
      '0' + buf[i - 1] :
      'A' + buf[i - 1] - 10));
}

void Print::printFloat(double number, uint8_t digits) 
{ 
  // Handle negative numbers
  if (number < 0.0)
  {
     print('-');
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
  print(int_part);

  // Print the decimal point, but only if there are digits beyond
  if (digits > 0)
    print("."); 

  // Extract digits from the remainder one at a time
  while (digits-- > 0)
  {
    remainder *= 10.0;
    int toPrint = int(remainder);
    print(toPrint);
    remainder -= toPrint; 
  } 
}
