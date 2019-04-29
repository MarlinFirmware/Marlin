/**
 * Marlin 3D Printer Firmware
 * Copyright (C) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extensible_ui/ui_api.h"
#endif

#include "printcounter.h"
#include "../Marlin.h"
#include "../HAL/shared/persistent_store_api.h"

#if HAS_BUZZER && SERVICE_WARNING_BUZZES > 0
  #include "../libs/buzzer.h"
#endif

// Service intervals
#if HAS_SERVICE_INTERVALS
  #if SERVICE_INTERVAL_1 > 0
    #define SERVICE_INTERVAL_SEC_1   (3600UL * SERVICE_INTERVAL_1)
  #else
    #define SERVICE_INTERVAL_SEC_1   (3600UL * 100)
  #endif
  #if SERVICE_INTERVAL_2 > 0
    #define SERVICE_INTERVAL_SEC_2   (3600UL * SERVICE_INTERVAL_2)
  #else
    #define SERVICE_INTERVAL_SEC_2   (3600UL * 100)
  #endif
  #if SERVICE_INTERVAL_3 > 0
    #define SERVICE_INTERVAL_SEC_3   (3600UL * SERVICE_INTERVAL_3)
  #else
    #define SERVICE_INTERVAL_SEC_3   (3600UL * 100)
  #endif
#endif

PrintCounter print_job_timer;   // Global Print Job Timer instance

printStatistics PrintCounter::data;

const PrintCounter::eeprom_address_t PrintCounter::address = STATS_EEPROM_ADDRESS;

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
  data = { 0, 0, 0, 0, 0.0
    #if HAS_SERVICE_INTERVALS
      #if SERVICE_INTERVAL_1 > 0
        , SERVICE_INTERVAL_SEC_1
      #endif
      #if SERVICE_INTERVAL_2 > 0
        , SERVICE_INTERVAL_SEC_2
      #endif
      #if SERVICE_INTERVAL_3 > 0
        , SERVICE_INTERVAL_SEC_3
      #endif
    #endif
  };

  saveStats();
  persistentStore.access_start();
  persistentStore.write_data(address, (uint8_t)0x16);
  persistentStore.access_finish();
}

#if HAS_SERVICE_INTERVALS
  inline void _print_divider() { SERIAL_ECHO_MSG("============================================="); }
  inline bool _service_warn(const char * const msg) {
    _print_divider();
    SERIAL_ECHO_START();
    serialprintPGM(msg);
    SERIAL_ECHOLNPGM("!");
    _print_divider();
    return true;
  }
#endif

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

  #if HAS_SERVICE_INTERVALS
    bool doBuzz = false;
    #if SERVICE_INTERVAL_1 > 0
      if (data.nextService1 == 0) doBuzz = _service_warn(PSTR(" " SERVICE_NAME_1));
    #endif
    #if SERVICE_INTERVAL_2 > 0
      if (data.nextService2 == 0) doBuzz = _service_warn(PSTR(" " SERVICE_NAME_2));
    #endif
    #if SERVICE_INTERVAL_3 > 0
      if (data.nextService3 == 0) doBuzz = _service_warn(PSTR(" " SERVICE_NAME_3));
    #endif
    #if HAS_BUZZER && SERVICE_WARNING_BUZZES > 0
      if (doBuzz) for (int i = 0; i < SERVICE_WARNING_BUZZES; i++) BUZZ(200, 404);
    #endif
  #endif // HAS_SERVICE_INTERVALS
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

  #if ENABLED(EXTENSIBLE_UI)
    ExtUI::onConfigurationStoreWritten(true);
  #endif
}

#if HAS_SERVICE_INTERVALS
  inline void _service_when(char buffer[], const char * const msg, const uint32_t when) {
    duration_t elapsed = when;
    elapsed.toString(buffer);
    SERIAL_ECHOPGM(MSG_STATS);
    serialprintPGM(msg);
    SERIAL_ECHOLNPAIR(" in ", buffer);
  }
#endif

void PrintCounter::showStats() {
  char buffer[21];

  SERIAL_ECHOPGM(MSG_STATS);
  SERIAL_ECHOLNPAIR(
    "Prints: ", data.totalPrints,
    ", Finished: ", data.finishedPrints,
    ", Failed: ", data.totalPrints - data.finishedPrints
                    - ((isRunning() || isPaused()) ? 1 : 0) // Remove 1 from failures with an active counter
  );

  SERIAL_ECHOPGM(MSG_STATS);
  duration_t elapsed = data.printTime;
  elapsed.toString(buffer);
  SERIAL_ECHOPAIR("Total time: ", buffer);
  #if ENABLED(DEBUG_PRINTCOUNTER)
    SERIAL_ECHOPAIR(" (", data.printTime);
    SERIAL_CHAR(')');
  #endif

  elapsed = data.longestPrint;
  elapsed.toString(buffer);
  SERIAL_ECHOPAIR(", Longest job: ", buffer);
  #if ENABLED(DEBUG_PRINTCOUNTER)
    SERIAL_ECHOPAIR(" (", data.longestPrint);
    SERIAL_CHAR(')');
  #endif

  SERIAL_ECHOPAIR("\n" MSG_STATS "Filament used: ", data.filamentUsed / 1000);
  SERIAL_CHAR('m');
  SERIAL_EOL();

  #if SERVICE_INTERVAL_1 > 0
    _service_when(buffer, PSTR(SERVICE_NAME_1), data.nextService1);
  #endif
  #if SERVICE_INTERVAL_2 > 0
    _service_when(buffer, PSTR(SERVICE_NAME_2), data.nextService2);
  #endif
  #if SERVICE_INTERVAL_3 > 0
    _service_when(buffer, PSTR(SERVICE_NAME_3), data.nextService3);
  #endif
}

void PrintCounter::tick() {
  if (!isRunning()) return;

  millis_t now = millis();

  static uint32_t update_next; // = 0
  if (ELAPSED(now, update_next)) {
    #if ENABLED(DEBUG_PRINTCOUNTER)
      debug(PSTR("tick"));
    #endif
    millis_t delta = deltaDuration();
    data.printTime += delta;

    #if SERVICE_INTERVAL_1 > 0
      data.nextService1 -= MIN(delta, data.nextService1);
    #endif
    #if SERVICE_INTERVAL_2 > 0
      data.nextService2 -= MIN(delta, data.nextService2);
    #endif
    #if SERVICE_INTERVAL_3 > 0
      data.nextService3 -= MIN(delta, data.nextService3);
    #endif

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

#if HAS_SERVICE_INTERVALS

  void PrintCounter::resetServiceInterval(const int index) {
    switch (index) {
      #if SERVICE_INTERVAL_1 > 0
        case 1: data.nextService1 = SERVICE_INTERVAL_SEC_1;
      #endif
      #if SERVICE_INTERVAL_2 > 0
        case 2: data.nextService2 = SERVICE_INTERVAL_SEC_2;
      #endif
      #if SERVICE_INTERVAL_3 > 0
        case 3: data.nextService3 = SERVICE_INTERVAL_SEC_3;
      #endif
    }
    saveStats();
  }

  bool PrintCounter::needsService(const int index) {
    switch (index) {
      #if SERVICE_INTERVAL_1 > 0
        case 1: return data.nextService1 == 0;
      #endif
      #if SERVICE_INTERVAL_2 > 0
        case 2: return data.nextService2 == 0;
      #endif
      #if SERVICE_INTERVAL_3 > 0
        case 3: return data.nextService3 == 0;
      #endif
      default: return false;
    }
  }

#endif // HAS_SERVICE_INTERVALS

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
