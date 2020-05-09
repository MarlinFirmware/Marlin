/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include "timers.h"

#include "../../inc/MarlinConfig.h"

// ------------------------
// Local defines
// ------------------------

#define NUM_HARDWARE_TIMERS 2

#ifndef STEP_TIMER_IRQ_PRIO
  #define STEP_TIMER_IRQ_PRIO 2
#endif
#ifndef TEMP_TIMER_IRQ_PRIO
  #define TEMP_TIMER_IRQ_PRIO 14   // 14 = after hardware ISRs
#endif

#if HAS_TMC_SW_SERIAL
  #include <SoftwareSerial.h>
  #ifndef SWSERIAL_TIMER_IRQ_PRIO
    #define SWSERIAL_TIMER_IRQ_PRIO 1
  #endif
#endif

#ifdef STM32F0xx
  #define HAL_TIMER_RATE (F_CPU)      // Frequency of timer peripherals
  #define MCU_STEP_TIMER 16
  #define MCU_TEMP_TIMER 17
#elif defined(STM32F1xx)
  #define HAL_TIMER_RATE (F_CPU)
  #define MCU_STEP_TIMER  4
  #define MCU_TEMP_TIMER  2
#elif defined(STM32F401xC) || defined(STM32F401xE)
  #define HAL_TIMER_RATE (F_CPU / 2)
  #define MCU_STEP_TIMER  9
  #define MCU_TEMP_TIMER 10
#elif defined(STM32F4xx) || defined(STM32F7xx)
  #define HAL_TIMER_RATE (F_CPU / 2)
  #define MCU_STEP_TIMER  6           // STM32F401 has no TIM6, TIM7, or TIM8
  #define MCU_TEMP_TIMER 14           // TIM7 is consumed by Software Serial if used.
#endif

#ifndef STEP_TIMER
  #define STEP_TIMER MCU_STEP_TIMER
#endif
#ifndef TEMP_TIMER
  #define TEMP_TIMER MCU_TEMP_TIMER
#endif

#define __TIMER_DEV(X) TIM##X
#define _TIMER_DEV(X) __TIMER_DEV(X)
#define STEP_TIMER_DEV _TIMER_DEV(STEP_TIMER)
#define TEMP_TIMER_DEV _TIMER_DEV(TEMP_TIMER)

#define __TIMER_IRQ_NAME(X) TIM##X##_IRQn
#define _TIMER_IRQ_NAME(X) __TIMER_IRQ_NAME(X)
#define STEP_TIMER_IRQ_NAME _TIMER_IRQ_NAME(STEP_TIMER)
#define TEMP_TIMER_IRQ_NAME _TIMER_IRQ_NAME(TEMP_TIMER)

// ------------------------
// Private Variables
// ------------------------

HardwareTimer *timer_instance[NUM_HARDWARE_TIMERS] = { NULL };

// ------------------------
// Public functions
// ------------------------

// frequency is in Hertz
void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  if (!HAL_timer_initialized(timer_num)) {
    switch (timer_num) {
      case STEP_TIMER_NUM: // STEPPER TIMER - use a 32bit timer if possible
        timer_instance[timer_num] = new HardwareTimer(STEP_TIMER_DEV);
        /* Set the prescaler to the final desired value.
         * This will change the effective ISR callback frequency but when
         * HAL_timer_start(timer_num=0) is called in the core for the first time
         * the real frequency isn't important as long as, after boot, the ISR
         * gets called with the correct prescaler and count register. So here
         * we set the prescaler to the correct, final value and ignore the frequency
         * asked. We will call back the ISR in 1 second to start at full speed.
         *
         * The proper fix, however, would be a correct initialization OR a
         * HAL_timer_change(const uint8_t timer_num, const uint32_t frequency)
         * which changes the prescaler when an IRQ frequency change is needed
         * (for example when steppers are turned on)
         */

        // Arduino_Core_STM32 1.8 requires setMode to be called on a channel to start the timer.
        // This is fixed in the following PR, which has not yet been released.
        // https://github.com/stm32duino/Arduino_Core_STM32/pull/849
        timer_instance[timer_num]->setMode(1, TIMER_OUTPUT_COMPARE, NC);
        timer_instance[timer_num]->setPrescaleFactor(STEPPER_TIMER_PRESCALE); //the -1 is done internally
        timer_instance[timer_num]->setOverflow(_MIN(hal_timer_t(HAL_TIMER_TYPE_MAX), (HAL_TIMER_RATE) / (STEPPER_TIMER_PRESCALE) /* /frequency */), TICK_FORMAT);
        break;
      case TEMP_TIMER_NUM: // TEMP TIMER - any available 16bit timer
        timer_instance[timer_num] = new HardwareTimer(TEMP_TIMER_DEV);

        // Arduino_Core_STM32 1.8 requires setMode to be called on a channel to start the timer.
        // This is fixed in the following PR, which has not yet been released.
        // https://github.com/stm32duino/Arduino_Core_STM32/pull/849        
        timer_instance[timer_num]->setMode(1, TIMER_OUTPUT_COMPARE, NC);
        // The prescale factor is computed automatically for HERTZ_FORMAT
        timer_instance[timer_num]->setOverflow(frequency, HERTZ_FORMAT);
        break;
    }

    HAL_timer_enable_interrupt(timer_num);

    // Start the timer.
    timer_instance[timer_num]->resume(); // First call to resume() MUST follow the attachInterrupt()

    // This is fixed in Arduino_Core_STM32 1.8.
    // These calls can be removed and replaced with
    // timer_instance[timer_num]->setInterruptPriority
    switch (timer_num) {
      case STEP_TIMER_NUM:
        timer_instance[timer_num]->setInterruptPriority(STEP_TIMER_IRQ_PRIO, 0);

        // Arduino_Core_STM32 1.8 does not apply the interrupt priority until the
        // timer is completely re-initialized, which never happens in this code.
        // Explicitly set the interrupt priority until fixed in a future Arduino_Core_STM32.
        HAL_NVIC_SetPriority(STEP_TIMER_IRQ_NAME, STEP_TIMER_IRQ_PRIO, 0);
        break;
      case TEMP_TIMER_NUM:
        timer_instance[timer_num]->setInterruptPriority(TEMP_TIMER_IRQ_PRIO, 0);

        // Arduino_Core_STM32 1.8 does not apply the interrupt priority until the
        // timer is completely re-initialized, which never happens in this code.
        // Explicitly set the interrupt priority until fixed in a future Arduino_Core_STM32.
        HAL_NVIC_SetPriority(TEMP_TIMER_IRQ_NAME, TEMP_TIMER_IRQ_PRIO, 0);
        break;
    }
  }
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  if (HAL_timer_initialized(timer_num) && !timer_instance[timer_num]->hasInterrupt()) {
    switch (timer_num) {
      case STEP_TIMER_NUM:
      timer_instance[timer_num]->attachInterrupt(Step_Handler);
      break;
    case TEMP_TIMER_NUM:
      timer_instance[timer_num]->attachInterrupt(Temp_Handler);
      break;
    }
  }
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  if (HAL_timer_initialized(timer_num)) timer_instance[timer_num]->detachInterrupt();
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  return HAL_timer_initialized(timer_num) && timer_instance[timer_num]->hasInterrupt();
}

void SetSoftwareSerialTimerInterruptPriority() {
  TERN_(HAS_TMC_SW_SERIAL, SoftwareSerial::setInterruptPriority(SWSERIAL_TIMER_IRQ_PRIO, 0));
}

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
