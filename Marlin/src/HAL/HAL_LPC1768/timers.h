/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#pragma once

/**
 *
 * HAL For LPC1768
 */

#include <stdint.h>

#include "../../core/macros.h"

#define SBIT_TIMER0 1
#define SBIT_TIMER1 2

#define SBIT_CNTEN 0

#define SBIT_MR0I  0 // Timer 0 Interrupt when TC matches MR0
#define SBIT_MR0R  1 // Timer 0 Reset TC on Match
#define SBIT_MR0S  2 // Timer 0 Stop TC and PC on Match
#define SBIT_MR1I  3
#define SBIT_MR1R  4
#define SBIT_MR1S  5
#define SBIT_MR2I  6
#define SBIT_MR2R  7
#define SBIT_MR2S  8
#define SBIT_MR3I  9
#define SBIT_MR3R 10
#define SBIT_MR3S 11

// ------------------------
// Defines
// ------------------------

#define _HAL_TIMER(T) _CAT(LPC_TIM, T)
#define _HAL_TIMER_IRQ(T) TIMER##T##_IRQn
#define __HAL_TIMER_ISR(T) extern "C" void TIMER##T##_IRQHandler()
#define _HAL_TIMER_ISR(T)  __HAL_TIMER_ISR(T)

typedef uint32_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFF

#define HAL_TIMER_RATE         ((F_CPU) / 4)  // frequency of timers peripherals

#define STEP_TIMER_NUM 0  // Timer Index for Stepper
#define TEMP_TIMER_NUM 1  // Timer Index for Temperature
#define PULSE_TIMER_NUM STEP_TIMER_NUM
#define PWM_TIMER_NUM 3   // Timer Index for PWM

#define TEMP_TIMER_RATE        1000000
#define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency

#define STEPPER_TIMER_RATE     HAL_TIMER_RATE   // frequency of stepper timer (HAL_TIMER_RATE / STEPPER_TIMER_PRESCALE)
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per µs
#define STEPPER_TIMER_PRESCALE (CYCLES_PER_MICROSECOND / STEPPER_TIMER_TICKS_PER_US)

#define PULSE_TIMER_RATE       STEPPER_TIMER_RATE   // frequency of pulse timer
#define PULSE_TIMER_PRESCALE   STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT() HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)

#define HAL_STEP_TIMER_ISR() _HAL_TIMER_ISR(STEP_TIMER_NUM)
#define HAL_TEMP_TIMER_ISR() _HAL_TIMER_ISR(TEMP_TIMER_NUM)

// Timer references by index
#define STEP_TIMER _HAL_TIMER(STEP_TIMER_NUM)
#define TEMP_TIMER _HAL_TIMER(TEMP_TIMER_NUM)

// ------------------------
// Public functions
// ------------------------
void HAL_timer_init();
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);

FORCE_INLINE static void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  switch (timer_num) {
    case 0: STEP_TIMER->MR0 = compare; break; // Stepper Timer Match Register 0
    case 1: TEMP_TIMER->MR0 = compare; break; //    Temp Timer Match Register 0
  }
}

FORCE_INLINE static hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return STEP_TIMER->MR0; // Stepper Timer Match Register 0
    case 1: return TEMP_TIMER->MR0; //    Temp Timer Match Register 0
  }
  return 0;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return STEP_TIMER->TC; // Stepper Timer Count
    case 1: return TEMP_TIMER->TC; //    Temp Timer Count
  }
  return 0;
}

FORCE_INLINE static void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: NVIC_EnableIRQ(TIMER0_IRQn); break; // Enable interrupt handler
    case 1: NVIC_EnableIRQ(TIMER1_IRQn); break; // Enable interrupt handler
  }
}

FORCE_INLINE static void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: NVIC_DisableIRQ(TIMER0_IRQn); break; // Disable interrupt handler
    case 1: NVIC_DisableIRQ(TIMER1_IRQn); break; // Disable interrupt handler
  }

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();
}

// This function is missing from CMSIS
FORCE_INLINE static bool NVIC_GetEnableIRQ(IRQn_Type IRQn) {
  return (NVIC->ISER[((uint32_t)IRQn) >> 5] & (1 << ((uint32_t)IRQn) & 0x1F)) != 0;
}

FORCE_INLINE static bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return NVIC_GetEnableIRQ(TIMER0_IRQn); // Check if interrupt is enabled or not
    case 1: return NVIC_GetEnableIRQ(TIMER1_IRQn); // Check if interrupt is enabled or not
  }
  return false;
}

FORCE_INLINE static void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: SBI(STEP_TIMER->IR, SBIT_CNTEN); break;
    case 1: SBI(TEMP_TIMER->IR, SBIT_CNTEN); break;
  }
}

#define HAL_timer_isr_epilogue(TIMER_NUM)
