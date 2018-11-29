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

#include "../inc/MarlinConfig.h"

#if DISABLED(PRINTCOUNTER)

#include "../libs/stopwatch.h"
Stopwatch print_job_timer;      // Global Print Job Timer instance

#else // PRINTCOUNTER

#include "printcounter.h"
#include "../Marlin.h"
#include "../HAL/shared/persistent_store_api.h"

PrintCounter print_job_timer;   // Global Print Job Timer instance

printStatistics PrintCounter::data;

const PrintCounter::promdress PrintCounter::address = STATS_EEPROM_ADDRESS;

millis_t PrintCounter::lastDuration;
bool PrintCounter::loaded = false;

millis_t PrintCounter::deltaDuration() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    debug(PSTR("deltaDuration"));
  #endif

  millis_t tmp = lastDuration;
  lastDuration = duration();
  return lastDuration - tmp;
}

void PrintCounter::incFilamentUsed(float const &amount) {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    debug(PSTR("incFilamentUsed"));
  #endif

  // Refuses to update data if object is not loaded
  if (!isLoaded()) return;

  data.filamentUsed += amount; // mm
}

void PrintCounter::initStats() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    debug(PSTR("initStats"));
  #endif

  loaded = true;
  data = { 0, 0, 0, 0, 0.0 };

  saveStats();
  persistentStore.access_start();
  persistentStore.write_data(address, (uint8_t)0x16);
  persistentStore.access_finish();
}

void PrintCounter::loadStats() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    debug(PSTR("loadStats"));
  #endif

  // Check if the EEPROM block is initialized
  uint8_t value = 0;
  persistentStore.access_start();
  persistentStore.read_data(address, &value, sizeof(uint8_t));
  if (value != 0x16)
    initStats();
  else
    persistentStore.read_data(address + sizeof(uint8_t), (uint8_t*)&data, sizeof(printStatistics));
  persistentStore.access_finish();
  loaded = true;
}

void PrintCounter::saveStats() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    debug(PSTR("saveStats"));
  #endif

  // Refuses to save data if object is not loaded
  if (!isLoaded()) return;

  // Saves the struct to EEPROM
  persistentStore.access_start();
  persistentStore.write_data(address + sizeof(uint8_t), (uint8_t*)&data, sizeof(printStatistics));
  persistentStore.access_finish();
}

void PrintCounter::showStats() {
  char buffer[21];

  SERIAL_ECHOPGM(MSG_STATS);

  SERIAL_ECHOPGM("Prints: ");
  SERIAL_ECHO(data.totalPrints);

  SERIAL_ECHOPGM(", Finished: ");
  SERIAL_ECHO(data.finishedPrints);

  SERIAL_ECHOPGM(", Failed: "); // Note: Removes 1 from failures with an active counter
  SERIAL_ECHO(data.totalPrints - data.finishedPrints
    - ((isRunning() || isPaused()) ? 1 : 0));

  SERIAL_EOL();
  SERIAL_ECHOPGM(MSG_STATS);

  duration_t elapsed = data.printTime;
  elapsed.toString(buffer);

  SERIAL_ECHOPGM("Total time: ");
  SERIAL_ECHO(buffer);

  #if ENABLED(DEBUG_PRINTCOUNTER)
    SERIAL_ECHOPGM(" (");
    SERIAL_ECHO(data.printTime);
    SERIAL_CHAR(')');
  #endif

  elapsed = data.longestPrint;
  elapsed.toString(buffer);

  SERIAL_ECHOPGM(", Longest job: ");
  SERIAL_ECHO(buffer);

  #if ENABLED(DEBUG_PRINTCOUNTER)
    SERIAL_ECHOPGM(" (");
    SERIAL_ECHO(data.longestPrint);
    SERIAL_CHAR(')');
  #endif

  SERIAL_EOL();
  SERIAL_ECHOPGM(MSG_STATS);

  SERIAL_ECHOPGM("Filament used: ");
  SERIAL_ECHO(data.filamentUsed / 1000);
  SERIAL_CHAR('m');

  SERIAL_EOL();
}

void PrintCounter::tick() {
  if (!isRunning()) return;

  millis_t now = millis();

  static uint32_t update_next; // = 0
  if (ELAPSED(now, update_next)) {
    #if ENABLED(DEBUG_PRINTCOUNTER)
      debug(PSTR("tick"));
    #endif
    data.printTime += deltaDuration();
    update_next = now + updateInterval * 1000;
  }

  static uint32_t eeprom_next; // = 0
  if (ELAPSED(now, eeprom_next)) {
    eeprom_next = now + saveInterval * 1000;
    saveStats();
  }
}

// @Override
bool PrintCounter::start() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    debug(PSTR("start"));
  #endif

  bool paused = isPaused();

  if (super::start()) {
    if (!paused) {
      data.totalPrints++;
      lastDuration = 0;
    }
    return true;
  }

  return false;
}

// @Override
bool PrintCounter::stop() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    debug(PSTR("stop"));
  #endif

  if (super::stop()) {
    data.finishedPrints++;
    data.printTime += deltaDuration();

    if (duration() > data.longestPrint)
      data.longestPrint = duration();

    saveStats();
    return true;
  }
  else return false;
}

// @Override
void PrintCounter::reset() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    debug(PSTR("stop"));
  #endif

  super::reset();
  lastDuration = 0;
}

#if ENABLED(DEBUG_PRINTCOUNTER)

  void PrintCounter::debug(const char func[]) {
    if (DEBUGGING(INFO)) {
      SERIAL_ECHOPGM("PrintCounter::");
      serialprintPGM(func);
      SERIAL_ECHOLNPGM("()");
    }
  }
#endif

#endif // PRINTCOUNTER
