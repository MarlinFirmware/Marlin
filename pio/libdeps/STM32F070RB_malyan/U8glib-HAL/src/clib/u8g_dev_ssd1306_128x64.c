/*

  u8g_dev_ssd1306_128x64.c

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

#define WIDTH 128
#define HEIGHT 64
#define PAGE_HEIGHT 8

/* select one init sequence here
  1 - adafruit 128x64 OLED (NOT TESTED)
  2 - adafruit 128x64 OLED (NOT TESTED)
  3 - adafruit 128x64 OLED (NOT TESTED), like adafruit3, but with page addressing mode
  4 - Univision datasheet (NOT TESTED)
*/

#ifndef U8G_INIT_SEQUENCE
  #define U8G_INIT_SEQUENCE 3
#endif

#if U8G_INIT_SEQUENCE == 1

  /* init sequence adafruit 128x64 OLED (NOT TESTED) */
  static const uint8_t u8g_dev_ssd1306_128x64_adafruit1_init_seq[] PROGMEM = {
    U8G_ESC_CS(0),             /* disable chip */
    U8G_ESC_ADR(0),           /* instruction mode */
    U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
    U8G_ESC_CS(1),             /* enable chip */

    0x0ae,        /* display off, sleep mode */
    0x0d5, 0x080,   /* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
    0x0a8, 0x03f,   /* */

    0x0d3, 0x000,   /*  */

    0x040,        /* start line */

    0x08d, 0x010,   /* [1] charge pump setting (p62): 0x014 enable, 0x010 disable */

    0x020, 0x000,   /* */
    0x0a1,        /* segment remap a0/a1*/
    0x0c8,        /* c0: scan dir normal, c8: reverse */
    0x0da, 0x012,   /* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
    0x081, 0x09f,   /* [1] set contrast control */
    0x0d9, 0x022,   /* [1] pre-charge period 0x022/f1*/
    0x0db, 0x040,   /* vcomh deselect level */

    0x02e,        /* 2012-05-27: Deactivate scroll */
    0x0a4,        /* output ram to display */
    0x0a6,        /* none inverted normal display mode */
    0x0af,        /* display on */

    U8G_ESC_CS(0),             /* disable chip */
    U8G_ESC_END                /* end of sequence */
  };
  #define u8g_dev_ssd1306_128x64_init_seq u8g_dev_ssd1306_128x64_adafruit1_init_seq

#elif U8G_INIT_SEQUENCE == 2

  /* init sequence adafruit 128x64 OLED (NOT TESTED) */
  #define u8g_dev_ssd1306_128x64_init_seq u8g_dev_ssd1306_128x64_adafruit2_init_seq

#elif U8G_INIT_SEQUENCE == 3

  /* init sequence adafruit 128x64 OLED (NOT TESTED), like adafruit3, but with page addressing mode */
  static const uint8_t u8g_dev_ssd1306_128x64_adafruit3_init_seq[] PROGMEM = {
    U8G_ESC_CS(0),             /* disable chip */
    U8G_ESC_ADR(0),           /* instruction mode */
    U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
    U8G_ESC_CS(1),             /* enable chip */

    0x0ae,        /* display off, sleep mode */
    0x0d5, 0x080,   /* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
    0x0a8, 0x03f,   /* */

    0x0d3, 0x000,   /*  */

    0x040,        /* start line */

    0x08d, 0x014,   /* [2] charge pump setting (p62): 0x014 enable, 0x010 disable */

    0x020, 0x002,   /* 2012-05-27: page addressing mode */
    0x0a1,        /* segment remap a0/a1*/
    0x0c8,        /* c0: scan dir normal, c8: reverse */
    0x0da, 0x012,   /* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
    0x081, 0x0cf,   /* [2] set contrast control */
    0x0d9, 0x0f1,   /* [2] pre-charge period 0x022/f1*/
    0x0db, 0x040,   /* vcomh deselect level */

    0x02e,        /* 2012-05-27: Deactivate scroll */
    0x0a4,        /* output ram to display */
    0x0a6,        /* none inverted normal display mode */
    0x0af,        /* display on */

    U8G_ESC_CS(0),             /* disable chip */
    U8G_ESC_END                /* end of sequence */
  };
  #define u8g_dev_ssd1306_128x64_init_seq u8g_dev_ssd1306_128x64_adafruit3_init_seq

