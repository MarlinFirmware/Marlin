/*

  u8g_dev_ssd1309_128x64.c

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

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8


/* ssd1309 ini sequence*/
static const uint8_t u8g_dev_ssd1309_128x64_init_seq[] PROGMEM={
	U8G_ESC_CS(0),             /* disable chip */
	U8G_ESC_ADR(0),           /* instruction mode */
	U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
	U8G_ESC_CS(1),             /* enable chip */
	
	0xfd,0x12,		/*Command Lock */
	0xae,			/*Set Display Off */
	0xd5,0xa0,		/*set Display Clock Divide Ratio/Oscillator Frequency */
	0xa8,0x3f,		/*Set Multiplex Ratio */
	0x3d,0x00,		/*Set Display Offset*/
	0x40,			/*Set Display Start Line*/
	0xa1,			/*Set Segment Re-Map*/
	0xc8,			/*Set COM Output Scan Direction*/
	0xda,0x12,		/*Set COM Pins Hardware Configuration*/
	0x81,0xdf,		/*Set Current Control */
	0xd9,0x82,		/*Set Pre-Charge Period */
	0xdb,0x34,		/*Set VCOMH Deselect Level */
	0xa4,			/*Set Entire Display On/Off */
	0xa6,			/*Set Normal/Inverse Display*/
	U8G_ESC_VCC(1),	/*Power up VCC & Stabilized */
	U8G_ESC_DLY(50),
	0xaf,			/*Set Display On */
	U8G_ESC_DLY(50),
	U8G_ESC_CS(0),             /* disable chip */
	U8G_ESC_END                /* end of sequence */
};

/* select one init sequence here */
  #define u8g_dev_ssd1309_128x64_init_seq u8g_dev_ssd1309_128x64_init_seq
  
  
 static const uint8_t u8g_dev_ssd1309_128x64_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x010,		/* set upper 4 bit of the col adr to 0 */
  0x000,		/* set lower 4 bit of the col adr to 4  */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_ssd13xx_sleep_on[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x0ae,		/* display off */      
  U8G_ESC_CS(1),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_ssd13xx_sleep_off[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x0af,		/* display on */      
  U8G_ESC_DLY(50),       /* delay 50 ms */
  U8G_ESC_CS(1),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_ssd1309_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1309_128x64_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1309_128x64_data_start);    
        u8g_WriteByte(u8g, dev, 0x0b0 | pb->p.page); /* select current page (SSD1306) */
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
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_on);    
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_off);    
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_ssd1309_128x64_hw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ssd1309_128x64_fn, U8G_COM_HW_SPI);
U8G_PB_DEV(u8g_dev_ssd1309_128x64_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ssd1309_128x64_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_ssd1309_128x64_i2c, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ssd1309_128x64_fn, U8G_COM_SSD_I2C);
 
  