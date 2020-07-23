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

#include <stdint.h>

#define SPI_TFT_CS_H  OUT_WRITE(SPI_TFT_CS_PIN, HIGH)
#define SPI_TFT_CS_L  OUT_WRITE(SPI_TFT_CS_PIN, LOW)

#define SPI_TFT_DC_H  OUT_WRITE(SPI_TFT_DC_PIN, HIGH)
#define SPI_TFT_DC_L  OUT_WRITE(SPI_TFT_DC_PIN, LOW)

#define SPI_TFT_RST_H OUT_WRITE(SPI_TFT_RST_PIN, HIGH)
#define SPI_TFT_RST_L OUT_WRITE(SPI_TFT_RST_PIN, LOW)

#define SPI_TFT_BLK_H OUT_WRITE(LCD_BACKLIGHT_PIN, HIGH)
#define SPI_TFT_BLK_L OUT_WRITE(LCD_BACKLIGHT_PIN, LOW)

class TFT {
public:
  void spi_init(uint8_t spiRate);
  uint8_t spi_Rec();
  uint8_t spi_read_write_byte(uint8_t data);
  void spi_Read(uint8_t* buf, uint16_t nbyte);
  void spi_Send(uint8_t b);
  void spi_SendBlock(uint8_t token, const uint8_t* buf);
  void LCD_WR_REG(uint8_t cmd);
  void LCD_WR_DATA(uint8_t data);
  void SetCursor(uint16_t x, uint16_t y);
  void SetWindows(uint16_t x, uint16_t y, uint16_t with, uint16_t height);
  void LCD_init();
  void LCD_clear(uint16_t color);
  void LCD_WriteRAM_Prepare();
};

extern TFT SPI_TFT;
