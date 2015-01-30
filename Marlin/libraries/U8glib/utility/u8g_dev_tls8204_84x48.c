/*

  u8g_dev_tls8204_84x48.c
  
  Display: Nokia 84x48
  
  Status: Tested with TLS8204V12 Display by Olimex MOD-LCD3310
  
  Contributed: http://code.google.com/p/u8glib/issues/detail?id=126

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

#define WIDTH 84
#define HEIGHT 48
#define PAGE_HEIGHT 8


static const uint8_t u8g_dev_tls8204_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
  U8G_ESC_CS(1),             /* enable chip */
  0x021,		/* activate chip (PD=0), horizontal increment (V=0), enter extended command set (H=1) */
  0x006,		/* temp. control: b10 = 2 */
  0x04 | !!((66-1)&(1u<<6)),
  0x40 | ((66-2) & ((1u<<6)-1)),
  0x013,		/* bias system 1:48 */
  0x0c0,		/* medium Vop */
  0x020,		/* activate chip (PD=0), horizontal increment (V=0), enter normal command set (H=0) */
  0x00c,		/* display on, normal operation */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x020,		                /* activate chip (PD=0), horizontal increment (V=0), enter normal command set (H=0) */
  0x00d,		                /* display on, invert */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x020,		                /* activate chip (PD=0), horizontal increment (V=0), enter normal command set (H=0) */
  0x00c,		                /* display on, normal */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};


uint8_t u8g_dev_tls8204_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_400NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_tls8204_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_SetAddress(u8g, dev, 0);           /* command mode */
        u8g_SetChipSelect(u8g, dev, 1);
        u8g_WriteByte(u8g, dev, 0x020 );		/* activate chip (PD=0), horizontal increment (V=0), enter normal command set (H=0) */
        u8g_WriteByte(u8g, dev, 0x080 );                        /* set X address */
        u8g_WriteByte(u8g, dev, 0x040 | pb->p.page); /* set Y address */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
          return 0;
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      /* the contrast adjustment does not work, needs to be analysed */
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_SetChipSelect(u8g, dev, 1);
      u8g_WriteByte(u8g, dev, 0x021);        /* command mode, extended function set */
      u8g_WriteByte(u8g, dev, 0x080 | ( (*(uint8_t *)arg) >> 1 ) );
      u8g_WriteByte(u8g, dev, 0x020);        /* command mode, extended function set */
      u8g_SetChipSelect(u8g, dev, 0);
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}


U8G_PB_DEV(u8g_dev_tls8204_84x48_sw_spi , WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_tls8204_fn, U8G_COM_SW_SPI);

