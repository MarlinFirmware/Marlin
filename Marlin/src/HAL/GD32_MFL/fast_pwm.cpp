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

#include <PinOpsMap.hpp>
#include <PinOps.hpp>
#include "timers.h"

static uint16_t timer_frequency[TIMER_COUNT];

void MarlinHAL::set_pwm_duty(const pin_t pin, const uint16_t value, const uint16_t scale, const bool invert) {
  // Calculate duty cycle based on inversion flag
  const uint16_t duty = invert ? scale - value : value;

  // Check if the pin supports PWM
  if (PWM_PIN(pin)) {
    // Get the timer peripheral base associated with the pin
    const auto timer_base = getPinOpsPeripheralBase<TIMERPinOps, timer::TIMER_Base>(TIMER_PinOps, static_cast<pin_size_t>(pin));

    // Initialize the timer instance
    auto& TimerInstance = GeneralTimer::get_instance(timer_base);

    // Get channel and previous channel mode
    const auto channel = getPackedPinChannel(getPackedPinOps(TIMER_PinOps, static_cast<pin_size_t>(pin)));
    const InputOutputMode previous = TimerInstance.getChannelMode(channel);

    if (timer_frequency[static_cast<size_t>(timer_base)] == 0) {
      set_pwm_frequency(pin, PWM_FREQUENCY);
    }

    // Set the PWM duty cycle
    TimerInstance.setCaptureCompare(channel, duty, CCFormat::B8);

    // Configure pin as PWM output
    pinOpsPinout(TIMER_PinOps, static_cast<pin_size_t>(pin));

    // Set channel mode if not already set and start timer
    if (previous != InputOutputMode::PWM0) {
      TimerInstance.setChannelMode(channel, InputOutputMode::PWM0, static_cast<pin_size_t>(pin));
      TimerInstance.start();
    }
  } else {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, duty < scale / 2 ? LOW : HIGH);
  }
}

void MarlinHAL::set_pwm_frequency(const pin_t pin, const uint16_t f_desired) {
  // Check if the pin supports PWM
  if (!PWM_PIN(pin)) return;

  // Get the timer peripheral base associated with the pin
  const auto timer_base = getPinOpsPeripheralBase<TIMERPinOps, timer::TIMER_Base>(TIMER_PinOps, static_cast<pin_size_t>(pin));

  // Guard against modifying protected timers
  #ifdef STEP_TIMER
    if (timer_base == static_cast<timer::TIMER_Base>(STEP_TIMER)) return;
  #endif
  #ifdef TEMP_TIMER
    if (timer_base == static_cast<timer::TIMER_Base>(TEMP_TIMER)) return;
  #endif
  #if defined(PULSE_TIMER) && MF_TIMER_PULSE != MF_TIMER_STEP
    if (timer_base == static_cast<timer::TIMER_Base>(PULSE_TIMER)) return;
  #endif

  // Initialize the timer instance
  auto& TimerInstance = GeneralTimer::get_instance(timer_base);

  TimerInstance.setRolloverValue(f_desired, TimerFormat::HERTZ);
  timer_frequency[timer_base_to_index(timer_base)] = f_desired;
}

#endif // ARDUINO_ARCH_MFL
