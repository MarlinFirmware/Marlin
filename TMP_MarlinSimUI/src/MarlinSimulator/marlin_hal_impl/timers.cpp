/**
 * Marlin 3D Printer Firmware
 *
 * Copyright (c) 2020 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include <src/inc/MarlinConfig.h>

#include <MarlinSimulator/execution_control.h>
extern Kernel kernel;

void HAL_timer_init() {
  Kernel::Timers::timerInit(MF_TIMER_STEP, STEPPER_TIMER_RATE);
  Kernel::Timers::timerInit(MF_TIMER_TEMP, TEMP_TIMER_RATE);
  // Configure and start systick early
  Kernel::Timers::timerInit(MF_TIMER_SYSTICK, 1000000);
  HAL_timer_enable_interrupt(MF_TIMER_SYSTICK);
  HAL_timer_start(MF_TIMER_SYSTICK, SYSTICK_TIMER_FREQUENCY);
}

void HAL_timer_start(const uint8_t timer_num, const uint32_t frequency) {
  Kernel::Timers::timerStart(timer_num, frequency);
}

void HAL_timer_enable_interrupt(const uint8_t timer_num) {
  Kernel::Timers::timerEnable(timer_num);
}

void HAL_timer_disable_interrupt(const uint8_t timer_num) {
  Kernel::Timers::timerDisable(timer_num);
}

bool HAL_timer_interrupt_enabled(const uint8_t timer_num) {
  return Kernel::Timers::timerEnabled(timer_num);
}

void HAL_timer_set_compare(const uint8_t timer_num, const hal_timer_t compare) {
  Kernel::Timers::timerSetCompare(timer_num, compare);
}

hal_timer_t HAL_timer_get_compare(const uint8_t timer_num) {
  return Kernel::Timers::timerGetCompare(timer_num);
}

hal_timer_t HAL_timer_get_count(const uint8_t timer_num) {
  return Kernel::Timers::timerGetCount(timer_num);
}
