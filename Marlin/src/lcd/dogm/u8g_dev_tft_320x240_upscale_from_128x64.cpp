/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#if HAS_GRAPHICAL_LCD

#include "U8glib.h"
#include "HAL_LCD_com_defines.h"
#include "string.h"

#include "../../lcd/ultralcd.h"
#if HAS_COLOR_LEDS && ENABLED(PRINTER_EVENT_LEDS)
#include "../../feature/leds/leds.h"
#endif
#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

#define X_MIN 32
#define Y_MIN 28
#define X_MAX (X_MIN + 2 * WIDTH  - 1)
#define Y_MAX (Y_MIN + 2 * HEIGHT - 1)

#define LCD_COLUMN      0x2A   /* Colomn address register */
#define LCD_ROW         0x2B   /* Row address register */
#define LCD_WRITE_RAM   0x2C

static uint32_t lcd_id = 0;
uint16_t color = 0xFFFF;
void drawButtons(u8g_t *u8g, u8g_dev_t *dev);

#define U8G_ESC_DATA(x) (uint8_t)(x >> 8), (uint8_t)(x & 0xFF)

static const uint8_t page_first_sequence[] = {
  U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(X_MIN), U8G_ESC_DATA(X_MAX),
  U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(Y_MIN), U8G_ESC_DATA(Y_MAX),
  U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
  U8G_ESC_END
};

static const uint8_t line_sequence[] = {
  U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(10), U8G_ESC_DATA(309),
  U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(170), U8G_ESC_DATA(171),
  U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
  U8G_ESC_END
};

static const uint8_t button0_sequence[] = {
  U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(20), U8G_ESC_DATA(99),
  U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(185), U8G_ESC_DATA(224),
  U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
  U8G_ESC_END
};

static const uint8_t button1_sequence[] = {
  U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(120), U8G_ESC_DATA(199),
  U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(185), U8G_ESC_DATA(224),
  U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
  U8G_ESC_END
};

static const uint8_t button2_sequence[] = {
  U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), U8G_ESC_DATA(220), U8G_ESC_DATA(299),
  U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), U8G_ESC_DATA(185), U8G_ESC_DATA(224),
  U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
  U8G_ESC_END
};
static const uint8_t button0[] = {
   B01111111,B11111111,B11111111,B11111111,B11111110,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00001000,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00111110,B00000000,B00000001,
   B10000000,B00000000,B01111111,B00000000,B00000001,
   B10000000,B00000000,B11111111,B10000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B01111111,B11111111,B11111111,B11111111,B11111110,
};

static const uint8_t button1[] = {
   B01111111,B11111111,B11111111,B11111111,B11111110,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B11111111,B10000000,B00000001,
   B10000000,B00000000,B01111111,B00000000,B00000001,
   B10000000,B00000000,B00111110,B00000000,B00000001,
   B10000000,B00000000,B00011100,B00000000,B00000001,
   B10000000,B00000000,B00001000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B01111111,B11111111,B11111111,B11111111,B11111110,
};

static const uint8_t button2[] = {
   B01111111,B11111111,B11111111,B11111111,B11111110,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B01000001,B11000000,B00000001,
   B10000000,B00000000,B11000001,B11000000,B00000001,
   B10000000,B00000001,B11111111,B11000000,B00000001,
   B10000000,B00000011,B11111111,B11000000,B00000001,
   B10000000,B00000001,B11111111,B11000000,B00000001,
   B10000000,B00000000,B11000000,B00000000,B00000001,
   B10000000,B00000000,B01000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B10000000,B00000000,B00000000,B00000000,B00000001,
   B01111111,B11111111,B11111111,B11111111,B11111110,
};

void drawButtons(u8g_t *u8g, u8g_dev_t *dev) {
  uint16_t buffer[160];
  uint32_t i, j, k, l;

  for (i = 0; i < 160; i++) { buffer[i] = color; }

  u8g_WriteEscSeqP(u8g, dev, line_sequence);
  for (k = 0; k < 8; k++) { u8g_WriteSequence(u8g, dev, 160, (uint8_t *)buffer); }

  u8g_WriteEscSeqP(u8g, dev, button0_sequence);
  for (j = 0; j < 20;  j++) {
    l = 0;
    for (i = 0; i < 5;  i++) {
      for (k = 0; k < 8 ; k++) {
        if (button0[j * 5 + i] & (128 >> k)) {
          buffer[l++] = color;
          buffer[l++] = color;
        } else {
          buffer[l++] = 0x0000;
          buffer[l++] = 0x0000;
        }
      }
    }
    for (k = 0; k < 2; k++) { u8g_WriteSequence(u8g, dev, 160, (uint8_t *)buffer); }
  }

  u8g_WriteEscSeqP(u8g, dev, button1_sequence);
  for (j = 0; j < 20;  j++) {
    l = 0;
    for (i = 0; i < 5;  i++) {
      for (k = 0; k < 8 ; k++) {
        if (button1[j * 5 + i] & (128 >> k)) {
          buffer[l++] = color;
          buffer[l++] = color;
        } else {
          buffer[l++] = 0x0000;
          buffer[l++] = 0x0000;
        }
      }
    }
    for (k = 0; k < 2; k++) { u8g_WriteSequence(u8g, dev, 160, (uint8_t *)buffer); }
  }

  u8g_WriteEscSeqP(u8g, dev, button2_sequence);
  for (j = 0; j < 20;  j++) {
    l = 0;
    for (i = 0; i < 5;  i++) {
      for (k = 0; k < 8 ; k++) {
        if (button2[j * 5 + i] & (128 >> k)) {
          buffer[l++] = color;
          buffer[l++] = color;
        } else {
          buffer[l++] = 0x0000;
          buffer[l++] = 0x0000;
        }
      }
    }
    for (k = 0; k < 2; k++) { u8g_WriteSequence(u8g, dev, 160, (uint8_t *)buffer); }
  }
}

