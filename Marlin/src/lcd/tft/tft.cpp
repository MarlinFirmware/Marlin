/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if HAS_GRAPHICAL_TFT

#include "tft.h"
#include "st7735.h"
#include "st7789v.h"
#include "ili9328.h"
#include "ili9341.h"

uint16_t TFT::buffer[];
uint32_t TFT::lcd_id = 0xFFFFFFFF;

void TFT::init() {
  if (lcd_id != 0xFFFFFFFF)
    return;

  io.Init();
  lcd_id = io.GetID();

  switch(lcd_id & 0xFFFF) {
    case 0x8552:    // ST7789V
      SERIAL_ECHO_MSG(" ST7789V");
      write_esc_sequence(st7789v_init);
      break;
    case 0x89F0:    // ST7735
      SERIAL_ECHO_MSG(" ST7735");
      write_esc_sequence(st7735_init);
      break;
    case 0x9328:    // ILI9328
      SERIAL_ECHO_MSG(" ILI9328");
      write_esc_sequence(ili9328_init);
      break;
    case 0x9341:    // ILI9341
      SERIAL_ECHO_MSG(" ILI9341");
      write_esc_sequence(ili9341_init);
      break;
    case 0x0404:  // No connected display on FSMC
    case 0xFFFF:  // No connected display on SPI
    default:
      lcd_id = 0;
  }
  lcd_id &= 0xFFFF;
}

void TFT::set_window(uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax) {
  #if defined(OFFSET_X)
    Xmin += OFFSET_X;
    Xmax += OFFSET_X;
  #endif
  #if defined(OFFSET_Y)
    Ymin += OFFSET_Y;
    Ymax += OFFSET_Y;
  #endif

  io.DataTransferBegin(DATASIZE_8BIT);

  switch(lcd_id) {
    case 0x8552:    // ST7789V
    case 0x89F0:    // ST7735
    case 0x9341:    // ILI9341
      /* CASET: Column Address Set */
      io.WriteReg(ILI9341_CASET);
      io.WriteData((Xmin >> 8) & 0xFF);
      io.WriteData(Xmin & 0xFF);
      io.WriteData((Xmax >> 8) & 0xFF);
      io.WriteData(Xmax & 0xFF);

      /* RASET: Row Address Set */
      io.WriteReg(ILI9341_RASET);
      io.WriteData((Ymin >> 8) & 0xFF);
      io.WriteData(Ymin & 0xFF);
      io.WriteData((Ymax >> 8) & 0xFF);
      io.WriteData(Ymax & 0xFF);

      /* RAMWR: Memory Write */
      io.WriteReg(ILI9341_RAMWR);
      break;
    case 0x9328:    // ILI9328
      io.WriteReg(ILI9328_HASTART);
      io.WriteData(Ymin);
      io.WriteReg(ILI9328_HAEND);
      io.WriteData(Ymax);
      io.WriteReg(ILI9328_VASTART);
      io.WriteData(Xmin);
      io.WriteReg(ILI9328_VAEND);
      io.WriteData(Xmax);

      io.WriteReg(ILI9328_HASET);
      io.WriteData(Ymin);
      io.WriteReg(ILI9328_VASET);
      io.WriteData(Xmin);

      io.WriteReg(ILI9328_WRITE_RAM);
      break;
    default:
      break;
  }

  io.DataTransferEnd();
}

void TFT::write_esc_sequence(const uint16_t *Sequence) {
  uint16_t dataWidth, data;

  dataWidth = *Sequence++;
  io.DataTransferBegin(dataWidth);

  for (;;) {
    data = *Sequence++;
    if (data != 0xFFFF) {
      io.WriteData(data);
      continue;
    }
    data = *Sequence++;
    if (data == 0x7FFF) return;
    if (data == 0xFFFF) {
      io.WriteData(0xFFFF);
    } else if (data & 0x8000) {
      HAL_Delay(data & 0x7FFF);
    } else if ((data & 0xFF00) == 0) {
      io.WriteReg(data);
    }
  }

  io.DataTransferEnd();
}

TFT tft;

#endif // HAS_GRAPHICAL_TFT
