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
// Description:          *** HAL for Arduino AVR ***
//
// **************************************************************************

#ifndef _HAL_H
#define _HAL_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

//#define analogInputToDigitalPin(IO) IO

#ifndef CRITICAL_SECTION_START
  #define CRITICAL_SECTION_START  unsigned char _sreg = SREG; cli();
  #define CRITICAL_SECTION_END    SREG = _sreg;
#endif //CRITICAL_SECTION_START


// On AVR this is in math.h?
//#define square(x) ((x)*(x))

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

//extern uint8_t MCUSR;

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

//void cli(void);

//void _delay_ms(int delay);

int freeMemory(void);


//void eeprom_write_byte(unsigned char *pos, unsigned char value);

//unsigned char eeprom_read_byte(unsigned char *pos);


// timers

#define HAL_STEP_TIMER_RATE 	F_CPU/8 

//void HAL_set_step_timer (uint16_t count);
#define HAL_set_step_timer(u16) OCR1A = (u16)

#define HAL_STEP_TIMER_ISR 	ISR(TIMER1_COMPA_vect)

#define HAL_clear_step_timer_irq

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#endif // _HAL_H

