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

#define DOGM_AREA_LEFT   TFT_PIXEL_OFFSET_X
#define DOGM_AREA_TOP    TFT_PIXEL_OFFSET_Y
#define DOGM_AREA_WIDTH  (GRAPHICAL_TFT_UPSCALE) * (LCD_PIXEL_WIDTH)
#define DOGM_AREA_HEIGHT (GRAPHICAL_TFT_UPSCALE) * (LCD_PIXEL_HEIGHT)

#define BUTTON_AREA_TOP BUTTON_Y_LO
#define BUTTON_AREA_BOT BUTTON_Y_HI

TouchButtons touch;

void TouchButtons::init() { touchIO.Init(); }

uint8_t TouchButtons::read_buttons() {
  #ifdef HAS_WIRED_LCD
    int16_t x, y;

    if (!touchIO.getRawPoint(&x, &y)) return 0;

    x = uint16_t((uint32_t(x) * XPT2046_X_CALIBRATION) >> 16) + XPT2046_X_OFFSET;
    y = uint16_t((uint32_t(y) * XPT2046_Y_CALIBRATION) >> 16) + XPT2046_Y_OFFSET;

    #if (TFT_ROTATION & TFT_ROTATE_180)
      x = TFT_WIDTH - x;
      y = TFT_HEIGHT - y;
    #endif

    // Touch within the button area simulates an encoder button
    if (y > BUTTON_AREA_TOP && y < BUTTON_AREA_BOT)
      return WITHIN(x, BUTTOND_X_LO, BUTTOND_X_HI) ? EN_D
           : WITHIN(x, BUTTONA_X_LO, BUTTONA_X_HI) ? EN_A
           : WITHIN(x, BUTTONB_X_LO, BUTTONB_X_HI) ? EN_B
           : WITHIN(x, BUTTONC_X_LO, BUTTONC_X_HI) ? EN_C
           : 0;

    if ( !WITHIN(x, DOGM_AREA_LEFT, DOGM_AREA_LEFT + DOGM_AREA_WIDTH)
      || !WITHIN(y, DOGM_AREA_TOP,  DOGM_AREA_TOP  + DOGM_AREA_HEIGHT)
    ) return 0;

    // Column and row above BUTTON_AREA_TOP
    int8_t col = (x - (DOGM_AREA_LEFT)) * (LCD_WIDTH)  / (DOGM_AREA_WIDTH),
           row = (y - (DOGM_AREA_TOP))  * (LCD_HEIGHT) / (DOGM_AREA_HEIGHT);

    // Send the touch to the UI (which will simulate the encoder wheel)
    MarlinUI::screen_click(row, col, x, y);
  #endif
  return 0;
}

#endif // HAS_TOUCH_XPT2046
