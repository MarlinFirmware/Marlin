/*

  u8g.h

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
#pragma once

/* uncomment the following line to support displays larger than 240x240 */
//#define U8G_16BIT 1

/* comment the following line to generate more compact but interrupt unsafe code */
#define U8G_INTERRUPT_SAFE 1


#include <stddef.h>

#ifdef __18CXX
typedef unsigned char uint8_t;
typedef signed char int8_t;
typedef unsigned short uint16_t;
typedef signed short int16_t;
#else
#include <stdint.h>
#endif

#ifdef __AVR__
#include <avr/pgmspace.h>
#endif

/*
  use the com interface directly on any systems which are not AVR or ARDUINO
*/
#if defined(__AVR__) || defined(ARDUINO) || defined(__MSP430__) || defined(U8G_HAL_LINKS)
#define U8G_WITH_PINLIST
#endif


#ifdef __cplusplus
extern "C" {
#endif


/*===============================================================*/
#ifdef __GNUC__
#  define U8G_NOINLINE __attribute__((noinline))
#  define U8G_PURE  __attribute__ ((pure))
#  define U8G_NOCOMMON __attribute__ ((nocommon))
#  define U8G_SECTION(name) __attribute__ ((section (name)))
#  if defined(__MSPGCC__)
/* mspgcc does not have .progmem sections. Use -fdata-sections. */
#    define U8G_FONT_SECTION(name)
#  endif
#  if defined(__AVR__)
#    define U8G_FONT_SECTION(name) U8G_SECTION(".progmem." name)
#  endif
#else
#  define U8G_NOINLINE
#  define U8G_PURE
#  define U8G_NOCOMMON
#  define U8G_SECTION(name)
#  define U8G_FONT_SECTION(name)
#endif

#ifdef __MSP430__
/*
  Specifying a section will cause the MSP-GCC to put even const data to RAM
  at least for the fonts. But as the fonts are consts we don't need to specify
  it manually - the MSP-GCC seems to be smart enough to put it into the
  flash memory.
*/
# undef U8G_SECTION
# define U8G_SECTION(name)
#endif

/*===============================================================*/

#ifndef U8G_FONT_SECTION
#  define U8G_FONT_SECTION(name)
#endif


/*===============================================================*/
/* flash memory access */

#ifdef __AVR__
/* U8G_PROGMEM is used by the XBM example */
#define U8G_PROGMEM U8G_SECTION(".progmem.data")
typedef uint8_t PROGMEM u8g_pgm_uint8_t;
typedef uint8_t u8g_fntpgm_uint8_t;
#define u8g_pgm_read(adr) pgm_read_byte_near(adr)
#define U8G_PSTR(s) ((u8g_pgm_uint8_t *)PSTR(s))

#else

#define U8G_PROGMEM
#ifndef PROGMEM
  #define PROGMEM
#endif
typedef uint8_t u8g_pgm_uint8_t;
typedef uint8_t u8g_fntpgm_uint8_t;
#define u8g_pgm_read(adr) (*(const u8g_pgm_uint8_t *)(adr))
#define U8G_PSTR(s) ((u8g_pgm_uint8_t *)(s))

#endif

/*===============================================================*/
/* interrupt safe code */
#ifdef U8G_INTERRUPT_SAFE
#  if defined(__AVR__)
extern uint8_t global_SREG_backup;  /* u8g_state.c */
#    define U8G_ATOMIC_START()    do { global_SREG_backup = SREG; cli(); } while(0)
#    define U8G_ATOMIC_END()      SREG = global_SREG_backup
#    define U8G_ATOMIC_OR(ptr, val)   do { uint8_t tmpSREG = SREG; cli(); (*(ptr) |= (val)); SREG = tmpSREG; } while(0)
#    define U8G_ATOMIC_AND(ptr, val)  do { uint8_t tmpSREG = SREG; cli(); (*(ptr) &= (val)); SREG = tmpSREG; } while(0)
#  else
#    define U8G_ATOMIC_OR(ptr, val) (*(ptr) |= (val))
#    define U8G_ATOMIC_AND(ptr, val) (*(ptr) &= (val))
#    define U8G_ATOMIC_START()
#    define U8G_ATOMIC_END()
#  endif /* __AVR__ */
#else
#  define U8G_ATOMIC_OR(ptr, val) (*(ptr) |= (val))
#  define U8G_ATOMIC_AND(ptr, val) (*(ptr) &= (val))
#  define U8G_ATOMIC_START()
#  define U8G_ATOMIC_END()
#endif /* U8G_INTERRUPT_SAFE */


/*===============================================================*/
/* forward */
typedef struct _u8g_t u8g_t;
typedef struct _u8g_dev_t u8g_dev_t;

typedef struct _u8g_dev_arg_pixel_t u8g_dev_arg_pixel_t;
typedef struct _u8g_dev_arg_bbx_t u8g_dev_arg_bbx_t;
typedef struct _u8g_box_t u8g_box_t;
typedef struct _u8g_dev_arg_irgb_t u8g_dev_arg_irgb_t;


/*===============================================================*/
/* generic */
#ifdef U8G_16BIT
typedef uint16_t u8g_uint_t;
typedef int16_t u8g_int_t;
#else
typedef uint8_t u8g_uint_t;
typedef int8_t u8g_int_t;
#endif

#ifdef OBSOLETE
struct _u8g_box_t
{
  u8g_uint_t x0, y0, x1, y1;
};
typedef struct _u8g_box_t u8g_box_t;
#endif /* OBSOLETE */


/*===============================================================*/
/* device structure */

#ifdef __XC8
/* device prototype */
typedef uint8_t (*u8g_dev_fnptr)(void *u8g, void *dev, uint8_t msg, void *arg);

/* com prototype */
typedef uint8_t (*u8g_com_fnptr)(void *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
#else
/* device prototype */
typedef uint8_t (*u8g_dev_fnptr)(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* com prototype */
typedef uint8_t (*u8g_com_fnptr)(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
#endif



struct _u8g_dev_t
{
  u8g_dev_fnptr dev_fn;         /* device procedure */
  void *dev_mem;                /* device memory */
  u8g_com_fnptr com_fn;         /* communication procedure */
};


/*===============================================================*/
/* device list */

/* Size: 128x64 SDL, u8g_dev_sdl.c */
extern u8g_dev_t u8g_dev_sdl_1bit;
extern u8g_dev_t u8g_dev_sdl_1bit_h;
extern u8g_dev_t u8g_dev_sdl_2bit;
extern u8g_dev_t u8g_dev_sdl_2bit_double_mem;
extern u8g_dev_t u8g_dev_sdl_8bit;
extern u8g_dev_t u8g_dev_sdl_hicolor;
extern u8g_dev_t u8g_dev_sdl_fullcolor;
int u8g_sdl_get_key(void);

/* Size: 70x30 monochrom, stdout */
extern u8g_dev_t u8g_dev_stdout;

/* Size: monochrom, writes "u8g.pbm" */
extern u8g_dev_t u8g_dev_pbm;
extern u8g_dev_t u8g_dev_pbm_8h1;
extern u8g_dev_t u8g_dev_pbm_8h2; /* grayscale simulation */

/* Size: 128x64 monochrom, no output, used for performance measure */
extern u8g_dev_t u8g_dev_gprof;

/* Display: EA DOGS102, Size: 102x64 monochrom */
extern u8g_dev_t u8g_dev_uc1701_dogs102_sw_spi;
extern u8g_dev_t u8g_dev_uc1701_dogs102_hw_spi;

extern u8g_dev_t u8g_dev_uc1701_dogs102_2x_sw_spi;
extern u8g_dev_t u8g_dev_uc1701_dogs102_2x_hw_spi;

/* Display: Mini12864 (dealextreme), Size: 128x64 monochrom */
extern u8g_dev_t u8g_dev_uc1701_mini12864_sw_spi;
extern u8g_dev_t u8g_dev_uc1701_mini12864_hw_spi;

extern u8g_dev_t u8g_dev_uc1701_mini12864_2x_sw_spi;
extern u8g_dev_t u8g_dev_uc1701_mini12864_2x_hw_spi;

/* Display: EA DOGM132, Size: 128x32 monochrom */
extern u8g_dev_t u8g_dev_st7565_dogm132_sw_spi;
extern u8g_dev_t u8g_dev_st7565_dogm132_hw_spi;

/* Display: EA DOGM128, Size: 128x64 monochrom */
extern u8g_dev_t u8g_dev_st7565_dogm128_sw_spi;
extern u8g_dev_t u8g_dev_st7565_dogm128_hw_spi;
extern u8g_dev_t u8g_dev_st7565_dogm128_parallel;

extern u8g_dev_t u8g_dev_st7565_dogm128_2x_sw_spi;
extern u8g_dev_t u8g_dev_st7565_dogm128_2x_hw_spi;
extern u8g_dev_t u8g_dev_st7565_dogm128_2x_parallel;

/* EA DOGM 240-6 */
extern u8g_dev_t u8g_dev_uc1611_dogm240_i2c;
extern u8g_dev_t u8g_dev_uc1611_dogm240_hw_spi;
extern u8g_dev_t u8g_dev_uc1611_dogm240_sw_spi;
extern u8g_dev_t u8g_dev_uc1611_dogm240_8bit;

/* EA DOGXL 240 */
extern u8g_dev_t u8g_dev_uc1611_dogxl240_i2c;
extern u8g_dev_t u8g_dev_uc1611_dogxl240_hw_spi;
extern u8g_dev_t u8g_dev_uc1611_dogxl240_sw_spi;
extern u8g_dev_t u8g_dev_uc1611_dogxl240_8bit;

/* Display: Topway LM6059 128x64 (Adafruit) */
extern u8g_dev_t u8g_dev_st7565_lm6059_sw_spi;
extern u8g_dev_t u8g_dev_st7565_lm6059_hw_spi;
extern u8g_dev_t u8g_dev_st7565_lm6059_2x_sw_spi;
extern u8g_dev_t u8g_dev_st7565_lm6059_2x_hw_spi;
/* Display: Topway LM6063 128x64 */
extern u8g_dev_t u8g_dev_st7565_lm6063_sw_spi;
extern u8g_dev_t u8g_dev_st7565_lm6063_hw_spi;
extern u8g_dev_t u8g_dev_st7565_lm6063_2x_sw_spi;
extern u8g_dev_t u8g_dev_st7565_lm6063_2x_hw_spi;
/* Display: Newhaven NHD-C12864 */
extern u8g_dev_t u8g_dev_st7565_nhd_c12864_sw_spi;
extern u8g_dev_t u8g_dev_st7565_nhd_c12864_hw_spi;
extern u8g_dev_t u8g_dev_st7565_nhd_c12864_2x_sw_spi;
extern u8g_dev_t u8g_dev_st7565_nhd_c12864_2x_hw_spi;

/* Display: Newhaven NHD-C12832 */
extern u8g_dev_t u8g_dev_st7565_nhd_c12832_sw_spi;
extern u8g_dev_t u8g_dev_st7565_nhd_c12832_hw_spi;
extern u8g_dev_t u8g_dev_st7565_nhd_c12832_parallel;
extern u8g_dev_t u8g_dev_st7565_nhd_c12832_hw_usart_spi;

/* Display: Displaytech 64128N */
extern u8g_dev_t u8g_dev_st7565_64128n_sw_spi;
extern u8g_dev_t u8g_dev_st7565_64128n_hw_spi;
extern u8g_dev_t u8g_dev_st7565_64128n_parallel;

extern u8g_dev_t u8g_dev_st7565_64128n_2x_sw_spi;
extern u8g_dev_t u8g_dev_st7565_64128n_2x_hw_spi;
extern u8g_dev_t u8g_dev_st7565_64128n_2x_parallel;

/* Display: LCD-AG-C128032R-DIW W/KK E6 PBF */
extern u8g_dev_t u8g_dev_uc1601_c128032_sw_spi;
extern u8g_dev_t u8g_dev_uc1601_c128032_hw_spi;

extern u8g_dev_t u8g_dev_uc1601_c128032_2x_sw_spi;
extern u8g_dev_t u8g_dev_uc1601_c128032_2x_hw_spi;

/* East Rising/buy-display.com ERC24064-1 */
extern u8g_dev_t u8g_dev_uc1608_240x64_sw_spi;
extern u8g_dev_t u8g_dev_uc1608_240x64_hw_spi;

extern u8g_dev_t u8g_dev_uc1608_240x64_2x_sw_spi;
extern u8g_dev_t u8g_dev_uc1608_240x64_2x_hw_spi;

/* UC1608 240x128 */
extern u8g_dev_t u8g_dev_uc1608_240x128_sw_spi;
extern u8g_dev_t u8g_dev_uc1608_240x128_hw_spi;

extern u8g_dev_t u8g_dev_uc1608_240x128_2x_sw_spi;
extern u8g_dev_t u8g_dev_uc1608_240x128_2x_hw_spi;

/* dfrobot 128x64 Graphic LCD (SKU:FIT0021) */
extern u8g_dev_t u8g_dev_st7920_128x64_sw_spi;
extern u8g_dev_t u8g_dev_st7920_128x64_hw_spi;
extern u8g_dev_t u8g_dev_st7920_128x64_8bit;
extern u8g_dev_t u8g_dev_st7920_128x64_custom;

extern u8g_dev_t u8g_dev_st7920_128x64_4x_sw_spi;
extern u8g_dev_t u8g_dev_st7920_128x64_4x_hw_spi;
extern u8g_dev_t u8g_dev_st7920_128x64_4x_8bit;
extern u8g_dev_t u8g_dev_st7920_128x64_4x_custom;

/* NHD-19232WG */
extern u8g_dev_t u8g_dev_st7920_192x32_sw_spi;
extern u8g_dev_t u8g_dev_st7920_192x32_hw_spi;
extern u8g_dev_t u8g_dev_st7920_192x32_8bit;

extern u8g_dev_t u8g_dev_st7920_192x32_4x_sw_spi;
extern u8g_dev_t u8g_dev_st7920_192x32_4x_hw_spi;
extern u8g_dev_t u8g_dev_st7920_192x32_4x_8bit;

/* CrystalFontz CFAG20232 */
extern u8g_dev_t u8g_dev_st7920_202x32_sw_spi;
extern u8g_dev_t u8g_dev_st7920_202x32_hw_spi;
extern u8g_dev_t u8g_dev_st7920_202x32_8bit;

extern u8g_dev_t u8g_dev_st7920_202x32_4x_sw_spi;
extern u8g_dev_t u8g_dev_st7920_202x32_4x_hw_spi;
extern u8g_dev_t u8g_dev_st7920_202x32_4x_8bit;

/* LC7981 160x80 display */
extern u8g_dev_t u8g_dev_lc7981_160x80_8bit;
/* LC7981 240x64 display */
extern u8g_dev_t u8g_dev_lc7981_240x64_8bit;
/* LC7981 240x128 display */
extern u8g_dev_t u8g_dev_lc7981_240x128_8bit;
/* LC7981 320x64 display */
extern u8g_dev_t u8g_dev_lc7981_320x64_8bit;

/* T6963, all t6963 devices have double page (2x) */
extern u8g_dev_t u8g_dev_t6963_240x128_8bit;
extern u8g_dev_t u8g_dev_t6963_128x128_8bit;
extern u8g_dev_t u8g_dev_t6963_240x64_8bit;
extern u8g_dev_t u8g_dev_t6963_128x64_8bit;

/* Display: EA DOGXL160, Size: 160x104 monochrom & gray level */
extern u8g_dev_t u8g_dev_uc1610_dogxl160_bw_sw_spi;
extern u8g_dev_t u8g_dev_uc1610_dogxl160_bw_hw_spi;
extern u8g_dev_t u8g_dev_uc1610_dogxl160_gr_sw_spi;
extern u8g_dev_t u8g_dev_uc1610_dogxl160_gr_hw_spi;

extern u8g_dev_t u8g_dev_uc1610_dogxl160_2x_bw_sw_spi;
extern u8g_dev_t u8g_dev_uc1610_dogxl160_2x_bw_hw_spi;
extern u8g_dev_t u8g_dev_uc1610_dogxl160_2x_gr_sw_spi;
extern u8g_dev_t u8g_dev_uc1610_dogxl160_2x_gr_hw_spi;

/* Display: Generic KS0108b, Size: 128x64 monochrom */
extern u8g_dev_t u8g_dev_ks0108_128x64;         /* official Arduino Library interface */
extern u8g_dev_t u8g_dev_ks0108_128x64_fast;    /* faster, but uses private tables from the Arduino Library */

/* Nokia 84x48 Display with PCD8544 */
extern u8g_dev_t u8g_dev_pcd8544_84x48_sw_spi;
extern u8g_dev_t u8g_dev_pcd8544_84x48_hw_spi;
extern u8g_dev_t u8g_dev_tls8204_84x48_sw_spi;

/* Nokia 96x65 Display with PCF8812 */
extern u8g_dev_t u8g_dev_pcf8812_96x65_sw_spi;
extern u8g_dev_t u8g_dev_pcf8812_96x65_hw_spi;

/* NHD-2.7-12864UCY3 OLED Display with SSD1325 Controller */
extern u8g_dev_t u8g_dev_ssd1325_nhd27oled_bw_sw_spi;
extern u8g_dev_t u8g_dev_ssd1325_nhd27oled_bw_hw_spi;
extern u8g_dev_t u8g_dev_ssd1325_nhd27oled_bw_parallel;
extern u8g_dev_t u8g_dev_ssd1325_nhd27oled_gr_sw_spi;
extern u8g_dev_t u8g_dev_ssd1325_nhd27oled_gr_hw_spi;

extern u8g_dev_t u8g_dev_ssd1325_nhd27oled_2x_bw_sw_spi;
extern u8g_dev_t u8g_dev_ssd1325_nhd27oled_2x_bw_hw_spi;
extern u8g_dev_t u8g_dev_ssd1325_nhd27oled_2x_bw_parallel;
extern u8g_dev_t u8g_dev_ssd1325_nhd27oled_2x_gr_sw_spi;
extern u8g_dev_t u8g_dev_ssd1325_nhd27oled_2x_gr_hw_spi;

/* LY120 OLED with SSD1327 Controller (tested with Seeedstudio module) */
extern u8g_dev_t u8g_dev_ssd1327_96x96_gr_sw_spi;
extern u8g_dev_t u8g_dev_ssd1327_96x96_gr_hw_spi;
extern u8g_dev_t u8g_dev_ssd1327_96x96_gr_i2c;

extern u8g_dev_t u8g_dev_ssd1327_96x96_2x_gr_sw_spi;
extern u8g_dev_t u8g_dev_ssd1327_96x96_2x_gr_hw_spi;
extern u8g_dev_t u8g_dev_ssd1327_96x96_2x_gr_i2c;

/* NHD-3.12-25664 OLED Display with SSD1322 Controller */
extern u8g_dev_t u8g_dev_ssd1322_nhd31oled_bw_sw_spi;
extern u8g_dev_t u8g_dev_ssd1322_nhd31oled_bw_hw_spi;
extern u8g_dev_t u8g_dev_ssd1322_nhd31oled_bw_parallel;
extern u8g_dev_t u8g_dev_ssd1322_nhd31oled_2x_bw_sw_spi;
extern u8g_dev_t u8g_dev_ssd1322_nhd31oled_2x_bw_hw_spi;

extern u8g_dev_t u8g_dev_ssd1322_nhd31oled_gr_sw_spi;
extern u8g_dev_t u8g_dev_ssd1322_nhd31oled_gr_hw_spi;
extern u8g_dev_t u8g_dev_ssd1322_nhd31oled_gr_parallel;
extern u8g_dev_t u8g_dev_ssd1322_nhd31oled_2x_gr_sw_spi;
extern u8g_dev_t u8g_dev_ssd1322_nhd31oled_2x_gr_hw_spi;

/* OLED 128x64 Display with SSD1306 Controller */
extern u8g_dev_t u8g_dev_ssd1306_128x64_sw_spi;
extern u8g_dev_t u8g_dev_ssd1306_128x64_hw_spi;
extern u8g_dev_t u8g_dev_ssd1306_128x64_i2c;

extern u8g_dev_t u8g_dev_ssd1306_adafruit_128x64_sw_spi;
extern u8g_dev_t u8g_dev_ssd1306_adafruit_128x64_hw_spi;
extern u8g_dev_t u8g_dev_ssd1306_adafruit_128x64_i2c;

extern u8g_dev_t u8g_dev_ssd1306_128x64_2x_sw_spi;
extern u8g_dev_t u8g_dev_ssd1306_128x64_2x_hw_spi;
extern u8g_dev_t u8g_dev_ssd1306_128x64_2x_i2c;

/* OLED 128x64 Display with SH1106 Controller */
extern u8g_dev_t u8g_dev_sh1106_128x64_sw_spi;
extern u8g_dev_t u8g_dev_sh1106_128x64_hw_spi;
extern u8g_dev_t u8g_dev_sh1106_128x64_i2c;

extern u8g_dev_t u8g_dev_sh1106_128x64_2x_sw_spi;
extern u8g_dev_t u8g_dev_sh1106_128x64_2x_hw_spi;
extern u8g_dev_t u8g_dev_sh1106_128x64_2x_i2c;

/* OLED 128x64 Display with SSD1309 Controller */
extern u8g_dev_t u8g_dev_ssd1309_128x64_sw_spi;
extern u8g_dev_t u8g_dev_ssd1309_128x64_hw_spi;
extern u8g_dev_t u8g_dev_ssd1309_128x64_i2c;

/* OLED 128x32 Display with SSD1306 Controller */
extern u8g_dev_t u8g_dev_ssd1306_128x32_sw_spi;
extern u8g_dev_t u8g_dev_ssd1306_128x32_hw_spi;
extern u8g_dev_t u8g_dev_ssd1306_128x32_i2c;

extern u8g_dev_t u8g_dev_ssd1306_128x32_2x_sw_spi;
extern u8g_dev_t u8g_dev_ssd1306_128x32_2x_hw_spi;
extern u8g_dev_t u8g_dev_ssd1306_128x32_2x_i2c;

/* OLED 64x48 Display with SSD1306 Controller */
extern u8g_dev_t u8g_dev_ssd1306_64x48_sw_spi;
extern u8g_dev_t u8g_dev_ssd1306_64x48_hw_spi;
extern u8g_dev_t u8g_dev_ssd1306_64x48_i2c;

extern u8g_dev_t u8g_dev_ssd1306_64x48_2x_sw_spi;
extern u8g_dev_t u8g_dev_ssd1306_64x48_2x_hw_spi;
extern u8g_dev_t u8g_dev_ssd1306_64x48_2x_i2c;

/* OLED 60x32 Display with LD7032 Controller */
extern u8g_dev_t u8g_dev_ld7032_60x32_sw_spi;
extern u8g_dev_t u8g_dev_ld7032_60x32_hw_spi;
extern u8g_dev_t u8g_dev_ld7032_60x32_parallel;

/* experimental 65K TFT with st7687 controller */
extern u8g_dev_t u8g_dev_st7687_c144mvgd_sw_spi;
extern u8g_dev_t u8g_dev_st7687_c144mvgd_8bit;

/* SBN1661/SED1520 display with 122x32 */
extern u8g_dev_t u8g_dev_sbn1661_122x32;

/* flip disc matrix */
extern u8g_dev_t u8g_dev_flipdisc_2x7;
void u8g_SetFlipDiscCallback(u8g_t *u8g, void (*cb)(uint8_t id, uint8_t page, uint8_t width, uint8_t *row1, uint8_t *row2));

/* ILI9325D based TFT */
extern u8g_dev_t u8g_dev_ili9325d_320x240_8bit;


/* SSD1351 OLED (breakout board from http://www.kickstarter.com/projects/ilsoftltd/colour-oled-breakout-board) */
extern u8g_dev_t u8g_dev_ssd1351_128x128_332_sw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128_332_hw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128_4x_332_sw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128_4x_332_hw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128_idx_sw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128_idx_hw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128_hicolor_sw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128_hicolor_hw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128_4x_hicolor_sw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128_4x_hicolor_hw_spi;

/* SSD1351 OLED (Freetronics, GPIOs set to high level) */
extern u8g_dev_t u8g_dev_ssd1351_128x128gh_332_sw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128gh_332_hw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128gh_4x_332_sw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128gh_4x_332_hw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128gh_hicolor_sw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128gh_hicolor_hw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128gh_4x_hicolor_sw_spi;
extern u8g_dev_t u8g_dev_ssd1351_128x128gh_4x_hicolor_hw_spi;


/* SSD1353 OLED Palmtronics */
extern u8g_dev_t u8g_dev_ssd1353_160x128_332_hw_spi;
extern u8g_dev_t u8g_dev_ssd1353_160x128_hicolor_hw_spi;

/* HT1632 */
extern u8g_dev_t u8g_dev_ht1632_24x16;

/* A2 Micro Printer */
extern u8g_dev_t u8g_dev_a2_micro_printer_384x240;
extern u8g_dev_t u8g_dev_a2_micro_printer_192x120_ds;
extern u8g_dev_t u8g_dev_a2_micro_printer_192x360_ds;
extern u8g_dev_t u8g_dev_a2_micro_printer_192x720_ds;

/* u8g_virtual_screen.c  */
extern u8g_dev_t u8g_dev_vs;


/*===============================================================*/
/* device messages */

struct _u8g_dev_arg_pixel_t
{
  u8g_uint_t x, y;              /* will be modified */
  uint8_t pixel;                  /* will be modified, pixel sequence or transparency value */
  uint8_t dir;
  uint8_t color;      /* color or index value, red value for true color mode */
  uint8_t hi_color;   /* high byte for 64K color mode, low byte is in "color", green value for true color mode */
  uint8_t blue;     /* blue value in true color mode */
};
/* typedef struct _u8g_dev_arg_pixel_t u8g_dev_arg_pixel_t; */ /* forward decl */

/* range for r,g,b: 0..255 */
#define U8G_GET_HICOLOR_BY_RGB(r,g,b) (((uint16_t)((r)&0x0f8))<<8)|(((uint16_t)((g)&0x0fc))<<3)|(((uint16_t)((b)>>3)))

struct _u8g_dev_arg_bbx_t
{
  u8g_uint_t x, y, w, h;
};
/* typedef struct _u8g_dev_arg_bbx_t u8g_dev_arg_bbx_t; */ /* forward decl */

struct _u8g_box_t
{
  u8g_uint_t x0, y0, x1, y1;
};
/* typedef struct _u8g_box_t u8g_box_t; */ /* forward decl */

struct _u8g_dev_arg_irgb_t
{
  u8g_uint_t idx, r, g, b;    /* index with rgb value */
};
/* typedef struct _u8g_dev_arg_irgb_t u8g_dev_arg_irgb_t; */ /* forward decl */



#define U8G_DEV_MSG_INIT                10
#define U8G_DEV_MSG_STOP                  11

/* arg: pointer to uint8_t, contranst value between 0 and 255 */
#define U8G_DEV_MSG_CONTRAST            15

#define U8G_DEV_MSG_SLEEP_ON            16
#define U8G_DEV_MSG_SLEEP_OFF            17

#define U8G_DEV_MSG_PAGE_FIRST                  20
#define U8G_DEV_MSG_PAGE_NEXT                    21

/* arg: u8g_dev_arg_bbx_t * */
/* new algorithm with U8G_DEV_MSG_GET_PAGE_BOX makes this msg obsolete */
/* #define U8G_DEV_MSG_IS_BBX_INTERSECTION 22 */

/* arg: u8g_box_t *, fill structure with current page properties */
#define U8G_DEV_MSG_GET_PAGE_BOX 23

/*
#define U8G_DEV_MSG_PRIMITIVE_START             30
#define U8G_DEV_MSG_PRIMITIVE_END               31
*/

/* arg: u8g_dev_arg_pixel_t * */
#define U8G_DEV_MSG_SET_TPIXEL        44
#define U8G_DEV_MSG_SET_4TPIXEL     45

#define U8G_DEV_MSG_SET_PIXEL                           50
#define U8G_DEV_MSG_SET_8PIXEL                          59

#define U8G_DEV_MSG_SET_COLOR_ENTRY                60

#define U8G_DEV_MSG_SET_XY_CB                           61

#define U8G_DEV_MSG_GET_WIDTH                           70
#define U8G_DEV_MSG_GET_HEIGHT                           71
#define U8G_DEV_MSG_GET_MODE                  72

/*===============================================================*/
/* device modes */
#define U8G_MODE(is_index_mode, is_color, bits_per_pixel) (((is_index_mode)<<6) | ((is_color)<<5)|(bits_per_pixel))

#define U8G_MODE_UNKNOWN     0
#define U8G_MODE_BW     U8G_MODE(0, 0, 1)
#define U8G_MODE_GRAY2BIT     U8G_MODE(0, 0, 2)
#define U8G_MODE_R3G3B2  U8G_MODE(0, 1, 8)
#define U8G_MODE_INDEX  U8G_MODE(1, 1, 8)
/* hicolor is R5G6B5 */
#define U8G_MODE_HICOLOR  U8G_MODE(0, 1, 16)
/* truecolor  */
#define U8G_MODE_TRUECOLOR  U8G_MODE(0, 1, 24)


#define U8G_MODE_GET_BITS_PER_PIXEL(mode) ((mode)&31)
#define U8G_MODE_IS_COLOR(mode) (((mode)&32)==0?0:1)
#define U8G_MODE_IS_INDEX_MODE(mode) (((mode)&64)==0?0:1)


/*===============================================================*/
/* com options */

/* uncomment the following line for Atmega HW SPI double speed, issue 89 */
/* #define U8G_HW_SPI_2X 1 */

/* com messages */

#define U8G_COM_MSG_STOP        0
#define U8G_COM_MSG_INIT        1

#define U8G_COM_MSG_ADDRESS 2

/* CHIP_SELECT argument: number of the chip which needs to be activated, so this is more like high active */
#define U8G_COM_MSG_CHIP_SELECT 3

#define U8G_COM_MSG_RESET 4

#define U8G_COM_MSG_WRITE_BYTE 5
#define U8G_COM_MSG_WRITE_SEQ 6
#define U8G_COM_MSG_WRITE_SEQ_P 7


/* com driver */

#ifdef U8G_HAL_LINKS
  #include <LCD_defines.h>
#endif

uint8_t u8g_com_null_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);               /* u8g_com_null.c */

uint8_t u8g_com_std_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr); /* requires U8G_WITH_PINLIST */


uint8_t u8g_com_arduino_std_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);        /* u8g_com_arduino_std_sw_spi.c */
uint8_t u8g_com_arduino_hw_usart_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);      /* u8g_com_atmega_hw_usart_spi.c */
uint8_t u8g_com_arduino_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);        /* u8g_com_arduino_sw_spi.c */
uint8_t u8g_com_arduino_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);          /* u8g_com_arduino_hw_spi.c */
uint8_t u8g_com_arduino_ATtiny85_std_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);          /* u8g_arduino_ATTiny85_std_hw_spi.c */
uint8_t u8g_com_arduino_st7920_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);  /* u8g_com_arduino_st7920_spi.c */
uint8_t u8g_com_arduino_st7920_custom_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr); /* u8g_com_arduino_st7920_custom.c */
uint8_t u8g_com_arduino_st7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);  /* u8g_com_arduino_st7920_hw_spi.c */
uint8_t u8g_com_arduino_parallel_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);           /* u8g_com_arduino_parallel.c */
uint8_t u8g_com_arduino_fast_parallel_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);      /* u8g_com_arduino_fast_parallel.c */
uint8_t u8g_com_arduino_port_d_wr_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);       /* u8g_com_arduino_port_d_wr.c */
uint8_t u8g_com_arduino_no_en_parallel_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr); /* u8g_com_arduino_no_en_parallel.c */
uint8_t u8g_com_arduino_ssd_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);    /* u8g_com_arduino_ssd_i2c.c */
uint8_t u8g_com_arduino_uc_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
uint8_t u8g_com_arduino_t6963_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);      /* u8g_com_arduino_t6963.c */


