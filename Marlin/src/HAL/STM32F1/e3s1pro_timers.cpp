/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

/**
 * HAL/STM32F1/e3s1pro_timers.cpp
 */

#ifdef __STM32F1__

#include "../../inc/MarlinConfig.h"

#if HAS_LASER_E3S1PRO

static unsigned int sw = 0, laser_h = 0, laser_l = 0;

void laser_timer_soft_pwm_start(const uint8_t pwm) {
  //SERIAL_ECHOLNPGM("laser_timer_soft_pwm_start():", pwm);

  LIMIT(pwm, 1, 255);

  if (pwm <= 0x00) { //
    timer_pause(LASER_TIMER_DEV);
    WRITE(LASER_SOFT_PWM_PIN, LOW); //WRITE(PC0, 0);
  }
  else if (pwm >= 0xFE) {
    timer_pause(LASER_TIMER_DEV);
    WRITE(LASER_SOFT_PWM_PIN, HIGH); //WRITE(PC0, 0);
  }
  else {
    timer_pause(LASER_TIMER_DEV);
    sw = 0;
    laser_l = 256 - pwm;
    laser_h = pwm;
    timer_resume(LASER_TIMER_DEV);
  }
}

void laser_timer_soft_pwm_stop() {
  //SERIAL_ECHOLNPGM("laser_timer_soft_pwm_stop()");
  laser_timer_soft_pwm_start(1);
}

void laser_timer_soft_pwm_close() {
  timer_pause(LASER_TIMER_DEV);
	WRITE(LASER_SOFT_PWM_PIN, LOW); // WRITE(PC0, 0)
}

void laser_timer_handler() {
  //SERIAL_ECHOLNPGM("laser_timer_handler");

  switch (sw) {
    case 0:
      timer_set_reload(LASER_TIMER_DEV, laser_l); // 0xFFFF
      WRITE(LASER_SOFT_PWM_PIN, HIGH);            // WRITE(PC0, 1)
      sw = 1;
      break;
    case 1:
      timer_set_reload(LASER_TIMER_DEV, laser_h); // 0xFFFF
      WRITE(LASER_SOFT_PWM_PIN, LOW);             // WRITE(PC0, 0)
      sw = 0;
      break;
  }
}

void laser_timer_soft_pwm_init(const uint32_t frequency) {
  timer_pause(LASER_TIMER_DEV);
  //timer_set_mode(LASER_TIMER_DEV, TEMP_TIMER_CHAN, TIMER_OUTPUT_COMPARE);
  timer_set_count(LASER_TIMER_DEV, 0);
  timer_set_prescaler(LASER_TIMER_DEV, (uint16_t)(LASER_TIMER_PRESCALE(frequency) - 1));
  timer_set_reload(LASER_TIMER_DEV, LASER_TIMER_PWM_MAX);//0xFFFF);
  //timer_set_compare(LASER_TIMER_DEV, TEMP_TIMER_CHAN, _MIN(hal_timer_t(HAL_TIMER_TYPE_MAX), (F_CPU) / (LASER_TIMER_PRESCALE) / frequency));
  timer_attach_interrupt(LASER_TIMER_DEV, TEMP_TIMER_CHAN, laser_timer_handler);
  timer_set_interrupt_priority(LASER_TIMER_NUM, LASER_TIMER_IRQ_PRIO);
  timer_generate_update(LASER_TIMER_DEV);
  // timer_resume(LASER_TIMER_DEV);
}

#endif // HAS_LASER_E3S1PRO
#endif // __STM32F1__
