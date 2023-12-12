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

#if ENABLED(U8GLIB_SSD1309)

#include "HAL_LCD_com_defines.h"
#include <U8glib-HAL.h>

#define WIDTH       128
#define HEIGHT       64
#define PAGE_HEIGHT   8

#define SSD1309_COL_ADR(N)        (0x10 | ((N)>>4)), ((N) & 0x0F)
#define SSD1309_ADDR_MODE(N)      (0x20), (N)
#define SSD1309_COLUMN_RANGE(N,O) (0x21), (N), (O)
#define SSD1309_PAGE_RANGE(N,O)   (0x22), (N), (O)
#define SSD1309_SCROLL(N)         ((N) ? 0x2F : 0x2E)
#define SSD1309_START_LINE(N)     (0x40 | (N))
#define SSD1309_CONTRAST(N)       (0x81), (N)
#define SSD1309_SEG_REMAP(N)      ((N) ? 0xA1 : 0xA0)
#define SSD1309_ALL_PIX(N)        ((N) ? 0xA5 : 0xA4)
#define SSD1309_INVERTED(N)       ((N) ? 0xA7 : 0xA6)
#define SSD1309_MUX_RATIO(N)      (0xA8), (N)
#define SSD1309_ON(N)             ((N) ? 0xAF : 0xAE)
#define SSD1309_OUT_MODE(N)       ((N) ? 0xC8 : 0xC0)
#define SSD1309_DISP_OFFS(N)      (0xD3), (N)
#define SSD1309_OSC_FREQ(R,F)     (0xD5), ((F) << 4 | (R))
#define SSD1309_CHARGE_PER(P,D)   (0xD9), ((D) << 4 | (P))
#define SSD1309_COMMAND_LOCK()    (0xFD), (0x12)
#define SSD1309_COM_CONFIG(N)     (0xDA), ((N) ? 0x12 : 0x02)
#define SSD1309_VCOM_DESEL(N)     (0xDB), (N)
#define SSD1309_NOP()             (0xE3)

// SSD1309 init sequence
static const uint8_t u8g_dev_ssd1309_128x64_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),            // Disable chip
  U8G_ESC_ADR(0),           // Instruction mode
  U8G_ESC_RST(1),           // Do reset low pulse with (1*16)+2 milliseconds
  U8G_ESC_CS(1),            // Enable chip

  SSD1309_COMMAND_LOCK(),   // Command Lock
  SSD1309_ON(0),            // Display OFF
  SSD1309_OSC_FREQ(0, 10),  // Clock divide ratio (0:1) and oscillator frequency (10)
  SSD1309_MUX_RATIO(0x3F),  // Multiplex Ratio
  SSD1309_DISP_OFFS(0),     // Display Offset
  SSD1309_START_LINE(0),    // Display Start Line
  SSD1309_SEG_REMAP(1),     // Segment Re-Map
  SSD1309_OUT_MODE(1),      // COM Output Scan Direction
  SSD1309_COM_CONFIG(1),    // COM Pins Hardware Configuration
  SSD1309_CONTRAST(0xDF),   // Current Control
  SSD1309_CHARGE_PER(2, 8), // Pre-Charge Period
  SSD1309_VCOM_DESEL(0x34), // VCOMH Deselect Level
  SSD1309_ALL_PIX(0),       // Entire Display On/Off
  SSD1309_INVERTED(0),      // Normal/Inverse Display
  U8G_ESC_VCC(1),           // Power up VCC & Stabilized
  U8G_ESC_DLY(50),          // Wait 50ms
  SSD1309_ON(1),            // Display ON
  U8G_ESC_DLY(50),          // Wait 50ms
  U8G_ESC_CS(0),            // Disable chip
  U8G_ESC_END               // End of sequence
};

static const uint8_t u8g_dev_ssd1309_128x64_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),     // Instruction mode
  U8G_ESC_CS(1),      // Enable chip
  SSD1309_COL_ADR(0), // Set the col adr to 0
  U8G_ESC_END         // End of sequence
};

static const uint8_t u8g_dev_ssd13xx_sleep_on[] PROGMEM = {
  U8G_ESC_ADR(0),   // Instruction mode
  U8G_ESC_CS(1),    // Enable chip
  SSD1309_ON(0),    // Display OFF
  U8G_ESC_CS(0),    // Disable chip
  U8G_ESC_END       // End of sequence
};

static const uint8_t u8g_dev_ssd13xx_sleep_off[] PROGMEM = {
  U8G_ESC_ADR(0),   // Instruction mode
  U8G_ESC_CS(1),    // Enable chip
  SSD1309_ON(1),    // Display ON
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
      u8g_WriteByte(u8g, dev, 0xB0 | pb->p.page);   // Select current page (SSD1309)
      u8g_SetAddress(u8g, dev, 1);                  // Data mode
      if (u8g_pb_WriteBuffer(pb, u8g, dev) == 0) return 0;
      u8g_SetChipSelect(u8g, dev, 0);
    }
    break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);                  // Instruction mode
      u8g_WriteByte(u8g, dev, 0x81);
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

uint8_t u8g_dev_ssd1309_buf[WIDTH * 2] U8G_NOCOMMON;
u8g_pb_t u8g_dev_ssd1309_pb = { { 8, HEIGHT, 0, 0, 0 }, WIDTH, u8g_dev_ssd1309_buf };
u8g_dev_t u8g_dev_ssd1309_sw_spi = { u8g_dev_ssd1309_128x64_fn, &u8g_dev_ssd1309_pb, U8G_COM_HAL_SW_SPI_FN };

#endif // U8GLIB_SSD1309