uint8_t u8g_com_atmega_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);      /* u8g_com_atmega_hw_spi.c */
uint8_t u8g_com_atmega_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);      /* u8g_com_atmega_sw_spi.c */
uint8_t u8g_com_atmega_st7920_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr); /* u8g_com_atmega_st7920_spi.c */
uint8_t u8g_com_atmega_st7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);
uint8_t u8g_com_atmega_parallel_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);    /* u8g_com_atmega_parallel.c */

uint8_t u8g_com_msp430_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);      /* u8g_com_msp430_hw_spi.c */

uint8_t u8g_com_raspberrypi_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);                /* u8g_com_rasperrypi_hw_spi.c */
uint8_t u8g_com_raspberrypi_ssd_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);    /* u8g_com_raspberrypi_ssd_i2c.c */


uint8_t u8g_com_stm32duino_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);     /* u8g_com_stm32duino_hw_spi.cpp */
uint8_t u8g_com_stm32duino_ssd_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);    /* u8g_com_stm32duino_ssd_i2c.cpp */

uint8_t u8g_com_esp32_ssd_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);    /* u8g_com_esp32_ssd_i2c.cpp */

uint8_t u8g_com_samd51_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);                /* u8g_com_samd51_hw_spi.cpp */
uint8_t u8g_com_samd51_st7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr);         /* u8g_com_samd51_st7920_hw_spi.cpp */

