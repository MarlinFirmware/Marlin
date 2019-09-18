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

/*

  u8g_dev_tft_320x240_upscale_from_128x64.cpp

  Universal 8bit Graphics Library

  Copyright (c) 2011, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
  CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
  INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
  MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
  CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
  NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
  STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
  ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

*/

#include "../../inc/MarlinConfig.h"

#if HAS_GRAPHICAL_LCD && PIN_EXISTS(FSMC_CS)

#include "HAL_LCD_com_defines.h"
#include "ultralcd_DOGM.h"

#include <string.h>

#if ENABLED(LCD_USE_DMA_FSMC)
  extern void LCD_IO_WriteSequence(uint16_t *data, uint16_t length);
  extern void LCD_IO_WriteSequence_Async(uint16_t *data, uint16_t length);
  extern void LCD_IO_WaitSequence_Async();
  extern void LCD_IO_WriteMultiple(uint16_t color, uint32_t count);
#endif

#define WIDTH LCD_PIXEL_WIDTH
#define HEIGHT LCD_PIXEL_HEIGHT
#define PAGE_HEIGHT 8

#define X_LO LCD_PIXEL_OFFSET_X
#define Y_LO LCD_PIXEL_OFFSET_Y
#define X_HI (X_LO + 2 * WIDTH  - 1)
#define Y_HI (Y_LO + 2 * HEIGHT - 1)

#define LCD_COLUMN      0x2A   /* Colomn address register */
#define LCD_ROW         0x2B   /* Row address register */
#define LCD_WRITE_RAM   0x2C

// see https://ee-programming-notepad.blogspot.com/2016/10/16-bit-color-generator-picker.html

#define COLOR_BLACK       0x0000  // #000000
#define COLOR_WHITE       0xFFFF  // #FFFFFF
#define COLOR_SILVER      0xC618  // #C0C0C0
#define COLOR_GREY        0x7BEF  // #808080
#define COLOR_DARKGREY    0x4208  // #404040
#define COLOR_DARKGREY2   0x39E7  // #303030
#define COLOR_DARK        0x0003  // Some dark color

#define COLOR_RED         0xF800  // #FF0000
#define COLOR_LIME        0x7E00  // #00FF00
#define COLOR_BLUE        0x001F  // #0000FF
#define COLOR_YELLOW      0xFFE0  // #FFFF00
#define COLOR_MAGENTA     0xF81F  // #FF00FF
#define COLOR_FUCHSIA     0xF81F  // #FF00FF
#define COLOR_CYAN        0x07FF  // #00FFFF
#define COLOR_AQUA        0x07FF  // #00FFFF

#define COLOR_MAROON      0x7800  // #800000
#define COLOR_GREEN       0x03E0  // #008000
#define COLOR_NAVY        0x000F  // #000080
#define COLOR_OLIVE       0x8400  // #808000
#define COLOR_PURPLE      0x8010  // #800080
#define COLOR_TEAL        0x0410  // #008080

#define COLOR_ORANGE      0xFC00  // #FF7F00

#ifndef TFT_MARLINUI_COLOR
  #define TFT_MARLINUI_COLOR COLOR_WHITE
#endif
#ifndef TFT_MARLINBG_COLOR
  #define TFT_MARLINBG_COLOR COLOR_BLACK
#endif
#ifndef TFT_DISABLED_COLOR
  #define TFT_DISABLED_COLOR COLOR_DARK
#endif
#ifndef TFT_BTCANCEL_COLOR
  #define TFT_BTCANCEL_COLOR COLOR_RED
#endif
#ifndef TFT_BTARROWS_COLOR
  #define TFT_BTARROWS_COLOR COLOR_BLUE
#endif
#ifndef TFT_BTOKMENU_COLOR
  #define TFT_BTOKMENU_COLOR COLOR_RED
#endif

static uint32_t lcd_id = 0;

#define U8G_ESC_DATA(x) (uint8_t)(x >> 8), (uint8_t)(x & 0xFF)

static const uint8_t page_first_sequence[] = {
  U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(X_LO), U8G_ESC_DATA(X_HI),
  U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(Y_LO), U8G_ESC_DATA(Y_HI),
  U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
  U8G_ESC_END
};

