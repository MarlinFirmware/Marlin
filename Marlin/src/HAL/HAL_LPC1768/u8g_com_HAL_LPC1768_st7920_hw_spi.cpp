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

  based on u8g_com_LPC1768_st7920_hw_spi.c

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

#ifdef TARGET_LPC1768

//  #include <inttypes.h>

//  #include "src/core/macros.h"
//  #include "Configuration.h"

  #include <U8glib.h>

  #define SPI_FULL_SPEED 0
  #define SPI_HALF_SPEED 1
  #define SPI_QUARTER_SPEED 2
  #define SPI_EIGHTH_SPEED 3
  #define SPI_SIXTEENTH_SPEED 4
  #define SPI_SPEED_5 5
  #define SPI_SPEED_6 6


  void spiBegin();
  void spiInit(uint8_t spiRate);
  void spiSend(uint8_t b);
  void spiSend(const uint8_t* buf, size_t n);


  static uint8_t rs_last_state = 255;

  static void u8g_com_LPC1768_st7920_write_byte_hw_spi(uint8_t rs, uint8_t val)
  {
    uint8_t i;

    if ( rs != rs_last_state) {  // time to send a command/data byte
      rs_last_state = rs;

      if ( rs == 0 )
        /* command */
        spiSend(0x0f8);
      else
         /* data */
        spiSend(0x0fa);

      for( i = 0; i < 4; i++ )   // give the controller some time to process the data
        u8g_10MicroDelay();      // 2 is bad, 3 is OK, 4 is safe
    }

    spiSend(val & 0x0f0);
    spiSend(val << 4);
  }


  uint8_t u8g_com_HAL_LPC1768_ST7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
  {
    switch(msg)
    {
      case U8G_COM_MSG_INIT:
        u8g_SetPILevel(u8g, U8G_PI_CS, 0);
        u8g_SetPIOutput(u8g, U8G_PI_CS);
        u8g_Delay(5);
        spiBegin();
        spiInit(SPI_EIGHTH_SPEED);  // ST7920 max speed is about 1.1 MHz
        u8g->pin_list[U8G_PI_A0_STATE] = 0;       /* inital RS state: command mode */
        break;

      case U8G_COM_MSG_STOP:
        break;

      case U8G_COM_MSG_RESET:
        u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
        break;

      case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
        u8g->pin_list[U8G_PI_A0_STATE] = arg_val;
        break;

      case U8G_COM_MSG_CHIP_SELECT:
        u8g_SetPILevel(u8g, U8G_PI_CS, arg_val);  //note: the st7920 has an active high chip select
        break;

      case U8G_COM_MSG_WRITE_BYTE:
        u8g_com_LPC1768_st7920_write_byte_hw_spi(u8g->pin_list[U8G_PI_A0_STATE], arg_val);
        break;

      case U8G_COM_MSG_WRITE_SEQ: {
          uint8_t *ptr = (uint8_t*) arg_ptr;
          while (arg_val > 0) {
            u8g_com_LPC1768_st7920_write_byte_hw_spi(u8g->pin_list[U8G_PI_A0_STATE], *ptr++);
            arg_val--;
          }
        }
        break;

        case U8G_COM_MSG_WRITE_SEQ_P: {
          uint8_t *ptr = (uint8_t*) arg_ptr;
          while (arg_val > 0) {
            u8g_com_LPC1768_st7920_write_byte_hw_spi(u8g->pin_list[U8G_PI_A0_STATE], *ptr++);
            arg_val--;
          }
        }
        break;
    }
    return 1;
  }

#endif  // TARGET_LPC1768
