/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
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

#ifdef __STM32F1__

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "../HAL.h"

#include "HAL_timers_Stm32f1.h"

// --------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

#define NUM_HARDWARE_TIMERS 4

//#define PRESCALER 1
// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------


// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private Variables
// --------------------------------------------------------------------------
/* VGPV
const tTimerConfig TimerConfig [NUM_HARDWARE_TIMERS] = {
  { TC0, 0, TC0_IRQn, 0},  // 0 - [servo timer5]
  { TC0, 1, TC1_IRQn, 0},  // 1
  { TC0, 2, TC2_IRQn, 0},  // 2
  { TC1, 0, TC3_IRQn, 2},  // 3 - stepper
  { TC1, 1, TC4_IRQn, 15}, // 4 - temperature
  { TC1, 2, TC5_IRQn, 0},  // 5 - [servo timer3]
  { TC2, 0, TC6_IRQn, 0},  // 6
  { TC2, 1, TC7_IRQn, 0},  // 7
  { TC2, 2, TC8_IRQn, 0},  // 8
};
*/
// --------------------------------------------------------------------------
// Function prototypes
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------

/*
Timer_clock1: Prescaler 2 -> 42MHz
Timer_clock2: Prescaler 8 -> 10.5MHz
Timer_clock3: Prescaler 32 -> 2.625MHz
Timer_clock4: Prescaler 128 -> 656.25kHz
*/

/**
 * TODO: Calculate Timer prescale value, so we get the 32bit to adjust
 */

void HAL_timer_start(uint8_t timer_num, uint32_t frequency) {
  switch (timer_num) {
    case STEP_TIMER_NUM:
      StepperTimer.pause();
      StepperTimer.setCount(0);
      StepperTimer.setPrescaleFactor(STEPPER_TIMER_PRESCALE);
      StepperTimer.setOverflow(0xFFFF);
      StepperTimer.setCompare(STEP_TIMER_CHAN, uint32_t(HAL_STEPPER_TIMER_RATE) / frequency);
      StepperTimer.refresh();
      StepperTimer.resume();
      break;
    case TEMP_TIMER_NUM:
      TempTimer.pause();
      TempTimer.setCount(0);
      TempTimer.setPrescaleFactor(TEMP_TIMER_PRESCALE);
      TempTimer.setOverflow(0xFFFF);
      TempTimer.setCompare(TEMP_TIMER_CHAN, (F_CPU) / (TEMP_TIMER_PRESCALE) / frequency);
      TempTimer.refresh();
      TempTimer.resume();
      break;
  }
}

void HAL_timer_enable_interrupt(uint8_t timer_num) {
  switch (timer_num) {
    case STEP_TIMER_NUM:
      StepperTimer.attachInterrupt(STEP_TIMER_CHAN, stepTC_Handler);
      break;
    case TEMP_TIMER_NUM:
      TempTimer.attachInterrupt(STEP_TIMER_CHAN, tempTC_Handler);
      break;
    default:
      break;
  }
}

void HAL_timer_disable_interrupt(uint8_t timer_num) {
  switch (timer_num) {
    case STEP_TIMER_NUM:
      StepperTimer.detachInterrupt(STEP_TIMER_CHAN);
      break;
    case TEMP_TIMER_NUM:
      TempTimer.detachInterrupt(STEP_TIMER_CHAN);
      break;
    default:
      break;
  }
}

#endif // __STM32F1__
