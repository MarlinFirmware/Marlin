/*

  u8g_com_arduino_hw_usart_spi.c

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

  SPI Clock Cycle Type

  SSD1351    50ns   20   MHz
  SSD1322   300ns    3.3 MHz
  SSD1327   300ns
  SSD1306   300ns
  ST7565    400ns    2.5 MHz
  ST7920    400ns

*/

#include "u8g.h"

#ifdef ARDUINO

#ifdef __AVR_ATmega32U4__ 

#include <avr/interrupt.h>
#include <avr/io.h>

#if ARDUINO < 100
#include <WProgram.h>
#else
#include <Arduino.h>
#endif



static uint8_t u8g_usart_spi_out(uint8_t data)
{
  /* send data */
  UDR1 = data;
  /* wait for empty transmit buffer */
  while(!(UCSR1A & (1 << UDRE1)));

  return  UDR1;
}


uint8_t u8g_com_arduino_hw_usart_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_INIT:
      /* SCK is already an output as we overwrite TXLED */
      u8g_com_arduino_assign_pin_output_high(u8g);
          u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);

          // Init interface at 2MHz
          UBRR1 = 0x00;
          UCSR1C = (1 << UMSEL11) | (1 << UMSEL10);
      UCSR1B = (1 << TXEN1);
      UBRR1 = 3;

      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g_com_arduino_digital_write(u8g, U8G_PI_A0, arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:
      if ( arg_val == 0 )
      {
        /* disable */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);
      }
      else
      {
        /* enable */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
      }
      break;

    case U8G_COM_MSG_RESET:
      if ( u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE )
        u8g_com_arduino_digital_write(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_usart_spi_out(arg_val);
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_usart_spi_out(*ptr++);
          arg_val--;
        }
      }
      break;
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_usart_spi_out(u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;
  }
  return 1;
}

/* #elif defined(__18CXX) || defined(__PIC32MX) */
/* #elif defined(__arm__)               // Arduino Due, maybe we should better check for __SAM3X8E__ */

#else /* __AVR_ATmega32U4__ */

#endif /* __AVR_ATmega32U4__ */

#else /* ARDUINO */

uint8_t u8g_com_arduino_hw_usart_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif /* ARDUINO */

