/*

  u8g_dev_ssd1322_nhd31oled_gr.c

  2-Bit (4L) Driver for SSD1322 Controller (OLED Display)
  Tested with NHD-3.12-25664

  Universal 8bit Graphics Library

  Copyright (c) 2012, olikraus@gmail.com
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

  SSD130x       Monochrom OLED Controller
  SSD131x       Character OLED Controller
  SSD132x       Graylevel OLED Controller
  SSD1331       Color OLED Controller

*/

#include "u8g.h"

/* width must be multiple of 8, largest value is 248 unless u8g 16 bit mode is enabled */
#ifdef U8G_16BIT
#define WIDTH 256
#else
#define WIDTH 248
#endif
#define HEIGHT 64
//#define PAGE_HEIGHT 8

/*
  http://www.newhavendisplay.com/app_notes/OLED_25664.txt
  http://www.newhavendisplay.com/forum/viewtopic.php?f=15&t=3758
*/

static const uint8_t u8g_dev_ssd1322_2bit_nhd_312_init_seq[] PROGMEM = {
  U8G_ESC_DLY(10),              /* delay 10 ms */
  U8G_ESC_CS(0),                 /* disable chip */
  U8G_ESC_ADR(0),               /* instruction mode */
  U8G_ESC_RST(1),               /* do reset low pulse with (1*16)+2 milliseconds */
  U8G_ESC_CS(1),                /* enable chip */

  U8G_ESC_DLY(100),             /* delay 100 ms */
  U8G_ESC_DLY(100),             /* delay 100 ms */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0fd,					/* lock command */
  U8G_ESC_ADR(1),               	/* data mode */
  0x012,					/* unlock */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0ae,                               	 /* display off, sleep mode */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0b3,
  U8G_ESC_ADR(1),               	/* data mode */
  0x091,                    /* set display clock divide ratio/oscillator frequency (set clock as 80 frames/sec) */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0ca, 					/* multiplex ratio */
  U8G_ESC_ADR(1),               	/* data mode */
  0x03f,                    		/* 1/64 Duty (0x0F~0x3F) */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0a2,
  U8G_ESC_ADR(1),               	/* data mode */
  0x000,                     		/* display offset, shift mapping ram counter */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0a1,
  U8G_ESC_ADR(1),               	/* data mode */
  0x000,                     		/* display start line */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0a0, 					/* Set Re-Map / Dual COM Line Mode */
  U8G_ESC_ADR(1),               	/* data mode */
  0x014, 					/* was 0x014 */
  0x011, 					/* was 0x011 */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0ab,
  U8G_ESC_ADR(1),               	/* data mode */
  0x001,                     		/* Enable Internal VDD Regulator */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0b4, 					/* Display Enhancement A */
  U8G_ESC_ADR(1),               	/* data mode */
  0x0a0,
  0x005|0x0fd,

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0c1,					/* contrast */
  U8G_ESC_ADR(1),               	/* data mode */
  0x09f,

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0c7,					/* Set Scale Factor of Segment Output Current Control */
  U8G_ESC_ADR(1),               	/* data mode */
  0x00f,

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0b9,                               	 /* linear gray scale */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0b1,					/* Phase 1 (Reset) & Phase 2 (Pre-Charge) Period Adjustment */
  U8G_ESC_ADR(1),               	/* data mode */
  0x0e2,

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0d1, 					/* Display Enhancement B */
  U8G_ESC_ADR(1),               	/* data mode */
  0x082|0x020,
  0x020,

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0bb,					/* precharge  voltage */
  U8G_ESC_ADR(1),               	/* data mode */
  0x01f,

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0b6,					/* precharge period */
  U8G_ESC_ADR(1),               	/* data mode */
  0x008,

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0be,					/* vcomh */
  U8G_ESC_ADR(1),               	/* data mode */
  0x007,

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0a6,                               	 /* normal display */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0a9,                               	 /* exit partial display */

  U8G_ESC_ADR(0),               	/* instruction mode */
  0x0af,                               	 /* display on */


  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_ssd1322_2bit_nhd_312_prepare_page_seq[] PROGMEM = {
  U8G_ESC_ADR(0),               /* instruction mode */
  U8G_ESC_CS(1),                /* enable chip */
  0x015,       /* column address... */
  U8G_ESC_ADR(1),               /* data mode */
  0x01c,       /* start at column 0 */
  0x05b,       /* end column */
  U8G_ESC_ADR(0),               /* instruction mode */
  0x075,       /* row address... */
  U8G_ESC_ADR(1),               /* data mode */
  U8G_ESC_END                /* end of sequence */
};

static void u8g_dev_ssd1322_2bit_prepare_row(u8g_t *u8g, u8g_dev_t *dev, uint8_t delta_row)
{
  uint8_t row = ((u8g_pb_t *)(dev->dev_mem))->p.page;

  row *= ((u8g_pb_t *)(dev->dev_mem))->p.page_height;
  row += delta_row;

  u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1322_2bit_nhd_312_prepare_page_seq);

  u8g_WriteByte(u8g, dev, row);       /* start at the selected row */
  u8g_WriteByte(u8g, dev, row+1);       /* end within the selected row */

  u8g_SetAddress(u8g, dev, 0);          /* instruction mode mode */
  u8g_WriteByte(u8g, dev, 0x05c);       /* write to ram */
  u8g_SetAddress(u8g, dev, 1);          /* data mode */
}

