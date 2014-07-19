/*

  u8g_dev_uc1608_240x64.c

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
#define HEIGHT 64
#define PAGE_HEIGHT 8

/* see also ERC24064-1 for init sequence example */
static const uint8_t u8g_dev_uc1608_240x64_init_seq[] PROGMEM = {
  U8G_ESC_CS(1),             /* disable chip (UC1608 has positive logic for CS) */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_RST(1),           /* do reset low pulse with (15*16)+2 milliseconds */
  
  
  U8G_ESC_CS(0),             /* enable chip */
  0x0e2,            		/* soft reset */
  
  U8G_ESC_DLY(100),       /* delay 100 ms */
  U8G_ESC_DLY(100),       /* delay 100 ms */
#if HEIGHT <= 96
  0x023,				/* Bit 0/1: Temp compenstation, Bit 2: Multiplex Rate 0=96, 1=128 */
#else
  /* 30 Nov 2013: not tested */
  0x027,				/* Bit 0/1: Temp compenstation, Bit 2: Multiplex Rate 0=96, 1=128 */
#endif  
  0x0c8,		                /* Map control, Bit 3: MY=1, Bit 2: MX=0, Bit 0: MSF =0 */
  0x0e8,		                /* LCD bias Bits 0/1: 00=10.7 01=10.3, 10=12.0, 11=12.7*/
  
  0x081,		                /* set contrast (bits 0..5) and gain (bits 6/7) */
  0x014,		                /* ECR24064-1 default: 0x040*/
  
  0x02f,		                /* power on, Bit 2 PC2=1 (internal charge pump), Bits 0/1: cap of panel */  
  U8G_ESC_DLY(50),         /* delay 50 ms */
  
  0x040,		                /* set display start line to 0 */
  0x090,		                /* no fixed lines */
  0x089,		                /* RAM access control  */
  
  0x0af,		                /* disable sleep mode  */
  0x0a4,		                /* normal display  */
  0x0a5,		                /* display all points, ST7565, UC1610 */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x0a4,		                /* normal display */
  U8G_ESC_CS(1),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_uc1608_240x64_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(0),             /* enable chip */
  0x010,		/* set upper 4 bit of the col adr to 0 (UC1608)  */
  0x000,		/* set lower 4 bit of the col adr to 0 */      
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_uc1608_240x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1608_240x64_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1608_240x64_data_start);    
        u8g_WriteByte(u8g, dev, 0x0b0 | pb->p.page); /* select current page (UC1608) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
          return 0;
        u8g_SetChipSelect(u8g, dev, 1);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      u8g_SetChipSelect(u8g, dev, 0);
      u8g_SetAddress(u8g, dev, 0);          /* instruction mode */
      u8g_WriteByte(u8g, dev, 0x081);
      u8g_WriteByte(u8g, dev, (*(uint8_t *)arg) >> 2);	/* set contrast from, keep gain at 0 */
      u8g_SetChipSelect(u8g, dev, 1);      
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_uc1608_240x64_2x_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1608_240x64_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
	
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1608_240x64_data_start);    
        u8g_WriteByte(u8g, dev, 0x0b0 | (2*pb->p.page)); /* select current page (ST7565R) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
	u8g_WriteSequence(u8g, dev, pb->width, pb->buf); 
        u8g_SetChipSelect(u8g, dev, 0);
	
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1608_240x64_data_start);    
        u8g_WriteByte(u8g, dev, 0x0b0 | (2*pb->p.page+1)); /* select current page (ST7565R) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
	u8g_WriteSequence(u8g, dev, pb->width, (uint8_t *)(pb->buf)+pb->width); 
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
  }
  return u8g_dev_pb16v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_uc1608_240x64_sw_spi , WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_uc1608_240x64_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_uc1608_240x64_hw_spi , WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_uc1608_240x64_fn, U8G_COM_HW_SPI);

uint8_t u8g_dev_uc1608_240x64_2x_buf[WIDTH*2] U8G_NOCOMMON ; 
u8g_pb_t u8g_dev_uc1608_240x64_2x_pb = { {16, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_uc1608_240x64_2x_buf}; 
u8g_dev_t u8g_dev_uc1608_240x64_2x_sw_spi = { u8g_dev_uc1608_240x64_2x_fn, &u8g_dev_uc1608_240x64_2x_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_uc1608_240x64_2x_hw_spi = { u8g_dev_uc1608_240x64_2x_fn, &u8g_dev_uc1608_240x64_2x_pb, U8G_COM_HW_SPI };

