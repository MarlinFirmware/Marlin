/**
 * Copyright (c) 2011-2018 Bill Greiman
 * This file is part of the SdFat library for SD memory cards.
 *
 * MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */
#include "FmtNumber.h"
// Use Stimmer div/mod 10 on avr
#ifdef __AVR__
#include <avr/pgmspace.h>
#define USE_STIMMER
#endif  // __AVR__
//------------------------------------------------------------------------------
// Stimmer div/mod 10 for AVR
// this code fragment works out i/10 and i%10 by calculating
// i*(51/256)*(256/255)/2 == i*51/510 == i/10
// by "j.k" I mean 32.8 fixed point, j is integer part, k is fractional part
// j.k = ((j+1.0)*51.0)/256.0
// (we add 1 because we will be using the floor of the result later)
// divmod10_asm16 and divmod10_asm32 are public domain code by Stimmer.
// http://forum.arduino.cc/index.php?topic=167414.msg1293679#msg1293679
#define divmod10_asm16(in32, mod8, tmp8)    \
asm volatile(          \
      " ldi %2,51     \n\t"     \
      " mul %A0,%2    \n\t"     \
      " clr %A0       \n\t"     \
      " add r0,%2     \n\t"     \
      " adc %A0,r1    \n\t"     \
      " mov %1,r0     \n\t"     \
      " mul %B0,%2    \n\t"     \
      " clr %B0       \n\t"     \
      " add %A0,r0    \n\t"     \
      " adc %B0,r1    \n\t"     \
      " clr r1        \n\t"     \
      " add %1,%A0    \n\t"     \
      " adc %A0,%B0   \n\t"     \
      " adc %B0,r1   \n\t"      \
      " add %1,%B0    \n\t"     \
      " adc %A0,r1   \n\t"      \
      " adc %B0,r1    \n\t"     \
      " lsr %B0       \n\t"     \
      " ror %A0       \n\t"     \
      " ror %1        \n\t"     \
      " ldi %2,10     \n\t"     \
      " mul %1,%2     \n\t"     \
      " mov %1,r1     \n\t"     \
      " clr r1        \n\t"     \
      :"+r"(in32), "=d"(mod8), "=d"(tmp8) : : "r0")

#define divmod10_asm32(in32, mod8, tmp8)    \
asm volatile(          \
      " ldi %2,51     \n\t"     \
      " mul %A0,%2    \n\t"     \
      " clr %A0       \n\t"     \
      " add r0,%2     \n\t"     \
      " adc %A0,r1    \n\t"     \
      " mov %1,r0     \n\t"     \
      " mul %B0,%2    \n\t"     \
      " clr %B0       \n\t"     \
      " add %A0,r0    \n\t"     \
      " adc %B0,r1    \n\t"     \
      " mul %C0,%2    \n\t"     \
      " clr %C0       \n\t"     \
      " add %B0,r0    \n\t"     \
      " adc %C0,r1    \n\t"     \
      " mul %D0,%2    \n\t"     \
      " clr %D0       \n\t"     \
      " add %C0,r0    \n\t"     \
      " adc %D0,r1    \n\t"     \
      " clr r1        \n\t"     \
      " add %1,%A0    \n\t"     \
      " adc %A0,%B0   \n\t"     \
      " adc %B0,%C0   \n\t"     \
      " adc %C0,%D0   \n\t"     \
      " adc %D0,r1    \n\t"     \
      " add %1,%B0    \n\t"     \
      " adc %A0,%C0   \n\t"     \
      " adc %B0,%D0   \n\t"     \
      " adc %C0,r1    \n\t"     \
      " adc %D0,r1    \n\t"     \
      " add %1,%D0    \n\t"     \
      " adc %A0,r1    \n\t"     \
      " adc %B0,r1    \n\t"     \
      " adc %C0,r1    \n\t"     \
      " adc %D0,r1    \n\t"     \
      " lsr %D0       \n\t"     \
      " ror %C0       \n\t"     \
      " ror %B0       \n\t"     \
      " ror %A0       \n\t"     \
      " ror %1        \n\t"     \
      " ldi %2,10     \n\t"     \
      " mul %1,%2     \n\t"     \
      " mov %1,r1     \n\t"     \
      " clr r1        \n\t"     \
      :"+r"(in32), "=d"(mod8), "=d"(tmp8) : : "r0")
