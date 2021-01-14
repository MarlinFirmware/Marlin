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

#include "../inc/MarlinConfig.h"
#include <WString.h>


// It's useless to have multiple version of this code in the binary, so let's make a common interface here
struct NumberPrinter
{
  // The only required underlying method is here
  virtual void write(uint8_t c) = 0;
  // Print a number with the given base
  void printNumber(unsigned long n, const uint8_t base)
  {
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
  void printFloat(double number, uint8_t digits)
  {
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


// flushTX is not implemented everywhere, so we need to fallback to SFINAE to detect it and implement it if it's the case
template <typename T>
class HasFlushTX
{
private:
  typedef char YesType[1];
  typedef char NoType[2];

  template <typename C> static YesType& test( decltype(&C::flushTX) ) ;
  template <typename C> static NoType& test(...);


public:
  enum { value = sizeof(test<T>(0)) == sizeof(YesType) };
};

template<typename T> 
FORCE_INLINE typename std::enable_if<HasFlushTX<T>::value, void>::type
CallFlushTX(T * t) { t->flushTX(); }

FORCE_INLINE static void CallFlushTX(...) {}


// Using Curiously recurring template pattern here to avoid a virtual table dispatch since all information is known at compile time
// When calling any of the method below, the compiler will immediately call the inlined child's method instead, resulting in zero cost 
// (no code, no performance penalty) for dispatching
template <class Child>
struct SerialHook
{
  void write(uint8_t c)         { static_cast<Child*>(this)->write(c); }
  void msgDone()                { static_cast<Child*>(this)->msgDone(); } 
  bool available(uint8_t index) { return static_cast<Child*>(this)->available(index); }
  int  read(uint8_t index)      { return static_cast<Child*>(this)->read(index); }

  // All other method are here for convenience for a common interface. They cost nothing
  FORCE_INLINE void print(const String & s) { static_cast<Child*>(this)->print(s); } 
  FORCE_INLINE void print(const char * s)   { static_cast<Child*>(this)->print(s); } 
  // Redirect all print method
  template <typename T>
  FORCE_INLINE void print(T c, int base) { static_cast<Child*>(this)->print((T)c, base); }
  // Redirect all println method
  template <typename T>
  FORCE_INLINE void println(T c, int base) { static_cast<Child*>(this)->println((T)c, base); }

  // Redirect printf
  template <typename... Args>
  FORCE_INLINE void printf(const char* format, Args ... args)  { static_cast<Child*>(this)->printf(format, args...); }
  
  // Redirect flush
  void flush()      { static_cast<Child*>(this)->flush(); }
  void flushTX()    { static_cast<Child*>(this)->flushTX(); }
};

// The most basic serial class: it dispatch to the base serial class with no hook whatsoever. This will compile to nothing but the base serial class 
template <class SerialT>
struct BaseSerial : public SerialHook< BaseSerial<SerialT> >, public SerialT
{
  // It's required to implement a write method here to help compiler disambiguate what method to call 
  using SerialT::write;
  using SerialT::print;
  using SerialT::println;
  using SerialT::flush;

  bool available(uint8_t index) { return index == 0 && SerialT::available(); }
  int read(uint8_t index)       { return index == 0 ? SerialT::read() : -1; }

  void flushTX() { CallFlushTX((SerialT*)this); }
};

// A class that's can be hooked and unhooked at runtime, useful to capturing the output of the serial interface
template <class SerialT>
struct RuntimeSerialHook : public SerialHook< RuntimeSerialHook<SerialT> >, public SerialT
{
  typedef void (*WriteHook)(void * userPointer, uint8_t c);
  typedef void (*EndOfMessageHook)(void * userPointer);

  WriteHook          writeHook;
  EndOfMessageHook   eofHook;
  void *             userPointer;

  void write(uint8_t c) { 
    if (writeHook) writeHook(userPointer, c);
    SerialT::write(c);
  }

  void msgDone() {
    if (eofHook) eofHook(userPointer);
    SerialT::msgDone();
  }

  bool available(uint8_t index) { return index == 0 && SerialT::available(); }
  int read(uint8_t index)       { return index == 0 ? SerialT::read() : -1; }


  void setHook(WriteHook writeHook = 0, EndOfMessageHook eofHook = 0, void * userPointer = 0) {
    // Order is important here as serial code can be called inside interrupts
    // When setting a hook, the user pointer must be set first so if writeHook is called as soon as it's set, it'll be valid
    if (userPointer) this->userPointer = userPointer;
    this->writeHook = writeHook;
    this->eofHook = eofHook;
    // Order is important here because of asynchronous access here
    // When unsetting a hook, the user pointer must be unset last so that any pending writeHook is still using the old pointer
    if (!userPointer) this->userPointer = 0;
  }

  RuntimeSerialHook() : writeHook(0), eofHook(0), userPointer(0) {}
};

// A class that's duplicating its output conditionally to 2 serial interface
template <class Serial0T, class Serial1T>
struct MultiSerialHook : public SerialHook< MultiSerialHook<Serial0T, Serial1T> >, public NumberPrinter
{
  int8_t     portMask;
  Serial0T & serial0;
  Serial1T & serial1;

  void write(uint8_t c) { 
    const bool open = TERN1(HAS_ETHERNET, ethernet.have_telnet_client);
    if (portMask & 1) serial0.write(c);
    if ((portMask & 2) && open) serial1.write(c);
  }
  void msgDone() { 
    if (portMask & 1) serial0.msgDone();
    if (portMask & 2) serial1.msgDone();
  }
  bool available(uint8_t index) { 
    switch(index) {
      case 0: return serial0.available();
      case 1: return serial1.available();
      default: return false;
    }
  }
  int read(uint8_t index) { 
    switch(index) {
      case 0: return serial0.read();
      case 1: return serial1.read();
      default: return -1;
    }
  }

  // Sadly, we can't derivate from any base class here since it's possible such class does not have virtual methods calling back our own implementation
  FORCE_INLINE void write(const char* str)                      { while (*str) write(*str++); }
  FORCE_INLINE void write(const uint8_t* buffer, size_t size)   { while (size--) write(*buffer++); }
  FORCE_INLINE void print(const String& s)                      { for (int i = 0; i < (int)s.length(); i++) write(s[i]); }
  FORCE_INLINE void print(const char* str)                      { write(str); }
  void print(char c, int base = 0)                              { print((long)c, base); }
  void print(unsigned char c, int base = 0)                     { print((unsigned long)c, base); }
  void print(int c, int base = DEC)                             { print((long)c, base); }
  void print(unsigned int c, int base = DEC)                    { print((unsigned long)c, base); }
  void print(long c, int base = DEC)                            { if (!base) write(c); write((const uint8_t*)"-", c < 0); printNumber(c < 0 ? -c : c, base); }
  void print(unsigned long c, int base = DEC)                   { printNumber(c, base); }
  void print(double c, int digits = 2)                          { printFloat(c, digits); }

  void println(const String& s)                                 { print(s); println(); }
  void println(const char s[])                                  { print(s); println(); }
  void println(char c, int base = 0)                            { print(c, base); println(); }
  void println(unsigned char c, int base = 0)                   { print(c, base); println(); }
  void println(int c, int base = DEC)                           { print(c, base); println(); }
  void println(unsigned int c, int base = DEC)                  { print(c, base); println(); }
  void println(long c, int base = DEC)                          { print(c, base); println(); }
  void println(unsigned long c, int base = DEC)                 { print(c, base); println(); }
  void println(double c, int digits = 2)                        { print(c, digits); println(); }
  void println()                                                { write("\r\n"); }
  
  // Redirect flush
  void flush()      { 
    if (portMask & 1) serial0.flush(); 
    if (portMask & 2) serial1.flush(); 
  }
  void flushTX()    { 
    if (portMask & 1) CallFlushTX(&serial0); 
    if (portMask & 2) CallFlushTX(&serial1); 
  }

  MultiSerialHook(Serial0T & serial0, Serial1T & serial1, int8_t mask) : portMask(mask), serial0(serial0), serial1(serial1) {}
};
