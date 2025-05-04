/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2025 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../platforms.h"

#ifdef ARDUINO_ARCH_MFL

#include "../../inc/MarlinConfig.h"
#include "timers.h"

// ------------------------
// Local defines
// ------------------------

#define SWSERIAL_TIMER_IRQ_PRIORITY_DEFAULT 1   // Requires tight bit timing to communicate reliably with TMC drivers
#define SERVO_TIMER_IRQ_PRIORITY_DEFAULT    1   // Requires tight PWM timing to control a BLTouch reliably
#define STEP_TIMER_IRQ_PRIORITY_DEFAULT     2
#define TEMP_TIMER_IRQ_PRIORITY_DEFAULT     14  // Low priority avoids interference with other hardware and timers

#ifndef TIMER_IRQ_PRIORITY
  #define TIMER_IRQ_PRIORITY 12
#endif

#ifndef STEP_TIMER_IRQ_PRIORITY
  #define STEP_TIMER_IRQ_PRIORITY STEP_TIMER_IRQ_PRIORITY_DEFAULT
#endif

#ifndef TEMP_TIMER_IRQ_PRIORITY
  #define TEMP_TIMER_IRQ_PRIORITY TEMP_TIMER_IRQ_PRIORITY_DEFAULT
#endif

#if HAS_TMC_SW_SERIAL
  #include <SoftwareSerial.h>
  #ifndef SWSERIAL_TIMER_IRQ_PRIORITY
    #define SWSERIAL_TIMER_IRQ_PRIORITY SWSERIAL_TIMER_IRQ_PRIORITY_DEFAULT
  #endif
#endif

#if HAS_SERVOS
  #include "Servo.h"
  #ifndef SERVO_TIMER_IRQ_PRIORITY
    #define SERVO_TIMER_IRQ_PRIORITY    SERVO_TIMER_IRQ_PRIORITY_DEFAULT
  #endif
#endif

#if ENABLED(SPEAKER)
  // The MFL framework default timer priority is 12. The TEMP timer must have lower priority
  // than this due to the long running temperature ISR, and STEP timer should higher priority.
  #if !(TIMER_IRQ_PRIORITY > STEP_TIMER_IRQ_PRIORITY && TIMER_IRQ_PRIORITY < TEMP_TIMER_IRQ_PRIORITY)
    #error "Default timer interrupt priority is unspecified or set to a value which may degrade performance."
  #endif
#endif

#ifndef HAL_TIMER_RATE
  #define HAL_TIMER_RATE  GetStepperTimerClkFreq()
#endif

#ifndef STEP_TIMER
  #define STEP_TIMER    MF_TIMER_STEP
#endif
#ifndef TEMP_TIMER
  #define TEMP_TIMER    MF_TIMER_TEMP
#endif

GeneralTimer& Step_Timer = GeneralTimer::get_instance(static_cast<timer::TIMER_Base>(STEP_TIMER));
GeneralTimer& Temp_Timer = GeneralTimer::get_instance(static_cast<timer::TIMER_Base>(TEMP_TIMER));

bool is_step_timer_initialized = false;
bool is_temp_timer_initialized = false;

// ------------------------
// Public functions
// ------------------------

// Retrieves the clock frequency of the stepper timer
uint32_t GetStepperTimerClkFreq() {
  // Cache result
  static uint32_t clkFreq = Step_Timer.getTimerClockFrequency();
  return clkFreq;
}

/**
 * @brief Starts a hardware timer
 *
 * If the timer is not already initialized, this function will initialize it with the given frequency.
 * The timer is started immediately after initialization
 *
 * @param timer The timer base index to start
 * @param frequency The frequency at which the timer should run
 * @return None
 */
void HAL_timer_start(const uint8_t timer_number, const uint32_t frequency) {
  if (HAL_timer_initialized(timer_number) || (timer_number != MF_TIMER_STEP && timer_number != MF_TIMER_TEMP))
    return;

  const bool is_step = (timer_number == MF_TIMER_STEP);
  const uint8_t priority = is_step ?
                           static_cast<uint8_t>(STEP_TIMER_IRQ_PRIORITY) :
                           static_cast<uint8_t>(TEMP_TIMER_IRQ_PRIORITY);

  // Get the reference of the timer instance
  GeneralTimer& timer = is_step ? Step_Timer : Temp_Timer;

  if (is_step) {
    timer.setPrescaler(STEPPER_TIMER_PRESCALE);
    timer.setRolloverValue(_MIN(static_cast<hal_timer_t>(HAL_TIMER_TYPE_MAX),
                               (HAL_TIMER_RATE) / (STEPPER_TIMER_PRESCALE)),
                               TimerFormat::TICK);
    is_step_timer_initialized = true;
  }
  else {
    timer.setRolloverValue(frequency, TimerFormat::HERTZ);
    is_temp_timer_initialized = true;
  }

  timer.setAutoReloadEnable(false);
  timer.setInterruptPriority(priority, 0U);
  HAL_timer_enable_interrupt(timer_number);
  timer.start();
}

