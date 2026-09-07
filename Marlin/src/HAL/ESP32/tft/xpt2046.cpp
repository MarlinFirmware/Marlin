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

#include "../../platforms.h"

#ifdef ARDUINO_ARCH_ESP32

#include "../../../inc/MarlinConfig.h"

#if HAS_TFT_XPT2046 || HAS_RES_TOUCH_BUTTONS

#if HAS_SPI_TFT
  #include "tft_spi.h"
#endif

#include "xpt2046.h"

/**
 * The touch controller sits on the same bus as the display, so it gets its own
 * SPIClass on the same peripheral. Each exchange opens a transaction at the
 * touch clock and closes it again, leaving the display free to use its own
 * speed. Only one of the two is ever mid-transaction.
 */
#if ENABLED(TOUCH_BUTTONS_HW_SPI)
  #ifndef TOUCH_SPI_DEVICE
    #define TOUCH_SPI_DEVICE VSPI
  #endif
  #if HAS_SPI_TFT
    /**
     * The display already drives this peripheral. A second SPIClass on the same
     * bus would re-initialise it and leave the display writing into a bus that
     * had been reconfigured, so share the one instance.
     */
    SPIClass &XPT2046::SPIx = TFT_SPI::SPIx;
  #else
    static SPIClass touchSPI(TOUCH_SPI_DEVICE);
    SPIClass &XPT2046::SPIx = touchSPI;
  #endif
#endif

static uint16_t delta(const uint16_t a, const uint16_t b) { return a > b ? a - b : b - a; }

void XPT2046::init() {
  SET_INPUT(TOUCH_MISO_PIN);
  SET_OUTPUT(TOUCH_MOSI_PIN);
  SET_OUTPUT(TOUCH_SCK_PIN);
  OUT_WRITE(TOUCH_CS_PIN, HIGH);

  #if PIN_EXISTS(TOUCH_INT)
    SET_INPUT(TOUCH_INT_PIN);   // Optional pen-down interrupt pin
  #endif

  #if ENABLED(TOUCH_BUTTONS_HW_SPI)
    SPIx.begin(TOUCH_SCK_PIN, TOUCH_MISO_PIN, TOUCH_MOSI_PIN, TOUCH_CS_PIN);
    SPIx.setHwCs(false);
  #endif

  // Read once to enable the pen-down status pin
  getRawData(XPT2046_X);
}

void XPT2046::dataTransferBegin() {
  #if HAS_SPI_TFT
    /**
     * Touch is polled from inside the display's draw path, so the display may
     * hold an open transfer on this shared SPIClass. beginTransaction() takes a
     * non-recursive lock with no timeout, so entering it twice hangs the board.
     * Close the display's transfer first; it reopens on its next write.
     */
    TFT_SPI::dataTransferEnd();
  #endif
  TERN_(TOUCH_BUTTONS_HW_SPI, SPIx.beginTransaction(SPISettings(SPI_CLOCK_MAX_TOUCH, MSBFIRST, SPI_MODE0)));
  WRITE(TOUCH_CS_PIN, LOW);
}

void XPT2046::dataTransferEnd() {
  WRITE(TOUCH_CS_PIN, HIGH);
  TERN_(TOUCH_BUTTONS_HW_SPI, SPIx.endTransaction());
}

#if ENABLED(TOUCH_BUTTONS_HW_SPI)
  uint16_t XPT2046::hardwareIO(uint16_t data) { return SPIx.transfer(uint8_t(data)); }
#endif

uint16_t XPT2046::softwareIO(uint16_t data) {
  uint16_t result = 0;
  for (uint8_t j = 0x80; j; j >>= 1) {
    WRITE(TOUCH_SCK_PIN, LOW);
    WRITE(TOUCH_MOSI_PIN, bool(data & j));
    if (READ(TOUCH_MISO_PIN)) result |= j;
    WRITE(TOUCH_SCK_PIN, HIGH);
  }
  WRITE(TOUCH_SCK_PIN, LOW);
  return result;
}

uint16_t XPT2046::IO(uint16_t data) {
  return TERN(TOUCH_BUTTONS_HW_SPI, hardwareIO, softwareIO)(data);
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

bool XPT2046::getRawPoint(int16_t * const x, int16_t * const y) {
  if (isBusy() || !isTouched()) return false;
  *x = getRawData(XPT2046_X);
  *y = getRawData(XPT2046_Y);
  return isTouched();
}

uint16_t XPT2046::getRawData(const XPTCoordinate coordinate) {
  uint16_t data[3];

  dataTransferBegin();

  for (uint16_t i = 0; i < 3; ++i) {
    IO(coordinate);
    data[i] = (IO() << 4) | (IO() >> 4);
  }

  dataTransferEnd();

  // Discard whichever of the three readings is the odd one out
  const uint16_t delta01 = delta(data[0], data[1]),
                 delta02 = delta(data[0], data[2]),
                 delta12 = delta(data[1], data[2]);

  if (delta01 > delta02 || delta01 > delta12)
    data[delta02 > delta12 ? 0 : 1] = data[2];

  return (data[0] + data[1]) >> 1;
}

#endif // HAS_TFT_XPT2046 || HAS_RES_TOUCH_BUTTONS
#endif // ARDUINO_ARCH_ESP32
