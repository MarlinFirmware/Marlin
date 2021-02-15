/*

  u8g_dev_ht1632.c

  1-Bit (BW) Driver for HT1632 controller

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



    U8G_PIN_NONE can be used as argument

    uint8_t u8g_InitSPI(u8g_t *u8g, u8g_dev_t *dev, uint8_t sck, uint8_t mosi, uint8_t cs, uint8_t a0, uint8_t reset)
    {
      ...
      u8g->pin_list[U8G_PI_SCK] = sck;
      u8g->pin_list[U8G_PI_MOSI] = mosi;
      u8g->pin_list[U8G_PI_CS] = cs;
      u8g->pin_list[U8G_PI_A0] = a0;
      u8g->pin_list[U8G_PI_RESET] = reset;

mapping

#define DATA_PIN --> U8G_PI_MOSI
#define WR_PIN    --> U8G_PI_SCK
#define CS_PIN      --> U8G_PI_CS
                                    U8G_PI_A0 --> not used
                                    U8G_PI_RESET --> not used

Usage:

    u8g_InitSPI(&u8g, &u8g_dev_ht1632_24x16, WR_PIN, DATA_IN, CS_PIN, U8G_PIN_NONE, U8G_PIN_NONE)

*/

#include "u8g.h"

#if defined(ARDUINO) && !defined(ARDUINO_ARCH_STM32)
  #if ARDUINO < 100
    #include <WProgram.h>
  #else
    #include <Arduino.h>
  #endif
#endif

#define WIDTH 24
#define HEIGHT 16
#define PAGE_HEIGHT 16

/* http://forum.arduino.cc/index.php?topic=168537.0 */

#define HT1632_CMD_SYSDIS       0x00    // CMD= 0000-0000-x Turn off oscil
#define HT1632_CMD_SYSON        0x01    // CMD= 0000-0001-x Enable system oscil
#define HT1632_CMD_LEDOFF       0x02    // CMD= 0000-0010-x LED duty cycle gen off
#define HT1632_CMD_LEDON        0x03    // CMD= 0000-0011-x LEDs ON
#define HT1632_CMD_BLOFF        0x08    // CMD= 0000-1000-x Blink OFF
#define HT1632_CMD_BLON         0x09    // CMD= 0000-1001-x Blink On
#define HT1632_CMD_SLVMD        0x10    // CMD= 0001-00xx-x Slave Mode
#define HT1632_CMD_MSTMD        0x14    // CMD= 0001-01xx-x Master Mode
#define HT1632_CMD_RCCLK        0x18    // CMD= 0001-10xx-x Use on-chip clock
#define HT1632_CMD_EXTCLK       0x1C    // CMD= 0001-11xx-x Use external clock
#define HT1632_CMD_COMS00       0x20    // CMD= 0010-ABxx-x commons options
#define HT1632_CMD_COMS01       0x24    // CMD= 0010-ABxx-x commons options
#define HT1632_CMD_COMS10       0x28    // CMD= 0010-ABxx-x commons options
#define HT1632_CMD_COMS11       0x2C    // P-MOS OUTPUT AND 16COMMON OPTION
#define HT1632_CMD_PWM          0xA0    // CMD= 101x-PPPP-x PWM duty cycle

#define HT1632_ID_CMD   4       /* ID = 100 - Commands */
#define HT1632_ID_RD    6       /* ID = 110 - Read RAM */
#define HT1632_ID_WR    5       /* ID = 101 - Write RAM */

#define HT1632_ID_LEN           3               // IDs are 3 bits
#define HT1632_CMD_LEN          8               // CMDs are 8 bits
#define HT1632_DATA_LEN         8               // Data are 4*2 bits
#define HT1632_ADDR_LEN         7               // Address are 7 bits

#if defined(ARDUINO) && !defined(ARDUINO_ARCH_STM32)

//#define WR_PIN 3
//#define DATA_PIN 2
//#define CS_PIN 4

void ht1632_write_data_MSB(u8g_t *u8g, uint8_t cnt, uint8_t data, uint8_t extra)
{
  int8_t i;
  uint8_t data_pin = u8g->pin_list[U8G_PI_MOSI];
  uint8_t wr_pin = u8g->pin_list[U8G_PI_SCK];

  for (i = cnt - 1; i >= 0; i--)
  {
    if ((data >> i) & 1)
    {
      digitalWrite(data_pin, HIGH);
    }
    else
    {
      digitalWrite(data_pin, LOW);
    }

    digitalWrite(wr_pin, LOW);
    u8g_MicroDelay();
    digitalWrite(wr_pin, HIGH);
    u8g_MicroDelay();
  }

  // Send an extra bit
  if (extra)
  {
    digitalWrite(data_pin, HIGH);
    digitalWrite(wr_pin, LOW);
    u8g_MicroDelay();
    digitalWrite(wr_pin, HIGH);
    u8g_MicroDelay();
  }
}

void ht1632_write_data(u8g_t *u8g, uint8_t cnt, uint8_t data)
{
  uint8_t i;
  uint8_t data_pin = u8g->pin_list[U8G_PI_MOSI];
  uint8_t wr_pin = u8g->pin_list[U8G_PI_SCK];
  for (i = 0; i < cnt; i++)
  {

    if ((data >> i) & 1) {
      digitalWrite(data_pin, HIGH);
    }
    else {
      digitalWrite(data_pin, LOW);
    }

    digitalWrite(wr_pin, LOW);
    u8g_MicroDelay();
    digitalWrite(wr_pin, HIGH);
    u8g_MicroDelay();
  }
}

