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
 * u8g_dev_tft_320x240_upscale_from_128x64.cpp
 *
 * Universal 8bit Graphics Library
 *
 * Copyright (c) 2011, olikraus@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
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

#include "../../inc/MarlinConfig.h"

#if HAS_MARLINUI_U8GLIB && (PIN_EXISTS(FSMC_CS) || ENABLED(SPI_GRAPHICAL_TFT))

#include "HAL_LCD_com_defines.h"
#include "ultralcd_DOGM.h"

#include <string.h>

#if EITHER(LCD_USE_DMA_FSMC, LCD_USE_DMA_SPI)
  #define HAS_LCD_IO 1
#endif

#if ENABLED(SPI_GRAPHICAL_TFT)
  #include HAL_PATH(../../HAL, tft/tft_spi.h)
#elif ENABLED(FSMC_GRAPHICAL_TFT)
  #include HAL_PATH(../../HAL, tft/tft_fsmc.h)
#endif

TFT_IO tftio;

#define WIDTH  LCD_PIXEL_WIDTH
#define HEIGHT LCD_PIXEL_HEIGHT
#define PAGE_HEIGHT 8

#include "../scaled_tft.h"

#define UPSCALE0(M) ((M) * (GRAPHICAL_TFT_UPSCALE))
#define UPSCALE(A,M) (UPSCALE0(M) + (A))
#define X_HI (UPSCALE(TFT_PIXEL_OFFSET_X, WIDTH) - 1)
#define Y_HI (UPSCALE(TFT_PIXEL_OFFSET_Y, HEIGHT) - 1)

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

#define ST7789V_CASET       0x2A   /* Column address register */
#define ST7789V_RASET       0x2B   /* Row address register */
#define ST7789V_WRITE_RAM   0x2C   /* Write data to GRAM */


/* Mind the mess: with landscape screen orientation 'Horizontal' is Y and 'Vertical' is X */
#define ILI9328_HASET       0x20   /* Horizontal GRAM address register (0-255) */
#define ILI9328_VASET       0x21   /* Vertical GRAM address register (0-511)*/
#define ILI9328_WRITE_RAM   0x22   /* Write data to GRAM */

#define ILI9328_HASTART     0x50   /* Horizontal address start position (0-255) */
#define ILI9328_HAEND       0x51   /* Horizontal address end position (0-255) */
#define ILI9328_VASTART     0x52   /* Vertical address start position (0-511) */
#define ILI9328_VAEND       0x53   /* Vertical address end position (0-511) */

static void setWindow_ili9328(u8g_t *u8g, u8g_dev_t *dev, uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax) {
  #if HAS_LCD_IO
    tftio.DataTransferBegin(DATASIZE_8BIT);
    #define IO_REG_DATA(R,D) do { tftio.WriteReg(R); tftio.WriteData(D); }while(0)
  #else
    #define IO_REG_DATA(R,D) do { u8g_WriteByte(u8g, dev, R); u8g_WriteSequence(u8g, dev, 2, (uint8_t *)&D); }while(0)
  #endif

  #if NONE(LCD_USE_DMA_FSMC, LCD_USE_DMA_SPI)
    u8g_SetAddress(u8g, dev, 0);
  #endif

  IO_REG_DATA(ILI9328_HASTART, Ymin);
  IO_REG_DATA(ILI9328_HAEND,   Ymax);
  IO_REG_DATA(ILI9328_VASTART, Xmin);
  IO_REG_DATA(ILI9328_VAEND,   Xmax);

  IO_REG_DATA(ILI9328_HASET,   Ymin);
  IO_REG_DATA(ILI9328_VASET,   Xmin);

  #if HAS_LCD_IO
    tftio.WriteReg(ILI9328_WRITE_RAM);
    tftio.DataTransferEnd();
  #else
    u8g_WriteByte(u8g, dev, ILI9328_WRITE_RAM);
    u8g_SetAddress(u8g, dev, 1);
  #endif
}

