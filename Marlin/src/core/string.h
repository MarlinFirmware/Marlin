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

/**
 * core/string.h
 * Copyright (c) 2021 X-Ryl669
 */

#include <stdarg.h>
#include "number_format.h"
#include "allocator.h"

class StringBase;

#ifndef ARDUINO
  // On non Arduino platform, __FlashStringHelper isn't defined. (And we don't care). So define it here
  struct __FlashStringHelper {};
#endif

/** Simple R/O string used for parsing task.
    Makes the code easier to grasp because it's using a fluent interface and works on concept, not chars.
    Unlike C string, this string class is not nul terminated, so it can't be used in C style like functions */
class ROString {
  const char *data;
  int length;

  // Interface
public:
  // Get a pointer on the data
  inline const char*  buffer() const { return data; }
  // Get the string length
  inline const int len() const { return length; }
  // Limit the string length to the given value
  inline bool limitTo(const int newLength)  { if (newLength > length) return false; (void)mutate(data, newLength); return true; }
  // Revert a position that was globbed previously. Calling this method is unsafe, since there's no check that the pointers are still valid.
  inline ROString& revert(const int amount) { length += amount; data -= amount; return *this; }
  // Get the substring from this string
  ROString midString(int left, int len) const { return ROString(left < length ? &data[left] : "", clamp(len, 0, length - left)); }

  /** Split at the given position.
      For example, the following code gives:
      @code
        ROString text = "abcdefdef";
        ROString ret = text.splitAt(3); // ret = "abc", text = "defdef"
        ret = text.splitAt(3, 1);     // ret = "de", text = "def"
        ret = text.splitAt(9);        // ret = "def", text = ""
      @endcode
      @param pos    The position to split this string.
                    If the position is larger than the string's length, the complete string is returned,
                    and this string is modified to be empty.
                    If the position is negative, an empty string is returned, and this string is left
                    unmodified.
      @param stripFromRet   This is the amount of characters to strip from the right of the returned string.
                            This is equivalent to .limitTo(len() - stripFromRet)
      @return The part from the left to the given position. */
  ROString splitAt(int pos, int stripFromRet = 0);

  // Trim the string from the given char (and direction)
  ROString trimRight(const char ch) const { int len = length; while(len > 1 && data && data[len - 1] == ch) len--; return ROString(data, len); }

  // Trim the string from the given char (and direction)
  ROString trimLeft(const char ch) const { int len = length; while(len > 1 && data && data[length - len] == ch) len--; return ROString(data + (length - len), len); }

  // Trim the beginning of string from any char in the given array
  ROString trimmedLeft(const char* chars, int nlen = 0) const { int len = length; if (!nlen && chars) nlen = (int)strlen(chars); while(len > 1 && data && memchr(chars, data[length - len], nlen) != NULL) len--; return ROString(data + (length - len), len); }

  // Trim the end of string from any char in the given array
  ROString trimmedRight(const char* chars, int nlen = 0) const { int len = length; if (!nlen && chars) nlen = (int)strlen(chars); while(len > 1 && data && memchr(chars, data[len - 1], nlen) != NULL) len--; return ROString(data, len); }

  /** Trim the beginning of string from any char in the given array.
      This is using fluent interface and modifies the internal object. */
  ROString& leftTrim(const char* chars, int nlen = 0) { int len = length; if (!nlen && chars) nlen = (int)strlen(chars); while(len > 1 && data && memchr(chars, data[length - len], nlen) != NULL) len--; return mutate(data + (length - len), len); }

  /** Trim the beginning of string from any char in the given array.
      This is using fluent interface and modifies the internal object. */
  template <size_t nlen> ROString& leftTrim(const char (&chars)[nlen]) { int len = length; while(len > 1 && data && memchr(chars, data[length - len], nlen-1) != NULL) len--; return mutate(data + (length - len), len); }