void ht1632_init(u8g_t *u8g)
{
  //uint8_t i;
  uint8_t data_pin = u8g->pin_list[U8G_PI_MOSI];
  uint8_t wr_pin = u8g->pin_list[U8G_PI_SCK];
  uint8_t cs_pin = u8g->pin_list[U8G_PI_CS];
  pinMode(data_pin, OUTPUT);
  pinMode(wr_pin, OUTPUT);
  pinMode(cs_pin, OUTPUT);

  digitalWrite(data_pin, HIGH);
  digitalWrite(wr_pin, HIGH);
  digitalWrite(cs_pin, HIGH);

  digitalWrite(cs_pin, LOW);
  /* init display once after startup */
  ht1632_write_data_MSB(u8g, 3, HT1632_ID_CMD, false); // IDs are 3 bits
  ht1632_write_data_MSB(u8g, 8, HT1632_CMD_SYSDIS, true); // 8 bits
  ht1632_write_data_MSB(u8g, 8, HT1632_CMD_SYSON, true); // 8 bits
  ht1632_write_data_MSB(u8g, 8, HT1632_CMD_COMS11, true); // 8 bits
  ht1632_write_data_MSB(u8g, 8, HT1632_CMD_LEDON, true); // 8 bits
  ht1632_write_data_MSB(u8g, 8, HT1632_CMD_BLOFF, true); // 8 bits
  ht1632_write_data_MSB(u8g, 8, HT1632_CMD_PWM+15, true); // 8 bits
  digitalWrite(cs_pin, HIGH);

  /* removed following (debug) code */
  /*
  digitalWrite(cs_pin, LOW);
  ht1632_write_data_MSB(u8g, 3, HT1632_ID_WR, false); // Send "write to display" command
  ht1632_write_data_MSB(u8g, 7, 0, false);
  for(i = 0; i<48; ++i)
  {
    ht1632_write_data(u8g, 8, 0xFF);
  }
  digitalWrite(cs_pin, HIGH);
  */
}

/*
  page: 0=data contain lines 0..16, 1=data contain lines 16..32  (a 24x16 display will only have page 0)
  cnt: width of the display
  data: pointer to a buffer with 2*cnt bytes.
*/
void ht1632_transfer_data(u8g_t *u8g, uint8_t page, uint8_t cnt, uint8_t *data)
{
  uint8_t addr;
  uint8_t cs_pin = u8g->pin_list[U8G_PI_CS];
  /* send data to the ht1632 */
  digitalWrite(cs_pin, LOW);
  ht1632_write_data_MSB(u8g, 3, HT1632_ID_WR, false); // Send "write to display" command
  ht1632_write_data_MSB(u8g, 7, page*2*cnt, false);

  // Operating in progressive addressing mode
  for (addr = 0; addr < cnt; addr++)
  {
    ht1632_write_data(u8g, 8, data[addr]);
    ht1632_write_data(u8g, 8, data[addr+cnt]);
  }
  digitalWrite(cs_pin, HIGH);
}

/* value is between 0...15 */
void ht1632_set_contrast(u8g_t *u8g, uint8_t value)
{
  uint8_t cs_pin = u8g->pin_list[U8G_PI_CS];
  digitalWrite(cs_pin, LOW);
  ht1632_write_data_MSB(u8g, 3, HT1632_ID_CMD, false);
  ht1632_write_data_MSB(u8g, 8, HT1632_CMD_PWM + value, false);
  digitalWrite(cs_pin, HIGH);
}

#else

void ht1632_init(u8g_t *u8g)
{
}

void ht1632_transfer_data(u8g_t *u8g, uint8_t page, uint8_t cnt, uint8_t *data)
{
}

void ht1632_set_contrast(u8g_t *u8g, uint8_t value)
{
}

#endif /* ARDUINO && !(STM32F1 || STM32F1xx || STM32F4 || STM32F4xx) */


uint8_t u8g_dev_ht1632_24x16_fn(u8g_t *u8g, u8g_dev_t *dev, uint8_t msg, void *arg)
{
  switch (msg)
  {
    case U8G_DEV_MSG_INIT:
      ht1632_init(u8g);
      break;
    case U8G_DEV_MSG_STOP:
      break;
    case U8G_DEV_MSG_PAGE_NEXT:
      {
        u8g_pb_t *pb = (u8g_pb_t *)(dev->dev_mem);

        /* current page: pb->p.page */
        /* ptr to the buffer: pb->buf */
        ht1632_transfer_data(u8g, pb->p.page, WIDTH, pb->buf);
      }
      break;
    case U8G_DEV_MSG_CONTRAST:
      /* values passed to SetContrast() are between 0 and 255, scale down to 0...15 */
      ht1632_set_contrast(u8g, (*(uint8_t *)arg) >> 4);
    return 1;
  }
  return u8g_dev_pb16v1_base_fn(u8g, dev, msg, arg);
}

uint8_t u8g_dev_ht1632_24x16_buf[WIDTH*2] U8G_NOCOMMON ;
u8g_pb_t u8g_dev_ht1632_24x16_pb = { {16, HEIGHT, 0, 0, 0},  WIDTH, u8g_dev_ht1632_24x16_buf};
u8g_dev_t u8g_dev_ht1632_24x16 = { u8g_dev_ht1632_24x16_fn, &u8g_dev_ht1632_24x16_pb, u8g_com_null_fn };
