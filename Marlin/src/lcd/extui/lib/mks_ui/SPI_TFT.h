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
#pragma once

#include "../../inc/MarlinConfigPre.h"

#if ENABLED(TFT_LVGL_UI_SPI)
  #include HAL_PATH(../../HAL, tft/tft_spi.h)
#elif ENABLED(TFT_LVGL_UI_FSMC)
  #include HAL_PATH(../../HAL, tft/tft_fsmc.h)
#endif

#define TFT_RST_H OUT_WRITE(TFT_RESET_PIN, HIGH)
#define TFT_RST_L OUT_WRITE(TFT_RESET_PIN, LOW)

#define TFT_BLK_H OUT_WRITE(LCD_BACKLIGHT_PIN, HIGH)
#define TFT_BLK_L OUT_WRITE(LCD_BACKLIGHT_PIN, LOW)

class TFT {
public:
  TFT_IO tftio;
  void spi_init(uint8_t spiRate);
  void LCD_WR_REG(uint8_t cmd);
  void LCD_WR_DATA(uint8_t data);
  void SetPoint(uint16_t x, uint16_t y, uint16_t point);
  void SetWindows(uint16_t x, uint16_t y, uint16_t with, uint16_t height);
  void LCD_init();
  void LCD_clear(uint16_t color);
  void LCD_Draw_Logo();
};

extern TFT SPI_TFT;
