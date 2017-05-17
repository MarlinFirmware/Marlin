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
  thermalManager.manage_heater(); // This keeps us safe if too many small safe_delay() calls are made
}

#if ENABLED(EEPROM_SETTINGS)

  void crc16(uint16_t *crc, const void * const data, uint16_t cnt) {
    uint8_t *ptr = (uint8_t*)data;
    while (cnt-- > 0) {
      *crc = (uint16_t)(*crc ^ (uint16_t)(((uint16_t)*ptr++) << 8));
      for (uint8_t x = 0; x < 8; x++)
        *crc = (uint16_t)((*crc & 0x8000) ? ((uint16_t)(*crc << 1) ^ 0x1021) : (*crc << 1));
    }
  }

#endif // EEPROM_SETTINGS

#if ENABLED(ULTRA_LCD)

  char conv[8] = { 0 };

  #define DIGIT(n) ('0' + (n))
  #define DIGIMOD(n, f) DIGIT((n)/(f) % 10)
  #define RJDIGIT(n, f) ((n) >= (f) ? DIGIMOD(n, f) : ' ')
  #define MINUSOR(n, alt) (n >= 0 ? (alt) : (n = -n, '-'))

  // Convert unsigned int to string with 12 format
  char* itostr2(const uint8_t& xx) {
    conv[5] = DIGIMOD(xx, 10);
    conv[6] = DIGIMOD(xx, 1);
    return &conv[5];
  }

  // Convert signed int to rj string with 123 or -12 format
  char* itostr3(const int& x) {
    int xx = x;
    conv[4] = MINUSOR(xx, RJDIGIT(xx, 100));
    conv[5] = RJDIGIT(xx, 10);
    conv[6] = DIGIMOD(xx, 1);
    return &conv[4];
  }

  // Convert unsigned int to lj string with 123 format
  char* itostr3left(const int& xx) {
    char *str = &conv[6];
    *str = DIGIMOD(xx, 1);
    if (xx >= 10) {
      *(--str) = DIGIMOD(xx, 10);
      if (xx >= 100)
        *(--str) = DIGIMOD(xx, 100);
    }
    return str;
  }

  // Convert signed int to rj string with 1234, _123, -123, _-12, or __-1 format
  char *itostr4sign(const int& x) {
    const bool neg = x < 0;
    const int xx = neg ? -x : x;
    if (x >= 1000) {
      conv[3] = DIGIMOD(xx, 1000);
      conv[4] = DIGIMOD(xx, 100);
      conv[5] = DIGIMOD(xx, 10);
    }
    else {
      if (xx >= 100) {
        conv[3] = neg ? '-' : ' ';
        conv[4] = DIGIMOD(xx, 100);
        conv[5] = DIGIMOD(xx, 10);
      }
      else {
        conv[3] = ' ';
        conv[4] = ' ';
        if (xx >= 10) {
          conv[4] = neg ? '-' : ' ';
          conv[5] = DIGIMOD(xx, 10);
        }
        else {
          conv[5] = neg ? '-' : ' ';
        }
      }
    }
    conv[6] = DIGIMOD(xx, 1);
    return &conv[3];
  }

  // Convert unsigned float to string with 1.23 format
  char* ftostr12ns(const float& x) {
    const long xx = (x < 0 ? -x : x) * 100;
    conv[3] = DIGIMOD(xx, 100);
    conv[4] = '.';
    conv[5] = DIGIMOD(xx, 10);
    conv[6] = DIGIMOD(xx, 1);
    return &conv[3];
  }

  // Convert signed float to fixed-length string with 023.45 / -23.45 format
  char *ftostr32(const float& x) {
    long xx = x * 100;
    conv[1] = MINUSOR(xx, DIGIMOD(xx, 10000));
    conv[2] = DIGIMOD(xx, 1000);
    conv[3] = DIGIMOD(xx, 100);
    conv[4] = '.';
    conv[5] = DIGIMOD(xx, 10);
    conv[6] = DIGIMOD(xx, 1);
    return &conv[1];
  }

  #if ENABLED(LCD_DECIMAL_SMALL_XY)

    // Convert float to rj string with 1234, _123, -123, _-12, 12.3, _1.2, or -1.2 format
    char *ftostr4sign(const float& fx) {
      const int x = fx * 10;
      if (!WITHIN(x, -99, 999)) return itostr4sign((int)fx);
      const bool neg = x < 0;
      const int xx = neg ? -x : x;
      conv[3] = neg ? '-' : (xx >= 100 ? DIGIMOD(xx, 100) : ' ');
      conv[4] = DIGIMOD(xx, 10);
      conv[5] = '.';
      conv[6] = DIGIMOD(xx, 1);
      return &conv[3];
    }

  #endif // LCD_DECIMAL_SMALL_XY

  // Convert float to fixed-length string with +123.4 / -123.4 format
  char* ftostr41sign(const float& x) {
    int xx = x * 10;
    conv[1] = MINUSOR(xx, '+');
    conv[2] = DIGIMOD(xx, 1000);
    conv[3] = DIGIMOD(xx, 100);
    conv[4] = DIGIMOD(xx, 10);
    conv[5] = '.';
    conv[6] = DIGIMOD(xx, 1);
    return &conv[1];
  }

  // Convert signed float to string (6 digit) with -1.234 / _0.000 / +1.234 format
  char* ftostr43sign(const float& x, char plus/*=' '*/) {
    long xx = x * 1000;
    conv[1] = xx ? MINUSOR(xx, plus) : ' ';
    conv[2] = DIGIMOD(xx, 1000);
    conv[3] = '.';
    conv[4] = DIGIMOD(xx, 100);
    conv[5] = DIGIMOD(xx, 10);
    conv[6] = DIGIMOD(xx, 1);
    return &conv[1];
  }

  // Convert unsigned float to rj string with 12345 format
  char* ftostr5rj(const float& x) {
    const long xx = x < 0 ? -x : x;
    conv[2] = RJDIGIT(xx, 10000);
    conv[3] = RJDIGIT(xx, 1000);
    conv[4] = RJDIGIT(xx, 100);
    conv[5] = RJDIGIT(xx, 10);
    conv[6] = DIGIMOD(xx, 1);
    return &conv[2];
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
    return conv;
  }

  // Convert unsigned float to string with 1234.56 format omitting trailing zeros
  char* ftostr62rj(const float& x) {
    const long xx = (x < 0 ? -x : x) * 100;
    conv[0] = RJDIGIT(xx, 100000);
    conv[1] = RJDIGIT(xx, 10000);
    conv[2] = RJDIGIT(xx, 1000);
    conv[3] = DIGIMOD(xx, 100);
    conv[4] = '.';
    conv[5] = DIGIMOD(xx, 10);
    conv[6] = DIGIMOD(xx, 1);
    return conv;
  }

  // Convert signed float to space-padded string with -_23.4_ format
  char* ftostr52sp(const float& x) {
    long xx = x * 100;
    uint8_t dig;
    conv[1] = MINUSOR(xx, RJDIGIT(xx, 10000));
    conv[2] = RJDIGIT(xx, 1000);
    conv[3] = DIGIMOD(xx, 100);

    if ((dig = xx % 10)) {          // second digit after decimal point?
      conv[4] = '.';
      conv[5] = DIGIMOD(xx, 10);
      conv[6] = DIGIT(dig);
    }
    else {
      if ((dig = (xx / 10) % 10)) { // first digit after decimal point?
        conv[4] = '.';
        conv[5] = DIGIT(dig);
      }
      else                          // nothing after decimal point
        conv[4] = conv[5] = ' ';
      conv[6] = ' ';
    }
    return &conv[1];
  }

#endif // ULTRA_LCD