static void setWindow_st7789v(u8g_t *u8g, u8g_dev_t *dev, uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax) {
  #if HAS_LCD_IO
    tftio.DataTransferBegin(DATASIZE_8BIT);
    tftio.WriteReg(ST7789V_CASET);
    tftio.WriteData((Xmin >> 8) & 0xFF);
    tftio.WriteData(Xmin & 0xFF);
    tftio.WriteData((Xmax >> 8) & 0xFF);
    tftio.WriteData(Xmax & 0xFF);

    tftio.WriteReg(ST7789V_RASET);
    tftio.WriteData((Ymin >> 8) & 0xFF);
    tftio.WriteData(Ymin & 0xFF);
    tftio.WriteData((Ymax >> 8) & 0xFF);
    tftio.WriteData(Ymax & 0xFF);

    tftio.WriteReg(ST7789V_WRITE_RAM);
    tftio.DataTransferEnd();
  #else
    u8g_SetAddress(u8g, dev, 0); u8g_WriteByte(u8g, dev, ST7789V_CASET); u8g_SetAddress(u8g, dev, 1);
    u8g_WriteByte(u8g, dev, (Xmin >> 8) & 0xFF);
    u8g_WriteByte(u8g, dev, Xmin & 0xFF);
    u8g_WriteByte(u8g, dev, (Xmax >> 8) & 0xFF);
    u8g_WriteByte(u8g, dev, Xmax & 0xFF);

    u8g_SetAddress(u8g, dev, 0); u8g_WriteByte(u8g, dev, ST7789V_RASET); u8g_SetAddress(u8g, dev, 1);
    u8g_WriteByte(u8g, dev, (Ymin >> 8) & 0xFF);
    u8g_WriteByte(u8g, dev, Ymin & 0xFF);
    u8g_WriteByte(u8g, dev, (Ymax >> 8) & 0xFF);
    u8g_WriteByte(u8g, dev, Ymax & 0xFF);

    u8g_SetAddress(u8g, dev, 0); u8g_WriteByte(u8g, dev, ST7789V_WRITE_RAM); u8g_SetAddress(u8g, dev, 1);
  #endif
}

static void setWindow_none(u8g_t *u8g, u8g_dev_t *dev, uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax) {}
void (*setWindow)(u8g_t *u8g, u8g_dev_t *dev, uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax) = setWindow_none;

#define ESC_REG(x)      0xFFFF, 0x00FF & (uint16_t)x
#define ESC_DELAY(x)    0xFFFF, 0x8000 | (x & 0x7FFF)
#define ESC_END         0xFFFF, 0x7FFF
#define ESC_FFFF        0xFFFF, 0xFFFF

#if HAS_LCD_IO
  void writeEscSequence(const uint16_t *sequence) {
    uint16_t data;
    for (;;) {
      data = *sequence++;
      if (data != 0xFFFF) {
        tftio.WriteData(data);
        continue;
      }
      data = *sequence++;
      if (data == 0x7FFF) return;
      if (data == 0xFFFF) {
        tftio.WriteData(data);
      } else if (data & 0x8000) {
        delay(data & 0x7FFF);
      } else if ((data & 0xFF00) == 0) {
        tftio.WriteReg(data);
      }
    }
  }
  #define WRITE_ESC_SEQUENCE(V) writeEscSequence(V)
  #define WRITE_ESC_SEQUENCE16(V) writeEscSequence(V)
