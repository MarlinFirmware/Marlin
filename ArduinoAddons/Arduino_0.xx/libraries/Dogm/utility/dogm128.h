/*

  dogm128.h

  (c) 2010 Oliver Kraus (olikraus@gmail.com)
  
  This file is part of the dogm128 library.

  The dogm128 library is free software: you can redistribute it and/or modify
  it under the terms of the Lesser GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  The dogm128 library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  Lesser GNU General Public License for more details.

  You should have received a copy of the Lesser GNU General Public License
  along with dogm128.  If not, see <http://www.gnu.org/licenses/>.

  Controller SPI Interface Selection 
    DOG_SPI_USI			Universal Serial Interface of ATTINY controller
    DOG_SPI_ATMEGA		ATMEGA SPI Interface
    DOG_SPI_ARDUINO		HW SPI Interface for ATMEGA within Arduino Environment
    DOG_SPI_SW_ARDUINO	SW SPI Interface
    DOG_SPI_CHIPKIT_PIC32	SW SPI Interface for the CHIPKIT Environment
    nothing defined			defaults to DOG_SPI_ARDUINO (ATMEGA) or DOG_SPI_CHIPKIT_PIC32 (PIC32)

  Normal or Revers Display Mode
    DOG_REVERSE		Display output rotated by 180 degree
    nothing defined		No rotation of the output
    
  Memory Usage
    DOG_DOUBLE_MEMORY 	Double size of the internal page memory. Will give some speed improvement
    nothing defined			Minimal memory usage
    
  Display Selection
  one of the following constants could be defined:
    DOGM128_HW		DOGM128 Display
    DOGS102_HW		DOGS102 Display
    DOGM132_HW		DOGM132 Display
    DOGXL160_HW_BW		DOGXL160 Display
    DOGXL160_HW_GR		DOGXL160 Display
    ADA_ST7565P_HW      Adafruit Graphics LCD based on the ST7565P
    
    nothing defined		error message
    
*/

#ifndef _DOGM128_H
#define _DOGM128_H

/*=========================================================================*/
/* Begin: User Configuration */
/*=========================================================================*/

//#define DOG_REVERSE			/* uncomment for 180 degree rotation */

//#define DOG_DOUBLE_MEMORY		/* uncomment for speed improvement */

#define DOGM128_HW				/* uncomment for the DOGM128 display */
//#define DOGS102_HW				/* uncomment for the DOGS102 display */
//#define DOGM132_HW				/* uncomment for the DOGM132 display */
//#define DOGXL160_HW_BW			/* uncomment for the DOGXL160 display, black & white mode */
//#define DOGXL160_HW_GR			/* uncomment for the DOGXL160 display gray level mode */

//#define ADA_ST7565P_HW                        /* uncomment for the Adafruit ST7565P display. */
                                                /* Note: if you are using a wiring scheme different from
                                                   http://www.ladyada.net/learn/lcd/st7565.html
                                                   then you will need to change the pin number accordingly 
                                                   (lower in this same file, look for PIN_RST)
                                                */
//#define ES13BB0_HW				/* uncomment for ES13BB0 128x64 display */



//#define DOG_SPI_USI			/* uncomment to force Universal Serial Interface of ATTINY controller  (HW SPI) */
//#define DOG_SPI_ATMEGA		/* uncomment to ATMEGA SPI Interface (HW SPI) */
#define DOG_SPI_ARDUINO		/* uncomment to HW SPI Interface for ATMEGA within Arduino Environment */
//#define DOG_SPI_SW_ARDUINO	/* uncomment to SW SPI Interface */
//#define DOG_SPI_CHIPKIT_PIC32	/* uncomment to SW SPI Interface for the CHIPKIT Environment */



/*=========================================================================*/
/* End: User Configuration */
/*=========================================================================*/

#if !defined ADA_ST7565P_HW && !defined DOGM128_HW && !defined DOGM132_HW && !defined DOGS102_HW && !defined DOGXL160_HW_BW  && !defined DOGXL160_HW_GR
/* print error message, please uncomment one of the displays above */
#error LCD model is not defined. Define your LCD in dogm128.h.
#endif

