/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*

  u8g_dev_st7920_128x64_HAL.c

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

#include <U8glib.h>

#include "HAL_LCD_com_defines.h"

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8


/* init sequence from https://github.com/adafruit/ST7565-LCD/blob/master/ST7565/ST7565.cpp */
static const uint8_t u8g_dev_st7920_128x64_HAL_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_RST(15),           /* do reset low pulse with (15*16)+2 milliseconds (=maximum delay)*/
  U8G_ESC_DLY(100),         /* 8 Dez 2012: additional delay 100 ms because of reset*/
  U8G_ESC_CS(1),             /* enable chip */
  U8G_ESC_DLY(50),         /* delay 50 ms */

  0x038,                                /* 8 Bit interface (DL=1), basic instruction set (RE=0) */
  0x00c,                                /* display on, cursor & blink off; 0x08: all off */
  0x006,                                /* Entry mode: Cursor move to right ,DDRAM address counter (AC) plus 1, no shift */
  0x002,                                /* disable scroll, enable CGRAM adress */
  0x001,                                /* clear RAM, needs 1.6 ms */
  U8G_ESC_DLY(100),               /* delay 100 ms */

  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_st7920_128x64_HAL_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_400NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_st7920_128x64_HAL_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        uint8_t y, i;
        uint8_t *ptr;
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

        u8g_SetAddress(u8g, dev, 0);           /* cmd mode */
        u8g_SetChipSelect(u8g, dev, 1);
        y = pb->p.page_y0;
        ptr = (uint8_t *)pb->buf;
        for( i = 0; i < 8; i ++ )
        {
          u8g_SetAddress(u8g, dev, 0);           /* cmd mode */
          u8g_WriteByte(u8g, dev, 0x03e );      /* enable extended mode */

          if ( y < 32 )
          {
                  u8g_WriteByte(u8g, dev, 0x080 | y );      /* y pos  */
                  u8g_WriteByte(u8g, dev, 0x080  );      /* set x pos to 0*/
          }
          else
          {
                  u8g_WriteByte(u8g, dev, 0x080 | (y-32) );      /* y pos  */
                  u8g_WriteByte(u8g, dev, 0x080 | 8);      /* set x pos to 64*/
          }

          u8g_SetAddress(u8g, dev, 1);                  /* data mode */
          u8g_WriteSequence(u8g, dev, WIDTH/8, ptr);
          ptr += WIDTH/8;
          y++;
        }
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
  }
  return u8g_dev_pb8h1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_st7920_128x64_HAL_4x_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_400NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_st7920_128x64_HAL_init_seq);
      break;

    case U8G_DEV_MSG_STOP:
      break;

    case U8G_DEV_MSG_PAGE_NEXT:
      {
        uint8_t y, i;
        uint8_t *ptr;
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

        u8g_SetAddress(u8g, dev, 0);           /* cmd mode */
        u8g_SetChipSelect(u8g, dev, 1);
        y = pb->p.page_y0;
        ptr = (uint8_t *)pb->buf;
        for( i = 0; i < 32; i ++ )
        {
          u8g_SetAddress(u8g, dev, 0);           /* cmd mode */
          u8g_WriteByte(u8g, dev, 0x03e );      /* enable extended mode */

          if ( y < 32 )
          {
                  u8g_WriteByte(u8g, dev, 0x080 | y );      /* y pos  */
                  u8g_WriteByte(u8g, dev, 0x080  );      /* set x pos to 0*/
          }
          else
          {
                  u8g_WriteByte(u8g, dev, 0x080 | (y-32) );      /* y pos  */
                  u8g_WriteByte(u8g, dev, 0x080 | 8);      /* set x pos to 64*/
          }

          u8g_SetAddress(u8g, dev, 1);                  /* data mode */
          u8g_WriteSequence(u8g, dev, WIDTH/8, ptr);
          ptr += WIDTH/8;
          y++;
        }
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
  }
  return u8g_dev_pb32h1_base_fn(u8g, dev, msg, arg);
}

U8G_PB_DEV(u8g_dev_st7920_128x64_HAL_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_st7920_128x64_HAL_fn, U8G_COM_ST7920_HAL_SW_SPI);

#define QWIDTH (WIDTH*4)
uint8_t u8g_dev_st7920_128x64_HAL_4x_buf[QWIDTH] U8G_NOCOMMON ;
u8g_pb_t u8g_dev_st7920_128x64_HAL_4x_pb = { {32, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_st7920_128x64_HAL_4x_buf};
u8g_dev_t u8g_dev_st7920_128x64_HAL_4x_sw_spi = { u8g_dev_st7920_128x64_HAL_4x_fn, &u8g_dev_st7920_128x64_HAL_4x_pb, U8G_COM_ST7920_HAL_SW_SPI };


U8G_PB_DEV(u8g_dev_st7920_128x64_HAL_hw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_st7920_128x64_HAL_fn, U8G_COM_ST7920_HAL_HW_SPI);
u8g_dev_t u8g_dev_st7920_128x64_HAL_4x_hw_spi = { u8g_dev_st7920_128x64_HAL_4x_fn, &u8g_dev_st7920_128x64_HAL_4x_pb, U8G_COM_ST7920_HAL_HW_SPI };


#ifdef U8G_HAL_LINKS
  // Also use this device for HAL version of rrd class. This results in the same device being used
  // for the ST7920 for HAL systems no matter what is selected in ultralcd_impl_DOGM.h.
  u8g_dev_t u8g_dev_st7920_128x64_rrd_sw_spi = { u8g_dev_st7920_128x64_HAL_4x_fn, &u8g_dev_st7920_128x64_HAL_4x_pb, U8G_COM_ST7920_HAL_SW_SPI };
#endif