#else
  void writeEscSequence8(u8g_t *u8g, u8g_dev_t *dev, const uint16_t *sequence) {
    uint16_t data;
    u8g_SetAddress(u8g, dev, 1);
    for (;;) {
      data = *sequence++;
      if (data != 0xFFFF) {
        u8g_WriteByte(u8g, dev, data & 0xFF);
        continue;
      }
      data = *sequence++;
      if (data == 0x7FFF) return;
      if (data == 0xFFFF) {
        u8g_WriteByte(u8g, dev, data & 0xFF);
      } else if (data & 0x8000) {
        delay(data & 0x7FFF);
      } else if ((data & 0xFF00) == 0) {
        u8g_SetAddress(u8g, dev, 0);
        u8g_WriteByte(u8g, dev, data & 0xFF);
        u8g_SetAddress(u8g, dev, 1);
      }
    }
  }

  #define WRITE_ESC_SEQUENCE(V) writeEscSequence8(u8g, dev, V)

  void writeEscSequence16(u8g_t *u8g, u8g_dev_t *dev, const uint16_t *sequence) {
    uint16_t data;
    u8g_SetAddress(u8g, dev, 0);
    for (;;) {
      data = *sequence++;
      if (data != 0xFFFF) {
        u8g_WriteSequence(u8g, dev, 2, (uint8_t *)&data);
        continue;
      }
      data = *sequence++;
      if (data == 0x7FFF) return;
      if (data == 0xFFFF) {
        u8g_WriteSequence(u8g, dev, 2, (uint8_t *)&data);
      } else if (data & 0x8000) {
        delay(data & 0x7FFF);
      } else if ((data & 0xFF00) == 0) {
        u8g_WriteByte(u8g, dev, data & 0xFF);
      }
    }
    u8g_SetAddress(u8g, dev, 1);
  }

  #define WRITE_ESC_SEQUENCE16(V) writeEscSequence16(u8g, dev, V)
#endif

static const uint16_t st7789v_init[] = {
  ESC_REG(0x0010), ESC_DELAY(10),
  ESC_REG(0x0001), ESC_DELAY(200),
  ESC_REG(0x0011), ESC_DELAY(120),
  ESC_REG(0x0036), TERN(GRAPHICAL_TFT_ROTATE_180, 0x0060, 0x00A0),
  ESC_REG(0x003A), 0x0055,
  ESC_REG(0x002A), 0x0000, 0x0000, 0x0001, 0x003F,
  ESC_REG(0x002B), 0x0000, 0x0000, 0x0000, 0x00EF,
  ESC_REG(0x00B2), 0x000C, 0x000C, 0x0000, 0x0033, 0x0033,
  ESC_REG(0x00B7), 0x0035,
  ESC_REG(0x00BB), 0x001F,
  ESC_REG(0x00C0), 0x002C,
  ESC_REG(0x00C2), 0x0001, 0x00C3,
  ESC_REG(0x00C4), 0x0020,
  ESC_REG(0x00C6), 0x000F,
  ESC_REG(0x00D0), 0x00A4, 0x00A1,
  ESC_REG(0x0029),
  ESC_REG(0x0011),
  ESC_END
};

static const uint16_t ili9328_init[] = {
  ESC_REG(0x0001), 0x0100,
  ESC_REG(0x0002), 0x0400,
  ESC_REG(0x0003), 0x1038,
  ESC_REG(0x0004), 0x0000,
  ESC_REG(0x0008), 0x0202,
  ESC_REG(0x0009), 0x0000,
  ESC_REG(0x000A), 0x0000,
  ESC_REG(0x000C), 0x0000,
  ESC_REG(0x000D), 0x0000,
  ESC_REG(0x000F), 0x0000,
  ESC_REG(0x0010), 0x0000,
  ESC_REG(0x0011), 0x0007,
  ESC_REG(0x0012), 0x0000,
  ESC_REG(0x0013), 0x0000,
  ESC_REG(0x0007), 0x0001,
  ESC_DELAY(200),
  ESC_REG(0x0010), 0x1690,
  ESC_REG(0x0011), 0x0227,
  ESC_DELAY(50),
  ESC_REG(0x0012), 0x008C,
  ESC_DELAY(50),
  ESC_REG(0x0013), 0x1500,
  ESC_REG(0x0029), 0x0004,
  ESC_REG(0x002B), 0x000D,
  ESC_DELAY(50),
  ESC_REG(0x0050), 0x0000,
  ESC_REG(0x0051), 0x00EF,
  ESC_REG(0x0052), 0x0000,
  ESC_REG(0x0053), 0x013F,
  ESC_REG(0x0020), 0x0000,
  ESC_REG(0x0021), 0x0000,
  ESC_REG(0x0060), 0x2700,
  ESC_REG(0x0061), 0x0001,
  ESC_REG(0x006A), 0x0000,
  ESC_REG(0x0080), 0x0000,
  ESC_REG(0x0081), 0x0000,
  ESC_REG(0x0082), 0x0000,
  ESC_REG(0x0083), 0x0000,
  ESC_REG(0x0084), 0x0000,
  ESC_REG(0x0085), 0x0000,
  ESC_REG(0x0090), 0x0010,
  ESC_REG(0x0092), 0x0600,
  ESC_REG(0x0007), 0x0133,
  ESC_REG(0x0022),
  ESC_END
};

