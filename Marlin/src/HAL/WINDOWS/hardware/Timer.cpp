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
#ifdef __PLAT_WINDOWS__

#include "Timer.h"
#include <stdio.h>


std::map<UINT_PTR,Timer*> Timer::_timers;

Timer::Timer() {
  active = false;
  compare = 0;
  frequency = 0;
  overruns = 0;
  timerid = 0;
  cbfn = nullptr;
  period = 0;
  start_time = 0;
  avg_error = 0;
}

Timer::~Timer() {
  KillTimer(NULL, this->timerid);
}

  
void Timer::init(uint32_t sig_id, uint32_t sim_freq, callback_fn* fn) {

  frequency = sim_freq;
  cbfn = fn;

  disable();

  timerid = SetTimer(NULL, timerid, USER_TIMER_MAXIMUM, handler);
  if(this->timerid == 0) {
    //error
  }
  _timers[this->timerid] = this;
}


void Timer::start(uint32_t frequency) {
  setCompare(this->frequency / frequency);
  //printf("timer(%ld) started\n", getID());
}

void Timer::enable() {
  /*
  if (sigprocmask(SIG_UNBLOCK, &mask, nullptr) == -1) {
    return; // todo: handle error
  }
  */
  active = true;
  //printf("timer(%ld) enabled\n", getID());
}

void Timer::disable() {
  /*
  if (sigprocmask(SIG_SETMASK, &mask, nullptr) == -1) {
    return; // todo: handle error
  }*/
  active = false;
}

void Timer::setCompare(uint32_t compare) {
  uint32_t nsec_offset = 0;
  if (active) {
    nsec_offset = Clock::nanos() - this->start_time; // calculate how long the timer would have been running for
    nsec_offset = nsec_offset < 1000 ? nsec_offset : 0; // constrain, this shouldn't be needed but apparently Marlin enables interrupts on the stepper timer before initialising it, todo: investigate ?bug?
  }
  this->compare = compare;
  uint64_t ns = Clock::ticksToNanos(compare, frequency) - nsec_offset;
  
  uint32_t ms = ns / 1000000;
  uint32_t sec = ms / 1000;
  
  timerid = SetTimer(NULL, timerid, ms, handler);

  if (timerid == 0) {
    printf("timer(%ld) failed, compare: %d(%ld)\n", getID(), compare, ms);
    return; // todo: handle error
  }
  
  this->period = sec;
  this->start_time = Clock::nanos();
}

uint32_t Timer::getCount() {
  return Clock::nanosToTicks(Clock::nanos() - this->start_time, frequency);
}

#endif // __PLAT_LINUX__
