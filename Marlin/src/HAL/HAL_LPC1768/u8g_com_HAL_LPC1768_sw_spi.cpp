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

  adapted from u8g_com_std_sw_spi.c

  Universal 8bit Graphics Library

  Copyright (c) 2015, olikraus@gmail.com
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



#if defined (TARGET_LPC1768)


#include <U8glib.h>

#include <lpc17xx_pinsel.h>

#define LPC_PORT_OFFSET         (0x0020)
#define LPC_PIN(pin)            (1UL << pin)
#define LPC_GPIO(port)          ((volatile LPC_GPIO_TypeDef *)(LPC_GPIO0_BASE + LPC_PORT_OFFSET * port))

void delayMicroseconds(uint32_t us);
void pinMode(int16_t pin, uint8_t mode);
void digitalWrite(int16_t pin, uint8_t pin_status);


uint8_t SCK_pin, SCK_port, MOSI_pin, MOSI_port;

#define SPI_SPEED 2  //20: 200KHz 5:750KHz 2:3-4MHz

static void u8g_sw_spi_HAL_LPC1768_shift_out(uint8_t dataPin, uint8_t clockPin, uint8_t val)
{
  for (uint8_t i = 0; i < 8; i++) {

    if (val & 0x80)
      for (uint8_t j = 0; j < SPI_SPEED; j++) {
        LPC_GPIO(MOSI_port)->FIOSET = LPC_PIN(MOSI_pin);
        LPC_GPIO(MOSI_port)->FIOSET = LPC_PIN(MOSI_pin);
        LPC_GPIO(MOSI_port)->FIOSET = LPC_PIN(MOSI_pin);
      }
    else
      for (uint8_t j = 0; j < SPI_SPEED; j++) {
        LPC_GPIO(MOSI_port)->FIOCLR = LPC_PIN(MOSI_pin);
        LPC_GPIO(MOSI_port)->FIOCLR = LPC_PIN(MOSI_pin);
        LPC_GPIO(MOSI_port)->FIOCLR = LPC_PIN(MOSI_pin);
      }

    for (uint8_t j = 0; j < SPI_SPEED; j++) {
      LPC_GPIO(SCK_port)->FIOSET = LPC_PIN(SCK_pin);
      LPC_GPIO(SCK_port)->FIOSET = LPC_PIN(SCK_pin);
      LPC_GPIO(SCK_port)->FIOSET = LPC_PIN(SCK_pin);
      LPC_GPIO(SCK_port)->FIOSET = LPC_PIN(SCK_pin);
      LPC_GPIO(SCK_port)->FIOSET = LPC_PIN(SCK_pin);
    }

    for (uint8_t j = 0; j < SPI_SPEED; j++) {
      LPC_GPIO(SCK_port)->FIOCLR = LPC_PIN(SCK_pin);
      LPC_GPIO(SCK_port)->FIOCLR = LPC_PIN(SCK_pin);
    }
    val = val << 1;
  }
}


uint8_t u8g_com_HAL_LPC1768_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{


  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      #define LPC1768_PIN_PORT(pin) ((uint8_t)((pin >> 5) & 0b111))
      #define LPC1768_PIN_PIN(pin) ((uint8_t)(pin & 0b11111))
      SCK_pin = LPC1768_PIN_PIN(u8g->pin_list[U8G_PI_SCK]);
      SCK_port = LPC1768_PIN_PORT(u8g->pin_list[U8G_PI_SCK]);
      MOSI_pin = LPC1768_PIN_PIN(u8g->pin_list[U8G_PI_MOSI]);
      MOSI_port = LPC1768_PIN_PORT(u8g->pin_list[U8G_PI_MOSI]);
      // As defined by Arduino INPUT(0x0), OUPUT(0x1), INPUT_PULLUP(0x2)
      #define OUPUT 0x1
      pinMode(u8g->pin_list[U8G_PI_SCK], OUPUT);
      pinMode(u8g->pin_list[U8G_PI_MOSI], OUPUT);
      pinMode(u8g->pin_list[U8G_PI_CS], OUPUT);
      pinMode(u8g->pin_list[U8G_PI_A0], OUPUT);
      if (U8G_PIN_NONE != u8g->pin_list[U8G_PI_RESET])  pinMode(u8g->pin_list[U8G_PI_RESET], OUPUT);
      digitalWrite(u8g->pin_list[U8G_PI_SCK], 0);
      digitalWrite(u8g->pin_list[U8G_PI_MOSI], 0);
      break;

    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_RESET:
      digitalWrite(u8g->pin_list[U8G_PI_RESET], arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      digitalWrite(u8g->pin_list[U8G_PI_CS], !arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_sw_spi_HAL_LPC1768_shift_out(u8g->pin_list[U8G_PI_MOSI], u8g->pin_list[U8G_PI_SCK], arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      {
        uint8_t *ptr = (uint8_t *)arg_ptr;
        while( arg_val > 0 )
        {
          u8g_sw_spi_HAL_LPC1768_shift_out(u8g->pin_list[U8G_PI_MOSI], u8g->pin_list[U8G_PI_SCK], *ptr++);
          arg_val--;
        }
      }
      break;

      case U8G_COM_MSG_WRITE_SEQ_P:
      {
        uint8_t *ptr = (uint8_t *)arg_ptr;
        while( arg_val > 0 )
        {
          u8g_sw_spi_HAL_LPC1768_shift_out(u8g->pin_list[U8G_PI_MOSI], u8g->pin_list[U8G_PI_SCK], u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      digitalWrite(u8g->pin_list[U8G_PI_A0], arg_val);
      break;
  }
  return 1;
}

#endif  // TARGET_LPC1768
