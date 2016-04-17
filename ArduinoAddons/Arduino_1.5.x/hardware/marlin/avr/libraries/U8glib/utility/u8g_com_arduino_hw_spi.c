/*
  
  u8g_com_arduino_hw_spi.c

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

#include "u8g.h"

#if defined(ARDUINO)

#if defined(__AVR__)

#include <avr/interrupt.h>
#include <avr/io.h>

#if ARDUINO < 100 
#include <WProgram.h> 

/* fixed pins */
#if defined(__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__) // Sanguino.cc board
#define PIN_SCK         7
#define PIN_MISO        6
#define PIN_MOSI        5
#define PIN_CS          4
#else                                   // Arduino Board
#define PIN_SCK 13
#define PIN_MISO  12
#define PIN_MOSI 11
#define PIN_CS 10
#endif // (__AVR_ATmega644P__) || defined(__AVR_ATmega1284P__)

#else 

#include <Arduino.h> 

/* use Arduino pin definitions */
#define PIN_SCK SCK
#define PIN_MISO  MISO
#define PIN_MOSI MOSI
#define PIN_CS SS

#endif



//static uint8_t u8g_spi_out(uint8_t data) U8G_NOINLINE;
static uint8_t u8g_spi_out(uint8_t data)
{
  /* unsigned char x = 100; */
  /* send data */
  SPDR = data;
  /* wait for transmission */
  while (!(SPSR & (1<<SPIF))) 
    ;
  /* clear the SPIF flag by reading SPDR */
  return  SPDR;
}


uint8_t u8g_com_arduino_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      break;
    
    case U8G_COM_MSG_INIT:
      u8g_com_arduino_assign_pin_output_high(u8g);
      pinMode(PIN_SCK, OUTPUT);
      digitalWrite(PIN_SCK, LOW);
      pinMode(PIN_MOSI, OUTPUT);
      digitalWrite(PIN_MOSI, LOW);
      /* pinMode(PIN_MISO, INPUT); */

      pinMode(PIN_CS, OUTPUT);			/* system chip select for the atmega board */
      digitalWrite(PIN_CS, HIGH);
    


      /*
        SPR1 SPR0
            0	0		fclk/4
            0	1		fclk/16
            1	0		fclk/64
            1	1		fclk/128
      */
      SPCR = 0;
      SPCR =  (1<<SPE) | (1<<MSTR)|(0<<SPR1)|(1<<SPR0)|(0<<CPOL)|(0<<CPHA);
#ifdef U8G_HW_SPI_2X
      SPSR = (1 << SPI2X);  /* double speed, issue 89 */
#endif
      
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
        u8g_com_arduino_digital_write(u8g, U8G_PI_SCK, LOW);
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
      }
      break;
      
    case U8G_COM_MSG_RESET:
      if ( u8g->pin_list[U8G_PI_RESET] != U8G_PIN_NONE )
        u8g_com_arduino_digital_write(u8g, U8G_PI_RESET, arg_val);
      break;
    
    case U8G_COM_MSG_WRITE_BYTE:
      u8g_spi_out(arg_val);
      break;
    
    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_spi_out(*ptr++);
          arg_val--;
        }
      }
      break;
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_spi_out(u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
      }
      break;
  }
  return 1;
}

/* #elif defined(__18CXX) || defined(__PIC32MX) */

#else /* __AVR__ */

#endif /* __AVR__ */

#else /* ARDUINO */

uint8_t u8g_com_arduino_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif /* ARDUINO */

