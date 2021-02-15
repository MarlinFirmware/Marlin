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
#include <string.h>
#include "ostream.h"
#ifndef PSTR
#define PSTR(x) x
#endif
//------------------------------------------------------------------------------
void ostream::do_fill(unsigned len) {
  for (; len < width(); len++) {
    putch(fill());
  }
  width(0);
}
//------------------------------------------------------------------------------
void ostream::fill_not_left(unsigned len) {
  if ((flags() & adjustfield) != left) {
    do_fill(len);
  }
}
//------------------------------------------------------------------------------
char* ostream::fmtNum(uint32_t n, char *ptr, uint8_t base) {
  char a = flags() & uppercase ? 'A' - 10 : 'a' - 10;
  do {
    uint32_t m = n;
    n /= base;
    char c = m - base * n;
    *--ptr = c < 10 ? c + '0' : c + a;
  } while (n);
  return ptr;
}
//------------------------------------------------------------------------------
void ostream::putBool(bool b) {
  if (flags() & boolalpha) {
    if (b) {
      putPgm(PSTR("true"));
    } else {
      putPgm(PSTR("false"));
    }
  } else {
    putChar(b ? '1' : '0');
  }
}
//------------------------------------------------------------------------------
void ostream::putChar(char c) {
  fill_not_left(1);
  putch(c);
  do_fill(1);
}
//------------------------------------------------------------------------------
void ostream::putDouble(double n) {
  uint8_t nd = precision();
  double round = 0.5;
  char sign;
  char buf[13];  // room for sign, 10 digits, '.', and zero byte
  char *end = buf + sizeof(buf) - 1;
  char *str = end;
  // terminate string
  *end = '\0';

  // get sign and make nonnegative
  if (n < 0.0) {
    sign = '-';
    n = -n;
  } else {
    sign = flags() & showpos ? '+' : '\0';
  }
  // check for larger than uint32_t
  if (n > 4.0E9) {
    putPgm(PSTR("BIG FLT"));
    return;
  }
  // round up and separate int and fraction parts
  for (uint8_t i = 0; i < nd; ++i) {
    round *= 0.1;
  }
  n += round;
  uint32_t intPart = n;
  double fractionPart = n - intPart;

  // format intPart and decimal point
  if (nd || (flags() & showpoint)) {
    *--str = '.';
  }
  str = fmtNum(intPart, str, 10);

  // calculate length for fill
  uint8_t len = sign ? 1 : 0;
  len += nd + end - str;

  // extract adjust field
  fmtflags adj = flags() & adjustfield;
  if (adj == internal) {
    if (sign) {
      putch(sign);
    }
    do_fill(len);
  } else {
    // do fill for internal or right
    fill_not_left(len);
    if (sign) {
      *--str = sign;
    }
  }
  putstr(str);
  // output fraction
  while (nd-- > 0) {
    fractionPart *= 10.0;
    int digit = static_cast<int>(fractionPart);
    putch(digit + '0');
    fractionPart -= digit;
  }
  // do fill if not done above
  do_fill(len);
}
//------------------------------------------------------------------------------
void ostream::putNum(int32_t n) {
  bool neg = n < 0 && flagsToBase() == 10;
  if (neg) {
    n = -n;
  }
  putNum(n, neg);
}
//------------------------------------------------------------------------------
void ostream::putNum(uint32_t n, bool neg) {
  char buf[13];
  char* end = buf + sizeof(buf) - 1;
  char* num;
  char* str;
  uint8_t base = flagsToBase();
  *end = '\0';
  str = num = fmtNum(n, end, base);
  if (base == 10) {
    if (neg) {
      *--str = '-';
    } else if (flags() & showpos) {
      *--str = '+';
    }
  } else if (flags() & showbase) {
    if (flags() & hex) {
      *--str = flags() & uppercase ? 'X' : 'x';
    }
    *--str = '0';
  }
  uint8_t len = end - str;
  fmtflags adj = flags() & adjustfield;
  if (adj == internal) {
    while (str < num) {
      putch(*str++);
    }
  }
  if (adj != left) {
    do_fill(len);
  }
  putstr(str);
  do_fill(len);
}
//------------------------------------------------------------------------------
void ostream::putPgm(const char* str) {
  int n;
  for (n = 0; pgm_read_byte(&str[n]); n++) {}
  fill_not_left(n);
  for (uint8_t c; (c = pgm_read_byte(str)); str++) {
    putch(c);
  }
  do_fill(n);
}
//------------------------------------------------------------------------------
void ostream::putStr(const char *str) {
  unsigned n = strlen(str);
  fill_not_left(n);
  putstr(str);
  do_fill(n);
}