  /** Trim the end of string from any char in the given array
      This is using fluent interface and modifies the internal object. */
  ROString& rightTrim(const char* chars, int nlen = 0) { int len = length; if (!nlen && chars) nlen = (int)strlen(chars); while(len > 1 && data && memchr(chars, data[len - 1], nlen) != NULL) len--; return mutate(data, len); }

  /** Trim the end of string from any char in the given array
      This is using fluent interface and modifies the internal object. */
  template <size_t nlen> ROString& rightTrim(const char (&chars)[nlen]) { int len = length; while(len > 1 && data && memchr(chars, data[len - 1], nlen - 1) != NULL) len--; return mutate(data, len); }

  // Trim the string from any char in the given array
  ROString Trimmed(const char* chars, int nlen = 0) const {
    int llen = length, rlen = length;
    if (!nlen && chars) nlen = (int)strlen(chars);
    while(nlen && llen > 1 && data && memchr(chars, data[length - llen], nlen) != NULL) llen--;
    while(nlen && rlen > 1 && data && memchr(chars, data[rlen - 1], nlen) != NULL) rlen--;
    return ROString(data + (length - llen), rlen - (length  - llen));
  }

  // Trim the string from any char in the given array
  ROString Trimmed(const ROString &t) const {
    int llen = length, rlen = length;
    while(t.length && llen > 1 && data && memchr(t.data, data[length - llen], t.length) != NULL) llen--;
    while(t.length && rlen > 1 && data && memchr(t.data, data[rlen - 1], t.length) != NULL) rlen--;
    return ROString(data + (length - llen), rlen - (length  - llen));
  }

  /** Trim the string from any char in the given array
      This is using fluent interface and modifies the internal object. */
  ROString& Trim(const char* chars, int nlen = 0) {
      int llen = length, rlen = length;
      if (!nlen && chars) nlen = (int)strlen(chars);
      while(nlen && llen > 1 && data && memchr(chars, data[length - llen], nlen) != NULL) llen--;
      while(nlen && rlen > 1 && data && memchr(chars, data[rlen - 1], nlen) != NULL) rlen--;
      return mutate(data + (length - llen), rlen - (length  - llen));
  }

  /** Trim the string from any char in the given array
      This is using fluent interface and modifies the internal object. */
  ROString& Trim(const ROString &t) {
    int llen = length, rlen = length;
    while(t.length && llen > 1 && data && memchr(t.data, data[length - llen], t.length) != NULL) llen--;
    while(t.length && rlen > 1 && data && memchr(t.data, data[rlen - 1], t.length) != NULL) rlen--;
    return mutate(data + (length - llen), rlen - (length  - llen));
  }

  /** Find the specific needle in the string.
      This is a very simple O(n*m) search.
      @return the position of the needle, or len() if not found. */
  const unsigned int find(const ROString &needle, unsigned int pos = 0, const bool caseless = false) const;

  /** Find any of the given set of chars
      @return the position of the needle, or len() if not found. */
  const unsigned int findAnyChar(const char * chars, unsigned int pos = 0, int nlen = 0) const { int len = pos; if (!nlen && chars) nlen = (int)strlen(chars); while(len < length && data && memchr(chars, data[len], nlen) == NULL) len++; return len; }

  /** Find first char that's not in the given set of chars
      @return the position of the needle, or len() if not found. */
  const unsigned int invFindAnyChar(const char * chars, unsigned int pos = 0, int nlen = 0) const { int len = pos; if (!nlen && chars) nlen = (int)strlen(chars); while(len < length && data && memchr(chars, data[len], nlen) != NULL) len++; return len; }

  /** Find the specific needle in the string, starting from the end of the string.
      This is a very simple O(n*m) search.
      @return the position of the needle, or len() if not found. */
  const unsigned int reverseFind(const ROString &needle, unsigned int pos = (unsigned int)-1, const bool caseless = false) const;

  // Count the number of times the given substring appears in the string
  const unsigned int count(const ROString &needle) const;