/*
  Translation of system specific com drives to generic com names
  At the moment, the following generic com drives are available
  U8G_COM_HW_SPI
  U8G_COM_SW_SPI
  U8G_COM_PARALLEL
  U8G_COM_T6963
  U8G_COM_FAST_PARALLEL
  U8G_COM_SSD_I2C
  U8G_COM_UC_I2C

defined(__18CXX) || defined(__PIC32MX)

*/

/* ==== HW SPI, msp430  ====*/
#ifdef __MSP430__
#define U8G_COM_HW_SPI u8g_com_msp430_hw_spi_fn
#define U8G_COM_ST7920_HW_SPI u8g_com_null_fn
#endif

/* ==== HW SPI, Raspberry PI ====*/
#ifdef U8G_RASPBERRY_PI
#define U8G_COM_HW_SPI u8g_com_raspberrypi_hw_spi_fn
#define U8G_COM_SW_SPI u8g_com_null_fn

/* I'm sure there must be some mad reason for needing this */
#define U8G_COM_ST7920_SW_SPI u8g_com_null_fn
#define U8G_COM_ST7920_HW_SPI u8g_com_null_fn
#endif

/* ==== HW SPI, Arduino ====*/
#ifdef ARDUINO
#ifdef __AVR__

#ifdef __AVR_ATtiny85__
#define U8G_COM_HW_SPI u8g_com_arduino_ATtiny85_std_hw_spi_fn
#define U8G_COM_ST7920_HW_SPI u8g_com_null_fn
#else

#define U8G_COM_HW_SPI u8g_com_arduino_hw_spi_fn
#ifdef __AVR_ATmega32U4__
#define U8G_COM_HW_USART_SPI u8g_com_arduino_hw_usart_spi_fn
#endif /* __AVR_ATmega32U4__ */
#define U8G_COM_ST7920_HW_SPI u8g_com_arduino_st7920_hw_spi_fn
#endif /* __AVR_ATtiny85__ */

#elif defined(__18CXX) || defined(__PIC32MX)
#define U8G_COM_HW_SPI u8g_com_null_fn
#define U8G_COM_ST7920_HW_SPI u8g_com_null_fn
#elif defined(__SAM3X8E__)   /* Arduino Due */
#define U8G_COM_HW_SPI u8g_com_arduino_hw_spi_fn
#define U8G_COM_ST7920_HW_SPI u8g_com_null_fn
#endif
#endif
/* ==== HW SPI, not Arduino ====*/
#ifndef U8G_COM_HW_SPI
#ifdef __AVR__
#define U8G_COM_HW_SPI u8g_com_atmega_hw_spi_fn
#define U8G_COM_ST7920_HW_SPI u8g_com_atmega_st7920_hw_spi_fn
#endif
#endif

#ifndef U8G_COM_HW_SPI
#ifdef ARDUINO_ARCH_STM32
#define U8G_COM_HW_SPI u8g_com_stm32duino_hw_spi_fn
#define U8G_COM_ST7920_HW_SPI u8g_com_null_fn
#endif
#endif

#ifndef U8G_COM_HW_SPI
#ifdef __SAMD51__
#define U8G_COM_HW_SPI u8g_com_samd51_hw_spi_fn
#define U8G_COM_ST7920_HW_SPI u8g_com_samd51_st7920_hw_spi_fn
#endif
#endif

#ifndef U8G_COM_HW_SPI
#define U8G_COM_HW_SPI u8g_com_null_fn
#define U8G_COM_ST7920_HW_SPI u8g_com_null_fn
#endif
#ifndef U8G_COM_HW_USART_SPI
#define U8G_COM_HW_USART_SPI u8g_com_null_fn
#endif


/* ==== SW SPI, Arduino ====*/
#ifdef ARDUINO
#ifdef __AVR__
#define U8G_COM_SW_SPI u8g_com_arduino_sw_spi_fn
#define U8G_COM_ST7920_SW_SPI u8g_com_arduino_st7920_spi_fn
#elif defined(__18CXX) || defined(__PIC32MX)
#define U8G_COM_SW_SPI u8g_com_arduino_sw_spi_fn
#define U8G_COM_ST7920_SW_SPI u8g_com_arduino_st7920_spi_fn
#elif defined(__SAM3X8E__)   /* Arduino Due */
//#define U8G_COM_SW_SPI u8g_com_arduino_std_sw_spi_fn
#define U8G_COM_SW_SPI u8g_com_arduino_sw_spi_fn
#define U8G_COM_ST7920_SW_SPI u8g_com_arduino_st7920_spi_fn
#elif defined(__arm__)   /* Teensy */
#define U8G_COM_SW_SPI u8g_com_arduino_std_sw_spi_fn
#define U8G_COM_ST7920_SW_SPI u8g_com_arduino_st7920_spi_fn
#endif
#endif

#ifndef U8G_COM_SW_SPI
/* ==== SW SPI, not Arduino ====*/

/* ==== SW SPI, msp430  ====*/
#ifdef __MSP430__
#define U8G_COM_SW_SPI u8g_com_std_sw_spi_fn
#define U8G_COM_ST7920_SW_SPI u8g_com_null_fn
#endif

#ifdef __AVR__
#define U8G_COM_SW_SPI u8g_com_atmega_sw_spi_fn
#define U8G_COM_ST7920_SW_SPI u8g_com_atmega_st7920_sw_spi_fn
#endif
#endif
#ifndef U8G_COM_SW_SPI
#define U8G_COM_SW_SPI u8g_com_null_fn
#define U8G_COM_ST7920_SW_SPI u8g_com_null_fn
#endif

/* ==== Parallel interface, Arduino ====*/
#ifdef ARDUINO
#ifdef __AVR__
#define U8G_COM_PARALLEL u8g_com_arduino_parallel_fn
#define U8G_COM_FAST_PARALLEL u8g_com_arduino_fast_parallel_fn
#define U8G_COM_T6963  u8g_com_arduino_t6963_fn
#else /* Arduino Due, Chipkit PIC32 */
#define U8G_COM_PARALLEL u8g_com_arduino_parallel_fn
#define U8G_COM_FAST_PARALLEL u8g_com_arduino_parallel_fn
#define U8G_COM_T6963  u8g_com_null_fn
#endif
#endif
#ifndef U8G_COM_PARALLEL
#ifdef __AVR__
#define U8G_COM_PARALLEL u8g_com_atmega_parallel_fn
#define U8G_COM_FAST_PARALLEL u8g_com_atmega_parallel_fn
#define U8G_COM_T6963  u8g_com_null_fn
#endif
#endif
#ifndef U8G_COM_PARALLEL
#define U8G_COM_PARALLEL u8g_com_null_fn
#define U8G_COM_FAST_PARALLEL u8g_com_null_fn
#define U8G_COM_T6963  u8g_com_null_fn
#endif

#ifdef ARDUINO
#ifdef __AVR__
#define U8G_COM_SSD_I2C u8g_com_arduino_ssd_i2c_fn
#define U8G_COM_UC_I2C u8g_com_arduino_uc_i2c_fn
#endif
#endif

#ifndef U8G_COM_SSD_I2C
#if defined(__AVR__) || defined(__SAM3X8E__)
/* AVR variant and also DUE can use the arduino version at the moment */
#define U8G_COM_SSD_I2C u8g_com_arduino_ssd_i2c_fn
#endif
#endif

#ifndef U8G_COM_SSD_I2C
#ifdef U8G_RASPBERRY_PI
#define U8G_COM_SSD_I2C u8g_com_raspberrypi_ssd_i2c_fn
#endif
#endif

#ifndef U8G_COM_SSD_I2C
#ifdef ARDUINO_ARCH_STM32
#define U8G_COM_SSD_I2C u8g_com_stm32duino_ssd_i2c_fn
#endif
#endif

#ifndef U8G_COM_SSD_I2C
#ifdef ARDUINO_ARCH_ESP32
#define U8G_COM_SSD_I2C u8g_com_esp32_ssd_i2c_fn
#endif
#endif

#ifndef U8G_COM_SSD_I2C
#define U8G_COM_SSD_I2C u8g_com_null_fn
#endif

#ifndef U8G_COM_UC_I2C
#ifdef __AVR__
/* AVR variant can use the arduino version at the moment */
#define U8G_COM_UC_I2C u8g_com_arduino_uc_i2c_fn
#endif
#endif
#ifndef U8G_COM_UC_I2C
#define U8G_COM_UC_I2C u8g_com_null_fn
#endif


/*===============================================================*/
/* com api */

#define U8G_SPI_CLK_CYCLE_50NS 1
#define U8G_SPI_CLK_CYCLE_300NS 2
#define U8G_SPI_CLK_CYCLE_400NS 3
#define U8G_SPI_CLK_CYCLE_NONE 255

uint8_t u8g_InitCom(u8g_t *u8g, u8g_dev_t *dev, uint8_t clk_cycle_time);
void u8g_StopCom(u8g_t *u8g, u8g_dev_t *dev);
void u8g_EnableCom(u8g_t *u8g, u8g_dev_t *dev);         /* obsolete */
void u8g_DisableCom(u8g_t *u8g, u8g_dev_t *dev);        /* obsolete */
void u8g_SetChipSelect(u8g_t *u8g, u8g_dev_t *dev, uint8_t cs);
void u8g_SetResetLow(u8g_t *u8g, u8g_dev_t *dev);
void u8g_SetResetHigh(u8g_t *u8g, u8g_dev_t *dev);
void u8g_SetAddress(u8g_t *u8g, u8g_dev_t *dev, uint8_t address);
uint8_t u8g_WriteByte(u8g_t *u8g, u8g_dev_t *dev, uint8_t val);
uint8_t u8g_WriteSequence(u8g_t *u8g, u8g_dev_t *dev, uint8_t cnt, uint8_t *seq);
uint8_t u8g_WriteSequenceP(u8g_t *u8g, u8g_dev_t *dev, uint8_t cnt, const uint8_t *seq);



#define U8G_ESC_DLY(x) 255, ((x) & 0x7f)
#define U8G_ESC_CS(x) 255, (0xd0 | ((x)&0x0f))
#define U8G_ESC_ADR(x) 255, (0xe0 | ((x)&0x0f))
#define U8G_ESC_RST(x) 255, (0xc0 | ((x)&0x0f))
#define U8G_ESC_VCC(x) 255, (0xbe | ((x)&0x01))
#define U8G_ESC_END 255, 254
#define U8G_ESC_255 255, 255
//uint8_t u8g_WriteEscSeqP(u8g_t *u8g, u8g_dev_t *dev, u8g_pgm_uint8_t *esc_seq);
uint8_t u8g_WriteEscSeqP(u8g_t *u8g, u8g_dev_t *dev, const uint8_t *esc_seq);


/* u8g_com_api_16gr.c */
uint8_t u8g_WriteByteBWTo16GrDevice(u8g_t *u8g, u8g_dev_t *dev, uint8_t b);
uint8_t u8g_WriteSequenceBWTo16GrDevice(u8g_t *u8g, u8g_dev_t *dev, uint8_t cnt, uint8_t *ptr);
uint8_t u8g_WriteByte4LTo16GrDevice(u8g_t *u8g, u8g_dev_t *dev, uint8_t b);
uint8_t u8g_WriteSequence4LTo16GrDevice(u8g_t *u8g, u8g_dev_t *dev, uint8_t cnt, uint8_t *ptr);


/*===============================================================*/
/* u8g_arduino_common.c */
void u8g_com_arduino_digital_write(u8g_t *u8g, uint8_t pin_index, uint8_t value);
void u8g_com_arduino_assign_pin_output_high(u8g_t *u8g);

/*===============================================================*/
/* u8g_com_io.c */

/* create internal number from port and pin */
uint8_t u8g_Pin(uint8_t port, uint8_t bitpos);
#define PN(port,bitpos) u8g_Pin(port,bitpos)

/* low level procedures */
void u8g_SetPinOutput(uint8_t internal_pin_number);
void u8g_SetPinLevel(uint8_t internal_pin_number, uint8_t level);
void u8g_SetPinInput(uint8_t internal_pin_number);
uint8_t u8g_GetPinLevel(uint8_t internal_pin_number);

/* u8g level procedures, expect U8G_PI_xxx macro */
void u8g_SetPIOutput(u8g_t *u8g, uint8_t pi);
void u8g_SetPILevel(u8g_t *u8g, uint8_t pi, uint8_t level);


/*===============================================================*/
/* page */
struct _u8g_page_t
{
  u8g_uint_t page_height;
  u8g_uint_t total_height;
  u8g_uint_t page_y0;
  u8g_uint_t page_y1;
  uint8_t page;
};
typedef struct _u8g_page_t u8g_page_t;

void u8g_page_First(u8g_page_t *p) U8G_NOINLINE;                                                                                        /* u8g_page.c */
void u8g_page_Init(u8g_page_t *p, u8g_uint_t page_height, u8g_uint_t total_height ) U8G_NOINLINE;            /* u8g_page.c */
uint8_t u8g_page_Next(u8g_page_t *p) U8G_NOINLINE;                                                                                   /* u8g_page.c */

/*===============================================================*/
/* page buffer (pb) */

struct _u8g_pb_t
{
  u8g_page_t p;
  u8g_uint_t width;   /* pixel width */
  void *buf;
};
typedef struct _u8g_pb_t u8g_pb_t;


/* u8g_pb.c */
void u8g_pb_Clear(u8g_pb_t *b);
uint8_t u8g_pb_IsYIntersection(u8g_pb_t *pb, u8g_uint_t v0, u8g_uint_t v1);
uint8_t u8g_pb_IsXIntersection(u8g_pb_t *b, u8g_uint_t v0, u8g_uint_t v1);
uint8_t u8g_pb_IsIntersection(u8g_pb_t *pb, u8g_dev_arg_bbx_t *bbx);
void u8g_pb_GetPageBox(u8g_pb_t *pb, u8g_box_t *box);
uint8_t u8g_pb_Is8PixelVisible(u8g_pb_t *b, u8g_dev_arg_pixel_t *arg_pixel);
uint8_t u8g_pb_WriteBuffer(u8g_pb_t *b, u8g_t *u8g, u8g_dev_t *dev);

