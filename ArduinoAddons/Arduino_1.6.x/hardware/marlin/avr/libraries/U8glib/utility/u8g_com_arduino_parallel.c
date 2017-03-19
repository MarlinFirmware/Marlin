/*
  
  u8g_com_arduino_parallel.c

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
  

  PIN_D0 8
  PIN_D1 9
  PIN_D2 10
  PIN_D3 11
  PIN_D4 4
  PIN_D5 5
  PIN_D6 6
  PIN_D7 7

  PIN_CS1 14
  PIN_CS2 15
  PIN_RW 16
  PIN_DI 17
  PIN_EN 18
  
  u8g_Init8Bit(u8g, dev, d0, d1, d2, d3, d4, d5, d6, d7, en, cs1, cs2, di, rw, reset)
  u8g_Init8Bit(u8g, dev,  8,    9, 10, 11,   4,   5,   6,   7, 18, 14, 15, 17, 16, U8G_PIN_NONE)

*/

#include "u8g.h"


#if  defined(ARDUINO)

#if ARDUINO < 100 
#include <WProgram.h> 
#else 
#include <Arduino.h> 
#endif






void u8g_com_arduino_parallel_write(u8g_t *u8g, uint8_t val)
{
  u8g_com_arduino_digital_write(u8g, U8G_PI_D0, val&1);
  val >>= 1;
  u8g_com_arduino_digital_write(u8g, U8G_PI_D1, val&1);
  val >>= 1;
  u8g_com_arduino_digital_write(u8g, U8G_PI_D2, val&1);
  val >>= 1;
  u8g_com_arduino_digital_write(u8g, U8G_PI_D3, val&1);
  val >>= 1;
  u8g_com_arduino_digital_write(u8g, U8G_PI_D4, val&1);
  val >>= 1;
  u8g_com_arduino_digital_write(u8g, U8G_PI_D5, val&1);
  val >>= 1;
  u8g_com_arduino_digital_write(u8g, U8G_PI_D6, val&1);
  val >>= 1;
  u8g_com_arduino_digital_write(u8g, U8G_PI_D7, val&1);
  
  /* EN cycle time must be 1 micro second, digitalWrite is slow enough to do this */
  u8g_com_arduino_digital_write(u8g, U8G_PI_EN, HIGH);
  u8g_MicroDelay(); /* delay by 1000ns, reference: ST7920: 140ns, SBN1661: 100ns */
  u8g_com_arduino_digital_write(u8g, U8G_PI_EN, LOW);
  u8g_10MicroDelay(); /* ST7920 commands: 72us */
}


uint8_t u8g_com_arduino_parallel_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      /* setup the RW pin as output and force it to low */
      if ( u8g->pin_list[U8G_PI_RW] != U8G_PIN_NONE )
      {
        pinMode(u8g->pin_list[U8G_PI_RW], OUTPUT);
        u8g_com_arduino_digital_write(u8g, U8G_PI_RW, LOW);
      }
      /* set all pins (except RW pin) */
      u8g_com_arduino_assign_pin_output_high(u8g);
      break;
    case U8G_COM_MSG_STOP:
      break;
    case U8G_COM_MSG_CHIP_SELECT:
      if ( arg_val == 0 )
      {
        /* disable */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS1, HIGH);
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS2, HIGH);
      }
      else if ( arg_val == 1 )
      {
        /* enable */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS1, LOW);
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS2, HIGH);
      }
      else if ( arg_val == 2 )
      {
        /* enable */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS1, HIGH);
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS2, LOW);
      }
      else
      {
        /* enable */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS1, LOW);
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS2, LOW);
      }
      break;
    case U8G_COM_MSG_WRITE_BYTE:
      u8g_com_arduino_parallel_write(u8g, arg_val);
      break;
    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_com_arduino_parallel_write(u8g, *ptr++);
          arg_val--;
        }
      }
      break;
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_com_arduino_parallel_write(u8g, u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;
    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g_com_arduino_digital_write(u8g, U8G_PI_DI, arg_val);
      break;
    case U8G_COM_MSG_RESET:
      if ( u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE )
        u8g_com_arduino_digital_write(u8g, U8G_PI_RESET, arg_val);
      break;
      
  }
  return 1;
}

#else


uint8_t u8g_com_arduino_parallel_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif /* ARDUINO */

