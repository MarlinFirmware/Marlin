/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
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
 * HAL for Arduino Due and compatible (SAM3X8E)
 *
 * For ARDUINO_ARCH_SAM
 */

#ifndef _HAL_TIMERS_H
#define _HAL_TIMERS_H

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include <stdint.h>

// --------------------------------------------------------------------------
// Defines
// --------------------------------------------------------------------------

#define FORCE_INLINE __attribute__((always_inline)) inline

typedef uint32_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFF

#define STEP_TIMER_NUM 0  // index of timer to use for stepper
#define TEMP_TIMER_NUM 1  // index of timer to use for temperature

#define HAL_TIMER_RATE         ((SystemCoreClock) / 4)  // frequency of timers peripherals
#define STEPPER_TIMER_PRESCALE 1.0              // prescaler for setting stepper frequency
#define HAL_STEPPER_TIMER_RATE HAL_TIMER_RATE   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define HAL_TICKS_PER_US       ((HAL_STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per us
#define HAL_TEMP_TIMER_RATE    1000000
#define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define ENABLE_TEMPERATURE_INTERRUPT() HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)

#define HAL_ENABLE_ISRs() do { if (thermalManager.in_temp_isr) DISABLE_TEMPERATURE_INTERRUPT(); else ENABLE_TEMPERATURE_INTERRUPT(); ENABLE_STEPPER_DRIVER_INTERRUPT(); } while(0)

#define HAL_STEP_TIMER_ISR  extern "C" void TIMER0_IRQHandler(void)
#define HAL_TEMP_TIMER_ISR  extern "C" void TIMER1_IRQHandler(void)

// --------------------------------------------------------------------------
// Types
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public Variables
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------
void HAL_timer_init(void);
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);

static FORCE_INLINE void HAL_timer_set_count(const uint8_t timer_num, const hal_timer_t count) {
  switch (timer_num) {
    case 0:
      LPC_TIM0->MR0 = count;
      if (LPC_TIM0->TC > count)
        LPC_TIM0->TC = count - 5; // generate an immediate stepper ISR
      break;
    case 1:
      LPC_TIM1->MR0 = count;
      if (LPC_TIM1->TC > count)
        LPC_TIM1->TC = count - 5; // make sure we don't have one extra long period
      break;
  }
}

static FORCE_INLINE hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return LPC_TIM0->MR0;
    case 1: return LPC_TIM1->MR0;
  }
  return 0;
}

static FORCE_INLINE hal_timer_t HAL_timer_get_current_count(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return LPC_TIM0->TC;
    case 1: return LPC_TIM1->TC;
  }
  return 0;
}

void HAL_timer_enable_interrupt(const uint8_t timer_num);
void HAL_timer_disable_interrupt(const uint8_t timer_num);
void HAL_timer_isr_prologue(const uint8_t timer_num);

#endif // _HAL_TIMERS_DUE_H
