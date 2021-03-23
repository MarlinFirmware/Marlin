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
  Stopwatch::debug(PSTR("stop"));

  if (isRunning() || isPaused()) {
    TERN_(EXTENSIBLE_UI, ExtUI::onPrintTimerStopped());
    state = STOPPED;
    stopTimestamp = millis();
    return true;
  }
  else return false;
}

bool Stopwatch::pause() {
  Stopwatch::debug(PSTR("pause"));

  if (isRunning()) {
    TERN_(EXTENSIBLE_UI, ExtUI::onPrintTimerPaused());
    state = PAUSED;
    stopTimestamp = millis();
    return true;
  }
  else return false;
}

bool Stopwatch::start() {
  Stopwatch::debug(PSTR("start"));

  TERN_(EXTENSIBLE_UI, ExtUI::onPrintTimerStarted());

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
  Stopwatch::debug(PSTR("resume"));

  reset();
  if ((accumulator = with_time)) state = RUNNING;
}

void Stopwatch::reset() {
  Stopwatch::debug(PSTR("reset"));

  state = STOPPED;
  startTimestamp = 0;
  stopTimestamp = 0;
  accumulator = 0;
}

millis_t Stopwatch::duration() {
  return accumulator + MS_TO_SEC((isRunning() ? millis() : stopTimestamp) - startTimestamp);
}

#if ENABLED(DEBUG_STOPWATCH)

  void Stopwatch::debug(const char func[]) {
    if (DEBUGGING(INFO)) {
      SERIAL_ECHOPGM("Stopwatch::");
      SERIAL_ECHOPGM_P(func);
      SERIAL_ECHOLNPGM("()");
    }
  }

#endif
