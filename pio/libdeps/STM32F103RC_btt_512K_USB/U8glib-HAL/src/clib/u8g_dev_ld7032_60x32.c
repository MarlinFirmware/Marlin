/*

  u8g_dev_ld7032_60x32.c

  60x32 OLED display

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

#include "u8g.h"

/* define width as 64, so that it is a multiple of 8 */
#define WIDTH 64
#define HEIGHT 32
#define PAGE_HEIGHT 8

static const uint8_t u8g_dev_ld7032_60x32_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_DLY(1),       	/* delay 1 ms */
  U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
  U8G_ESC_CS(1),             /* enable chip */


  U8G_ESC_ADR(0),           /* instruction mode */
  0x002,				/* Dot Matrix Display ON/OFF */
  U8G_ESC_ADR(1),           /* data mode */
  0x001,				/* ON */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x014,				/* Dot Matrix Display Stand-by ON/OFF */
  U8G_ESC_ADR(1),           /* data mode */
  0x000,				/* ON */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x01a,				/* Dot Matrix Frame Rate */
  U8G_ESC_ADR(1),           /* data mode */
  0x004,				/* special value for this OLED from manual */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x01d,				/* Graphics Memory Writing Direction */
  U8G_ESC_ADR(1),           /* data mode */
  0x000,				/* reset default (right down, horizontal) */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x009,				/* Display Direction */
  U8G_ESC_ADR(1),           /* data mode */
  0x000,				/* reset default (x,y: min --> max) */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x030,				/* Display Size X */
  U8G_ESC_ADR(1),           /* data mode */
  0x000,				/* Column Start Output */
  0x03b,				/* Column End Output */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x032,				/* Display Size Y */
  U8G_ESC_ADR(1),           /* data mode */
  0x000,				/* Row Start Output */
  0x01f,				/* Row End Output */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x010,				/* Peak Pulse Width Set */
  U8G_ESC_ADR(1),           /* data mode */
  0x000,				/* 0 SCLK */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x016,				/* Peak Pulse Delay Set */
  U8G_ESC_ADR(1),           /* data mode */
  0x000,				/* 0 SCLK */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x012,				/* Dot Matrix Current Level Set */
  U8G_ESC_ADR(1),           /* data mode */
  0x050,				/* 0x050 * 1 uA = 80 uA */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x018,				/* Pre-Charge Pulse Width */
  U8G_ESC_ADR(1),           /* data mode */
  0x003,				/* 3 SCLK */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x044,				/* Pre-Charge Mode */
  U8G_ESC_ADR(1),           /* data mode */
  0x002,				/* Every Time */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x048,				/* Row overlap timing */
  U8G_ESC_ADR(1),           /* data mode */
  0x003,				/* Pre-Charge + Peak Delay + Peak boot Timing */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x03f,				/* VCC_R_SEL */
  U8G_ESC_ADR(1),           /* data mode */
  0x011,				/* ??? */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x03d,				/* VSS selection */
  U8G_ESC_ADR(1),           /* data mode */
  0x000,				/* 2.8V */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x002,				/* Dot Matrix Display ON/OFF */
  U8G_ESC_ADR(1),           /* data mode */
  0x001,				/* ON */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x008,				/* write data */

  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

/* use box commands to set start adr */
static const uint8_t u8g_dev_ld7032_60x32_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */


  U8G_ESC_ADR(0),           /* instruction mode */
  0x034,				/* box x start */
  U8G_ESC_ADR(1),           /* data mode */
  0x000,				/* 0 */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x035,				/* box x end */
  U8G_ESC_ADR(1),           /* data mode */
  0x007,				/*  */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x037,				/* box y end */
  U8G_ESC_ADR(1),           /* data mode */
  0x01f,				/*  */

  U8G_ESC_ADR(0),           /* instruction mode */
  0x036,				/* box y start */
  U8G_ESC_ADR(1),           /* data mode */

  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_ld7032_60x32_sleep_on[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  /* ... */
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_ld7032_60x32_sleep_off[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  /* ... */
  U8G_ESC_DLY(50),       /* delay 50 ms */
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_ld7032_60x32_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_400NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ld7032_60x32_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_ld7032_60x32_data_start);
        u8g_WriteByte(u8g, dev, pb->p.page_y0); /* y start */
	u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
	u8g_WriteByte(u8g, dev, 0x008);
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
          return 0;
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 2);
      u8g_SetChipSelect(u8g, dev, 0);
      return 1;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ld7032_60x32_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ld7032_60x32_sleep_off);
      return 1;
  }
  return u8g_dev_pb8h1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_ld7032_60x32_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ld7032_60x32_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_ld7032_60x32_hw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ld7032_60x32_fn, U8G_COM_HW_SPI);
U8G_PB_DEV(u8g_dev_ld7032_60x32_parallel, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ld7032_60x32_fn, U8G_COM_PARALLEL);
U8G_PB_DEV(u8g_dev_ld7032_60x32_hw_usart_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ld7032_60x32_fn, U8G_COM_HW_USART_SPI);

