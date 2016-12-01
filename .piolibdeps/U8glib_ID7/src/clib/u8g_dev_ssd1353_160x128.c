/*
  u8g_dev_ssd1353_160x128.c

  Universal 8bit Graphics Library
  
  Copyright (c) 2015, hugodan3@googlemail.com
  Copyright (c) 2013, jamjardavies@gmail.com
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
  
  History:
  Initial version	8 Mar 2015 hugodan3@googlemail.com. This version has
			been derived from the ssd1351 driver by jamjarda. It has
			been improved by in-lining time critical functions.

   
  
*/

#include "u8g.h"

#define WIDTH		160
#define HEIGHT		128
#define PAGE_HEIGHT	8

#define USE_GREY_TABLE	0

static const uint8_t u8g_dev_ssd1353_160x128_init_seq[] PROGMEM = {
	U8G_ESC_CS(0),					/* disable chip */
	U8G_ESC_DLY(50),
	U8G_ESC_ADR(0),					/* instruction mode */
	U8G_ESC_RST(1),					/* do reset low pulse with (1*16)+2 milliseconds */
	U8G_ESC_DLY(50),
	U8G_ESC_CS(1),					/* enable chip */

	0xFD,						/* Command unlock */
	U8G_ESC_ADR(1),
	0x12,						

	U8G_ESC_ADR(0),					
	0xAE,						/* Set Display Off */

	U8G_ESC_ADR(0),			
	0xA8,
	U8G_ESC_ADR(1),
	0x7F,						/* Set Multiplex Ratio */

	U8G_ESC_ADR(0),			
	0xA0,
	U8G_ESC_ADR(1),
	0xB4,						/* Set remapping */

	U8G_ESC_ADR(0),		
	0xA1,
	U8G_ESC_ADR(1),
	0x00,						/* Set Display Start Line */

	U8G_ESC_ADR(0),	
	0xA2,
	U8G_ESC_ADR(1),
	0x00,						/* Set Display Offset */

	U8G_ESC_ADR(0),
	0xB1,
	U8G_ESC_ADR(1),
	0x32,						/* Set Phase Length */

	U8G_ESC_ADR(0),	
	0xB4,
	U8G_ESC_ADR(1),
	0x04,						/* Set Second Precharge Period */

	U8G_ESC_ADR(0),
	0xA4,						/* Set Display Mode ON */

	U8G_ESC_ADR(0),
	0xB3,
	U8G_ESC_ADR(1),					/* frame rate  */
	0x40,
	
	U8G_ESC_ADR(0),	
	0xBB,	
	U8G_ESC_ADR(1),					/* pre-charge level */
	0x08,

	U8G_ESC_ADR(0),
	0xBE,
	U8G_ESC_ADR(1),					/* vcomh */
	0x3C,

	/* color adjustments */
#if USE_GREY_TABLE != 1
	U8G_ESC_ADR(0),					/* instruction mode */
	0x81,
	U8G_ESC_ADR(1),
	0xC8,							/* Set Contrast Color 1*/
	U8G_ESC_ADR(0),					/* instruction mode */
	0x82,
	U8G_ESC_ADR(1),
	0x80,							/* Set Contrast Color 2*/
	U8G_ESC_ADR(0),					/* instruction mode */
	0x83,
	U8G_ESC_ADR(1),
	0xF8,							/* Set Contrast Color 3*/
	U8G_ESC_ADR(0),					/* instruction mode */
	0x87,
	U8G_ESC_ADR(1),
	0x09,							/* Set Master Contrast MAX */
	U8G_ESC_ADR(0),					/* instruction mode */
	0xB9,							/* Set CMD Grayscale Linear */
#else
	U8G_ESC_ADR(0),					/* instruction mode */
	0x81,
	U8G_ESC_ADR(1),
	0xC8,							/* Set Contrast Color 1*/
	U8G_ESC_ADR(0),					/* instruction mode */
	0x82,
	U8G_ESC_ADR(1),
	0xA0,							/* Set Contrast Color 2*/
	U8G_ESC_ADR(0),					/* instruction mode */
	0x83,
	U8G_ESC_ADR(1),
	0xB0,							/* Set Contrast Color 3*/
	U8G_ESC_ADR(0),					/* instruction mode */
	0x87,
	U8G_ESC_ADR(1),
	0x0F,							/* Set Master Contrast MAX */

	U8G_ESC_ADR(0),					/* instruction mode */
	0xB8,							/* Set CMD Grayscale Lookup */
	U8G_ESC_ADR(1),
	0x05, 0x06, 0x07, 0x08,
	0x09, 0x0a, 0x0b, 0x0c,
	0x0D, 0x0E, 0x0F, 0x10, 
	0x11, 0x12, 0x13, 0x14, 
	0x15, 0x16, 0x18, 0x1a,
	0x1b, 0x1C, 0x1D, 0x1F, 
	0x21, 0x23, 0x25, 0x27, 
	0x2A, 0x2D, 0x30, 0x33, 
	0x36, 0x39, 0x3C, 0x3F, 
	0x42, 0x45, 0x48, 0x4C,
	0x50, 0x54, 0x58, 0x5C,
	0x60, 0x64, 0x68, 0x6C,
	0x70, 0x74, 0x78, 0x7D,
	0x82, 0x87, 0x8C, 0x91,
	0x96, 0x9B, 0xA0, 0xA5,
	0xAA, 0xAF, 0xB4, 
#endif
	U8G_ESC_ADR(0),
	0xAF,							/* Set Display On */
	U8G_ESC_CS(0),					/* disable chip */
	U8G_ESC_ADR(1),
	U8G_ESC_END						/* end of sequence */
};


