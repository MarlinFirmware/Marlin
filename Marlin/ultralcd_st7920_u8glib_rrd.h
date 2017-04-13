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

static void ST7920_SWSPI_SND_8BIT(uint8_t val) {
  ST7920_SND_BIT; // 1
  ST7920_SND_BIT; // 2
  ST7920_SND_BIT; // 3
  ST7920_SND_BIT; // 4
  ST7920_SND_BIT; // 5
  ST7920_SND_BIT; // 6
  ST7920_SND_BIT; // 7
  ST7920_SND_BIT; // 8
}

#if defined(DOGM_SPI_DELAY_US) && DOGM_SPI_DELAY_US > 0
  #define U8G_DELAY() delayMicroseconds(DOGM_SPI_DELAY_US)
#else
  #define U8G_DELAY() u8g_10MicroDelay()
#endif

#define ST7920_CS()              { WRITE(ST7920_CS_PIN,1); U8G_DELAY(); }
#define ST7920_NCS()             { WRITE(ST7920_CS_PIN,0); }
#define ST7920_SET_CMD()         { ST7920_SWSPI_SND_8BIT(0xf8); U8G_DELAY(); }
#define ST7920_SET_DAT()         { ST7920_SWSPI_SND_8BIT(0xfa); U8G_DELAY(); }
#define ST7920_WRITE_BYTE(a)     { ST7920_SWSPI_SND_8BIT((uint8_t)((a)&0xf0u)); ST7920_SWSPI_SND_8BIT((uint8_t)((a)<<4u)); U8G_DELAY(); }
#define ST7920_WRITE_BYTES(p,l)  { for (uint8_t i = l + 1; --i;) { ST7920_SWSPI_SND_8BIT(*p&0xf0); ST7920_SWSPI_SND_8BIT(*p<<4); p++; } U8G_DELAY(); }

uint8_t u8g_dev_rrd_st7920_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  uint8_t i, y;
  switch (msg) {
    case U8G_DEV_MSG_INIT: {
      OUT_WRITE(ST7920_CS_PIN, LOW);
      OUT_WRITE(ST7920_DAT_PIN, LOW);
      OUT_WRITE(ST7920_CLK_PIN, HIGH);

      ST7920_CS();
      u8g_Delay(120);                 //initial delay for boot up
      ST7920_SET_CMD();
      ST7920_WRITE_BYTE(0x08);       //display off, cursor+blink off
      ST7920_WRITE_BYTE(0x01);       //clear CGRAM ram
      u8g_Delay(15);                 //delay for CGRAM clear
      ST7920_WRITE_BYTE(0x3E);       //extended mode + GDRAM active
      for (y = 0; y < (LCD_PIXEL_HEIGHT) / 2; y++) { //clear GDRAM
        ST7920_WRITE_BYTE(0x80 | y); //set y
        ST7920_WRITE_BYTE(0x80);     //set x = 0
        ST7920_SET_DAT();
        for (i = 0; i < 2 * (LCD_PIXEL_WIDTH) / 8; i++) //2x width clears both segments
          ST7920_WRITE_BYTE(0);
        ST7920_SET_CMD();
      }
      ST7920_WRITE_BYTE(0x0C); //display on, cursor+blink off
      ST7920_NCS();
    }
    break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT: {
      uint8_t* ptr;
      u8g_pb_t* pb = (u8g_pb_t*)(dev->dev_mem);
      y = pb->p.page_y0;
      ptr = (uint8_t*)pb->buf;

      ST7920_CS();
      for (i = 0; i < PAGE_HEIGHT; i ++) {
        ST7920_SET_CMD();
        if (y < 32) {
          ST7920_WRITE_BYTE(0x80 | y);       //y
          ST7920_WRITE_BYTE(0x80);           //x=0
        }
        else {
          ST7920_WRITE_BYTE(0x80 | (y - 32)); //y
          ST7920_WRITE_BYTE(0x80 | 8);       //x=64
        }
        ST7920_SET_DAT();
        ST7920_WRITE_BYTES(ptr, (LCD_PIXEL_WIDTH) / 8); //ptr is incremented inside of macro
        y++;
      }
      ST7920_NCS();
    }
    break;
  }
  #if PAGE_HEIGHT == 8
    return u8g_dev_pb8h1_base_fn(u8g, dev, msg, arg);
  #elif PAGE_HEIGHT == 16
    return u8g_dev_pb16h1_base_fn(u8g, dev, msg, arg);
  #else
    return u8g_dev_pb32h1_base_fn(u8g, dev, msg, arg);
  #endif
}

uint8_t   u8g_dev_st7920_128x64_rrd_buf[(LCD_PIXEL_WIDTH) * (PAGE_HEIGHT) / 8] U8G_NOCOMMON;
u8g_pb_t  u8g_dev_st7920_128x64_rrd_pb = {{PAGE_HEIGHT, LCD_PIXEL_HEIGHT, 0, 0, 0}, LCD_PIXEL_WIDTH, u8g_dev_st7920_128x64_rrd_buf};
u8g_dev_t u8g_dev_st7920_128x64_rrd_sw_spi = {u8g_dev_rrd_st7920_128x64_fn, &u8g_dev_st7920_128x64_rrd_pb, &u8g_com_null_fn};

class U8GLIB_ST7920_128X64_RRD : public U8GLIB {
 public:
  U8GLIB_ST7920_128X64_RRD(uint8_t dummy) : U8GLIB(&u8g_dev_st7920_128x64_rrd_sw_spi) { UNUSED(dummy); }
};

#pragma GCC reset_options

#endif //U8GLIB_ST7920
#endif //ULCDST7920_H
