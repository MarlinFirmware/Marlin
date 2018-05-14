/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016, 2017, 2018 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

  based on u8g_dev_uc1701_mini12864_HAL.c (dealextreme)

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

#include "../../inc/MarlinConfig.h"

#if ENABLED(DOGLCD)

#include <U8glib.h>

#include "HAL_LCD_com_defines.h"

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

static const uint8_t u8g_dev_uc1701_mini12864_HAL_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
  U8G_ESC_CS(1),             /* enable chip */

  0x0E2,            /* soft reset */
  0x040,    /* set display start line to 0 */
  0x0A0,    /* ADC set to reverse */
  0x0C8,    /* common output mode */
  0x0A6,    /* display normal, bit val 0: LCD pixel off. */
  0x0A2,    /* LCD bias 1/9 */
  0x02F,    /* all power  control circuits on */
  0x0F8,    /* set booster ratio to */
  0x000,    /* 4x */
  0x023,    /* set V0 voltage resistor ratio to large */
  0x081,    /* set contrast */
  0x027,    /* contrast value */
  0x0AC,    /* indicator */
  0x000,    /* disable */
  0x0AF,    /* display on */

  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x0A5,                    /* display all points, ST7565 */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  U8G_ESC_DLY(100),       /* delay 100 ms */
  0x0A4,                    /* normal display */
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_uc1701_mini12864_HAL_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x010,    /* set upper 4 bit of the col adr to 0 */
  0x000,    /* set lower 4 bit of the col adr to 4  */
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_uc1701_mini12864_HAL_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1701_mini12864_HAL_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1701_mini12864_HAL_data_start);
        u8g_WriteByte(u8g, dev, 0x0B0 | pb->p.page); /* select current page */
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
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_uc1701_mini12864_HAL_2x_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1701_mini12864_HAL_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1701_mini12864_HAL_data_start);
        u8g_WriteByte(u8g, dev, 0x0B0 | (2*pb->p.page)); /* select current page */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
  u8g_WriteSequence(u8g, dev, pb->width, (uint8_t *)pb->buf);
        u8g_SetChipSelect(u8g, dev, 0);

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1701_mini12864_HAL_data_start);
        u8g_WriteByte(u8g, dev, 0x0B0 | (2*pb->p.page+1)); /* select current page */
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

U8G_PB_DEV(u8g_dev_uc1701_mini12864_HAL_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_uc1701_mini12864_HAL_fn, U8G_COM_HAL_SW_SPI_FN);
U8G_PB_DEV(u8g_dev_uc1701_mini12864_HAL_hw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_uc1701_mini12864_HAL_fn, U8G_COM_HAL_HW_SPI_FN);

uint8_t u8g_dev_uc1701_mini12864_HAL_2x_buf[WIDTH*2] U8G_NOCOMMON ;
u8g_pb_t u8g_dev_uc1701_mini12864_HAL_2x_pb = { {16, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_uc1701_mini12864_HAL_2x_buf};
u8g_dev_t u8g_dev_uc1701_mini12864_HAL_2x_sw_spi = { u8g_dev_uc1701_mini12864_HAL_2x_fn, &u8g_dev_uc1701_mini12864_HAL_2x_pb, U8G_COM_HAL_SW_SPI_FN };
u8g_dev_t u8g_dev_uc1701_mini12864_HAL_2x_hw_spi = { u8g_dev_uc1701_mini12864_HAL_2x_fn, &u8g_dev_uc1701_mini12864_HAL_2x_pb, U8G_COM_HAL_HW_SPI_FN };

#endif // DOGLCD