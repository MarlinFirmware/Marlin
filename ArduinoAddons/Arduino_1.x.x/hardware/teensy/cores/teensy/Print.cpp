/*
 Print.cpp - Base class that provides print() and println()
 Copyright (c) 2008 David A. Mellis.  All right reserved.
 many modifications, by Paul Stoffregen <paul@pjrc.com>
 
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

#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include <math.h>
#include <avr/pgmspace.h>
#include "wiring.h"

#include "Print.h"


#if ARDUINO >= 100
#else
void Print::write(const char *str)
{
	write((const uint8_t *)str, strlen(str));
}
#endif


#if ARDUINO >= 100
size_t Print::write(const uint8_t *buffer, size_t size)
{
	size_t count = 0;
	while (size--) count += write(*buffer++);
	return count;
}
#else
void Print::write(const uint8_t *buffer, size_t size)
{
	while (size--) write(*buffer++);
}
#endif


#if ARDUINO >= 100
size_t Print::print(const String &s)
{
	uint8_t buffer[33];
	size_t count = 0;
	unsigned int index = 0;
	unsigned int len = s.length();
	while (len > 0) {
		s.getBytes(buffer, sizeof(buffer), index);
		unsigned int nbytes = len;
		if (nbytes > sizeof(buffer)-1) nbytes = sizeof(buffer)-1;
		index += nbytes;
		len -= nbytes;
		count += write(buffer, nbytes);
	}
	return count;
}
#else
void Print::print(const String &s)
{
	unsigned int len = s.length();
	for (unsigned int i=0; i < len; i++) {
		write(s[i]);
	}
}
#endif


#if ARDUINO >= 100
size_t Print::print(const __FlashStringHelper *ifsh)
{
	uint8_t buffer[32];
	size_t count = 0;
	const char PROGMEM *p = (const char PROGMEM *)ifsh;
	unsigned int len = strlen_P(p);
	while (len > 0) {
		unsigned int nbytes = len;
		if (nbytes > sizeof(buffer)) nbytes = sizeof(buffer);
		memcpy_P(buffer, p, nbytes);
		p += nbytes;
		len -= nbytes;
		count += write(buffer, nbytes);
	}
	return count;
}
#else
void Print::print(const __FlashStringHelper *ifsh)
{
	const char PROGMEM *p = (const char PROGMEM *)ifsh;
	while (1) {
		unsigned char c = pgm_read_byte(p++);
		if (c == 0) return;
		write(c);
	}
}
#endif


#if ARDUINO >= 100
size_t Print::print(long n)
{
	uint8_t sign=0;

	if (n < 0) {
		sign = '-';
		n = -n;
	}
	return printNumber(n, 10, sign);
}
#else
void Print::print(long n)
{
	uint8_t sign=0;

	if (n < 0) {
		sign = '-';
		n = -n;
	}
	printNumber(n, 10, sign);
}
#endif


#if ARDUINO >= 100
size_t Print::println(void)
{
	uint8_t buf[2]={'\r', '\n'};
	return write(buf, 2);
}
#else
void Print::println(void)
{
	uint8_t buf[2]={'\r', '\n'};
	write(buf, 2);
}
#endif


#if ARDUINO >= 100
size_t Print::printNumber(unsigned long n, uint8_t base, uint8_t sign)
#else
void Print::printNumber(unsigned long n, uint8_t base, uint8_t sign)
#endif
{
	uint8_t buf[8 * sizeof(long) + 1];
	uint8_t digit, i;

	// TODO: make these checks as inline, since base is
	// almost always a constant.  base = 0 (BYTE) should
	// inline as a call directly to write()
	if (base == 0) {
#if ARDUINO >= 100
		return write((uint8_t)n);
#else
		write((uint8_t)n);
		return;
#endif
	} else if (base == 1) {
		base = 10;
	}

	if (n == 0) {
		buf[sizeof(buf) - 1] = '0';
		i = sizeof(buf) - 1;
	} else {
		i = sizeof(buf) - 1;
		while (1) {
			digit = n % base;
			buf[i] = ((digit < 10) ? '0' + digit : 'A' + digit - 10);
			n /= base;
			if (n == 0) break;
			i--;
		}
	}
	if (sign) {
		i--;
		buf[i] = '-';
	}
#if ARDUINO >= 100
	return write(buf + i, sizeof(buf) - i);
#else
	write(buf + i, sizeof(buf) - i);
#endif
}


#if ARDUINO >= 100
size_t Print::printFloat(double number, uint8_t digits) 
#else
void Print::printFloat(double number, uint8_t digits) 
#endif
{
	uint8_t sign=0;
#if ARDUINO >= 100
	size_t count=0;
#endif

	// Handle negative numbers
	if (number < 0.0) {
		sign = 1;
		number = -number;
	}

	// Round correctly so that print(1.999, 2) prints as "2.00"
	double rounding = 0.5;
	for (uint8_t i=0; i<digits; ++i) {
		rounding *= 0.1;
	}
	number += rounding;

	// Extract the integer part of the number and print it
	unsigned long int_part = (unsigned long)number;
	double remainder = number - (double)int_part;
#if ARDUINO >= 100
	count += printNumber(int_part, 10, sign);
#else
	printNumber(int_part, 10, sign);
#endif

	// Print the decimal point, but only if there are digits beyond
	if (digits > 0) {
		uint8_t n, buf[8], count=1;
		buf[0] = '.';

		// Extract digits from the remainder one at a time
		if (digits > sizeof(buf) - 1) digits = sizeof(buf) - 1;

		while (digits-- > 0) {
			remainder *= 10.0;
			n = (uint8_t)(remainder);
			buf[count++] = '0' + n;
			remainder -= n; 
		}
#if ARDUINO >= 100
		count += write(buf, count);
#else
		write(buf, count);
#endif
	}
#if ARDUINO >= 100
	return count;
#endif
}


