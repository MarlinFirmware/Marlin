/*
  wiring.c - Partial implementation of the Wiring API for the ATmega8.
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2005-2006 David A. Mellis

  Modified for Teensyduino by Paul Stoffregen, paul@pjrc.com
  http://www.pjrc.com/teensy/teensyduino.html

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA
*/

#include "wiring_private.h"
#include "pins_arduino.h"
#include "core_pins.h"


#define PULSEIN_CYCLES_PER_LOOP  21
#define PULSEIN_CYCLES_LATENCY   11

/* Measures the length (in microseconds) of a pulse on the pin; state is HIGH
 * or LOW, the type of pulse to measure.  Works on pulses from 2-3 microseconds
 * to 3 minutes in length, but must be called at least a few dozen microseconds
 * before the start of the pulse. */
unsigned long pulseIn(uint8_t pin, uint8_t state, unsigned long timeout)
{
	// cache the port and bit of the pin in order to speed up the
	// pulse width measuring loop and achieve finer resolution.  calling
	// digitalRead() instead yields much coarser resolution.
	uint8_t bit = digitalPinToBitMask(pin);
	volatile uint8_t *reg = portInputRegister(digitalPinToPort(pin));
	uint8_t stateMask = (state ? bit : 0);
	unsigned long width = 0; // keep initialization out of time critical area
	
	// convert the timeout from microseconds to a number of times through
	// the initial loop
	unsigned long numloops = 0;
	//unsigned long maxloops = microsecondsToClockCycles(timeout) / PULSEIN_CYCLES_PER_LOOP;
	unsigned long maxloops = timeout * clockCyclesPerMicrosecond() / PULSEIN_CYCLES_PER_LOOP;

	// wait for any previous pulse to end
	while ((*reg & bit) == stateMask)
		if (numloops++ == maxloops)
			return 0;
	
	// wait for the pulse to start
	while ((*reg & bit) != stateMask)
		if (numloops++ == maxloops)
			return 0;
	
	// wait for the pulse to stop
	while ((*reg & bit) == stateMask) {
		width++;
		if (numloops++ == maxloops)
			return 0;
	}

	// convert the reading to microseconds. The loop has been determined
	// to be PULSEIN_CYCLES_LATENCY clock cycles long and have about
	// PULSEIN_CYCLES_PER_LOOP clocks between the edge and the start of
	// the loop. There will be some error introduced by the interrupt
	// handlers.
	//return clockCyclesToMicroseconds(PULSEIN_CYCLES_PER_LOOP * width + PULSEIN_CYCLES_LATENCY); 
	return (width * PULSEIN_CYCLES_PER_LOOP + PULSEIN_CYCLES_LATENCY + (clockCyclesPerMicrosecond() / 2)) / clockCyclesPerMicrosecond();
}


