/*
  Print.h - Base class that provides print() and println()
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
*/

#ifndef Print_h
#define Print_h

#include <inttypes.h>
#include <stdio.h> // for size_t
#include "core_id.h"
#include "WString.h"
#include "Printable.h"

#define DEC 10
#define HEX 16
#define OCT 8
#define BIN 2
#define BYTE 0

class __FlashStringHelper;

#if ARDUINO >= 100
class Print
{
  public:
	Print() : write_error(0) {}
	virtual size_t write(uint8_t b);
	size_t write(const char *str)			{ return write((const uint8_t *)str, strlen(str)); }
	virtual size_t write(const uint8_t *buffer, size_t size);
	size_t print(const String &s);
	size_t print(char c)				{ return write((uint8_t)c); }
	size_t print(const char s[])			{ return write(s); }
	size_t print(const __FlashStringHelper *f);

	size_t print(uint8_t b)				{ return printNumber(b, 10, 0); }
	size_t print(int n)				{ return print((long)n); }
	size_t print(unsigned int n)			{ return printNumber(n, 10, 0); }
	size_t print(long n);
	size_t print(unsigned long n)			{ return printNumber(n, 10, 0); }

	size_t print(unsigned char n, int base)		{ return printNumber(n, base, 0); }
	size_t print(int n, int base)			{ return (base == 10) ? print(n) : printNumber(n, base, 0); }
	size_t print(unsigned int n, int base)		{ return printNumber(n, base, 0); }
	size_t print(long n, int base)			{ return (base == 10) ? print(n) : printNumber(n, base, 0); }
	size_t print(unsigned long n, int base)		{ return printNumber(n, base, 0); }

	size_t print(double n, int digits = 2)		{ return printFloat(n, digits); }
	size_t print(const Printable &obj)		{ return obj.printTo(*this); }
	size_t println(void);
	size_t println(const String &s)			{ return print(s) + println(); }
	size_t println(char c)				{ return print(c) + println(); }
	size_t println(const char s[])			{ return print(s) + println(); }
	size_t println(const __FlashStringHelper *f)	{ return print(f) + println(); }

	size_t println(uint8_t b)			{ return print(b) + println(); }
	size_t println(int n)				{ return print(n) + println(); }
	size_t println(unsigned int n)			{ return print(n) + println(); }
	size_t println(long n)				{ return print(n) + println(); }
	size_t println(unsigned long n)			{ return print(n) + println(); }

	size_t println(unsigned char n, int base)	{ return print(n, base) + println(); }
	size_t println(int n, int base)			{ return print(n, base) + println(); }
	size_t println(unsigned int n, int base)	{ return print(n, base) + println(); }
	size_t println(long n, int base)		{ return print(n, base) + println(); }
	size_t println(unsigned long n, int base)	{ return print(n, base) + println(); }

	size_t println(double n, int digits = 2)	{ return print(n, digits) + println(); }
	size_t println(const Printable &obj)		{ return obj.printTo(*this) + println(); }
	int getWriteError() { return write_error; }
	void clearWriteError() { setWriteError(0); }
  protected:
	void setWriteError(int err = 1) { write_error = err; }
  private:
	char write_error;
	size_t printNumber(unsigned long n, uint8_t base, uint8_t sign);
	size_t printFloat(double n, uint8_t digits);
};

#else
class Print
{
  public:
	virtual void write(uint8_t);
	virtual void write(const char *str);
	virtual void write(const uint8_t *buffer, size_t size);
	void print(const String &s);
	void print(char c)				{ write((uint8_t)c); }
	void print(const char s[])			{ write(s); }
	void print(const __FlashStringHelper *f);

	void print(uint8_t b)				{ write(b); }
	void print(int n)				{ print((long)n); }
	void print(unsigned int n)			{ printNumber(n, 10, 0); }
	void print(long n);
	void print(unsigned long n)			{ printNumber(n, 10, 0); }

	void print(unsigned char n, int base)		{ printNumber(n, base, 0); }
	void print(int n, int base)			{ (base == 10) ? print(n) : printNumber(n, base, 0); }
	void print(unsigned int n, int base)		{ printNumber(n, base, 0); }
	void print(long n, int base)			{ (base == 10) ? print(n) : printNumber(n, base, 0); }
	void print(unsigned long n, int base)		{ printNumber(n, base, 0); }

	void print(double n, int digits = 2)		{ printFloat(n, digits); }
	void println(void);
	void println(const String &s)			{ print(s); println(); }
	void println(char c)				{ print(c); println(); }
	void println(const char s[])			{ print(s); println(); }
	void println(const __FlashStringHelper *f)	{ print(f); println(); }
	void println(uint8_t b)				{ print(b); println(); }
	void println(int n)				{ print(n); println(); }
	void println(unsigned int n)			{ print(n); println(); }
	void println(long n)				{ print(n); println(); }
	void println(unsigned long n)			{ print(n); println(); }

	void println(unsigned char n, int base)		{ print(n, base); println(); }
	void println(int n, int base)			{ print(n, base); println(); }
	void println(unsigned int n, int base)		{ print(n, base); println(); }
	void println(long n, int base)			{ print(n, base); println(); }
	void println(unsigned long n, int base)		{ print(n, base); println(); }

	void println(double n, int digits = 2)		{ print(n, digits); println(); }
  private:
	void printNumber(unsigned long n, uint8_t base, uint8_t sign);
	void printFloat(double n, uint8_t digits);
};
#endif


#endif
