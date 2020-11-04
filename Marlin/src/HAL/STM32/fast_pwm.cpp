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

#include "../../inc/MarlinConfigPre.h"

#if NEEDS_HARDWARE_PWM

#include "HAL.h"
#include "timers.h"

void set_pwm_frequency(const pin_t pin, int f_desired) {
  if (!PWM_PIN(pin)) return;            // Don't proceed if no hardware timer

  PinName pin_name = digitalPinToPinName(pin);
  TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(pin_name, PinMap_PWM); // Get HAL timer instance
  
  LOOP_S_L_N(i, 0, NUM_HARDWARE_TIMERS) // Protect used timers
    if (timer_instance[i] && timer_instance[i]->getHandle()->Instance == Instance)
      return;

  pwm_start(pin_name, f_desired, 0, RESOLUTION_8B_COMPARE_FORMAT);
}

void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size/*=255*/, const bool invert/*=false*/) {
  PinName pin_name = digitalPinToPinName(pin);
  TIM_TypeDef *Instance = (TIM_TypeDef *)pinmap_peripheral(pin_name, PinMap_PWM);
  uint16_t adj_val = Instance->ARR * v / v_size;
  if (invert) adj_val = Instance->ARR - adj_val;

  switch (get_pwm_channel(pin_name)) {
    case TIM_CHANNEL_1: LL_TIM_OC_SetCompareCH1(Instance, adj_val); break;
    case TIM_CHANNEL_2: LL_TIM_OC_SetCompareCH2(Instance, adj_val); break;
    case TIM_CHANNEL_3: LL_TIM_OC_SetCompareCH3(Instance, adj_val); break;
    case TIM_CHANNEL_4: LL_TIM_OC_SetCompareCH4(Instance, adj_val); break;
  }
}

#endif // NEEDS_HARDWARE_PWM
