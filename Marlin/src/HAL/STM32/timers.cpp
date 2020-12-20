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

// This list is based on the framework code in SoftwareSerial.cpp, which attempts to select the
// timer least likely to be used on the system. This roughly correlates to how many external pins
// these connect to, and how uncommon they are across MCU lines. Use this as the fallback order
// if a match cannot be found the the MCU-specific list.
static constexpr uintptr_t default_preferred_timers[] = {
  #if defined (TIM18_BASE)
    uintptr_t(TIM18),
  #endif
  #if defined (TIM7_BASE)
    uintptr_t(TIM7),
  #endif
  #if defined (TIM6_BASE)
    uintptr_t(TIM6),
  #endif
  #if defined (TIM22_BASE)
    uintptr_t(TIM22),
  #endif
  #if defined (TIM21_BASE)
    uintptr_t(TIM21),
  #endif
  #if defined (TIM17_BASE)
    uintptr_t(TIM17),
  #endif
  #if defined (TIM16_BASE)
    uintptr_t(TIM16),
  #endif
  #if defined (TIM15_BASE)
    uintptr_t(TIM15),
  #endif
  #if defined (TIM14_BASE)
    uintptr_t(TIM14),
  #endif
  #if defined (TIM13_BASE)
    uintptr_t(TIM13),
  #endif
  #if defined (TIM11_BASE)
    uintptr_t(TIM11),
  #endif
  #if defined (TIM10_BASE)
    uintptr_t(TIM10),
  #endif
  #if defined (TIM12_BASE)
    uintptr_t(TIM12),
  #endif
  #if defined (TIM19_BASE)
    uintptr_t(TIM19),
  #endif
  #if defined (TIM9_BASE)
    uintptr_t(TIM9),
  #endif
  #if defined (TIM5_BASE)
    uintptr_t(TIM5),
  #endif
  #if defined (TIM4_BASE)
    uintptr_t(TIM4),
  #endif
  #if defined (TIM3_BASE)
    uintptr_t(TIM3),
  #endif
  #if defined (TIM2_BASE)
    uintptr_t(TIM2),
  #endif
  #if defined (TIM20_BASE)
    uintptr_t(TIM20),
  #endif
  #if defined (TIM8_BASE)
    uintptr_t(TIM8),
  #endif
  #if defined (TIM1_BASE)
    uintptr_t(TIM1),
  #endif
};

// The platform's SoftwareSerial.cpp will use the first timer from the list above.
#if HAS_TMC_SW_SERIAL && !defined(TIMER_SERIAL)
  constexpr auto TIMER_SERIAL = default_preferred_timers[0];
#endif

// mcu_preferred_timers allows a customized list of timers to be considered before the
// default_preferred_timers above.
#if defined(STM32F1xx)
  // Customized preferences for F1 chips
  // This defers to historic precedent, since these timers were most likely left without
  // usage by fans.
  // TIM6/7 - Basic timers with no pin connections. Cannot conflict with fans.
  // TIM5   - Previous default in STM32F1 HAL, most likely free on most boards
  // TIM2   - Previous default in both STM32 and STM32F1 HALs
  // TIM4   - Previous default in STM32 HAL and for value-line chips in STM32F1 HAL
  static constexpr uintptr_t mcu_preferred_timers[] = {
    #if defined (TIM6_BASE)
      uintptr_t(TIM6),
    #endif
    #if defined (TIM7_BASE)
      uintptr_t(TIM7),
    #endif
    #if defined (TIM5_BASE)
      uintptr_t(TIM5),
    #endif
    uintptr_t(TIM2),
    uintptr_t(TIM4),
    };
#elif defined(STM32F0xx) || defined(STM32F4xx) || defined(STM32F7xx)
  // default selection order is fine
  static constexpr uintptr_t mcu_preferred_timers[] = {0};
#else
  #error "MCU not yet supported in this HAL"
