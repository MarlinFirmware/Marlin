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
#pragma once

#include "tft_queue.h"
#include "canvas.h"
#include "tft_color.h"
#include "tft_string.h"
#include "tft_image.h"
#include "../tft_io/tft_io.h"

#include "../../inc/MarlinConfig.h"

#if ENABLED(TFT_INTERFACE_FSMC_8BIT)
  // When we have a 8 bit interface, we need to invert the bytes of the color
  #define ENDIAN_COLOR(C) (((C) >> 8) | ((C) << 8))
#else
  #define ENDIAN_COLOR(C) (C)
#endif

#if HAS_UI_320x240
  #define TFT_WIDTH         320
  #define TFT_HEIGHT        240
#elif HAS_UI_480x320
  #define TFT_WIDTH         480
  #define TFT_HEIGHT        320
#elif HAS_UI_480x272
  #define TFT_WIDTH         480
  #define TFT_HEIGHT        272
#elif HAS_UI_1024x600
  #define TFT_WIDTH         1024
  #define TFT_HEIGHT        600
#else
  #error "Unsupported display resolution!"
#endif

#ifndef TFT_BUFFER_SIZE
  #ifdef STM32F103xB
    #define TFT_BUFFER_SIZE       1024
  #elif defined(STM32F103xE)
    #define TFT_BUFFER_SIZE       19200 // 320 * 60
  #elif defined(STM32F1)
    #define TFT_BUFFER_SIZE       8192
  #else
    #define TFT_BUFFER_SIZE       19200 // 320 * 60
  #endif
#endif

#if TFT_BUFFER_SIZE > 65535
  // DMA Count parameter is uint16_t
  #error "TFT_BUFFER_SIZE can not exceed 65535"
#endif

class TFT {
  private:
    static TFT_String string;
    static TFT_IO io;

  public:
    static TFT_Queue queue;

    static uint16_t buffer[TFT_BUFFER_SIZE];

    static void init();
    static void set_font(const uint8_t *Font) { string.set_font(Font); }
    static void add_glyphs(const uint8_t *Font) { string.add_glyphs(Font); }

    static bool is_busy() { return io.isBusy(); }
    static void abort() { io.Abort(); }
    static void write_multiple(uint16_t Data, uint16_t Count) { io.WriteMultiple(Data, Count); }
    static void write_sequence(uint16_t *Data, uint16_t Count) { io.WriteSequence(Data, Count); }
    static void set_window(uint16_t Xmin, uint16_t Ymin, uint16_t Xmax, uint16_t Ymax) { io.set_window(Xmin, Ymin, Xmax, Ymax); }

    static void fill(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) { queue.fill(x, y, width, height, color); }
    static void canvas(uint16_t x, uint16_t y, uint16_t width, uint16_t height) { queue.canvas(x, y, width, height); }
    static void set_background(uint16_t color) { queue.set_background(color); }
    static void add_text(uint16_t x, uint16_t y, uint16_t color, TFT_String tft_string, uint16_t maxWidth = 0) { queue.add_text(x, y, color, tft_string.string(), maxWidth); }
    static void add_text(uint16_t x, uint16_t y, uint16_t color, const char *string, uint16_t maxWidth = 0) { queue.add_text(x, y, color, string, maxWidth); }
    static void add_image(int16_t x, int16_t y, MarlinImage image, uint16_t *colors) { queue.add_image(x, y, image, colors); }
    static void add_image(int16_t x, int16_t y, MarlinImage image, uint16_t color_main = COLOR_WHITE, uint16_t color_background = COLOR_BACKGROUND, uint16_t color_shadow = COLOR_BLACK) { queue.add_image(x, y, image, color_main,  color_background, color_shadow); }
    static void add_bar(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) { queue.add_bar(x, y, width, height, color); }
    static void add_rectangle(uint16_t x, uint16_t y, uint16_t width, uint16_t height, uint16_t color) { queue.add_rectangle(x, y, width, height, color); }
    static void draw_edit_screen_buttons();
};

extern TFT tft;
