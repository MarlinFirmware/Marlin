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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */
#if defined(ARDUINO_ARCH_STM32) && !defined(STM32GENERIC)

#include "../../inc/MarlinConfig.h"

// ------------------------
// Local defines
// ------------------------

#define NUM_HARDWARE_TIMERS 2

// Default timer priorities. Override by specifying alternate priorities in the board pins file.
// The TONE timer is not present here, as it currently cannot be set programmatically. It is set
// by defining TIM_IRQ_PRIO in the variant.h or platformio.ini file, which adjusts the default
// priority for STM32 HardwareTimer objects.
#define SWSERIAL_TIMER_IRQ_PRIO_DEFAULT  1 // Requires tight bit timing to communicate reliably with TMC drivers
#define SERVO_TIMER_IRQ_PRIO_DEFAULT     1 // Requires tight PWM timing to control a BLTouch reliably
#define STEP_TIMER_IRQ_PRIO_DEFAULT      2
#define TEMP_TIMER_IRQ_PRIO_DEFAULT     14 // Low priority avoids interference with other hardware and timers

#ifndef STEP_TIMER_IRQ_PRIO
  #define STEP_TIMER_IRQ_PRIO STEP_TIMER_IRQ_PRIO_DEFAULT
#endif
#ifndef TEMP_TIMER_IRQ_PRIO
  #define TEMP_TIMER_IRQ_PRIO TEMP_TIMER_IRQ_PRIO_DEFAULT
#endif
#if HAS_TMC_SW_SERIAL
  #include <SoftwareSerial.h>
  #ifndef SWSERIAL_TIMER_IRQ_PRIO
    #define SWSERIAL_TIMER_IRQ_PRIO SWSERIAL_TIMER_IRQ_PRIO_DEFAULT
  #endif
#endif
#if HAS_SERVOS
  #include "Servo.h"
  #ifndef SERVO_TIMER_IRQ_PRIO
    #define SERVO_TIMER_IRQ_PRIO SERVO_TIMER_IRQ_PRIO_DEFAULT
  #endif
#endif
#if ENABLED(SPEAKER)
  // Ensure the default timer priority is somewhere between the STEP and TEMP priorities.
  // The STM32 framework defaults to interrupt 14 for all timers. This should be increased so that
  // timing-sensitive operations such as speaker output are not impacted by the long-running
  // temperature ISR. This must be defined in the platformio.ini file or the board's variant.h,
  // so that it will be consumed by framework code.
  #if !(TIM_IRQ_PRIO > STEP_TIMER_IRQ_PRIO && TIM_IRQ_PRIO < TEMP_TIMER_IRQ_PRIO)
    #error "Default timer interrupt priority is unspecified or set to a value which may degrade performance."
  #endif
#endif

#ifdef STM32F0xx
  #define MCU_TIMER_RATE (F_CPU)      // Frequency of timer peripherals
  #define MCU_STEP_TIMER 16
  #define MCU_TEMP_TIMER 17
#elif defined(STM32F1xx)
  #define MCU_TIMER_RATE (F_CPU)
  #define MCU_STEP_TIMER  4
  #define MCU_TEMP_TIMER  2
#elif defined(STM32F401xC) || defined(STM32F401xE)
  #define MCU_TIMER_RATE (F_CPU / 2)
  #define MCU_STEP_TIMER  9
  #define MCU_TEMP_TIMER 10
#elif defined(STM32F4xx) || defined(STM32F7xx)
  #define MCU_TIMER_RATE (F_CPU / 2)
  #define MCU_STEP_TIMER  6           // STM32F401 has no TIM6, TIM7, or TIM8
  #define MCU_TEMP_TIMER 14           // TIM7 is consumed by Software Serial if used.
#endif

#ifndef HAL_TIMER_RATE
  #define HAL_TIMER_RATE MCU_TIMER_RATE
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