#elif U8G_INIT_SEQUENCE == 4

  /* init sequence Univision datasheet (NOT TESTED) */
  static const uint8_t u8g_dev_ssd1306_128x64_univision_init_seq[] PROGMEM = {
    U8G_ESC_CS(0),             /* disable chip */
    U8G_ESC_ADR(0),           /* instruction mode */
    U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
    U8G_ESC_CS(1),             /* enable chip */

    0x0ae,        /* display off, sleep mode */
    0x0d5, 0x080,   /* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
    0x0a8, 0x03f,   /* multiplex ratio */
    0x0d3, 0x000,   /* display offset */
    0x040,        /* start line */
    0x08d, 0x010,   /* charge pump setting (p62): 0x014 enable, 0x010 disable */
    0x0a1,        /* segment remap a0/a1*/
    0x0c8,        /* c0: scan dir normal, c8: reverse */
    0x0da, 0x012,   /* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
    0x081, 0x09f,   /* set contrast control */
    0x0d9, 0x022,   /* pre-charge period */
    0x0db, 0x040,   /* vcomh deselect level */
    0x022, 0x000,   /* page addressing mode WRONG: 3 byte cmd! */
    0x0a4,        /* output ram to display */
    0x0a6,        /* none inverted normal display mode */
    0x0af,        /* display on */
    U8G_ESC_CS(0),             /* disable chip */
    U8G_ESC_END                /* end of sequence */
  };
  #define u8g_dev_ssd1306_128x64_init_seq u8g_dev_ssd1306_128x64_univision_init_seq

#endif

// 26 Apr 2014: In the thread http://forum.arduino.cc/index.php?topic=234930.msg1696754;topicseen#msg1696754
//              it is mentioned that adafruit2_init_seq works better.
//              This will be used by the ssd1306_adafruit device.
static const uint8_t u8g_dev_ssd1306_128x64_adafruit2_init_seq[] PROGMEM = {
  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_RST(1),           /* do reset low pulse with (1*16)+2 milliseconds */
  U8G_ESC_CS(1),             /* enable chip */

  0x0ae,        /* display off, sleep mode */
  0x0d5, 0x080,   /* clock divide ratio (0x00=1) and oscillator frequency (0x8) */
  0x0a8, 0x03f,   /* */

  0x0d3, 0x000,   /*  */

  0x040,        /* start line */

  0x08d, 0x014,   /* [2] charge pump setting (p62): 0x014 enable, 0x010 disable */

  0x020, 0x000,   /* */
  0x0a1,        /* segment remap a0/a1*/
  0x0c8,        /* c0: scan dir normal, c8: reverse */
  0x0da, 0x012,   /* com pin HW config, sequential com pin config (bit 4), disable left/right remap (bit 5) */
  0x081, 0x0cf,   /* [2] set contrast control */
  0x0d9, 0x0f1,   /* [2] pre-charge period 0x022/f1*/
  0x0db, 0x040,   /* vcomh deselect level */

  0x02e,        /* 2012-05-27: Deactivate scroll */
  0x0a4,        /* output ram to display */
  0x0a6,        /* none inverted normal display mode */
  0x0af,        /* display on */

  U8G_ESC_CS(0),             /* disable chip */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_ssd1306_128x64_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x010,    /* set upper 4 bit of the col adr to 0 */
  0x000,    /* set lower 4 bit of the col adr to 0  */
  U8G_ESC_END                /* end of sequence */
};

/* the sh1106 is compatible to the ssd1306, but is 132x64. display seems to be centered */
static const uint8_t u8g_dev_sh1106_128x64_data_start[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x010,    /* set upper 4 bit of the col adr to 0 */
  0x002,    /* set lower 4 bit of the col adr to 2 (centered display with sh1106)  */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_ssd13xx_sleep_on[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x0ae,    /* display off */
  U8G_ESC_CS(0),             /* disable chip, bugfix 12 nov 2014 */
  U8G_ESC_END                /* end of sequence */
};

static const uint8_t u8g_dev_ssd13xx_sleep_off[] PROGMEM = {
  U8G_ESC_ADR(0),           /* instruction mode */
  U8G_ESC_CS(1),             /* enable chip */
  0x0af,    /* display on */
  U8G_ESC_DLY(50),       /* delay 50 ms */
  U8G_ESC_CS(0),             /* disable chip, bugfix 12 nov 2014 */
  U8G_ESC_END                /* end of sequence */
};

uint8_t u8g_dev_ssd1306_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_adafruit2_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_data_start);
        u8g_WriteByte(u8g, dev, 0x0b0 | pb->p.page); /* select current page (SSD1306) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
          return 0;
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_off);
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_ssd1306_adafruit_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_data_start);
        u8g_WriteByte(u8g, dev, 0x0b0 | pb->p.page); /* select current page (SSD1306) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
          return 0;
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_off);
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_sh1106_128x64_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);
        u8g_WriteEscSeqP(u8g, dev, u8g_dev_sh1106_128x64_data_start);
        u8g_WriteByte(u8g, dev, 0x0b0 | pb->p.page); /* select current page (SSD1306) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
        if ( u8g_pb_WriteBuffer(pb, u8g, dev) == 0 )
          return 0;
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_off);
      return 1;
  }
  return u8g_dev_pb8v1_base_fn(u8g, dev, msg, arg);
}


uint8_t u8g_dev_ssd1306_128x64_2x_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_data_start);
        u8g_WriteByte(u8g, dev, 0x0b0 | (pb->p.page*2)); /* select current page (SSD1306) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
  u8g_WriteSequence(u8g, dev, pb->width, pb->buf);
        u8g_SetChipSelect(u8g, dev, 0);

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_data_start);
        u8g_WriteByte(u8g, dev, 0x0b0 | (pb->p.page*2+1)); /* select current page (SSD1306) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
  u8g_WriteSequence(u8g, dev, pb->width, (uint8_t *)(pb->buf)+pb->width);
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_off);
      return 1;
  }
  return u8g_dev_pb16v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_sh1106_128x64_2x_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch(msg)
  {
    case U8G_DEV_MSG_INIT:
      u8g_InitCom(u8g, dev, U8G_SPI_CLK_CYCLE_300NS);
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd1306_128x64_init_seq);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_sh1106_128x64_data_start);
        u8g_WriteByte(u8g, dev, 0x0b0 | (pb->p.page*2)); /* select current page (SSD1306) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
  u8g_WriteSequence(u8g, dev, pb->width, pb->buf);
        u8g_SetChipSelect(u8g, dev, 0);

        u8g_WriteEscSeqP(u8g, dev, u8g_dev_sh1106_128x64_data_start);
        u8g_WriteByte(u8g, dev, 0x0b0 | (pb->p.page*2+1)); /* select current page (SSD1306) */
        u8g_SetAddress(u8g, dev, 1);           /* data mode */
  u8g_WriteSequence(u8g, dev, pb->width, (uint8_t *)(pb->buf)+pb->width);
        u8g_SetChipSelect(u8g, dev, 0);
      }
      break;
    case U8G_DEV_MSG_SLEEP_ON:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_on);
      return 1;
    case U8G_DEV_MSG_SLEEP_OFF:
      u8g_WriteEscSeqP(u8g, dev, u8g_dev_ssd13xx_sleep_off);
      return 1;
  }
  return u8g_dev_pb16v1_base_fn(u8g, dev, msg, arg);
}


