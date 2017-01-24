/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "Marlin.h"
#include "utility.h"
#include "temperature.h"

void safe_delay(millis_t ms) {
  while (ms > 50) {
    ms -= 50;
    delay(50);
    thermalManager.manage_heater();
  }
  delay(ms);
}

#if ENABLED(ULTRA_LCD)

  char conv[9];

  #define DIGIT(n) ('0' + (n))
  #define DIGIMOD(n, f) DIGIT((n)/(f) % 10)
  #define RJDIGIT(n, f) ((n) >= (f) ? DIGIMOD(n, f) : ' ')
  #define MINUSOR(n, alt) (n >= 0 ? (alt) : (n = -n, '-'))

  // Convert unsigned int to string with 12 format
  char* itostr2(const uint8_t& x) {
    int xx = x;
    conv[0] = DIGIMOD(xx, 10);
    conv[1] = DIGIMOD(xx, 1);
    conv[2] = '\0';
    return conv;
  }

  // Convert signed int to rj string with 123 or -12 format
  char* itostr3(const int& x) {
    int xx = x;
    conv[0] = MINUSOR(xx, RJDIGIT(xx, 100));
    conv[1] = RJDIGIT(xx, 10);
    conv[2] = DIGIMOD(xx, 1);
    conv[3] = '\0';
    return conv;
  }

  // Convert unsigned int to lj string with 123 format
  char* itostr3left(const int& xx) {
    char *str = &conv[3];
    *str = '\0';
    *(--str) = DIGIMOD(xx, 1);
    if (xx >= 10) {
      *(--str) = DIGIMOD(xx, 10);
      if (xx >= 100)
        *(--str) = DIGIMOD(xx, 100);
    }
    return str;
  }

  // Convert signed int to rj string with 1234, _123, -123, _-12, or __-1 format
  char *itostr4sign(const int& x) {
    int xx = abs(x);
    if (x >= 1000) {
      conv[0] = DIGIMOD(xx, 1000);
      conv[1] = DIGIMOD(xx, 100);
      conv[2] = DIGIMOD(xx, 10);
    }
    else {
      if (xx >= 100) {
        conv[0] = x < 0 ? '-' : ' ';
        conv[1] = DIGIMOD(xx, 100);
        conv[2] = DIGIMOD(xx, 10);
      }
      else {
        conv[0] = ' ';
        if (xx >= 10) {
          conv[1] = x < 0 ? '-' : ' ';
          conv[2] = DIGIMOD(xx, 10);
        }
        else {
          conv[1] = ' ';
          conv[2] = x < 0 ? '-' : ' ';
        }
      }
    }
    conv[3] = DIGIMOD(xx, 1);
    conv[4] = '\0';
    return conv;
  }

  // Convert unsigned float to string with 1.23 format
  char* ftostr12ns(const float& x) {
    long xx = abs(x * 100);
    conv[0] = DIGIMOD(xx, 100);
    conv[1] = '.';
    conv[2] = DIGIMOD(xx, 10);
    conv[3] = DIGIMOD(xx, 1);
    conv[4] = '\0';
    return conv;
  }

  // Convert signed float to fixed-length string with 023.45 / -23.45 format
  char *ftostr32(const float& x) {
    long xx = x * 100;
    conv[0] = MINUSOR(xx, DIGIMOD(xx, 10000));
    conv[1] = DIGIMOD(xx, 1000);
    conv[2] = DIGIMOD(xx, 100);
    conv[3] = '.';
    conv[4] = DIGIMOD(xx, 10);
    conv[5] = DIGIMOD(xx, 1);
    conv[6] = '\0';
    return conv;
  }

  #if ENABLED(LCD_DECIMAL_SMALL_XY)

    // Convert float to rj string with 1234, _123, -123, _-12, 12.3, _1.2, or -1.2 format
    char *ftostr4sign(const float& fx) {
      int x = fx * 10;
      if (x <= -100 || x >= 1000) return itostr4sign((int)fx);
      int xx = abs(x);
      conv[0] = x < 0 ? '-' : (xx >= 100 ? DIGIMOD(xx, 100) : ' ');
      conv[1] = DIGIMOD(xx, 10);
      conv[2] = '.';
      conv[3] = DIGIMOD(xx, 1);
      conv[4] = '\0';
      return conv;
    }

  #endif // LCD_DECIMAL_SMALL_XY

  // Convert float to fixed-length string with +123.4 / -123.4 format
  char* ftostr41sign(const float& x) {
    int xx = x * 10;
    conv[0] = MINUSOR(xx, '+');
    conv[1] = DIGIMOD(xx, 1000);
    conv[2] = DIGIMOD(xx, 100);
    conv[3] = DIGIMOD(xx, 10);
    conv[4] = '.';
    conv[5] = DIGIMOD(xx, 1);
    conv[6] = '\0';
    return conv;
  }

  // Convert signed float to string (6 digit) with -1.234 / _0.000 / +1.234 format
  char* ftostr43sign(const float& x, char plus/*=' '*/) {
    long xx = x * 1000;
    conv[0] = xx ? MINUSOR(xx, plus) : ' ';
    conv[1] = DIGIMOD(xx, 1000);
    conv[2] = '.';
    conv[3] = DIGIMOD(xx, 100);
    conv[4] = DIGIMOD(xx, 10);
    conv[5] = DIGIMOD(xx, 1);
    conv[6] = '\0';
    return conv;
  }

  // Convert unsigned float to rj string with 12345 format
  char* ftostr5rj(const float& x) {
    long xx = abs(x);
    conv[0] = RJDIGIT(xx, 10000);
    conv[1] = RJDIGIT(xx, 1000);
    conv[2] = RJDIGIT(xx, 100);
    conv[3] = RJDIGIT(xx, 10);
    conv[4] = DIGIMOD(xx, 1);
    conv[5] = '\0';
    return conv;
  }

  // Convert signed float to string with +1234.5 format
  char* ftostr51sign(const float& x) {
    long xx = x * 10;
    conv[0] = MINUSOR(xx, '+');
    conv[1] = DIGIMOD(xx, 10000);
    conv[2] = DIGIMOD(xx, 1000);
    conv[3] = DIGIMOD(xx, 100);
    conv[4] = DIGIMOD(xx, 10);
    conv[5] = '.';
    conv[6] = DIGIMOD(xx, 1);
    conv[7] = '\0';
    return conv;
  }

  // Convert signed float to string with +123.45 format
  char* ftostr52sign(const float& x) {
    long xx = x * 100;
    conv[0] = MINUSOR(xx, '+');
    conv[1] = DIGIMOD(xx, 10000);
    conv[2] = DIGIMOD(xx, 1000);
    conv[3] = DIGIMOD(xx, 100);
    conv[4] = '.';
    conv[5] = DIGIMOD(xx, 10);
    conv[6] = DIGIMOD(xx, 1);
    conv[7] = '\0';
    return conv;
  }

  // Convert signed float to string with +1234.56 format
  char* ftostr62sign(const float& x) {
    long xx = abs(x * 100);
    conv[0] = MINUSOR(xx, '+');
    conv[1] = DIGIMOD(xx, 100000);
    conv[2] = DIGIMOD(xx, 10000);
    conv[3] = DIGIMOD(xx, 1000);
    conv[4] = DIGIMOD(xx, 100);
    conv[5] = '.';
    conv[6] = DIGIMOD(xx, 10);
    conv[7] = DIGIMOD(xx, 1);
    conv[8] = '\0';
    return conv;
  }

  // Convert signed float to space-padded string with -_23.4_ format
  char* ftostr52sp(const float& x) {
    long xx = x * 100;
    uint8_t dig;
    conv[0] = MINUSOR(xx, RJDIGIT(xx, 10000));
    conv[1] = RJDIGIT(xx, 1000);
    conv[2] = DIGIMOD(xx, 100);

    if ((dig = xx % 10)) {          // second digit after decimal point?
      conv[3] = '.';
      conv[4] = DIGIMOD(xx, 10);
      conv[5] = DIGIT(dig);
    }
    else {
      if ((dig = (xx / 10) % 10)) { // first digit after decimal point?
        conv[3] = '.';
        conv[4] = DIGIT(dig);
      }
      else                          // nothing after decimal point
        conv[3] = conv[4] = ' ';
      conv[5] = ' ';
    }
    conv[6] = '\0';
    return conv;
  }

#endif // ULTRA_LCD