HardwareTimer *timer_instance[NUM_HARDWARE_TIMERS] = { nullptr };

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

        timer_instance[timer_num]->setPrescaleFactor(STEPPER_TIMER_PRESCALE); //the -1 is done internally
        timer_instance[timer_num]->setOverflow(_MIN(hal_timer_t(HAL_TIMER_TYPE_MAX), (HAL_TIMER_RATE) / (STEPPER_TIMER_PRESCALE) /* /frequency */), TICK_FORMAT);
        break;
      case TEMP_TIMER_NUM: // TEMP TIMER - any available 16bit timer
        timer_instance[timer_num] = new HardwareTimer(TEMP_TIMER_DEV);
        // The prescale factor is computed automatically for HERTZ_FORMAT
        timer_instance[timer_num]->setOverflow(frequency, HERTZ_FORMAT);
        break;
    }

    // Disable preload. Leaving it default-enabled can cause the timer to stop if it happens
    // to exit the ISR after the start time for the next interrupt has already passed.
    timer_instance[timer_num]->setPreloadEnable(false);

    HAL_timer_enable_interrupt(timer_num);

    // Start the timer.
    timer_instance[timer_num]->resume(); // First call to resume() MUST follow the attachInterrupt()

    // This is fixed in Arduino_Core_STM32 1.8.
    // These calls can be removed and replaced with
    // timer_instance[timer_num]->setInterruptPriority
    switch (timer_num) {
      case STEP_TIMER_NUM:
        timer_instance[timer_num]->setInterruptPriority(STEP_TIMER_IRQ_PRIO, 0);
        break;
      case TEMP_TIMER_NUM:
        timer_instance[timer_num]->setInterruptPriority(TEMP_TIMER_IRQ_PRIO, 0);
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

void SetTimerInterruptPriorities() {
  TERN_(HAS_TMC_SW_SERIAL, SoftwareSerial::setInterruptPriority(SWSERIAL_TIMER_IRQ_PRIO, 0));
  TERN_(HAS_SERVOS, libServo::setInterruptPriority(SERVO_TIMER_IRQ_PRIO, 0));
}

// This is a terrible hack to replicate the behavior used in the framework's SoftwareSerial.cpp
// to choose a serial timer. It will select TIM7 on most boards used by Marlin, but this is more
// resiliant to new MCUs which may not have a TIM7. Best practice is to explicitly specify
// TIMER_SERIAL to avoid relying on framework selections which may not be predictable.
#if !defined(TIMER_SERIAL)
  #if defined (TIM18_BASE)
    #define TIMER_SERIAL TIM18
  #elif defined (TIM7_BASE)
    #define TIMER_SERIAL TIM7
  #elif defined (TIM6_BASE)
    #define TIMER_SERIAL TIM6
  #elif defined (TIM22_BASE)
    #define TIMER_SERIAL TIM22
  #elif defined (TIM21_BASE)
    #define TIMER_SERIAL TIM21
  #elif defined (TIM17_BASE)
    #define TIMER_SERIAL TIM17
  #elif defined (TIM16_BASE)
    #define TIMER_SERIAL TIM16
  #elif defined (TIM15_BASE)
    #define TIMER_SERIAL TIM15
  #elif defined (TIM14_BASE)
    #define TIMER_SERIAL TIM14
  #elif defined (TIM13_BASE)
    #define TIMER_SERIAL TIM13
  #elif defined (TIM11_BASE)
    #define TIMER_SERIAL TIM11
  #elif defined (TIM10_BASE)
    #define TIMER_SERIAL TIM10
  #elif defined (TIM12_BASE)
    #define TIMER_SERIAL TIM12
  #elif defined (TIM19_BASE)
    #define TIMER_SERIAL TIM19
  #elif defined (TIM9_BASE)
    #define TIMER_SERIAL TIM9
  #elif defined (TIM5_BASE)
    #define TIMER_SERIAL TIM5
  #elif defined (TIM4_BASE)
    #define TIMER_SERIAL TIM4
  #elif defined (TIM3_BASE)
    #define TIMER_SERIAL TIM3
  #elif defined (TIM2_BASE)
    #define TIMER_SERIAL TIM2
  #elif defined (TIM20_BASE)
    #define TIMER_SERIAL TIM20
  #elif defined (TIM8_BASE)
    #define TIMER_SERIAL TIM8
  #elif defined (TIM1_BASE)
    #define TIMER_SERIAL TIM1
  #else
    #error No suitable timer found for SoftwareSerial, define TIMER_SERIAL in variant.h
  #endif
#endif

// Place all timers used into an array, then recursively check for duplicates during compilation.
// This does not currently account for timers used for PWM, such as for fans.
// Timers are actually pointers. Convert to integers to simplify constexpr logic.
static constexpr uintptr_t timers_in_use[] = {
  uintptr_t(TEMP_TIMER_DEV),  // Override in pins file
  uintptr_t(STEP_TIMER_DEV),  // Override in pins file
  #if HAS_TMC_SW_SERIAL
    uintptr_t(TIMER_SERIAL),  // Set in variant.h, or as a define in platformio.h if not present in variant.h
  #endif
  #if ENABLED(SPEAKER)
    uintptr_t(TIMER_TONE),    // Set in variant.h, or as a define in platformio.h if not present in variant.h
  #endif
  #if HAS_SERVOS
    uintptr_t(TIMER_SERVO),   // Set in variant.h, or as a define in platformio.h if not present in variant.h
  #endif
  };

static constexpr bool verify_no_duplicate_timers() {
  LOOP_L_N(i, COUNT(timers_in_use))
    LOOP_S_L_N(j, i + 1, COUNT(timers_in_use))
      if (timers_in_use[i] == timers_in_use[j]) return false;
  return true;
}

// If this assertion fails at compile time, review the timers_in_use array. If default_envs is
// defined properly in platformio.ini, VS Code can evaluate the array when hovering over it,
// making it easy to identify the conflicting timers.
static_assert(verify_no_duplicate_timers(), "One or more timer conflict detected");

#endif // ARDUINO_ARCH_STM32 && !STM32GENERIC
