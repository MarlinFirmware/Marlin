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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "numtostr.h"

#include "../inc/MarlinConfigPre.h"
#include "../core/utility.h"

char conv[8] = { 0 };

#define DIGIT(n) ('0' + (n))
#define DIGIMOD(n, f) DIGIT((n)/(f) % 10)
#define RJDIGIT(n, f) ((n) >= (f) ? DIGIMOD(n, f) : ' ')
#define MINUSOR(n, alt) (n >= 0 ? (alt) : (n = -n, '-'))

// Convert a full-range unsigned 8bit int to a percentage
const char* ui8tostr4pctrj(const uint8_t i) {
  const uint8_t n = ui8_to_percent(i);
  conv[3] = RJDIGIT(n, 100);
  conv[4] = RJDIGIT(n, 10);
  conv[5] = DIGIMOD(n, 1);
  conv[6] = '%';
  return &conv[3];
}

// Convert unsigned 8bit int to string 123 format
const char* ui8tostr3rj(const uint8_t i) {
  conv[4] = RJDIGIT(i, 100);
  conv[5] = RJDIGIT(i, 10);
  conv[6] = DIGIMOD(i, 1);
  return &conv[4];
}

// Convert signed 8bit int to rj string with 123 or -12 format
const char* i8tostr3rj(const int8_t x) {
  int xx = x;
  conv[4] = MINUSOR(xx, RJDIGIT(xx, 100));
  conv[5] = RJDIGIT(xx, 10);
  conv[6] = DIGIMOD(xx, 1);
  return &conv[4];
}

#if HAS_PRINT_PROGRESS_PERMYRIAD
  // Convert unsigned 16-bit permyriad to percent with 100 / 23 / 23.4 / 3.45 format
  const char* permyriadtostr4(const uint16_t xx) {
    if (xx >= 10000)
      return "100";
    else if (xx >= 1000) {
      conv[3] = DIGIMOD(xx, 1000);
      conv[4] = DIGIMOD(xx, 100);
      conv[5] = '.';
      conv[6] = DIGIMOD(xx, 10);
      return &conv[3];
    }
    else if (xx % 100 == 0) {
      conv[4] = ' ';
      conv[5] = RJDIGIT(xx, 1000);
      conv[6] = DIGIMOD(xx, 100);
      return &conv[4];
    }
    else {
      conv[3] = DIGIMOD(xx, 100);
      conv[4] = '.';
      conv[5] = DIGIMOD(xx, 10);
      conv[6] = RJDIGIT(xx, 1);
      return &conv[3];
    }
  }
#endif

// Convert unsigned 16bit int to string 12345 format
const char* ui16tostr5rj(const uint16_t xx) {
  conv[2] = RJDIGIT(xx, 10000);
  conv[3] = RJDIGIT(xx, 1000);
  conv[4] = RJDIGIT(xx, 100);
  conv[5] = RJDIGIT(xx, 10);
  conv[6] = DIGIMOD(xx, 1);
  return &conv[2];
}

// Convert unsigned 16bit int to string 1234 format
const char* ui16tostr4rj(const uint16_t xx) {
  conv[3] = RJDIGIT(xx, 1000);
  conv[4] = RJDIGIT(xx, 100);
  conv[5] = RJDIGIT(xx, 10);
  conv[6] = DIGIMOD(xx, 1);
  return &conv[3];
}

// Convert unsigned 16bit int to string 123 format
const char* ui16tostr3rj(const uint16_t xx) {
  conv[4] = RJDIGIT(xx, 100);
  conv[5] = RJDIGIT(xx, 10);
  conv[6] = DIGIMOD(xx, 1);
  return &conv[4];
}

// Convert signed 16bit int to rj string with 123 or -12 format
const char* i16tostr3rj(const int16_t x) {
  int xx = x;
  conv[4] = MINUSOR(xx, RJDIGIT(xx, 100));
  conv[5] = RJDIGIT(xx, 10);
  conv[6] = DIGIMOD(xx, 1);
  return &conv[4];
}

// Convert unsigned 16bit int to lj string with 123 format
const char* i16tostr3left(const int16_t i) {
  char *str = &conv[6];
  *str = DIGIMOD(i, 1);
  if (i >= 10) {
    *(--str) = DIGIMOD(i, 10);
    if (i >= 100)
      *(--str) = DIGIMOD(i, 100);
  }
  return str;
}