static const uint8_t u8g_dev_ssd13xx_sleep_on[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x0ae,		/* display off */
  U8G_ESC_CS(0),             /* disable chip, bugfix 12 nov 2014 */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_ssd13xx_sleep_off[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x0af,		/* display on */
  U8G_ESC_DLY(50),       /* delay 50 ms */
  U8G_ESC_CS(0),             /* disable chip, bugfix 12 nov 2014 */
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_ssd1322_nhd31oled_gr_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1322_2bit_nhd_312_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
	uint8_t i;
	u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
	uint8_t *p = pb->buf;
	u8g_uint_t cnt;
	cnt = pb->width;
	cnt >>= 2;

	for( i = 0; i < pb->p.page_height; i++ )
	{
	  u8g_dev_ssd1322_2bit_prepare_row(u8g, dev, i);  /* this will also enable chip select */
#ifndef U8G_16BIT
	  u8g_WriteByte(u8g, dev, 0x00);
	  u8g_WriteByte(u8g, dev, 0x00);
#endif
	  u8g_WriteSequence4LTo16GrDevice(u8g, dev, cnt, p);
#ifndef U8G_16BIT
	  u8g_WriteByte(u8g, dev, 0x00);
	  u8g_WriteByte(u8g, dev, 0x00);
#endif
	  u8g_MicroDelay();	// for DUE?
	  u8g_SetChipSelect(u8g, dev, 0);
	  p+=cnt;
	}
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_SetAddress(u8g, dev, 1);          /* data mode */
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 1);
      u8g_MicroDelay();	// for DUE?
      u8g_SetChipSelect(u8g, dev, 0);
      break;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_off);
      return 1;
  }
  return u8g_dev_pb8h2_base_fn(u8g, dev, msg, arg);
}


uint8_t u8g_dev_ssd1322_nhd31oled_2x_gr_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1322_2bit_nhd_312_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
	uint8_t i;
	u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
	uint8_t *p = pb->buf;
	u8g_uint_t cnt;
	cnt = pb->width;
	cnt >>= 2;    /* 23 Oct 2013, changed to 2 */

	for( i = 0; i < pb->p.page_height; i++ )
	{
	  u8g_dev_ssd1322_2bit_prepare_row(u8g, dev, i);		/* this will also enable chip select */
#ifndef U8G_16BIT
	  u8g_WriteByte(u8g, dev, 0x00);
	  u8g_WriteByte(u8g, dev, 0x00);
#endif
	  u8g_WriteSequence4LTo16GrDevice(u8g, dev, cnt, p);
#ifndef U8G_16BIT
	  u8g_WriteByte(u8g, dev, 0x00);
	  u8g_WriteByte(u8g, dev, 0x00);
#endif
	  u8g_MicroDelay();	// for DUE?
	  u8g_SetChipSelect(u8g, dev, 0);
	  p+=cnt;
	}
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_SetAddress(u8g, dev, 1);          /* data mode */
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 1);
      u8g_SetChipSelect(u8g, dev, 0);
      break;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_off);
      return 1;
  }
  return u8g_dev_pb16h2_base_fn(u8g, dev, msg, arg);
}


U8G_PB_DEV(u8g_dev_ssd1322_nhd31oled_gr_sw_spi , WIDTH, HEIGHT, 4, u8g_dev_ssd1322_nhd31oled_gr_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_ssd1322_nhd31oled_gr_hw_spi , WIDTH, HEIGHT, 4, u8g_dev_ssd1322_nhd31oled_gr_fn, U8G_COM_HW_SPI);
U8G_PB_DEV(u8g_dev_ssd1322_nhd31oled_gr_parallel , WIDTH, HEIGHT, 4, u8g_dev_ssd1322_nhd31oled_gr_fn, U8G_COM_FAST_PARALLEL);


#define DWIDTH (WIDTH*2)
uint8_t u8g_dev_ssd1322_nhd31oled_2x_gr_buf[DWIDTH] U8G_NOCOMMON ;
u8g_pb_t u8g_dev_ssd1322_nhd31oled_2x_gr_pb = { {8, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_ssd1322_nhd31oled_2x_gr_buf};
u8g_dev_t u8g_dev_ssd1322_nhd31oled_2x_gr_sw_spi = { u8g_dev_ssd1322_nhd31oled_2x_gr_fn, &u8g_dev_ssd1322_nhd31oled_2x_gr_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_ssd1322_nhd31oled_2x_gr_hw_spi = { u8g_dev_ssd1322_nhd31oled_2x_gr_fn, &u8g_dev_ssd1322_nhd31oled_2x_gr_pb, U8G_COM_HW_SPI };