static const uint16_t ili9341_init[] = {
  ESC_REG(0x0010), ESC_DELAY(10),
  ESC_REG(0x0001), ESC_DELAY(200),
  ESC_REG(0x0036), TERN(GRAPHICAL_TFT_ROTATE_180, 0x0028, 0x00E8),
  ESC_REG(0x003A), 0x0055,
  ESC_REG(0x002A), 0x0000, 0x0000, 0x0001, 0x003F,
  ESC_REG(0x002B), 0x0000, 0x0000, 0x0000, 0x00EF,
  ESC_REG(0x00C5), 0x003E, 0x0028,
  ESC_REG(0x00C7), 0x0086,
  ESC_REG(0x00B1), 0x0000, 0x0018,
  ESC_REG(0x00C0), 0x0023,
  ESC_REG(0x00C1), 0x0010,
  ESC_REG(0x0029),
  ESC_REG(0x0011),
  ESC_DELAY(100),
  ESC_END
};

static const uint16_t ili9488_init[] = {
  ESC_REG(0x00E0), 0x0000, 0x0007, 0x000F, 0x000D, 0x001B, 0x000A, 0x003C, 0x0078, 0x004A, 0x0007, 0x000E, 0x0009, 0x001B, 0x001E, 0x000F,
  ESC_REG(0x00E1), 0x0000, 0x0022, 0x0024, 0x0006, 0x0012, 0x0007, 0x0036, 0x0047, 0x0047, 0x0006, 0x000A, 0x0007, 0x0030, 0x0037, 0x000F,
  ESC_REG(0x00C0), 0x0010, 0x0010,
  ESC_REG(0x00C1), 0x0041,
  ESC_REG(0x00C5), 0x0000, 0x0022, 0x0080,
  ESC_REG(0x0036), TERN(GRAPHICAL_TFT_ROTATE_180, 0x00A8, 0x0068),
  ESC_REG(0x003A), 0x0055,
  ESC_REG(0x00B0), 0x0000,
  ESC_REG(0x00B1), 0x00B0, 0x0011,
  ESC_REG(0x00B4), 0x0002,
  ESC_REG(0x00B6), 0x0002, 0x0042,
  ESC_REG(0x00B7), 0x00C6,
  ESC_REG(0x00E9), 0x0000,
  ESC_REG(0x00F0), 0x00A9, 0x0051, 0x002C, 0x0082,
  ESC_REG(0x0029),
  ESC_REG(0x0011),
  ESC_DELAY(100),
  ESC_END
};

