/*

  u8g_com_psoc5_ssd_hw_parallel.c

  com interface for Cypress PSoC5 and the SSDxxxx chip variant
  I2C protocol

  Universal 8bit Graphics Library

  Copyright (c) 2015, olikraus@gmail.com, schmidt.ronny@gmail.com
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

#ifdef U8G_CYPRESS_PSOC5

#include <project.h>

static uint8 dc = 0; // need to store whether next write is data or command

uint8_t u8g_com_psoc5_ssd_hw_parallel_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      // stop the device
      GraphicLCDIntf_Stop();
      break;

    case U8G_COM_MSG_INIT:
  		// init hardware interfaces, timers, gpios, ...
      GraphicLCDIntf_Init();
      break;

    case U8G_COM_MSG_ADDRESS:
      // switch from cmd (arg_val = 0) to data mode (arg_val = 1) or vice versa
      dc = arg_val;
      break;

    case U8G_COM_MSG_CHIP_SELECT:
		  /* done by the hardware */
      break;

    case U8G_COM_MSG_RESET:
      // toggle the reset pin of the display by value in arg_val
      nRES_Write(0);
      u8g_10MicroDelay();
      nRES_Write(1);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      // write byte to the device
		  GraphicLCDIntf_Write8(dc, arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ:
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        // write a sequence of bytes to the device
        register uint8_t *ptr = arg_ptr;
        while (arg_val-- > 0)
        {
          GraphicLCDIntf_Write8(dc, *ptr++);
        }
      }
      break;


  }
  return 1;
}

#else

uint8_t u8g_com_psoc5_ssd_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif
