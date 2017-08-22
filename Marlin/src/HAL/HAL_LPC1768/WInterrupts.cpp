/*
  Copyright (c) 2011-2012 Arduino.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include "../../../macros.h"
#include "../HAL.h"
#include "arduino.h"
#include "pinmapping.h"
//#include "HAL_timers.h"
#include "fastio.h"

#define GNUM 31

typedef void (*interruptCB)(void);

static interruptCB callbacksP0[GNUM];
static interruptCB callbacksP2[GNUM];

	extern "C" void GpioEnableInt(uint32_t port, uint32_t pin, uint32_t mode);
	extern "C" void GpioDisableInt(uint32_t port, uint32_t pin);

//void deadloop(void) {}

/* Configure PIO interrupt sources */
static void __initialize() {
	int i;
	for (i=0; i<GNUM; i++) {
		callbacksP0[i] = 0;
		callbacksP2[i] = 0;
		}
NVIC_EnableIRQ(EINT3_IRQn);
}

void attachInterrupt(uint32_t pin, void (*callback)(void), uint32_t mode)
{
	static int enabled = 0;

	if(!INTERRUPT_PIN(pin)) return;

	if (!enabled) {
		__initialize();
		enabled = 1;
	}
	uint8_t myport = pin_map[pin].port;
	uint8_t mypin = pin_map[pin].pin;


	if (myport == 0 )
		callbacksP0[mypin] = callback;
	else
		callbacksP2[mypin] = callback;

	// Enable interrupt
		GpioEnableInt(myport,mypin,mode);
}

void detachInterrupt(uint32_t pin)
{
	if(!INTERRUPT_PIN(pin)) return;

  uint8_t myport = pin_map[pin].port;
  uint8_t mypin = pin_map[pin].pin;

	// Disable interrupt
	GpioDisableInt(myport,mypin);

//unset callback
	if (myport == 0 )
		callbacksP0[mypin] = 0;
	else //if (myport == 2 )
		callbacksP2[mypin] = 0;
	}


	extern "C" void GpioEnableInt(uint32_t port, uint32_t pin, uint32_t mode) {
	//pin here is the processor pin, not logical pin
	if (port==0) {
		LPC_GPIOINT->IO0IntClr = (1 << pin);
		if (mode ==RISING) {
			LPC_GPIOINT->IO0IntEnR |= (1<<pin);
  		LPC_GPIOINT->IO0IntEnF &= ~(1<<pin);
		}
		else if (mode==FALLING) {
			LPC_GPIOINT->IO0IntEnF |= (1<<pin);
  		LPC_GPIOINT->IO0IntEnR &= ~(1<<pin);
		}
		else if (mode==CHANGE) {
		LPC_GPIOINT->IO0IntEnR |= (1<<pin);
		LPC_GPIOINT->IO0IntEnF |= (1<<pin);
		}
	}
	else{
		LPC_GPIOINT->IO2IntClr = (1 << pin);
		if (mode ==RISING) {
			LPC_GPIOINT->IO2IntEnR |= (1<<pin);
			LPC_GPIOINT->IO2IntEnF &= ~(1<<pin);
		}
		else if (mode==FALLING) {
			LPC_GPIOINT->IO2IntEnF |= (1<<pin);
			LPC_GPIOINT->IO2IntEnR &= ~(1<<pin);
		}
		else if (mode==CHANGE) {
		LPC_GPIOINT->IO2IntEnR |= (1<<pin);
		LPC_GPIOINT->IO2IntEnF |= (1<<pin);
		}
	}
}
extern "C"  void GpioDisableInt(uint32_t port, uint32_t pin)
{
	if (port==0){
		LPC_GPIOINT->IO0IntEnR &= ~(1<<pin);
		LPC_GPIOINT->IO0IntEnF &= ~(1<<pin);
		LPC_GPIOINT->IO0IntClr = 1 << pin;
	}
	else {
		LPC_GPIOINT->IO2IntEnR &= ~(1<<pin);
		LPC_GPIOINT->IO2IntEnF &= ~(1<<pin);
		LPC_GPIOINT->IO2IntClr = 1 << pin;
	}
}

bool isPowerOf2(unsigned int n)

{

    return n == 1 || (n & (n-1)) == 0;

}

#if 0
extern "C" void EINT3_IRQHandler () {
	LPC_GPIOINT->IO0IntClr = LPC_GPIOINT->IO2IntClr = 0xFFFFFFFF;
	TOGGLE(13);
	//NVIC_ClearPendingIRQ(EINT3_IRQn);
}
#else
extern "C" void EINT3_IRQHandler(void)
{
	// Read in all current interrupt registers. We do this once as the
// GPIO interrupt registers are on the APB bus, and this is slow.
uint32_t rise0 = LPC_GPIOINT->IO0IntStatR;
uint32_t fall0 = LPC_GPIOINT->IO0IntStatF;
uint32_t rise2 = LPC_GPIOINT->IO2IntStatR;
uint32_t fall2 = LPC_GPIOINT->IO2IntStatF;
//Clear teh interrupts ASAP
LPC_GPIOINT->IO0IntClr = LPC_GPIOINT->IO2IntClr = 0xFFFFFFFF;
NVIC_ClearPendingIRQ(EINT3_IRQn);
uint8_t bitloc;
if (rise0 == 0)
	goto fall0;
	/* multiple pins changes happened.*/
	while(rise0 > 0) {      //Continue as long as there are interrupts pending
			bitloc = 31 - __CLZ(rise0); //CLZ returns number of leading zeros, 31 minus that is location of first pending interrupt
			if (callbacksP0[bitloc]!=0)
				callbacksP0[bitloc]();
			rise0 -= 1<<bitloc;
	}
fall0:
if (fall0==0)
	goto rise2;
/* if (isPowerOf2(fall0) && callbacksP0[31 - __CLZ(rise0)])
  callbacksP0[31 - __CLZ(rise0)](); */
	//LPC_GPIOINT->IO0IntClr = fall0;*/
else {
while(fall0 > 0) {
		bitloc = 31 - __CLZ(fall0);
		if (callbacksP0[bitloc]!=0)
			callbacksP0[bitloc]();
		fall0 -= 1<<bitloc;
  }
}
rise2:
if (rise2==0)
	goto fall2;
/*if ((rise2 & (rise2 - 1)) == 0) {
  callbacksP2[rise2]();
	//LPC_GPIOINT->IO2IntClr = rise2;
}*/
else {
while(rise2 > 0) {
		bitloc = 31 - __CLZ(rise2);
		if (callbacksP2[bitloc]!=0)
			callbacksP2[bitloc]();
		//LPC_GPIOINT->IO2IntClr = 1 << bitloc;
		rise2 -= 1<<bitloc;
  }
}
fall2:
if (fall2==0)
	goto end;
/*if ((fall2 & (fall2 - 1)) == 0) {
  callbacksP2[fall2]();
	//LPC_GPIOINT->IO2IntClr = fall2;
}*/
else {
while(fall2 > 0) {
		bitloc = 31 - __CLZ(fall2);
		if (callbacksP2[bitloc]!=0)
			callbacksP2[bitloc]();
		//LPC_GPIOINT->IO2IntClr = 1 << bitloc;
		fall2 -= 1<<bitloc;
}
end:
//NVIC_ClearPendingIRQ(EINT3_IRQn);
//LPC_GPIOINT->IO0IntClr = LPC_GPIOINT->IO2IntClr = 0xFFFFFFFF;
//NVIC_ClearPendingIRQ(EINT3_IRQn);
return; //silences warning
}
}
#endif