  /** Split a string when the needle is found first, returning the part before the needle, and
      updating the string to start on or after the needle.
      If the needle is not found, it returns an empty string if includeFind is false, or the whole string if true.
      For example this code returns:
      @code
        ROString text = "abcdefdef";
        ROString ret = text.splitFrom("d"); // ret = "abc", text = "efdef"
        ret = text.splitFrom("f", true);  // ret = "e", text = "fdef"
      @endcode
      @param find         The string to look for
      @param includeFind  If true the string is updated to start on the find text. */
  const ROString splitFrom(const ROString &find, const bool includeFind = false, const bool caseless = false);

  /** Get the substring from the given needle up to the given needle.
      For example, this code returns:
      @code
        ROString text = "abcdefdef";
        ROString ret = text.fromTo("d", "f"); // ret = "e"
        ret = text.fromTo("d", "f", true);  // ret = "def"
        ret = text.fromTo("d", "g"); // ret = ""
        ret = text.fromTo("d", "g", true); // ret = "defdef"
        ret = text.fromTo("g", "f", [true or false]); // ret = ""
      @endcode

      @param from         The first needle to look for
      @param to           The second needle to look for
      @param includeFind  If true, the text searched for is included in the result
      @return If "from" needle is not found, it returns an empty string, else if "to" needle is not found,
              it returns an empty string upon includeFind being false, or the string starting from "from" if true. */
  const ROString fromTo(const ROString &from, const ROString &to, const bool includeFind = false, const bool caseless = false) const;

  /** Get the string up to the first occurrence of the given string
      If not found, it returns the whole string unless includeFind is true (empty string in that case).
      For example, this code returns:
      @code
        ROString ret = String("abcdefdef").upToFirst("d"); // ret == "abc"
        ROString ret = String("abcdefdef").upToFirst("g"); // ret == "abcdefdef"
      @endcode
      @param find         The text to look for
      @param includeFind  If set, the needle is included in the result */
  const ROString upToFirst(const ROString &find, const bool includeFind = false, const bool caseless = false) const;

  /** Get the string up to the last occurrence of the given string
      If not found, it returns the whole string unless includeFind is true (empty string in that case).
      For example, this code returns:
      @code
        ROString ret = String("abcdefdef").upToLast("d"); // ret == "abcdef"
        ROString ret = String("abcdefdef").upToLast("g"); // ret == "abcdefdef"
      @endcode
      @param find         The text to look for
      @param includeFind  If set, the needle is included in the result */
  const ROString upToLast(const ROString &find, const bool includeFind = false, const bool caseless = false) const;

  /** Get the string from the last occurrence of the given string.
      If not found, it returns an empty string if includeFind is false, or the whole string if true
      For example, this code returns:
      @code
        ROString ret = String("abcdefdef").fromLast("d"); // ret == "ef"
        ROString ret = String("abcdefdef").fromLast("d", true); // ret == "def"
        ROString ret = String("abcdefdef").fromLast("g"); // ret == ""
      @endcode
      @param find         The text to look for
      @param includeFind  If set, the needle is included in the result */
  const ROString fromLast(const ROString &find, const bool includeFind = false, const bool caseless = false) const;

  /** Get the string from the first occurrence of the given string
      If not found, it returns an empty string if includeFind is false, or the whole string if true
      For example, this code returns:
      @code
        ROString ret = String("abcdefdef").fromFirst("d"); // ret == "efdef"
        ROString ret = String("abcdefdef").fromFirst("d", true); // ret == "defdef"
        ROString ret = String("abcdefdef").fromFirst("g"); // ret == ""
      @endcode
      @param find         The text to look for
      @param includeFind  If set, the needle is included in the result */
  const ROString fromFirst(const ROString &find, const bool includeFind = false, const bool caseless = false) const;