#endif

#define _TIMER_DEV(X) TIM##X
#define TIMER_DEV(X) _TIMER_DEV(X)

static constexpr uintptr_t timers_in_use[] = {
    #if HAS_TMC_SW_SERIAL
      uintptr_t(TIMER_SERIAL),  // Set in variant.h, or as a define in platformio.h if not present in variant.h
    #endif
    #if ENABLED(SPEAKER)
      uintptr_t(TIMER_TONE),    // Set in variant.h, or as a define in platformio.h if not present in variant.h
    #endif
    #if HAS_SERVOS
      uintptr_t(TIMER_SERVO),   // Set in variant.h, or as a define in platformio.h if not present in variant.h
    #endif
    #ifdef STEP_TIMER
      uintptr_t(TIMER_DEV(STEP_TIMER)),
    #endif
    #ifdef TEMP_TIMER
      uintptr_t(TIMER_DEV(TEMP_TIMER)),
    #endif
    0u
  };

static constexpr bool is_timer_in_use(uintptr_t timer) {
  for (auto timer_in_use : timers_in_use)
    if (timer_in_use && timer_in_use == timer) return true;
  return false;
}

static constexpr bool is_timer_in_mcu_preferred_list(uintptr_t timer) {
  for (auto preferred_timer : mcu_preferred_timers)
    if (timer == preferred_timer) return true;
  return false;
}

// Choose an available timer. 1-indexed instance allows skipping already used timers.
static constexpr uintptr_t get_free_timer(int instance) {
  int found = 0;
  for (auto timer : mcu_preferred_timers)
    if (timer && !is_timer_in_use(timer) && instance == ++found)
      return timer;
  for (auto timer : default_preferred_timers)
    if (timer && !is_timer_in_use(timer) && !is_timer_in_mcu_preferred_list(timer) && instance == ++found)
      return timer;
  return 0;
}

#ifdef STEP_TIMER
  #define TEMP_TIMER_SEARCH_INDEX 1
  #define STEP_TIMER_DEV TIMER_DEV(STEP_TIMER)
#else
  constexpr uintptr_t MCU_STEP_TIMER = get_free_timer(1);
  static_assert(0u != MCU_STEP_TIMER, "Available timer could not be found for STEP_TIMER");
  #define TEMP_TIMER_SEARCH_INDEX 2
  #define STEP_TIMER_DEV ((TIM_TypeDef*)(MCU_STEP_TIMER))
#endif
#ifdef TEMP_TIMER
  #define TEMP_TIMER_DEV TIMER_DEV(TEMP_TIMER)
#else
  constexpr uintptr_t MCU_TEMP_TIMER = get_free_timer(TEMP_TIMER_SEARCH_INDEX);
  static_assert(0u != MCU_TEMP_TIMER, "Available timer could not be found for TEMP_TIMER");
  #define TEMP_TIMER_DEV ((TIM_TypeDef*)(MCU_TEMP_TIMER))
#endif

#ifndef HAL_TIMER_RATE
  #define HAL_TIMER_RATE GetStepperTimerClkFreq()
#endif

// ------------------------
// Private Variables
// ------------------------

HardwareTimer *timer_instance[NUM_HARDWARE_TIMERS] = { nullptr };

// ------------------------
// Public functions
// ------------------------

uint32_t GetStepperTimerClkFreq() {
  // Timer input clocks vary between devices, and in some cases between timers on the same device.
  // Retrieve at runtime to ensure device compatibility. Cache result to avoid repeated overhead.
  static uint32_t clkfreq = timer_instance[STEP_TIMER_NUM]->getTimerClkFreq();
  return clkfreq;
}

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

// This does not include automatically assigned TEMP and STEP timers in
// conflict detection, since they won't be assigned if already in use.
// This does NOT account for PWM outputs such as fans, lasers, etc. When they
// are available at build-time they should impact automatic timer selection.
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
