/**********************************************************/
/* Serial Bootloader for Atmel megaAVR Controllers        */
/*                                                        */
/* tested with ATmega644 and ATmega644P                   */
/* should work with other mega's, see code for details    */
/*                                                        */
/* ATmegaBOOT.c                                           */
/*                                                        */
/* 20090131: Added 324P support from Alex Leone           */
/*           Marius Kintel                                */
/* 20080915: applied ADABoot mods for Sanguino 644P       */
/*           Brian Riley                                  */
/* 20080711: hacked for Sanguino by Zach Smith            */
/*           and Justin Day                               */
/* 20070626: hacked for Arduino Diecimila (which auto-    */
/*           resets when a USB connection is made to it)  */
/*           by D. Mellis                                 */
/* 20060802: hacked for Arduino by D. Cuartielles         */
/*           based on a previous hack by D. Mellis        */
/*           and D. Cuartielles                           */
/*                                                        */
/* Monitor and debug functions were added to the original */
/* code by Dr. Erik Lins, chip45.com. (See below)         */
/*                                                        */
/* Thanks to Karl Pitrich for fixing a bootloader pin     */
/* problem and more informative LED blinking!             */
/*                                                        */
/* For the latest version see:                            */
/* http://www.chip45.com/                                 */
/*                                                        */
/* ------------------------------------------------------ */
/*                                                        */
/* based on stk500boot.c                                  */
/* Copyright (c) 2003, Jason P. Kyle                      */
/* All rights reserved.                                   */
/* see avr1.org for original file and information         */
/*                                                        */
/* This program is free software; you can redistribute it */
/* and/or modify it under the terms of the GNU General    */
/* Public License as published by the Free Software       */
/* Foundation; either version 2 of the License, or        */
/* (at your option) any later version.                    */
/*                                                        */
/* This program is distributed in the hope that it will   */
/* be useful, but WITHOUT ANY WARRANTY; without even the  */
/* implied warranty of MERCHANTABILITY or FITNESS FOR A   */
/* PARTICULAR PURPOSE.  See the GNU General Public        */
/* License for more details.                              */
/*                                                        */
/* You should have received a copy of the GNU General     */
/* Public License along with this program; if not, write  */
/* to the Free Software Foundation, Inc.,                 */
/* 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA */
/*                                                        */
/* Licence can be viewed at                               */
/* http://www.fsf.org/licenses/gpl.txt                    */
/*                                                        */
/* Target = Atmel AVR m128,m64,m32,m16,m8,m162,m163,m169, */
/* m8515,m8535. ATmega161 has a very small boot block so  */
/* isn't supported.                                       */
/*                                                        */
/* Tested with m168                                       */
/**********************************************************/

/* $Id$ */


/* some includes */
#include <inttypes.h>
#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/interrupt.h>
#include <avr/wdt.h>
#include <avr/boot.h>
#include <util/delay.h>

#ifdef ADABOOT
    #define NUM_LED_FLASHES 3
    #define ADABOOT_VER	1
#endif


/* 20070707: hacked by David A. Mellis - after this many errors give up and launch application */
#define MAX_ERROR_COUNT 5

/* set the UART baud rate */
/* 20080711: hack by Zach Hoeken */
#define BAUD_RATE   38400

/* SW_MAJOR and MINOR needs to be updated from time to time to avoid warning message from AVR Studio */
/* never allow AVR Studio to do an update !!!! */
#define HW_VER	 0x02
#define SW_MAJOR 0x01
#define SW_MINOR 0x10

/* onboard LED is used to indicate, that the bootloader was entered (3x flashing) */
/* if monitor functions are included, LED goes on after monitor was entered */
#define LED_DDR  DDRB
#define LED_PORT PORTB
#define LED_PIN  PINB
#define LED      PINB0

/* define various device id's */
/* manufacturer byte is always the same */
#define SIG1	0x1E	// Yep, Atmel is the only manufacturer of AVR micros.  Single source :(
#if defined(__AVR_ATmega1284P__)
#define SIG2	0x97
#define SIG3	0x05
#elif defined(__AVR_ATmega644P__)
#define SIG2	0x96
#define SIG3	0x0A
#elif defined(__AVR_ATmega644__)
#define SIG2	0x96
#define SIG3	0x09
#elif defined(__AVR_ATmega324P__)
#define SIG2	0x95
#define SIG3	0x08
#endif
#define PAGE_SIZE		0x080U   //128 words
#define PAGE_SIZE_BYTES	0x100U   //256 bytes

/* function prototypes */
void putch(char);
char getch(void);
void getNch(uint8_t);
void byte_response(uint8_t);
void nothing_response(void);
char gethex(void);
void puthex(char);
void flash_led(uint8_t);

/* some variables */
union address_union
{
    uint16_t word;
    uint8_t  byte[2];
} address;

union length_union
{
    uint16_t word;
    uint8_t  byte[2];
} length;

struct flags_struct
{
    unsigned eeprom : 1;
    unsigned rampz  : 1;
} flags;

uint8_t buff[256];

uint8_t error_count = 0;
uint8_t sreg;

void (*app_start)(void) = 0x0000;

