/*
  
  u8g_com_io.c
  
  abstraction layer for low level i/o 

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

  Update for ATOMIC operation done (01 Jun 2013)
    U8G_ATOMIC_OR(ptr, val)
    U8G_ATOMIC_AND(ptr, val)
    U8G_ATOMIC_START();
    U8G_ATOMIC_END();

  uint8_t u8g_Pin(uint8_t port, uint8_t bitpos)						Convert to internal number: AVR: port*8+bitpos, ARM: port*16+bitpos
  void u8g_SetPinOutput(uint8_t internal_pin_number)
  void u8g_SetPinInput(uint8_t internal_pin_number)
  void u8g_SetPinLevel(uint8_t internal_pin_number, uint8_t level)
  uint8_t u8g_GetPinLevel(uint8_t internal_pin_number)


*/

#include "u8g.h"

#if defined(__AVR__)

#include <avr/interrupt.h>
#include <avr/io.h>

typedef volatile uint8_t * IO_PTR;

/* create internal pin number */
uint8_t u8g_Pin(uint8_t port, uint8_t bitpos)
{
  port <<= 3;
  port += bitpos;
  return port;
}

#if defined(__AVR_XMEGA__)

const IO_PTR u8g_avr_ddr_P[] PROGMEM = {
#ifdef PORTA
  &PORTA.DIR,
#else
  0,
#endif
  &PORTB.DIR,
#ifdef PORTC
  &PORTC.DIR,
#ifdef PORTD
  &PORTD.DIR,
#ifdef PORTE
  &PORTE.DIR,
#ifdef PORTF
  &PORTF.DIR,
#ifdef PORTR
  &PORTR.DIR,
#endif
#endif
#endif
#endif
#endif
};


const IO_PTR u8g_avr_port_P[] PROGMEM = {
#ifdef PORTA
  &PORTA.OUT,
#else
  0,
#endif
  &PORTB.OUT,
#ifdef PORTC
  &PORTC.OUT,
#ifdef PORTD
  &PORTD.OUT,
#ifdef PORTE
  &PORTE.OUT,
#ifdef PORTF
  &PORTF.OUT,
#ifdef PORTR
  &PORTR.OUT,
#endif
#endif
#endif
#endif
#endif
};

const IO_PTR u8g_avr_pin_P[] PROGMEM = {
#ifdef PORTA
  &PORTA.IN,
#else
  0,
#endif
  &PORTB.IN,
#ifdef PORTC
  &PORTC.IN,
#ifdef PORTD
  &PORTD.IN,
#ifdef PORTE
  &PORTE.IN,
#ifdef PORTF
  &PORTF.IN,
#ifdef PORTR
  &PORTR.IN,
#endif
#endif
#endif
#endif
#endif
};


#else
const IO_PTR u8g_avr_ddr_P[] PROGMEM = {
#ifdef DDRA
  &DDRA,
#else
  0,
#endif
  &DDRB,
#ifdef DDRC
  &DDRC,
#ifdef DDRD
  &DDRD,
#ifdef DDRE
  &DDRE,
#ifdef DDRF
  &DDRF,
#ifdef DDRG
  &DDRG,
#ifdef DDRH
  &DDRH,
#endif
#endif
#endif
#endif
#endif
#endif
};


const IO_PTR u8g_avr_port_P[] PROGMEM = {
#ifdef PORTA
  &PORTA,
#else
  0,
#endif
  &PORTB,
#ifdef PORTC
  &PORTC,
#ifdef PORTD
  &PORTD,
#ifdef PORTE
  &PORTE,
#ifdef PORTF
  &PORTF,
#ifdef PORTG
  &PORTG,
#ifdef PORTH
  &PORTH,
#endif
#endif
#endif
#endif
#endif
#endif
};

const IO_PTR u8g_avr_pin_P[] PROGMEM = {
#ifdef PINA
  &PINA,
#else
  0,
#endif
  &PINB,
#ifdef PINC
  &PINC,
#ifdef PIND
  &PIND,
#ifdef PINE
  &PINE,
#ifdef PINF
  &PINF,
#ifdef PING
  &PING,
#ifdef PINH
  &PINH,
#endif
#endif
#endif
#endif
#endif
#endif
};
#endif

static volatile uint8_t *u8g_get_avr_io_ptr(const IO_PTR *base, uint8_t offset)
{
  volatile uint8_t * tmp;
  base += offset;
  memcpy_P(&tmp, base, sizeof(volatile uint8_t * PROGMEM));
  return tmp; 
}

/* set direction to output of the specified pin (internal pin number) */
void u8g_SetPinOutput(uint8_t internal_pin_number)
{
  *u8g_get_avr_io_ptr(u8g_avr_ddr_P, internal_pin_number>>3) |= _BV(internal_pin_number&7);
}

void u8g_SetPinInput(uint8_t internal_pin_number)
{
  *u8g_get_avr_io_ptr(u8g_avr_ddr_P, internal_pin_number>>3) &= ~_BV(internal_pin_number&7);
}

void u8g_SetPinLevel(uint8_t internal_pin_number, uint8_t level)
{
  volatile uint8_t * tmp = u8g_get_avr_io_ptr(u8g_avr_port_P, internal_pin_number>>3);
  
  if ( level == 0 )
  {
    U8G_ATOMIC_AND(tmp, ~_BV(internal_pin_number&7));
   // *tmp &= ~_BV(internal_pin_number&7);
  }
  else
  {
    U8G_ATOMIC_OR(tmp, _BV(internal_pin_number&7));
    //*tmp |= _BV(internal_pin_number&7);
  }
  
}

