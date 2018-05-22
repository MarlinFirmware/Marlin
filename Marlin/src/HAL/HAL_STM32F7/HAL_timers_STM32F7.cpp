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

#ifdef STM32F7

// --------------------------------------------------------------------------
// Includes
// --------------------------------------------------------------------------

#include "HAL.h"

#include "HAL_timers_STM32F7.h"

// --------------------------------------------------------------------------
// Externals
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Local defines
// --------------------------------------------------------------------------

#define NUM_HARDWARE_TIMERS 2

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

tTimerConfig timerConfig[NUM_HARDWARE_TIMERS];

// --------------------------------------------------------------------------
// Function prototypes
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Private functions
// --------------------------------------------------------------------------

// --------------------------------------------------------------------------
// Public functions
// --------------------------------------------------------------------------


bool timers_initialised[NUM_HARDWARE_TIMERS] = {false};

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {

  if (!timers_initialised[timer_num]) {
    switch (timer_num) {
      case STEP_TIMER_NUM:
      //STEPPER TIMER TIM5 //use a 32bit timer
      __HAL_RCC_TIM5_CLK_ENABLE();
      timerConfig[0].timerdef.Instance               = TIM5;
      timerConfig[0].timerdef.Init.Prescaler         = (STEPPER_TIMER_PRESCALE);
      timerConfig[0].timerdef.Init.CounterMode       = TIM_COUNTERMODE_UP;
      timerConfig[0].timerdef.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
      timerConfig[0].IRQ_Id = TIM5_IRQn;
      timerConfig[0].callback = (uint32_t)TC5_Handler;
      HAL_NVIC_SetPriority(timerConfig[0].IRQ_Id, 1, 0);
      pinMode(STEPPER_ENABLE_PIN,OUTPUT);
      digitalWrite(STEPPER_ENABLE_PIN,LOW);
      break;
    case TEMP_TIMER_NUM:
      //TEMP TIMER TIM7 // any available 16bit Timer (1 already used for PWM)
      __HAL_RCC_TIM7_CLK_ENABLE();
      timerConfig[1].timerdef.Instance               = TIM7;
      timerConfig[1].timerdef.Init.Prescaler         = (TEMP_TIMER_PRESCALE);
      timerConfig[1].timerdef.Init.CounterMode       = TIM_COUNTERMODE_UP;
      timerConfig[1].timerdef.Init.ClockDivision     = TIM_CLOCKDIVISION_DIV1;
      timerConfig[1].IRQ_Id = TIM7_IRQn;
      timerConfig[1].callback = (uint32_t)TC7_Handler;
      HAL_NVIC_SetPriority(timerConfig[1].IRQ_Id, 2, 0);
      break;
    }
    timers_initialised[timer_num] = true;
  }

  timerConfig[timer_num].timerdef.Init.Period = (((HAL_TIMER_RATE) / timerConfig[timer_num].timerdef.Init.Prescaler) / frequency) - 1;

  if (HAL_TIM_Base_Init(&timerConfig[timer_num].timerdef) == HAL_OK)
    HAL_TIM_Base_Start_IT(&timerConfig[timer_num].timerdef);
}

//forward the interrupt
extern "C" void TIM5_IRQHandler() {
  ((void(*)(void))timerConfig[0].callback)();
}
extern "C" void TIM7_IRQHandler() {
  ((void(*)(void))timerConfig[1].callback)();
}

void HAL_timer_set_compare(const uint8_t timer_num, const uint32_t compare) {
  __HAL_TIM_SetAutoreload(&timerConfig[timer_num].timerdef, compare);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  HAL_NVIC_EnableIRQ(timerConfig[timer_num].IRQ_Id);
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  HAL_NVIC_DisableIRQ(timerConfig[timer_num].IRQ_Id);

  // We NEED memory barriers to ensure Interrupts are actually disabled!
  // ( https://dzone.com/articles/nvic-disabling-interrupts-on-arm-cortex-m-and-the )
  __DSB();
  __ISB();
}

hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  return __HAL_TIM_GetAutoreload(&timerConfig[timer_num].timerdef);
}

uint32_t HAL_timer_get_count(const uint8_t timer_num) {
  return __HAL_TIM_GetCounter(&timerConfig[timer_num].timerdef);
}

void HAL_timer_restrain(const uint8_t timer_num, const uint16_t interval_ticks) {
  const hal_timer_t mincmp = HAL_timer_get_count(timer_num) + interval_ticks;
  if (HAL_timer_get_compare(timer_num) < mincmp) HAL_timer_set_compare(timer_num, mincmp);
}

void HAL_timer_isr_prologue(const uint8_t timer_num) {
  if (__HAL_TIM_GET_FLAG(&timerConfig[timer_num].timerdef, TIM_FLAG_UPDATE) == SET) {
    __HAL_TIM_CLEAR_FLAG(&timerConfig[timer_num].timerdef, TIM_FLAG_UPDATE);
  }
}

#endif // STM32F7