/*
  note on __attribute__ ((nocommon))
    AVR scripts often use  --gc-sections on the linker to remove unused section.
    This works fine for initialed data and text sections. In principle .bss is also
    handled, but the name##_pb definition is not removed. Reason is, that
    array definitions are placed in the COMMON section, by default
    The attribute "nocommon" removes this automatic assignment to the
    COMMON section and directly puts it into .bss. As a result, if more
    than one buffer is defined in one file, then it will be removed with --gc-sections

    .. not sure if Arduino IDE uses -fno-common... if yes, then the attribute is
    redundant.
*/
#define U8G_PB_DEV(name, width, height, page_height, dev_fn, com_fn) \
uint8_t name##_buf[width] U8G_NOCOMMON ; \
u8g_pb_t name##_pb = { {page_height, height, 0, 0, 0},  width, name##_buf}; \
u8g_dev_t name = { dev_fn, &name##_pb, com_fn }


void u8g_pb8v1_Init(u8g_pb_t *b, void *buf, u8g_uint_t width)   U8G_NOINLINE;
void u8g_pb8v1_Clear(u8g_pb_t *b) U8G_NOINLINE;

uint8_t u8g_pb8v1_IsYIntersection(u8g_pb_t *b, u8g_uint_t v0, u8g_uint_t v1);
uint8_t u8g_pb8v1_IsXIntersection(u8g_pb_t *b, u8g_uint_t v0, u8g_uint_t v1);
uint8_t u8g_pb8v1_WriteBuffer(u8g_pb_t *b, u8g_t *u8g, u8g_dev_t *dev);

uint8_t u8g_dev_pb8v1_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pb16v1.c */
uint8_t u8g_dev_pb16v1_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pb14v1.c */
uint8_t u8g_dev_pb14v1_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pb8v2.c */
uint8_t u8g_dev_pb8v2_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pb16v2.c (double memory of pb8v2) */
uint8_t u8g_dev_pb16v2_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);


/* u8g_pb8h1.c */
uint8_t u8g_dev_pb8h1_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pb16h1.c */
uint8_t u8g_dev_pb16h1_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pb32h1.c */
uint8_t u8g_dev_pb32h1_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);


/* u8g_pb8h2.c 8 pixel rows, byte has horzontal orientation */
uint8_t u8g_dev_pb8h2_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pb16h2.c */
uint8_t u8g_dev_pb16h2_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);



/* u8g_pb8h1f.c */
uint8_t u8g_dev_pb8h1f_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pb8h8.c */
uint8_t u8g_dev_pb8h8_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pbxh16.c */
uint8_t u8g_dev_pbxh16_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

/* u8g_pbxh24.c */
uint8_t u8g_dev_pbxh24_base_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);


/*===============================================================*/
/* u8g_ll_api.c */

/* cursor draw callback */
typedef void (*u8g_draw_cursor_fn)(u8g_t *u8g);

/* vertical reference point calculation callback */
typedef u8g_uint_t (*u8g_font_calc_vref_fnptr)(u8g_t *u8g);

/* state backup and restore procedure */
typedef void (*u8g_state_cb)(uint8_t msg);


/* PI = Pin Index */

/* reset pin, usually optional */
#define U8G_PI_RESET 0

/* address / data or instruction */
#define U8G_PI_A0 1
#define U8G_PI_DI 1

/* chip select line */
#define U8G_PI_CS 2
#define U8G_PI_CS1 2
#define U8G_PI_CS2 3
/* Feb 2013: A0 state moved from 7 to 3 for t6963 controller*/
#define U8G_PI_A0_STATE 3

/* enable / clock signal */
#define U8G_PI_EN 4
#define U8G_PI_CS_STATE 4
#define U8G_PI_SCK 4
#define U8G_PI_SCL 4
#define U8G_PI_RD 4


/* data pins, shared with SPI and I2C pins */
#define U8G_PI_D0 5
#define U8G_PI_MOSI 5
#define U8G_PI_SDA 5
#define U8G_PI_D1 6
#define U8G_PI_MISO 6
#define U8G_PI_D2 7
#define U8G_PI_D3 8
#define U8G_PI_SET_A0 8
#define U8G_PI_D4 9
#define U8G_PI_D5 10
#define U8G_PI_I2C_OPTION 11
#define U8G_PI_D6 11
#define U8G_PI_D7 12

/* read/write pin, must be the last pin in the list, this means U8G_PIN_LIST_LEN =  U8G_PI_RW + 1*/
#define U8G_PI_WR 13
#define U8G_PI_RW 13

#define U8G_PIN_LIST_LEN 14


#define U8G_PIN_DUMMY 254
#define U8G_PIN_NONE 255

#define U8G_FONT_HEIGHT_MODE_TEXT 0
#define U8G_FONT_HEIGHT_MODE_XTEXT 1
#define U8G_FONT_HEIGHT_MODE_ALL 2

struct _u8g_t
{
  u8g_uint_t width;
  u8g_uint_t height;


  u8g_dev_t *dev;               /* first device in the device chain */
  const u8g_pgm_uint8_t *font;             /* regular font for all text procedures */
  const u8g_pgm_uint8_t *cursor_font;  /* special font for cursor procedures */
  uint8_t cursor_fg_color, cursor_bg_color;
  uint8_t cursor_encoding;
  uint8_t mode;                         /* display mode, one of U8G_MODE_xxx */
  u8g_uint_t cursor_x;
  u8g_uint_t cursor_y;
  u8g_draw_cursor_fn cursor_fn;

  int8_t glyph_dx;
  int8_t glyph_x;
  int8_t glyph_y;
  uint8_t glyph_width;
  uint8_t glyph_height;

  u8g_font_calc_vref_fnptr font_calc_vref;
  uint8_t font_height_mode;
  int8_t font_ref_ascent;
  int8_t font_ref_descent;
  uint8_t font_line_spacing_factor;     /* line_spacing = factor * (ascent - descent) / 64 */
  uint8_t line_spacing;

  u8g_dev_arg_pixel_t arg_pixel;
  /* uint8_t color_index; */

#ifdef U8G_WITH_PINLIST
  uint8_t pin_list[U8G_PIN_LIST_LEN];
#endif

  u8g_state_cb state_cb;

  u8g_box_t current_page;   /* current box of the visible page */

};

#define u8g_GetFontAscent(u8g) ((u8g)->font_ref_ascent)
#define u8g_GetFontDescent(u8g) ((u8g)->font_ref_descent)
#define u8g_GetFontLineSpacing(u8g) ((u8g)->line_spacing)

uint8_t u8g_call_dev_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg);

uint8_t u8g_InitLL(u8g_t *u8g, u8g_dev_t *dev);
void u8g_FirstPageLL(u8g_t *u8g, u8g_dev_t *dev);
uint8_t u8g_NextPageLL(u8g_t *u8g, u8g_dev_t *dev);
uint8_t u8g_SetContrastLL(u8g_t *u8g, u8g_dev_t *dev, uint8_t contrast);
void u8g_DrawPixelLL(u8g_t *u8g, u8g_dev_t *dev, u8g_uint_t x, u8g_uint_t y);
void u8g_Draw8PixelLL(u8g_t *u8g, u8g_dev_t *dev, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t pixel);
void u8g_Draw4TPixelLL(u8g_t *u8g, u8g_dev_t *dev, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t pixel);
uint8_t u8g_IsBBXIntersectionLL(u8g_t *u8g, u8g_dev_t *dev, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h);  /* obsolete */
u8g_uint_t u8g_GetWidthLL(u8g_t *u8g, u8g_dev_t *dev);
u8g_uint_t u8g_GetHeightLL(u8g_t *u8g, u8g_dev_t *dev);

void u8g_UpdateDimension(u8g_t *u8g);
uint8_t u8g_Begin(u8g_t *u8g);        /* reset device, put it into default state and call u8g_UpdateDimension() */
uint8_t u8g_Init(u8g_t *u8g, u8g_dev_t *dev);   /* only usefull if the device only as hardcoded ports */
uint8_t u8g_InitComFn(u8g_t *u8g, u8g_dev_t *dev, u8g_com_fnptr com_fn);  /* Init procedure for anything which is not Arduino or AVR (e.g. ARM, but not Due, which is Arduino) */

#ifdef U8G_WITH_PINLIST
uint8_t u8g_InitSPI(u8g_t *u8g, u8g_dev_t *dev, uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset);
uint8_t u8g_InitHWSPI(u8g_t *u8g, u8g_dev_t *dev, uint8_t cs, uint8_t a0, uint8_t reset);
uint8_t u8g_InitI2C(u8g_t *u8g, u8g_dev_t *dev, uint8_t options); /* use U8G_I2C_OPT_NONE as options */
uint8_t u8g_Init8BitFixedPort(u8g_t *u8g, u8g_dev_t *dev, uint8_t en, uint8_t cs, uint8_t di, uint8_t rw, uint8_t reset);
uint8_t u8g_Init8Bit(u8g_t *u8g, u8g_dev_t *dev, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
  uint8_t en, uint8_t cs1, uint8_t cs2, uint8_t di, uint8_t rw, uint8_t reset);
uint8_t u8g_InitRW8Bit(u8g_t *u8g, u8g_dev_t *dev, uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
  uint8_t cs, uint8_t a0, uint8_t wr, uint8_t rd, uint8_t reset);
#endif

void u8g_FirstPage(u8g_t *u8g);
uint8_t u8g_NextPage(u8g_t *u8g);
uint8_t u8g_SetContrast(u8g_t *u8g, uint8_t contrast);
void u8g_SleepOn(u8g_t *u8g);
void u8g_SleepOff(u8g_t *u8g);
void u8g_DrawPixel(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y);
void u8g_Draw8Pixel(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t pixel);
void u8g_Draw4TPixel(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t pixel);
void u8g_Draw8ColorPixel(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t colpixel);

uint8_t u8g_Stop(u8g_t *u8g);
void u8g_SetColorEntry(u8g_t *u8g, uint8_t idx, uint8_t r, uint8_t g, uint8_t b);
void u8g_SetColorIndex(u8g_t *u8g, uint8_t idx);
void u8g_SetHiColor(u8g_t *u8g, uint16_t rgb);
void u8g_SetHiColorByRGB(u8g_t *u8g, uint8_t r, uint8_t g, uint8_t b);
void u8g_SetRGB(u8g_t *u8g, uint8_t r, uint8_t g, uint8_t b);
uint8_t u8g_GetColorIndex(u8g_t *u8g);

uint8_t u8g_GetDefaultForegroundColor(u8g_t *u8g);
void u8g_SetDefaultForegroundColor(u8g_t *u8g);

uint8_t u8g_GetDefaultBackgroundColor(u8g_t *u8g);
void u8g_SetDefaultBackgroundColor(u8g_t *u8g);

uint8_t u8g_GetDefaultMidColor(u8g_t *u8g);
void u8g_SetDefaultMidColor(u8g_t *u8g);

#define u8g_GetWidth(u8g) ((u8g)->width)
#define u8g_GetHeight(u8g) ((u8g)->height)
#define u8g_GetMode(u8g) ((u8g)->mode)
/*
  U8G_MODE_GET_BITS_PER_PIXEL(u8g_GetMode(u8g))
  U8G_MODE_IS_COLOR(u8g_GetMode(u8g))
*/

/* u8g_state.c */
#define U8G_STATE_ENV_IDX 0
#define U8G_STATE_U8G_IDX 1
#define U8G_STATE_RESTORE 0
#define U8G_STATE_BACKUP 1
#define U8G_STATE_MSG_COMPOSE(cmd,idx) (((cmd)<<1) | (idx))

#define U8G_STATE_MSG_RESTORE_ENV U8G_STATE_MSG_COMPOSE(U8G_STATE_RESTORE,U8G_STATE_ENV_IDX)
#define U8G_STATE_MSG_BACKUP_ENV U8G_STATE_MSG_COMPOSE(U8G_STATE_BACKUP,U8G_STATE_ENV_IDX)
#define U8G_STATE_MSG_RESTORE_U8G U8G_STATE_MSG_COMPOSE(U8G_STATE_RESTORE,U8G_STATE_U8G_IDX)
#define U8G_STATE_MSG_BACKUP_U8G U8G_STATE_MSG_COMPOSE(U8G_STATE_BACKUP,U8G_STATE_U8G_IDX)

#define U8G_STATE_MSG_GET_IDX(msg) ((msg)&1)
#define U8G_STATE_MSG_IS_BACKUP(msg) ((msg)&2)



void u8g_state_dummy_cb(uint8_t msg);
void u8g_backup_spi(uint8_t msg);   /* backup SPI state controller */
/* backward compatible definition */
#define u8g_backup_avr_spi u8g_backup_spi

void u8g_SetHardwareBackup(u8g_t *u8g, u8g_state_cb backup_cb);

/* u8g_clip.c */

uint8_t u8g_IsBBXIntersection(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h);


/* u8g_rot.c */

void u8g_UndoRotation(u8g_t *u8g);
void u8g_SetRot90(u8g_t *u8g);
void u8g_SetRot180(u8g_t *u8g);
void u8g_SetRot270(u8g_t *u8g);

/* u8g_scale.c */

void u8g_UndoScale(u8g_t *u8g);
void u8g_SetScale2x2(u8g_t *u8g);


/* u8g_font.c */

size_t u8g_font_GetSize(const void *font);
uint8_t u8g_font_GetFontStartEncoding(const void *font) U8G_NOINLINE;
uint8_t u8g_font_GetFontEndEncoding(const void *font) U8G_NOINLINE;

void u8g_SetFont(u8g_t *u8g, const u8g_fntpgm_uint8_t *font);

uint8_t u8g_GetFontBBXWidth(u8g_t *u8g);
uint8_t u8g_GetFontBBXHeight(u8g_t *u8g);
int8_t u8g_GetFontBBXOffX(u8g_t *u8g);
int8_t u8g_GetFontBBXOffY(u8g_t *u8g);
uint8_t u8g_GetFontCapitalAHeight(u8g_t *u8g);

uint8_t u8g_IsGlyph(u8g_t *u8g, uint8_t requested_encoding);
int8_t u8g_GetGlyphDeltaX(u8g_t *u8g, uint8_t requested_encoding);

int8_t u8g_draw_glyph(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding); /* used by u8g_cursor.c */

int8_t u8g_DrawGlyphDir(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t encoding);
int8_t u8g_DrawGlyph(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding);
int8_t u8g_DrawGlyph90(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding);
int8_t u8g_DrawGlyph180(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding);
int8_t u8g_DrawGlyph270(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t encoding);
int8_t u8g_DrawGlyphFontBBX(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, uint8_t encoding);

u8g_uint_t u8g_DrawStr(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s);
u8g_uint_t u8g_DrawStr90(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s);
u8g_uint_t u8g_DrawStr180(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s);
u8g_uint_t u8g_DrawStr270(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s);

u8g_uint_t u8g_DrawStrDir(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, const char *s);


u8g_uint_t u8g_DrawStrP(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s);
u8g_uint_t u8g_DrawStr90P(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s);
u8g_uint_t u8g_DrawStr180P(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s);
u8g_uint_t u8g_DrawStr270P(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const u8g_pgm_uint8_t *s);


void u8g_SetFontRefHeightText(u8g_t *u8g);
void u8g_SetFontRefHeightExtendedText(u8g_t *u8g);
void u8g_SetFontRefHeightAll(u8g_t *u8g);
void u8g_SetFontLineSpacingFactor(u8g_t *u8g, uint8_t factor);