//------------------------------------------------------------------------------
/*
// C++ code is based on this version of divmod10 by robtillaart.
// http://forum.arduino.cc/index.php?topic=167414.msg1246851#msg1246851
// from robtillaart post:
// The code is based upon the divu10() code from the book Hackers Delight1.
// My insight was that the error formula in divu10() was in fact modulo 10
// but not always. Sometimes it was 10 more.
void divmod10(uint32_t in, uint32_t &div, uint32_t &mod)
{
  // q = in * 0.8;
  uint32_t q = (in >> 1) + (in >> 2);
  q = q + (q >> 4);
  q = q + (q >> 8);
  q = q + (q >> 16);  // not needed for 16 bit version

  // q = q / 8;  ==> q =  in *0.1;
  q = q >> 3;

  // determine error
  uint32_t  r = in - ((q << 3) + (q << 1));   // r = in - q*10;
  div = q + (r > 9);
  if (r > 9) mod = r - 10;
  else mod = r;
}
// Hackers delight function is here:
// http://www.hackersdelight.org/hdcodetxt/divuc.c.txt
// Code below uses 8/10 = 0.1100 1100 1100 1100 1100 1100 1100 1100.
// 15 ops including the multiply, or 17 elementary ops.
unsigned divu10(unsigned n) {
   unsigned q, r;

   q = (n >> 1) + (n >> 2);
   q = q + (q >> 4);
   q = q + (q >> 8);
   q = q + (q >> 16);
   q = q >> 3;
   r = n - q*10;
   return q + ((r + 6) >> 4);
// return q + (r > 9);
}
*/
//------------------------------------------------------------------------------
#ifndef DOXYGEN_SHOULD_SKIP_THIS
#ifdef __AVR__
static const float m[] PROGMEM = {1e-1, 1e-2, 1e-4, 1e-8, 1e-16, 1e-32};
static const float p[] PROGMEM = {1e+1, 1e+2, 1e+4, 1e+8, 1e+16, 1e+32};
#else  // __AVR__
static const float m[] = {1e-1, 1e-2, 1e-4, 1e-8, 1e-16, 1e-32};
static const float p[] = {1e+1, 1e+2, 1e+4, 1e+8, 1e+16, 1e+32};
#endif  // __AVR__
#endif  // DOXYGEN_SHOULD_SKIP_THIS
// scale float v by power of ten. return v*10^n
float scale10(float v, int8_t n) {
  const float *s;
  if (n < 0) {
    n = -n;
    s = m;
  } else {
    s = p;
  }
  n &= 63;
  for (uint8_t i = 0; n; n >>= 1, i++) {
#ifdef __AVR__
    if (n & 1) {
      v *= pgm_read_float(&s[i]);
    }
#else  // __AVR__
    if (n & 1) {
      v *= s[i];
    }
#endif  // __AVR__
  }
  return v;
}
//------------------------------------------------------------------------------
// Format 16-bit unsigned
char* fmtDec(uint16_t n, char* p) {
  while (n > 9) {
#ifdef USE_STIMMER
    uint8_t tmp8, r;
    divmod10_asm16(n, r, tmp8);
#else  // USE_STIMMER
    uint16_t t = n;
    n = (n >> 1) + (n >> 2);
    n = n + (n >> 4);
    n = n + (n >> 8);
    // n = n + (n >> 16);  // no code for 16-bit n
    n = n >> 3;
    uint8_t r = t - (((n << 2) + n) << 1);
    if (r > 9) {
      n++;
      r -= 10;
    }
#endif  // USE_STIMMER
    *--p = r + '0';
  }
  *--p = n + '0';
  return p;
}
//------------------------------------------------------------------------------
// format 32-bit unsigned
char* fmtDec(uint32_t n, char* p) {
  while (n >> 16) {
#ifdef USE_STIMMER
    uint8_t tmp8, r;
    divmod10_asm32(n, r, tmp8);
#else  //  USE_STIMMER
    uint32_t t = n;
    n = (n >> 1) + (n >> 2);
    n = n + (n >> 4);
    n = n + (n >> 8);
    n = n + (n >> 16);
    n = n >> 3;
    uint8_t r = t - (((n << 2) + n) << 1);
    if (r > 9) {
      n++;
      r -= 10;
    }
#endif  // USE_STIMMER
    *--p = r + '0';
  }
  return fmtDec((uint16_t)n, p);
}
//------------------------------------------------------------------------------
char* fmtFloat(float value, char* p, uint8_t prec) {
  char sign = value < 0 ? '-' : 0;
  if (sign) {
    value = -value;
  }

  if (isnan(value)) {
    *--p = 'n';
    *--p = 'a';
    *--p = 'n';
    return p;
  }
  if (isinf(value)) {
    *--p = 'f';
    *--p = 'n';
    *--p = 'i';
    return p;
  }
  if (value > 4294967040.0) {
    *--p = 'f';
    *--p = 'v';
    *--p = 'o';
    return p;
  }
  if (prec > 9) {
    prec = 9;
  }
  value += scale10(0.5, -prec);

  uint32_t whole = value;
  if (prec) {
    char* tmp = p - prec;
    uint32_t fraction = scale10(value - whole, prec);
    p = fmtDec(fraction, p);
    while (p > tmp) {
      *--p = '0';
    }
    *--p = '.';
  }
  p = fmtDec(whole, p);
  if (sign) {
    *--p = sign;
  }
  return p;
}
//------------------------------------------------------------------------------
/** Print a number followed by a field terminator.
 * \param[in] value The number to be printed.
 * \param[in] ptr Pointer to last char in buffer.
 * \param[in] prec Number of digits after decimal point.
 * \param[in] expChar Use exp format if non zero.
 * \return Pointer to first character of result.
 */