static const uint16_t st7796_init[] = {
  ESC_REG(0x0010), ESC_DELAY(120),
  ESC_REG(0x0001), ESC_DELAY(120),
  ESC_REG(0x0011), ESC_DELAY(120),
  ESC_REG(0x00F0), 0x00C3,
  ESC_REG(0x00F0), 0x0096,
  ESC_REG(0x0036), TERN(GRAPHICAL_TFT_ROTATE_180, 0x00E8, 0x0028),
  ESC_REG(0x003A), 0x0055,
  ESC_REG(0x00B4), 0x0001,
  ESC_REG(0x00B7), 0x00C6,
  ESC_REG(0x00E8), 0x0040, 0x008A, 0x0000, 0x0000, 0x0029, 0x0019, 0x00A5, 0x0033,
  ESC_REG(0x00C1), 0x0006,
  ESC_REG(0x00C2), 0x00A7,
  ESC_REG(0x00C5), 0x0018,
  ESC_REG(0x00E0), 0x00F0, 0x0009, 0x000B, 0x0006, 0x0004, 0x0015, 0x002F, 0x0054, 0x0042, 0x003C, 0x0017, 0x0014, 0x0018, 0x001B,
  ESC_REG(0x00E1), 0x00F0, 0x0009, 0x000B, 0x0006, 0x0004, 0x0003, 0x002D, 0x0043, 0x0042, 0x003B, 0x0016, 0x0014, 0x0017, 0x001B,
  ESC_REG(0x00F0), 0x003C,
  ESC_REG(0x00F0), 0x0069, ESC_DELAY(120),
  ESC_REG(0x0029),
  ESC_REG(0x0011),
  ESC_DELAY(100),
  ESC_END
};

#if HAS_TOUCH_XPT2046

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

  #define BUTTON_SIZE_X 32
  #define BUTTON_SIZE_Y 20

  // 14, 90, 166, 242, 185 are the original values upscaled 2x.
  #define BUTTOND_X_LO UPSCALE0(14 / 2)
  #define BUTTOND_X_HI (UPSCALE(BUTTOND_X_LO, BUTTON_SIZE_X) - 1)

  #define BUTTONA_X_LO UPSCALE0(90 / 2)
  #define BUTTONA_X_HI (UPSCALE(BUTTONA_X_LO, BUTTON_SIZE_X) - 1)

  #define BUTTONB_X_LO UPSCALE0(166 / 2)
  #define BUTTONB_X_HI (UPSCALE(BUTTONB_X_LO, BUTTON_SIZE_X) - 1)

  #define BUTTONC_X_LO UPSCALE0(242 / 2)
  #define BUTTONC_X_HI (UPSCALE(BUTTONC_X_LO, BUTTON_SIZE_X) - 1)

  #define BUTTON_Y_LO UPSCALE0(140 / 2) + 44 // 184 2x, 254 3x
  #define BUTTON_Y_HI (UPSCALE(BUTTON_Y_LO, BUTTON_SIZE_Y) - 1)

  void drawImage(const uint8_t *data, u8g_t *u8g, u8g_dev_t *dev, uint16_t length, uint16_t height, uint16_t color) {
    uint16_t buffer[BUTTON_SIZE_X * sq(GRAPHICAL_TFT_UPSCALE)];

    if (length > BUTTON_SIZE_X) return;

    for (uint16_t i = 0; i < height; i++) {
      uint16_t k = 0;
      for (uint16_t j = 0; j < length; j++) {
        uint16_t v = TFT_MARLINBG_COLOR;
        if (*(data + (i * (length >> 3) + (j >> 3))) & (0x80 >> (j & 7)))
          v = color;
        else
          v = TFT_MARLINBG_COLOR;
        LOOP_L_N(n, GRAPHICAL_TFT_UPSCALE) buffer[k++] = v;
      }
      #if HAS_LCD_IO
        LOOP_S_L_N(n, 1, GRAPHICAL_TFT_UPSCALE)
          for (uint16_t l = 0; l < UPSCALE0(length); l++)
            buffer[l + n * UPSCALE0(length)] = buffer[l];

        tftio.WriteSequence(buffer, length * sq(GRAPHICAL_TFT_UPSCALE));
      #else
        for (uint8_t i = GRAPHICAL_TFT_UPSCALE; i--;)
          u8g_WriteSequence(u8g, dev, k << 1, (uint8_t*)buffer);
      #endif
    }
  }

#endif // HAS_TOUCH_XPT2046