u8g_uint_t u8g_font_calc_vref_font(u8g_t *u8g);
u8g_uint_t u8g_font_calc_vref_bottom(u8g_t *u8g);
u8g_uint_t u8g_font_calc_vref_top(u8g_t *u8g);
u8g_uint_t u8g_font_calc_vref_center(u8g_t *u8g);

void u8g_SetFontPosBaseline(u8g_t *u8g);
void u8g_SetFontPosBottom(u8g_t *u8g);
void u8g_SetFontPosCenter(u8g_t *u8g);
void u8g_SetFontPosTop(u8g_t *u8g);


u8g_uint_t u8g_GetStrPixelWidth(u8g_t *u8g, const char *s);
u8g_uint_t u8g_GetStrPixelWidthP(u8g_t *u8g, const u8g_pgm_uint8_t *s);
int8_t u8g_GetStrX(u8g_t *u8g, const char *s);
int8_t u8g_GetStrXP(u8g_t *u8g, const u8g_pgm_uint8_t *s);
u8g_uint_t u8g_GetStrWidth(u8g_t *u8g, const char *s) U8G_NOINLINE;
u8g_uint_t u8g_GetStrWidthP(u8g_t *u8g, const u8g_pgm_uint8_t *s);

u8g_uint_t u8g_DrawStrFontBBX(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, uint8_t dir, const char *s);

void u8g_GetStrMinBox(u8g_t *u8g, const char *s, u8g_uint_t *x, u8g_uint_t *y, u8g_uint_t *width, u8g_uint_t *height);
void u8g_GetStrAMinBox(u8g_t *u8g, const char *s, u8g_uint_t *x, u8g_uint_t *y, u8g_uint_t *width, u8g_uint_t *height);


u8g_uint_t u8g_DrawAAStr(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, const char *s);

/* u8g_rect.c */

void u8g_draw_box(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h) U8G_NOINLINE;

void u8g_DrawHLine(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w) U8G_NOINLINE;
void u8g_DrawVLine(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w) U8G_NOINLINE;
void u8g_DrawFrame(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h) U8G_NOINLINE;
void u8g_DrawBox(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h) U8G_NOINLINE;

void u8g_DrawRFrame(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, u8g_uint_t r) U8G_NOINLINE;
void u8g_DrawRBox(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, u8g_uint_t r) U8G_NOINLINE;

/* u8g_bitmap.c */

void u8g_DrawHBitmap(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, const uint8_t *bitmap);
void u8g_DrawHBitmapP(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, const u8g_pgm_uint8_t *bitmap);
void u8g_DrawBitmap(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, const uint8_t *bitmap);
void u8g_DrawBitmapP(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t cnt, u8g_uint_t h, const u8g_pgm_uint8_t *bitmap);

void u8g_DrawXBM(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, const uint8_t *bitmap);
void u8g_DrawXBMP(u8g_t *u8g, u8g_uint_t x, u8g_uint_t y, u8g_uint_t w, u8g_uint_t h, const u8g_pgm_uint8_t *bitmap);


/* u8g_line.c */
void u8g_DrawLine(u8g_t *u8g, u8g_uint_t x1, u8g_uint_t y1, u8g_uint_t x2, u8g_uint_t y2);


/* u8g_circle.c */

/* the following, commented code has been rewritten or is not yet finished
#define U8G_CIRC_UPPER_RIGHT 0x01
#define U8G_CIRC_UPPER_LEFT  0x02
#define U8G_CIRC_LOWER_LEFT 0x04
#define U8G_CIRC_LOWER_RIGHT  0x08
#define U8G_CIRC_ALL (U8G_CIRC_UPPER_RIGHT|U8G_CIRC_UPPER_LEFT|U8G_CIRC_LOWER_RIGHT|U8G_CIRC_LOWER_LEFT)
void u8g_DrawEmpCirc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option);
void u8g_DrawFillCirc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option);
void u8g_DrawEllipseRect(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t x1, u8g_uint_t y1);
*/

#define U8G_DRAW_UPPER_RIGHT 0x01
#define U8G_DRAW_UPPER_LEFT  0x02
#define U8G_DRAW_LOWER_LEFT 0x04
#define U8G_DRAW_LOWER_RIGHT  0x08
#define U8G_DRAW_ALL (U8G_DRAW_UPPER_RIGHT|U8G_DRAW_UPPER_LEFT|U8G_DRAW_LOWER_RIGHT|U8G_DRAW_LOWER_LEFT)

void u8g_draw_circle(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option) U8G_NOINLINE;
void u8g_draw_disc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option) U8G_NOINLINE;

void u8g_DrawCircle(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option);
void u8g_DrawDisc(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rad, uint8_t option);

/* u8g_ellipse.c */
void u8g_DrawEllipse(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rx, u8g_uint_t ry, uint8_t option);
void u8g_DrawFilledEllipse(u8g_t *u8g, u8g_uint_t x0, u8g_uint_t y0, u8g_uint_t rx, u8g_uint_t ry, uint8_t option);

/* u8g_clip.c */
uint8_t u8g_is_box_bbx_intersection(u8g_box_t *box, u8g_dev_arg_bbx_t *bbx);


/* u8g_cursor.c */
void u8g_SetCursorFont(u8g_t *u8g, const u8g_pgm_uint8_t *cursor_font);
void u8g_SetCursorStyle(u8g_t *u8g, uint8_t encoding);
void u8g_SetCursorPos(u8g_t *u8g, u8g_uint_t cursor_x, u8g_uint_t cursor_y);
void u8g_SetCursorColor(u8g_t *u8g, uint8_t fg, uint8_t bg);
void u8g_EnableCursor(u8g_t *u8g);
void u8g_DisableCursor(u8g_t *u8g);
void u8g_DrawCursor(u8g_t *u8g);

/* u8g_polygon.c */

typedef int16_t pg_word_t;

#define PG_NOINLINE U8G_NOINLINE

struct pg_point_struct
{
  pg_word_t x;
  pg_word_t y;
};

typedef struct _pg_struct pg_struct;  /* forward declaration */

struct pg_edge_struct
{
  pg_word_t x_direction;  /* 1, if x2 is greater than x1, -1 otherwise */
  pg_word_t height;
  pg_word_t current_x_offset;
  pg_word_t error_offset;

  /* --- line loop --- */
  pg_word_t current_y;
  pg_word_t max_y;
  pg_word_t current_x;
  pg_word_t error;

  /* --- outer loop --- */
  uint8_t (*next_idx_fn)(pg_struct *pg, uint8_t i);
  uint8_t curr_idx;
};

/* maximum number of points in the polygon */
/* can be redefined, but highest possible value is 254 */
#define PG_MAX_POINTS 6

/* index numbers for the pge structures below */
#define PG_LEFT 0
#define PG_RIGHT 1


struct _pg_struct
{
  struct pg_point_struct list[PG_MAX_POINTS];
  uint8_t cnt;
  uint8_t is_min_y_not_flat;
  pg_word_t total_scan_line_cnt;
  struct pg_edge_struct pge[2]; /* left and right line draw structures */
};

void pg_ClearPolygonXY(pg_struct *pg);
void pg_AddPolygonXY(pg_struct *pg, u8g_t *u8g, int16_t x, int16_t y);
void pg_DrawPolygon(pg_struct *pg, u8g_t *u8g);
void u8g_ClearPolygonXY(void);
void u8g_AddPolygonXY(u8g_t *u8g, int16_t x, int16_t y);
void u8g_DrawPolygon(u8g_t *u8g);
void u8g_DrawTriangle(u8g_t *u8g, int16_t x0, int16_t y0, int16_t x1, int16_t y1, int16_t x2, int16_t y2);


/*===============================================================*/
/* u8g_virtual_screen.c */
void u8g_SetVirtualScreenDimension(u8g_t *vs_u8g, u8g_uint_t width, u8g_uint_t height);
uint8_t u8g_AddToVirtualScreen(u8g_t *vs_u8g, u8g_uint_t x, u8g_uint_t y, u8g_t *child_u8g);

/*===============================================================*/
void st_Draw(uint8_t fps);
void st_Step(uint8_t player_pos, uint8_t is_auto_fire, uint8_t is_fire);

/*===============================================================*/
/* u8g_com_i2c.c */

/* options for u8g_i2c_init() */
#define U8G_I2C_OPT_NONE 0
#define U8G_I2C_OPT_NO_ACK 2
#define U8G_I2C_OPT_DEV_0 0
#define U8G_I2C_OPT_DEV_1 4
#define U8G_I2C_OPT_FAST 16

/* retrun values from u8g_twi_get_error() */
#define U8G_I2C_ERR_NONE 0x00
/* the following values are bit masks */
#define U8G_I2C_ERR_TIMEOUT 0x01
#define U8G_I2C_ERR_BUS 0x02

void u8g_i2c_clear_error(void) U8G_NOINLINE;
uint8_t  u8g_i2c_get_error(void) U8G_NOINLINE;
uint8_t u8g_i2c_get_err_pos(void) U8G_NOINLINE;
void u8g_i2c_init(uint8_t options) U8G_NOINLINE;    /* use U8G_I2C_OPT_NONE as options */
uint8_t u8g_i2c_wait(uint8_t mask, uint8_t pos) U8G_NOINLINE;
uint8_t u8g_i2c_start(uint8_t sla) U8G_NOINLINE;
uint8_t u8g_i2c_send_byte(uint8_t data) U8G_NOINLINE;
uint8_t u8g_i2c_send_mode(uint8_t mode) U8G_NOINLINE;
void u8g_i2c_stop(void) U8G_NOINLINE;


/*===============================================================*/
/* u8g_u8toa.c */
/* v = value, d = number of digits */
const char *u8g_u8toa(uint8_t v, uint8_t d);

/* u8g_u8toa.c */
/* v = value, d = number of digits */
const char *u8g_u16toa(uint16_t v, uint8_t d);

/*===============================================================*/
/* u8g_delay.c */

/* delay by the specified number of milliseconds */
void u8g_Delay(uint16_t val);

/* delay by one microsecond */
void u8g_MicroDelay(void);

/* delay by 10 microseconds */
void u8g_10MicroDelay(void);

/*===============================================================*/
/* chessengine.c */
#define CHESS_KEY_NONE 0
#define CHESS_KEY_NEXT 1
#define CHESS_KEY_PREV 2
#define CHESS_KEY_SELECT 3
#define CHESS_KEY_BACK 4

void chess_Init(u8g_t *u8g, uint8_t empty_body_color);
void chess_Draw(void);
void chess_Step(uint8_t keycode);

/*===============================================================*/
/* font definitions */
extern const u8g_fntpgm_uint8_t u8g_font_m2icon_5[] U8G_FONT_SECTION("u8g_font_m2icon_5");
extern const u8g_fntpgm_uint8_t u8g_font_m2icon_7[] U8G_FONT_SECTION("u8g_font_m2icon_7");
extern const u8g_fntpgm_uint8_t u8g_font_m2icon_9[] U8G_FONT_SECTION("u8g_font_m2icon_9");

extern const u8g_fntpgm_uint8_t u8g_font_u8glib_4[] U8G_FONT_SECTION("u8g_font_u8glib_4");
extern const u8g_fntpgm_uint8_t u8g_font_u8glib_4r[] U8G_FONT_SECTION("u8g_font_u8glib_4r");


extern const u8g_fntpgm_uint8_t u8g_font_6x12_75r[] U8G_FONT_SECTION("u8g_font_6x12_75r");
extern const u8g_fntpgm_uint8_t u8g_font_6x13_75r[] U8G_FONT_SECTION("u8g_font_6x13_75r");
extern const u8g_fntpgm_uint8_t u8g_font_7x13_75r[] U8G_FONT_SECTION("u8g_font_7x13_75r");
extern const u8g_fntpgm_uint8_t u8g_font_8x13_75r[] U8G_FONT_SECTION("u8g_font_8x13_75r");
extern const u8g_fntpgm_uint8_t u8g_font_9x15_75r[] U8G_FONT_SECTION("u8g_font_9x15_75r");
extern const u8g_fntpgm_uint8_t u8g_font_9x18_75r[] U8G_FONT_SECTION("u8g_font_9x18_75r");
extern const u8g_fntpgm_uint8_t u8g_font_cu12_75r[] U8G_FONT_SECTION("u8g_font_cu12_75r");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_75r[] U8G_FONT_SECTION("u8g_font_unifont_75r");
extern const u8g_fntpgm_uint8_t u8g_font_10x20_75r[] U8G_FONT_SECTION("u8g_font_10x20_75r");

extern const u8g_fntpgm_uint8_t u8g_font_10x20_67_75[] U8G_FONT_SECTION("u8g_font_10x20_67_75");
extern const u8g_fntpgm_uint8_t u8g_font_10x20_78_79[] U8G_FONT_SECTION("u8g_font_10x20_78_79");
extern const u8g_fntpgm_uint8_t u8g_font_10x20[] U8G_FONT_SECTION("u8g_font_10x20");
extern const u8g_fntpgm_uint8_t u8g_font_10x20r[] U8G_FONT_SECTION("u8g_font_10x20r");
extern const u8g_fntpgm_uint8_t u8g_font_4x6[] U8G_FONT_SECTION("u8g_font_4x6");
extern const u8g_fntpgm_uint8_t u8g_font_4x6r[] U8G_FONT_SECTION("u8g_font_4x6r");
//extern const u8g_fntpgm_uint8_t u8g_font_4x6n[] U8G_FONT_SECTION("u8g_font_4x6n");
extern const u8g_fntpgm_uint8_t u8g_font_5x7[] U8G_FONT_SECTION("u8g_font_5x7");
extern const u8g_fntpgm_uint8_t u8g_font_5x7r[] U8G_FONT_SECTION("u8g_font_5x7r");
extern const u8g_fntpgm_uint8_t u8g_font_5x8[] U8G_FONT_SECTION("u8g_font_5x8");
extern const u8g_fntpgm_uint8_t u8g_font_5x8r[] U8G_FONT_SECTION("u8g_font_5x8r");
extern const u8g_fntpgm_uint8_t u8g_font_6x10[] U8G_FONT_SECTION("u8g_font_6x10");
extern const u8g_fntpgm_uint8_t u8g_font_6x10r[] U8G_FONT_SECTION("u8g_font_6x10r");
extern const u8g_fntpgm_uint8_t u8g_font_6x12_67_75[] U8G_FONT_SECTION("u8g_font_6x12_67_75");
extern const u8g_fntpgm_uint8_t u8g_font_6x12_78_79[] U8G_FONT_SECTION("u8g_font_6x12_78_79");
extern const u8g_fntpgm_uint8_t u8g_font_6x12[] U8G_FONT_SECTION("u8g_font_6x12");
extern const u8g_fntpgm_uint8_t u8g_font_6x12r[] U8G_FONT_SECTION("u8g_font_6x12r");
extern const u8g_fntpgm_uint8_t u8g_font_6x13_67_75[] U8G_FONT_SECTION("u8g_font_6x13_67_75");
extern const u8g_fntpgm_uint8_t u8g_font_6x13_78_79[] U8G_FONT_SECTION("u8g_font_6x13_78_79");
extern const u8g_fntpgm_uint8_t u8g_font_6x13B[] U8G_FONT_SECTION("u8g_font_6x13B");
extern const u8g_fntpgm_uint8_t u8g_font_6x13Br[] U8G_FONT_SECTION("u8g_font_6x13Br");
extern const u8g_fntpgm_uint8_t u8g_font_6x13[] U8G_FONT_SECTION("u8g_font_6x13");
extern const u8g_fntpgm_uint8_t u8g_font_6x13r[] U8G_FONT_SECTION("u8g_font_6x13r");
extern const u8g_fntpgm_uint8_t u8g_font_6x13O[] U8G_FONT_SECTION("u8g_font_6x13O");
extern const u8g_fntpgm_uint8_t u8g_font_6x13Or[] U8G_FONT_SECTION("u8g_font_6x13Or");
extern const u8g_fntpgm_uint8_t u8g_font_7x13_67_75[] U8G_FONT_SECTION("u8g_font_7x13_67_75");
extern const u8g_fntpgm_uint8_t u8g_font_7x13_78_79[] U8G_FONT_SECTION("u8g_font_7x13_78_79");
extern const u8g_fntpgm_uint8_t u8g_font_7x13B[] U8G_FONT_SECTION("u8g_font_7x13B");
extern const u8g_fntpgm_uint8_t u8g_font_7x13Br[] U8G_FONT_SECTION("u8g_font_7x13Br");
extern const u8g_fntpgm_uint8_t u8g_font_7x13[] U8G_FONT_SECTION("u8g_font_7x13");
extern const u8g_fntpgm_uint8_t u8g_font_7x13r[] U8G_FONT_SECTION("u8g_font_7x13r");
extern const u8g_fntpgm_uint8_t u8g_font_7x13O[] U8G_FONT_SECTION("u8g_font_7x13O");
extern const u8g_fntpgm_uint8_t u8g_font_7x13Or[] U8G_FONT_SECTION("u8g_font_7x13Or");
extern const u8g_fntpgm_uint8_t u8g_font_7x14B[] U8G_FONT_SECTION("u8g_font_7x14B");
extern const u8g_fntpgm_uint8_t u8g_font_7x14Br[] U8G_FONT_SECTION("u8g_font_7x14Br");
extern const u8g_fntpgm_uint8_t u8g_font_7x14[] U8G_FONT_SECTION("u8g_font_7x14");
extern const u8g_fntpgm_uint8_t u8g_font_7x14r[] U8G_FONT_SECTION("u8g_font_7x14r");
extern const u8g_fntpgm_uint8_t u8g_font_8x13_67_75[] U8G_FONT_SECTION("u8g_font_8x13_67_75");
extern const u8g_fntpgm_uint8_t u8g_font_8x13B[] U8G_FONT_SECTION("u8g_font_8x13B");
extern const u8g_fntpgm_uint8_t u8g_font_8x13Br[] U8G_FONT_SECTION("u8g_font_8x13Br");
extern const u8g_fntpgm_uint8_t u8g_font_8x13[] U8G_FONT_SECTION("u8g_font_8x13");
extern const u8g_fntpgm_uint8_t u8g_font_8x13r[] U8G_FONT_SECTION("u8g_font_8x13r");
extern const u8g_fntpgm_uint8_t u8g_font_8x13O[] U8G_FONT_SECTION("u8g_font_8x13O");
extern const u8g_fntpgm_uint8_t u8g_font_8x13Or[] U8G_FONT_SECTION("u8g_font_8x13Or");

