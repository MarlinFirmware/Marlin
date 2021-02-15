/**
 * FYSETC
 *
 * Copyright (c) 2019 SoftwareSerialM [https://github.com/FYSETC/SoftwareSerialM]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/**
 * HAL for stm32duino.com based on Libmaple and compatible (STM32F1)
 */
#include "HAL_platform.h"

#if HAL_SS_PLATFORM == HAL_PLATFORM_STM32F1

#include "HAL_softserial_STM32F1.h"

#ifdef STM32_HIGH_DENSITY
// define default timer
	#ifndef SS_TIMER
	#define SS_TIMER 3
	#endif
	#ifndef SS_TIMER_CHANNEL
	#define SS_TIMER_CHANNEL 4
	#endif

  HardwareTimer ssTimer6(6), ssTimer7(7);
  HardwareTimer *SSTimer[8] =  { &Timer1,&Timer2,&Timer3,&Timer4,&Timer5,&ssTimer6,&ssTimer7,&Timer8 };
  #define ss_timer SSTimer[SS_TIMER-1]
#else
	// define default timer and channel
	#ifndef SS_TIMER
	#define SS_TIMER 3
	#endif
	#ifndef SS_TIMER_CHANNEL
	#define SS_TIMER_CHANNEL 4
	#endif

  HardwareTimer *SSTimer[4] =  { &Timer1,&Timer2,&Timer3,&Timer4 };
	#define ss_timer SSTimer[SS_TIMER-1]
#endif

void HAL_softSerial_init() {
  ss_timer->attachInterrupt(SS_TIMER_CHANNEL,SoftSerial_Handler); // attach corresponding handler routine    
}

//#define MAX_RELOAD ((1 << 16) - 1)
uint32_t HAL_softserial_setSpeed(uint32_t speed) {
  ss_timer->pause();
  ss_timer->setCount(0);
  if (speed != 0) {
    // TODO:it may need more accurate calculation.Especially when using higher baudrate.
    ss_timer->setPeriod(1000000ul/(speed*OVERSAMPLE));
    //uint32 period_cyc = microseconds * CYCLES_PER_MICROSECOND;
    //uint16 prescaler = (uint16)(period_cyc / MAX_RELOAD + 1);
    //uint16 overflow = (uint16)((period_cyc + (prescaler / 2)) / prescaler);
    //ss_timer->setPrescaleFactor(prescaler);
    //ss_timer->setOverflow(overflow);
    ss_timer->refresh(); // Refresh the timer    
    ss_timer->resume();  // Start the timer counting
  }      
  return speed;
}
#endif
