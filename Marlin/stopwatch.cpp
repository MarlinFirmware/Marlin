/*
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

#include "Marlin.h"
#include "stopwatch.h"

Stopwatch::Stopwatch() {
   this->reset();
 }

void Stopwatch::stop() {
  if (DEBUGGING(INFO)) SERIAL_ECHOLNPGM("Stopwatch::stop()");
  if (!this->isRunning()) return;

  this->status = STPWTCH_STOPPED;
  this->stopTimestamp = millis();
}

void Stopwatch::pause() {
  if (DEBUGGING(INFO)) SERIAL_ECHOLNPGM("Stopwatch::pause()");
  if (!this->isRunning()) return;

  this->status = STPWTCH_PAUSED;
  this->stopTimestamp = millis();
}

void Stopwatch::start() {
  if (DEBUGGING(INFO)) SERIAL_ECHOLNPGM("Stopwatch::start()");
  if (this->isRunning()) return;

  if (this->isPaused()) this->accumulator = this->duration();
  else this->reset();

  this->status = STPWTCH_RUNNING;
  this->startTimestamp = millis();
}

void Stopwatch::reset() {
  if (DEBUGGING(INFO)) SERIAL_ECHOLNPGM("Stopwatch::reset()");

  this->status = STPWTCH_STOPPED;
  this->startTimestamp = 0;
  this->stopTimestamp = 0;
  this->accumulator = 0;
}

bool Stopwatch::isRunning() {
  return (this->status == STPWTCH_RUNNING) ? true : false;
}

bool Stopwatch::isPaused() {
  return (this->status == STPWTCH_PAUSED) ? true : false;
}

uint16_t Stopwatch::duration() {
  return (((this->isRunning()) ? millis() : this->stopTimestamp)
          - this->startTimestamp) / 1000 + this->accumulator;
}
