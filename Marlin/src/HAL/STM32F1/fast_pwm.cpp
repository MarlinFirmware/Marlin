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
#ifdef __STM32F1__

#include "../../inc/MarlinConfigPre.h"

#if NEEDS_HARDWARE_PWM

#include <pwm.h>
#include "HAL.h"
#include "timers.h"

void set_pwm_frequency(const pin_t pin, int f_desired) {
  if (!PWM_PIN(pin)) return;                    // Don't proceed if no hardware timer

  timer_dev *timer = PIN_MAP[pin].timer_device;
  uint8_t channel = PIN_MAP[pin].timer_channel;

  // Protect used timers
  if (timer == get_timer_dev(TEMP_TIMER_NUM)) return;
  if (timer == get_timer_dev(STEP_TIMER_NUM)) return;
  #if PULSE_TIMER_NUM != STEP_TIMER_NUM
    if (timer == get_timer_dev(PULSE_TIMER_NUM)) return;
  #endif

  if (!(timer->regs.bas->SR & TIMER_CR1_CEN))   // Ensure the timer is enabled
    timer_init(timer);

  timer_set_mode(timer, channel, TIMER_PWM);
  uint16_t preload = 255;                       // Lock 255 PWM resolution for high frequencies
  int32_t prescaler = (HAL_TIMER_RATE) / (preload + 1) / f_desired - 1;
  if (prescaler > 65535) {                      // For low frequencies increase prescaler
    prescaler = 65535;
    preload = (HAL_TIMER_RATE) / (prescaler + 1) / f_desired - 1;
  }
  if (prescaler < 0) return;                    // Too high frequency
  timer_set_reload(timer, preload);
  timer_set_prescaler(timer, prescaler);
}

void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size/*=255*/, const bool invert/*=false*/) {
  timer_dev *timer = PIN_MAP[pin].timer_device;
  uint16_t max_val = timer->regs.bas->ARR * v / v_size;
  if (invert) max_val = v_size - max_val;
  pwmWrite(pin, max_val);
}

#endif // NEEDS_HARDWARE_PWM
#endif // __STM32F1__
