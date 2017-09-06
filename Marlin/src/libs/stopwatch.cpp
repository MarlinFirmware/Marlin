/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2016 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
 *
 * Based on Sprinter and grbl.
 * Copyright (C) 2011 Camiel Gubbels / Erik van der Zalm
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

#include "../Marlin.h"
#include "stopwatch.h"

Stopwatch::Stopwatch() {
  this->reset();
}

bool Stopwatch::stop() {
  #if ENABLED(DEBUG_STOPWATCH)
    Stopwatch::debug(PSTR("stop"));
  #endif

  if (this->isRunning() || this->isPaused()) {
    this->state = STOPPED;
    this->stopTimestamp = millis();
    return true;
  }
  else return false;
}

bool Stopwatch::pause() {
  #if ENABLED(DEBUG_STOPWATCH)
    Stopwatch::debug(PSTR("pause"));
  #endif

  if (this->isRunning()) {
    this->state = PAUSED;
    this->stopTimestamp = millis();
    return true;
  }
  else return false;
}

bool Stopwatch::start() {
  #if ENABLED(DEBUG_STOPWATCH)
    Stopwatch::debug(PSTR("start"));
  #endif

  if (!this->isRunning()) {
    if (this->isPaused()) this->accumulator = this->duration();
    else this->reset();

    this->state = RUNNING;
    this->startTimestamp = millis();
    return true;
  }
  else return false;
}

void Stopwatch::reset() {
  #if ENABLED(DEBUG_STOPWATCH)
    Stopwatch::debug(PSTR("reset"));
  #endif

  this->state = STOPPED;
  this->startTimestamp = 0;
  this->stopTimestamp = 0;
  this->accumulator = 0;
}

bool Stopwatch::isRunning() {
  return (this->state == RUNNING) ? true : false;
}

bool Stopwatch::isPaused() {
  return (this->state == PAUSED) ? true : false;
}

millis_t Stopwatch::duration() {
  return (((this->isRunning()) ? millis() : this->stopTimestamp)
          - this->startTimestamp) / 1000UL + this->accumulator;
}

#if ENABLED(DEBUG_STOPWATCH)

  void Stopwatch::debug(const char func[]) {
    if (DEBUGGING(INFO)) {
      SERIAL_ECHOPGM("Stopwatch::");
      serialprintPGM(func);
      SERIAL_ECHOLNPGM("()");
    }
  }

#endif