static const uint8_t clear_screen_sequence[] = {
  U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), 0x00, 0x00, U8G_ESC_DATA(LCD_FULL_PIXEL_WIDTH),
  U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), 0x00, 0x00, U8G_ESC_DATA(LCD_FULL_PIXEL_HEIGHT),
  U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
  U8G_ESC_END
};

#if ENABLED(TOUCH_BUTTONS)

  static const uint8_t separation_line_sequence_left[] = {
    U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(10), U8G_ESC_DATA(159),
    U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(170), U8G_ESC_DATA(173),
    U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
    U8G_ESC_END
  };

  static const uint8_t separation_line_sequence_right[] = {
    U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(160), U8G_ESC_DATA(309),
    U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(170), U8G_ESC_DATA(173),
    U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
    U8G_ESC_END
  };

  static const uint8_t buttonD_sequence[] = {
    U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(14), U8G_ESC_DATA(77),
    U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(185), U8G_ESC_DATA(224),
    U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
    U8G_ESC_END
  };

  static const uint8_t buttonA_sequence[] = {
    U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(90), U8G_ESC_DATA(153),
    U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(185), U8G_ESC_DATA(224),
    U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
    U8G_ESC_END
  };

  static const uint8_t buttonB_sequence[] = {
    U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(166), U8G_ESC_DATA(229),
    U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(185), U8G_ESC_DATA(224),
    U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
    U8G_ESC_END
  };

  static const uint8_t buttonC_sequence[] = {
    U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(242), U8G_ESC_DATA(305),
    U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(185), U8G_ESC_DATA(224),
    U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
    U8G_ESC_END
  };

#endif

static const uint8_t st7789v_init_sequence[] = { // 0x8552 - ST7789V
  U8G_ESC_ADR(0),
  0x10,
  U8G_ESC_DLY(10),
  0x01,
  U8G_ESC_DLY(100), U8G_ESC_DLY(100),
  0x11,
  U8G_ESC_DLY(120),
  0x36, U8G_ESC_ADR(1), 0xA0,
  U8G_ESC_ADR(0), 0x3A, U8G_ESC_ADR(1), 0x05,
  U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), 0x00, 0x00, 0x01, 0x3F,
  U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), 0x00, 0x00, 0x00, 0xEF,
  U8G_ESC_ADR(0), 0xB2, U8G_ESC_ADR(1), 0x0C, 0x0C, 0x00, 0x33, 0x33,
  U8G_ESC_ADR(0), 0xB7, U8G_ESC_ADR(1), 0x35,
  U8G_ESC_ADR(0), 0xBB, U8G_ESC_ADR(1), 0x1F,
  U8G_ESC_ADR(0), 0xC0, U8G_ESC_ADR(1), 0x2C,
  U8G_ESC_ADR(0), 0xC2, U8G_ESC_ADR(1), 0x01, 0xC3,
  U8G_ESC_ADR(0), 0xC4, U8G_ESC_ADR(1), 0x20,
  U8G_ESC_ADR(0), 0xC6, U8G_ESC_ADR(1), 0x0F,
  U8G_ESC_ADR(0), 0xD0, U8G_ESC_ADR(1), 0xA4, 0xA1,
  U8G_ESC_ADR(0), 0xE0, U8G_ESC_ADR(1), 0xD0, 0x08, 0x11, 0x08, 0x0C, 0x15, 0x39, 0x33, 0x50, 0x36, 0x13, 0x14, 0x29, 0x2D,
  U8G_ESC_ADR(0), 0xE1, U8G_ESC_ADR(1), 0xD0, 0x08, 0x10, 0x08, 0x06, 0x06, 0x39, 0x44, 0x51, 0x0B, 0x16, 0x14, 0x2F, 0x31,
  U8G_ESC_ADR(0), 0x29, 0x11, 0x35, U8G_ESC_ADR(1), 0x00,
  U8G_ESC_END
};

