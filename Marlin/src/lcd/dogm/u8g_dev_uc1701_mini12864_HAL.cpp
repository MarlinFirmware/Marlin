/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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

/**
 * Based on u8g_dev_uc1701_mini12864.c (dealextreme)
 *
 * Universal 8bit Graphics Library
 *
 * Copyright (c) 2011, olikraus@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice, this list
 *    of conditions and the following disclaimer.
 *
 *  * Redistributions in binary form must reproduce the above copyright notice, this
 *    list of conditions and the following disclaimer in the documentation and/or other
 *    materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
 * CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "../../inc/MarlinConfigPre.h"

#if HAS_GRAPHICAL_LCD

#include "HAL_LCD_com_defines.h"

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

#define UC1701_ADC_REVERSE(N)    ((N) ? 0xA1 : 0xA0)
#define UC1701_BIAS_MODE(N)      ((N) ? 0xA3 : 0xA2)
#define UC1701_ALL_PIX(N)        ((N) ? 0xA5 : 0xA4)
#define UC1701_INVERTED(N)       ((N) ? 0xA7 : 0xA6)
#define UC1701_ON(N)             ((N) ? 0xAF : 0xAE)
#define UC1701_OUT_MODE(N)       ((N) ? 0xC8 : 0xC0)
#define UC1701_POWER_CONTROL(N)  (0x28 | (N))
#define UC1701_V5_RATIO(N)       (0x20 | ((N) & 0x7))
#define UC1701_CONTRAST(N)       (0x81), (N)

#define UC1701_COLUMN_HI(N)      (0x10 | (((N) >> 4) & 0xF))
#define UC1701_COLUMN_ADR(N)     UC1701_COLUMN_HI(N), ((N) & 0xF)
#define UC1701_PAGE_ADR(N)       (0xB0 | (N))
#define UC1701_START_LINE(N)     (0x40 | (N))
#define UC1701_INDICATOR(N)      (0xAC), (N)
#define UC1701_RESET()           (0xE2)
#define UC1701_NOOP()            (0xE3)
#define UC1701_BOOST_RATIO(N)    (0xF8), (N)

static const uint8_t u8g_dev_uc1701_mini12864_HAL_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),              // disable chip
  U8G_ESC_ADR(0),             // instruction mode
  U8G_ESC_RST(1),             // do reset low pulse with (1*16)+2 milliseconds
  U8G_ESC_CS(1),              // enable chip

  UC1701_RESET(),             // soft reset
  UC1701_START_LINE(0),       // set display start line to 0
  UC1701_ADC_REVERSE(0),      // ADC set to reverse
  UC1701_OUT_MODE(1),         // common output mode
  UC1701_INVERTED(0),         // display normal, bit val 0: LCD pixel off
  UC1701_BIAS_MODE(0),        // LCD bias 1/9
  UC1701_POWER_CONTROL(0x7),  // all power control circuits on
  UC1701_BOOST_RATIO(0x0),    // set booster ratio to 4x
  UC1701_V5_RATIO(3),         // set V0 voltage resistor ratio to large
  UC1701_CONTRAST(0x27),      // set contrast
  UC1701_INDICATOR(0),        // indicator disable
  UC1701_ON(1),               // display on

  U8G_ESC_CS(0),              // disable chip
  U8G_ESC_DLY(100),           // delay 100 ms
  U8G_ESC_CS(1),              // enable chip

  UC1701_ALL_PIX(1),          // display all points, ST7565
  U8G_ESC_CS(0),              // disable chip
  U8G_ESC_DLY(100),           // delay 100 ms
  U8G_ESC_DLY(100),           // delay 100 ms
  U8G_ESC_CS(1),              // enable chip
  UC1701_ALL_PIX(0),          // normal display
  U8G_ESC_CS(0),              // disable chip
  U8G_ESC_END                 // end of sequence
};

static const uint8_t u8g_dev_uc1701_mini12864_HAL_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),             // instruction mode
  U8G_ESC_CS(1),              // enable chip
  #if ENABLED(MKS_MINI_12864)
    UC1701_START_LINE(0),     // set display start line to 0
    UC1701_ADC_REVERSE(0),    // ADC set to reverse
    UC1701_OUT_MODE(1),       // common output mode
    UC1701_INVERTED(0),       // display normal, bit val 0: LCD pixel off
    UC1701_BIAS_MODE(0),      // LCD bias 1/9
    UC1701_POWER_CONTROL(0x7),// all power control circuits on
    UC1701_BOOST_RATIO(0x0),  // set booster ratio to 4x
    UC1701_V5_RATIO(3),       // set V0 voltage resistor ratio to large
    UC1701_INDICATOR(0),      // indicator disable
    UC1701_ON(1),             // display on
    UC1701_COLUMN_HI(0),      // set upper 4 bit of the col adr to 0
  #else
    UC1701_COLUMN_ADR(0),     // address 0
  #endif
  U8G_ESC_END                 // end of sequence
};

uint8_t u8g_dev_uc1701_mini12864_HAL_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  switch (msg) {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1701_mini12864_HAL_init_seq);
      break;

    case U8G_DEV_MSG_STOP: break;

    case U8G_DEV_MSG_PAGE_NEXT: {
      u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1701_mini12864_HAL_data_start);
      u8g_WriteByte(u8g, dev, 0x0B0 | pb->p.page); /* select current page */
      u8g_SetAddress(u8g, dev, 1);           /* data mode */
      if (!u8g_pb_WriteBuffer(pb, u8g, dev)) return 0;
      u8g_SetChipSelect(u8g, dev, 0);
    } break;

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

uint8_t u8g_dev_uc1701_mini12864_HAL_2x_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg) {
  switch (msg) {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_uc1701_mini12864_HAL_init_seq);
      break;

    case U8G_DEV_MSG_STOP: break;

    case U8G_DEV_MSG_PAGE_NEXT: {
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
    } break;

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

#endif // HAS_GRAPHICAL_LCD
