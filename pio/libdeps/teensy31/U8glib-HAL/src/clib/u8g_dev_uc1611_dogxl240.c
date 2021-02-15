/*

  u8g_dev_uc1611_dogxl240.c

  Universal 8bit Graphics Library

  Copyright (c) 2014, dev.menges.jonas@gmail.com, olikraus@gmail.com
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

#include "u8g.h"


#define WIDTH 240
#define HEIGHT 128
#define PAGE_HEIGHT 8


static const uint8_t u8g_dev_uc1611_dogxl240_init_seq[] PROGMEM = {
  U8G_ESC_CS(1),             // enable chip
  U8G_ESC_ADR(0),           // instruction mode
  0xF1,     // set last COM electrode
  0x7F,     // DOGXL240
  0xF2,     // set display start line
  0x00,     // 0
  0xF3,     // set display end line
  0x7F,     // DOGXL240
  0x81,     // set contrast (0-255)
  0xAA,     // DOGXL240
  0xC0,     // set view
  //0x04,     // topview
  0x02,     // bottomview
  0xA3,     // set line rate (9.4k)
  0xE9,     // set bias ratio (10)
  0xA9,     // enable display
  0xD1,      // set black and white mode
  U8G_ESC_CS(0),             // disable chip
  U8G_ESC_END                // end of sequence
};

static void u8g_dev_dogxl240_set_page(u8g_t *u8g, u8g_dev_t *dev, unsigned char page)
{
  u8g_WriteByte(u8g, dev, 0x70);
  u8g_WriteByte(u8g, dev, 0x60 + (page&0x0F));
}

static const uint8_t u8g_dev_uc1611_dogxl240_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),            /* enable chip */
  0x10,                     /* set upper 4 bit of the col adr to 0 */
  0x00,                     /* set lower 4 bit of the col adr to 0 */
  U8G_ESC_END               /* end of sequence */
};

static uint8_t u8g_dev_uc1611_dogxl240_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1611_dogxl240_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
      u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1611_dogxl240_data_start);
      u8g_dev_dogxl240_set_page(u8g, dev, pb->p.page);   /* select current page (uc1611) */
      u8g_SetAddress(u8g, dev, 1);     /* data mode */
      if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
        return 0;
      u8g_SetChipSelect(u8g, dev, 1);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 0);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x81);
      /* 11 Jul 2015: bugfix, github issue 339 */
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) );	/* set contrast from, keep gain at 0 */
      u8g_SetChipSelect(u8g, dev, 1);
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_uc1611_dogxl240_i2c , WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_uc1611_dogxl240_fn, U8G_COM_UC_I2C);
U8G_PB_DEV(u8g_dev_uc1611_dogxl240_sw_spi , WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_uc1611_dogxl240_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_uc1611_dogxl240_hw_spi , WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_uc1611_dogxl240_fn, U8G_COM_HW_SPI);
U8G_PB_DEV(u8g_dev_uc1611_dogxl240_8bit , WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_uc1611_dogxl240_fn, U8G_COM_FAST_PARALLEL);

