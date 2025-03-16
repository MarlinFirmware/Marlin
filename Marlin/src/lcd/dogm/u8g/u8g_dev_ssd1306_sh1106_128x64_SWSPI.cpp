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

/**
 * Based on u8g_dev_ssd1306_128x64.c
 *
 * Universal 8bit Graphics Library
 *
 * Copyright (c) 2015, olikraus@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this list
 *    of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice, this
 *    list of conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../../../inc/MarlinConfig.h"

#if ALL(HAS_MARLINUI_U8GLIB, FORCE_SOFT_SPI) && ANY(U8GLIB_SH1106_SPI, U8GLIB_SSD1306_SPI)

#include "HAL_LCD_com_defines.h"

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

#define SH1106_PAGE_ADR(N)        (0x20), (N)
#define SH1106_COL_ADR(N)         (0x10 | ((N) >> 4)), ((N) & 0xFF)
#define SH1106_COLUMN_RANGE(N,O)  (0x21), (N), (O)
#define SH1106_PAGE_RANGE(N,O)    (0x22), (N), (O)
#define SH1106_SCROLL(N)          ((N) ? 0x2F : 0x2E)
#define SH1106_START_LINE(N)      (0x40 | (N))
#define SH1106_CONTRAST(N)        (0x81), (N)
#define SH1106_CHARGE_PUMP(N)     (0x8D), ((N) ? 0x14 : 0x10)
#define SH1106_ADC_REVERSE(N)     ((N) ? 0xA1 : 0xA0)
#define SH1106_ALL_PIX(N)         ((N) ? 0xA5 : 0xA4)
#define SH1106_INVERTED(N)        ((N) ? 0xA7 : 0xA6)
#define SH1106_MUX_RATIO(N)       (0xA8), (N)
#define SH1106_ON(N)              ((N) ? 0xAF : 0xAE)
#define SH1106_OUT_MODE(N)        ((N) ? 0xC8 : 0xC0)
#define SH1106_DISP_OFFS(N)       (0xD3), (N)
#define SH1106_OSC_FREQ(R,F)      (0xD5), ((F) << 4 | (R))
#define SH1106_CHARGE_PER(P,D)    (0xD9), ((D) << 4 | (P))
#define SH1106_COM_CONFIG(N)      (0xDA), ((N) ? 0x12 : 0x02)
#define SH1106_VCOM_DESEL(N)      (0xDB), (N)
#define SH1106_NOOP()             (0xE3)

static const uint8_t u8g_dev_ssd13xx_HAL_sleep_on[] PROGMEM = {
  U8G_ESC_ADR(0),   // Instruction mode
  U8G_ESC_CS(1),    // Enable chip
  SH1106_ON(0)      // Display off
  U8G_ESC_CS(0),    // Disable chip
  U8G_ESC_END       // End of sequence
};

static const uint8_t u8g_dev_ssd13xx_HAL_sleep_off[] PROGMEM = {
  U8G_ESC_ADR(0),   // Instruction mode
  U8G_ESC_CS(1),    // Enable chip
  SH1106_ON(1),     // Display on
  U8G_ESC_DLY(50),  // Delay 50 ms
  U8G_ESC_CS(0),    // Disable chip
  U8G_ESC_END       // End of sequence
};

#if ENABLED(U8GLIB_SH1106_SPI)

// Init sequence Adafruit 128x64 OLED (NOT TESTED). Like Adafruit3, but with page addressing mode.
static const uint8_t u8g_dev_sh1106_128x64_HAL_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),                // Disable chip
  U8G_ESC_ADR(0),               // Instruction mode
  U8G_ESC_RST(1),               // Do reset low pulse with (1*16)+2 milliseconds
  U8G_ESC_CS(1),                // Enable chip
  SH1106_ON(0),                 // Display off, sleep mode
  SH1106_OSC_FREQ(0, 8),        // Clock divide ratio (0:1) and oscillator frequency (8)
  SH1106_MUX_RATIO(0x3F),       // Mux ratio
  SH1106_DISP_OFFS(0),          // Display offset
  SH1106_START_LINE(0),         // Start line
  SH1106_CHARGE_PUMP(1),        // Charge pump setting
  SH1106_PAGE_ADR(0x02),        // page addressing mode
  SH1106_ADC_REVERSE(1),        // Segment remap A0/A1
  SH1106_OUT_MODE(1),           // 0: scan dir normal, 1: reverse
  SH1106_COM_CONFIG(1),         // COM pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5)
  SH1106_CONTRAST(0x80),        // Set contrast control
  SH1106_CHARGE_PER(0x1, 0xF),  // Pre-charge period
  SH1106_VCOM_DESEL(0x40),      // Vcomh deselect level
  SH1106_SCROLL(0),             // Deactivate scroll
  SH1106_ALL_PIX(0),            // Output RAM to display
  SH1106_INVERTED(0),           // Normal display mode
  SH1106_ON(1),                 // Display on
  U8G_ESC_CS(0),                // Disable chip
  U8G_ESC_END                   // End of sequence
};

// SH1106 (132x64) is compatible with SSD1306 (128x64) by adding a small margin to the larger display
static const uint8_t u8g_dev_sh1106_128x64_HAL_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),               // Instruction mode
  U8G_ESC_CS(1),                // Enable chip
  SH1106_COL_ADR(2),            // Column 2 to center 128 pixels in 132 pixels
  U8G_ESC_END                   // End of sequence
};

uint8_t u8g_dev_sh1106_128x64_HAL_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  switch(msg) {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_sh1106_128x64_HAL_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT: {
      u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_sh1106_128x64_HAL_data_start);
      u8g_WriteByte(u8g, dev, 0x0B0 | pb->p.page);  // Select current page (SSD1306)
      u8g_SetAddress(u8g, dev, 1);                  // Data mode
      if (u8g_pb_WriteBuffer(pb, u8g, dev) == 0) return 0;
      u8g_SetChipSelect(u8g, dev, 0);
    } break;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_HAL_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_HAL_sleep_off);
      return 1;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);                  // Instruction mode
      u8g_WriteByte(u8g, dev, 0x81);
      u8g_WriteByte(u8g, dev, *(uint8_t *) arg);
      u8g_SetChipSelect(u8g, dev, 0);
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_sh1106_128x64_HAL_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_sh1106_128x64_HAL_fn, U8G_COM_HAL_SW_SPI_FN);

#elif ENABLED(U8GLIB_SSD1306_SPI)

static const uint8_t u8g_dev_ssd1306_128x64_HAL_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),                // Disable chip
  U8G_ESC_ADR(0),               // Instruction mode
  U8G_ESC_RST(1),               // Do reset low pulse with (1*16)+2 milliseconds
  U8G_ESC_CS(1),                // Enable chip
  SH1106_ON(0),                 // Display off, sleep mode
  SH1106_OSC_FREQ(0, 8),        // Clock divide ratio (0:1) and oscillator frequency (8)
  SH1106_MUX_RATIO(0x3F),       // Mux ratio
  SH1106_DISP_OFFS(0),          // Display offset
  SH1106_START_LINE(0),         // Start line
  SH1106_CHARGE_PUMP(1),        // Charge pump setting
  SH1106_PAGE_ADR(0x02),        // page addressing mode
  SH1106_ADC_REVERSE(1),        // Segment remap A0/A1
  SH1106_OUT_MODE(1),           // 0: scan dir normal, 1: reverse
  SH1106_COM_CONFIG(1),         // COM pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5)
  SH1106_CONTRAST(0x80),        // Set contrast control
  SH1106_CHARGE_PER(0x1, 0xF),  // Pre-charge period
  SH1106_VCOM_DESEL(0x40),      // Vcomh deselect level
  SH1106_SCROLL(0),             // Deactivate scroll
  SH1106_ALL_PIX(0),            // Output RAM to display
  SH1106_INVERTED(0),           // Normal display mode
  SH1106_ON(1),                 // Display on
  U8G_ESC_CS(0),                // Disable chip
  U8G_ESC_END                   // End of sequence
};

static const uint8_t u8g_dev_ssd1306_128x64_HAL_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),               // Instruction mode
  U8G_ESC_CS(1),                // Enable chip
  SH1106_COL_ADR(0),            // Column 0
  U8G_ESC_END                   // End of sequence
};

uint8_t u8g_dev_ssd1306_128x64_HAL_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  switch(msg) {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_400NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_HAL_init_seq);
      break;
    case U8G_DEV_MSG_STOP: break;
    case U8G_DEV_MSG_PAGE_NEXT: {
      u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_HAL_data_start);
      u8g_WriteByte(u8g, dev, 0x0b0 | pb->p.page);  // Select current page (SSD1306)
      u8g_SetAddress(u8g, dev, 1);                  // Data mode
      if (u8g_pb_WriteBuffer(pb, u8g, dev) == 0) return 0;
      u8g_SetChipSelect(u8g, dev, 0);
    } break;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_HAL_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_HAL_sleep_off);
      return 1;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);                  // Instruction mode
      u8g_WriteByte(u8g, dev, 0x81);
      u8g_WriteByte(u8g, dev, *(uint8_t *) arg);
      u8g_SetChipSelect(u8g, dev, 0);
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_ssd1306_128x64_HAL_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ssd1306_128x64_HAL_fn, U8G_COM_HAL_SW_SPI_FN);

#endif // U8GLIB_SSD1306_SPI
#endif // HAS_MARLINUI_U8GLIB
