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

// NOTE - the HAL version of the rrd device uses a generic ST7920 device.
// See u8g_dev_st7920_128x64_HAL.cpp for the HAL version.

#include "../../inc/MarlinConfigPre.h"

#if ANY(__AVR__, ARDUINO_ARCH_STM32, ARDUINO_ARCH_ESP32)

#include "../../inc/MarlinConfig.h"

#if IS_U8GLIB_ST7920

#include "ultralcd_st7920_u8glib_rrd_AVR.h"

// Optimize this code with -O3
#pragma GCC optimize (3)

#ifndef ST7920_DELAY_1
  #ifndef LCD_ST7920_DELAY_1
    #define LCD_ST7920_DELAY_1 0
  #endif
  #ifndef BOARD_ST7920_DELAY_1
    #define BOARD_ST7920_DELAY_1 0
  #endif
  #ifndef CPU_ST7920_DELAY_1
    #define CPU_ST7920_DELAY_1 0
  #endif
  #if LCD_ST7920_DELAY_1 || BOARD_ST7920_DELAY_1 || CPU_ST7920_DELAY_1
    #define ST7920_DELAY_1 DELAY_NS(_MAX(LCD_ST7920_DELAY_1, BOARD_ST7920_DELAY_1, CPU_ST7920_DELAY_1))
  #else
    #define ST7920_DELAY_1
  #endif
#endif
#ifndef ST7920_DELAY_2
  #ifndef LCD_ST7920_DELAY_2
    #define LCD_ST7920_DELAY_2 0
  #endif
  #ifndef BOARD_ST7920_DELAY_2
    #define BOARD_ST7920_DELAY_2 0
  #endif
  #ifndef CPU_ST7920_DELAY_2
    #define CPU_ST7920_DELAY_2 0
  #endif
  #if LCD_ST7920_DELAY_2 || BOARD_ST7920_DELAY_2 || CPU_ST7920_DELAY_2
    #define ST7920_DELAY_2 DELAY_NS(_MAX(LCD_ST7920_DELAY_2, BOARD_ST7920_DELAY_2, CPU_ST7920_DELAY_2))
  #else
    #define ST7920_DELAY_2
  #endif
#endif
#ifndef ST7920_DELAY_3
  #ifndef LCD_ST7920_DELAY_3
    #define LCD_ST7920_DELAY_3 0
  #endif
  #ifndef BOARD_ST7920_DELAY_3
    #define BOARD_ST7920_DELAY_3 0
  #endif
  #ifndef CPU_ST7920_DELAY_3
    #define CPU_ST7920_DELAY_3 0
  #endif
  #if LCD_ST7920_DELAY_3 || BOARD_ST7920_DELAY_3 || CPU_ST7920_DELAY_3
    #define ST7920_DELAY_3 DELAY_NS(_MAX(LCD_ST7920_DELAY_3, BOARD_ST7920_DELAY_3, CPU_ST7920_DELAY_3))
  #else
    #define ST7920_DELAY_3
  #endif
#endif

#ifdef ARDUINO_ARCH_STM32F1
  #define ST7920_DAT(V) !!((V) & 0x80)
#else
  #define ST7920_DAT(V) ((V) & 0x80)
#endif

#define ST7920_SND_BIT do{ \
  WRITE(ST7920_CLK_PIN, LOW);             ST7920_DELAY_1; \
  WRITE(ST7920_DAT_PIN, ST7920_DAT(val)); ST7920_DELAY_2; \
  WRITE(ST7920_CLK_PIN, HIGH);            ST7920_DELAY_3; \
  val <<= 1; }while(0)

// Optimize this code with -O3
#pragma GCC optimize (3)

void ST7920_SWSPI_SND_8BIT(uint8_t val) {
  ST7920_SND_BIT; // 1
  ST7920_SND_BIT; // 2
  ST7920_SND_BIT; // 3
  ST7920_SND_BIT; // 4
  ST7920_SND_BIT; // 5
  ST7920_SND_BIT; // 6
  ST7920_SND_BIT; // 7
  ST7920_SND_BIT; // 8
}

