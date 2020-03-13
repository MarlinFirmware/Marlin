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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "stopwatch.h"

#include "../inc/MarlinConfig.h"

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extui/ui_api.h"
#endif

Stopwatch::State Stopwatch::state;
millis_t Stopwatch::accumulator;
millis_t Stopwatch::startTimestamp;
millis_t Stopwatch::stopTimestamp;

bool Stopwatch::stop() {
  #if ENABLED(DEBUG_STOPWATCH)
    Stopwatch::debug(PSTR("stop"));
  #endif

  if (isRunning() || isPaused()) {
    #if ENABLED(EXTENSIBLE_UI)
      ExtUI::onPrintTimerStopped();
    #endif
    state = STOPPED;
    stopTimestamp = millis();
    return true;
  }
  else return false;
}

bool Stopwatch::pause() {
  #if ENABLED(DEBUG_STOPWATCH)
    Stopwatch::debug(PSTR("pause"));
  #endif

  if (isRunning()) {
    #if ENABLED(EXTENSIBLE_UI)
      ExtUI::onPrintTimerPaused();
    #endif
    state = PAUSED;
    stopTimestamp = millis();
    return true;
  }
  else return false;
}

bool Stopwatch::start() {
  #if ENABLED(DEBUG_STOPWATCH)
    Stopwatch::debug(PSTR("start"));
  #endif

  #if ENABLED(EXTENSIBLE_UI)
    ExtUI::onPrintTimerStarted();
  #endif

  if (!isRunning()) {
    if (isPaused()) accumulator = duration();
    else reset();

    state = RUNNING;
    startTimestamp = millis();
    return true;
  }
  else return false;
}

void Stopwatch::resume(const millis_t with_time) {
  #if ENABLED(DEBUG_STOPWATCH)
    Stopwatch::debug(PSTR("resume"));
  #endif

  reset();
  if ((accumulator = with_time)) state = RUNNING;
}

void Stopwatch::reset() {
  #if ENABLED(DEBUG_STOPWATCH)
    Stopwatch::debug(PSTR("reset"));
  #endif

  state = STOPPED;
  startTimestamp = 0;
  stopTimestamp = 0;
  accumulator = 0;
}

millis_t Stopwatch::duration() {
  return ((isRunning() ? millis() : stopTimestamp)
          - startTimestamp) / 1000UL + accumulator;
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
