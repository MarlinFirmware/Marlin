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

// ------------------------
// Local defines
// ------------------------

#define SWSERIAL_TIMER_IRQ_PRIORITY_DEFAULT 1   // Requires tight bit timing to communicate reliably with TMC drivers
#define SERVO_TIMER_IRQ_PRIORITY_DEFAULT    1   // Requires tight PWM timing to control a BLTouch reliably
#define STEP_TIMER_IRQ_PRIORITY_DEFAULT     2
#define TEMP_TIMER_IRQ_PRIORITY_DEFAULT     14  // Low priority avoids interference with other hardware and timers

#ifndef TIMER_IRQ_PRIORITY
    #define TIMER_IRQ_PRIORITY 13
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
    // Ensure the default timer priority is somewhere between the STEP and TEMP priorities.
    // The GD32 MFL framework defaults to priority 14 for all timers. This should be increased so that
    // timing sensitive operations such as speaker output are not impacted by the long running
    // temperature ISR. This must be defined in the platformio.ini file or the board's variant.h,
    // so that it will be consumed by framework code.
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

GeneralTimer& Step_Timer = GeneralTimer::get_instance(timer::TIMER_Base::TIMER3_BASE);
GeneralTimer& Temp_Timer = GeneralTimer::get_instance(timer::TIMER_Base::TIMER1_BASE);

bool is_step_timer_initialized = false;
bool is_temp_timer_initialized = false;


// ------------------------
// Public functions
// ------------------------

// Retrieves the clock frequency of the stepper timer
// @return The clock frequency of the stepper timer in Hz
uint32_t GetStepperTimerClkFreq() {
    return Step_Timer.getTimerClockFrequency();
}

// Starts a hardware timer
//
// If the timer is not already initialized, this function will initialize it with the given frequency.
// The timer is started immediately after initialization
//
// @param timer The timer base index to start
// @param frequency The frequency at which the timer should run
//
// @return None
void HAL_timer_start(const uint8_t timer_number, const uint32_t frequency) {
    if (!HAL_timer_initialized(timer_number)) {
        switch (timer_number) {
        case MF_TIMER_STEP:
            Step_Timer.setPrescaler(STEPPER_TIMER_PRESCALE);
            Step_Timer.setRolloverValue(_MIN(static_cast<hal_timer_t>(HAL_TIMER_TYPE_MAX),
                                    (HAL_TIMER_RATE) / (STEPPER_TIMER_PRESCALE)), TimerFormat::TICK);
            is_step_timer_initialized = true;
            Step_Timer.setAutoReloadEnable(false);
            HAL_timer_enable_interrupt(timer_number);
            Step_Timer.start();
            Step_Timer.setInterruptPriority(static_cast<uint8_t>(STEP_TIMER_IRQ_PRIORITY), 0U);
            break;
        case MF_TIMER_TEMP:
            Temp_Timer.setRolloverValue(frequency, TimerFormat::HERTZ);
            is_temp_timer_initialized = true;
            Temp_Timer.setAutoReloadEnable(false);
            HAL_timer_enable_interrupt(timer_number);
            Temp_Timer.start();
            Temp_Timer.setInterruptPriority(static_cast<uint8_t>(TEMP_TIMER_IRQ_PRIORITY), 0U);
            break;
        }
    }
}

// Enables the interrupt for the specified timer
//
// @param handle The timer handle for which to enable the interrupt
// @return None
void HAL_timer_enable_interrupt(const uint8_t timer_number) {
    if (HAL_timer_initialized(timer_number)) {
        switch (timer_number) {
        case MF_TIMER_STEP:
            if (!Step_Timer.hasInterrupt()) {
                Step_Timer.attachInterrupt(Step_Handler);
            }
            break;
        case MF_TIMER_TEMP:
            if (!Temp_Timer.hasInterrupt()) {
                Temp_Timer.attachInterrupt(Temp_Handler);
            }
            break;
        }
    }
}

// Disables the interrupt for the specified timer
//
// @param handle The timer handle for which to disable the interrupt
// @return None
void HAL_timer_disable_interrupt(const uint8_t timer_number) {
    if (HAL_timer_initialized(timer_number)) {
        if (timer_number == MF_TIMER_STEP) {
            Step_Timer.detachInterrupt();
        } else if (timer_number == MF_TIMER_TEMP) {
            Temp_Timer.detachInterrupt();
        }
    }
}