// prevent duplicate definition
#if defined DOGM128_HW 
#if defined DOGM132_HW
#undef DOGM132_HW
#endif
#if defined DOGS102_HW
#undef DOGS102_HW
#endif
#endif
#if defined DOGM132_HW && defined DOGS102_HW
#undef DOGS102_HW
#endif

#include "dogmpgm.h"
#include "dogmfont.h"

/* Arduino Pin assignments have been moved to dogmspi.c */

/* height of a page for all supported DOG devices (with speciall case for DOGMXL160_HW_GR) */ 
#if defined(DOG_DOUBLE_MEMORY)
#if defined(DOGXL160_HW_GR)
#define DOG_PAGE_HEIGHT 8
#else
#define DOG_PAGE_HEIGHT 16
#endif
#else /*DOG_DOUBLE_MEMORY*/
#if defined(DOGXL160_HW_GR)
#define DOG_PAGE_HEIGHT 4
#else
#define DOG_PAGE_HEIGHT 8
#endif
#endif


#ifdef ES13BB0_HW
#define DOG_WIDTH 128
#define DOG_HEIGHT 64
#endif

#ifdef ADA_ST7565P_HW
#define DOG_WIDTH 128
#define DOG_HEIGHT 64
#endif

/* setings for the various DOG displays */
#ifdef DOGM128_HW
#define DOG_WIDTH 128
#define DOG_HEIGHT 64
#endif

#ifdef DOGS102_HW
#define DOG_WIDTH 102
#define DOG_HEIGHT 64
#endif

#ifdef DOGM132_HW
#define DOG_WIDTH 132
#define DOG_HEIGHT 32
#endif

#ifdef DOGXL160_HW_BW
#define DOG_WIDTH 160
#define DOG_HEIGHT 104
#endif

#ifdef DOGXL160_HW_GR
#define DOG_WIDTH 160
#define DOG_HEIGHT 104
#endif

/* derived constants */
#define DOG_PAGE_CNT ((DOG_HEIGHT+DOG_PAGE_HEIGHT-1)/DOG_PAGE_HEIGHT)
#define DOG_PAGE_WIDTH DOG_WIDTH
#if defined(DOG_DOUBLE_MEMORY)
#define DOG_PAGE_SIZE (2*DOG_PAGE_WIDTH)
#else
#define DOG_PAGE_SIZE DOG_PAGE_WIDTH
#endif

