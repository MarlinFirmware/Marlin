/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Copyright (c) 2021 X-Ryl669
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

#include "number_format.h"
#include "bug_on.h"

/** Basic R/W string class that's not managing the memory it's using. Use one of the child class to provide the memory management you need */
class StringBase : public NumberFormatter< StringBase >
{
  typedef struct NumberFormatter< StringBase > BaseClass;
protected:
  char *    buffer;
  uint16_t  len;
  uint16_t  allocSize;

  // Helpers
private:
  // Depending on the implementation, will resize or dump an error if it does not fit
  virtual bool checkItFits(const uint16_t newSize) = 0;

  // Append a string here
  StringBase & append(const char * txt, size_t txtLen = 0) {
    if (!txtLen) txtLen = strlen(txt);
    if (checkItFits(len + txtLen)) {
      memcpy(&buffer[len], txt, txtLen+1); // Always zero terminate the string
      len += txtLen;
    }
    return *this;
  }
  // Not the most efficient here, but very simple
  size_t write(uint8_t c) { if (checkItFits(len+1)) { buffer[len++] = c; return 1; } return 0; }
  void writeDone()        { buffer[len] = 0; }

  friend BaseClass;

  // Operators
public:
  // Strings
  StringBase & operator += (const StringBase & other) { return append(other.buffer, other.len); }
  StringBase & operator += (const char * txt)         { return append(txt); }
  StringBase & operator += (const uint8_t * txt)      { return append((const char*)txt); }
  StringBase & operator += (const char ch)            { char txt[2] = { ch, 0}; return append(txt, 1); }

  // Numbers
  FORCE_INLINE StringBase & operator += (const int c)              { print(c, PrintBase::Dec); return *this; }
  FORCE_INLINE StringBase & operator += (const unsigned int c)     { print(c, PrintBase::Dec); return *this; }
  FORCE_INLINE StringBase & operator += (const unsigned long c)    { print(c, PrintBase::Dec); return *this; }
  FORCE_INLINE StringBase & operator += (const long c)             { print(c, PrintBase::Dec); return *this; }
  FORCE_INLINE StringBase & operator += (const double c)           { print(c, 2); return *this; }

  // print functions too
  using BaseClass::print;

  // Compare operator
  bool operator == (const char * txt) const           { return strlen(txt) == len && memcmp(txt, buffer, len) == 0; }
  bool operator == (const StringBase & other) const   { return other.len == len && memcmp(other.buffer, buffer, len) == 0; }
  bool operator != (const char * txt) const           { return !(*this == txt); }
  bool operator != (const StringBase & other) const   { return !(*this == other); }

  // Interface
public:
  // C style casting
  FORCE_INLINE operator const char *() const { return buffer; }
  // Get the current length of the string
  FORCE_INLINE size_t length() const { return len; }

  // Construct the string
  StringBase(char * buffer, uint16_t allocSize) : buffer(buffer), len(0), allocSize(allocSize) {}
};


/** A stack based string. Mainly used to the build a complex string from simple operations.
    The maximum string size is preallocated upon construction on the stack, so you can't pass this object around. */
template <size_t N>
struct SString : public StringBase
{
  char obj[N];
  // StringBase interface
public:
  // Ensure the string fits the buffer, or bug on
  bool checkItFits(uint16_t newSize) {
    if (newSize >= this->allocSize - 1) {
      BUG_ON("Overflowing string", newSize, " / ", this->allocSize);
      return false;
    }
    return true;
  }

  // Construction with allocation
  SString() : StringBase(obj, N) {}
  ~SString() {
    // Not absolutely required, but it'll crash instantly if used, it's easier to debug this way
    this->allocSize = this->len = 0; this->buffer = nullptr;
  }
};


/** A dynamic allocation string class. Use very very sparsely and don't let any instance
    lingering around since it'll cause fragmentation on the allocator */
struct DString : public StringBase
{
  // StringBase interface
public:
  // Ensure the string fits the buffer, or bug on
  bool checkItFits(uint16_t newSize) {
    if (newSize >= allocSize - 1) {
      char * old = buffer;
      buffer = (char*)realloc(buffer, newSize + 1);
      if (buffer == NULL) {
        buffer = old;
        BUG_ON("Overflowing string", newSize, " / ", allocSize);
        return false;
      }
    }
    return true;
  }

  // Construction with allocation
  DString(const uint16_t alloc = 0) : StringBase((char*)malloc(alloc), alloc) {}
  ~DString() {
    free(buffer);
    // Not absolutely required, but it'll crash instantly if used, it's easier to debug this way
    allocSize = len = 0; buffer = nullptr;
  }
};
