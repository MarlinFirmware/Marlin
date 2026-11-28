/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 * U8g2lib display classes with the subset of the U8glib API used by MarlinUI.
 * Construction only sets up the u8g2 structure. The display is set up by init().
 */

#include "../../../inc/MarlinConfig.h"

#include <U8g2lib.h>
#include <stdio.h>

#ifdef __PLAT_NATIVE_SIM__
  #include "../../../HAL/NATIVE_SIM/u8g2/u8g2_sim_gpio.h"
  #define U8G2_SW_SPI_BYTE_CB u8x8_byte_native_sim_4wire_sw_spi
  #define U8G2_HW_SPI_BYTE_CB u8x8_byte_native_sim_hw_spi
  #define U8G2_GPIO_CB        u8x8_native_sim_gpio_and_delay
#else
  #define U8G2_SW_SPI_BYTE_CB u8x8_byte_arduino_4wire_sw_spi
  #define U8G2_HW_SPI_BYTE_CB u8x8_byte_arduino_hw_spi
  #define U8G2_GPIO_CB        u8x8_gpio_and_delay_arduino
#endif

// U8glib types used by MarlinUI
typedef u8g2_uint_t u8g_uint_t;
typedef u8g2_int_t u8g_int_t;

class U8G2_HAL : public U8G2 {
public:
  u8g2_t* getU8g() { return getU8g2(); }

  void setPrintPos(const u8g2_uint_t x, const u8g2_uint_t y) { setCursor(x, y); }
  u8g2_uint_t getPrintCol() { return tx; }
  u8g2_uint_t getPrintRow() { return ty; }

  // U8g2 reads bitmaps from RAM, so copy each row out of PROGMEM
  void drawBitmapP(const u8g2_uint_t x, u8g2_uint_t y, const u8g2_uint_t cnt, u8g2_uint_t h, const uint8_t *bitmap) {
    uint8_t row[(LCD_PIXEL_WIDTH) / 8];
    if (cnt > COUNT(row)) return;
    for (; h; --h, ++y, bitmap += cnt) {
      memcpy_P(row, bitmap, cnt);
      drawBitmap(x, y, cnt, 1, row);
    }
  }

  #ifndef ARDUINO
    // U8G2 inherits print() from Arduino's Print class
    void print(const char * const s) { tx += drawStr(tx, ty, s); }
    void print(const int i) { char s[12]; sprintf(s, "%d", i); print(s); }
  #endif

  // Draw glyphs and bitmaps transparently, as U8glib does, so they don't erase what they overlap
  bool begin() { const bool ok = U8G2::begin(); setFontMode(1); setBitmapMode(1); return ok; }

  void setRot90()  { setDisplayRotation(U8G2_R1); }
  void setRot180() { setDisplayRotation(U8G2_R2); }
  void setRot270() { setDisplayRotation(U8G2_R3); }
};

// ST7920 128x64, full frame buffer
class U8G2_ST7920_128X64_HAL : public U8G2_HAL {
public:
  // Software SPI
  void init(const uint8_t clock, const uint8_t data, const uint8_t cs, const uint8_t reset=U8X8_PIN_NONE) {
    u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0, U8G2_SW_SPI_BYTE_CB, U8G2_GPIO_CB);
    u8x8_SetPin_3Wire_SW_SPI(getU8x8(), clock, data, cs, reset);
    begin();
  }
  // Hardware SPI
  void init(const uint8_t cs, const uint8_t reset=U8X8_PIN_NONE) {
    u8g2_Setup_st7920_s_128x64_f(&u8g2, U8G2_R0, U8G2_HW_SPI_BYTE_CB, U8G2_GPIO_CB);
    u8x8_SetPin_ST7920_HW_SPI(getU8x8(), cs, reset);
    begin();
  }
};