static const uint8_t u8g_dev_ssd1353_160x128_column_seq[] PROGMEM = {
	U8G_ESC_CS(1),
	U8G_ESC_ADR(0), 0x15,
	U8G_ESC_ADR(1), 0x00, 0x9f,
	U8G_ESC_ADR(0), 0x75,
	U8G_ESC_ADR(1), 0x00, 0x7f,
	U8G_ESC_ADR(0), 0x5c,
	U8G_ESC_ADR(1),
	U8G_ESC_CS(0),
	U8G_ESC_END
};

static const uint8_t u8g_dev_ssd1353_160x128_sleep_on[] PROGMEM = {
	U8G_ESC_CS(1),
	U8G_ESC_ADR(0), 0xAE,
	U8G_ESC_CS(0),
	U8G_ESC_END
};

static const uint8_t u8g_dev_ssd1353_160x128_sleep_off[] PROGMEM = {
	U8G_ESC_CS(1),
	U8G_ESC_ADR(0), 0xAF,
	U8G_ESC_CS(0),
	U8G_ESC_END
};


#define RGB332_STREAM_BYTES 8
static uint8_t u8g_ssd1353_stream_bytes[RGB332_STREAM_BYTES*3];

uint8_t u8g_dev_ssd1353_160x128_332_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
	switch(msg)
	{
	case U8G_DEV_MSG_INIT:
		u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_50NS);
		u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1353_160x128_init_seq);
		break;

	case U8G_DEV_MSG_STOP:
		break;

	case U8G_DEV_MSG_PAGE_FIRST:
		u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1353_160x128_column_seq);
		break;

	case U8G_DEV_MSG_PAGE_NEXT:
		{
			u8g_uint_t x;
			uint8_t page_height;
			uint8_t i;
			uint8_t cnt;
			u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
			uint8_t *ptr = pb->buf;

			u8g_SetChipSelect(u8g, dev, 1);

			page_height = pb->p.page_y1;
			page_height -= pb->p.page_y0;
			page_height++;
			for( i = 0; i < page_height; i++ )
			{
			  
			  for (x = 0; x < pb->width; x+=RGB332_STREAM_BYTES)
			  {
				/* inline operation for better perf */
			    uint8_t *dest = u8g_ssd1353_stream_bytes;
			    for( cnt = 0; cnt < RGB332_STREAM_BYTES; cnt++ )
			    {
			      uint8_t val = *ptr++;
			      *dest++ = ((val & 0xe0) >> 2);
			      *dest++ = ((val & 0x1c) << 1);
			      *dest++ = ((val & 0x03) << 4);
			    }
			    u8g_WriteSequence(u8g, dev, RGB332_STREAM_BYTES*3, u8g_ssd1353_stream_bytes);
			  }
			}
			u8g_SetChipSelect(u8g, dev, 0);
		}

		break;

	case U8G_DEV_MSG_SLEEP_ON:
		u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1353_160x128_sleep_on);
		break;
	case U8G_DEV_MSG_SLEEP_OFF:
		u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1353_160x128_sleep_off);
		break;
	}

	return u8g_dev_pb8h8_base_fn(u8g, dev, msg, arg);
}