static const uint8_t ili9341_init_sequence[] = { // 0x9341 - ILI9341
  U8G_ESC_ADR(0),
  0x10,
  U8G_ESC_DLY(10),
  0x01,
  U8G_ESC_DLY(100), U8G_ESC_DLY(100),
  0x36, U8G_ESC_ADR(1), 0xE8,
  U8G_ESC_ADR(0), 0x3A, U8G_ESC_ADR(1), 0x55,
  U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), 0x00, 0x00, 0x01, 0x3F,
  U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), 0x00, 0x00, 0x00, 0xEF,
  U8G_ESC_ADR(0), 0xC5, U8G_ESC_ADR(1), 0x3E, 0x28,
  U8G_ESC_ADR(0), 0xC7, U8G_ESC_ADR(1), 0x86,
  U8G_ESC_ADR(0), 0xB1, U8G_ESC_ADR(1), 0x00, 0x18,
  U8G_ESC_ADR(0), 0xC0, U8G_ESC_ADR(1), 0x23,
  U8G_ESC_ADR(0), 0xC1, U8G_ESC_ADR(1), 0x10,
  U8G_ESC_ADR(0), 0x29,
  U8G_ESC_ADR(0), 0x11,
  U8G_ESC_DLY(100),
  U8G_ESC_END
};

#if ENABLED(TOUCH_BUTTONS)

  static const uint8_t buttonD[] = {
    B01111111,B11111111,B11111111,B11111110,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00011000,B00110000,B00000001,
    B10000000,B00001100,B01100000,B00000001,
    B10000000,B00000110,B11000000,B00000001,
    B10000000,B00000011,B10000000,B00000001,
    B10000000,B00000011,B10000000,B00000001,
    B10000000,B00000110,B11000000,B00000001,
    B10000000,B00001100,B01100000,B00000001,
    B10000000,B00011000,B00110000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B01111111,B11111111,B11111111,B11111110,
  };

  #if ENABLED(REVERSE_MENU_DIRECTION)

    static const uint8_t buttonA[] = {
      B01111111,B11111111,B11111111,B11111110,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B11100000,B00000000,B00000001,
      B10000000,B11100000,B00000000,B00000001,
      B10000000,B11100000,B00000000,B00000001,
      B10000000,B11100000,B00000000,B00000001,
      B10000000,B11100000,B00111111,B11100001,
      B10000111,B11111100,B00111111,B11100001,
      B10000011,B11111000,B00000000,B00000001,
      B10000001,B11110000,B00000000,B00000001,
      B10000000,B11100000,B00000000,B00000001,
      B10000000,B01000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B01111111,B11111111,B11111111,B11111110,
    };
    static const uint8_t buttonB[] = {
      B01111111,B11111111,B11111111,B11111110,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B01100000,B00000010,B00000001,
      B10000000,B01100000,B00000111,B00000001,
      B10000000,B01100000,B00001111,B10000001,
      B10000000,B01100000,B00011111,B11000001,
      B10000111,B11111110,B00111111,B11100001,
      B10000111,B11111110,B00000111,B00000001,
      B10000000,B01100000,B00000111,B00000001,
      B10000000,B01100000,B00000111,B00000001,
      B10000000,B01100000,B00000111,B00000001,
      B10000000,B01100000,B00000111,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B01111111,B11111111,B11111111,B11111110,
    };

  #else

    static const uint8_t buttonA[] = {
      B01111111,B11111111,B11111111,B11111110,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B01000000,B00000000,B00000001,
      B10000000,B11100000,B00000000,B00000001,
      B10000001,B11110000,B00000000,B00000001,
      B10000011,B11111000,B00000000,B00000001,
      B10000111,B11111100,B00111111,B11100001,
      B10000000,B11100000,B00111111,B11100001,
      B10000000,B11100000,B00000000,B00000001,
      B10000000,B11100000,B00000000,B00000001,
      B10000000,B11100000,B00000000,B00000001,
      B10000000,B11100000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B01111111,B11111111,B11111111,B11111110,
    };

    static const uint8_t buttonB[] = {
      B01111111,B11111111,B11111111,B11111110,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B01100000,B00000111,B00000001,
      B10000000,B01100000,B00000111,B00000001,
      B10000000,B01100000,B00000111,B00000001,
      B10000000,B01100000,B00000111,B00000001,
      B10000111,B11111110,B00000111,B00000001,
      B10000111,B11111110,B00111111,B11100001,
      B10000000,B01100000,B00011111,B11000001,
      B10000000,B01100000,B00001111,B10000001,
      B10000000,B01100000,B00000111,B00000001,
      B10000000,B01100000,B00000010,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B10000000,B00000000,B00000000,B00000001,
      B01111111,B11111111,B11111111,B11111110,
    };

  #endif

  static const uint8_t buttonC[] = {
    B01111111,B11111111,B11111111,B11111110,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00011100,B00000001,
    B10000000,B00000100,B00011100,B00000001,
    B10000000,B00001100,B00011100,B00000001,
    B10000000,B00011111,B11111100,B00000001,
    B10000000,B00111111,B11111100,B00000001,
    B10000000,B00011111,B11111100,B00000001,
    B10000000,B00001100,B00000000,B00000001,
    B10000000,B00000100,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B10000000,B00000000,B00000000,B00000001,
    B01111111,B11111111,B11111111,B11111110,
  };

  void drawImage(const uint8_t *data, u8g_t *u8g, u8g_dev_t *dev, uint16_t length, uint16_t height, uint16_t color) {
    uint16_t buffer[128];

    for (uint16_t i = 0; i < height; i++) {
      uint16_t k = 0;
      for (uint16_t j = 0; j < length; j++) {
        uint16_t v = TFT_MARLINBG_COLOR;
        if (*(data + (i * (length >> 3) + (j >> 3))) & (0x80 >> (j & 7)))
          v = color;
        else
          v = TFT_MARLINBG_COLOR;
        buffer[k++] = v; buffer[k++] = v;
      }
      #ifdef LCD_USE_DMA_FSMC
        if (k <= 80) { // generally is... for our buttons
          memcpy(&buffer[k], &buffer[0], k * sizeof(uint16_t));
          LCD_IO_WriteSequence(buffer, k * sizeof(uint16_t));
        }
        else {
          LCD_IO_WriteSequence(buffer, k);
          LCD_IO_WriteSequence(buffer, k);
        }
      #else
        u8g_WriteSequence(u8g, dev, k << 1, (uint8_t *)buffer);
        u8g_WriteSequence(u8g, dev, k << 1, (uint8_t *)buffer);
      #endif
    }
  }

