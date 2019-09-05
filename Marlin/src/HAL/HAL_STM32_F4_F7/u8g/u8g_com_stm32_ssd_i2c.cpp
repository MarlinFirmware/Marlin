/*

  u8g_com_stm32_ssd_i2c.c

  com interface for stm32 and the SSDxxxx chip (SOLOMON) variant
  I2C protocol

  Universal 8bit Graphics Library

  Copyright (c) 2018, ghent360@iqury.us
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

  Special pin usage:
    U8G_PI_I2C_OPTION	additional options
    U8G_PI_A0_STATE	    used to store the last value of the command/data register selection
    U8G_PI_SET_A0		1: Signal request to update I2C device with new A0_STATE,
                        0: Do nothing, A0_STATE matches I2C device
    U8G_PI_SCL		    clock line (NOT USED)
    U8G_PI_SDA		    data line (NOT USED)

    U8G_PI_RESET		reset line (currently disabled, see below)

  Protocol:
    SLA, Cmd/Data Selection, Arguments
    The command/data register is selected by a special instruction byte, which is sent after SLA

    The continue bit is always 0 so that a (re)start is equired for the change from cmd to/data mode
*/

#include "../HAL.h"
#if HAL_IS_STM32F4

#include <U8glib.h>
#include <Arduino.h>

#if !defined(U8G_WITH_PINLIST)
#error U8G_WITH_PINLIST should be defined for this driver to work.
#endif

#define I2C_SLA         (0x3c*2)
#define I2C_CMD_MODE    0x000
#define I2C_DATA_MODE   0x040

static i2c_t i2c_;

static void init_i2c_hw()
{
  i2c_.sda = digitalPinToPinName(PIN_WIRE_SDA);
  i2c_.scl = digitalPinToPinName(PIN_WIRE_SCL);
  i2c_custom_init(&i2c_, I2C_400KHz, I2C_ADDRESSINGMODE_7BIT, 0);
}

static void wait_i2c_ready()
{
  while(HAL_I2C_GetState(&i2c_.handle) & 3);
}

static uint8_t buffer[256];

uint8_t u8g_com_HAL_stm32f4_ssd_hw_i2c_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      init_i2c_hw();
      break;

    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_RESET:
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      u8g->pin_list[U8G_PI_A0_STATE] = 0;
      u8g->pin_list[U8G_PI_SET_A0] = 1;		/* force a0 to set again, also forces start condition */
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      wait_i2c_ready();
      buffer[0] = arg_val;
      HAL_I2C_Mem_Write_IT(
        &i2c_.handle,
        I2C_SLA,
        u8g->pin_list[U8G_PI_A0_STATE] == 0 ? I2C_CMD_MODE :I2C_DATA_MODE,
        1,
        buffer,
        1);
      break;

    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:
      wait_i2c_ready();
      memcpy(buffer, arg_ptr, arg_val);
      HAL_I2C_Mem_Write_IT(
        &i2c_.handle,
        I2C_SLA,
        u8g->pin_list[U8G_PI_A0_STATE] == 0 ? I2C_CMD_MODE :I2C_DATA_MODE,
        1,
        buffer,
        arg_val);
      break;

    case U8G_COM_MSG_ADDRESS:             /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g->pin_list[U8G_PI_A0_STATE] = arg_val;
      u8g->pin_list[U8G_PI_SET_A0] = 1;		/* force a0 to set again */
      break;
  }
  return 1;
}

#endif