U8G_PB_DEV(u8g_dev_ssd1306_128x64_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ssd1306_128x64_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_ssd1306_128x64_hw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ssd1306_128x64_fn, U8G_COM_HW_SPI);
U8G_PB_DEV(u8g_dev_ssd1306_128x64_i2c, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ssd1306_128x64_fn, U8G_COM_SSD_I2C);

U8G_PB_DEV(u8g_dev_ssd1306_adafruit_128x64_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ssd1306_adafruit_128x64_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_ssd1306_adafruit_128x64_hw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ssd1306_adafruit_128x64_fn, U8G_COM_HW_SPI);
U8G_PB_DEV(u8g_dev_ssd1306_adafruit_128x64_i2c, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_ssd1306_adafruit_128x64_fn, U8G_COM_SSD_I2C);


uint8_t u8g_dev_ssd1306_128x64_2x_buf[WIDTH*2] U8G_NOCOMMON ;
u8g_pb_t u8g_dev_ssd1306_128x64_2x_pb = { {16, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_ssd1306_128x64_2x_buf};
u8g_dev_t u8g_dev_ssd1306_128x64_2x_sw_spi = { u8g_dev_ssd1306_128x64_2x_fn, &u8g_dev_ssd1306_128x64_2x_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_ssd1306_128x64_2x_hw_spi = { u8g_dev_ssd1306_128x64_2x_fn, &u8g_dev_ssd1306_128x64_2x_pb, U8G_COM_HW_SPI };
u8g_dev_t u8g_dev_ssd1306_128x64_2x_i2c = { u8g_dev_ssd1306_128x64_2x_fn, &u8g_dev_ssd1306_128x64_2x_pb, U8G_COM_SSD_I2C };


U8G_PB_DEV(u8g_dev_sh1106_128x64_sw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_sh1106_128x64_fn, U8G_COM_SW_SPI);
U8G_PB_DEV(u8g_dev_sh1106_128x64_hw_spi, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_sh1106_128x64_fn, U8G_COM_HW_SPI);
U8G_PB_DEV(u8g_dev_sh1106_128x64_i2c, WIDTH, HEIGHT, PAGE_HEIGHT, u8g_dev_sh1106_128x64_fn, U8G_COM_SSD_I2C);

uint8_t u8g_dev_sh1106_128x64_2x_buf[WIDTH*2] U8G_NOCOMMON ;
u8g_pb_t u8g_dev_sh1106_128x64_2x_pb = { {16, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_sh1106_128x64_2x_buf};
u8g_dev_t u8g_dev_sh1106_128x64_2x_sw_spi = { u8g_dev_sh1106_128x64_2x_fn, &u8g_dev_sh1106_128x64_2x_pb, U8G_COM_SW_SPI };
u8g_dev_t u8g_dev_sh1106_128x64_2x_hw_spi = { u8g_dev_sh1106_128x64_2x_fn, &u8g_dev_sh1106_128x64_2x_pb, U8G_COM_HW_SPI };
u8g_dev_t u8g_dev_sh1106_128x64_2x_i2c = { u8g_dev_sh1106_128x64_2x_fn, &u8g_dev_sh1106_128x64_2x_pb, U8G_COM_SSD_I2C };

