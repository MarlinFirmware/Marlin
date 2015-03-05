/*

  u8g_dev_lc7981_320x64.c

  Note: Requires 16 bit mode (Must be enabled in u8g.h)
  
  Tested with Varitronix MGLS32064-03.pdf
  
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
  
  
*/

#include "u8g.h"

#define WIDTH 320
#define HEIGHT 64
#define PAGE_HEIGHT 8


/*
  http://www.gaw.ru/pdf/lcd/lcm/Varitronix/graf/MGLS32064-03.pdf
*/

static const uint8_t u8g_dev_lc7981_320x64_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(1),           /* instruction mode */
  U8G_ESC_RST(15),           /* do reset low pulse with (15*16)+2 milliseconds (=maximum delay)*/
  U8G_ESC_CS(1),             /* enable chip */
  U8G_ESC_DLY(50),         /* delay 50 ms */
  
  
  U8G_ESC_ADR(1),               /* instruction mode */
  0x000,                                /* mode register */
  U8G_ESC_ADR(0),               /* data mode */
  0x032,                                /* display on (bit 5), master mode on (bit 4), graphics mode on (bit 1)*/

  U8G_ESC_ADR(1),               /* instruction mode */
  0x001,                                /* character/bits per pixel pitch */
  U8G_ESC_ADR(0),               /* data mode */
  0x007,                                /* 8 bits per pixel */

  U8G_ESC_ADR(1),               /* instruction mode */
  0x002,                                /* number of chars/byte width of the screen */
  U8G_ESC_ADR(0),               /* data mode */
  WIDTH/8-1,                         /* 8 bits per pixel */

  U8G_ESC_ADR(1),               /* instruction mode */
  0x003,                                /* time division */
  U8G_ESC_ADR(0),               /* data mode */
  0x07f,                                /*  */

  U8G_ESC_ADR(1),               /* instruction mode */
  0x008,                                /* display start low */
  U8G_ESC_ADR(0),               /* data mode */
  0x000,                                /*  */

  U8G_ESC_ADR(1),               /* instruction mode */
  0x009,                                /* display start high */
  U8G_ESC_ADR(0),               /* data mode */
  0x000,                                /*  */
    
  U8G_ESC_DLY(10),               /* delay 10 ms */
  
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_lc7981_320x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_lc7981_320x64_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        uint8_t y, i;
        uint16_t disp_ram_adr;
        uint8_t *ptr;
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        
        u8g_SetAddress(u8g, dev, 1);           /* cmd mode */
        u8g_SetChipSelect(u8g, dev, 1);
        y = pb->p.page_y0;
        ptr = pb->buf;
        disp_ram_adr = WIDTH/8;
        disp_ram_adr *= y;
        for( i = 0; i < 8; i ++ )
        {
          u8g_SetAddress(u8g, dev, 1);           /* cmd mode */
          u8g_WriteByte(u8g, dev, 0x00a );      /* display ram (cursor) address low byte */
          u8g_SetAddress(u8g, dev, 0);           /* data mode */
          u8g_WriteByte(u8g, dev, disp_ram_adr & 0x0ff );  

          u8g_SetAddress(u8g, dev, 1);           /* cmd mode */
          u8g_WriteByte(u8g, dev, 0x00b );      /* display ram (cursor) address hight byte */
          u8g_SetAddress(u8g, dev, 0);           /* data mode */
          u8g_WriteByte(u8g, dev, disp_ram_adr >> 8 );  
          
          u8g_SetAddress(u8g, dev, 1);           /* cmd mode */
          u8g_WriteByte(u8g, dev, 0x00c );      /* write data */
          u8g_SetAddress(u8g, dev, 0);           /* data mode */
          u8g_WriteSequence(u8g, dev, WIDTH/8, ptr);
          ptr += WIDTH/8;
          disp_ram_adr += WIDTH/8;
        }
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
  }
  return u8g_dev_pb8h1f_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_lc7981_320x64_8bit, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_lc7981_320x64_fn, U8G_COM_FAST_PARALLEL);
