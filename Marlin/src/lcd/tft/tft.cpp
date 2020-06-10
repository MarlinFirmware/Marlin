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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if HAS_GRAPHICAL_TFT

#include "tft.h"
#include "st7735.h"
#include "st7789v.h"
#include "st7796s.h"
#include "ili9328.h"
#include "ili9341.h"
#include "ili9488.h"

uint16_t TFT::buffer[];
uint32_t TFT::lcd_id = 0xFFFFFFFF;

void TFT::init() {
  if (lcd_id != 0xFFFFFFFF)
    return;

  io.Init();
  lcd_id = io.GetID() & 0xFFFF;

  switch(lcd_id) {
    case 0x7796:    // ST7796     480x320
      SERIAL_ECHO_MSG(" ST7796S");
      write_esc_sequence(st7796s_init);
      break;
    case 0x8552:    // ST7789V    320x240
      SERIAL_ECHO_MSG(" ST7789V");
      write_esc_sequence(st7789v_init);
      break;
    case 0x89F0:    // ST7735     160x128
      SERIAL_ECHO_MSG(" ST7735");
      write_esc_sequence(st7735_init);
      break;
    case 0x9328:    // ILI9328    320x240
      SERIAL_ECHO_MSG(" ILI9328");
      write_esc_sequence(ili9328_init);
      break;
    case 0x9341:    // ILI9341    320x240
      SERIAL_ECHO_MSG(" ILI9341");
      write_esc_sequence(ili9341_init);
      break;
    case 0x9488:    // ILI9488    480x320
      SERIAL_ECHO_MSG(" ILI9488");
      write_esc_sequence(ili9488_init);
      break;
    default:
      lcd_id = 0;
  }
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


  switch(lcd_id) {
    case 0x7796:    // ST7796     480x320
    case 0x8552:    // ST7789V    320x240
    case 0x89F0:    // ST7735     160x128
    case 0x9341:    // ILI9341    320x240
    case 0x9488:    // ILI9488    480x320
      io.DataTransferBegin(DATASIZE_8BIT);

      // CASET: Column Address Set
      io.WriteReg(ILI9341_CASET);
      io.WriteData((Xmin >> 8) & 0xFF);
      io.WriteData(Xmin & 0xFF);
      io.WriteData((Xmax >> 8) & 0xFF);
      io.WriteData(Xmax & 0xFF);

      // RASET: Row Address Set
      io.WriteReg(ILI9341_PASET);
      io.WriteData((Ymin >> 8) & 0xFF);
      io.WriteData(Ymin & 0xFF);
      io.WriteData((Ymax >> 8) & 0xFF);
      io.WriteData(Ymax & 0xFF);

      // RAMWR: Memory Write
      io.WriteReg(ILI9341_RAMWR);
      break;
    case 0x9328:    // ILI9328    320x240
      io.DataTransferBegin(DATASIZE_16BIT);

      // Mind the mess: with landscape screen orientation 'Horizontal' is Y and 'Vertical' is X
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

      io.WriteReg(ILI9328_RAMWR);
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
