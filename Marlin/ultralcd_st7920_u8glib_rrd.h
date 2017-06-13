/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#ifndef ULCDST7920_H
#define ULCDST7920_H

#include "Marlin.h"

#if ENABLED(U8GLIB_ST7920)

  #define ST7920_CLK_PIN  LCD_PINS_D4
  #define ST7920_DAT_PIN  LCD_PINS_ENABLE
  #define ST7920_CS_PIN   LCD_PINS_RS

  //#define PAGE_HEIGHT 8   //128 byte framebuffer
  #define PAGE_HEIGHT 16  //256 byte framebuffer
  //#define PAGE_HEIGHT 32  //512 byte framebuffer

  #define LCD_PIXEL_WIDTH 128
  #define LCD_PIXEL_HEIGHT 64

  #include <U8glib.h>

  //set optimization so ARDUINO optimizes this file
  #pragma GCC optimize (3)

  // If you want you can define your own set of delays in Configuration.h
  //#define ST7920_DELAY_1 DELAY_0_NOP
  //#define ST7920_DELAY_2 DELAY_0_NOP
  //#define ST7920_DELAY_3 DELAY_0_NOP

  #if F_CPU >= 20000000
    #define CPU_ST7920_DELAY_1 DELAY_0_NOP
    #define CPU_ST7920_DELAY_2 DELAY_0_NOP
    #define CPU_ST7920_DELAY_3 DELAY_1_NOP
  #elif (MOTHERBOARD == BOARD_3DRAG) || (MOTHERBOARD == BOARD_K8200) || (MOTHERBOARD == BOARD_K8400)
    #define CPU_ST7920_DELAY_1 DELAY_0_NOP
    #define CPU_ST7920_DELAY_2 DELAY_3_NOP
    #define CPU_ST7920_DELAY_3 DELAY_0_NOP
  #elif (MOTHERBOARD == BOARD_MINIRAMBO)
    #define CPU_ST7920_DELAY_1 DELAY_0_NOP
    #define CPU_ST7920_DELAY_2 DELAY_4_NOP
    #define CPU_ST7920_DELAY_3 DELAY_0_NOP
  #elif (MOTHERBOARD == BOARD_RAMBO)
    #define CPU_ST7920_DELAY_1 DELAY_0_NOP
    #define CPU_ST7920_DELAY_2 DELAY_0_NOP
    #define CPU_ST7920_DELAY_3 DELAY_0_NOP
  #elif F_CPU == 16000000
    #define CPU_ST7920_DELAY_1 DELAY_0_NOP
    #define CPU_ST7920_DELAY_2 DELAY_0_NOP
    #define CPU_ST7920_DELAY_3 DELAY_1_NOP
  #else
    #error "No valid condition for delays in 'ultralcd_st7920_u8glib_rrd.h'"
  #endif

  #ifndef ST7920_DELAY_1
    #define ST7920_DELAY_1 CPU_ST7920_DELAY_1
  #endif
  #ifndef ST7920_DELAY_2
    #define ST7920_DELAY_2 CPU_ST7920_DELAY_2
  #endif
  #ifndef ST7920_DELAY_3
    #define ST7920_DELAY_3 CPU_ST7920_DELAY_3
  #endif

  #define ST7920_SND_BIT \
    WRITE(ST7920_CLK_PIN, LOW);        ST7920_DELAY_1; \
    WRITE(ST7920_DAT_PIN, val & 0x80); ST7920_DELAY_2; \
    WRITE(ST7920_CLK_PIN, HIGH);       ST7920_DELAY_3; \
    val <<= 1

  #if defined(DOGM_SPI_DELAY_US) && DOGM_SPI_DELAY_US > 0
    #define U8G_DELAY() delayMicroseconds(DOGM_SPI_DELAY_US)
  #else
    #define U8G_DELAY() u8g_10MicroDelay()
  #endif

  #define ST7920_CS()              { WRITE(ST7920_CS_PIN,1); U8G_DELAY(); }
  #define ST7920_NCS()             { WRITE(ST7920_CS_PIN,0); }
  #define ST7920_SET_CMD()         { ST7920_SWSPI_SND_8BIT(0xF8); U8G_DELAY(); }
  #define ST7920_SET_DAT()         { ST7920_SWSPI_SND_8BIT(0xFA); U8G_DELAY(); }
  #define ST7920_WRITE_BYTE(a)     { ST7920_SWSPI_SND_8BIT((uint8_t)((a)&0xF0u)); ST7920_SWSPI_SND_8BIT((uint8_t)((a)<<4u)); U8G_DELAY(); }
  #define ST7920_WRITE_BYTES(p,l)  { for (uint8_t i = l + 1; --i;) { ST7920_SWSPI_SND_8BIT(*p&0xF0); ST7920_SWSPI_SND_8BIT(*p<<4); p++; } U8G_DELAY(); }

  extern uint8_t   u8g_dev_st7920_128x64_rrd_buf[(LCD_PIXEL_WIDTH) * (PAGE_HEIGHT) / 8] U8G_NOCOMMON;
  extern u8g_pb_t  u8g_dev_st7920_128x64_rrd_pb;
  extern u8g_dev_t u8g_dev_st7920_128x64_rrd_sw_spi;

  class U8GLIB_ST7920_128X64_RRD : public U8GLIB {
   public:
    U8GLIB_ST7920_128X64_RRD(uint8_t dummy) : U8GLIB(&u8g_dev_st7920_128x64_rrd_sw_spi) { UNUSED(dummy); }
  };

  #pragma GCC reset_options

#endif // U8GLIB_ST7920

#endif // ULCDST7920_H