extern const u8g_fntpgm_uint8_t u8g_font_9x15_67_75[] U8G_FONT_SECTION("u8g_font_9x15_67_75");
extern const u8g_fntpgm_uint8_t u8g_font_9x15_78_79[] U8G_FONT_SECTION("u8g_font_9x15_78_79");
extern const u8g_fntpgm_uint8_t u8g_font_9x15B[] U8G_FONT_SECTION("u8g_font_9x15B");
extern const u8g_fntpgm_uint8_t u8g_font_9x15Br[] U8G_FONT_SECTION("u8g_font_9x15Br");
extern const u8g_fntpgm_uint8_t u8g_font_9x15[] U8G_FONT_SECTION("u8g_font_9x15");
extern const u8g_fntpgm_uint8_t u8g_font_9x15r[] U8G_FONT_SECTION("u8g_font_9x15r");

extern const u8g_fntpgm_uint8_t u8g_font_9x18_67_75[] U8G_FONT_SECTION("u8g_font_9x18_67_75");
extern const u8g_fntpgm_uint8_t u8g_font_9x18_78_79[] U8G_FONT_SECTION("u8g_font_9x18_78_79");
extern const u8g_fntpgm_uint8_t u8g_font_9x18B[] U8G_FONT_SECTION("u8g_font_9x18B");
extern const u8g_fntpgm_uint8_t u8g_font_9x18[] U8G_FONT_SECTION("u8g_font_9x18");
extern const u8g_fntpgm_uint8_t u8g_font_9x18Br[] U8G_FONT_SECTION("u8g_font_9x18Br");
extern const u8g_fntpgm_uint8_t u8g_font_9x18r[] U8G_FONT_SECTION("u8g_font_9x18r");

extern const u8g_fntpgm_uint8_t u8g_font_cursor[] U8G_FONT_SECTION("u8g_font_cursor");
extern const u8g_fntpgm_uint8_t u8g_font_cursorr[] U8G_FONT_SECTION("u8g_font_cursorr");
extern const u8g_fntpgm_uint8_t u8g_font_micro[] U8G_FONT_SECTION("u8g_font_micro");

extern const u8g_fntpgm_uint8_t u8g_font_cu12_67_75[] U8G_FONT_SECTION("u8g_font_cu12_67_75");
extern const u8g_fntpgm_uint8_t u8g_font_cu12_78_79[] U8G_FONT_SECTION("u8g_font_cu12_78_79");
extern const u8g_fntpgm_uint8_t u8g_font_cu12[] U8G_FONT_SECTION("u8g_font_cu12");

/*
  Free-Universal Bold
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_fub11[] U8G_FONT_SECTION("u8g_font_fub11");
extern const u8g_fntpgm_uint8_t u8g_font_fub11r[] U8G_FONT_SECTION("u8g_font_fub11r");
extern const u8g_fntpgm_uint8_t u8g_font_fub11n[] U8G_FONT_SECTION("u8g_font_fub11n");
extern const u8g_fntpgm_uint8_t u8g_font_fub14[] U8G_FONT_SECTION("u8g_font_fub14");
extern const u8g_fntpgm_uint8_t u8g_font_fub14r[] U8G_FONT_SECTION("u8g_font_fub14r");
extern const u8g_fntpgm_uint8_t u8g_font_fub14n[] U8G_FONT_SECTION("u8g_font_fub14n");
extern const u8g_fntpgm_uint8_t u8g_font_fub17[] U8G_FONT_SECTION("u8g_font_fub17");
extern const u8g_fntpgm_uint8_t u8g_font_fub17r[] U8G_FONT_SECTION("u8g_font_fub17r");
extern const u8g_fntpgm_uint8_t u8g_font_fub17n[] U8G_FONT_SECTION("u8g_font_fub17n");
extern const u8g_fntpgm_uint8_t u8g_font_fub20[] U8G_FONT_SECTION("u8g_font_fub20");
extern const u8g_fntpgm_uint8_t u8g_font_fub20r[] U8G_FONT_SECTION("u8g_font_fub20r");
extern const u8g_fntpgm_uint8_t u8g_font_fub20n[] U8G_FONT_SECTION("u8g_font_fub20n");
extern const u8g_fntpgm_uint8_t u8g_font_fub20t[] U8G_FONT_SECTION("u8g_font_fub20t");
extern const u8g_fntpgm_uint8_t u8g_font_fub25[] U8G_FONT_SECTION("u8g_font_fub25");
extern const u8g_fntpgm_uint8_t u8g_font_fub25r[] U8G_FONT_SECTION("u8g_font_fub25r");
extern const u8g_fntpgm_uint8_t u8g_font_fub25n[] U8G_FONT_SECTION("u8g_font_fub25n");
extern const u8g_fntpgm_uint8_t u8g_font_fub30[] U8G_FONT_SECTION("u8g_font_fub30");
extern const u8g_fntpgm_uint8_t u8g_font_fub30r[] U8G_FONT_SECTION("u8g_font_fub30r");
extern const u8g_fntpgm_uint8_t u8g_font_fub30n[] U8G_FONT_SECTION("u8g_font_fub30n");
extern const u8g_fntpgm_uint8_t u8g_font_fub35n[] U8G_FONT_SECTION("u8g_font_fub35n");
extern const u8g_fntpgm_uint8_t u8g_font_fub42n[] U8G_FONT_SECTION("u8g_font_fub42n");
extern const u8g_fntpgm_uint8_t u8g_font_fub49n[] U8G_FONT_SECTION("u8g_font_fub49n");

/*
  Free-Universal Regular
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_fur11[] U8G_FONT_SECTION("u8g_font_fur11");
extern const u8g_fntpgm_uint8_t u8g_font_fur11r[] U8G_FONT_SECTION("u8g_font_fur11r");
extern const u8g_fntpgm_uint8_t u8g_font_fur11n[] U8G_FONT_SECTION("u8g_font_fur11n");
extern const u8g_fntpgm_uint8_t u8g_font_fur14[] U8G_FONT_SECTION("u8g_font_fur14");
extern const u8g_fntpgm_uint8_t u8g_font_fur14r[] U8G_FONT_SECTION("u8g_font_fur14r");
extern const u8g_fntpgm_uint8_t u8g_font_fur14n[] U8G_FONT_SECTION("u8g_font_fur14n");
extern const u8g_fntpgm_uint8_t u8g_font_fur17[] U8G_FONT_SECTION("u8g_font_fur17");
extern const u8g_fntpgm_uint8_t u8g_font_fur17r[] U8G_FONT_SECTION("u8g_font_fur17r");
extern const u8g_fntpgm_uint8_t u8g_font_fur17n[] U8G_FONT_SECTION("u8g_font_fur17n");
extern const u8g_fntpgm_uint8_t u8g_font_fur20[] U8G_FONT_SECTION("u8g_font_fur20");
extern const u8g_fntpgm_uint8_t u8g_font_fur20r[] U8G_FONT_SECTION("u8g_font_fur20r");
extern const u8g_fntpgm_uint8_t u8g_font_fur20n[] U8G_FONT_SECTION("u8g_font_fur20n");
extern const u8g_fntpgm_uint8_t u8g_font_fur25[] U8G_FONT_SECTION("u8g_font_fur25");
extern const u8g_fntpgm_uint8_t u8g_font_fur25r[] U8G_FONT_SECTION("u8g_font_fur25r");
extern const u8g_fntpgm_uint8_t u8g_font_fur25n[] U8G_FONT_SECTION("u8g_font_fur25n");
extern const u8g_fntpgm_uint8_t u8g_font_fur30[] U8G_FONT_SECTION("u8g_font_fur30");
extern const u8g_fntpgm_uint8_t u8g_font_fur30r[] U8G_FONT_SECTION("u8g_font_fur30r");
extern const u8g_fntpgm_uint8_t u8g_font_fur30n[] U8G_FONT_SECTION("u8g_font_fur30n");
extern const u8g_fntpgm_uint8_t u8g_font_fur35n[] U8G_FONT_SECTION("u8g_font_fur35n");
extern const u8g_fntpgm_uint8_t u8g_font_fur42n[] U8G_FONT_SECTION("u8g_font_fur42n");
extern const u8g_fntpgm_uint8_t u8g_font_fur49n[] U8G_FONT_SECTION("u8g_font_fur49n");

/*
  Gentium Bold
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_gdb11[] U8G_FONT_SECTION("u8g_font_gdb11");
extern const u8g_fntpgm_uint8_t u8g_font_gdb12[] U8G_FONT_SECTION("u8g_font_gdb12");
extern const u8g_fntpgm_uint8_t u8g_font_gdb14[] U8G_FONT_SECTION("u8g_font_gdb14");
extern const u8g_fntpgm_uint8_t u8g_font_gdb17[] U8G_FONT_SECTION("u8g_font_gdb17");
extern const u8g_fntpgm_uint8_t u8g_font_gdb20[] U8G_FONT_SECTION("u8g_font_gdb20");
extern const u8g_fntpgm_uint8_t u8g_font_gdb25[] U8G_FONT_SECTION("u8g_font_gdb25");
extern const u8g_fntpgm_uint8_t u8g_font_gdb30[] U8G_FONT_SECTION("u8g_font_gdb30");

extern const u8g_fntpgm_uint8_t u8g_font_gdb11r[] U8G_FONT_SECTION("u8g_font_gdb11r");
extern const u8g_fntpgm_uint8_t u8g_font_gdb12r[] U8G_FONT_SECTION("u8g_font_gdb12r");
extern const u8g_fntpgm_uint8_t u8g_font_gdb14r[] U8G_FONT_SECTION("u8g_font_gdb14r");
extern const u8g_fntpgm_uint8_t u8g_font_gdb17r[] U8G_FONT_SECTION("u8g_font_gdb17r");
extern const u8g_fntpgm_uint8_t u8g_font_gdb20r[] U8G_FONT_SECTION("u8g_font_gdb20r");
extern const u8g_fntpgm_uint8_t u8g_font_gdb25r[] U8G_FONT_SECTION("u8g_font_gdb25r");
extern const u8g_fntpgm_uint8_t u8g_font_gdb30r[] U8G_FONT_SECTION("u8g_font_gdb30r");

extern const u8g_fntpgm_uint8_t u8g_font_gdb11n[] U8G_FONT_SECTION("u8g_font_gdb11n");
extern const u8g_fntpgm_uint8_t u8g_font_gdb12n[] U8G_FONT_SECTION("u8g_font_gdb12n");
extern const u8g_fntpgm_uint8_t u8g_font_gdb14n[] U8G_FONT_SECTION("u8g_font_gdb14n");
extern const u8g_fntpgm_uint8_t u8g_font_gdb17n[] U8G_FONT_SECTION("u8g_font_gdb17n");
extern const u8g_fntpgm_uint8_t u8g_font_gdb20n[] U8G_FONT_SECTION("u8g_font_gdb20n");
extern const u8g_fntpgm_uint8_t u8g_font_gdb25n[] U8G_FONT_SECTION("u8g_font_gdb25n");
extern const u8g_fntpgm_uint8_t u8g_font_gdb30n[] U8G_FONT_SECTION("u8g_font_gdb30n");

/*
  Gentium Regular
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_gdr9[] U8G_FONT_SECTION("u8g_font_gdr9");
extern const u8g_fntpgm_uint8_t u8g_font_gdr10[] U8G_FONT_SECTION("u8g_font_gdr10");
extern const u8g_fntpgm_uint8_t u8g_font_gdr11[] U8G_FONT_SECTION("u8g_font_gdr11");
extern const u8g_fntpgm_uint8_t u8g_font_gdr12[] U8G_FONT_SECTION("u8g_font_gdr12");
extern const u8g_fntpgm_uint8_t u8g_font_gdr14[] U8G_FONT_SECTION("u8g_font_gdr14");
extern const u8g_fntpgm_uint8_t u8g_font_gdr17[] U8G_FONT_SECTION("u8g_font_gdr17");
extern const u8g_fntpgm_uint8_t u8g_font_gdr20[] U8G_FONT_SECTION("u8g_font_gdr20");
extern const u8g_fntpgm_uint8_t u8g_font_gdr25[] U8G_FONT_SECTION("u8g_font_gdr25");
extern const u8g_fntpgm_uint8_t u8g_font_gdr30[] U8G_FONT_SECTION("u8g_font_gdr30");

extern const u8g_fntpgm_uint8_t u8g_font_gdr9r[] U8G_FONT_SECTION("u8g_font_gdr9r");
extern const u8g_fntpgm_uint8_t u8g_font_gdr10r[] U8G_FONT_SECTION("u8g_font_gdr10r");
extern const u8g_fntpgm_uint8_t u8g_font_gdr11r[] U8G_FONT_SECTION("u8g_font_gdr11r");
extern const u8g_fntpgm_uint8_t u8g_font_gdr12r[] U8G_FONT_SECTION("u8g_font_gdr12r");
extern const u8g_fntpgm_uint8_t u8g_font_gdr14r[] U8G_FONT_SECTION("u8g_font_gdr14r");
extern const u8g_fntpgm_uint8_t u8g_font_gdr17r[] U8G_FONT_SECTION("u8g_font_gdr17r");
extern const u8g_fntpgm_uint8_t u8g_font_gdr20r[] U8G_FONT_SECTION("u8g_font_gdr20r");
extern const u8g_fntpgm_uint8_t u8g_font_gdr25r[] U8G_FONT_SECTION("u8g_font_gdr25r");
extern const u8g_fntpgm_uint8_t u8g_font_gdr30r[] U8G_FONT_SECTION("u8g_font_gdr30r");

extern const u8g_fntpgm_uint8_t u8g_font_gdr9n[] U8G_FONT_SECTION("u8g_font_gdr9n");
extern const u8g_fntpgm_uint8_t u8g_font_gdr10n[] U8G_FONT_SECTION("u8g_font_gdr10n");
extern const u8g_fntpgm_uint8_t u8g_font_gdr11n[] U8G_FONT_SECTION("u8g_font_gdr11n");
extern const u8g_fntpgm_uint8_t u8g_font_gdr12n[] U8G_FONT_SECTION("u8g_font_gdr12n");
extern const u8g_fntpgm_uint8_t u8g_font_gdr14n[] U8G_FONT_SECTION("u8g_font_gdr14n");
extern const u8g_fntpgm_uint8_t u8g_font_gdr17n[] U8G_FONT_SECTION("u8g_font_gdr17n");
extern const u8g_fntpgm_uint8_t u8g_font_gdr20n[] U8G_FONT_SECTION("u8g_font_gdr20n");
extern const u8g_fntpgm_uint8_t u8g_font_gdr25n[] U8G_FONT_SECTION("u8g_font_gdr25n");
extern const u8g_fntpgm_uint8_t u8g_font_gdr30n[] U8G_FONT_SECTION("u8g_font_gdr30n");

/*
  Old-Standard Bold
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_osb18[] U8G_FONT_SECTION("u8g_font_osb18");
extern const u8g_fntpgm_uint8_t u8g_font_osb21[] U8G_FONT_SECTION("u8g_font_osb21");
extern const u8g_fntpgm_uint8_t u8g_font_osb26[] U8G_FONT_SECTION("u8g_font_osb26");
extern const u8g_fntpgm_uint8_t u8g_font_osb29[] U8G_FONT_SECTION("u8g_font_osb29");
extern const u8g_fntpgm_uint8_t u8g_font_osb35[] U8G_FONT_SECTION("u8g_font_osb35");

extern const u8g_fntpgm_uint8_t u8g_font_osb18r[] U8G_FONT_SECTION("u8g_font_osb18r");
extern const u8g_fntpgm_uint8_t u8g_font_osb21r[] U8G_FONT_SECTION("u8g_font_osb21r");
extern const u8g_fntpgm_uint8_t u8g_font_osb26r[] U8G_FONT_SECTION("u8g_font_osb26r");
extern const u8g_fntpgm_uint8_t u8g_font_osb29r[] U8G_FONT_SECTION("u8g_font_osb29r");
extern const u8g_fntpgm_uint8_t u8g_font_osb35r[] U8G_FONT_SECTION("u8g_font_osb35r");

extern const u8g_fntpgm_uint8_t u8g_font_osb18n[] U8G_FONT_SECTION("u8g_font_osb18n");
extern const u8g_fntpgm_uint8_t u8g_font_osb21n[] U8G_FONT_SECTION("u8g_font_osb21n");
extern const u8g_fntpgm_uint8_t u8g_font_osb26n[] U8G_FONT_SECTION("u8g_font_osb26n");
extern const u8g_fntpgm_uint8_t u8g_font_osb29n[] U8G_FONT_SECTION("u8g_font_osb29n");
extern const u8g_fntpgm_uint8_t u8g_font_osb35n[] U8G_FONT_SECTION("u8g_font_osb35n");

/*
  Old-Standard Regular
  r: Reduced char set (codes 32 - 128)
  n: Numbers (codes 42 - 57)
  no char: Full set (codes 32 - 255)
*/

