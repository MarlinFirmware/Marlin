/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if HAS_TOUCH_XPT2046

#include "xpt2046.h"
#include "../scaled_tft.h"

#ifndef XPT2046_Z1_THRESHOLD
  #define XPT2046_Z1_THRESHOLD 10
#endif

/**
 * Draw and Touch processing
 *
 *      LCD_PIXEL_WIDTH/HEIGHT (128x64) is the (emulated DOGM) Pixel Drawing resolution.
 *   TOUCH_SENSOR_WIDTH/HEIGHT (320x240) is the Touch Area resolution.
 * LCD_FULL_PIXEL_WIDTH/HEIGHT (320x240 or 480x320) is the Actual (FSMC) Display resolution.
 *
 *  - All native (u8g) drawing is done in LCD_PIXEL_* (128x64)
 *  - The DOGM pixels are is upscaled 2-3x (as needed) for display.
 *  - Touch coordinates use TOUCH_SENSOR_* resolution and are converted to
 *    click and scroll-wheel events (emulating of a common DOGM display).
 *
 *  TOUCH_SCREEN resolution exists to fit our calibration values. The original touch code was made
 *  and originally calibrated for 320x240. If you decide to change the resolution of the touch code,
 *  new calibration values will be needed.
 *
 *  The Marlin menus are drawn scaled in the upper region of the screen. The bottom region (in a
 *  fixed location in TOUCH_SCREEN* coordinate space) is used for 4 general-purpose buttons to
 *  navigate and select menu items. Both regions are touchable.
 *
 * The Marlin screen touchable area starts at LCD_PIXEL_OFFSET_X/Y (translated to SCREEN_START_LEFT/TOP)
 * and spans LCD_PIXEL_WIDTH/HEIGHT (scaled to SCREEN_WIDTH/HEIGHT).
 */

// Coordinates in terms of touch area
#define BUTTON_AREA_TOP 175
#define BUTTON_AREA_BOT 234

// Touch sensor resolution independent of display resolution
#define TOUCH_SENSOR_WIDTH  320
#define TOUCH_SENSOR_HEIGHT 240

#define SCREEN_WIDTH_PCT(X) ((X) * (TOUCH_SENSOR_WIDTH) / (LCD_FULL_PIXEL_WIDTH))
#define SCREEN_HEIGHT_PCT(Y) ((Y) * (TOUCH_SENSOR_HEIGHT) / (LCD_FULL_PIXEL_HEIGHT))

#define SCREEN_START_LEFT SCREEN_WIDTH_PCT(LCD_PIXEL_OFFSET_X)
#define SCREEN_START_TOP  SCREEN_HEIGHT_PCT(LCD_PIXEL_OFFSET_Y)
#define SCREEN_WIDTH      SCREEN_WIDTH_PCT((LCD_PIXEL_WIDTH) * (FSMC_UPSCALE))
#define SCREEN_HEIGHT     SCREEN_HEIGHT_PCT((LCD_PIXEL_HEIGHT) * (FSMC_UPSCALE))

#define TOUCHABLE_X_WIDTH  SCREEN_WIDTH
#define TOUCHABLE_Y_HEIGHT SCREEN_HEIGHT

#ifndef TOUCH_INT_PIN
  #define TOUCH_INT_PIN  -1
#endif
#ifndef TOUCH_MISO_PIN
  #define TOUCH_MISO_PIN MISO_PIN
#endif
#ifndef TOUCH_MOSI_PIN
  #define TOUCH_MOSI_PIN MOSI_PIN
#endif
#ifndef TOUCH_SCK_PIN
  #define TOUCH_SCK_PIN  SCK_PIN
#endif
#ifndef TOUCH_CS_PIN
  #define TOUCH_CS_PIN   CS_PIN
#endif

XPT2046 touch;

void XPT2046::init() {
  SET_INPUT(TOUCH_MISO_PIN);
  SET_OUTPUT(TOUCH_MOSI_PIN);
  SET_OUTPUT(TOUCH_SCK_PIN);
  OUT_WRITE(TOUCH_CS_PIN, HIGH);

  #if PIN_EXISTS(TOUCH_INT)
    // Optional Pendrive interrupt pin
    SET_INPUT(TOUCH_INT_PIN);
  #endif

  // Read once to enable pendrive status pin
  getInTouch(XPT2046_X);
}

#include "../../lcd/ultralcd.h" // For EN_C bit mask

