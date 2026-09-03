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

#include <stdint.h>

#include "../../core/macros.h"

#define SBIT_CNTEN 0

// ------------------------
// Defines
// ------------------------

#ifndef MF_TIMER_STEP
  #define MF_TIMER_STEP     0  // Timer Index for Stepper
#endif
#ifndef MF_TIMER_TEMP
  #define MF_TIMER_TEMP     1  // Timer Index for Temperature
#endif
#ifndef MF_TIMER_PULSE
  #define MF_TIMER_PULSE    2
#endif
#ifndef TMC_TIMER
  #define TMC_TIMER         4
#endif

#define _HAL_TIMER(T) _CAT(CTIMER, T)
#define _HAL_TIMER_IRQ(T) CTIMER##T##_IRQn
#define __HAL_TIMER_ISR(T) extern "C" void CTIMER##T##_IRQHandler()
#define _HAL_TIMER_ISR(T)  __HAL_TIMER_ISR(T)

typedef uint32_t hal_timer_t;
#define HAL_TIMER_TYPE_MAX 0xFFFFFFFF

/**
 * The framework's timer_init() attaches CTIMER0/1/2 to PLL0
 * (kPLL0_to_CTIMER0/1/2) and there is no CTIMER clock divider, so
 * CLOCK_GetCTimerClkFreq() returns CLOCK_GetPll0OutFreq(). The boot clock is
 * BOARD_BootClockPLL150M, which runs PLL0 - and therefore the core and the
 * CTimers - at 150 MHz, the part's maximum (data sheet 3.1 Table 2).
 */
#define HAL_TIMER_RATE          F_CPU

#ifndef MF_TIMER_STEP
  #define MF_TIMER_STEP         0  // Timer Index for Stepper
#endif
#ifndef MF_TIMER_PULSE
  #define MF_TIMER_PULSE        MF_TIMER_STEP
#endif
#ifndef MF_TIMER_TEMP
  #define MF_TIMER_TEMP         1  // Timer Index for Temperature
#endif
#ifndef MF_TIMER_PWM
  #define MF_TIMER_PWM          3  // Timer Index for PWM
#endif

#define TEMP_TIMER_RATE        1000000
#define TEMP_TIMER_FREQUENCY   1000 // temperature interrupt frequency

extern uint32_t GetStepperTimerClkFreq(void);
#define STEPPER_TIMER_RATE     HAL_TIMER_RATE
#define STEPPER_TIMER_PRESCALE 1  // The CTimer runs directly at STEPPER_TIMER_RATE
#define STEPPER_TIMER_TICKS_PER_US ((STEPPER_TIMER_RATE) / 1000000) // stepper timer ticks per µs

#define PULSE_TIMER_RATE       STEPPER_TIMER_RATE   // frequency of pulse timer
#define PULSE_TIMER_PRESCALE   STEPPER_TIMER_PRESCALE
#define PULSE_TIMER_TICKS_PER_US STEPPER_TIMER_TICKS_PER_US

#define ENABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_enable_interrupt(MF_TIMER_STEP)
#define DISABLE_STEPPER_DRIVER_INTERRUPT() HAL_timer_disable_interrupt(MF_TIMER_STEP)
#define STEPPER_ISR_ENABLED() HAL_timer_interrupt_enabled(MF_TIMER_STEP)

#define ENABLE_TEMPERATURE_INTERRUPT() HAL_timer_enable_interrupt(MF_TIMER_TEMP)
#define DISABLE_TEMPERATURE_INTERRUPT() HAL_timer_disable_interrupt(MF_TIMER_TEMP)

#ifndef HAL_STEP_TIMER_ISR
  #define HAL_STEP_TIMER_ISR() _HAL_TIMER_ISR(MF_TIMER_STEP)
#endif
#ifndef HAL_TEMP_TIMER_ISR
  #define HAL_TEMP_TIMER_ISR() _HAL_TIMER_ISR(MF_TIMER_TEMP)
#endif

// Timer references by index
#define STEP_TIMER_PTR _HAL_TIMER(MF_TIMER_STEP)
#define TEMP_TIMER_PTR _HAL_TIMER(MF_TIMER_TEMP)

// ------------------------
// Public functions
// ------------------------
void HAL_timer_init();
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency);

FORCE_INLINE static void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  switch (timer_num) {
    case MF_TIMER_STEP: STEP_TIMER_PTR->MSR[0] = compare; break; // Stepper Timer Match Register 0
    case MF_TIMER_TEMP: TEMP_TIMER_PTR->MSR[0] = compare; break; //    Temp Timer Match Register 0
  }
}

FORCE_INLINE static hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: return STEP_TIMER_PTR->MSR[0]; // Stepper Timer Match Register 0
    case MF_TIMER_TEMP: return TEMP_TIMER_PTR->MSR[0]; //    Temp Timer Match Register 0
  }
  return 0;
}

FORCE_INLINE static hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: return STEP_TIMER_PTR->TC; // Stepper Timer Count
    case MF_TIMER_TEMP: return TEMP_TIMER_PTR->TC; //    Temp Timer Count
  }
  return 0;
}

FORCE_INLINE static void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: NVIC_EnableIRQ(CTIMER0_IRQn); break; // Enable interrupt handler
    case MF_TIMER_TEMP: NVIC_EnableIRQ(CTIMER1_IRQn); break; // Enable interrupt handler
  }
}

FORCE_INLINE static void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: NVIC_DisableIRQ(CTIMER0_IRQn); break; // Disable interrupt handler
    case MF_TIMER_TEMP: NVIC_DisableIRQ(CTIMER1_IRQn); break; // Disable interrupt handler
  }

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();
}

FORCE_INLINE static bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: return NVIC_GetEnableIRQ(CTIMER0_IRQn); // Check if interrupt is enabled or not
    case MF_TIMER_TEMP: return NVIC_GetEnableIRQ(CTIMER1_IRQn); // Check if interrupt is enabled or not
  }
  return false;
}

FORCE_INLINE static void HAL_timer_isr_prologue(const uint8_t timer_num) {
  switch (timer_num) {
    case MF_TIMER_STEP: SBI(STEP_TIMER_PTR->IR, SBIT_CNTEN); break;
    case MF_TIMER_TEMP: SBI(TEMP_TIMER_PTR->IR, SBIT_CNTEN); break;
  }
}

#define HAL_timer_isr_epilogue(T) NOOP