// Convert signed 16bit int to rj string with 1234, _123, -123, _-12, or __-1 format
const char* i16tostr4signrj(const int16_t i) {
  const bool neg = i < 0;
  const int ii = neg ? -i : i;
  if (i >= 1000) {
    conv[3] = DIGIMOD(ii, 1000);
    conv[4] = DIGIMOD(ii, 100);
    conv[5] = DIGIMOD(ii, 10);
  }
  else if (ii >= 100) {
    conv[3] = neg ? '-' : ' ';
    conv[4] = DIGIMOD(ii, 100);
    conv[5] = DIGIMOD(ii, 10);
  }
  else {
    conv[3] = ' ';
    conv[4] = ' ';
    if (ii >= 10) {
      conv[4] = neg ? '-' : ' ';
      conv[5] = DIGIMOD(ii, 10);
    }
    else {
      conv[5] = neg ? '-' : ' ';
    }
  }
  conv[6] = DIGIMOD(ii, 1);
  return &conv[3];
}

// Convert unsigned float to string with 1.23 format
const char* ftostr12ns(const float &f) {
  const long i = ((f < 0 ? -f : f) * 1000 + 5) / 10;
  conv[3] = DIGIMOD(i, 100);
  conv[4] = '.';
  conv[5] = DIGIMOD(i, 10);
  conv[6] = DIGIMOD(i, 1);
  return &conv[3];
}

// Convert signed float to fixed-length string with 12.34 / _2.34 / -2.34 or -23.45 / 123.45 format
const char* ftostr42_52(const float &f) {
  if (f <= -10 || f >= 100) return ftostr52(f); // -23.45 / 123.45
  long i = (f * 1000 + (f < 0 ? -5: 5)) / 10;
  conv[2] = (f >= 0 && f < 10) ? ' ' : MINUSOR(i, DIGIMOD(i, 1000));
  conv[3] = DIGIMOD(i, 100);
  conv[4] = '.';
  conv[5] = DIGIMOD(i, 10);
  conv[6] = DIGIMOD(i, 1);
  return &conv[2];
}

// Convert signed float to fixed-length string with 023.45 / -23.45 format
const char* ftostr52(const float &f) {
  long i = (f * 1000 + (f < 0 ? -5: 5)) / 10;
  conv[1] = MINUSOR(i, DIGIMOD(i, 10000));
  conv[2] = DIGIMOD(i, 1000);
  conv[3] = DIGIMOD(i, 100);
  conv[4] = '.';
  conv[5] = DIGIMOD(i, 10);
  conv[6] = DIGIMOD(i, 1);
  return &conv[1];
}

// Convert signed float to fixed-length string with 12.345 / _2.345 / -2.345 or -23.45 / 123.45 format
const char* ftostr53_63(const float &f) {
  if (f <= -10 || f >= 100) return ftostr63(f); // -23.456 / 123.456
  long i = (f * 10000 + (f < 0 ? -5: 5)) / 10;
  conv[1] = (f >= 0 && f < 10) ? ' ' : MINUSOR(i, DIGIMOD(i, 10000));
  conv[2] = DIGIMOD(i, 1000);
  conv[3] = '.';
  conv[4] = DIGIMOD(i, 100);
  conv[5] = DIGIMOD(i, 10);
  conv[6] = DIGIMOD(i, 1);
  return &conv[1];
}

// Convert signed float to fixed-length string with 023.456 / -23.456 format
const char* ftostr63(const float &f) {
  long i = (f * 10000 + (f < 0 ? -5: 5)) / 10;
  conv[0] = MINUSOR(i, DIGIMOD(i, 100000));
  conv[1] = DIGIMOD(i, 10000);
  conv[2] = DIGIMOD(i, 1000);
  conv[3] = '.';
  conv[4] = DIGIMOD(i, 100);
  conv[5] = DIGIMOD(i, 10);
  conv[6] = DIGIMOD(i, 1);
  return &conv[0];
}

#if ENABLED(LCD_DECIMAL_SMALL_XY)

  // Convert float to rj string with 1234, _123, -123, _-12, 12.3, _1.2, or -1.2 format
  const char* ftostr4sign(const float &f) {
    const int i = (f * 100 + (f < 0 ? -5: 5)) / 10;
    if (!WITHIN(i, -99, 999)) return i16tostr4signrj((int)f);
    const bool neg = i < 0;
    const int ii = neg ? -i : i;
    conv[3] = neg ? '-' : (ii >= 100 ? DIGIMOD(ii, 100) : ' ');
    conv[4] = DIGIMOD(ii, 10);
    conv[5] = '.';
    conv[6] = DIGIMOD(ii, 1);
    return &conv[3];
  }

#endif

// Convert float to fixed-length string with +123.4 / -123.4 format
const char* ftostr41sign(const float &f) {
  int i = (f * 100 + (f < 0 ? -5: 5)) / 10;
  conv[1] = MINUSOR(i, '+');
  conv[2] = DIGIMOD(i, 1000);
  conv[3] = DIGIMOD(i, 100);
  conv[4] = DIGIMOD(i, 10);
  conv[5] = '.';
  conv[6] = DIGIMOD(i, 1);
  return &conv[1];
}

