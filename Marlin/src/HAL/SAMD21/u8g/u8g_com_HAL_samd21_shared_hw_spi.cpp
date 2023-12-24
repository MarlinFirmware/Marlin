/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2022 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

/**
 * SAMD21 HAL developed by Bart Meijer (brupje)
 * Based on SAMD51 HAL by Giuliano Zaro (AKA GMagician)
 */

/**
 * Based on u8g_com_msp430_hw_spi.c
 *
 * Universal 8bit Graphics Library
 *
 * Copyright (c) 2012, olikraus@gmail.com
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * * Redistributions of source code must retain the above copyright notice, this list
 * of conditions and the following disclaimer.
 *
 * * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or other
 * materials provided with the distribution.
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

#ifdef __SAMD21__

#include "../../../inc/MarlinConfigPre.h"

#if HAS_MARLINUI_U8GLIB

#include <U8glib-HAL.h>
#include "SPI.h"

#include "../../shared/HAL_SPI.h"

#ifndef LCD_SPI_SPEED
  #define LCD_SPI_SPEED SPI_HALF_SPEED
#endif

void u8g_SetPIOutput(u8g_t *u8g, uint8_t pin_index) {
  if (u8g->pin_list[pin_index]!= U8G_PIN_NONE)
  pinMode(u8g->pin_list[pin_index],OUTPUT);
}

void u8g_SetPILevel(u8g_t *u8g, uint8_t pin_index, uint8_t level) {
  if (u8g->pin_list[pin_index]!= U8G_PIN_NONE)
  digitalWrite(u8g->pin_list[pin_index],level);
}

uint8_t u8g_com_samd21_st7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr) {


  switch (msg) {
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_INIT:
      u8g_SetPIOutput(u8g, U8G_PI_CS);
      u8g_SetPIOutput(u8g, U8G_PI_A0);
      u8g_SetPIOutput(u8g, U8G_PI_RESET);

      u8g_SetPILevel(u8g, U8G_PI_CS, LOW);

      spiBegin();
      u8g->pin_list[U8G_PI_A0_STATE] = 0;
      break;

    case U8G_COM_MSG_ADDRESS:             // define cmd (arg_val = 0) or data mode (arg_val = 1)
      u8g_SetPILevel(u8g, U8G_PI_A0, arg_val);
      u8g->pin_list[U8G_PI_A0_STATE] = arg_val;
      break;

    case U8G_COM_MSG_CHIP_SELECT:         // arg_val == 1 means chip selected, but ST7920 is active high, so needs inverting
      u8g_SetPILevel(u8g, U8G_PI_CS, arg_val ? HIGH : LOW);
      break;

    case U8G_COM_MSG_RESET:
      u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      spiBeginTransaction(LCD_SPI_SPEED, MSBFIRST, SPI_MODE0);

      if (u8g->pin_list[U8G_PI_A0_STATE] == 0) { // command
        SPI.transfer(0x0f8); u8g->pin_list[U8G_PI_A0_STATE] = 2;
      }
      else if (u8g->pin_list[U8G_PI_A0_STATE] == 1) { // data
        SPI.transfer(0x0fa); u8g->pin_list[U8G_PI_A0_STATE] = 2;
      }

      SPI.transfer(arg_val & 0x0f0);
      SPI.transfer(arg_val << 4);
      SPI.endTransaction();
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      spiBeginTransaction(LCD_SPI_SPEED, MSBFIRST, SPI_MODE0);

      if (u8g->pin_list[U8G_PI_A0_STATE] == 0 ) { // command
        SPI.transfer(0x0f8); u8g->pin_list[U8G_PI_A0_STATE] = 2;
      }
      else if (u8g->pin_list[U8G_PI_A0_STATE] == 1) { // data
        SPI.transfer(0x0fa); u8g->pin_list[U8G_PI_A0_STATE] = 2;
      }

      uint8_t *ptr = (uint8_t*)arg_ptr;
      while (arg_val > 0) {
        SPI.transfer((*ptr) & 0x0f0);
        SPI.transfer((*ptr) << 4);
        ptr++;
        arg_val--;
      }

      SPI.endTransaction();
      break;
  }
  return 1;
}

#endif // HAS_MARLINUI_U8GLIB

#endif // __SAMD21__