#endif // TOUCH_BUTTONS

// Used to fill RGB565 (16bits) background
inline void memset2(const void *ptr, uint16_t fill, size_t cnt) {
  uint16_t* wptr = (uint16_t*)ptr;
  for (size_t i = 0; i < cnt; i += 2) { *wptr = fill; wptr++; }
}

static bool preinit = true;
static uint8_t page;

uint8_t u8g_dev_tft_320x240_upscale_from_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
  #ifdef LCD_USE_DMA_FSMC
    static uint16_t bufferA[512], bufferB[512];
    uint16_t* buffer = &bufferA[0];
    bool allow_async = true;
  #else
    uint16_t buffer[WIDTH*2]; // 16-bit RGB 565 pixel line buffer
  #endif
  switch (msg) {
    case U8G_DEV_MSG_INIT:
      dev->com_fn(u8g, U8G_COM_MSG_INIT, U8G_SPI_CLK_CYCLE_NONE, &lcd_id);
      if (lcd_id == 0x040404) return 0; // No connected display on FSMC
      if (lcd_id == 0xFFFFFF) return 0; // No connected display on SPI

      if ((lcd_id & 0xFFFF) == 0x8552)  // ST7789V
        u8g_WriteEscSeqP(u8g, dev, st7789v_init_sequence);
      if ((lcd_id & 0xFFFF) == 0x9341)  // ILI9341
        u8g_WriteEscSeqP(u8g, dev, ili9341_init_sequence);

      if (preinit) {
        preinit = false;
        return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
      }

      // Clear Screen Sequence
      u8g_WriteEscSeqP(u8g, dev, clear_screen_sequence);
      #ifdef LCD_USE_DMA_FSMC
        LCD_IO_WriteMultiple(TFT_MARLINBG_COLOR, (320*240));
      #else
        memset2(buffer, TFT_MARLINBG_COLOR, 160);
        for (uint16_t i = 0; i < 960; i++)
          u8g_WriteSequence(u8g, dev, 160, (uint8_t *)buffer);
      #endif

      // bottom line and buttons
      #if ENABLED(TOUCH_BUTTONS)

        #ifdef LCD_USE_DMA_FSMC
          u8g_WriteEscSeqP(u8g, dev, separation_line_sequence_left);
          LCD_IO_WriteMultiple(TFT_DISABLED_COLOR, 300);
          u8g_WriteEscSeqP(u8g, dev, separation_line_sequence_right);
          LCD_IO_WriteMultiple(TFT_DISABLED_COLOR, 300);
        #else
          memset2(buffer, TFT_DISABLED_COLOR, 150);
          u8g_WriteEscSeqP(u8g, dev, separation_line_sequence_left);
          for (uint8_t i = 4; i--;)
            u8g_WriteSequence(u8g, dev, 150, (uint8_t *)buffer);
          u8g_WriteEscSeqP(u8g, dev, separation_line_sequence_right);
          for (uint8_t i = 4; i--;)
            u8g_WriteSequence(u8g, dev, 150, (uint8_t *)buffer);
        #endif

        u8g_WriteEscSeqP(u8g, dev, buttonD_sequence);
        drawImage(buttonD, u8g, dev, 32, 20, TFT_BTCANCEL_COLOR);

        u8g_WriteEscSeqP(u8g, dev, buttonA_sequence);
        drawImage(buttonA, u8g, dev, 32, 20, TFT_BTARROWS_COLOR);

        u8g_WriteEscSeqP(u8g, dev, buttonB_sequence);
        drawImage(buttonB, u8g, dev, 32, 20, TFT_BTARROWS_COLOR);

        u8g_WriteEscSeqP(u8g, dev, buttonC_sequence);
        drawImage(buttonC, u8g, dev, 32, 20, TFT_BTOKMENU_COLOR);
      #endif // TOUCH_BUTTONS

      return 0;

    case U8G_DEV_MSG_STOP: preinit = true; break;

    case U8G_DEV_MSG_PAGE_FIRST:
      page = 0;
      u8g_WriteEscSeqP(u8g, dev, page_first_sequence);
      break;

    case U8G_DEV_MSG_PAGE_NEXT:
      if (++page > (HEIGHT / PAGE_HEIGHT)) return 1;

      for (uint8_t y = 0; y < PAGE_HEIGHT; y++) {
        uint32_t k = 0;
        #ifdef LCD_USE_DMA_FSMC
          buffer = (y & 1) ? bufferB : bufferA;
        #endif
        for (uint16_t i = 0; i < (uint32_t)pb->width; i++) {
          const uint8_t b = *(((uint8_t *)pb->buf) + i);
          const uint16_t c = TEST(b, y) ? TFT_MARLINUI_COLOR : TFT_MARLINBG_COLOR;
          buffer[k++] = c; buffer[k++] = c;
        }
        #ifdef LCD_USE_DMA_FSMC
          memcpy(&buffer[256], &buffer[0], 512);
          if (allow_async) {
            if (y > 0 || page > 1) LCD_IO_WaitSequence_Async();
            if (y == 7 && page == 8)
              LCD_IO_WriteSequence(buffer, 512); // last line of last page
            else
              LCD_IO_WriteSequence_Async(buffer, 512);
          }
          else
            LCD_IO_WriteSequence(buffer, 512);
        #else
          uint8_t* bufptr = (uint8_t*) buffer;
          for (uint8_t i = 2; i--;) {
            u8g_WriteSequence(u8g, dev, WIDTH, &bufptr[0]);
            u8g_WriteSequence(u8g, dev, WIDTH, &bufptr[WIDTH]);
            u8g_WriteSequence(u8g, dev, WIDTH, &bufptr[WIDTH*2]);
            u8g_WriteSequence(u8g, dev, WIDTH, &bufptr[WIDTH*3]);
          }
        #endif
      }
      break;

    case U8G_DEV_MSG_SLEEP_ON:
      // Enter Sleep Mode (10h)
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      // Sleep Out (11h)
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_tft_320x240_upscale_from_128x64, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_tft_320x240_upscale_from_128x64_fn, U8G_COM_HAL_FSMC_FN);

#endif // HAS_GRAPHICAL_LCD && FSMC_CS
