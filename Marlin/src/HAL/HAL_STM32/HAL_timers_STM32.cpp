/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "HAL.h"

#include "HAL_timers_STM32.h"

// ------------------------
// Local defines
// ------------------------

#define NUM_HARDWARE_TIMERS 2

//#define PRESCALER 1

// ------------------------
// Private Variables
// ------------------------

stm32_timer_t TimerHandle[NUM_HARDWARE_TIMERS];

// ------------------------
// Public functions
// ------------------------

bool timers_initialized[NUM_HARDWARE_TIMERS] = { false };

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {

  if (!timers_initialized[timer_num]) {
    uint32_t step_prescaler = STEPPER_TIMER_PRESCALE - 1,
                       temp_prescaler = TEMP_TIMER_PRESCALE - 1;
    switch (timer_num) {
      case STEP_TIMER_NUM:
        // STEPPER TIMER - use a 32bit timer if possible
        TimerHandle[timer_num].timer = STEP_TIMER_DEV;
        TimerHandle[timer_num].irqHandle = Step_Handler;
        TimerHandleInit(&TimerHandle[timer_num], (((HAL_TIMER_RATE) / step_prescaler) / frequency) - 1, step_prescaler);
        HAL_NVIC_SetPriority(STEP_TIMER_IRQ_NAME, STEP_TIMER_IRQ_PRIO, 0);
        break;

      case TEMP_TIMER_NUM:
        // TEMP TIMER - any available 16bit Timer
        TimerHandle[timer_num].timer = TEMP_TIMER_DEV;
        TimerHandle[timer_num].irqHandle = Temp_Handler;
        TimerHandleInit(&TimerHandle[timer_num], (((HAL_TIMER_RATE) / temp_prescaler) / frequency) - 1, temp_prescaler);
        HAL_NVIC_SetPriority(TEMP_TIMER_IRQ_NAME, TEMP_TIMER_IRQ_PRIO, 0);
        break;
    }
    timers_initialized[timer_num] = true;
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  const IRQn_Type IRQ_Id = IRQn_Type(getTimerIrq(TimerHandle[timer_num].timer));
  HAL_NVIC_EnableIRQ(IRQ_Id);
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  const IRQn_Type IRQ_Id = IRQn_Type(getTimerIrq(TimerHandle[timer_num].timer));
  HAL_NVIC_DisableIRQ(IRQ_Id);

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  const uint32_t IRQ_Id = getTimerIrq(TimerHandle[timer_num].timer);
  return NVIC->ISER[IRQ_Id >> 5] & _BV32(IRQ_Id & 0x1F);
}

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
