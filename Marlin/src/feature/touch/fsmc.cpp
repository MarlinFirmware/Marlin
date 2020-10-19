/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "../../inc/MarlinConfig.h"

#if ENABLED(TOUCH_CALIBRATION) && PIN_EXISTS(FSMC_CS)

#include "fsmc.h"

#ifndef USE_DOGM_IO
void TOUCH_LCD_IO_WriteData(uint16_t RegValue) {
  LCD->RAM = RegValue;
  __DSB();
}

void TOUCH_LCD_IO_WriteReg(uint16_t Reg) {
  LCD->REG = Reg;
  __DSB();
}

uint32_t TOUCH_LCD_IO_ReadData(uint16_t RegValue, uint8_t ReadSize) {
  volatile uint32_t data;
  LCD->REG = RegValue;
  __DSB();

  data = LCD->RAM; // dummy read
  data = LCD->RAM & 0x00ff;

  while (--ReadSize) {
    data <<= 8;
    data |= (LCD->RAM & 0x00ff);
  }

  return uint32_t(data);
}
#endif

uint16_t TOUCH_LCD_IO_ReadReg(uint16_t RegValue) {
  LCD->REG = RegValue;
  __DSB();

  return LCD->RAM;
}

#endif // TOUCH_CALIBRATION && FSMC
