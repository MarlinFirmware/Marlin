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

#include "../../platforms.h"

#include "../../../inc/MarlinConfig.h"

#if HAS_TFT_XPT2046 || HAS_RES_TOUCH_BUTTONS

#include "xpt2046.h"

#ifndef TOUCH_BAUDRATE
  // Default.
  #define TOUCH_BAUDRATE 0
#endif

void XPT2046::Init() {
  OUT_WRITE(TOUCH_CS_PIN, HIGH);

#if 0
  // DEBUG PIN.
  while (true) {
    OUT_WRITE(EXP1_01_PIN, HIGH);
    delay(1000);
    OUT_WRITE(EXP1_01_PIN, LOW);
    
    // HIGH
    OUT_WRITE(TOUCH_CS_PIN, HIGH);
    delay(10000);

    OUT_WRITE(EXP1_01_PIN, HIGH);
    delay(500);
    OUT_WRITE(EXP1_01_PIN, LOW);
    delay(500);
    OUT_WRITE(EXP1_01_PIN, HIGH);
    delay(500);
    OUT_WRITE(EXP1_01_PIN, LOW);

    // LOW
    OUT_WRITE(TOUCH_CS_PIN, LOW);
    delay(10000);
  }
#endif

  #if PIN_EXISTS(TOUCH_INT)
    // Optional Pendrive interrupt pin
    SET_INPUT(TOUCH_INT_PIN);
  #endif

  getRawData(XPT2046_Z1);
}

void XPT2046::DataTransferBegin(void) {
  spiInitEx(TOUCH_BAUDRATE, TOUCH_SCK_PIN, TOUCH_MISO_PIN, TOUCH_MOSI_PIN, TOUCH_CS_PIN);
  spiSetClockMode(SPI_CLKMODE_3);
}

void XPT2046::DataTransferEnd(void) {
  spiClose();
}

bool XPT2046::isTouched() {
  return isBusy() ? false : (
    #if PIN_EXISTS(TOUCH_INT)
      READ(TOUCH_INT_PIN) != HIGH
    #else
      getRawData(XPT2046_Z1) >= XPT2046_Z1_THRESHOLD
    #endif
  );
}

bool XPT2046::getRawPoint(int16_t *x, int16_t *y) {
  if (isBusy()) return false;
  if (!isTouched()) return false;
  *x = getRawData(XPT2046_X);
  *y = getRawData(XPT2046_Y);
  return isTouched();
}

inline uint16_t _delta(uint16_t a, uint16_t b) { return a > b ? a - b : b - a; }

uint16_t XPT2046::getRawData(const XPTCoordinate coordinate) {
  uint16_t data[3];

  DataTransferBegin();

  // Perform a burst-read of 3 values of the coordinate, then
  // send a good approximation.

  for (int i = 0; i < 3; i++) {
    spiSend(coordinate);
    uint16_t res = (uint16_t)spiRec(0) << 4;
    res |= (uint16_t)spiRec(0) >> 4;
    data[i] = res;
  }

  DataTransferEnd();

  uint16_t delta01 = _delta(data[0], data[1]);
  uint16_t delta02 = _delta(data[0], data[2]);
  uint16_t delta12 = _delta(data[1], data[2]);

  if (delta01 > delta02 || delta01 > delta12) {
    if (delta02 > delta12)
      data[0] = data[2];
    else
      data[1] = data[2];
  }

  return (data[0] + data[1]) >> 1;
}

#endif // HAS_TFT_XPT2046