/**
 * @brief Enables the interrupt for the specified timer
 *
 * @param handle The timer handle for which to enable the interrupt
 * @return None
 */
void HAL_timer_enable_interrupt(const uint8_t timer_number) {
  if (!HAL_timer_initialized(timer_number)) return;

  GeneralTimer& timer = (timer_number == MF_TIMER_STEP) ? Step_Timer : Temp_Timer;

  if (timer_number == MF_TIMER_STEP && !timer.hasInterrupt())
    timer.attachInterrupt(Step_Handler);
  else if (timer_number == MF_TIMER_TEMP && !timer.hasInterrupt())
    timer.attachInterrupt(Temp_Handler);
}

/**
 * @brief Disables the interrupt for the specified timer
 *
 * @param handle The timer handle for which to disable the interrupt
 * @return None
 */
void HAL_timer_disable_interrupt(const uint8_t timer_number) {
  if (!HAL_timer_initialized(timer_number)) return;

  GeneralTimer& timer = (timer_number == MF_TIMER_STEP) ? Step_Timer : Temp_Timer;
  if (timer_number == MF_TIMER_STEP || timer_number == MF_TIMER_TEMP)
    timer.detachInterrupt();
}

/**
 * @brief Checks if the interrupt is enabled for the specified timer
 *
 * @param handle The timer handle to check
 * @return True if the interrupt is enabled, false otherwise
 */
bool HAL_timer_interrupt_enabled(const uint8_t timer_number) {
  if (!HAL_timer_initialized(timer_number)) return false;

  GeneralTimer& timer = (timer_number == MF_TIMER_STEP) ? Step_Timer : Temp_Timer;
  return (timer_number == MF_TIMER_STEP || timer_number == MF_TIMER_TEMP)
         ? timer.hasInterrupt()
         : false;
}

// Sets the interrupt priorities for timers used by TMC SW serial and servos.
void SetTimerInterruptPriorities() {
  TERN_(HAS_TMC_SW_SERIAL, SoftwareSerial::setInterruptPriority(SWSERIAL_TIMER_IRQ_PRIORITY, 0));
  TERN_(HAS_SERVOS, libServo::setInterruptPriority(SERVO_TIMER_IRQ_PRIORITY, 0));
}

// ------------------------
// Detect timer conflicts
// ------------------------

TERN_(HAS_TMC_SW_SERIAL, static constexpr timer::TIMER_Base timer_serial[] = {static_cast<timer::TIMER_Base>(TIMER_SERIAL)});
TERN_(SPEAKER, static constexpr timer::TIMER_Base timer_tone[] = {static_cast<timer::TIMER_Base>(TIMER_TONE)});
TERN_(HAS_SERVOS, static constexpr timer::TIMER_Base timer_servo[] = {static_cast<timer::TIMER_Base>(TIMER_SERVO)});

enum TimerPurpose {
  PURPOSE_SERIAL,
  PURPOSE_TONE,
  PURPOSE_SERVO,
  PURPOSE_STEP,
  PURPOSE_TEMP
};

// List of timers to check for conflicts
// Includes the timer purpose to ease debugging when evaluating at build-time
// This cannot yet account for timers used for PWM output, such as for fans
static constexpr struct { TimerPurpose p; int t; } timers_in_use[] = {
  #if HAS_TMC_SW_SERIAL
    { PURPOSE_SERIAL, timer_base_to_index(timer_serial[0]) }, // Set in variant.h
  #endif
  #if ENABLED(SPEAKER)
    { PURPOSE_TONE, timer_base_to_index(timer_tone[0]) },     // Set in variant.h
  #endif
  #if HAS_SERVOS
    { PURPOSE_SERVO, timer_base_to_index(timer_servo[0]) },   // Set in variant.h
  #endif
  { PURPOSE_STEP, MF_TIMER_STEP },
  { PURPOSE_TEMP, MF_TIMER_TEMP },
};

// Verifies if there are any timer conflicts in the timers_in_use array
static constexpr bool verify_no_timer_conflicts() {
  for (uint8_t i = 0; i < COUNT(timers_in_use); i++)
    for (uint8_t j = i + 1; j < COUNT(timers_in_use); j++)
      if (timers_in_use[i].t == timers_in_use[j].t)
        return false;

  return true;
}

// If this assertion fails at compile time, review the timers_in_use array.
// If default_envs is defined properly in platformio.ini, VSCode can evaluate the array
// when hovering over it, making it easy to identify the conflicting timers
static_assert(verify_no_timer_conflicts(), "One or more timer conflict detected. Examine \"timers_in_use\" to help identify conflict.");

#endif // ARDUINO_ARCH_MFL