/* main program starts here */
int main(void)
{
    uint8_t ch,ch2;
    uint16_t w;
	uint16_t i;
	
    asm volatile("nop\n\t");

#ifdef ADABOOT		// BBR/LF 10/8/2007 & 9/13/2008
    ch = MCUSR;
    MCUSR = 0;

    WDTCSR |= _BV(WDCE) | _BV(WDE);
    WDTCSR = 0;

    // Check if the WDT was used to reset, in which case we dont bootload and skip straight to the code. woot.
    if (! (ch &  _BV(EXTRF))) // if its a not an external reset...
      app_start();  // skip bootloader
#endif


	//initialize our serial port.
    UBRR0L = (uint8_t)(F_CPU/(BAUD_RATE*16L)-1);
    UBRR0H = (F_CPU/(BAUD_RATE*16L)-1) >> 8;
    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    UCSR0C = (1<<UCSZ00) | (1<<UCSZ01);

    /* Enable internal pull-up resistor on pin D0 (RX), in order
    to supress line noise that prevents the bootloader from
    timing out (DAM: 20070509) */
    DDRD &= ~_BV(PIND0);
    PORTD |= _BV(PIND0);

    /* set LED pin as output */
    LED_DDR |= _BV(LED);

	    /* flash onboard LED to signal entering of bootloader                   */
	    /* ADABOOT will do two series of flashes. first 4 - signifying ADABOOT  */
	    /* then a pause and another flash series signifying ADABOOT sub-version */


	flash_led(NUM_LED_FLASHES);

	#ifdef	ADABOOT
		flash_led(ADABOOT_VER);		// BBR 9/13/2008
	#endif 

    /* forever loop */
    for (;;)
	{
          putch('\r');
          _delay_ms(500);
        }
    /* end of forever loop */
}


char gethex(void)
{
    char ah,al;

    ah = getch();
	putch(ah);
    al = getch();
	putch(al);
    
	if(ah >= 'a')
		ah = ah - 'a' + 0x0a;
	else if(ah >= '0')
		ah -= '0';
    if(al >= 'a')
		al = al - 'a' + 0x0a;
	else if(al >= '0')
		al -= '0';

    return (ah << 4) + al;
}


void puthex(char ch)
{
    char ah,al;

    ah = (ch & 0xf0) >> 4;
	if(ah >= 0x0a)
		ah = ah - 0x0a + 'a';
	else
		ah += '0';

    al = (ch & 0x0f);
	if(al >= 0x0a)
		al = al - 0x0a + 'a';
	else
		al += '0';

    putch(ah);
    putch(al);
}


void putch(char ch)
{
    while (!(UCSR0A & _BV(UDRE0)));
    UDR0 = ch;
}




char getch(void)
{
    uint32_t count = 0;

#ifdef ADABOOT
	LED_PORT &= ~_BV(LED);          // toggle LED to show activity - BBR/LF 10/3/2007 & 9/13/2008
#endif

    while(!(UCSR0A & _BV(RXC0)))
	{
    	/* 20060803 DojoCorp:: Addon coming from the previous Bootloader*/               
    	/* HACKME:: here is a good place to count times*/
    	count++;
    	if (count > MAX_TIME_COUNT)
    		app_start();
     }

#ifdef ADABOOT
	LED_PORT |= _BV(LED);          // toggle LED to show activity - BBR/LF 10/3/2007 & 9/13/2008
#endif

    return UDR0;
}


void getNch(uint8_t count)
{
    uint8_t i;
    for(i=0;i<count;i++)
	{
		while(!(UCSR0A & _BV(RXC0)));
		UDR0;
    }
}


void byte_response(uint8_t val)
{
    if (getch() == ' ')
	{
		putch(0x14);
		putch(val);
		putch(0x10);
    }
	else
	{
		if (++error_count == MAX_ERROR_COUNT)
		    app_start();
    }
}


void nothing_response(void)
{
    if (getch() == ' ')
	{
		putch(0x14);
		putch(0x10);
    }
	else
	{
		if (++error_count == MAX_ERROR_COUNT)
		    app_start();
    }
}

#ifdef ADABOOT

void flash_led(uint8_t count)
{
    /* flash onboard LED count times to signal entering of bootloader */
	/* l needs to be volatile or the delay loops below might get      */
	/* optimized away if compiling with optimizations (DAM).          */
	
    volatile uint32_t l;

    if (count == 0) {
      count = ADABOOT;
    }
    

	int8_t i;
    for (i = 0; i < count; ++i) {
		LED_PORT |= _BV(LED);					// LED on
		for(l = 0; l < (F_CPU / 1000); ++l);		// delay NGvalue was 1000 for both loops - BBR
		LED_PORT &= ~_BV(LED);					// LED off
		for(l = 0; l < (F_CPU / 250); ++l);		// delay asymmteric for ADA BOOT BBR 
	}

	for(l = 0; l < (F_CPU / 100); ++l);		    // pause ADA BOOT BBR 
		
}

#else

void flash_led(uint8_t count)
{
    /* flash onboard LED three times to signal entering of bootloader */
	/* l needs to be volatile or the delay loops below might get
	optimized away if compiling with optimizations (DAM). */
    volatile uint32_t l;

    if (count == 0) {
      count = 3;
    }
    
	int8_t i;
    for (i = 0; i < count; ++i) {
		LED_PORT |= _BV(LED);
		for(l = 0; l < (F_CPU / 1000); ++l);
		LED_PORT &= ~_BV(LED);
		for(l = 0; l < (F_CPU / 1000); ++l); 
	}
		
}


#endif


/* end of file ATmegaBOOT.c */