  /** Get the substring from the given needle if found, or the whole string if not.
      For example, this code returns:
      @code
        ROString text = "abcdefdef";
        ROString ret = text.dropUpTo("d"); // ret = "efdef"
        ret = text.dropUpTo("d", true); // ret = "defdef"
        ret = text.dropUpTo("g", [true or false]); // ret = "abcdefdef"
      @endcode
      @param find         The string to look for
      @param includeFind  If true the string is updated to start on the find text. */
  const ROString dropUpTo(const ROString &find, const bool includeFind = false, const bool caseless = false) const;

  /** Get the substring up to the given needle if found, or the whole string if not, and split from here.
      For example, this code returns:
      @code
        ROString text = "abcdefdef";
        ROString ret = text.splitUpTo("d"); // ret = "abc", text = "efdef"
        ret = text.splitUpTo("g", [true or false]); // ret = "efdef", text = ""
        text = "abcdefdef";
        ret = text.splitUpTo("d", true); // ret = "abcd", text = "efdef"
      @endcode
      @param find         The string to look for
      @param includeFind  If true the string is updated to start on the find text. */
  const ROString splitUpTo(const ROString &find, const bool includeFind = false, const bool caseless = false);

  /** Eat the characters until the text is no more in the given set.
      The string is split at this position.
      @return The string made only from characters from the given set.
      @sa letterSet, digitSet
      For example, this code returns:
      @code
        ROString text = "_abs123 defgh";
        ROString ret = text.splitWhenNoMore("abcdefghijklmnopqrstuvwxyz_0123456789"); // text = " defgh", ret = "_abs123"
      @endcode */
  const ROString splitWhenNoMore(const ROString &set);

  //  So you can check the string directly for emptiness
  inline bool operator !() const { return length == 0; }
  //  So you can check the string directly for emptiness
  inline explicit operator bool() const { return length > 0; }
  //  Operator [] to access a single char
  char operator[] (int index) const { return index < length ? data[index] : 0; }

  //  Caseless compare
  inline const int caselessCmp(const ROString &copy) const {
    int ret = strncasecmp(data, copy.data, length < copy.length ? length : copy.length);
    if (!ret && length == copy.length) return 0;
    if (ret < 0 || (!ret && length < copy.length)) return -1;
    return 1;
  }

  /** Get the integer out of this string.
      This method support any usual encoding of the integer, and detect the integer format automatically.
      This method is optimized for speed, and does no memory allocation on heap
      Supported formats examples: "0x1234, 0700, -1234, 0b00010101"
      @param base     If provided, only the given base is supported (default to 0 for auto-detection).
      @param consumed If provided, will be filled with the number of consumed characters.
      @return The largest possible integer that's parseable. */
  int parseInt(const int base = 0, int * consumed = 0) const { char * end = 0; int out = strtol((const char*)data, &end, base); if (consumed) *consumed = (int)(end - (char*)data); return out; }
  /** Extract an integer from this string. This is equivalent to:
      @code
        int pos = 0;
        int ret = str.extractInt(base, &pos);
        str.splitAt(pos);
      @endcode
      @param base     If provided, only the given base is supported (default to 0 for auto-detection).
      @return The largest possible integer that's parseable. */
  int extractInt(const int base = 0) { int pos = 0, ret = parseInt(base, &pos); (void)mutate(data + pos, length - pos); return ret; }

