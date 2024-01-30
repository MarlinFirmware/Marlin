/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * Lightweight string class template providing operators for all common tasks
 * and conversion from F() and PSTR() strings into SRAM strings that reside
 * on the stack or persistently, with overflow prevention.
 *
 * Examples:
 *
 *   MString<50> mystr(F("Hello "), intvar, " World"); // "Hello 3 World"
 *
 *   mystr.append(" (", p_float_t(123.4, 2), ')');     // "Hello 3 World (123.40)"
 *
 *   mystr.clear();
 *
 *   mystr.append(spaces_t(10), repchr_t('-', 5));     // Repeats are sometimes cheaper than strings
 *
 *   mystr.appendf(F(" ... %i/%i"), count, total);     // Works like printf, requires F string
 *
 */

#include "types.h"
#include "utility.h" // AXIS_CHAR
#include "../lcd/utf8.h"

#ifndef DEFAULT_MSTRING_SIZE
  #define DEFAULT_MSTRING_SIZE 20
#endif

//#define UNSAFE_MSTRING  // Don't initialize the string to "" or set a terminating nul
//#define USE_SPRINTF     // Use sprintf instead of snprintf
//#define DJB2_HASH       // 32-bit hash with Djb2 algorithm
//#define MSTRING_DEBUG   // Debug string operations to diagnose memory leaks
//#define FASTER_APPEND   // Append without using an intermediate buffer

// Declare externs for serial debug output
template <typename T> extern void SERIAL_ECHO(T x);
template <typename T> extern void SERIAL_ECHOLN(T x);
extern void SERIAL_ECHO(serial_char_t x);
extern void SERIAL_CHAR(char c);

#define START_OF_UTF8_CHAR(C) (((C) & 0xC0u) != 0x80U)

#if ENABLED(USE_SPRINTF)
  #define SNPRINTF(A, S, V...)   sprintf(A, V)
  #define SNPRINTF_P(A, S, V...) sprintf_P(A, V)
#else
  #define SNPRINTF(V...)   snprintf(V)
  #define SNPRINTF_P(V...) snprintf_P(V)
#endif

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"

#if DISABLED(UNSAFE_MSTRING) && GCC_VERSION >= 80000
  #pragma GCC diagnostic ignored "-Wstringop-truncation"
#endif

/**
 * @brief MString class template
 * @details A class template providing convenient string operators,
 *          very similar to the Arduino String class, as it turns out.
 *
 * @tparam SIZE The pre-allocated storage for the string buffer
 */
template <int SIZE=DEFAULT_MSTRING_SIZE, bool SAFE=DISABLED(UNSAFE_MSTRING)>
class MString {
protected:
  char str[SIZE+1];
public:
  MString() { safety(0); safety(SIZE); }

  template<typename T>
  MString(const T v) { set(v); safety(SIZE); }

  static_assert(SIZE > 0, "Bad SIZE for MString!");

  void debug(FSTR_P const f) {
    #if ENABLED(MSTRING_DEBUG)
      SERIAL_ECHOLN(f, C(':'), uintptr_t(str), C(' '), length(), C(' '), str);
    #endif
  }

  void safety(const int n) { if (SAFE && n <= SIZE) str[n] = '\0'; }