// Used to fill RGB565 (16bits) background
inline void memset2(const void *ptr, uint16_t fill, size_t cnt) {
  uint16_t* wptr = (uint16_t*)ptr;
  for (size_t i = 0; i < cnt; i += 2) { *wptr = fill; wptr++; }
}

static bool preinit = true;
static uint8_t page;

uint8_t u8g_dev_tft_320x240_upscale_from_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

  #if HAS_LCD_IO
    static uint16_t bufferA[WIDTH * sq(GRAPHICAL_TFT_UPSCALE)], bufferB[WIDTH * sq(GRAPHICAL_TFT_UPSCALE)];
    uint16_t* buffer = &bufferA[0];
  #else
    uint16_t buffer[WIDTH * GRAPHICAL_TFT_UPSCALE]; // 16-bit RGB 565 pixel line buffer
  #endif

  switch (msg) {
    case U8G_DEV_MSG_INIT:
      dev->com_fn(u8g, U8G_COM_MSG_INIT, U8G_SPI_CLK_CYCLE_NONE, &lcd_id);
      tftio.DataTransferBegin(DATASIZE_8BIT);
      switch (lcd_id & 0xFFFF) {
        case 0x8552:   // ST7789V
          WRITE_ESC_SEQUENCE(st7789v_init);
          setWindow = setWindow_st7789v;
          break;
        case 0x9328:  // ILI9328
          WRITE_ESC_SEQUENCE16(ili9328_init);
          setWindow = setWindow_ili9328;
          break;
        case 0x9341:   // ILI9341
          WRITE_ESC_SEQUENCE(ili9341_init);
          setWindow = setWindow_st7789v;
          break;
        case 0x8066:   // Anycubic / TronXY TFTs (480x320)
          WRITE_ESC_SEQUENCE(ili9488_init);
          setWindow = setWindow_st7789v;
          break;
        case 0x7796:
          WRITE_ESC_SEQUENCE(st7796_init);
          setWindow = setWindow_st7789v;
          break;
        case 0x9488:
          WRITE_ESC_SEQUENCE(ili9488_init);
          setWindow = setWindow_st7789v;
        case 0x0404:  // No connected display on FSMC
          lcd_id = 0;
          return 0;
        case 0xFFFF:  // No connected display on SPI
          lcd_id = 0;
          return 0;
        default:
          setWindow = (lcd_id & 0xFF000000) ? setWindow_st7789v : setWindow_ili9328;
          break;
      }
      tftio.DataTransferEnd();

      if (preinit) {
        preinit = false;
        return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
      }

      // Clear Screen
      setWindow(u8g, dev, 0, 0, (TFT_WIDTH) - 1, (TFT_HEIGHT) - 1);
      #if HAS_LCD_IO
        tftio.WriteMultiple(TFT_MARLINBG_COLOR, uint32_t(TFT_WIDTH) * (TFT_HEIGHT));
      #else
        memset2(buffer, TFT_MARLINBG_COLOR, (TFT_WIDTH) / 2);
        for (uint16_t i = 0; i < (TFT_HEIGHT) * sq(GRAPHICAL_TFT_UPSCALE); i++)
          u8g_WriteSequence(u8g, dev, (TFT_WIDTH) / 2, (uint8_t *)buffer);
      #endif

      // Bottom buttons
      #if HAS_TOUCH_XPT2046
        setWindow(u8g, dev, BUTTOND_X_LO, BUTTON_Y_LO, BUTTOND_X_HI, BUTTON_Y_HI);
        drawImage(buttonD, u8g, dev, 32, 20, TFT_BTCANCEL_COLOR);

        setWindow(u8g, dev, BUTTONA_X_LO, BUTTON_Y_LO, BUTTONA_X_HI, BUTTON_Y_HI);
        drawImage(buttonA, u8g, dev, 32, 20, TFT_BTARROWS_COLOR);

        setWindow(u8g, dev, BUTTONB_X_LO, BUTTON_Y_LO, BUTTONB_X_HI, BUTTON_Y_HI);
        drawImage(buttonB, u8g, dev, 32, 20, TFT_BTARROWS_COLOR);

        setWindow(u8g, dev, BUTTONC_X_LO, BUTTON_Y_LO, BUTTONC_X_HI, BUTTON_Y_HI);
        drawImage(buttonC, u8g, dev, 32, 20, TFT_BTOKMENU_COLOR);
      #endif // HAS_TOUCH_XPT2046

      return 0;

    case U8G_DEV_MSG_STOP: preinit = true; break;

    case U8G_DEV_MSG_PAGE_FIRST:
      page = 0;
      setWindow(u8g, dev, TFT_PIXEL_OFFSET_X, TFT_PIXEL_OFFSET_Y, X_HI, Y_HI);
      break;

    case U8G_DEV_MSG_PAGE_NEXT:
      if (++page > (HEIGHT / PAGE_HEIGHT)) return 1;

      LOOP_L_N(y, PAGE_HEIGHT) {
        uint32_t k = 0;
        #if HAS_LCD_IO
          buffer = (y & 1) ? bufferB : bufferA;
        #endif
        for (uint16_t i = 0; i < (uint32_t)pb->width; i++) {
          const uint8_t b = *(((uint8_t *)pb->buf) + i);
          const uint16_t c = TEST(b, y) ? TFT_MARLINUI_COLOR : TFT_MARLINBG_COLOR;
          LOOP_L_N(n, GRAPHICAL_TFT_UPSCALE) buffer[k++] = c;
        }
        #if HAS_LCD_IO
          LOOP_S_L_N(n, 1, GRAPHICAL_TFT_UPSCALE)
            for (uint16_t l = 0; l < UPSCALE0(WIDTH); l++)
              buffer[l + n * UPSCALE0(WIDTH)] = buffer[l];

          tftio.WriteSequence(buffer, COUNT(bufferA));
        #else
          uint8_t* bufptr = (uint8_t*) buffer;
          for (uint8_t i = GRAPHICAL_TFT_UPSCALE; i--;) {
            LOOP_S_L_N(n, 0, GRAPHICAL_TFT_UPSCALE * 2) {
              u8g_WriteSequence(u8g, dev, WIDTH, &bufptr[WIDTH * n]);
            }
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

static uint8_t msgInitCount = 2; // Ignore all messages until 2nd U8G_COM_MSG_INIT

uint8_t u8g_com_hal_tft_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {
  if (msgInitCount) {
    if (msg == U8G_COM_MSG_INIT) msgInitCount--;
    if (msgInitCount) return -1;
  }

  static uint8_t isCommand;

  switch (msg) {
    case U8G_COM_MSG_STOP: break;
    case U8G_COM_MSG_INIT:
      u8g_SetPIOutput(u8g, U8G_PI_RESET);

      u8g_Delay(50);

      tftio.Init();

      if (arg_ptr) {
        *((uint32_t *)arg_ptr) = tftio.GetID();
      }
      isCommand = 0;
      break;

    case U8G_COM_MSG_ADDRESS: // define cmd (arg_val = 0) or data mode (arg_val = 1)
      isCommand = arg_val == 0 ? 1 : 0;
      break;

    case U8G_COM_MSG_RESET:
      u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      tftio.DataTransferBegin(DATASIZE_8BIT);
      if (isCommand)
        tftio.WriteReg(arg_val);
      else
        tftio.WriteData((uint16_t)arg_val);
      tftio.DataTransferEnd();
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      tftio.DataTransferBegin(DATASIZE_16BIT);
      for (uint8_t i = 0; i < arg_val; i += 2)
        tftio.WriteData(*(uint16_t *)(((uint32_t)arg_ptr) + i));
      tftio.DataTransferEnd();
      break;

  }
  return 1;
}

U8G_PB_DEV(u8g_dev_tft_320x240_upscale_from_128x64, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_tft_320x240_upscale_from_128x64_fn, U8G_COM_HAL_TFT_FN);

#endif // HAS_MARLINUI_U8GLIB && FSMC_CS
