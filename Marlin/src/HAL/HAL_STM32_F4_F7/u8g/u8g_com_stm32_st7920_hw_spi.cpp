/*

  u8g_com_stm32_st7920_hw_spi.c

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

  A special HW SPI interface for ST7920 controller

*/

#include "../HAL.h"
#if HAL_IS_STM32F4

#include <U8glib.h>
#include <Arduino.h>
#include "../../../inc/MarlinConfig.h"
#include <SPI.h>

#define SPI_TRANSFER_TIMEOUT    1000
#define SPI_MODE0 0x00
#define SPI_MODE1 0x01
#define SPI_MODE2 0x02
#define SPI_MODE3 0x03

static uint8_t _buf[256*2 + 1];

static void u8g_com_arduino_st7920_write_byte_hw_spi_seq(u8g_t *u8g, uint8_t rs, uint8_t *ptr, uint8_t len)
{
  uint8_t idx;

  idx = 0;
  if (rs == 0) {
    _buf[0] = 0xf8;
    idx = 1;
  } else if (rs == 1) {
    _buf[0] = 0xfa;
    idx = 1;
  }
  while (len > 0) {
      _buf[idx++] = *ptr & 0x0f0;
      _buf[idx++] = *ptr << 4;
      ptr++;
      len--;
  }
  SPI.transfer(_buf, idx);
}

static void u8g_com_arduino_st7920_write_byte_hw_spi(u8g_t *u8g, uint8_t rs, uint8_t val)
{
  uint8_t idx;

  idx = 0;
  if (rs == 0) {
    _buf[0] = 0xf8;
    idx = 1;
  } else if (rs == 1) {
    _buf[0] = 0xfa;
    idx = 1;
  }
  _buf[idx++] = val & 0x0f0;
  _buf[idx++] = val << 4;
  SPI.transfer(_buf, idx);
}

uint8_t u8g_com_HAL_stm32f4_ST7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      pinMode(u8g->pin_list[U8G_PI_CS], OUTPUT);
      digitalWrite(u8g->pin_list[U8G_PI_CS], HIGH);
      if (u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE) {
        pinMode(u8g->pin_list[U8G_PI_RESET], OUTPUT);
        digitalWrite(u8g->pin_list[U8G_PI_RESET], HIGH);
      }
      u8g->pin_list[U8G_PI_A0_STATE] = 0;       /* inital RS state: command mode */
      break;

    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_RESET:
      if ( u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE ) {
	      u8g_com_arduino_digital_write(u8g, U8G_PI_RESET, arg_val);
      }
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      if ( arg_val == 0 )
      {
        /* disable, note: the st7920 has an active high chip select */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
        SPI.endTransaction();
      }
      else
      {
        /* enable */
        SPI.beginTransaction(SPISettings(400000, MSBFIRST, SPI_MODE0));
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);
      }
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_com_arduino_st7920_write_byte_hw_spi(u8g,  u8g->pin_list[U8G_PI_A0_STATE], arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:
      u8g_com_arduino_st7920_write_byte_hw_spi_seq(u8g, u8g->pin_list[U8G_PI_A0_STATE], (uint8_t *)arg_ptr, arg_val);
      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g->pin_list[U8G_PI_A0_STATE] = arg_val;
      break;
  }
  return 1;
}

#endif // HAL_IS_STM32F4
