/*

  u8g_delay.c

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


  void u8g_Delay(uint16_t val)		Delay by "val" milliseconds
  void u8g_MicroDelay(void)		Delay be one microsecond
  void u8g_10MicroDelay(void)	Delay by 10 microseconds

  
*/


#include "u8g.h"

/*==== Part 1: Derive suitable delay procedure ====*/

#if defined(ARDUINO)

#  if ARDUINO < 100 
#    include <WProgram.h> 
#  else 
#    include <Arduino.h> 
#  endif

#  if defined(__AVR__)
#    define USE_AVR_DELAY
#  elif defined(__PIC32MX)
#    define USE_PIC32_DELAY
#  elif defined(__arm__)		/* Arduino Due & Teensy */
#    define USE_ARDUINO_DELAY
#  else
#    define USE_ARDUINO_DELAY
#  endif
#elif defined(__AVR__)
#  define USE_AVR_DELAY
#elif defined(__18CXX)
#  define USE_PIC18_DELAY
#elif defined(__arm__)
/* do not define anything, all procedures are expected to be defined outside u8glib */

/*
void u8g_Delay(uint16_t val);
void u8g_MicroDelay(void);
void u8g_10MicroDelay(void);
*/

#else
#  define USE_DUMMY_DELAY
#endif



/*==== Part 2: Definition of the delay procedures ====*/

/*== AVR Delay ==*/

#if defined(USE_AVR_DELAY)
#include <avr/interrupt.h>
#include <avr/io.h>
#include <util/delay.h>

/*
  Delay by the provided number of milliseconds.
  Thus, a 16 bit value will allow a delay of 0..65 seconds
  Makes use of the _delay_loop_2
  
  _delay_loop_2 will do a delay of n * 4 prozessor cycles.
  with f = F_CPU cycles per second,
  n = f / (1000 * 4 )
  with f = 16000000 the result is 4000
  with f = 1000000 the result is 250
  
  the millisec loop, gcc requires the following overhead:
  - movev 1
  - subwi 2x2
  - bne i 2
  ==> 7 cycles
  ==> must be devided by 4, rounded up 7/4 = 2
*/
void u8g_Delay(uint16_t val)
{
  /* old version did a call to the arduino lib: delay(val); */
  while( val != 0 )
  {
    _delay_loop_2( (F_CPU / 4000 ) -2);
    val--;
  }
}

/* delay by one micro second */
void u8g_MicroDelay(void)
{
#if (F_CPU / 4000000 ) > 0 
  _delay_loop_2( (F_CPU / 4000000 ) );
#endif
}

/* delay by 10 micro seconds */
void u8g_10MicroDelay(void)
{
#if (F_CPU / 400000 ) > 0 
  _delay_loop_2( (F_CPU / 400000 ) );
#endif
}

#endif 


/*== Delay for PIC18 (not tested) ==*/

#if defined(USE_PIC18_DELAY)
#include <delays.h>
#define GetSystemClock()		(64000000ul)      // Hz
#define GetInstructionClock()	(GetSystemClock()/4)

void u8g_Delay(uint16_t val)
{/*
	unsigned int _iTemp = (val);
	while(_iTemp--)		
		Delay1KTCYx((GetInstructionClock()+999999)/1000000);
		*/
}
void u8g_MicroDelay(void)
{
  /* not implemented */
}
void u8g_10MicroDelay(void)
{
  /* not implemented */
}
#endif


/*== Arduino Delay ==*/
#if defined(USE_ARDUINO_DELAY)
void u8g_Delay(uint16_t val)
{
#if defined(__arm__)
	delayMicroseconds((uint32_t)val*(uint32_t)1000);
#else
	delay(val);
#endif
}
void u8g_MicroDelay(void)
{
	delayMicroseconds(1);
}
void u8g_10MicroDelay(void)
{
	delayMicroseconds(10);
}
#endif

#if defined(USE_PIC32_DELAY)
/* 
  Assume chipkit here with F_CPU correctly defined
  The problem was, that u8g_Delay() is called within the constructor.
  It seems that the chipkit is not fully setup at this time, so a
  call to delay() will not work. So here is my own implementation.

*/
#define CPU_COUNTS_PER_SECOND (F_CPU/2UL)
#define TICKS_PER_MILLISECOND  (CPU_COUNTS_PER_SECOND/1000UL)
#include "plib.h"
void u8g_Delay(uint16_t val)
{
	uint32_t d;
	uint32_t s;
	d = val;
	d *= TICKS_PER_MILLISECOND;
	s = ReadCoreTimer();
	while ( (uint32_t)(ReadCoreTimer() - s) < d )
		;
} 

void u8g_MicroDelay(void)
{
	uint32_t d;
	uint32_t s;
	d = TICKS_PER_MILLISECOND/1000;
	s = ReadCoreTimer();
	while ( (uint32_t)(ReadCoreTimer() - s) < d )
		;
} 

void u8g_10MicroDelay(void)
{
	uint32_t d;
	uint32_t s;
	d = TICKS_PER_MILLISECOND/100;
	s = ReadCoreTimer();
	while ( (uint32_t)(ReadCoreTimer() - s) < d )
		;
} 

#endif

/*== Any other systems: Dummy Delay ==*/
#if defined(USE_DUMMY_DELAY)
void u8g_Delay(uint16_t val)
{
	/* do not know how to delay... */
}
void u8g_MicroDelay(void)
{
}
void u8g_10MicroDelay(void)
{
}
#endif
