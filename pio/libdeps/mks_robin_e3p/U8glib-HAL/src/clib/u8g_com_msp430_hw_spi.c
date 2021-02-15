/*

  u8g_com_msp430_hw_spi.c

  Universal 8bit Graphics Library

  Copyright (c) 2012, olikraus@gmail.com
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

#ifdef __MSP430__
#define U8G_MSP430_HW_SPI
#endif

#ifdef U8G_MSP430_HW_SPI

#include <msp430.h>

#ifndef F_CPU
#error "Please specifiy actual master clock using F_CPU in HZ"
#endif
#ifndef F_SPI
#define F_SPI  1000000UL
#endif

#define U8G_USE_USCIA0	1
#define U8G_USE_USCIB0	2
#define U8G_USE_USCIA1	3
#define U8G_USE_USCIB1	4
#define U8G_USE_USCIA2	5
#define U8G_USE_USCIB2	6
#define U8G_USE_USCIA3	7
#define U8G_USE_USCIB3	8

#ifndef U8G_USE_USCI
#define U8G_USE_USCI	U8G_USE_USCIB0
#endif

#if U8G_USE_USCI == 1
#define UCIFG	UCA0IFG
#define UCTXBUF UCA0TXBUF
#define UCSTAT  UCA0STAT
#define UCCTL0  UCA0CTL0
#define UCCTL1  UCA0CTL1
#define UCBR0   UCA0BR0
#define UCBR1   UCA0BR1
#elif U8G_USE_USCI == 2
#define UCIFG	UCB0IFG
#define UCTXBUF UCB0TXBUF
#define UCSTAT  UCB0STAT
#define UCCTL0  UCB0CTL0
#define UCCTL1  UCB0CTL1
#define UCBR0   UCB0BR0
#define UCBR1   UCB0BR1
#elif U8G_USE_USCI == 3
#define UCIFG	UCA1IFG
#define UCTXBUF UCA1TXBUF
#define UCSTAT  UCA1STAT
#define UCCTL0  UCA1CTL0
#define UCCTL1  UCA1CTL1
#define UCBR0   UCA1BR0
#define UCBR1   UCA1BR1
#elif U8G_USE_USCI == 4
#define UCIFG	UCB1IFG
#define UCTXBUF UCB1TXBUF
#define UCSTAT  UCB1STAT
#define UCCTL0  UCB1CTL0
#define UCCTL1  UCB1CTL1
#define UCBR0   UCB1BR0
#define UCBR1   UCB1BR1
#elif U8G_USE_USCI == 5
#define UCIFG	UCA2IFG
#define UCTXBUF UCA2TXBUF
#define UCSTAT  UCA2STAT
#define UCCTL0  UCA2CTL0
#define UCCTL1  UCA2CTL1
#define UCBR0   UCA2BR0
#define UCBR1   UCA2BR1
#elif U8G_USE_USCI == 6
#define UCIFG	UCB2IFG
#define UCTXBUF UCB2TXBUF
#define UCSTAT  UCB2STAT
#define UCCTL0  UCB2CTL0
#define UCCTL1  UCB2CTL1
#define UCBR0   UCB2BR0
#define UCBR1   UCB2BR1
#elif U8G_USE_USCI == 7
#define UCIFG	UCA3IFG
#define UCTXBUF UCA3TXBUF
#define UCSTAT  UCA3STAT
#define UCCTL0  UCA3CTL0
#define UCCTL1  UCA3CTL1
#define UCBR0   UCA3BR0
#define UCBR1   UCA3BR1
#elif U8G_USE_USCI == 8
#define UCIFG	UCB3IFG
#define UCTXBUF UCB3TXBUF
#define UCSTAT  UCB3STAT
#define UCCTL0  UCB3CTL0
#define UCCTL1  UCB3CTL1
#define UCBR0   UCB3BR0
#define UCBR1   UCB3BR1
#endif

inline void u8g_msp430_spi_out(uint8_t data)
{
  while (!(UCIFG&UCTXIFG));
  UCTXBUF = data;
}

uint8_t u8g_com_msp430_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  switch(msg)
  {
    case U8G_COM_MSG_STOP:
      break;

    case U8G_COM_MSG_INIT:
	/*
 	 * on MSP430 you need to set PSEL alternative function which
 	 * may not be required on other MCU's - should be handled
 	 * by a low level u8g_SetPinAlternate(pin_number)...
 	 */
      UCCTL1 |= UCSWRST;                      // **Put state machine in reset**
      UCCTL0 |= UCMST|UCSYNC|UCCKPL|UCMSB;    // 3-pin, 8-bit SPI master Clock polarity high, MSB
      UCCTL1 |= UCSSEL_2;                     // SMCLK
      UCBR0 = (unsigned char)(F_CPU/F_SPI);   //
      UCBR1 = 0;                              //
      UCCTL1 &= ~UCSWRST;                     // **Initialize USCI state machine**
      u8g_SetPILevel(u8g, U8G_PI_CS, 1);
      u8g_SetPILevel(u8g, U8G_PI_A0, 1);
      u8g_SetPILevel(u8g, U8G_PI_RESET, 1);
      u8g_SetPIOutput(u8g, U8G_PI_CS);
      u8g_SetPIOutput(u8g, U8G_PI_A0);
      u8g_SetPIOutput(u8g, U8G_PI_RESET);

      break;

    case U8G_COM_MSG_ADDRESS:                     /* define cmd (arg_val = 0) or data mode (arg_val = 1) */
      u8g_SetPILevel(u8g, U8G_PI_A0, arg_val);
      break;

    case U8G_COM_MSG_CHIP_SELECT:

       u8g_SetPILevel(u8g, U8G_PI_CS, (arg_val ? 0 : 1));
      break;

    case U8G_COM_MSG_RESET:

      u8g_SetPILevel(u8g, U8G_PI_RESET, arg_val);
      break;

    case U8G_COM_MSG_WRITE_BYTE:
      u8g_msp430_spi_out(arg_val);
      while ((UCSTAT&UCBUSY));
      break;

    case U8G_COM_MSG_WRITE_SEQ:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_msp430_spi_out(*ptr++);
          arg_val--;
        }
        while ((UCSTAT&UCBUSY));
      }
      break;
    case U8G_COM_MSG_WRITE_SEQ_P:
      {
        register uint8_t *ptr = arg_ptr;
        while( arg_val > 0 )
        {
          u8g_msp430_spi_out(u8g_pgm_read(ptr));
          ptr++;
          arg_val--;
        }
        while ((UCSTAT&UCBUSY));
      }
      break;
  }
  return 1;
}

#else

uint8_t u8g_com_msp430_hw_spi_fn(u8g_t *u8g, uint8_t msg, uint8_t arg_val, void *arg_ptr)
{
  return 1;
}

#endif


