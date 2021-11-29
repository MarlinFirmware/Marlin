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

#include <pwm.h>
#include "HAL.h"
#include "timers.h"

static uint16_t timer_freq[TIMER_NUM];

void set_pwm_duty(const pin_t pin, const uint16_t v, const uint16_t v_size/*=255*/, const bool invert/*=false*/) {
  if (!PWM_PIN(pin)) return;
  uint16_t duty = v;
  if (invert) duty = v_size - duty;
  timer_dev * const timer = PIN_MAP[pin].timer_device;
  const uint8_t channel = PIN_MAP[pin].timer_channel;
  uint8_t timer_index = 0;
  for (uint8_t i = 0; i < TIMER_NUM; i++) if (timer == get_timer_dev(i)) { timer_index = i; break; }
  if (timer_freq[timer_index] == 0) set_pwm_frequency(pin, PWM_FREQUENCY);

  timer_set_compare(timer, channel, duty);
  timer_set_mode(timer, channel, TIMER_PWM); // PWM Output Mode
}

void set_pwm_frequency(const pin_t pin, int f_desired) {
  if (!PWM_PIN(pin)) return;                    // Don't proceed if no hardware timer

  timer_dev *timer = PIN_MAP[pin].timer_device;
  uint8_t channel = PIN_MAP[pin].timer_channel;

  uint8_t timer_index = 0;
  for (uint8_t i = 0; i < TIMER_NUM; i++) if (timer == get_timer_dev(i)) { timer_index = i; break; }
  timer_freq[timer_index] = f_desired;

  // Protect used timers
  if (timer == HAL_get_timer_dev(MF_TIMER_TEMP)) return;
  if (timer == HAL_get_timer_dev(MF_TIMER_STEP)) return;
  #if MF_TIMER_PULSE != MF_TIMER_STEP
    if (timer == HAL_get_timer_dev(MF_TIMER_PULSE)) return;
  #endif

  if (!(timer->regs.bas->SR & TIMER_CR1_CEN))   // Ensure the timer is enabled
    timer_init(timer);

  timer_set_mode(timer, channel, TIMER_PWM);
  // Preload (resolution) cannot be equal to duty of 255 otherwise it may not result in digital off or on.
  uint16_t preload = 254;
  int32_t prescaler = (HAL_TIMER_RATE) / (preload + 1) / f_desired - 1;
  if (prescaler > 65535) {                      // For low frequencies increase prescaler
    prescaler = 65535;
    preload = (HAL_TIMER_RATE) / (prescaler + 1) / f_desired - 1;
  }
  if (prescaler < 0) return;                    // Too high frequency
  timer_set_reload(timer, preload);
  timer_set_prescaler(timer, prescaler);
}

#endif // __STM32F1__