  // Construction and operators
public:
  // Default constructor
  ROString(const char * _data = 0, const int _length = -1) : data(_data), length(_length == -1 ? (_data ? strlen(_data) : 0) : _length) { }
  // Convertion constructor, the given object must live after around this lifetime
  ROString(const StringBase &other);
  // The destructor
  ~ROString() {}
  // Copy constructor
  ROString(const ROString &copy) = default;
  // Move constructor
  ROString(ROString && copy) = default;
  // Equal operator
  inline ROString& operator = (const ROString &copy) { if (&copy != this) return mutate(copy.data, copy.length); return *this; }
  // Compare operator
  inline const bool operator == (const ROString &copy) const { return length == copy.length && memcmp(data, copy.data, length) == 0; }
  // Sort operator
  inline const bool operator < (const ROString &copy) const { int ret = memcmp(data, copy.data, length < copy.length ? length : copy.length); return (ret < 0 || (!ret && length < copy.length)); }
  // Compare operator
  inline const bool operator == (const char * copy) const { return length == (int)strlen(copy) && memcmp(data, copy, length) == 0; }
  // Compare operator
  template <size_t N> inline const bool operator == (const char (&copy)[N]) const { return length == N-1 && memcmp(data, copy, length) == 0; }
  // Compare operator
  const bool operator == (const StringBase &copy) const;
  // Inverted compare operator
  inline const bool operator != (const ROString &copy) const { return !operator ==(copy); }
  // Inverted compare operator
  inline const bool operator != (const char * copy) const { return !operator ==(copy); }
  // Inverted compare operator
  inline const bool operator != (const StringBase &copy) const { return !operator ==(copy); }

private:
  // mutate this string head positions (not the content)
  //inline const ROString& mutate(const char * d, const int len) const { const_cast<const char * & >(data) = d; const_cast<int&>(length) = len; return *this; }

  // mutate this string head positions (not the content)
  inline ROString& mutate(const char * d, const int len) { const_cast<const char * & >(data) = d; const_cast<int&>(length) = len; return *this; }
  #if ENABLED(MARLIN_DEV_MODE)
    // Prevent unwanted conversion
    // If compiler stops here, it's because you're trying to compare a string with a type it doesn't understand.
    template <typename T> bool operator == (const T &t) const {
      static_assert(sizeof(T) != sizeof(T)); return false;
    }
    template <typename T> bool operator < (const T &t) const {
      static_assert(sizeof(T) != sizeof(T)); return false;
    }
    template <typename T> bool operator > (const T &t) const {
      static_assert(sizeof(T) != sizeof(T)); return false;
    }
    template <typename T> bool operator <= (const T &t) const {
      static_assert(sizeof(T) != sizeof(T)); return false;
    }
    template <typename T> bool operator >= (const T &t) const {
      static_assert(sizeof(T) != sizeof(T)); return false;
    }
    template <typename T> bool operator != (const T &t) const {
      static_assert(sizeof(T) != sizeof(T)); return false;
    }
  #endif
};

// Basic R/W string class that's not managing the memory it's using.
// Use one of the child classes to provide the reuired memory management.
class StringBase : public NumberFormatter< StringBase > {
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
  StringBase& append(const char * txt, size_t txtLen = 0) {
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
  friend ROString;

  // Prevent copy construction
  StringBase(const StringBase &other);

  // Operators
public:
  // Strings
  StringBase& operator += (const StringBase &other)  { return append(other.buffer, other.len); }
  StringBase& operator += (const char * txt)         { return append(txt); }
  StringBase& operator += (const uint8_t * txt)      { return append((const char*)txt); }
  StringBase& operator += (const char ch)            { char txt[2] = { ch, 0}; return append(txt, 1); }
  // Arduino hack for handling PROGMEM strings
  StringBase& operator += (const __FlashStringHelper * txt)      { return append((const char*)pgm_read_ptr(txt)); }

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
  bool operator == (const StringBase &other) const   { return other.len == len && memcmp(other.buffer, buffer, len) == 0; }
  // Arduino hack for handling PROGMEM strings
  bool operator == (const __FlashStringHelper * txt)  { return *this == (const char*)pgm_read_ptr(txt); }
  bool operator != (const char * txt) const           { return !(*this == txt); }
  bool operator != (const StringBase &other) const   { return !(*this == other); }

  StringBase& operator = (const StringBase &other)   { len = 0; return *this += other; }

  // Reset the string
  void clear()  { len = 0; buffer[0] = 0; }
  // Non empty check
  explicit operator bool() const { return len > 0; }

  // Interface
public:
  // C style casting
  FORCE_INLINE operator const char *() const { return buffer; }
  // Get the current length of the string
  FORCE_INLINE size_t length() const { return len; }
  // Get access to the underlying buffer
  FORCE_INLINE char * buf() { return buffer; }

  // Construct the string
  StringBase(char * buffer, uint16_t allocSize, uint16_t len = 0) : buffer(buffer), len(len), allocSize(allocSize) {}
  StringBase(StringBase &&other) = default;
};


/** A stack based string. Mainly used to the build a complex string from simple operations.
    The maximum string size is preallocated upon construction on the stack, so you can't pass this object around. */
template <size_t N>
struct SString : public StringBase {
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
  SString(const SString &other) : StringBase(obj, N) { memcpy(obj, other.obj, N); }
  // For PSTR strings (use the F macro here)
  SString(const __FlashStringHelper * str) : StringBase(obj, N, strlen_P(reinterpret_cast<const char*>(str))) { memcpy_P(obj, str, len + 1); }
  // Simple copy construct
  SString(const char * str) : StringBase(obj, N, strlen(str) < N - 1 ? strlen(str) : N - 1) { memcpy(obj, str, len + 1); }


