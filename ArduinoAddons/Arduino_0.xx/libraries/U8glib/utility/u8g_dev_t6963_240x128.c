/*

  u8g_dev_t6963_240x128.c
  
  Tested with Varitronix MGLS240128TZ
  
  Universal 8bit Graphics Library
  
  Copyright (c) 2013, olikraus@gmail.com
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


/*
  http://www.mark-products.com/graphics.htm#240x64%20Pixel%20Format
*/

static const uint8_t u8g_dev_t6963_240x128_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* data mode */
  U8G_ESC_RST(15),           /* do reset low pulse with (15*16)+2 milliseconds (=maximum delay)*/

  U8G_ESC_CS(1),             /* enable chip */
  U8G_ESC_DLY(50),         /* delay 50 ms */

  U8G_ESC_ADR(0),               /* data mode */
  0x000,                                /* low byte */
  0x000,                                /* height byte */
  U8G_ESC_ADR(1),               /* instruction mode */
  0x021,                                /* set cursor position */
  
  U8G_ESC_ADR(0),               /* data mode */
  0x000,                                /* low byte */
  0x000,                                /* height byte */
  U8G_ESC_ADR(1),               /* instruction mode */
  0x022,                                /* set offset */

  U8G_ESC_ADR(0),               /* data mode */
  0x000,                                /* low byte */
  0x000,                                /* height byte */
  U8G_ESC_ADR(1),               /* instruction mode */
  0x040,				     /* text home */

  U8G_ESC_ADR(0),               /* data mode */
  WIDTH/8,                                /* low byte */
  0x000,                                /* height byte */
  U8G_ESC_ADR(1),               /* instruction mode */
  0x041,				     /* text columns */

  U8G_ESC_ADR(0),               /* data mode */
  0x000,                                /* low byte */
  0x000,                                /* height byte */
  U8G_ESC_ADR(1),               /* instruction mode */
  0x042,				     /* graphics home */

  U8G_ESC_ADR(0),               /* data mode */
  WIDTH/8,                                /* low byte */
  0x000,                                /* height byte */
  U8G_ESC_ADR(1),               /* instruction mode */
  0x043,				     /* graphics columns */
  
  U8G_ESC_ADR(1),               /* instruction mode */
  0x080,                                /* mode register: OR Mode, Internal Character Mode */
  U8G_ESC_ADR(1),               /* instruction mode */
  0x098,                                /* mode register: Display Mode, Graphics on, Text off, Cursor off */
  
  U8G_ESC_ADR(0),               /* data mode */
  0x000,                                /* low byte */
  0x000,                                /* height byte */
  U8G_ESC_ADR(1),               /* instruction mode */
  0x024,                                /* set adr pointer */
  
  U8G_ESC_ADR(1),               /* instruction mode */
  //0x0B0,                                /* auto write data */
  U8G_ESC_ADR(0),               /* data mode */
  0x055,
  U8G_ESC_ADR(1),               /* instruction mode */
  0x0c0,                                /* write data and increment */
  U8G_ESC_ADR(0),               /* data mode */  
  0x0aa,
  U8G_ESC_ADR(1),               /* instruction mode */
  0x0c0,                                /* write data and increment */
  //U8G_ESC_ADR(1),               /* instruction mode */
  //0x0B2,                                /* auto write data off */

  U8G_ESC_DLY(100),         /* delay 100 ms */
  
  U8G_ESC_ADR(0),               /* data mode */
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_t6963_240x128_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev);
    //for(;;)
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_t6963_240x128_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        uint8_t y, i;
        uint16_t disp_ram_adr;
        uint8_t *ptr;
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);


        
	u8g_SetAddress(u8g, dev, 0);           /* data mode */
        u8g_SetChipSelect(u8g, dev, 1);
        y = pb->p.page_y0;
        ptr = pb->buf;
        disp_ram_adr = WIDTH/8;
        disp_ram_adr *= y;
	disp_ram_adr += 512*12;
        for( i = 0; i < 8; i ++ )
        {
          u8g_SetAddress(u8g, dev, 0);           /* data mode */
          u8g_WriteByte(u8g, dev, disp_ram_adr&255 );      /* address low byte */
          u8g_WriteByte(u8g, dev, disp_ram_adr>>8 );      /* address hight byte */
          u8g_SetAddress(u8g, dev, 1);           /* cmd mode */
          u8g_WriteByte(u8g, dev, 0x024 );      /* set adr ptr */
          u8g_WriteByte(u8g, dev, 0x0b0 );      /* auto write data */
	  
          u8g_SetAddress(u8g, dev, 0);           /* data mode */
          u8g_WriteSequence(u8g, dev, WIDTH/8, ptr);
	  
          u8g_SetAddress(u8g, dev, 1);           /* cmd mode */
          u8g_WriteByte(u8g, dev, 0x0b2 );      /* auto write data off */
          ptr += WIDTH/8;
          disp_ram_adr += WIDTH/8;
        }
	u8g_SetAddress(u8g, dev, 0);           /* data mode */
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
  }
  return u8g_dev_pb8h1f_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_t6963_240x128_8bit, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_t6963_240x128_fn, U8G_COM_PARALLEL);


