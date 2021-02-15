/*

  u8g_com_atmega_st7920_spi.c

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

  A special SPI interface for ST7920 controller

*/

#include "u8g.h"

#ifdef __AVR__

static void u8g_atmega_st7920_sw_spi_shift_out(u8g_t *u8g, uint8_t val) U8G_NOINLINE;
static void u8g_atmega_st7920_sw_spi_shift_out(u8g_t *u8g, uint8_t val)
{
  uint8_t i = 8;
  do
  {
    u8g_SetPILevel(u8g, U8G_PI_MOSI, val & 128 );
    val <<= 1;
    u8g_SetPILevel(u8g, U8G_PI_SCK, 1 );
    u8g_MicroDelay();		/* 15 Aug 2012: added for high speed uC */
    u8g_SetPILevel(u8g, U8G_PI_SCK, 0 );
    u8g_MicroDelay();		/* 15 Aug 2012: added for high speed uC */
    i--;
  } while( i != 0 );
}

static void u8g_com_atmega_st7920_write_byte(u8g_t *u8g, uint8_t rs, uint8_t val) U8G_NOINLINE;
static void u8g_com_atmega_st7920_write_byte(u8g_t *u8g, uint8_t rs, uint8_t val)
{
  uint8_t i;

  if ( rs == 0 )
  {
    /* command */
    u8g_atmega_st7920_sw_spi_shift_out(u8g, 0x0f8);
  }
  else if ( rs == 1 )
  {
    /* data */
    u8g_atmega_st7920_sw_spi_shift_out(u8g, 0x0fa);
  }

  u8g_atmega_st7920_sw_spi_shift_out(u8g, val & 0x0f0);
  u8g_atmega_st7920_sw_spi_shift_out(u8g, val << 4);

  for( i = 0; i < 4; i++ )
    u8g_10MicroDelay();
}


uint8_t u8g_com_atmega_st7920_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_INIT:
      u8g_SetPIOutput(u8g, U8G_PI_SCK);
      u8g_SetPIOutput(u8g, U8G_PI_MOSI);
      /* u8g_SetPIOutput(u8g, U8G_PI_A0); */
      u8g_SetPIOutput(u8g, U8G_PI_CS);
      u8g_SetPIOutput(u8g, U8G_PI_RESET);

      u8g_SetPILevel(u8g, U8G_PI_SCK, 0 );
      u8g_SetPILevel(u8g, U8G_PI_MOSI, 0 );
      u8g_SetPILevel(u8g, U8G_PI_CS, 0 );
      /* u8g_SetPILevel(u8g, U8G_PI_A0, 0); */

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
      if ( arg_val == 0 )
      {
        /* disable, note: the st7920 has an active high chip select */
        u8g_SetPILevel(u8g, U8G_PI_CS, 0);
      }
      else
      {
        /* u8g_SetPILevel(u8g, U8G_PI_SCK, 0 ); */
        /* enable */
        u8g_SetPILevel(u8g, U8G_PI_CS, 1); /* CS = 1 (high active) */
      }
      break;


    case U8G_COM_MSG_WRITE_BYTE:
      u8g_com_atmega_st7920_write_byte(u8g, u8g->pin_list[U8G_PI_A0_STATE], arg_val);
      u8g->pin_list[U8G_PI_A0_STATE] = 2;
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_com_atmega_st7920_write_byte(u8g, u8g->pin_list[U8G_PI_A0_STATE], *ptr++);
	  u8g->pin_list[U8G_PI_A0_STATE] = 2;
          arg_val--;
        }
      }
      break;

      case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_com_atmega_st7920_write_byte(u8g, u8g->pin_list[U8G_PI_A0_STATE], u8g_pgm_read(ptr));
	  u8g->pin_list[U8G_PI_A0_STATE] = 2;
          ptr++;
          arg_val--;
        }
      }
      break;
  }
  return 1;
}

#else


uint8_t u8g_com_atmega_st7920_sw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}


#endif

