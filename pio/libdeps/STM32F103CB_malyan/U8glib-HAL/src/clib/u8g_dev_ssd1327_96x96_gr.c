/*

  u8g_dev_ssd1327_96x96_gr.c

  2-Bit (graylevel) Driver for SSD1327 Controller (OLED Display)
  Tested with Seedstudio 96x96 Oled (LY120)
  http://www.seeedstudio.com/wiki/index.php?title=Twig_-_OLED_96x96

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

#define WIDTH 96
#define HEIGHT 96
#define XOFFSET 8

/*
  http://www.seeedstudio.com/wiki/index.php?title=Twig_-_OLED_96x96
*/
static const uint8_t u8g_dev_ssd1327_2bit_96x96_init_seq[] PROGMEM = {
  U8G_ESC_DLY(10),              /* delay 10 ms */
  U8G_ESC_CS(0),                 /* disable chip */
  U8G_ESC_ADR(0),               /* instruction mode */
  U8G_ESC_RST(1),               /* do reset low pulse with (1*16)+2 milliseconds */
  U8G_ESC_CS(1),                /* enable chip */
  0x0fd, 0x012,			/* unlock display, usually not required because the display is unlocked after reset */
  0x0ae,             			/* display off, sleep mode */
  0x0a8, 0x05f,			/* multiplex ratio: 0x05f * 1/64 duty */
  0x0a1, 0x000,            		/* display start line */
  0x0a2, 0x060,           		/* display offset, shift mapping ram counter */
  //0x0a2, 0x04c,           		/* NHD: display offset, shift mapping ram counter */
  0x0a0, 0x046,  			/* remap configuration, vertical address increment, enable nibble remap (upper nibble is left) */
  //0x0a0, 0x056,  			/* NHD: remap configuration, vertical address increment, enable nibble remap (upper nibble is left) */
  0x0ab, 0x001,			/* Enable internal VDD regulator (RESET) */
  0x081, 0x053,          		/* contrast, brightness, 0..128, Newhaven: 0x040, LY120 0x053, 0x070 seems also ok */
  0x0b1, 0x051,          		/* phase length */
  0x0b3, 0x001,           		/* set display clock divide ratio/oscillator frequency */
  0x0b9,					/* use linear lookup table */
#if 0
  0x0b8,                                /* set gray scale table */
      //0x01, 0x011, 0x022, 0x032, 0x043, 0x054, 0x065, 0x076,
      0x01, 0x011, 0x022, 0x032, 0x043, 0x054, 0x077, 0x077,            // 4L mode uses 0, 2, 4, 7
#endif
  0x0bc, 0x008,                    	/* pre-charge voltage level */
  0x0be, 0x007,                     	/* VCOMH voltage */
  0x0b6, 0x001,			/* second precharge */
  0x0d5, 0x062,			/* enable second precharge, internal vsl (bit0 = 0) */

#if 0
  // the following commands are not used by the SeeedGrayOLED sequence */
  0x0ad, 0x002,                     /* master configuration: disable embedded DC-DC, enable internal VCOMH */
  0x086,                                /* full current range (0x084, 0x085, 0x086) */
  0x0b2, 0x051,                    /* frame frequency (row period) */
  0x0b4, 0x002,                    /* set pre-charge compensation level (not documented in the SDD1325 datasheet, but used in the NHD init seq.) */
  0x0b0, 0x028,                    /* enable pre-charge compensation (not documented in the SDD1325 datasheet, but used in the NHD init seq.) */
  0x0bf, 0x002|0x00d,           /* VSL voltage level (not documented in the SDD1325 datasheet, but used in the NHD init seq.) */
#endif

  0x0a5,                                 /* all pixel on */
  //0x02e,					/* no scroll (according to SeeedGrayOLED sequence) */
  0x0af,                                  /* display on */
  U8G_ESC_DLY(100),             /* delay 100 ms */
  0x0a4,                                 /* normal display mode */
  U8G_ESC_DLY(100),             /* delay 100 ms */
  0x0a5,                                 /* all pixel on */
  0x0af,                                  /* display on */
  U8G_ESC_DLY(100),             /* delay 100 ms */
  0x0a4,                                 /* normal display mode */

  0x015,       /* column address... */
  0x008,       /* start at column 8, special for the LY120 ??? */
  0x037,       /* end at column 55, note: there are two pixel in one column */

  0x075,       /* row address... */
  0x008,
  0x05f,

  U8G_ESC_ADR(1),               /* data mode */
  0x000f, 0x000f, 0x0000, 0x0000, 0x000f,0x000f,0x0000,0x0000,
  0x000f, 0x000f, 0x0000, 0x0000, 0x000f,0x000f,0x0000,0x0000,
  0x000f, 0x000f, 0x0000, 0x0000, 0x000f,0x000f,0x0000,0x0000,
  0x000f, 0x000f, 0x0000, 0x0000, 0x000f,0x000f,0x0000,0x0000,

  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_ssd1327_2bit_96x96_prepare_page_seq[] PROGMEM = {
  U8G_ESC_ADR(0),               /* instruction mode */
  U8G_ESC_CS(1),                /* enable chip */
  0x015,       /* column address... */
  XOFFSET,       /* start at column 8, special for the LY120 ??? */
  0x037,       /* end at column 55, note: there are two pixel in one column */
  0x075,       /* row address... */
  U8G_ESC_END                /* end of sequence */
};


static void u8g_dev_ssd1327_2bit_prepare_page(u8g_t *u8g, u8g_dev_t *dev)
{
  uint8_t page = ((u8g_pb_t *)(dev->dev_mem))->p.page;

  u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1327_2bit_96x96_prepare_page_seq);

  page <<= 2;
  u8g_WriteByte(u8g, dev, page);       /* start at the selected page */
  page += 3;
  u8g_WriteByte(u8g, dev, page);       /* end within the selected page */

  u8g_SetAddress(u8g, dev, 1);          /* data mode */
}

static void u8g_dev_ssd1327_2bit_2x_prepare_page(u8g_t *u8g, u8g_dev_t *dev, uint8_t is_odd)
{
  uint8_t page = ((u8g_pb_t *)(dev->dev_mem))->p.page;

  u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1327_2bit_96x96_prepare_page_seq);

  page <<= 1;
  page += is_odd;

  page <<= 2;
  u8g_WriteByte(u8g, dev, page);       /* start at the selected page */
  page += 3;
  u8g_WriteByte(u8g, dev, page);       /* end within the selected page */

  u8g_SetAddress(u8g, dev, 1);          /* data mode */
}

/* assumes row autoincrement and activated nibble remap */
static  void u8g_dev_ssd1327_2bit_write_4_pixel(u8g_t *u8g, u8g_dev_t *dev, uint8_t left, uint8_t right)
{
  uint8_t d, tmp, cnt;
  static uint8_t buf[4];
  buf[0] = 0;
  buf[1] = 0;
  buf[2] = 0;
  buf[3] = 0;
  cnt = 0;
  do
  {
    if ( left == 0 && right == 0 )
      break;
    d = left;
    d &= 3;
    d <<= 4;
    tmp = right;
    tmp &= 3;
    d |= tmp;
    d <<= 2;
    buf[cnt] = d;
    left >>= 2;
    right >>= 2;
    cnt++;
  }while ( cnt < 4 );
  u8g_WriteSequence(u8g, dev, 4, buf);
}

static void u8g_dev_ssd1327_2bit_write_buffer(u8g_t *u8g, u8g_dev_t *dev)
{
  uint8_t cnt, left, right;
  uint8_t *ptr;
  u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

  cnt = pb->width;
  cnt >>= 1;
  ptr = pb->buf;
  do
  {
    left = *ptr++;
    right = *ptr++;
    u8g_dev_ssd1327_2bit_write_4_pixel(u8g, dev, left, right);
    cnt--;
  } while( cnt > 0 );
}

static void u8g_dev_ssd1327_2bit_2x_write_buffer(u8g_t *u8g, u8g_dev_t *dev, uint8_t is_odd)
{
  uint8_t cnt, left, right;
  uint8_t *ptr;
  u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

  ptr = pb->buf;
  cnt = pb->width;
  if ( is_odd )
    ptr += cnt;
  cnt >>= 1;
  do
  {
    left = *ptr++;
    right = *ptr++;
    u8g_dev_ssd1327_2bit_write_4_pixel(u8g, dev, left, right);
    cnt--;
  } while( cnt > 0 );
}

uint8_t u8g_dev_ssd1327_96x96_gr_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1327_2bit_96x96_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_dev_ssd1327_2bit_prepare_page(u8g, dev);
        u8g_dev_ssd1327_2bit_write_buffer(u8g, dev);
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 1);
      u8g_SetChipSelect(u8g, dev, 0);
      return 1;
  }
  return u8g_dev_pb8v2_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_ssd1327_96x96_2x_gr_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1327_2bit_96x96_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_dev_ssd1327_2bit_2x_prepare_page(u8g, dev, 0);
        u8g_dev_ssd1327_2bit_2x_write_buffer(u8g, dev, 0);
        u8g_dev_ssd1327_2bit_2x_prepare_page(u8g, dev, 1);
        u8g_dev_ssd1327_2bit_2x_write_buffer(u8g, dev, 1);
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 1);
      u8g_SetChipSelect(u8g, dev, 0);
      return 1;
  }
  return u8g_dev_pb16v2_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_ssd1327_96x96_gr_sw_spi , WIDTH, HEIGHT, 4, u8g_dev_ssd1327_96x96_gr_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_ssd1327_96x96_gr_hw_spi , WIDTH, HEIGHT, 4, u8g_dev_ssd1327_96x96_gr_fn, U8G_COM_HW_SPI);
U8G_PB_DEV(u8g_dev_ssd1327_96x96_gr_i2c , WIDTH, HEIGHT, 4, u8g_dev_ssd1327_96x96_gr_fn, U8G_COM_SSD_I2C);

#define DWIDTH (2*WIDTH)
uint8_t u8g_dev_ssd1327_96x96_2x_buf[DWIDTH] U8G_NOCOMMON ;
u8g_pb_t u8g_dev_ssd1327_96x96_2x_pb = { {8, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_ssd1327_96x96_2x_buf};
u8g_dev_t u8g_dev_ssd1327_96x96_2x_gr_sw_spi = { u8g_dev_ssd1327_96x96_2x_gr_fn, &u8g_dev_ssd1327_96x96_2x_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_ssd1327_96x96_2x_gr_hw_spi = { u8g_dev_ssd1327_96x96_2x_gr_fn, &u8g_dev_ssd1327_96x96_2x_pb, U8G_COM_HW_SPI };
u8g_dev_t u8g_dev_ssd1327_96x96_2x_gr_i2c = { u8g_dev_ssd1327_96x96_2x_gr_fn, &u8g_dev_ssd1327_96x96_2x_pb, U8G_COM_SSD_I2C };