  ~SString() {
    // Not absolutely required, but it'll crash instantly if used, it's easier to debug this way
    this->allocSize = this->len = 0; this->buffer = nullptr;
  }

private:
  // This can't be used so prevent to compile code using this
  SString(const SString &&other);
};

/** A dynamic allocation string class using the StackLikeAllocator.
    It's using RAII (resource acquisition is initialization) principle to manage a buffer that's not allocated on the stack. */
struct DString : public StringBase {
  // StringBase interface
public:
  // Ensure the string fits the buffer, or bug on
  bool checkItFits(uint16_t newSize) {
    if (newSize >= allocSize - 1) {
      if (!SLAlloc::instance().grow(newSize - allocSize + 1)) {
        BUG_ON("Overflowing string", newSize, " / ", allocSize);
        return false;
      }
    }
    return true;
  }

  // Printf like formating and construction at the same time
  static DString format(const __FlashStringHelper * str, ...) {
    //TODO: Compute the minimum required size for the format string at compile time.
    // Right now, let's do it at runtime and chain the call

    va_list arglist, snd;
    va_start(arglist, str);
    va_copy(snd, arglist);
    // Count the required size
    size_t sz = vsnprintf_P(NULL, 0, (const char*)str, arglist);
    va_end(arglist);
    // Then output the string here
    DString ret(sz);
    ret.len = vsnprintf_P(ret.buffer, ret.allocSize, (const char*)str, snd);
    va_end(snd);
    return ret;
  }

  // Construction with allocation
  DString(const uint16_t alloc = 0) : StringBase((char*)SLAlloc::instance().lease(alloc), alloc) {}

  DString(DString &&other) : StringBase(other.buffer, other.allocSize, other.len) { other.buffer = nullptr; other.len = other.allocSize = 0; }
  DString(const __FlashStringHelper * str) : StringBase(
        (char*)SLAlloc::instance().lease(strlen_P(reinterpret_cast<const char*>(str))+1),
        strlen_P(reinterpret_cast<const char*>(str))+1,
        strlen_P(reinterpret_cast<const char*>(str))) {
    memcpy_P(buffer, str, len+1);
  }
  DString(const char * str) : StringBase((char*)SLAlloc::instance().lease(strlen(str)), strlen(str+1), strlen(str)) { memcpy(buffer, str, len+1); }
  DString(const ROString &s) : StringBase((char*)SLAlloc::instance().lease(s.len()+1), s.len()+1, s.len()) { memcpy(buffer, s.buffer(), len+1); }

  ~DString() {
    SLAlloc::instance().release(allocSize);
    // Not absolutely required, but it'll crash instantly if used, it's easier to debug this way
    allocSize = len = 0; buffer = nullptr;
  }
};
