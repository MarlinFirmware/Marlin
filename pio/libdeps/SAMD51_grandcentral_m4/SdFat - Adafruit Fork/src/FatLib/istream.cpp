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
//  #include <ctype.h>
#include <float.h>
#include <ctype.h>
#include "istream.h"
//------------------------------------------------------------------------------
int istream::get() {
  int c;
  m_gcount = 0;
  c = getch();
  if (c < 0) {
    setstate(failbit);
  } else {
    m_gcount = 1;
  }
  return c;
}
//------------------------------------------------------------------------------
istream& istream::get(char& c) {
  int tmp = get();
  if (tmp >= 0) {
    c = tmp;
  }
  return *this;
}
//------------------------------------------------------------------------------
istream& istream::get(char *str, streamsize n, char delim) {
  int c;
  FatPos_t pos;
  m_gcount = 0;
  while ((m_gcount + 1)  < n) {
    c = getch(&pos);
    if (c < 0) {
      break;
    }
    if (c == delim) {
      setpos(&pos);
      break;
    }
    str[m_gcount++] = c;
  }
  if (n > 0) {
    str[m_gcount] = '\0';
  }
  if (m_gcount == 0) {
    setstate(failbit);
  }
  return *this;
}
//------------------------------------------------------------------------------
void istream::getBool(bool *b) {
  if ((flags() & boolalpha) == 0) {
    getNumber(b);
    return;
  }
#ifdef __AVR__
  PGM_P truePtr = PSTR("true");
  PGM_P falsePtr = PSTR("false");
#else  // __AVR__
  const char* truePtr = "true";
  const char* falsePtr = "false";
#endif  // __AVR
  const uint8_t true_len = 4;
  const uint8_t false_len = 5;
  bool trueOk = true;
  bool falseOk = true;
  uint8_t i = 0;
  int c = readSkip();
  while (1) {
#ifdef __AVR__
    falseOk = falseOk && c == pgm_read_byte(falsePtr + i);
    trueOk = trueOk && c == pgm_read_byte(truePtr + i);
#else  // __AVR__
    falseOk = falseOk && c == falsePtr[i];
    trueOk = trueOk && c == truePtr[i];
#endif  // __AVR__
    if (trueOk == false && falseOk == false) {
      break;
    }
    i++;
    if (trueOk && i == true_len) {
      *b = true;
      return;
    }
    if (falseOk && i == false_len) {
      *b = false;
      return;
    }
    c = getch();
  }
  setstate(failbit);
}
//------------------------------------------------------------------------------
void istream::getChar(char* ch) {
  int16_t c = readSkip();
  if (c < 0) {
    setstate(failbit);
  } else {
    *ch = c;
  }
}
//------------------------------------------------------------------------------
//
// http://www.exploringbinary.com/category/numbers-in-computers/
//
int16_t const EXP_LIMIT = 100;
static const uint32_t uint32_max = (uint32_t)-1;
bool istream::getDouble(double* value) {
  bool got_digit = false;
  bool got_dot = false;
  bool neg;
  int16_t c;
  bool expNeg = false;
  int16_t exp = 0;
  int16_t fracExp = 0;
  uint32_t frac = 0;
  FatPos_t endPos;
  double pow10;
  double v;

  getpos(&endPos);
  c = readSkip();
  neg = c == '-';
  if (c == '-' || c == '+') {
    c = getch();
  }
  while (1) {
    if (isdigit(c)) {
      got_digit = true;
      if (frac < uint32_max/10) {
        frac = frac * 10 + (c  - '0');
        if (got_dot) {
          fracExp--;
        }
      } else {
        if (!got_dot) {
          fracExp++;
        }
      }
    } else if (!got_dot && c == '.') {
      got_dot = true;
    } else {
      break;
    }
    if (fracExp < -EXP_LIMIT || fracExp > EXP_LIMIT) {
      goto fail;
    }
    c = getch(&endPos);
  }
  if (!got_digit) {
    goto fail;
  }
  if (c == 'e' || c == 'E') {
    c = getch();
    expNeg = c == '-';
    if (c == '-' || c == '+') {
      c = getch();
    }
    while (isdigit(c)) {
      if (exp > EXP_LIMIT) {
        goto fail;
      }
      exp = exp * 10 + (c - '0');
      c = getch(&endPos);
    }
  }
  v = static_cast<double>(frac);
  exp = expNeg ? fracExp - exp : fracExp + exp;
  expNeg = exp < 0;
  if (expNeg) {
    exp = -exp;
  }
  pow10 = 10.0;
  while (exp) {
    if (exp & 1) {
      if (expNeg) {
        // check for underflow
        if (v < FLT_MIN * pow10  && frac != 0) {
          goto fail;
        }
        v /= pow10;
      } else {
        // check for overflow
        if (v > FLT_MAX / pow10) {
          goto fail;
        }
        v *= pow10;
      }
    }
    pow10 *= pow10;
    exp >>= 1;
  }
  setpos(&endPos);
  *value = neg ? -v : v;
  return true;

fail:
  // error restore position to last good place
  setpos(&endPos);
  setstate(failbit);
  return false;
}
//------------------------------------------------------------------------------