#ifdef __cplusplus
extern "C" {
#endif


extern unsigned char dog_page_buffer[DOG_PAGE_SIZE];

extern uint8_t dog_min_y;
extern uint8_t dog_max_y;

/* --- dogm128.c --- */

/* pin assignment for arduino, should be renamed */
  
#ifndef ADA_ST7565P_HW
#define PIN_SCK   7
#define PIN_MISO  6
#define PIN_MOSI  5
#define PIN_SS    29	// This is the pin where the !CS line of the LCD is connected don't mix it with the SS of the AVR SPI interface
#define PIN_SS_AVR	  4 // This is the SS pin of AVRs SPI interface
#define PIN_A0_DEFAULT     30
#else
#define PIN_RST   6     // This is for the default Adafruit wiring scheme
#define PIN_SCK   8     // described in:
#define PIN_MOSI  9     // http://www.ladyada.net/learn/lcd/st7565.html
#define PIN_SS    5     // change these numbers to reflect your wiring.
#define PIN_A0_DEFAULT  7
#endif

extern uint8_t dog_spi_pin_a0;
extern uint8_t dog_spi_pin_cs;	/* arduino chip select pin */

#ifdef ADA_ST7565P_HW
extern uint8_t dog_spi_pin_rst; /* ST7565P reset pin */
#endif

extern uint8_t dog_spi_result;		/* last value returned from SPI system (after executing the picture loop) */

  
void dog_Delay(uint16_t val);				/* delay in milliseconds */
void dog_InitA0CS(uint8_t pin_a0, uint8_t pin_cs);
void dog_Init(unsigned short pin_a0);
void dog_SetContrast(uint8_t val);			/* values between 0 and 63 allowed, previosly named dog_set_contranst() */
void dog_SetInvertPixelMode(uint8_t val);	/* previosly named dog_set_inverse() */
void dog_SetUC1610GrayShade(uint8_t val);	/* dogmsysgr.c, values are from 0 to 3 */

/* --- page functions --- */

void dog_StartPage(void);
uint8_t dog_NextPage(void);

/* --- dogmspi.c --- */
void dog_spi_init(void);
unsigned char dog_spi_out(unsigned char data);
void dog_spi_enable_client(void);
void dog_spi_disable_client(void);
void dog_cmd_mode(void);
void dog_data_mode(void);


/* --- dogmop.c (other pixel: pixel value) --- */


#if defined(DOGXL160_HW_GR)
extern uint8_t dog_pixel_value;
#endif
void dog_SetPixelValue(uint8_t value);


/* --- set/clr functions --- */

extern unsigned char dog_bit_to_mask[8];	/* dogmsd.c */

void dog_set_pixel(uint8_t x, uint8_t y) DOG_ATTR_FN_NOINLINE;
void dog_xor_pixel(uint8_t x, uint8_t y) DOG_ATTR_FN_NOINLINE;
void dog_clr_pixel(uint8_t x, uint8_t y) DOG_ATTR_FN_NOINLINE;

void dog_SetPixel(uint8_t x, uint8_t y);	/* dogmsp.c */
void dog_ClrPixel(uint8_t x, uint8_t y);	/* dogmcp.c */
void dog_XorPixel(uint8_t x, uint8_t y);	/* v1.01, dogmxp.c */

/* x1 must be lower or equal to x2 */
void dog_SetHLine(uint8_t x1, uint8_t x2, uint8_t y); 	/* v1.01, dogmsh.c */
void dog_ClrHLine(uint8_t x1, uint8_t x2, uint8_t y); 	/* v1.03, dogmch.c */
void dog_XorHLine(uint8_t x1, uint8_t x2, uint8_t y); 	/* v1.03, dogmxh.c */

uint8_t dog_get_vline_mask(uint8_t y1, uint8_t y2);	/* internal function, dogmov.c */
/* y1 must be lower or equal to y2 */
void dog_SetVLine(uint8_t x, uint8_t y1, uint8_t y2);			/* dogmsv.c */
void dog_ClrVLine(uint8_t x, uint8_t y1, uint8_t y2);			/* dogmcv.c */
void dog_XorVLine(uint8_t x, uint8_t y1, uint8_t y2);			/* dogmxv.c */

/* x1 must be lower or equal to x2 */
/* y1 must be lower or equal to y2 */
void dog_SetBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);	/* dogmsb.c */
void dog_ClrBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);	/* dogmcb.c */
void dog_XorBox(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2);	/* dogmxb.c */

/* --- bitmap functions --- */

/* at position (x,y) set a pixel for each logical one bit in the bitmap pattern */
/* the bitmap must contain (w+7)/8 bytes, each byte is interpreted as bitmap pattern */
/* most significant bit of the byte in the pattern is on the left */
void dog_SetHBitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w); /* v1.01, dogmsm.c */
void dog_SetHBitmapP(uint8_t x, uint8_t y, DOG_PGM_P bitmap, uint8_t w); /* v1.01, dogmsmp.c */
void dog_ClrHBitmapP(uint8_t x, uint8_t y, DOG_PGM_P bitmap, uint8_t w); /* v1.01, dogmcmp.c */
void dog_SetBitmap(uint8_t x, uint8_t y, const unsigned char *bitmap, uint8_t w, uint8_t h); /* v1.01, dogmsm.c */
void dog_SetBitmapP(uint8_t x, uint8_t y, DOG_PGM_P bitmap, uint8_t w, uint8_t h); /* v1.01, dogmsmp.c */
void dog_ClrBitmapP(uint8_t x, uint8_t y, DOG_PGM_P bitmap, uint8_t w, uint8_t h); /* v1.09, dogmcmp.c */

/* --- draw functions --- */
/*
  size = 0:
  +++
  +#+
  +++
  size = 1:
  +#+
  ###
  +#+
*/