extern const u8g_fntpgm_uint8_t u8g_font_osr18[] U8G_FONT_SECTION("u8g_font_osr18");
extern const u8g_fntpgm_uint8_t u8g_font_osr21[] U8G_FONT_SECTION("u8g_font_osr21");
extern const u8g_fntpgm_uint8_t u8g_font_osr26[] U8G_FONT_SECTION("u8g_font_osr26");
extern const u8g_fntpgm_uint8_t u8g_font_osr29[] U8G_FONT_SECTION("u8g_font_osr29");
extern const u8g_fntpgm_uint8_t u8g_font_osr35[] U8G_FONT_SECTION("u8g_font_osr35");

extern const u8g_fntpgm_uint8_t u8g_font_osr18r[] U8G_FONT_SECTION("u8g_font_osr18r");
extern const u8g_fntpgm_uint8_t u8g_font_osr21r[] U8G_FONT_SECTION("u8g_font_osr21r");
extern const u8g_fntpgm_uint8_t u8g_font_osr26r[] U8G_FONT_SECTION("u8g_font_osr26r");
extern const u8g_fntpgm_uint8_t u8g_font_osr29r[] U8G_FONT_SECTION("u8g_font_osr29r");
extern const u8g_fntpgm_uint8_t u8g_font_osr35r[] U8G_FONT_SECTION("u8g_font_osr35r");

extern const u8g_fntpgm_uint8_t u8g_font_osr18n[] U8G_FONT_SECTION("u8g_font_osr18n");
extern const u8g_fntpgm_uint8_t u8g_font_osr21n[] U8G_FONT_SECTION("u8g_font_osr21n");
extern const u8g_fntpgm_uint8_t u8g_font_osr26n[] U8G_FONT_SECTION("u8g_font_osr26n");
extern const u8g_fntpgm_uint8_t u8g_font_osr29n[] U8G_FONT_SECTION("u8g_font_osr29n");
extern const u8g_fntpgm_uint8_t u8g_font_osr35n[] U8G_FONT_SECTION("u8g_font_osr35n");

//extern const u8g_fntpgm_uint8_t u8g_font_osr41[] U8G_FONT_SECTION("u8g_font_osr41");

/* GNU unifont */

extern const u8g_fntpgm_uint8_t u8g_font_unifont_18_19[] U8G_FONT_SECTION("u8g_font_unifont_18_19");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_72_73[] U8G_FONT_SECTION("u8g_font_unifont_72_73");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_67_75[] U8G_FONT_SECTION("u8g_font_unifont_67_75");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_76[] U8G_FONT_SECTION("u8g_font_unifont_76");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_77[] U8G_FONT_SECTION("u8g_font_unifont_77");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_78_79[] U8G_FONT_SECTION("u8g_font_unifont_78_79");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_86[] U8G_FONT_SECTION("u8g_font_unifont_86");
extern const u8g_fntpgm_uint8_t u8g_font_unifont[] U8G_FONT_SECTION("u8g_font_unifont");
extern const u8g_fntpgm_uint8_t u8g_font_unifontr[] U8G_FONT_SECTION("u8g_font_unifontr");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_0_8[] U8G_FONT_SECTION("u8g_font_unifont_0_8");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_2_3[] U8G_FONT_SECTION("u8g_font_unifont_2_3");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_4_5[] U8G_FONT_SECTION("u8g_font_unifont_4_5");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_8_9[] U8G_FONT_SECTION("u8g_font_unifont_8_9");
extern const u8g_fntpgm_uint8_t u8g_font_unifont_12_13[] U8G_FONT_SECTION("u8g_font_unifont_12_13");


/* 04b fonts */

extern const u8g_fntpgm_uint8_t u8g_font_04b_03b[] U8G_FONT_SECTION("u8g_font_04b_03b");
extern const u8g_fntpgm_uint8_t u8g_font_04b_03bn[] U8G_FONT_SECTION("u8g_font_04b_03bn");
extern const u8g_fntpgm_uint8_t u8g_font_04b_03br[] U8G_FONT_SECTION("u8g_font_04b_03br");
extern const u8g_fntpgm_uint8_t u8g_font_04b_03[] U8G_FONT_SECTION("u8g_font_04b_03");
extern const u8g_fntpgm_uint8_t u8g_font_04b_03n[] U8G_FONT_SECTION("u8g_font_04b_03n");
extern const u8g_fntpgm_uint8_t u8g_font_04b_03r[] U8G_FONT_SECTION("u8g_font_04b_03r");
extern const u8g_fntpgm_uint8_t u8g_font_04b_24[] U8G_FONT_SECTION("u8g_font_04b_24");
extern const u8g_fntpgm_uint8_t u8g_font_04b_24n[] U8G_FONT_SECTION("u8g_font_04b_24n");
extern const u8g_fntpgm_uint8_t u8g_font_04b_24r[] U8G_FONT_SECTION("u8g_font_04b_24r");

/* orgdot fonts */

extern const u8g_fntpgm_uint8_t u8g_font_orgv01[] U8G_FONT_SECTION("u8g_font_orgv01");
extern const u8g_fntpgm_uint8_t u8g_font_orgv01r[] U8G_FONT_SECTION("u8g_font_orgv01r");
extern const u8g_fntpgm_uint8_t u8g_font_orgv01n[] U8G_FONT_SECTION("u8g_font_orgv01n");

extern const u8g_fntpgm_uint8_t u8g_font_fixed_v0[] U8G_FONT_SECTION("u8g_font_fixed_v0");
extern const u8g_fntpgm_uint8_t u8g_font_fixed_v0r[] U8G_FONT_SECTION("u8g_font_fixed_v0r");
extern const u8g_fntpgm_uint8_t u8g_font_fixed_v0n[] U8G_FONT_SECTION("u8g_font_fixed_v0n");

extern const u8g_fntpgm_uint8_t u8g_font_tpssb[] U8G_FONT_SECTION("u8g_font_tpssb");
extern const u8g_fntpgm_uint8_t u8g_font_tpssbr[] U8G_FONT_SECTION("u8g_font_tpssbr");
extern const u8g_fntpgm_uint8_t u8g_font_tpssbn[] U8G_FONT_SECTION("u8g_font_tpssbn");

extern const u8g_fntpgm_uint8_t u8g_font_tpss[] U8G_FONT_SECTION("u8g_font_tpss");
extern const u8g_fntpgm_uint8_t u8g_font_tpssr[] U8G_FONT_SECTION("u8g_font_tpssr");
extern const u8g_fntpgm_uint8_t u8g_font_tpssn[] U8G_FONT_SECTION("u8g_font_tpssn");

/* contributed */

extern const u8g_fntpgm_uint8_t u8g_font_freedoomr25n[] U8G_FONT_SECTION("u8g_font_freedoomr25n");
extern const u8g_fntpgm_uint8_t u8g_font_freedoomr10r[] U8G_FONT_SECTION("u8g_font_freedoomr10r");

/* adobe X11 */
extern const u8g_fntpgm_uint8_t u8g_font_courB08[] U8G_FONT_SECTION("u8g_font_courB08");
extern const u8g_fntpgm_uint8_t u8g_font_courB08r[] U8G_FONT_SECTION("u8g_font_courB08r");
extern const u8g_fntpgm_uint8_t u8g_font_courB10[] U8G_FONT_SECTION("u8g_font_courB10");
extern const u8g_fntpgm_uint8_t u8g_font_courB10r[] U8G_FONT_SECTION("u8g_font_courB10r");
extern const u8g_fntpgm_uint8_t u8g_font_courB12[] U8G_FONT_SECTION("u8g_font_courB12");
extern const u8g_fntpgm_uint8_t u8g_font_courB12r[] U8G_FONT_SECTION("u8g_font_courB12r");
extern const u8g_fntpgm_uint8_t u8g_font_courB14[] U8G_FONT_SECTION("u8g_font_courB14");
extern const u8g_fntpgm_uint8_t u8g_font_courB14r[] U8G_FONT_SECTION("u8g_font_courB14r");
extern const u8g_fntpgm_uint8_t u8g_font_courB18[] U8G_FONT_SECTION("u8g_font_courB18");
extern const u8g_fntpgm_uint8_t u8g_font_courB18r[] U8G_FONT_SECTION("u8g_font_courB18r");
extern const u8g_fntpgm_uint8_t u8g_font_courB24[] U8G_FONT_SECTION("u8g_font_courB24");
extern const u8g_fntpgm_uint8_t u8g_font_courB24r[] U8G_FONT_SECTION("u8g_font_courB24r");
extern const u8g_fntpgm_uint8_t u8g_font_courB24n[] U8G_FONT_SECTION("u8g_font_courB24n");

extern const u8g_fntpgm_uint8_t u8g_font_courR08[] U8G_FONT_SECTION("u8g_font_courR08");
extern const u8g_fntpgm_uint8_t u8g_font_courR08r[] U8G_FONT_SECTION("u8g_font_courR08r");
extern const u8g_fntpgm_uint8_t u8g_font_courR10[] U8G_FONT_SECTION("u8g_font_courR10");
extern const u8g_fntpgm_uint8_t u8g_font_courR10r[] U8G_FONT_SECTION("u8g_font_courR10r");
extern const u8g_fntpgm_uint8_t u8g_font_courR12[] U8G_FONT_SECTION("u8g_font_courR12");
extern const u8g_fntpgm_uint8_t u8g_font_courR12r[] U8G_FONT_SECTION("u8g_font_courR12r");
extern const u8g_fntpgm_uint8_t u8g_font_courR14[] U8G_FONT_SECTION("u8g_font_courR14");
extern const u8g_fntpgm_uint8_t u8g_font_courR14r[] U8G_FONT_SECTION("u8g_font_courR14r");
extern const u8g_fntpgm_uint8_t u8g_font_courR18[] U8G_FONT_SECTION("u8g_font_courR18");
extern const u8g_fntpgm_uint8_t u8g_font_courR18r[] U8G_FONT_SECTION("u8g_font_courR18r");
extern const u8g_fntpgm_uint8_t u8g_font_courR24[] U8G_FONT_SECTION("u8g_font_courR24");
extern const u8g_fntpgm_uint8_t u8g_font_courR24r[] U8G_FONT_SECTION("u8g_font_courR24r");
extern const u8g_fntpgm_uint8_t u8g_font_courR24n[] U8G_FONT_SECTION("u8g_font_courR24n");

extern const u8g_fntpgm_uint8_t u8g_font_helvB08[] U8G_FONT_SECTION("u8g_font_helvB08");
extern const u8g_fntpgm_uint8_t u8g_font_helvB08r[] U8G_FONT_SECTION("u8g_font_helvB08r");
extern const u8g_fntpgm_uint8_t u8g_font_helvB08n[] U8G_FONT_SECTION("u8g_font_helvB08n");
extern const u8g_fntpgm_uint8_t u8g_font_helvB10[] U8G_FONT_SECTION("u8g_font_helvB10");
extern const u8g_fntpgm_uint8_t u8g_font_helvB10r[] U8G_FONT_SECTION("u8g_font_helvB10r");
extern const u8g_fntpgm_uint8_t u8g_font_helvB10n[] U8G_FONT_SECTION("u8g_font_helvB10n");
extern const u8g_fntpgm_uint8_t u8g_font_helvB12[] U8G_FONT_SECTION("u8g_font_helvB12");
extern const u8g_fntpgm_uint8_t u8g_font_helvB12r[] U8G_FONT_SECTION("u8g_font_helvB12r");
extern const u8g_fntpgm_uint8_t u8g_font_helvB12n[] U8G_FONT_SECTION("u8g_font_helvB12n");
extern const u8g_fntpgm_uint8_t u8g_font_helvB14[] U8G_FONT_SECTION("u8g_font_helvB14");
extern const u8g_fntpgm_uint8_t u8g_font_helvB14r[] U8G_FONT_SECTION("u8g_font_helvB14r");
extern const u8g_fntpgm_uint8_t u8g_font_helvB14n[] U8G_FONT_SECTION("u8g_font_helvB14n");
extern const u8g_fntpgm_uint8_t u8g_font_helvB18[] U8G_FONT_SECTION("u8g_font_helvB18");
extern const u8g_fntpgm_uint8_t u8g_font_helvB18r[] U8G_FONT_SECTION("u8g_font_helvB18r");
extern const u8g_fntpgm_uint8_t u8g_font_helvB18n[] U8G_FONT_SECTION("u8g_font_helvB18n");
extern const u8g_fntpgm_uint8_t u8g_font_helvB24[] U8G_FONT_SECTION("u8g_font_helvB24");
extern const u8g_fntpgm_uint8_t u8g_font_helvB24r[] U8G_FONT_SECTION("u8g_font_helvB24r");
extern const u8g_fntpgm_uint8_t u8g_font_helvB24n[] U8G_FONT_SECTION("u8g_font_helvB24n");

