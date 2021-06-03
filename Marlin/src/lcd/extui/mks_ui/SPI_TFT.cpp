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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "SPI_TFT.h"
#include "pic_manager.h"
#include "tft_lvgl_configuration.h"

#include "../../../inc/MarlinConfig.h"

#include <SPI.h>

#include "draw_ui.h"

TFT SPI_TFT;

// use SPI1 for the spi tft.
void TFT::spi_init(uint8_t spiRate) {
  tftio.Init();
}

void TFT::SetPoint(uint16_t x, uint16_t y, uint16_t point) {
  if ((x > 480) || (y > 320)) return;

  setWindow(x, y, 1, 1);
  tftio.WriteMultiple(point, (uint16_t)1);
}

void TFT::setWindow(uint16_t x, uint16_t y, uint16_t with, uint16_t height) {
  tftio.set_window(x, y, (x + with - 1), (y + height - 1));
}

void TFT::LCD_init() {
  tftio.InitTFT();
  #if PIN_EXISTS(TFT_BACKLIGHT)
    OUT_WRITE(TFT_BACKLIGHT_PIN, LOW);
  #endif
  delay(100);
  LCD_clear(0x0000);
  LCD_Draw_Logo();
  #if PIN_EXISTS(TFT_BACKLIGHT)
    OUT_WRITE(TFT_BACKLIGHT_PIN, HIGH);
  #endif
  #if HAS_LOGO_IN_FLASH
    delay(2000);
  #endif
}

void TFT::LCD_clear(uint16_t color) {
  setWindow(0, 0, (TFT_WIDTH), (TFT_HEIGHT));
  tftio.WriteMultiple(color, (uint32_t)(TFT_WIDTH) * (TFT_HEIGHT));
}

void TFT::LCD_Draw_Logo() {
  #if HAS_LOGO_IN_FLASH
    setWindow(0, 0, TFT_WIDTH, TFT_HEIGHT);
    for (uint16_t i = 0; i < (TFT_HEIGHT); i ++) {
      Pic_Logo_Read((uint8_t *)"", (uint8_t *)bmp_public_buf, (TFT_WIDTH) * 2);
      tftio.WriteSequence((uint16_t *)bmp_public_buf, TFT_WIDTH);
    }
  #endif
}

#endif // HAS_TFT_LVGL_UI