uint8_t u8g_GetPinLevel(uint8_t internal_pin_number)
{
  volatile uint8_t * tmp = u8g_get_avr_io_ptr(u8g_avr_pin_P, internal_pin_number>>3);
  if ( ((*tmp) & _BV(internal_pin_number&7))  != 0 )
    return 1;
  return 0;
}

#elif defined (__MSP430__)
#include <msp430.h>

typedef volatile uint8_t * IO_PTR;

// MSP430 F5XXX / F6XXX series. 
const IO_PTR u8g_msp_ddr_P[] PROGMEM = {
	&P1DIR
	,&P2DIR
	,&P3DIR
	,&P4DIR
	,&P5DIR
	,&P6DIR
	,&P7DIR
	,&P8DIR
#if defined (__MSP430_HAS_PORT9_R__)
	,&P9DIR
#if defined (__MSP430_HAS_PORT10_R__)
	,&P10DIR
#endif
#endif
};

const IO_PTR u8g_msp_port_P[] PROGMEM = {
	&P1OUT
	,&P2OUT
	,&P3OUT
	,&P4OUT
	,&P5OUT
	,&P6OUT
	,&P7OUT
	,&P8OUT
#if defined (__MSP430_HAS_PORT9_R__)
	,&P9OUT
#if defined (__MSP430_HAS_PORT10_R__)
	,&P10OUT
#endif
#endif
};

const IO_PTR u8g_msp_pin_P[] PROGMEM = {
	&P1IN
	,&P2IN
	,&P3IN
	,&P4IN
	,&P5IN
	,&P6IN
	,&P7IN
	,&P8IN
#if defined (__MSP430_HAS_PORT9_R__)
	,&P9IN
#if defined (__MSP430_HAS_PORT10_R__)
	,&P10IN
#endif
#endif
};

uint8_t u8g_Pin(uint8_t port, uint8_t bitpos)
{
	port <<= 3;
	port += bitpos;
	return port;
}

void u8g_SetPinOutput(uint8_t internal_pin_number)
{
	uint8_t port = (internal_pin_number >> 3)-1;
	uint8_t output = 1 << (internal_pin_number & 0x07);
	*u8g_msp_ddr_P[port] |= output;
}

void u8g_SetPinInput(uint8_t internal_pin_number)
{
	uint8_t port = (internal_pin_number >> 3)-1;
	*u8g_msp_ddr_P[port] &= ~(1 << (internal_pin_number & 0x07));
}

void u8g_SetPinLevel(uint8_t internal_pin_number, uint8_t level)
{
	uint8_t port = (internal_pin_number >> 3)-1;
	if (level == 0)
	{
		*u8g_msp_port_P[port] &= ~(1 << (internal_pin_number & 0x07));
	}
	else
	{
		*u8g_msp_port_P[port]|= (1 << (internal_pin_number & 0x07));
	}
}

uint8_t u8g_GetPinLevel(uint8_t internal_pin_number)
{
	uint8_t port = (internal_pin_number >> 3)-1;
	uint8_t tmp = *u8g_msp_pin_P[port];
	if (tmp & (1 << (internal_pin_number & 0x07)))
	{
		return 1;
	}
	return 0;
}

#elif defined(U8G_RASPBERRY_PI)

#include <wiringPi.h>
//#include "/usr/local/include/wiringPi.h"

void u8g_SetPinOutput(uint8_t internal_pin_number) {
   pinMode(internal_pin_number, OUTPUT);
}

void u8g_SetPinInput(uint8_t internal_pin_number) {
   pinMode(internal_pin_number, INPUT);
}

void u8g_SetPinLevel(uint8_t internal_pin_number, uint8_t level) {
   digitalWrite(internal_pin_number, level);
}

uint8_t u8g_GetPinLevel(uint8_t internal_pin_number) {
   return digitalRead(internal_pin_number);
}


#else

/* convert "port" and "bitpos" to internal pin number */
uint8_t u8g_Pin(uint8_t port, uint8_t bitpos)
{
  port <<= 3;
  port += bitpos;
  return port;
}

void u8g_SetPinOutput(uint8_t internal_pin_number)
{
}

void u8g_SetPinInput(uint8_t internal_pin_number)
{
}

void u8g_SetPinLevel(uint8_t internal_pin_number, uint8_t level)
{
}

uint8_t u8g_GetPinLevel(uint8_t internal_pin_number)
{
  return 0;
}

#endif


#if defined(U8G_WITH_PINLIST)

void u8g_SetPIOutput(u8g_t *u8g, uint8_t pi)
{
  uint8_t pin;
  pin = u8g->pin_list[pi];
  if ( pin != U8G_PIN_NONE )
    u8g_SetPinOutput(pin);
}

void u8g_SetPILevel(u8g_t *u8g, uint8_t pi, uint8_t level)
{
  uint8_t pin;
  pin = u8g->pin_list[pi];
  if ( pin != U8G_PIN_NONE )
    u8g_SetPinLevel(pin, level);
}

#else  /* defined(U8G_WITH_PINLIST) */
void u8g_SetPIOutput(u8g_t *u8g, uint8_t pi)
{
}

void u8g_SetPILevel(u8g_t *u8g, uint8_t pi, uint8_t level)
{
}

#endif /* defined(U8G_WITH_PINLIST) */
