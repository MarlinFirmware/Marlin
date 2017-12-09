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

/*
 * PLEASE NOTE >>>
 * We need our custom implementation for Software SPI, as the default implementation
 * of U8GLIB, when running in an ARM based board, is too fast and the display will not
 * recognize commands and/or data at such speeds. This implementation autoderives the
 * required delays to get the maximum possible performance by using the F_CPU macro that
 * specifies the CPU speed. According to the ST7920 datasheet, the maximum SCLK is 1MHz.
 */

#ifndef ULCDST7920_SWSPI_H
#define ULCDST7920_SWSPI_H

#include "../../inc/MarlinConfig.h"

#if ENABLED(U8GLIB_ST7920)

#include <U8glib.h>
#include "HAL_LCD_com_defines.h"

#define ST7920_CLK_PIN  LCD_PINS_D4
#define ST7920_DAT_PIN  LCD_PINS_ENABLE
#define ST7920_CS_PIN   LCD_PINS_RS

//#define PAGE_HEIGHT 8   //128 byte framebuffer
#define PAGE_HEIGHT 16    //256 byte framebuffer
//#define PAGE_HEIGHT 32  //512 byte framebuffer

#define LCD_PIXEL_WIDTH 128
#define LCD_PIXEL_HEIGHT 64

//set optimization so ARDUINO optimizes this file
#pragma GCC optimize (3)

/* ---------------- Delay Cycles routine -------------- */

#ifdef __arm__
/* https://blueprints.launchpad.net/gcc-arm-embedded/+spec/delay-cycles */

#define nop() __asm__ __volatile__("nop;\n\t":::)

FORCE_INLINE static void __delay_4cycles(uint32_t cy) { // +1 cycle
  #if ARCH_PIPELINE_RELOAD_CYCLES<2
    #define EXTRA_NOP_CYCLES "nop"
  #else
    #define EXTRA_NOP_CYCLES ""
  #endif

  __asm__ __volatile__(
    ".syntax unified" "\n\t" // is to prevent CM0,CM1 non-unified syntax

    "loop%=:" "\n\t"
    " subs %[cnt],#1" "\n\t"
    EXTRA_NOP_CYCLES "\n\t"
    " bne loop%=" "\n\t"
    : [cnt]"+r"(cy) // output: +r means input+output
    : // input:
    : "cc" // clobbers:
  );
}

FORCE_INLINE static void DELAY_CYCLES(uint32_t x) {

  if (__builtin_constant_p(x)) {

    #define MAXNOPS 4

    if (x <= (MAXNOPS)) {
      switch(x) { case 4: nop(); case 3: nop(); case 2: nop(); case 1: nop(); }
    }
    else { // because of +1 cycle inside delay_4cycles
      const uint32_t rem = (x - 1) % (MAXNOPS);
      switch(rem) { case 3: nop(); case 2: nop(); case 1: nop(); }
      if ((x = (x - 1) / (MAXNOPS)))
        __delay_4cycles(x); // if need more then 4 nop loop is more optimal
    }
  }
  else
    __delay_4cycles(x / 4);
}

#ifdef __TEST_DELAY

  void calibrateTimer() {

    // Use DWT to calibrate cycles
    uint32_t count = 0;

    // addresses of registers
    volatile uint32_t *DWT_CONTROL = (uint32_t *)0xE0001000,
                      *DWT_CYCCNT = (uint32_t *)0xE0001004,
                      *DEMCR = (uint32_t *)0xE000EDFC;

    cli();

    // enable the use DWT
    *DEMCR = *DEMCR | 0x01000000;

    // Reset cycle counter
    *DWT_CYCCNT = 0;

    // enable cycle counter
    *DWT_CONTROL = *DWT_CONTROL | 1;

    // Perform a delay of 10000 cycles
    DELAY_CYCLES(10000U);

    // number of cycles stored in count variable
    count = *DWT_CYCCNT;

    sei();

    SERIAL_ECHO_START();
    SERIAL_ECHOLNPAIR("calibrated Cycles: ", (int)count);
  }

#endif // __TEST_DELAY

#elif defined(__AVR__)
  #define DELAY_CYCLES(cycles) __builtin_avr_delay_cycles(cycles)
#else
  #error "DELAY_CYCLES not implemented for this architecture."
#endif

/* ---------------- Delay in nanoseconds and in microseconds */

#define DELAY_NS(x) DELAY_CYCLES((x) * (F_CPU / 1000000) / 1000)
#define DELAY_US(x) DELAY_CYCLES((x) * (F_CPU / 1000000))

/* ---------------- ST7920 commands ------------------------ */

