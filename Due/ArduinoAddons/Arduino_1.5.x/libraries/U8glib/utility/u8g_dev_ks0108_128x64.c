/*

  u8g_dev_ks0108_128x64.c

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
  

  ADDRESS = 0   (Command Mode)
    0x03f       Display On
    0x0c0       Start Display at line 0
    0x040 | y   write to y address (y:0..63)
    0x0b8 | x   write to page [0..7]


  u8g_Init8Bit(u8g, dev, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, cs2, di, rw, reset)
  u8g_Init8Bit(u8g, dev,  8,    9, 10, 11,   4,   5,   6,   7, 18, 14, 15, 17, 16, U8G_PIN_NONE)
  
*/

#include "u8g.h"

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

static const uint8_t u8g_dev_ks0108_128x64_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
  U8G_ESC_CS(1),             /* enable chip 1 */
  0x03f,		                /* display on */
  0x0c0,		                /* start at line 0 */
  U8G_ESC_DLY(20),         /* delay 20 ms */
  U8G_ESC_CS(2),             /* enable chip 2 */
  0x03f,		                /* display on */
  0x0c0,		                /* start at line 0 */
  U8G_ESC_DLY(20),         /* delay 20 ms */
  U8G_ESC_CS(0),             /* disable all chips */
  U8G_ESC_END                /* end of sequence */
};


uint8_t u8g_dev_ks0108_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_NONE);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ks0108_128x64_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        
        u8g_SetAddress(u8g, dev, 0);           /* command mode */
        u8g_SetChipSelect(u8g, dev, 2);
        u8g_WriteByte(u8g, dev, 0x0b8 | pb->p.page); /* select current page (KS0108b) */
        u8g_WriteByte(u8g, dev, 0x040 );		/* set address 0 */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        u8g_WriteSequence(u8g, dev, 64, pb->buf);
        u8g_SetChipSelect(u8g, dev, 0);
        
        u8g_SetAddress(u8g, dev, 0);           /* command mode */
        u8g_SetChipSelect(u8g, dev, 1);
        u8g_WriteByte(u8g, dev, 0x0b8 | pb->p.page); /* select current page (KS0108b) */
        u8g_WriteByte(u8g, dev, 0x040 );		/* set address 0 */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        u8g_WriteSequence(u8g, dev, 64, 64+(uint8_t *)pb->buf);
        u8g_SetChipSelect(u8g, dev, 0);
        
      }
      break;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_ks0108_128x64, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ks0108_128x64_fn, U8G_COM_PARALLEL);
U8G_PB_DEV(u8g_dev_ks0108_128x64_fast, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ks0108_128x64_fn, U8G_COM_FAST_PARALLEL);


