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
// ARDUINO_ARCH_AVR
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
#define STEP_TIMER_NUM OCR1A
#define TEMP_TIMER_NUM 0

#define HAL_TIMER_RATE 	F_CPU/8.0 

#define ENABLE_STEPPER_DRIVER_INTERRUPT()  TIMSK1 |= (1<<OCIE1A)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() TIMSK1 &= ~(1<<OCIE1A)

//void HAL_timer_start (uint8_t timer_num, uint32_t frequency);
#define HAL_timer_start (timer_num,frequency)

//void HAL_timer_set_count (uint8_t timer_num, uint16_t count);
#define HAL_timer_set_count(timer,count) timer = (count)

//void HAL_timer_isr_prologue (uint8_t timer_num);
#define HAL_timer_isr_prologue(timer_num)

#define HAL_STEP_TIMER_ISR 	ISR(TIMER1_COMPA_vect)
#define HAL_TEMP_TIMER_ISR  ISR(TIMER0_COMPB_vect)

// --------------------------------------------------------------------------
//
// --------------------------------------------------------------------------

#endif // _HAL_H

