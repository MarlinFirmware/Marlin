/*
  
  u8g_com_arduino_st7920_hw_spi.c

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

#include "u8g.h"

#if defined(ARDUINO)

#if ARDUINO < 100 
#include <WProgram.h>    
#include "wiring_private.h"
#include "pins_arduino.h"

#else 
#include <Arduino.h> 
#include "wiring_private.h"
#endif

#if defined(__AVR__)
#define U8G_ARDUINO_ATMEGA_HW_SPI

/* remove the definition for attiny */
#if __AVR_ARCH__ == 2
#undef U8G_ATMEGA_HW_SPI
#endif
#if __AVR_ARCH__ == 25
#undef U8G_ATMEGA_HW_SPI
#endif

#endif


#if defined(U8G_ARDUINO_ATMEGA_HW_SPI)

#include <avr/interrupt.h>
#include <avr/io.h>

static uint8_t u8g_arduino_st7920_hw_spi_shift_out(u8g_t *u8g, uint8_t val) U8G_NOINLINE;
static uint8_t u8g_arduino_st7920_hw_spi_shift_out(u8g_t *u8g, uint8_t val)
{
  /* send data */
  SPDR = val;
  /* wait for transmission */
  while (!(SPSR & (1<<SPIF))) 
    ;
  /* clear the SPIF flag by reading SPDR */
  return  SPDR;
}


static void u8g_com_arduino_st7920_write_byte_hw_spi(u8g_t *u8g, uint8_t rs, uint8_t val) U8G_NOINLINE;
static void u8g_com_arduino_st7920_write_byte_hw_spi(u8g_t *u8g, uint8_t rs, uint8_t val)
{
  uint8_t i;

  if ( rs == 0 )
  {
    /* command */
    u8g_arduino_st7920_hw_spi_shift_out(u8g, 0x0f8);
  }
  else if ( rs == 1 )
  {
    /* data */
    u8g_arduino_st7920_hw_spi_shift_out(u8g, 0x0fa);
  }
  else
  {
    /* do nothing, keep same state */
  }
  
  u8g_arduino_st7920_hw_spi_shift_out(u8g, val & 0x0f0);
  u8g_arduino_st7920_hw_spi_shift_out(u8g, val << 4);

  for( i = 0; i < 4; i++ )
    u8g_10MicroDelay();
}


uint8_t u8g_com_arduino_st7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      u8g_com_arduino_assign_pin_output_high(u8g);
      u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
      
      DDRB |= _BV(3);          /* D0, MOSI */
      DDRB |= _BV(5);          /* SCK */
      DDRB |= _BV(2);		/* slave select */
    
      PORTB &= ~_BV(3);        /* D0, MOSI = 0 */
      PORTB &= ~_BV(5);        /* SCK = 0 */

      /*
        SPR1 SPR0
            0	0		fclk/4
            0	1		fclk/16 
            1	0		fclk/64  
            1	1		fclk/128
      */
      SPCR = 0;
      
      /* 20 Dez 2012: set CPOL and CPHA to 1 !!! */
      SPCR =  (1<<SPE) | (1<<MSTR)|(0<<SPR1)|(0<<SPR0)|(1<<CPOL)|(1<<CPHA);
#ifdef U8G_HW_SPI_2X
      SPSR = (1 << SPI2X);  /* double speed, issue 89 */
#endif
      u8g->pin_list[U8G_PI_A0_STATE] = 0;       /* inital RS state: command mode */
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
        /* disable, note: the st7920 has an active high chip select */
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, LOW);
      }
      else
      {
        /* enable */
        //u8g_com_arduino_digital_write(u8g, U8G_PI_SCK, LOW);
        u8g_com_arduino_digital_write(u8g, U8G_PI_CS, HIGH);
      }
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_com_arduino_st7920_write_byte_hw_spi(u8g,  u8g->pin_list[U8G_PI_A0_STATE], arg_val);
      // u8g->pin_list[U8G_PI_A0_STATE] = 2; 
      //u8g_arduino_sw_spi_shift_out(u8g->pin_list[U8G_PI_MOSI], u8g->pin_list[U8G_PI_SCK], arg_val);
      break;
    
    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_com_arduino_st7920_write_byte_hw_spi(u8g, u8g->pin_list[U8G_PI_A0_STATE], *ptr++);
          // u8g->pin_list[U8G_PI_A0_STATE] = 2; 
          arg_val--;
        }
      }
      break;

      case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_com_arduino_st7920_write_byte_hw_spi(u8g, u8g->pin_list[U8G_PI_A0_STATE], u8g_pgm_read(ptr) );
          // u8g->pin_list[U8G_PI_A0_STATE] = 2; 
          ptr++;
          arg_val--;
        }
      }
      break;
      
    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g->pin_list[U8G_PI_A0_STATE] = arg_val;
      break;
  }
  return 1;
}

#else

uint8_t u8g_com_arduino_st7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}
#endif

#else /* ARDUINO */

uint8_t u8g_com_arduino_st7920_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif /* ARDUINO */