/*
 * not tested and not released, just taken from ssd1351
 */
static uint8_t u8g_dev_ssd1353_160x128_r[256];
static uint8_t u8g_dev_ssd1353_160x128_g[256];
static uint8_t u8g_dev_ssd1353_160x128_b[256];

uint8_t u8g_dev_ssd1353_160x128_idx_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
//	u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

	switch(msg)
	{
	case U8G_DEV_MSG_INIT:
		u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_50NS);
		u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1353_160x128_init_seq);
		break;

	case U8G_DEV_MSG_STOP:
		break;
	
	case U8G_DEV_MSG_SET_COLOR_ENTRY:
		  u8g_dev_ssd1353_160x128_r[ ((u8g_dev_arg_irgb_t *)arg)->idx ] = ((u8g_dev_arg_irgb_t *)arg)->r;
		  u8g_dev_ssd1353_160x128_g[ ((u8g_dev_arg_irgb_t *)arg)->idx ] = ((u8g_dev_arg_irgb_t *)arg)->g;
		  u8g_dev_ssd1353_160x128_b[ ((u8g_dev_arg_irgb_t *)arg)->idx ] = ((u8g_dev_arg_irgb_t *)arg)->b;
		  break;
	
	case U8G_DEV_MSG_PAGE_FIRST:
		u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1353_160x128_column_seq);
		break;

	case U8G_DEV_MSG_PAGE_NEXT:
		{
			int x;
			u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
			uint8_t *ptr = pb->buf;

			u8g_SetChipSelect(u8g, dev, 1);

			for (x = 0; x < pb->width; x++)
			{
				u8g_WriteByte(u8g, dev, u8g_dev_ssd1353_160x128_r[(*ptr)>>2]);
				u8g_WriteByte(u8g, dev, u8g_dev_ssd1353_160x128_g[(*ptr)>>2]);
				u8g_WriteByte(u8g, dev, u8g_dev_ssd1353_160x128_b[(*ptr)>>2]);

				ptr++;
			}

			u8g_SetChipSelect(u8g, dev, 0);
		}

		break;
	case U8G_DEV_MSG_GET_MODE:
	  return U8G_MODE_INDEX;
	}

	return u8g_dev_pb8h8_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_ssd1353_160x128_hicolor_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_50NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1353_160x128_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_FIRST:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1353_160x128_column_seq);
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        uint8_t i, j;
        uint8_t page_height;
	uint8_t *ptr = pb->buf;

	u8g_SetChipSelect(u8g, dev, 1);

        page_height = pb->p.page_y1;
        page_height -= pb->p.page_y0;
        page_height++;
        for( j = 0; j < page_height; j++ )
        {
            for (i = 0; i < pb->width; i+=RGB332_STREAM_BYTES)
            {
              register uint8_t cnt, low, high, r, g, b;
              uint8_t *dest = u8g_ssd1353_stream_bytes;
              for( cnt = 0; cnt < RGB332_STREAM_BYTES; cnt++ )
              {
                low = *ptr++;
                high = *ptr++;
            
                r = high & ~7;
                r >>= 2;
                b = low & 31;
                b <<= 1;
                g = high & 7;
                g <<= 3;
                g |= (low>>5)&7;
    
                *dest++ = r;
                *dest++ = g;
                *dest++ = b;
              } 
              u8g_WriteSequence(u8g, dev, RGB332_STREAM_BYTES*3, u8g_ssd1353_stream_bytes);
	    }
        }
	u8g_SetChipSelect(u8g, dev, 0);
      }
      break;    /* continue to base fn */
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1353_160x128_sleep_on);
      break;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1353_160x128_sleep_off);
      break;
  }
  return u8g_dev_pbxh16_base_fn(u8g, dev, msg, arg);
}