  // Chainable String Setters
  MString& set()                          { str[0] = '\0';                            debug(F("clear"));   return *this; }
  MString& set(char *s)                   { strlcpy(str, s, SIZE + 1);                debug(F("string"));  return *this; }
  MString& set(const char *s)             { return set(const_cast<char*>(s)); }
  MString& set_P(PGM_P const s)           { strlcpy_P(str, s, SIZE + 1);              debug(F("pstring")); return *this; }
  MString& set(FSTR_P const f)            { return set_P(FTOP(f)); }
  MString& set(const bool &b)             { return set(b ? F("true") : F("false")); }
  MString& set(const char c)              { str[0] = c; str[1] = '\0';                debug(F("char"));    return *this; }
  MString& set(const int8_t &i)           { SNPRINTF_P(str, SIZE, PSTR("%d"),  i);    debug(F("int8_t"));  return *this; }
  MString& set(const short &i)            { SNPRINTF_P(str, SIZE, PSTR("%d"),  i);    debug(F("short"));   return *this; }
  MString& set(const int &i)              { SNPRINTF_P(str, SIZE, PSTR("%d"),  i);    debug(F("int"));     return *this; }
  MString& set(const long &l)             { SNPRINTF_P(str, SIZE, PSTR("%ld"), l);    debug(F("long"));    return *this; }
  MString& set(const unsigned char &i)    { SNPRINTF_P(str, SIZE, PSTR("%u"),  i);    debug(F("uchar"));   return *this; }
  MString& set(const unsigned short &i)   { SNPRINTF_P(str, SIZE, PSTR("%u"),  i);    debug(F("ushort"));  return *this; }
  MString& set(const unsigned int &i)     { SNPRINTF_P(str, SIZE, PSTR("%u"),  i);    debug(F("uint"));    return *this; }
  MString& set(const unsigned long &l)    { SNPRINTF_P(str, SIZE, PSTR("%lu"), l);    debug(F("ulong"));   return *this; }
  MString& set(const float &f)            { return set(p_float_t(f, SERIAL_FLOAT_PRECISION)); }
  MString& set(const p_float_t &pf)       { return set(w_float_t(pf.value, 1, pf.prec)); }
  MString& set(const w_float_t &wf)       { char f1[20]; return set(dtostrf(wf.value, wf.width, wf.prec, f1)); }
  MString& set(const serial_char_t &v)    { return set(char(v.c)); }
  MString& set(const xyz_pos_t &v)        { set(); return append(v); }
  MString& set(const xyze_pos_t &v)       { set(); return append(v); }

  template <int S>
  MString& set(const MString<S> &m)       { strlcpy(str, &m, SIZE + 1); debug(F("MString")); return *this; }

  MString& setn(char *s, int len)         { int c = _MIN(len, SIZE); strlcpy(str, s, c + 1); debug(F("string")); return *this; }
  MString& setn(const char *s, int len)   { return setn(const_cast<char*>(s), len); }
  MString& setn_P(PGM_P const s, int len) { int c = _MIN(len, SIZE); strlcpy_P(str, s, c + 1); debug(F("pstring")); return *this; }
  MString& setn(FSTR_P const f, int len)  { return setn_P(FTOP(f), len); }

  // set(repchr_t('-', 10))
  MString& set(const repchr_t &s)         { int c = _MIN(s.count, SIZE); if (c >= 0) { if (c > 0) memset(str, s.asc, c); str[c] = '\0'; } debug(F("repchr_t")); return *this; }

  // set(spaces_t(10))
  MString& set(const spaces_t &s)         { repchr_t r(' ', s.count); return set(r); }

  // Set with format string and arguments, like printf
  template<typename... Args>
  MString& setf_P(PGM_P const fmt, Args... more) { SNPRINTF_P(str, SIZE, fmt, more...); debug(F("setf_P")); return *this; }

  template<typename... Args>
  MString& setf(const char *fmt, Args... more)   { SNPRINTF(str, SIZE, fmt, more...);   debug(F("setf"));   return *this; }

  template<typename... Args>
  MString& setf(FSTR_P const fmt, Args... more)  { return setf_P(FTOP(fmt), more...); }

