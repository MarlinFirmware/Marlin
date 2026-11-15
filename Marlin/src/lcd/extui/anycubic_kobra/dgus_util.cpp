/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

  #include "../../../core/types.h"
  #include "../../../inc/MarlinConfigPre.h"
  #include "../../../libs/numtostr.h"

char dgus_conv[16] = {0};

constexpr char DIGIT(const uint8_t n) { return '0' + n; }

template <typename T1, typename T2>
constexpr char DIGIMOD(const T1 n, const T2 f) {
  return DIGIT((n / f) % 10);
}

constexpr long INTFLOAT(const float V, const int N) {
  return long((V * 10.0f * pow(10.0f, N) + (V < 0.0f ? -5.0f : 5.0f)) / 10.0f);
}

char *dgus_ftostr(const float x) {
  long xx = INTFLOAT(x, 2);
  uint8_t index = 0;

  if (xx < 0) {
    xx = -xx;
    dgus_conv[index] = '-';
    index++;
  }
  if (xx >= 10000) {
    dgus_conv[index] = DIGIMOD(xx, 10000);
    index++;
    dgus_conv[index] = DIGIMOD(xx, 1000);
    index++;
    dgus_conv[index] = DIGIMOD(xx, 100);
    index++;
    dgus_conv[index] = '.';
    index++;
    dgus_conv[index] = DIGIMOD(xx, 10);
    index++;
    dgus_conv[index] = DIGIMOD(xx, 1);
    index++;
    dgus_conv[index] = '\0';
  } else if (xx >= 1000) {
    dgus_conv[index] = DIGIMOD(xx, 1000);
    index++;
    dgus_conv[index] = DIGIMOD(xx, 100);
    index++;
    dgus_conv[index] = '.';
    index++;
    dgus_conv[index] = DIGIMOD(xx, 10);
    index++;
    dgus_conv[index] = DIGIMOD(xx, 1);
    index++;
    dgus_conv[index] = '\0';
  } else if (xx >= 100) {
    dgus_conv[index] = DIGIMOD(xx, 100);
    index++;
    dgus_conv[index] = '.';
    index++;
    dgus_conv[index] = DIGIMOD(xx, 10);
    index++;
    dgus_conv[index] = DIGIMOD(xx, 1);
    index++;
    dgus_conv[index] = '\0';
  } else {
    dgus_conv[index] = '0';
    index++;
    dgus_conv[index] = '.';
    index++;
    dgus_conv[index] = DIGIMOD(xx, 10);
    index++;
    dgus_conv[index] = DIGIMOD(xx, 1);
    index++;
    dgus_conv[index] = '\0';
  }
  return dgus_conv;
}