#ifdef __arm__

  /* ARM: Plain implementation is more than enough */
  static void ST7920_SWSPI_SND_8BIT(uint8_t val) {
    uint8_t n = 8;
    do {
      WRITE(ST7920_CLK_PIN, LOW);
      WRITE(ST7920_DAT_PIN, val & 0x80);
      DELAY_NS(500);
      WRITE(ST7920_CLK_PIN, HIGH);
      DELAY_NS(500);
      val <<= 1;
    } while (--n);
  }

#else // !ARM

  /* AVR: Unrolling loop makes sense */
  #define ST7920_SND_BIT(nr)              \
    WRITE(ST7920_CLK_PIN, LOW);           \
    WRITE(ST7920_DAT_PIN, TEST(val, nr)); \
    DELAY_NS(500);                        \
    WRITE(ST7920_CLK_PIN, HIGH);          \
    DELAY_NS(500);

  static void ST7920_SWSPI_SND_8BIT(const uint8_t val) {
    ST7920_SND_BIT(7); // MSBit
    ST7920_SND_BIT(6); //
    ST7920_SND_BIT(5); //
    ST7920_SND_BIT(4); //
    ST7920_SND_BIT(3); //
    ST7920_SND_BIT(2); //
    ST7920_SND_BIT(1); //
    ST7920_SND_BIT(0); // LSBit
  }

#endif // !ARM

#define ST7920_CS()              { WRITE(ST7920_CS_PIN,1); DELAY_NS(200); }
#define ST7920_NCS()             { WRITE(ST7920_CS_PIN,0); }
#define ST7920_SET_CMD()         { ST7920_SWSPI_SND_8BIT(0xF8); DELAY_US(3); }
#define ST7920_SET_DAT()         { ST7920_SWSPI_SND_8BIT(0xFA); DELAY_US(3); }
#define ST7920_WRITE_BYTE(a)     { ST7920_SWSPI_SND_8BIT((uint8_t)((a)&0xF0u)); ST7920_SWSPI_SND_8BIT((uint8_t)((a)<<4u)); DELAY_US(3); }
#define ST7920_WRITE_BYTES(p,l)  { for (uint8_t i = l + 1; --i;) { ST7920_SWSPI_SND_8BIT(*p&0xF0); ST7920_SWSPI_SND_8BIT(*p<<4); p++; } DELAY_US(3); }


uint8_t u8g_dev_st7920_custom_sw_spi_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {

  uint8_t i, y;
  switch (msg) {
    case U8G_DEV_MSG_INIT: {

      /* Set to output and write */
      OUT_WRITE(ST7920_CS_PIN, LOW);
      OUT_WRITE(ST7920_DAT_PIN, LOW);
      OUT_WRITE(ST7920_CLK_PIN, HIGH);

      ST7920_CS();
      u8g_Delay(120);                //initial delay for boot up

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

      ST7920_WRITE_BYTE(0x0C);       //display on, cursor+blink off
      ST7920_NCS();
    }
    break;

    case U8G_DEV_MSG_STOP:
      break;

    case U8G_DEV_MSG_PAGE_NEXT: {
      u8g_pb_t* pb = (u8g_pb_t*)(dev->dev_mem);
      y = pb->p.page_y0;
      uint8_t* ptr = (uint8_t*)pb->buf;

      ST7920_CS();
      for (i = 0; i < PAGE_HEIGHT; i++) {
        ST7920_SET_CMD();
        if (y < 32) {
          ST7920_WRITE_BYTE(0x80 | y);   //y
          ST7920_WRITE_BYTE(0x80);       //x=0
        }
        else {
          ST7920_WRITE_BYTE(0x80 | (y - 32)); //y
          ST7920_WRITE_BYTE(0x80 | 8);   //x=64
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

static uint8_t   u8g_dev_st7920_128x64_custom_sw_spi_buf[(LCD_PIXEL_WIDTH) * (PAGE_HEIGHT) / 8] U8G_NOCOMMON;
static u8g_pb_t  u8g_dev_st7920_128x64_custom_sw_spi_pb = {{PAGE_HEIGHT, LCD_PIXEL_HEIGHT, 0, 0, 0}, LCD_PIXEL_WIDTH, u8g_dev_st7920_128x64_custom_sw_spi_buf};
u8g_dev_t u8g_dev_st7920_128x64_custom_sw_spi = {u8g_dev_st7920_custom_sw_spi_128x64_fn, &u8g_dev_st7920_128x64_custom_sw_spi_pb, &u8g_com_null_fn};

#pragma GCC reset_options

#endif // U8GLIB_ST7920
#endif // ULCDST7920_SWSPI_H
