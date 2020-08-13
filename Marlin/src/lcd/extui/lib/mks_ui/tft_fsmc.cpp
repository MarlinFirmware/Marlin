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

#include "../../../../inc/MarlinConfig.h"

#if ENABLED(TFT_LVGL_UI_FSMC)

#include HAL_PATH(../../HAL, tft/tft_fsmc.h)
TFT_IO tftio;

void LCD_IO_Init(uint8_t cs, uint8_t rs);
void LCD_IO_WriteData(uint16_t RegValue);
void LCD_IO_WriteReg(uint16_t Reg);
#ifdef LCD_USE_DMA_FSMC
  void LCD_IO_WriteMultiple(uint16_t data, uint32_t count);
  void LCD_IO_WriteSequence(uint16_t *data, uint16_t length);
#endif

void LCD_IO_Init(uint8_t cs, uint8_t rs) {
  tftio.Init();
}

void LCD_IO_WriteData(uint16_t RegValue) {
  tftio.WriteData(RegValue);
}

void LCD_IO_WriteReg(uint16_t Reg) {
  tftio.WriteReg(Reg);
}

#ifdef LCD_USE_DMA_FSMC
  void LCD_IO_WriteMultiple(uint16_t color, uint32_t count) {
    tftio.WriteMultiple(color, count);
  }

  void LCD_IO_WriteSequence(uint16_t *data, uint16_t length) {
    tftio.WriteSequence(data, length);
  }
#endif // LCD_USE_DMA_FSMC

#endif // HAS_TFT_LVGL_UI