istream& istream::getline(char *str, streamsize n, char delim) {
  FatPos_t pos;
  int c;
  m_gcount = 0;
  if (n > 0) {
    str[0] = '\0';
  }
  while (1) {
    c = getch(&pos);
    if (c < 0) {
      break;
    }
    if (c == delim) {
      m_gcount++;
      break;
    }
    if ((m_gcount + 1)  >=  n) {
      setpos(&pos);
      setstate(failbit);
      break;
    }
    str[m_gcount++] = c;
    str[m_gcount] = '\0';
  }
  if (m_gcount == 0) {
    setstate(failbit);
  }
  return *this;
}
//------------------------------------------------------------------------------
bool istream::getNumber(uint32_t posMax, uint32_t negMax, uint32_t* num) {
  int16_t c;
  int8_t any = 0;
  int8_t have_zero = 0;
  uint8_t neg;
  uint32_t val = 0;
  uint32_t cutoff;
  uint8_t cutlim;
  FatPos_t endPos;
  uint8_t f = flags() & basefield;
  uint8_t base = f == oct ? 8 : f != hex ? 10 : 16;
  getpos(&endPos);
  c = readSkip();

  neg = c == '-' ? 1 : 0;
  if (c == '-' || c == '+') {
    c = getch();
  }

  if (base == 16 && c == '0') {  // TESTSUITE
    c = getch(&endPos);
    if (c == 'X' || c == 'x') {
      c = getch();
      // remember zero in case no hex digits follow x/X
      have_zero = 1;
    } else {
      any = 1;
    }
  }
  // set values for overflow test
  cutoff = neg ? negMax : posMax;
  cutlim = cutoff % base;
  cutoff /= base;

  while (1) {
    if (isdigit(c)) {
      c -= '0';
    } else if (isalpha(c)) {
      c -= isupper(c) ? 'A' - 10 : 'a' - 10;
    } else {
      break;
    }
    if (c >= base) {
      break;
    }
    if (val > cutoff || (val == cutoff && c > cutlim)) {
      // indicate overflow error
      any = -1;
      break;
    }
    val = val * base + c;
    c = getch(&endPos);
    any = 1;
  }
  setpos(&endPos);
  if (any > 0 || (have_zero && any >= 0)) {
    *num =  neg ? -val : val;
    return true;
  }
  setstate(failbit);
  return false;
}
//------------------------------------------------------------------------------
void istream::getStr(char *str) {
  FatPos_t pos;
  uint16_t i = 0;
  uint16_t m = width() ? width() - 1 : 0XFFFE;
  if (m != 0) {
    getpos(&pos);
    int c = readSkip();

    while (i < m) {
      if (c < 0) {
        break;
      }
      if (isspace(c)) {
        setpos(&pos);
        break;
      }
      str[i++] = c;
      c = getch(&pos);
    }
  }
  str[i] = '\0';
  if (i == 0) {
    setstate(failbit);
  }
  width(0);
}
//------------------------------------------------------------------------------
istream& istream::ignore(streamsize n, int delim) {
  int c;
  m_gcount = 0;
  while (m_gcount < n) {
    c = getch();
    if (c < 0) {
      break;
    }
    m_gcount++;
    if (c == delim) {
      break;
    }
  }
  return *this;
}
//------------------------------------------------------------------------------
int istream::peek() {
  int16_t c;
  FatPos_t pos;
  m_gcount = 0;
  getpos(&pos);
  c = getch();
  if (c < 0) {
    if (!bad()) {
      setstate(eofbit);
    }
  } else {
    setpos(&pos);
  }
  return c;
}
//------------------------------------------------------------------------------
int16_t istream::readSkip() {
  int16_t c;
  do {
    c = getch();
  } while (isspace(c) && (flags() & skipws));
  return c;
}
//------------------------------------------------------------------------------
/** used to implement ws() */
void istream::skipWhite() {
  int c;
  FatPos_t pos;
  do {
    c = getch(&pos);
  } while (isspace(c));
  setpos(&pos);
}
