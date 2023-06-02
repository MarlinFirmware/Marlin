/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (c) 2011 Camiel Gubbels / Erik van der Zalm
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#pragma once

/**
 * HAL For LPC5528
 */
#include "../../inc/MarlinConfig.h"

#include <stdint.h>

#include "../../core/macros.h"

#define SBIT_CNTEN 0

// ------------------------
// Defines
// ------------------------

#define MF_TIMER_STEP       0  // Timer Index for Stepper
#define MF_TIMER_TEMP       1  // Timer Index for Temperature
#define MF_TIMER_PULSE      2  // MF_TIMER_STEP //
#define TMC_TIMER           4  //

#define _HAL_TIMER(T) _CAT(CTIMER, T)
#define _HAL_TIMER_IRQ(T) CTIMER##T##_IRQn
#define __HAL_TIMER_ISR(T) extern "C" void CTIMER##T##_IRQHandler()
#define _HAL_TIMER_ISR(T)  __HAL_TIMER_ISR(T)

typedef uint32_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFF

#define HAL_TIMER_RATE          160000000 //((F_CPU) / 1) // GetStepperTimerClkFreq()   // 150MHz

#ifndef STEP_TIMER_NUM
  #define STEP_TIMER_NUM        0  // Timer Index for Stepper
#endif
#ifndef PULSE_TIMER_NUM
  #define PULSE_TIMER_NUM       STEP_TIMER_NUM
#endif
#ifndef TEMP_TIMER_NUM
  #define TEMP_TIMER_NUM        1  // Timer Index for Temperature
#endif

#ifndef PWM_TIMER_NUM
  #define PWM_TIMER_NUM         2  // Timer Index for PWM
#endif

#define TEMP_TIMER_RATE        1000000
#define TEMP_TIMER_FREQUENCY   1000               // temperature interrupt frequency

extern uint32_t GetStepperTimerClkFreq(void);
#define STEPPER_TIMER_RATE     HAL_TIMER_RATE
#define STEPPER_TIMER_PRESCALE (GetStepperTimerClkFreq() / (STEPPER_TIMER_RATE))
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per µs

#define PULSE_TIMER_RATE STEPPER_TIMER_RATE             // frequency of pulse timer
#define PULSE_TIMER_PRESCALE STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(STEP_TIMER_NUM)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt(STEP_TIMER_NUM)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(STEP_TIMER_NUM)

#define ENABLE_TEMPERATURE_INTERRUPT() HAL_timer_enable_interrupt(TEMP_TIMER_NUM)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(TEMP_TIMER_NUM)

#ifndef HAL_STEP_TIMER_ISR
  #define HAL_STEP_TIMER_ISR() _HAL_TIMER_ISR(STEP_TIMER_NUM)
#endif
#ifndef HAL_TEMP_TIMER_ISR
  #define HAL_TEMP_TIMER_ISR() _HAL_TIMER_ISR(TEMP_TIMER_NUM)
#endif

// Timer references by index
#define STEP_TIMER_PTR _HAL_TIMER(STEP_TIMER_NUM)
#define TEMP_TIMER_PTR _HAL_TIMER(TEMP_TIMER_NUM)

// ------------------------
// Public functions
// ------------------------
void HAL_timer_init();
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);

FORCE_INLINE static void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  switch (timer_num) {
    case 0: STEP_TIMER_PTR->MSR[0] = compare; break; // Stepper Timer Match Register 0
    case 1: TEMP_TIMER_PTR->MSR[0] = compare; break; //    Temp Timer Match Register 0
  }
}

FORCE_INLINE static hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return STEP_TIMER_PTR->MSR[0]; // Stepper Timer Match Register 0
    case 1: return TEMP_TIMER_PTR->MSR[0]; //    Temp Timer Match Register 0
  }
  return 0;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return STEP_TIMER_PTR->TC; // Stepper Timer Count
    case 1: return TEMP_TIMER_PTR->TC; //    Temp Timer Count
  }
  return 0;
}

FORCE_INLINE static void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: NVIC_EnableIRQ(CTIMER0_IRQn); break; // Enable interrupt handler
    case 1: NVIC_EnableIRQ(CTIMER1_IRQn); break; // Enable interrupt handler
  }
}

FORCE_INLINE static void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: NVIC_DisableIRQ(CTIMER0_IRQn); break; // Disable interrupt handler
    case 1: NVIC_DisableIRQ(CTIMER1_IRQn); break; // Disable interrupt handler
  }

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();
}

FORCE_INLINE static bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: return NVIC_GetEnableIRQ(CTIMER0_IRQn); // Check if interrupt is enabled or not
    case 1: return NVIC_GetEnableIRQ(CTIMER1_IRQn); // Check if interrupt is enabled or not
  }
  return false;
}

FORCE_INLINE static void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch (timer_num) {
    case 0: SBI(STEP_TIMER_PTR->IR, SBIT_CNTEN); break;
    case 1: SBI(TEMP_TIMER_PTR->IR, SBIT_CNTEN); break;
  }
}

#define HAL_timer_isr_epilogue(TIMER_NUM)