void dog_DrawPoint(uint8_t x, uint8_t y, uint8_t size); 	/* dogmdp.c */
void dog_DrawLine(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2, uint8_t size); 	/* dogmdl.c */
void dog_DrawArc(uint8_t mx, uint8_t my, uint8_t r, uint8_t w0, uint8_t w1, uint8_t size);  	/* dogmda.c */


/* --- font information --- */

/* start: internal definitions. Do not call or use these definitions directly */
uint16_t dog_font_pgm_read_word(DOG_PGM_P buf) DOG_ATTR_FN_NOINLINE;
uint8_t dog_char(uint8_t x, uint8_t y, DOG_PGM_P font, uint8_t mode, uint8_t rot, unsigned char code);
#define fnt_get_bbox_capital_a(buf) (dog_font_pgm_read_word((buf)+0))
#define fnt_get_bbox_small_a(buf) (dog_font_pgm_read_word((buf)+2))
#define fnt_get_bbox_width(buf) (dog_pgm_read((buf)+4))
#define fnt_get_bbox_height(buf) (dog_pgm_read((buf)+5))
#define fnt_get_bbox_descent(buf) (dog_pgm_read((buf)+6))
#define FNT_DATA 7
/* end: internal definitions */


uint8_t dog_GetCharWidth(DOG_PGM_P font, unsigned char code);	/* dogmfont.c */

uint8_t dog_GetFontBBXHeight(DOG_PGM_P buf);
uint8_t dog_GetFontBBXWidth(DOG_PGM_P buf);
uint8_t dog_GetFontBBXDescent(DOG_PGM_P buf);
  
uint8_t dog_DrawChar(uint8_t x, uint8_t y, DOG_PGM_P font, unsigned char code); /* dogmfont.c */
uint8_t dog_DrawRChar(uint8_t x, uint8_t y, uint8_t rot, DOG_PGM_P font, unsigned char code);

uint8_t dog_GetStrWidth(DOG_PGM_P font, const char *s); /* dogmds.c */
uint8_t dog_DrawStr(uint8_t x, uint8_t y, DOG_PGM_P font, const char *s);	/* dogmds.c */
uint8_t dog_DrawRStr(uint8_t x, uint8_t y, uint8_t rot, DOG_PGM_P font, const char *s);

uint8_t dog_GetStrWidthP(DOG_PGM_P font, DOG_PSTR_P s); /* dogmdsp.c */
uint8_t dog_DrawStrP(uint8_t x, uint8_t y, DOG_PGM_P font, DOG_PSTR_P s); /* dogmdsp.c */
uint8_t dog_DrawRStrP(uint8_t x, uint8_t y, uint8_t rot, DOG_PGM_P font, DOG_PSTR_P s);


/* --- math --- */

  /* w: 0..255     64 = PI/2, 128 = PI, 192 = 3*PI/2 */
  /* returns: -64 .. 64 */
signed char dog_sin(uint8_t w);	/* dogmma.c */
signed char dog_cos(uint8_t w);	/* dogmma.c */

/* --- other --- */
char *dog_itoa(unsigned long v);

uint8_t dog_GetSPIByte(void);		/* dogmoi.c */


/* --- breakout --- */

void bo_Setup(uint8_t level);
void bo_Draw(void);
void bo_Step(void);
void bo_SetX(uint8_t pos);
void bo_DrawFPS(uint8_t fps);

/* --- walk --- */

void walk_Draw(void);
void walk_Step(void);

/* --- spacetrash --- */

void st_Draw(uint8_t fps);	/* use value 0 to suppress output of fps (frames per second) value */
void st_Step(uint8_t player_pos, uint8_t is_auto_fire, uint8_t is_fire);

/*--- chess ---*/
#define CHESS_KEY_NONE 0
#define CHESS_KEY_NEXT 1
#define CHESS_KEY_PREV 2
#define CHESS_KEY_SELECT 3
#define CHESS_KEY_BACK 4

/*--- libinfo.c ---*/
void libinfo_draw(void);

#ifdef __cplusplus
}
#endif


#endif 