static const uint8_t clear_screen_sequence[] = {
  U8G_ESC_ADR(0), LCD_COLUMN, U8G_ESC_ADR(1), 0x00, 0x00, 0x01, 0x3F,
  U8G_ESC_ADR(0), LCD_ROW,    U8G_ESC_ADR(1), 0x00, 0x00, 0x00, 0xEF,
  U8G_ESC_ADR(0), LCD_WRITE_RAM, U8G_ESC_ADR(1),
  U8G_ESC_END
};

static const uint8_t st7789v_init_sequence[] = { // 0x8552 - ST7789V
  U8G_ESC_ADR(0),
  0x10,
  U8G_ESC_DLY(10),
  0x01,
  U8G_ESC_DLY(100), U8G_ESC_DLY(100),
  0x11,
  U8G_ESC_DLY(120),
  0x36, U8G_ESC_ADR(1), 0xA0,
  U8G_ESC_ADR(0), 0x3A, U8G_ESC_ADR(1), 0x55,
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
  U8G_ESC_ADR(0), 0x29, 0x11, U8G_ESC_ADR(1),
  U8G_ESC_END
};

uint8_t u8g_dev_tft_320x240_upscale_from_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
#if HAS_COLOR_LEDS && ENABLED(PRINTER_EVENT_LEDS)
  uint16_t newColor;
#endif
  u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
  uint16_t buffer[256];
  uint32_t i, j, k;
  uint8_t byte;

  switch(msg) {
    case U8G_DEV_MSG_INIT:
      dev->com_fn(u8g, U8G_COM_MSG_INIT, U8G_SPI_CLK_CYCLE_NONE, &lcd_id);
      if (lcd_id == 0x040404) { return 0; } /* No connected display on FSMC */
      if (lcd_id == 0xFFFFFF) { return 0; } /* No connected display on SPI */

      memset(buffer, 0x00, sizeof(buffer));

      if ((lcd_id & 0xFFFF) == 0x8552) { // ST7789V
        u8g_WriteEscSeqP(u8g, dev, st7789v_init_sequence);
      }

      u8g_WriteEscSeqP(u8g, dev, clear_screen_sequence);
      for (i = 0; i < 960; i++) {
        u8g_WriteSequence(u8g, dev, 160, (uint8_t *)buffer);
      }
      break;

    case U8G_DEV_MSG_STOP:
      break;

    case U8G_DEV_MSG_PAGE_FIRST:
#if HAS_COLOR_LEDS && ENABLED(PRINTER_EVENT_LEDS)
      newColor = (0xF800 & (((uint16_t)leds.color.r) << 8)) | (0x07E0 & (((uint16_t)leds.color.g) << 3)) | (0x001F & (((uint16_t)leds.color.b) >> 3));
      if ((newColor != 0) && (newColor != color)) {
        color = newColor;
      }
      drawButtons(u8g, dev);

#endif
      u8g_WriteEscSeqP(u8g, dev, page_first_sequence);
      break;

    case U8G_DEV_MSG_PAGE_NEXT:
      for (j = 0; j < 8; j++) {
        k = 0;
        for (i = 0; i < (uint32_t) pb->width;  i++) {
          byte = *(((uint8_t *)pb->buf) + i);
          if (byte & (1 << j)) {
            buffer[k++] = color;
            buffer[k++] = color;
          } else {
            buffer[k++] = 0x0000;
            buffer[k++] = 0x0000;
          }
        }
        for (k = 0; k < 2; k++) {
          u8g_WriteSequence(u8g, dev, 128, (uint8_t*)buffer);
          u8g_WriteSequence(u8g, dev, 128, (uint8_t*)&(buffer[64]));
          u8g_WriteSequence(u8g, dev, 128, (uint8_t*)&(buffer[128]));
          u8g_WriteSequence(u8g, dev, 128, (uint8_t*)&(buffer[192]));
        }
      }
      break;

    case U8G_DEV_MSG_SLEEP_ON:
      return 1;

    case U8G_DEV_MSG_SLEEP_OFF:
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_tft_320x240_upscale_from_128x64, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_tft_320x240_upscale_from_128x64_fn, U8G_COM_HAL_FSMC_FN);

#endif // HAS_GRAPHICAL_LCD
