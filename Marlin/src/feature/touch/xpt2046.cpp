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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(TOUCH_BUTTONS)

#include "xpt2046.h"
#include "../../inc/MarlinConfig.h"
#include "../../lcd/dogm/ultralcd_DOGM.h" // for LCD_FULL_PIXEL_WIDTH, etc.

/*
 * Drawing and Touch proccess and logic
 *
 * All the drawings are done in LCD_PIXEL_WIDTH x LCD_PIXEL_HEIGHT (independent of display resolution) -> 128x64 
 * The touch resolution is TOUCH_SCREEN_WIDTH x TOUCH_SCREEN_HEIGHT independent of display resolution -> 320x240
 * And, finally we have the display resolution: LCD_FULL_PIXEL_WIDTH x LCD_FULL_PIXEL_HEIGHT   -> most common: 320x240 and 480x320
 *
 * The process is:
 *  1) draw things in LCD_PIXEL* (128x64)
 *  2) upscale it 2x or 3x to display resolution, to show in the screen
 *  3) upscale the touch coordinates to TOUCH_SCREEN* resolution, to process the touches
 *
 *  Why do need a TOUCH_SCREEN* resolution? Why don't just use the same upscale logic as in (2)?
 *    Because of calibration values. The first code of the touch was made to 320x240 in mind, so the calibration values were create using this.
 *    If we change the resolutio space of the touch code now, the current calibration values wont work anymore and it will break the compatiblity.
 *    Everyone that use tft with Marlin default menus will need new calibration values.
 *
 *  The Marlin menus are draw in:
 *  ____________________
 * | LCD_PIXEL_OFFSET_Y |
 * |     __________     |
 * |    | 128x64   |    |
 * | X  | upscaled |    |
 * |    |__________|    |
 * |                    |
 * |____________________|
 * |                    |
 * |   BUTTON AREA      |
 * |____________________|
 *
 * So, the touchable are are:
 *  - the touch button area (as it ever was)
 *  - plus the rectangle area of the marlin menus
 *
 * The button area are X,Y fixed, in TOUCH_SCREEN* coordinate space
 *
 * The Marlin screen touchable rectangle:
 *  - Y0 at LCD_PIXEL_OFFSET_Y, that needs to be translated to TOUCH_SCREEN* coordinate space => SCREEN_START_TOP
 *  - X0 at LCD_PIXEL_OFFSET_X, that needs to be translated to TOUCH_SCREEN* coordinate space => SCREEN_START_LEFT
 *  - Height: LCD_PIXEL_HEIGHT, that needs to be translated to TOUCH_SCREEN* coordinate space => SCREEN_HEIGHT
 *  - Width: LCD_PIXEL_WIDTH, that needs to be translated to TOUCH_SCREEN* coordinate space => SCREEN_WIDTH
 *
 */
// Touch screen resolution independent of display resolution
#define TOUCH_SCREEN_HEIGHT 240
#define TOUCH_SCREEN_WIDTH 320

// Coordinates in terms of touch area
#define BUTTON_AREA_TOP 175
#define BUTTON_AREA_BOT 234

#define SCREEN_START_TOP  ((LCD_PIXEL_OFFSET_Y) * (TOUCH_SCREEN_HEIGHT) / (LCD_FULL_PIXEL_HEIGHT))
#define SCREEN_START_LEFT ((LCD_PIXEL_OFFSET_X) * (TOUCH_SCREEN_WIDTH) / (LCD_FULL_PIXEL_WIDTH))
#define SCREEN_HEIGHT     ((LCD_PIXEL_HEIGHT * FSMC_UPSCALE) * (TOUCH_SCREEN_HEIGHT) / (LCD_FULL_PIXEL_HEIGHT))
#define SCREEN_WIDTH      ((LCD_PIXEL_WIDTH * FSMC_UPSCALE) * (TOUCH_SCREEN_WIDTH) / (LCD_FULL_PIXEL_WIDTH))

#define TOUCHABLE_Y_HEIGHT  SCREEN_HEIGHT
#define TOUCHABLE_X_WIDTH  SCREEN_WIDTH

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
extern int8_t encoderDiff;

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
  const uint16_t x = uint16_t(((uint32_t(getInTouch(XPT2046_X))) * tsoffsets[0]) >> 16) + tsoffsets[1],
                 y = uint16_t(((uint32_t(getInTouch(XPT2046_Y))) * tsoffsets[2]) >> 16) + tsoffsets[3];
  if (!isTouched()) return 0; // Fingers must still be on the TS for a valid read.

  // Touch within the button area simulates an encoder button
  if (y > BUTTON_AREA_TOP && y < BUTTON_AREA_BOT)
    return WITHIN(x,  14,  77) ? EN_D
         : WITHIN(x,  90, 153) ? EN_A
         : WITHIN(x, 166, 229) ? EN_B
         : WITHIN(x, 242, 305) ? EN_C
         : 0;

  if (x > TOUCH_SCREEN_WIDTH || !WITHIN(y, SCREEN_START_TOP, SCREEN_START_TOP + SCREEN_HEIGHT)) return 0;

  // Column and row above BUTTON_AREA_TOP
  int8_t col = (x - (SCREEN_START_LEFT)) * (LCD_WIDTH) / (TOUCHABLE_X_WIDTH),
         row = (y - (SCREEN_START_TOP)) * (LCD_HEIGHT) / (TOUCHABLE_Y_HEIGHT);

  // Send the touch to the UI (which will simulate the encoder wheel)
  MarlinUI::screen_click(row, col, x, y);
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

uint16_t XPT2046::getInTouch(const XPTCoordinate coordinate) {
  uint16_t data[3];

  OUT_WRITE(TOUCH_CS_PIN, LOW);

  const uint8_t coord = uint8_t(coordinate) | XPT2046_CONTROL | XPT2046_DFR_MODE;
  for (uint16_t i = 0; i < 3 ; i++) {
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

#endif // TOUCH_BUTTONS
