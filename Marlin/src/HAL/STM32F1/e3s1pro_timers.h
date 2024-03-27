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
#pragma once

/**
 * HAL/STM32F1/e3s1pro_timers.h
 */

#include <stdint.h>

#define LASER_TIMER_NUM             3
#define LASER_TIMER_DEV             TIMER_DEV(LASER_TIMER_NUM)
#define LASER_TIMER_FREQUENCY       1000 // PWM freq:1000Hz
#define LASER_TIMER_PWM_MAX         255 // PWM value range: 0~255
#define LASER_TIMER_PRESCALE(freq)  (HAL_TIMER_RATE / (freq * (LASER_TIMER_PWM_MAX + 1))) // (72M/1000*256)=281
#define LASER_TIMER_CHAN            1
#define LASER_TIMER_IRQ_PRIO        1

void laser_timer_soft_pwm_init(const uint32_t frequency);
void laser_timer_soft_pwm_start(uint8_t pwm);
void laser_timer_soft_pwm_stop();
void laser_timer_soft_pwm_close();
