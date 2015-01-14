/*

  u8g_dev_sbn1661_122x32.c
  
  WG12232 display with 2xSBN1661 / SED1520 controller (122x32 display)
  At the moment only available in the Arduino Environment

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

#define WIDTH 122
#define HEIGHT 32
#define PAGE_HEIGHT 8


static const uint8_t u8g_dev_sbn1661_122x32_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_RST(15),           /* do reset low pulse with (15*16)+2 milliseconds */
  U8G_ESC_CS(1),             /* enable chip 1 */
  0x0af,				/* display on */
  0x0c0,				/* display start at line 0 */
  0x0a0,				/* a0: ADC forward, a1: ADC reverse */
  0x0a9,				/* a8: 1/16, a9: 1/32 duty */
  U8G_ESC_CS(2),             /* enable chip 2 */
  0x0af,				/* display on */
  0x0c0,				/* display start at line 0 */
  0x0a0,				/* a0: ADC forward, a1: ADC reverse */
  0x0a9,				/* a8: 1/16, a9: 1/32 duty */
  
  U8G_ESC_CS(0),             /* disable chip */
  
  
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_sbn1661_122x32_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_sbn1661_122x32_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
	
        u8g_SetAddress(u8g, dev, 0);           /* command mode */
        u8g_SetChipSelect(u8g, dev, 1);
        u8g_WriteByte(u8g, dev, 0x0b8 | pb->p.page); /* select current page (SBN1661/SED1520) */
        u8g_WriteByte(u8g, dev, 0x000 ); /* set X address */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        u8g_WriteSequence(u8g, dev, WIDTH/2, pb->buf);
	
        u8g_SetAddress(u8g, dev, 0);           /* command mode */
        u8g_SetChipSelect(u8g, dev, 2);
        u8g_WriteByte(u8g, dev, 0x0b8 | pb->p.page); /* select current page (SBN1661/SED1520) */
        u8g_WriteByte(u8g, dev, 0x000 ); /* set X address */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        u8g_WriteSequence(u8g, dev, WIDTH/2, WIDTH/2+(uint8_t *)pb->buf);
	
        u8g_SetChipSelect(u8g, dev, 0);
	
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      break;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

/* u8g_com_arduino_sw_spi_fn does not work, too fast??? */
U8G_PB_DEV(u8g_dev_sbn1661_122x32 , WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_sbn1661_122x32_fn, u8g_com_arduino_no_en_parallel_fn);