char* fmtFloat(float value, char* ptr, uint8_t prec, char expChar) {
  bool neg = value < 0;
  if (neg) {
    value = -value;
  }

  // check for nan inf ovf
  if (isnan(value)) {
    *--ptr = 'n';
    *--ptr = 'a';
    *--ptr = 'n';
    return ptr;
  }
  if (isinf(value)) {
    *--ptr = 'f';
    *--ptr = 'n';
    *--ptr = 'i';
    return ptr;
  }
  if (!expChar && value > 4294967040.0) {
    *--ptr = 'f';
    *--ptr = 'v';
    *--ptr = 'o';
    return ptr;
  }
  if (prec > 9) {
    prec = 9;
  }
  float round = scale10(0.5, -prec);
  if (expChar) {
    int8_t exp = 0;
    bool expNeg = false;
    if (value) {
      while (value > 10.0) {
        value *= 0.1;
        exp++;
      }
      while (value < 1.0) {
        value *= 10.0;
        exp--;
      }
      value += round;
      if (value > 10.0) {
        value *= 0.1;
        exp++;
      }
      expNeg = exp < 0;
      if (expNeg) {
        exp = -exp;
      }
    }
    ptr = fmtDec((uint16_t)exp, ptr);
    if (exp < 10) {
      *--ptr = '0';
    }
    *--ptr = expNeg ? '-' : '+';
    *--ptr = expChar;
  } else {
    // round value
    value += round;
  }
  uint32_t whole = value;
  if (prec) {
    char* tmp = ptr - prec;
    uint32_t fraction = scale10(value - whole, prec);
    ptr = fmtDec(fraction, ptr);
    while (ptr > tmp) {
      *--ptr = '0';
    }
    *--ptr = '.';
  }
  ptr = fmtDec(whole, ptr);
  if (neg) {
    *--ptr = '-';
  }
  return ptr;
}
//------------------------------------------------------------------------------
char* fmtHex(uint32_t n, char* p) {
  do {
    uint8_t h = n & 0XF;
    *--p = h + (h < 10 ? '0' : 'A' - 10);
    n >>= 4;
  } while (n);
  return p;
}
//------------------------------------------------------------------------------
float scanFloat(const char* str, char** ptr) {
  int16_t const EXP_LIMIT = 100;
  bool digit = false;
  bool dot = false;
  uint32_t fract = 0;
  int fracExp = 0;
  uint8_t nd = 0;
  bool neg;
  int c;
  float v;
  const char* successPtr = str;

  if (ptr) {
    *ptr = const_cast<char*>(str);
  }

  while (isSpace((c = *str++))) {}
  neg = c == '-';
  if (c == '-' || c == '+') {
    c = *str++;
  }
  // Skip leading zeros
  while (c == '0') {
    c = *str++;
    digit = true;
  }
  for (;;) {
    if (isDigit(c)) {
      digit = true;
      if (nd < 9) {
        fract = 10*fract + c - '0';
        nd++;
        if (dot) {
          fracExp--;
        }
      } else {
        if (!dot) {
          fracExp++;
        }
      }
    } else if (c == '.') {
      if (dot) {
        goto fail;
      }
      dot = true;
    } else {
      if (!digit) {
        goto fail;
      }
      break;
    }
    successPtr = str;
    c = *str++;
  }
  if (c == 'e' || c == 'E') {
    int exp = 0;
    c = *str++;
    bool expNeg = c == '-';
    if (c == '-' || c == '+') {
      c = *str++;
    }
    while (isDigit(c)) {
      if (exp > EXP_LIMIT) {
        goto fail;
      }
      exp = 10*exp + c - '0';
      successPtr = str;
      c = *str++;
    }
    fracExp += expNeg ? -exp : exp;
  }
  if (ptr) {
    *ptr = const_cast<char*>(successPtr);
  }
  v = scale10(static_cast<float>(fract), fracExp);
  return neg ? -v : v;

fail:
  return 0;
}