uint8_t u8g_dev_ssd1353_160x128_byte_buf[WIDTH*PAGE_HEIGHT] U8G_NOCOMMON ; 

u8g_pb_t u8g_dev_ssd1353_160x128_byte_pb = { {PAGE_HEIGHT, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_ssd1353_160x128_byte_buf};  
u8g_dev_t u8g_dev_ssd1353_160x128_332_sw_spi = { u8g_dev_ssd1353_160x128_332_fn, &u8g_dev_ssd1353_160x128_byte_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_ssd1353_160x128_332_hw_spi = { u8g_dev_ssd1353_160x128_332_fn, &u8g_dev_ssd1353_160x128_byte_pb, U8G_COM_HW_SPI };

//u8g_dev_t u8g_dev_ssd1353_160x128_idx_sw_spi = { u8g_dev_ssd1353_160x128_idx_fn, &u8g_dev_ssd1353_160x128_byte_pb, U8G_COM_SW_SPI };
//u8g_dev_t u8g_dev_ssd1353_160x128_idx_hw_spi = { u8g_dev_ssd1353_160x128_idx_fn, &u8g_dev_ssd1353_160x128_byte_pb, U8G_COM_HW_SPI };


/* only half of the height, because two bytes are needed for one pixel */
u8g_pb_t u8g_dev_ssd1353_160x128_hicolor_byte_pb = { {PAGE_HEIGHT/2, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_ssd1353_160x128_byte_buf}; 
u8g_dev_t u8g_dev_ssd1353_160x128_hicolor_sw_spi = { u8g_dev_ssd1353_160x128_hicolor_fn, &u8g_dev_ssd1353_160x128_hicolor_byte_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_ssd1353_160x128_hicolor_hw_spi = { u8g_dev_ssd1353_160x128_hicolor_fn, &u8g_dev_ssd1353_160x128_hicolor_byte_pb, U8G_COM_HW_SPI };


/* the 4x buffer is removed since it does not fit the RAM space of very small MCUs */
#if 0
uint8_t u8g_dev_ssd1353_160x128_4x_byte_buf[WIDTH*PAGE_HEIGHT*4] U8G_NOCOMMON ; 

u8g_pb_t u8g_dev_ssd1353_160x128_4x_332_byte_pb = { {PAGE_HEIGHT*4, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_ssd1353_160x128_4x_byte_buf};  
u8g_dev_t u8g_dev_ssd1353_160x128_4x_332_sw_spi = { u8g_dev_ssd1353_160x128_332_fn, &u8g_dev_ssd1353_160x128_4x_332_byte_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_ssd1353_160x128_4x_332_hw_spi = { u8g_dev_ssd1353_160x128_332_fn, &u8g_dev_ssd1353_160x128_4x_332_byte_pb, U8G_COM_HW_SPI };

u8g_pb_t u8g_dev_ssd1353_160x128_4x_hicolor_byte_pb = { {PAGE_HEIGHT/2*4, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_ssd1353_160x128_4x_byte_buf}; 
u8g_dev_t u8g_dev_ssd1353_160x128_4x_hicolor_sw_spi = { u8g_dev_ssd1353_160x128_hicolor_fn, &u8g_dev_ssd1353_160x128_4x_hicolor_byte_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_ssd1353_160x128_4x_hicolor_hw_spi = { u8g_dev_ssd1353_160x128_hicolor_fn, &u8g_dev_ssd1353_160x128_4x_hicolor_byte_pb, U8G_COM_HW_SPI };
#endif
