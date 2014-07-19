/*
  
  u8g_arduino_ATtiny85_std_hw_spi.c

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

// Uses code from tinySPI Written by Nick Gammon
// March 2013

// ATMEL ATTINY45 / ARDUINO pin mappings
//
//                         +-\/-+
// RESET  Ain0 (D 5) PB5  1|    |8  Vcc
// CLK1   Ain3 (D 3) PB3  2|    |7  PB2 (D 2) Ain1  SCK  / USCK / SCL
// CLK0   Ain2 (D 4) PB4  3|    |6  PB1 (D 1) pwm1  MISO / DO
//                   GND  4|    |5  PB0 (D 0) pwm0  MOSI / DI / SDA
//                         +----+


#include "u8g.h"


#if defined(ARDUINO) && defined(__AVR_ATtiny85__)

#if ARDUINO < 100 
#include <WProgram.h>
#else 
#include <Arduino.h> 
#endif

const byte DI   = 0;  // D0, pin 5  Data In
const byte DO   = 1;  // D1, pin 6  Data Out (this is *not* MOSI)
const byte USCK = 2;  // D2, pin 7  Universal Serial Interface clock

uint8_t u8g_arduino_ATtiny85_spi_out(uint8_t val)
{
  USIDR = val;  // byte to output
  USISR = _BV (USIOIF);  // clear Counter Overflow Interrupt Flag, set count to zero 
  do
  {
    USICR = _BV (USIWM0)   // 3-wire mode
          | _BV (USICS1) | _BV (USICLK)  // Software clock strobe
          | _BV (USITC);   // Toggle Clock Port Pin
  }
  while ((USISR & _BV (USIOIF)) == 0);  // until Counter Overflow Interrupt Flag set
  
  return USIDR;  // return read data
}

uint8_t u8g_com_arduino_ATtiny85_std_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);  // ensure SS stays high until needed
      pinMode (USCK, OUTPUT);
      pinMode (DO, OUTPUT);
      pinMode (u8g->pin_list[U8G_PI_CS], OUTPUT);
      pinMode (u8g->pin_list[U8G_PI_A0], OUTPUT);
      USICR = _BV (USIWM0);  // 3-wire mode
      u8g_MicroDelay();
      break;
    
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_RESET:
      if ( u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE )
        u8g_com_arduino_digital_write(u8g, U8G_PI_RESET, arg_val);
      break;
      
    case U8G_COM_MSG_CHIP_SELECT:
      if ( arg_val == 0 )
      {
        /* disable */
        u8g_MicroDelay();
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);
        u8g_MicroDelay();
      }
      else
      {
        /* enable */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
        u8g_MicroDelay();
      }
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_arduino_ATtiny85_spi_out(arg_val);
      u8g_MicroDelay();
      break;
    
    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_arduino_ATtiny85_spi_out(*ptr++);
          arg_val--;
        }
      }
      break;

      case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_arduino_ATtiny85_spi_out(u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;
      
    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g_com_arduino_digital_write(u8g, U8G_PI_A0, arg_val);
      u8g_MicroDelay();
      break;
  }
  return 1;
}

#else /* ARDUINO */

uint8_t u8g_com_arduino_ATtiny85_std_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif /* ARDUINO */