// Checks if the interrupt is enabled for the specified timer
//
// @param handle The timer handle to check
// @return True if the interrupt is enabled, false otherwise
bool HAL_timer_interrupt_enabled(const uint8_t timer_number) {
    if (HAL_timer_initialized(timer_number)) {
        if (timer_number == MF_TIMER_STEP) {
            return Step_Timer.hasInterrupt();
        } else if (timer_number == MF_TIMER_TEMP) {
            return Temp_Timer.hasInterrupt();
        }
    }
    return false;
}

// Sets the interrupt priorities for timers used by TMC SW serial and servos.
//
// @return None
void SetTimerInterruptPriorities() {
    TERN_(HAS_TMC_SW_SERIAL, SoftwareSerial::setInterruptPriority(SWSERIAL_TIMER_IRQ_PRIORITY, 0));
    TERN_(HAS_SERVOS, libServo::setInterruptPriority(SERVO_TIMER_IRQ_PRIORITY, 0));
}

// ------------------------
// Detect timer conflicts
// ------------------------

// This list serves two purposes. Firstly, it facilitates build-time mapping between
// variant-defined timer names and timer numbers. It also replicates
// the order of timers used in the framework's SoftwareSerial.cpp. The first timer in
// this list will be automatically used by SoftwareSerial if it is not already defined
// in the board's variant or compiler options.
static constexpr struct {timer::TIMER_Base base; uint8_t timer_number;} mfl_timer_map[] = {
        { timer::TIMER_Base::TIMER6_BASE, 6 },
        { timer::TIMER_Base::TIMER5_BASE, 5 },
        { timer::TIMER_Base::TIMER4_BASE, 4 },
        { timer::TIMER_Base::TIMER3_BASE, 3 },
        { timer::TIMER_Base::TIMER2_BASE, 2 },
        { timer::TIMER_Base::TIMER1_BASE, 1 },
        { timer::TIMER_Base::TIMER7_BASE, 7 },
        { timer::TIMER_Base::TIMER0_BASE, 0 }
};

// Converts a timer base address to its corresponding integer timer number.
//
// @param base_address The base address of the timer to look up.
// @return The integer timer number corresponding to the base address, or -1 if not found.
static constexpr int get_timer_num_from_base_address(timer::TIMER_Base base) {
    for (const auto &timer : mfl_timer_map) {
        if (timer.base == base){
            return static_cast<int>(timer.timer_number);
        }
    }
    return -1;
}

// The platform's SoftwareSerial.cpp will use the first timer from mfl_timer_map
#if HAS_TMC_SW_SERIAL && !defined(TIMER_SERIAL)
    #define TIMER_SERIAL  (mfl_timer_map[0].base)
#endif

TERN_(HAS_TMC_SW_SERIAL, static constexpr timer::TIMER_Base timer_serial[] = {static_cast<timer::TIMER_Base>(TIMER_SERIAL)});
TERN_(SPEAKER,           static constexpr timer::TIMER_Base timer_tone[]   = {static_cast<timer::TIMER_Base>(TIMER_TONE)});
TERN_(HAS_SERVOS,        static constexpr timer::TIMER_Base timer_servo[]  = {static_cast<timer::TIMER_Base>(TIMER_SERVO)});

enum TimerPurpose {
    PURPOSE_SERIAL,
    PURPOSE_TONE,
    PURPOSE_SERVO,
    PURPOSE_STEP,
    PURPOSE_TEMP
};

// List of timers to enable checking for conflicts
// Includes the timer purpose to ease debugging when evaluating at build-time
// This cannot yet account for timers used for PWM output, such as for fans
static constexpr struct { TimerPurpose p; int t; } timers_in_use[] = {
    #if HAS_TMC_SW_SERIAL
        { PURPOSE_SERIAL, get_timer_num_from_base_address(timer_serial[0]) }, // Set in variant.h, or as a define in platformio.h if not present in variant.h
    #endif
    #if ENABLED(SPEAKER)
        { PURPOSE_TONE, get_timer_num_from_base_address(timer_tone[0]) },     // Set in variant.h, or as a define in platformio.h if not present in variant.h
    #endif
    #if HAS_SERVOS
        { PURPOSE_SERVO, get_timer_num_from_base_address(timer_servo[0]) },   // Set in variant.h, or as a define in platformio.h if not present in variant.h
    #endif
    { PURPOSE_STEP, MF_TIMER_STEP },
    { PURPOSE_TEMP, MF_TIMER_TEMP },
};

// Verifies if there are any timer conflicts in the timers_in_use array
// @return True if no timer conflicts are found, false otherwise
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
