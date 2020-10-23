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

#include "touch_buttons.h"
#include "../scaled_tft.h"

#include HAL_PATH(../../HAL, tft/xpt2046.h)
XPT2046 touchIO;

#include "../../lcd/marlinui.h" // For EN_C bit mask

/**
 * Draw and Touch processing
 *
 *      LCD_PIXEL_WIDTH/HEIGHT (128x64) is the (emulated DOGM) Pixel Drawing resolution.
 *   TOUCH_SENSOR_WIDTH/HEIGHT (320x240) is the Touch Area resolution.
 * TFT_WIDTH/HEIGHT (320x240 or 480x320) is the Actual (FSMC) Display resolution.
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
 * The Marlin screen touchable area starts at TFT_PIXEL_OFFSET_X/Y (translated to SCREEN_PCT_LEFT/TOP)
 * and spans LCD_PIXEL_WIDTH/HEIGHT (scaled to SCREEN_PCT_WIDTH/HEIGHT).
 */

// Touch sensor resolution independent of display resolution
#define TOUCH_SENSOR_WIDTH  320
#define TOUCH_SENSOR_HEIGHT 240

#define SCREEN_PCT_WIDE(X) ((X) * (TOUCH_SENSOR_WIDTH)  / (TFT_WIDTH))
#define SCREEN_PCT_HIGH(Y) ((Y) * (TOUCH_SENSOR_HEIGHT) / (TFT_HEIGHT))

#define SCREEN_PCT_LEFT   SCREEN_PCT_WIDE(TFT_PIXEL_OFFSET_X)
#define SCREEN_PCT_TOP    SCREEN_PCT_HIGH(TFT_PIXEL_OFFSET_Y)
#define SCREEN_PCT_WIDTH  SCREEN_PCT_WIDE((GRAPHICAL_TFT_UPSCALE) * (LCD_PIXEL_WIDTH))
#define SCREEN_PCT_HEIGHT SCREEN_PCT_HIGH((GRAPHICAL_TFT_UPSCALE) * (LCD_PIXEL_HEIGHT))

// Coordinates in terms of 240-unit-tall touch area
#define BUTTON_AREA_TOP 175
#define BUTTON_AREA_BOT 234

TouchButtons touch;

void TouchButtons::init() { touchIO.Init(); }

uint8_t TouchButtons::read_buttons() {
  #ifdef HAS_WIRED_LCD
    int16_t x, y;

    if (!touchIO.getRawPoint(&x, &y)) return 0;

    x = uint16_t((uint32_t(x) * XPT2046_X_CALIBRATION) >> 16) + XPT2046_X_OFFSET;
    y = uint16_t((uint32_t(y) * XPT2046_Y_CALIBRATION) >> 16) + XPT2046_Y_OFFSET;

    #if (TFT_ROTATION & TFT_ROTATE_180)
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

    if ( !WITHIN(x, SCREEN_PCT_LEFT, SCREEN_PCT_LEFT + SCREEN_PCT_WIDTH)
      || !WITHIN(y, SCREEN_PCT_TOP,  SCREEN_PCT_TOP  + SCREEN_PCT_HEIGHT)
    ) return 0;

    // Column and row above BUTTON_AREA_TOP
    int8_t col = (x - (SCREEN_PCT_LEFT)) * (LCD_WIDTH)  / (SCREEN_PCT_WIDTH),
           row = (y - (SCREEN_PCT_TOP))  * (LCD_HEIGHT) / (SCREEN_PCT_HEIGHT);

    // Send the touch to the UI (which will simulate the encoder wheel)
    MarlinUI::screen_click(row, col, x, y);
  #endif
  return 0;
}

#endif // HAS_TOUCH_XPT2046