  // Chainable String appenders
  MString& append()                           { debug(F("nil")); return *this; } // for macros that might emit no output
  MString& append(char *s)                    { int sz = length(); if (sz < SIZE) strlcpy(str + sz, s, SIZE - sz + 1); debug(F("string")); return *this; }
  MString& append(const char *s)              { return append(const_cast<char *>(s)); }
  MString& append_P(PGM_P const s)            { int sz = length(); if (sz < SIZE) strlcpy_P(str + sz, s, SIZE - sz + 1); debug(F("pstring")); return *this; }
  MString& append(FSTR_P const f)             { return append_P(FTOP(f)); }
  MString& append(const bool &b)              { return append(b ? F("true") : F("false")); }
  MString& append(const char c)               { int sz = length(); if (sz < SIZE) { str[sz] = c; if (sz < SIZE - 1) str[sz + 1] = '\0'; } return *this; }
  #if ENABLED(FASTER_APPEND)
    MString& append(const int8_t &i)          { int sz = length(); SNPRINTF(&str[sz], SIZE - sz, "%d",  i); return *this; }
    MString& append(const short &i)           { int sz = length(); SNPRINTF(&str[sz], SIZE - sz, "%d",  i); return *this; }
    MString& append(const int &i)             { int sz = length(); SNPRINTF(&str[sz], SIZE - sz, "%d",  i); return *this; }
    MString& append(const long &l)            { int sz = length(); SNPRINTF(&str[sz], SIZE - sz, "%ld", l); return *this; }
    MString& append(const unsigned char &i)   { int sz = length(); SNPRINTF(&str[sz], SIZE - sz, "%u",  i); return *this; }
    MString& append(const unsigned short &i)  { int sz = length(); SNPRINTF(&str[sz], SIZE - sz, "%u",  i); return *this; }
    MString& append(const unsigned int &i)    { int sz = length(); SNPRINTF(&str[sz], SIZE - sz, "%u",  i); return *this; }
    MString& append(const unsigned long &l)   { int sz = length(); SNPRINTF(&str[sz], SIZE - sz, "%lu", l); return *this; }
  #else
    MString& append(const int8_t &i)          { char buf[ 5]; sprintf(buf, "%d",  i); return append(buf); }
    MString& append(const short &i)           { char buf[12]; sprintf(buf, "%d",  i); return append(buf); }
    MString& append(const int &i)             { char buf[12]; sprintf(buf, "%d",  i); return append(buf); }
    MString& append(const long &l)            { char buf[12]; sprintf(buf, "%ld", l); return append(buf); }
    MString& append(const unsigned char &i)   { char buf[ 5]; sprintf(buf, "%u",  i); return append(buf); }
    MString& append(const unsigned short &i)  { char buf[11]; sprintf(buf, "%u",  i); return append(buf); }
    MString& append(const unsigned int &i)    { char buf[11]; sprintf(buf, "%u",  i); return append(buf); }
    MString& append(const unsigned long &l)   { char buf[11]; sprintf(buf, "%lu", l); return append(buf); }
  #endif
  MString& append(const float &f)             { return append(p_float_t(f, SERIAL_FLOAT_PRECISION)); }
  MString& append(const p_float_t &pf)        { return append(w_float_t(pf.value, 1, pf.prec)); }
  MString& append(const w_float_t &wf)        { char f1[20]; return append(dtostrf(wf.value, wf.width, wf.prec, f1)); }
  MString& append(const serial_char_t &v)     { return append(char(v.c)); }
  MString& append(const xyz_pos_t &v)         { LOOP_NUM_AXES(i)     { if (i) append(' '); append(AXIS_CHAR(i), v[i]); } debug(F("xyz")); return *this; }
  MString& append(const xyze_pos_t &v)        { LOOP_LOGICAL_AXES(i) { if (i) append(' '); append(AXIS_CHAR(i), v[i]); } debug(F("xyze")); return *this; }

  template<int S>
  MString& append(const MString<S> &m)        { return append(&m); }

  // Append only if the given space is available
  MString& appendn(char *s, int len)          { int sz = length(), c = _MIN(len, SIZE - sz); if (c > 0) { strlcpy(str + sz, s, c + 1); } debug(F("string")); return *this; }
  MString& appendn(const char *s, int len)    { return appendn(const_cast<char *>(s), len); }
  MString& appendn_P(PGM_P const s, int len)  { int sz = length(), c = _MIN(len, SIZE - sz); if (c > 0) { strlcpy_P(str + sz, s, c + 1); } debug(F("pstring")); return *this; }
  MString& appendn(FSTR_P const f, int len)   { return appendn_P(FTOP(f), len); }

  // append(repchr_t('-', 10))
  MString& append(const repchr_t &s) {
    const int sz = length(), c = _MIN(s.count, SIZE - sz);
    if (c > 0) { memset(str + sz, s.asc, c); str[sz + c] = '\0'; }
    debug(F("repchr"));
    return *this;
  }

  // append(spaces_t(10))
  MString& append(const spaces_t &s) { return append(repchr_t(' ', s.count)); }

  template<typename... Args>
  MString& appendf_P(PGM_P const fmt, Args... more) {
    int sz = length();
    if (sz < SIZE) SNPRINTF_P(str + sz, SIZE - sz, fmt, more...);
    debug(F("appendf_P"));
    return *this;
  }