extern const u8g_fntpgm_uint8_t u8g_font_helvR08[] U8G_FONT_SECTION("u8g_font_helvR08");
extern const u8g_fntpgm_uint8_t u8g_font_helvR08r[] U8G_FONT_SECTION("u8g_font_helvR08r");
extern const u8g_fntpgm_uint8_t u8g_font_helvR08n[] U8G_FONT_SECTION("u8g_font_helvR08n");
extern const u8g_fntpgm_uint8_t u8g_font_helvR10[] U8G_FONT_SECTION("u8g_font_helvR10");
extern const u8g_fntpgm_uint8_t u8g_font_helvR10r[] U8G_FONT_SECTION("u8g_font_helvR10r");
extern const u8g_fntpgm_uint8_t u8g_font_helvR10n[] U8G_FONT_SECTION("u8g_font_helvR10n");
extern const u8g_fntpgm_uint8_t u8g_font_helvR12[] U8G_FONT_SECTION("u8g_font_helvR12");
extern const u8g_fntpgm_uint8_t u8g_font_helvR12r[] U8G_FONT_SECTION("u8g_font_helvR12r");
extern const u8g_fntpgm_uint8_t u8g_font_helvR12n[] U8G_FONT_SECTION("u8g_font_helvR12n");
extern const u8g_fntpgm_uint8_t u8g_font_helvR14[] U8G_FONT_SECTION("u8g_font_helvR14");
extern const u8g_fntpgm_uint8_t u8g_font_helvR14r[] U8G_FONT_SECTION("u8g_font_helvR14r");
extern const u8g_fntpgm_uint8_t u8g_font_helvR14n[] U8G_FONT_SECTION("u8g_font_helvR14n");
extern const u8g_fntpgm_uint8_t u8g_font_helvR18[] U8G_FONT_SECTION("u8g_font_helvR18");
extern const u8g_fntpgm_uint8_t u8g_font_helvR18r[] U8G_FONT_SECTION("u8g_font_helvR18r");
extern const u8g_fntpgm_uint8_t u8g_font_helvR18n[] U8G_FONT_SECTION("u8g_font_helvR18n");
extern const u8g_fntpgm_uint8_t u8g_font_helvR24[] U8G_FONT_SECTION("u8g_font_helvR24");
extern const u8g_fntpgm_uint8_t u8g_font_helvR24r[] U8G_FONT_SECTION("u8g_font_helvR24r");
extern const u8g_fntpgm_uint8_t u8g_font_helvR24n[] U8G_FONT_SECTION("u8g_font_helvR24n");

extern const u8g_fntpgm_uint8_t u8g_font_ncenB08[] U8G_FONT_SECTION("u8g_font_ncenB08");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB08r[] U8G_FONT_SECTION("u8g_font_ncenB08r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB10[] U8G_FONT_SECTION("u8g_font_ncenB10");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB10r[] U8G_FONT_SECTION("u8g_font_ncenB10r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB12[] U8G_FONT_SECTION("u8g_font_ncenB12");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB12r[] U8G_FONT_SECTION("u8g_font_ncenB12r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB14[] U8G_FONT_SECTION("u8g_font_ncenB14");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB14r[] U8G_FONT_SECTION("u8g_font_ncenB14r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB18[] U8G_FONT_SECTION("u8g_font_ncenB18");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB18r[] U8G_FONT_SECTION("u8g_font_ncenB18r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB24[] U8G_FONT_SECTION("u8g_font_ncenB24");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB24r[] U8G_FONT_SECTION("u8g_font_ncenB24r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenB24n[] U8G_FONT_SECTION("u8g_font_ncenB24n");

extern const u8g_fntpgm_uint8_t u8g_font_ncenR08[] U8G_FONT_SECTION("u8g_font_ncenR08");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR08r[] U8G_FONT_SECTION("u8g_font_ncenR08r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR10[] U8G_FONT_SECTION("u8g_font_ncenR10");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR10r[] U8G_FONT_SECTION("u8g_font_ncenR10r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR12[] U8G_FONT_SECTION("u8g_font_ncenR12");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR12r[] U8G_FONT_SECTION("u8g_font_ncenR12r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR14[] U8G_FONT_SECTION("u8g_font_ncenR14");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR14r[] U8G_FONT_SECTION("u8g_font_ncenR14r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR18[] U8G_FONT_SECTION("u8g_font_ncenR18");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR18r[] U8G_FONT_SECTION("u8g_font_ncenR18r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR24[] U8G_FONT_SECTION("u8g_font_ncenR24");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR24r[] U8G_FONT_SECTION("u8g_font_ncenR24r");
extern const u8g_fntpgm_uint8_t u8g_font_ncenR24n[] U8G_FONT_SECTION("u8g_font_ncenR24n");

extern const u8g_fntpgm_uint8_t u8g_font_symb08[] U8G_FONT_SECTION("u8g_font_symb08");
extern const u8g_fntpgm_uint8_t u8g_font_symb08r[] U8G_FONT_SECTION("u8g_font_symb08r");
extern const u8g_fntpgm_uint8_t u8g_font_symb10[] U8G_FONT_SECTION("u8g_font_symb10");
extern const u8g_fntpgm_uint8_t u8g_font_symb10r[] U8G_FONT_SECTION("u8g_font_symb10r");
extern const u8g_fntpgm_uint8_t u8g_font_symb12[] U8G_FONT_SECTION("u8g_font_symb12");
extern const u8g_fntpgm_uint8_t u8g_font_symb12r[] U8G_FONT_SECTION("u8g_font_symb12r");
extern const u8g_fntpgm_uint8_t u8g_font_symb14[] U8G_FONT_SECTION("u8g_font_symb14");
extern const u8g_fntpgm_uint8_t u8g_font_symb14r[] U8G_FONT_SECTION("u8g_font_symb14r");
extern const u8g_fntpgm_uint8_t u8g_font_symb18[] U8G_FONT_SECTION("u8g_font_symb18");
extern const u8g_fntpgm_uint8_t u8g_font_symb18r[] U8G_FONT_SECTION("u8g_font_symb18r");
extern const u8g_fntpgm_uint8_t u8g_font_symb24[] U8G_FONT_SECTION("u8g_font_symb24");
extern const u8g_fntpgm_uint8_t u8g_font_symb24r[] U8G_FONT_SECTION("u8g_font_symb24r");

extern const u8g_fntpgm_uint8_t u8g_font_timB08[] U8G_FONT_SECTION("u8g_font_timB08");
extern const u8g_fntpgm_uint8_t u8g_font_timB08r[] U8G_FONT_SECTION("u8g_font_timB08r");
extern const u8g_fntpgm_uint8_t u8g_font_timB10[] U8G_FONT_SECTION("u8g_font_timB10");
extern const u8g_fntpgm_uint8_t u8g_font_timB10r[] U8G_FONT_SECTION("u8g_font_timB10r");
extern const u8g_fntpgm_uint8_t u8g_font_timB12[] U8G_FONT_SECTION("u8g_font_timB12");
extern const u8g_fntpgm_uint8_t u8g_font_timB12r[] U8G_FONT_SECTION("u8g_font_timB12r");
extern const u8g_fntpgm_uint8_t u8g_font_timB14[] U8G_FONT_SECTION("u8g_font_timB14");
extern const u8g_fntpgm_uint8_t u8g_font_timB14r[] U8G_FONT_SECTION("u8g_font_timB14r");
extern const u8g_fntpgm_uint8_t u8g_font_timB18[] U8G_FONT_SECTION("u8g_font_timB18");
extern const u8g_fntpgm_uint8_t u8g_font_timB18r[] U8G_FONT_SECTION("u8g_font_timB18r");
extern const u8g_fntpgm_uint8_t u8g_font_timB24[] U8G_FONT_SECTION("u8g_font_timB24");
extern const u8g_fntpgm_uint8_t u8g_font_timB24r[] U8G_FONT_SECTION("u8g_font_timB24r");
extern const u8g_fntpgm_uint8_t u8g_font_timB24n[] U8G_FONT_SECTION("u8g_font_timB24n");

extern const u8g_fntpgm_uint8_t u8g_font_timR08[] U8G_FONT_SECTION("u8g_font_timR08");
extern const u8g_fntpgm_uint8_t u8g_font_timR08r[] U8G_FONT_SECTION("u8g_font_timR08r");
extern const u8g_fntpgm_uint8_t u8g_font_timR10[] U8G_FONT_SECTION("u8g_font_timR10");
extern const u8g_fntpgm_uint8_t u8g_font_timR10r[] U8G_FONT_SECTION("u8g_font_timR10r");
extern const u8g_fntpgm_uint8_t u8g_font_timR12[] U8G_FONT_SECTION("u8g_font_timR12");
extern const u8g_fntpgm_uint8_t u8g_font_timR12r[] U8G_FONT_SECTION("u8g_font_timR12r");
extern const u8g_fntpgm_uint8_t u8g_font_timR14[] U8G_FONT_SECTION("u8g_font_timR14");
extern const u8g_fntpgm_uint8_t u8g_font_timR14r[] U8G_FONT_SECTION("u8g_font_timR14r");
extern const u8g_fntpgm_uint8_t u8g_font_timR18[] U8G_FONT_SECTION("u8g_font_timR18");
extern const u8g_fntpgm_uint8_t u8g_font_timR18r[] U8G_FONT_SECTION("u8g_font_timR18r");
extern const u8g_fntpgm_uint8_t u8g_font_timR24[] U8G_FONT_SECTION("u8g_font_timR24");
extern const u8g_fntpgm_uint8_t u8g_font_timR24r[] U8G_FONT_SECTION("u8g_font_timR24r");
extern const u8g_fntpgm_uint8_t u8g_font_timR24n[] U8G_FONT_SECTION("u8g_font_timR24n");

/* fontstruct */

extern const u8g_fntpgm_uint8_t u8g_font_p01type[] U8G_FONT_SECTION("u8g_font_p01type");
extern const u8g_fntpgm_uint8_t u8g_font_p01typer[] U8G_FONT_SECTION("u8g_font_p01typer");
extern const u8g_fntpgm_uint8_t u8g_font_p01typen[] U8G_FONT_SECTION("u8g_font_p01typen");

extern const u8g_fntpgm_uint8_t u8g_font_lucasfont_alternate[] U8G_FONT_SECTION("u8g_font_lucasfont_alternate");
extern const u8g_fntpgm_uint8_t u8g_font_lucasfont_alternater[] U8G_FONT_SECTION("u8g_font_lucasfont_alternater");
extern const u8g_fntpgm_uint8_t u8g_font_lucasfont_alternaten[] U8G_FONT_SECTION("u8g_font_lucasfont_alternaten");

extern const u8g_fntpgm_uint8_t u8g_font_chikita[] U8G_FONT_SECTION("u8g_font_chikita");
extern const u8g_fntpgm_uint8_t u8g_font_chikitar[] U8G_FONT_SECTION("u8g_font_chikitar");
extern const u8g_fntpgm_uint8_t u8g_font_chikitan[] U8G_FONT_SECTION("u8g_font_chikitan");

extern const u8g_fntpgm_uint8_t u8g_font_pixelle_micro[] U8G_FONT_SECTION("u8g_font_pixelle_micro");
extern const u8g_fntpgm_uint8_t u8g_font_pixelle_micror[] U8G_FONT_SECTION("u8g_font_pixelle_micror");
extern const u8g_fntpgm_uint8_t u8g_font_pixelle_micron[] U8G_FONT_SECTION("u8g_font_pixelle_micron");

extern const u8g_fntpgm_uint8_t u8g_font_trixel_square[] U8G_FONT_SECTION("u8g_font_trixel_square");
extern const u8g_fntpgm_uint8_t u8g_font_trixel_squarer[] U8G_FONT_SECTION("u8g_font_trixel_squarer");
extern const u8g_fntpgm_uint8_t u8g_font_trixel_squaren[] U8G_FONT_SECTION("u8g_font_trixel_squaren");

extern const u8g_fntpgm_uint8_t u8g_font_robot_de_niro[] U8G_FONT_SECTION("u8g_font_robot_de_niro");
extern const u8g_fntpgm_uint8_t u8g_font_robot_de_niror[] U8G_FONT_SECTION("u8g_font_robot_de_niror");
extern const u8g_fntpgm_uint8_t u8g_font_robot_de_niron[] U8G_FONT_SECTION("u8g_font_robot_de_niron");

extern const u8g_fntpgm_uint8_t u8g_font_baby[] U8G_FONT_SECTION("u8g_font_baby");
extern const u8g_fntpgm_uint8_t u8g_font_babyr[] U8G_FONT_SECTION("u8g_font_babyr");
extern const u8g_fntpgm_uint8_t u8g_font_babyn[] U8G_FONT_SECTION("u8g_font_babyn");

extern const u8g_fntpgm_uint8_t u8g_font_blipfest_07[] U8G_FONT_SECTION("u8g_font_blipfest_07");
extern const u8g_fntpgm_uint8_t u8g_font_blipfest_07r[] U8G_FONT_SECTION("u8g_font_blipfest_07r");
extern const u8g_fntpgm_uint8_t u8g_font_blipfest_07n[] U8G_FONT_SECTION("u8g_font_blipfest_07n");

/* profont */

extern const u8g_fntpgm_uint8_t u8g_font_profont10[] U8G_FONT_SECTION("u8g_font_profont10");
extern const u8g_fntpgm_uint8_t u8g_font_profont10r[] U8G_FONT_SECTION("u8g_font_profont10r");
extern const u8g_fntpgm_uint8_t u8g_font_profont11[] U8G_FONT_SECTION("u8g_font_profont11");
extern const u8g_fntpgm_uint8_t u8g_font_profont11r[] U8G_FONT_SECTION("u8g_font_profont11r");
extern const u8g_fntpgm_uint8_t u8g_font_profont12[] U8G_FONT_SECTION("u8g_font_profont12");
extern const u8g_fntpgm_uint8_t u8g_font_profont12r[] U8G_FONT_SECTION("u8g_font_profont12r");
extern const u8g_fntpgm_uint8_t u8g_font_profont15[] U8G_FONT_SECTION("u8g_font_profont15");
extern const u8g_fntpgm_uint8_t u8g_font_profont15r[] U8G_FONT_SECTION("u8g_font_profont15r");
extern const u8g_fntpgm_uint8_t u8g_font_profont17[] U8G_FONT_SECTION("u8g_font_profont17");
extern const u8g_fntpgm_uint8_t u8g_font_profont17r[] U8G_FONT_SECTION("u8g_font_profont17r");
extern const u8g_fntpgm_uint8_t u8g_font_profont22[] U8G_FONT_SECTION("u8g_font_profont22");
extern const u8g_fntpgm_uint8_t u8g_font_profont22r[] U8G_FONT_SECTION("u8g_font_profont22r");
extern const u8g_fntpgm_uint8_t u8g_font_profont29[] U8G_FONT_SECTION("u8g_font_profont29");
extern const u8g_fntpgm_uint8_t u8g_font_profont29r[] U8G_FONT_SECTION("u8g_font_profont29r");


#ifdef __cplusplus
}
#endif
