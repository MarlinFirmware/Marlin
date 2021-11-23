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

#include "../platforms.h"

#ifdef HAL_STM32

#include "../../inc/MarlinConfig.h"
#include "timers.h"

// Array to support sticky frequency sets per timer
static uint16_t timer_freq[TIMER_NUM];

void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size/*=255*/, const bool invert/*=false*/) {
  if (!PWM_PIN(pin)) return; // Don't proceed if no hardware timer
  bool needs_freq;
  PinName pin_name = digitalPinToPinName(pin);
  TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(pin_name, PinMap_PWM);
  HardwareTimer *HT;
  TimerModes_t previousMode;

  uint16_t value = v;
  if (invert) value = v_size - value;

  uint32_t index = get_timer_index(Instance);

  if (HardwareTimer_Handle[index] == nullptr) {
    HardwareTimer_Handle[index]->__this = new HardwareTimer((TIM_TypeDef *)pinmap_peripheral(pin_name, PinMap_PWM));
    needs_freq = true;                  // The instance must be new set the default frequency of PWM_FREQUENCY
  }

  HT = (HardwareTimer *)(HardwareTimer_Handle[index]->__this);
  uint32_t channel = STM_PIN_CHANNEL(pinmap_function(pin_name, PinMap_PWM));
  previousMode = HT->getMode(channel);

  if (previousMode != TIMER_OUTPUT_COMPARE_PWM1)
    HT->setMode(channel, TIMER_OUTPUT_COMPARE_PWM1, pin);

  if (needs_freq) {
    if (timer_freq[index] == 0 ) {                    // If the timer is unconfigured and no freq is set then default PWM_FREQUENCY.
      timer_freq[index] = PWM_FREQUENCY;
      set_pwm_frequency(pin_name, timer_freq[index]); // Set the frequency and save the value to the assigned index no.
    }
  }
  // Note the resolution is sticky here, the input can be upto 16 bits and that would require RESOLUTION_16B_COMPARE_FORMAT (16)
  // If such a need were to manifest then we would need to calc the resolution based on the v_size parameter and add code for it.
  HT->setCaptureCompare(channel, value, RESOLUTION_8B_COMPARE_FORMAT); // Sets the duty, the calc is done in the library :)
  pinmap_pinout(pin_name, PinMap_PWM); // Make sure the pin output state is set.
  if (previousMode != TIMER_OUTPUT_COMPARE_PWM1) HT->resume();
}

void set_pwm_frequency(const pin_t pin, int f_desired) {
  if (!PWM_PIN(pin)) return; // Don't proceed if no hardware timer
  HardwareTimer *HT;
  PinName pin_name = digitalPinToPinName(pin);
  TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(pin_name, PinMap_PWM); // Get HAL timer instance

  uint32_t index = get_timer_index(Instance);

  // Protect used timers
  if (index == TEMP_TIMER_NUM || index == STEP_TIMER_NUM
    #if PULSE_TIMER_NUM != STEP_TIMER_NUM
      || index == PULSE_TIMER_NUM
    #endif
  ) return;

  if (HardwareTimer_Handle[index] == nullptr) // If frequency is set before duty we need to create a handle here. 
    HardwareTimer_Handle[index]->__this = new HardwareTimer((TIM_TypeDef *)pinmap_peripheral(pin_name, PinMap_PWM));
  HT = (HardwareTimer *)(HardwareTimer_Handle[index]->__this);
  timer_freq[index] = f_desired; // Save the last frequency so duty will not set the default for this timer number.
  HT->setOverflow(f_desired, HERTZ_FORMAT);   
}

#endif // HAL_STM32
