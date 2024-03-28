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
 * HAL/STM32/e3s1pro_timers.cpp
 */

#include "../platforms.h"

#ifdef HAL_STM32

#include "../../inc/MarlinConfig.h"

#if HAS_LASER_E3S1PRO

#define LASER_TIMER_NUM 3
#define LASER_TIMER_DEV _TIMER_DEV(LASER_TIMER_NUM)
#define LASER_TIMER_PRESCALE(timer_clk, freq) ((timer_clk) / ((freq) * (LASER_TIMER_PWM_MAX + 1)))
#define LASER_TIMER_IRQ_PRIO 1

typedef enum {
  LASER_PWM_STATE_L = 0,
  LASER_PWM_STATE_H
} laser_pwm_state_t;

static HardwareTimer *timer_laser = nullptr;
static uint8_t laser_h = 0, laser_l = 0;
static laser_pwm_state_t laser_pwm_state;

FORCE_INLINE static void laser_timer_set_compare(const hal_timer_t overflow) {
  if (timer_laser) {
    timer_laser->setOverflow(overflow + 1, TICK_FORMAT);
    if (overflow < timer_laser->getCount())
      timer_laser->refresh();
  }
}

static void laser_timer_handler() {
  //SERIAL_ECHOLNPGM("laser_timer_handler");

  switch (laser_pwm_state) {
    case LASER_PWM_STATE_L:
      laser_timer_set_compare(laser_h);
      WRITE(LASER_SOFT_PWM_PIN, HIGH);
      laser_pwm_state = LASER_PWM_STATE_H;
      break;
    case LASER_PWM_STATE_H:
      laser_timer_set_compare(laser_l);
      WRITE(LASER_SOFT_PWM_PIN, LOW);
      laser_pwm_state = LASER_PWM_STATE_L;
      break;
  }
}

void laser_timer_soft_pwm_start(const uint8_t pwm) {
  //SERIAL_ECHOLNPGM("laser_timer_soft_pwm_start():", pwm);

  if (timer_laser == nullptr) return;

  if (pwm > LASER_TIMER_PWM_MAX) pwm = LASER_TIMER_PWM_MAX;

  if (pwm == 0x00) {
    laser_timer_soft_pwm_stop();
  }
  else if (pwm == 0xFF) {
    timer_laser->pause();
    OUT_WRITE(LASER_SOFT_PWM_PIN, HIGH);
  }
  else {
    timer_laser->pause();
    laser_pwm_state = LASER_PWM_STATE_H;
    WRITE(LASER_SOFT_PWM_PIN, HIGH);
    laser_l = LASER_TIMER_PWM_MAX - pwm;
    laser_h = pwm;
    laser_timer_set_compare(laser_h);
    timer_laser->resume();
    // 立即进入中断 -> 设置为0无法触发中断
    //laser_timer_set_compare(0);
  }
}

void laser_timer_soft_pwm_stop() {
  //SERIAL_ECHOLNPGM("laser_timer_soft_pwm_stop()");
  laser_timer_soft_pwm_start(1);
}

void laser_timer_soft_pwm_close() {
  //SERIAL_ECHOLNPGM("laser_timer_soft_pwm_close()");
  if (timer_laser == nullptr) return;
  timer_laser->pause();
  WRITE(LASER_SOFT_PWM_PIN, LOW);
}

void laser_timer_soft_pwm_init(const uint32_t frequency) {
  if (timer_laser == nullptr) {
    timer_laser = new HardwareTimer(LASER_TIMER_DEV);
    uint32_t prescale = LASER_TIMER_PRESCALE(timer_laser->getTimerClkFreq(), frequency);
    timer_laser->setPrescaleFactor(prescale);
    timer_laser->setOverflow(_MIN(hal_timer_t(HAL_TIMER_TYPE_MAX), (timer_laser->getTimerClkFreq()) / (prescale) /* /frequency */), TICK_FORMAT);
    timer_laser->setPreloadEnable(false);
    if (!timer_laser->hasInterrupt()) timer_laser->attachInterrupt(laser_timer_handler);
    timer_laser->pause();
    timer_laser->setInterruptPriority(LASER_TIMER_IRQ_PRIO, 0);
  }
}

#endif // HAS_LASER_E3S1PRO
#endif // HAL_STM32
