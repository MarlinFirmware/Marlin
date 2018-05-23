/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 * Copyright (c) 2016 Bob Cousins bobcousins42@googlemail.com
 * Copyright (c) 2015-2016 Nico Tonnhofer wurstnase.reprap@gmail.com
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifdef __PLAT_X86_64__

#include "hardware/Timer.h"

#include "../../inc/MarlinConfig.h"
#include "HAL_timers.h"

/*
 *
 * Uses posix signals to attempt to emulate Interrupts
 * This has many limitations and is not fit for purpose
 *
 */

uint64_t native_ticks(uint32_t frequency) {
  auto now = std::chrono::high_resolution_clock::now();
  auto now_ns = std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
  auto value = now_ns.time_since_epoch();
  static auto startup_value = value;
  return (value.count() - startup_value.count()) / (1000000000ULL / frequency);
}

HAL_STEP_TIMER_ISR;
HAL_TEMP_TIMER_ISR;

Timer timers[2];


void HAL_timer_init(void) {
  timers[0].init(0, HAL_STEPPER_TIMER_RATE, TIMER0_IRQHandler);
  timers[1].init(1, HAL_TEMP_TIMER_RATE, TIMER1_IRQHandler);
}

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  timers[timer_num].start(frequency);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  timers[timer_num].enable();
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  timers[timer_num].disable();
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  return timers[timer_num].enabled();
}

void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  timers[timer_num].setCompare(compare);
}

hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  return timers[timer_num].getCompare();
}

hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  return timers[timer_num].getCount();
}

#endif // __PLAT_X86_64__
