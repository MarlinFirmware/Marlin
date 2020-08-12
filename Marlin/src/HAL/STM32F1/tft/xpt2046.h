/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_BUTTONS_HW_SPI)
  #include <SPI.h>
#endif

#if !PIN_EXISTS(TOUCH_MISO)
  #error "TOUCH_MISO_PIN is not defined."
#elif !PIN_EXISTS(TOUCH_MOSI)
  #error "TOUCH_MOSI_PIN is not defined."
#elif !PIN_EXISTS(TOUCH_SCK)
  #error "TOUCH_SCK_PIN is not defined."
#elif !PIN_EXISTS(TOUCH_CS)
  #error "TOUCH_CS_PIN is not defined."
#endif

#ifndef TOUCH_INT_PIN
  #define TOUCH_INT_PIN  -1
#endif

#define XPT2046_DFR_MODE        0x00
#define XPT2046_SER_MODE        0x04
#define XPT2046_CONTROL         0x80

enum XPTCoordinate : uint8_t {
  XPT2046_X  = 0x10 | XPT2046_CONTROL | XPT2046_DFR_MODE,
  XPT2046_Y  = 0x50 | XPT2046_CONTROL | XPT2046_DFR_MODE,
  XPT2046_Z1 = 0x30 | XPT2046_CONTROL | XPT2046_DFR_MODE,
  XPT2046_Z2 = 0x40 | XPT2046_CONTROL | XPT2046_DFR_MODE,
};

#if !defined(XPT2046_Z1_THRESHOLD)
  #define XPT2046_Z1_THRESHOLD 10
#endif

class XPT2046 {
private:
  static bool isBusy() { return false; }

  static uint16_t getRawData(const XPTCoordinate coordinate);
  static bool isTouched();

  static inline void DataTransferBegin() { WRITE(TOUCH_CS_PIN, LOW); };
  static inline void DataTransferEnd() { WRITE(TOUCH_CS_PIN, HIGH); };
  #if ENABLED(TOUCH_BUTTONS_HW_SPI)
    static uint16_t HardwareIO(uint16_t data);
  #endif
  static uint16_t SoftwareIO(uint16_t data);
  static uint16_t IO(uint16_t data = 0);

public:
  #if ENABLED(TOUCH_BUTTONS_HW_SPI)
    static SPIClass SPIx;
  #endif

  static void Init();
  static bool getRawPoint(int16_t *x, int16_t *y);
};