uint8_t u8g_dev_rrd_st7920_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  uint8_t i, y;
  switch (msg) {
    case U8G_DEV_MSG_INIT: {
      OUT_WRITE(ST7920_CS_PIN, LOW);
      OUT_WRITE(ST7920_DAT_PIN, LOW);
      OUT_WRITE(ST7920_CLK_PIN, HIGH);

      ST7920_CS();
      u8g_Delay(120);                 // Initial delay for boot up
      ST7920_SET_CMD();
      ST7920_WRITE_BYTE(0x20);        // Non-extended mode
      ST7920_WRITE_BYTE(0x08);        // Display off, cursor+blink off
      ST7920_WRITE_BYTE(0x01);        // Clear DDRAM ram
      u8g_Delay(15);                  // Delay for DDRAM clear
      ST7920_WRITE_BYTE(0x24);        // Extended mode
      ST7920_WRITE_BYTE(0x26);        // Extended mode + GDRAM active
      for (y = 0; y < (LCD_PIXEL_HEIGHT) / 2; y++) {  // Clear GDRAM
        ST7920_WRITE_BYTE(0x80 | y);  // Set y
        ST7920_WRITE_BYTE(0x80);      // Set x = 0
        ST7920_SET_DAT();
        for (i = 0; i < 2 * (LCD_PIXEL_WIDTH) / 8; i++) // 2x width clears both segments
          ST7920_WRITE_BYTE(0);
        ST7920_SET_CMD();
      }
      ST7920_WRITE_BYTE(0x0C);        // Display on, cursor+blink off
      ST7920_NCS();
    }
    break;

    case U8G_DEV_MSG_STOP: break;

    case U8G_DEV_MSG_PAGE_NEXT: {
      uint8_t *ptr;
      u8g_pb_t *pb = (u8g_pb_t*)(dev->dev_mem);
      y = pb->p.page_y0;
      ptr = (uint8_t*)pb->buf;

      ST7920_CS();
      for (i = 0; i < PAGE_HEIGHT; i ++) {
        ST7920_SET_CMD();
        if (y < 32) {
          ST7920_WRITE_BYTE(0x80 | y);        // y
          ST7920_WRITE_BYTE(0x80);            // x = 0
        }
        else {
          ST7920_WRITE_BYTE(0x80 | (y - 32)); // y
          ST7920_WRITE_BYTE(0x80 | 8);        // x = 64
        }
        ST7920_SET_DAT();
        ST7920_WRITE_BYTES(ptr, (LCD_PIXEL_WIDTH) / 8); // ptr incremented inside of macro!
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
u8g_pb_t  u8g_dev_st7920_128x64_rrd_pb = { { PAGE_HEIGHT, LCD_PIXEL_HEIGHT, 0, 0, 0 }, LCD_PIXEL_WIDTH, u8g_dev_st7920_128x64_rrd_buf };
u8g_dev_t u8g_dev_st7920_128x64_rrd_sw_spi = { u8g_dev_rrd_st7920_128x64_fn, &u8g_dev_st7920_128x64_rrd_pb, &u8g_com_null_fn };

#pragma GCC reset_options

#if ENABLED(LIGHTWEIGHT_UI)
  #include "../../HAL/shared/HAL_ST7920.h"
  void ST7920_cs()                          { ST7920_CS(); }
  void ST7920_ncs()                         { ST7920_NCS(); }
  void ST7920_set_cmd()                     { ST7920_SET_CMD(); }
  void ST7920_set_dat()                     { ST7920_SET_DAT(); }
  void ST7920_write_byte(const uint8_t val) { ST7920_WRITE_BYTE(val); }
#endif

#endif // IS_U8GLIB_ST7920
#endif // __AVR__ || ARDUINO_ARCH_STM32 || ARDUINO_ARCH_ESP32