uint8_t XPT2046::read_buttons() {
  #ifdef HAS_SPI_LCD
    int16_t tsoffsets[4] = { 0 };

    if (tsoffsets[0] + tsoffsets[1] == 0) {
      // Not yet set, so use defines as fallback...
      tsoffsets[0] = XPT2046_X_CALIBRATION;
      tsoffsets[1] = XPT2046_X_OFFSET;
      tsoffsets[2] = XPT2046_Y_CALIBRATION;
      tsoffsets[3] = XPT2046_Y_OFFSET;
    }

    // We rely on XPT2046 compatible mode to ADS7843, hence no Z1 and Z2 measurements possible.

    if (!isTouched()) return 0;
    uint16_t x = uint16_t(((uint32_t(getInTouch(XPT2046_X))) * tsoffsets[0]) >> 16) + tsoffsets[1],
             y = uint16_t(((uint32_t(getInTouch(XPT2046_Y))) * tsoffsets[2]) >> 16) + tsoffsets[3];
    if (!isTouched()) return 0; // Fingers must still be on the TS for a valid read.

    #if ENABLED(GRAPHICAL_TFT_ROTATE_180)
      x = TOUCH_SENSOR_WIDTH - x;
      y = TOUCH_SENSOR_HEIGHT - y;
    #endif

    // Touch within the button area simulates an encoder button
    if (y > BUTTON_AREA_TOP && y < BUTTON_AREA_BOT)
      return WITHIN(x,  14,  77) ? EN_D
          : WITHIN(x,  90, 153) ? EN_A
          : WITHIN(x, 166, 229) ? EN_B
          : WITHIN(x, 242, 305) ? EN_C
          : 0;

    if (x > TOUCH_SENSOR_WIDTH || !WITHIN(y, SCREEN_START_TOP, SCREEN_START_TOP + SCREEN_HEIGHT)) return 0;

    // Column and row above BUTTON_AREA_TOP
    int8_t col = (x - (SCREEN_START_LEFT)) * (LCD_WIDTH) / (TOUCHABLE_X_WIDTH),
           row = (y - (SCREEN_START_TOP)) * (LCD_HEIGHT) / (TOUCHABLE_Y_HEIGHT);

    // Send the touch to the UI (which will simulate the encoder wheel)
    MarlinUI::screen_click(row, col, x, y);
  #endif
  return 0;
}

bool XPT2046::isTouched() {
  return (
    #if PIN_EXISTS(TOUCH_INT)
      READ(TOUCH_INT_PIN) != HIGH
    #else
      getInTouch(XPT2046_Z1) >= XPT2046_Z1_THRESHOLD
    #endif
  );
}

#if ENABLED(TOUCH_BUTTONS_HW_SPI)
  #include <SPI.h>

  static void touch_spi_init(uint8_t spiRate) {
    /**
     * STM32F1 APB2 = 72MHz, APB1 = 36MHz, max SPI speed of this MCU if 18Mhz
     * STM32F1 has 3 SPI ports, SPI1 in APB2, SPI2/SPI3 in APB1
     * so the minimum prescale of SPI1 is DIV4, SPI2/SPI3 is DIV2
     */
    uint8_t clock;
    switch (spiRate) {
      case SPI_FULL_SPEED:    clock = SPI_CLOCK_DIV4;  break;
      case SPI_HALF_SPEED:    clock = SPI_CLOCK_DIV4; break;
      case SPI_QUARTER_SPEED: clock = SPI_CLOCK_DIV8; break;
      case SPI_EIGHTH_SPEED:  clock = SPI_CLOCK_DIV16; break;
      case SPI_SPEED_5:       clock = SPI_CLOCK_DIV32; break;
      case SPI_SPEED_6:       clock = SPI_CLOCK_DIV64; break;
      default:                clock = SPI_CLOCK_DIV2;        // Default from the SPI library
    }
    SPI.setModule(TOUCH_BUTTONS_HW_SPI_DEVICE);
    SPI.begin();
    SPI.setClockDivider(clock);
    SPI.setBitOrder(MSBFIRST);
    SPI.setDataMode(SPI_MODE0);
  }
#endif // TOUCH_BUTTONS_HW_SPI

uint16_t XPT2046::getInTouch(const XPTCoordinate coordinate) {
  uint16_t data[3];
  const uint8_t coord = uint8_t(coordinate) | XPT2046_CONTROL | XPT2046_DFR_MODE;

  #if ENABLED(TOUCH_BUTTONS_HW_SPI)

    touch_spi_init(SPI_SPEED_6);
    for (uint16_t i = 0; i < 3; i++) {
      OUT_WRITE(TOUCH_CS_PIN, LOW);
      SPI.transfer(coord);
      data[i] = (((SPI.transfer(0xFF) << 8) | SPI.transfer(0xFF)) >> 3) & 0x0FFF;
      WRITE(TOUCH_CS_PIN, HIGH);
    }

  #else // !TOUCH_BUTTONS_HW_SPI

    OUT_WRITE(TOUCH_CS_PIN, LOW);
    for (uint16_t i = 0; i < 3; i++) {
      for (uint8_t j = 0x80; j; j >>= 1) {
        WRITE(TOUCH_SCK_PIN, LOW);
        WRITE(TOUCH_MOSI_PIN, bool(coord & j));
        WRITE(TOUCH_SCK_PIN, HIGH);
      }

      data[i] = 0;
      for (uint16_t j = 0x8000; j; j >>= 1) {
        WRITE(TOUCH_SCK_PIN, LOW);
        if (READ(TOUCH_MISO_PIN)) data[i] |= j;
        WRITE(TOUCH_SCK_PIN, HIGH);
      }
      WRITE(TOUCH_SCK_PIN, LOW);
      data[i] >>= 4;
    }
    WRITE(TOUCH_CS_PIN, HIGH);

  #endif // !TOUCH_BUTTONS_HW_SPI

  uint16_t delta01 = _MAX(data[0], data[1]) - _MIN(data[0], data[1]),
           delta02 = _MAX(data[0], data[2]) - _MIN(data[0], data[2]),
           delta12 = _MAX(data[1], data[2]) - _MIN(data[1], data[2]);

  if (delta01 <= delta02 && delta01 <= delta12)
    return (data[0] + data[1]) >> 1;

  if (delta02 <= delta12)
    return (data[0] + data[2]) >> 1;

  return (data[1] + data[2]) >> 1;
}

bool XPT2046::getTouchPoint(uint16_t &x, uint16_t &y) {
  if (isTouched()) {
    x = getInTouch(XPT2046_X);
    y = getInTouch(XPT2046_Y);
  }
  return isTouched();
}

#endif // HAS_TOUCH_XPT2046
