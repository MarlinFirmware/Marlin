/*

  u8g_com_atxmega_hw_spi.c

  Universal 8bit Graphics Library

  Copyright (c) 2011, olikraus@gmail.com
  Copyright (c) 2015, florianmenne@t-online.de
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

  A special SPI interface for ST7920 controller with HW SPI Support

  Assumes, that
    MOSI is at PORTB, Pin 3
  and
    SCK is at PORTB, Pin 5

  Update for ATOMIC operation done (01 Jun 2013)
    U8G_ATOMIC_OR(ptr, val)
    U8G_ATOMIC_AND(ptr, val)
    U8G_ATOMIC_START()
    U8G_ATOMIC_END()


*/

#include "u8g.h"

#ifdef __AVR_XMEGA__
#define U8G_ATXMEGA_HW_SPI
#endif


#ifdef U8G_ATXMEGA_HW_SPI

#include <avr/interrupt.h>
#include <avr/io.h>


static uint8_t u8g_atxmega_spi_out(uint8_t data)
{
  /* send data */
  SPIC.DATA = data;

  /* wait for transmission */
  while(!(SPIC.STATUS & SPI_IF_bm));

  /* clear the SPIF flag by reading SPDR */
  return SPIC.DATA;
}


uint8_t u8g_com_atxmega_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
    break;

    case U8G_COM_MSG_INIT:

    u8g_SetPIOutput(u8g, U8G_PI_CS);
    u8g_SetPIOutput(u8g, U8G_PI_A0);
    u8g_SetPIOutput(u8g, U8G_PI_RESET);

    //U8G_ATOMIC_START();

    PORTC.DIR |= PIN4_bm | PIN5_bm | PIN7_bm;
    PORTC.DIR &= ~PIN6_bm;

    //U8G_ATOMIC_END();

    u8g_SetPILevel(u8g, U8G_PI_CS, 1);

    SPIC.CTRL = 0;
    SPIC.CTRL     = SPI_PRESCALER_DIV4_gc |		// SPI prescaler.
    //SPI_CLK2X_bm |			 //SPI Clock double.
    SPI_ENABLE_bm |			 //Enable SPI module.
    //SPI_DORD_bm |			 //Data order.
    SPI_MASTER_bm |			 //SPI master.
    SPI_MODE_0_gc;			// SPI mode.

#ifdef U8G_HW_SPI_2X
    SPIC.CTRL |= SPI_CLK2X_bm;  /* double speed, issue 89 */
#endif

    break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
    u8g_SetPILevel(u8g, U8G_PI_A0, arg_val);
    break;

    case U8G_COM_MSG_CHIP_SELECT:

    if ( arg_val == 0 )
    {
      /* disable */
      u8g_SetPILevel(u8g, U8G_PI_CS, 1);
    }
    else
    {
      //PORTB &= ~_BV(5);        /* SCK = 0 */
      PORTC.OUT &= ~PIN7_bm;
      /* enable */
      u8g_SetPILevel(u8g, U8G_PI_CS, 0); /* CS = 0 (low active) */
    }

    break;

    case U8G_COM_MSG_RESET:
    u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
    break;

    case U8G_COM_MSG_WRITE_BYTE:
    u8g_atxmega_spi_out(arg_val);
    break;

    case U8G_COM_MSG_WRITE_SEQ:
    {
      register uint8_t *ptr = arg_ptr;
      while( arg_val > 0 )
      {
        u8g_atxmega_spi_out(*ptr++);
        arg_val--;
      }
    }
    break;
    case U8G_COM_MSG_WRITE_SEQ_P:
    {
      register uint8_t *ptr = arg_ptr;
      while( arg_val > 0 )
      {
        u8g_atxmega_spi_out(u8g_pgm_read(ptr));
        ptr++;
        arg_val--;
      }
    }
    break;
  }
  return 1;
}

#else

uint8_t u8g_com_atxmega_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif