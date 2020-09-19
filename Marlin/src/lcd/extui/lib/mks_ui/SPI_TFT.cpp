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

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(TFT_LVGL_UI_SPI)

#include "SPI_TFT.h"
#include "pic_manager.h"
#include "tft_lvgl_configuration.h"

#include "../../../../inc/MarlinConfig.h"

#include <SPI.h>

TFT SPI_TFT;

// use SPI1 for the spi tft.
void TFT::spi_init(uint8_t spiRate) {
  tftio.Init();
}

void TFT::LCD_WR_REG(uint8_t cmd) {
  tftio.WriteReg(cmd);
}

void TFT::LCD_WR_DATA(uint8_t data) {
  tftio.WriteData(data);
}

void TFT::SetPoint(uint16_t x, uint16_t y, uint16_t point) {
  if ((x > 480) || (y > 320)) return;

  SetWindows(x, y, 1, 1);
  tftio.WriteMultiple(point, (uint16_t)1);
}

void TFT::SetWindows(uint16_t x, uint16_t y, uint16_t with, uint16_t height) {
  tftio.DataTransferBegin(DATASIZE_8BIT);

  LCD_WR_REG(0x2A);
  LCD_WR_DATA(x >> 8);
  LCD_WR_DATA(x);
  LCD_WR_DATA((x + with - 1) >> 8);
  LCD_WR_DATA((x + with - 1));

  LCD_WR_REG(0x2B);
  LCD_WR_DATA(y >> 8);
  LCD_WR_DATA(y);
  LCD_WR_DATA((y + height - 1) >> 8);
  LCD_WR_DATA(y + height - 1);

  LCD_WR_REG(0X2C);

  tftio.DataTransferEnd();
}

void TFT::LCD_init() {
  TFT_BLK_L;
  TFT_RST_H;
  delay(150);
  TFT_RST_L;
  delay(150);
  TFT_RST_H;

  tftio.DataTransferBegin(DATASIZE_8BIT);

  delay(120);
  LCD_WR_REG(0x11);
  delay(120);

  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0xC3);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x96);

  LCD_WR_REG(0x36);
  LCD_WR_DATA(0x28 + TERN0(GRAPHICAL_TFT_ROTATE_180, 0x80));
  LCD_WR_REG(0x3A);
  LCD_WR_DATA(0x55);

  LCD_WR_REG(0xB4);
  LCD_WR_DATA(0x01);
  LCD_WR_REG(0xB7);
  LCD_WR_DATA(0xC6);
  LCD_WR_REG(0xE8);
  LCD_WR_DATA(0x40);
  LCD_WR_DATA(0x8A);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x00);
  LCD_WR_DATA(0x29);
  LCD_WR_DATA(0x19);
  LCD_WR_DATA(0xA5);
  LCD_WR_DATA(0x33);
  LCD_WR_REG(0xC1);
  LCD_WR_DATA(0x06);
  LCD_WR_REG(0xC2);
  LCD_WR_DATA(0xA7);
  LCD_WR_REG(0xC5);
  LCD_WR_DATA(0x18);
  LCD_WR_REG(0xE0);     // Positive Voltage Gamma Control
  LCD_WR_DATA(0xF0);
  LCD_WR_DATA(0x09);
  LCD_WR_DATA(0x0B);
  LCD_WR_DATA(0x06);
  LCD_WR_DATA(0x04);
  LCD_WR_DATA(0x15);
  LCD_WR_DATA(0x2F);
  LCD_WR_DATA(0x54);
  LCD_WR_DATA(0x42);
  LCD_WR_DATA(0x3C);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x14);
  LCD_WR_DATA(0x18);
  LCD_WR_DATA(0x1B);
  LCD_WR_REG(0xE1);     // Negative Voltage Gamma Control
  LCD_WR_DATA(0xF0);
  LCD_WR_DATA(0x09);
  LCD_WR_DATA(0x0B);
  LCD_WR_DATA(0x06);
  LCD_WR_DATA(0x04);
  LCD_WR_DATA(0x03);
  LCD_WR_DATA(0x2D);
  LCD_WR_DATA(0x43);
  LCD_WR_DATA(0x42);
  LCD_WR_DATA(0x3B);
  LCD_WR_DATA(0x16);
  LCD_WR_DATA(0x14);
  LCD_WR_DATA(0x17);
  LCD_WR_DATA(0x1B);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x3C);
  LCD_WR_REG(0xF0);
  LCD_WR_DATA(0x69);
  delay(120);     // Delay 120ms
  LCD_WR_REG(0x29);     // Display ON

  tftio.DataTransferEnd();

  LCD_clear(0x0000);    //
  LCD_Draw_Logo();
  TFT_BLK_H;
  delay(2000);
}

void TFT::LCD_clear(uint16_t color) {
  SetWindows(0, 0, (TFT_WIDTH) - 1, (TFT_HEIGHT) - 1);
  tftio.WriteMultiple(color, (uint32_t)(TFT_WIDTH) * (TFT_HEIGHT));
}

extern unsigned char bmp_public_buf[17 * 1024];

void TFT::LCD_Draw_Logo() {
  SetWindows(0, 0, TFT_WIDTH, TFT_HEIGHT);
  for (uint16_t i = 0; i < (TFT_HEIGHT); i ++) {
    Pic_Logo_Read((uint8_t *)"", (uint8_t *)bmp_public_buf, (TFT_WIDTH) * 2);
    tftio.WriteSequence((uint16_t *)bmp_public_buf, TFT_WIDTH);
  }
}

#endif // HAS_TFT_LVGL_UI_SPI
