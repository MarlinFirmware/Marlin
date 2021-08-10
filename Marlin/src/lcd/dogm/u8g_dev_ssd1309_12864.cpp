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
#include "../../inc/MarlinConfigPre.h"

#if HAS_MARLINUI_U8GLIB

#include "HAL_LCD_com_defines.h"
#include <U8glib-HAL.h>

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

// SSD1309 init sequence
static const uint8_t u8g_dev_ssd1309_128x64_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),  // Disable chip
  U8G_ESC_ADR(0), // Instruction mode
  U8G_ESC_RST(1), // Do reset low pulse with (1*16)+2 milliseconds
  U8G_ESC_CS(1),  // Enable chip

  0xFD,0x12,      // Command Lock
  0xAE,           // Set Display Off
  0xD5,0xA0,      // Set Display Clock Divide Ratio/Oscillator Frequency
  0xA8,0x3F,      // Set Multiplex Ratio
  0x3D,0x00,      // Set Display Offset
  0x40,           // Set Display Start Line
  0xA1,           // Set Segment Re-Map
  0xC8,           // Set COM Output Scan Direction
  0xDA,0x12,      // Set COM Pins Hardware Configuration
  0x81,0xDF,      // Set Current Control
  0xD9,0x82,      // Set Pre-Charge Period
  0xDB,0x34,      // Set VCOMH Deselect Level
  0xA4,           // Set Entire Display On/Off
  0xA6,           // Set Normal/Inverse Display
  U8G_ESC_VCC(1), // Power up VCC & Stabilized
  U8G_ESC_DLY(50),
  0xAF,           // Set Display On
  U8G_ESC_DLY(50),
  U8G_ESC_CS(0),  // Disable chip
  U8G_ESC_END     // End of sequence
};

// Select one init sequence here
#define u8g_dev_ssd1309_128x64_init_seq u8g_dev_ssd1309_128x64_init_seq

static const uint8_t u8g_dev_ssd1309_128x64_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),   // Instruction mode
  U8G_ESC_CS(1),    // Enable chip
  0x010,            // Set upper 4 bit of the col adr to 0
  0x000,            // Set lower 4 bit of the col adr to 4
  U8G_ESC_END       // End of sequence
};

static const uint8_t u8g_dev_ssd13xx_sleep_on[] PROGMEM = {
  U8G_ESC_ADR(0),   // Instruction mode
  U8G_ESC_CS(1),    // Enable chip
  0x0AE,            // Display off
  U8G_ESC_CS(0),    // Disable chip
  U8G_ESC_END       // End of sequence
};

static const uint8_t u8g_dev_ssd13xx_sleep_off[] PROGMEM = {
  U8G_ESC_ADR(0),   // Instruction mode
  U8G_ESC_CS(1),    // Enable chip
  0x0AF,            // Display on
  U8G_ESC_DLY(50),  // Delay 50 ms
  U8G_ESC_CS(0),    // Disable chip
  U8G_ESC_END       // End of sequence
};

uint8_t u8g_dev_ssd1309_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  switch (msg) {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1309_128x64_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT: {
      u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1309_128x64_data_start);
      u8g_WriteByte(u8g, dev, 0x0B0 | pb->p.page);  // Select current page (SSD1306)
      u8g_SetAddress(u8g, dev, 1);                  // Data mode
      if (u8g_pb_WriteBuffer(pb, u8g, dev) == 0) return 0;
      u8g_SetChipSelect(u8g, dev, 0);
    }
    break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);                  // Instruction mode
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) );  // 11 Jul 2015: fixed contrast calculation
      u8g_SetChipSelect(u8g, dev, 0);
      return 1;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_off);
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_ssd1309_buf[WIDTH*2] U8G_NOCOMMON ;
u8g_pb_t u8g_dev_ssd1309_pb = { {8, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_ssd1309_buf};
u8g_dev_t u8g_dev_ssd1309_sw_spi = { u8g_dev_ssd1309_128x64_fn, &u8g_dev_ssd1309_pb, U8G_COM_HAL_SW_SPI_FN };

#endif // HAS_MARLINUI_U8GLIB