  template<typename... Args>
  MString& appendf(const char *fmt, Args... more) {
    const int sz = length();
    if (sz < SIZE) SNPRINTF(str + sz, SIZE - sz, fmt, more...);
    debug(F("appendf"));
    return *this;
  }

  template<typename... Args>
  MString& appendf(FSTR_P const fmt, Args... more) { return appendf_P(FTOP(fmt), more...); }

  // Instantiate with a list of things
  template <typename T, typename... Args>
  MString(T arg1, Args... more)           { set(arg1); append(more...); }

  // Catch unhandled types to prevent infinite recursion
  template<typename T> MString& append(T) { return append(TERN(MSTRING_DEBUG, typeid(T).name(), '?')); }

  // Take a list of any number of arguments and append them to the string
  template<typename T, typename... Args>
  MString& append(T arg1, Args... more)   { return append(arg1).append(more...); }

  // Take a list of any number of arguments and set them in the string
  template<typename T, typename... Args>
  MString& set(T arg1, Args... more)      { return set(arg1).append(more...); }

  // Operator = as shorthand for set()
  template<typename T>
  MString& operator=(const T &v)          { return set(v);     }

  // Operator += as shorthand for append()
  template<typename T>
  MString& operator+=(const T &v)         { return append(v); }

  // Operator + as shorthand for append-to-copy
  template<typename T>
  MString operator+(const T &v)           { return MString(str, v); }

  #ifndef __AVR__
    MString(const double d)               { set(d); }
    MString& set(const double &f)         { return set(p_double_t(f, SERIAL_FLOAT_PRECISION)); }
    MString& set(const p_double_t &pf)    { return set(w_double_t(pf.value, 1, pf.prec)); }
    MString& set(const w_double_t &wf)    { char d1[20]; return set(dtostrf(wf.value, wf.width, wf.prec, d1)); }
    MString& append(const double &f)      { return append(p_double_t(f, SERIAL_FLOAT_PRECISION)); }
    MString& append(const p_double_t &pf) { return append(w_double_t(pf.value, 1, pf.prec)); }
    MString& append(const w_double_t &wf) { char d1[20]; return append(dtostrf(wf.value, wf.width, wf.prec, d1)); }
  #endif

  // Get the character at a given index
  char operator[](const int i) const { return str[i]; }

  // Cast to char* (explicit?)
  operator char* () { return str; }

  // Use &mystring as shorthand for mystring.str
  char* operator&() { return str; }

  // Return the buffer address (same as &)
  char* buffer() { return str; }

  int length() const { return strlen(str); }
  int glyphs() { return utf8_strlen(str); }
  bool empty() { return !str[0]; }

  // Quick hash to detect change (e.g., to avoid expensive drawing)
  typedef IF<ENABLED(DJB2_HASH), uint32_t, uint16_t>::type hash_t;
  hash_t hash() const {
    #if ENABLED(DJB2_HASH)
      hash_t hval = 5381;
      char c;
      while ((c = *str++)) hval += (hval << 5) + c; // = hval * 33 + c
    #else
      const int sz = length();
      hash_t hval = hash_t(sz);
      for (int i = 0; i < sz; i++) hval = ((hval << 1) | (hval >> 15)) ^ str[i]; // ROL, XOR
    #endif
    return hval;
  }

  void copyto(char * const dst) const { strcpy(dst, str); }
  void copyto(char * const dst, int len) const { strlcpy(dst, str, len + 1); }

  MString& clear() { return set(); }
  MString& eol() { return append('\n'); }
  MString& trunc(const int &i) { if (i <= SIZE) str[i] = '\0'; debug(F("trunc")); return *this; }

  // Truncate on a Unicode boundary
  MString& utrunc(const int &n=SIZE) {
    const int sz = length();
    if (sz && n <= sz)
      for (int i = n; i >= 0; i--) if (START_OF_UTF8_CHAR(str[i])) { str[i] = '\0'; break; }
    debug(F("utrunc"));
    return *this;
  }

};

#pragma GCC diagnostic pop

// Temporary inline string typically used to compose a G-code command
#ifndef TS_SIZE
  #define TS_SIZE 63
#endif
typedef MString<TS_SIZE, DISABLED(UNSAFE_MSTRING)> TString;
#define TS(V...) TString(V)
