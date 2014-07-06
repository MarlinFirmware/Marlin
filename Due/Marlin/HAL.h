/* -*- c++ -*- */

/*
 Contributors:
    Copyright (c) 2014 Bob Cousins bobcousins42@googlemail.com
*/
/* **************************************************************************
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
****************************************************************************/

// **************************************************************************
//
// Description:          *** HAL for Arduino Due ***
//
// ARDUINO_ARCH_SAM
// **************************************************************************

#ifndef _HAL_H
#define _HAL_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

#include "Arduino.h"

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

#define analogInputToDigitalPin(IO) IO

#define     CRITICAL_SECTION_START	do {} while (0);
#define     CRITICAL_SECTION_END    do {} while (0);

// On AVR this is in math.h?
#define square(x) ((x)*(x))

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

// reset reason set by bootloader
extern uint8_t MCUSR;

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

// Disable interrupts
void cli(void);

// Enable interrupts
void sei(void);

void _delay_ms(int delay);

int freeMemory(void);


void eeprom_write_byte(unsigned char *pos, unsigned char value);

unsigned char eeprom_read_byte(unsigned char *pos);


// timers
#define TICKS_PER_MICROSECOND   F_CPU/2/1000000
#define HAL_STEP_TIMER_RATE 	F_CPU/2

#define HAL_STEP_TIMER_ISR 	void TC3_Handler()

#define ENABLE_STEPPER_DRIVER_INTERRUPT()	TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
#define DISABLE_STEPPER_DRIVER_INTERRUPT()	TC1->TC_CHANNEL[0].TC_IDR = TC_IER_CPCS;


void HAL_startTimer(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t frequency);
void HAL_set_step_timer (uint32_t count);

#define HAL_clear_step_timer_irq TC_GetStatus(TC1, 0)


// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#endif // _HAL_H