// Convert signed float to string (6 digit) with -1.234 / _0.000 / +1.234 format
const char* ftostr43sign(const float &f, char plus/*=' '*/) {
  long i = (f * 10000 + (f < 0 ? -5: 5)) / 10;
  conv[1] = i ? MINUSOR(i, plus) : ' ';
  conv[2] = DIGIMOD(i, 1000);
  conv[3] = '.';
  conv[4] = DIGIMOD(i, 100);
  conv[5] = DIGIMOD(i, 10);
  conv[6] = DIGIMOD(i, 1);
  return &conv[1];
}

// Convert signed float to string (5 digit) with -1.2345 / _0.0000 / +1.2345 format
const char* ftostr54sign(const float &f, char plus/*=' '*/) {
  long i = (f * 100000 + (f < 0 ? -5: 5)) / 10;
  conv[0] = i ? MINUSOR(i, plus) : ' ';
  conv[1] = DIGIMOD(i, 10000);
  conv[2] = '.';
  conv[3] = DIGIMOD(i, 1000);
  conv[4] = DIGIMOD(i, 100);
  conv[5] = DIGIMOD(i, 10);
  conv[6] = DIGIMOD(i, 1);
  return &conv[0];
}

// Convert unsigned float to rj string with 12345 format
const char* ftostr5rj(const float &f) {
  const long i = ((f < 0 ? -f : f) * 10 + 5) / 10;
  return ui16tostr5rj(i);
}

// Convert signed float to string with +1234.5 format
const char* ftostr51sign(const float &f) {
  long i = (f * 100 + (f < 0 ? -5: 5)) / 10;
  conv[0] = MINUSOR(i, '+');
  conv[1] = DIGIMOD(i, 10000);
  conv[2] = DIGIMOD(i, 1000);
  conv[3] = DIGIMOD(i, 100);
  conv[4] = DIGIMOD(i, 10);
  conv[5] = '.';
  conv[6] = DIGIMOD(i, 1);
  return conv;
}

// Convert signed float to string with +123.45 format
const char* ftostr52sign(const float &f) {
  long i = (f * 1000 + (f < 0 ? -5: 5)) / 10;
  conv[0] = MINUSOR(i, '+');
  conv[1] = DIGIMOD(i, 10000);
  conv[2] = DIGIMOD(i, 1000);
  conv[3] = DIGIMOD(i, 100);
  conv[4] = '.';
  conv[5] = DIGIMOD(i, 10);
  conv[6] = DIGIMOD(i, 1);
  return conv;
}

// Convert signed float to string with +12.345 format
const char* ftostr53sign(const float &f) {
  long i = (f * 1000 + (f < 0 ? -5: 5)) / 10;
  conv[0] = MINUSOR(i, '+');
  conv[1] = DIGIMOD(i, 10000);
  conv[2] = DIGIMOD(i, 1000);
  conv[3] = '.';
  conv[4] = DIGIMOD(i, 100);
  conv[5] = DIGIMOD(i, 10);
  conv[6] = DIGIMOD(i, 1);
  return conv;
}

// Convert unsigned float to string with ____4.5, __34.5, _234.5, 1234.5 format
const char* ftostr51rj(const float &f) {
  const long i = ((f < 0 ? -f : f) * 100 + 5) / 10;
  conv[0] = ' ';
  conv[1] = RJDIGIT(i, 10000);
  conv[2] = RJDIGIT(i, 1000);
  conv[3] = RJDIGIT(i, 100);
  conv[4] = DIGIMOD(i, 10);
  conv[5] = '.';
  conv[6] = DIGIMOD(i, 1);
  return conv;
}

// Convert signed float to space-padded string with -_23.4_ format
const char* ftostr52sp(const float &f) {
  long i = (f * 1000 + (f < 0 ? -5: 5)) / 10;
  uint8_t dig;
  conv[0] = MINUSOR(i, ' ');
  conv[1] = RJDIGIT(i, 10000);
  conv[2] = RJDIGIT(i, 1000);
  conv[3] = DIGIMOD(i, 100);

  if ((dig = i % 10)) {          // second digit after decimal point?
    conv[4] = '.';
    conv[5] = DIGIMOD(i, 10);
    conv[6] = DIGIT(dig);
  }
  else {
    if ((dig = (i / 10) % 10)) { // first digit after decimal point?
      conv[4] = '.';
      conv[5] = DIGIT(dig);
    }
    else                          // nothing after decimal point
      conv[4] = conv[5] = ' ';
    conv[6] = ' ';
  }
  return conv;
}
