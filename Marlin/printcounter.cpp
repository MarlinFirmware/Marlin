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
#include "printcounter.h"

PrintCounter::PrintCounter(): super() {
  this->loadStats();
}

uint16_t PrintCounter::deltaDuration() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("deltaDuration"));
  #endif

  uint16_t tmp = this->lastDuration;
  this->lastDuration = this->duration();
  return this->lastDuration - tmp;
}

bool PrintCounter::isLoaded() {
  return this->loaded;
}

void PrintCounter::initStats() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("initStats"));
  #endif

  this->loaded = true;
  this->data = { 0, 0, 0, 0 };

  this->saveStats();
  eeprom_write_byte((uint8_t *) this->address, 0x16);
}

void PrintCounter::loadStats() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("loadStats"));
  #endif

  // Checks if the EEPROM block is initialized
  if (eeprom_read_byte((uint8_t *) this->address) != 0x16) this->initStats();
  else eeprom_read_block(&this->data, (void *)(this->address + sizeof(uint8_t)), sizeof(printStatistics));

  this->loaded = true;
}

void PrintCounter::saveStats() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("saveStats"));
  #endif

  // Refuses to save data is object is not loaded
  if (!this->isLoaded()) return;

  eeprom_update_block(&this->data, (void *)(this->address + sizeof(uint8_t)), sizeof(printStatistics));
}

void PrintCounter::showStats() {
  SERIAL_ECHOPGM("Print statistics: Total: ");
  SERIAL_ECHO(this->data.totalPrints);

  SERIAL_ECHOPGM(", Finished: ");
  SERIAL_ECHO(this->data.finishedPrints);

  SERIAL_ECHOPGM(", Failed: ");
  SERIAL_ECHO(this->data.totalPrints - this->data.finishedPrints
    - (this->isRunning() || this->isPaused()) ? 1 : 0); // Removes 1 from failures with an active counter

  uint32_t t = this->data.printTime /60;
  SERIAL_ECHOPGM(", Total print time: ");
  SERIAL_ECHO(t / 60);

  SERIAL_ECHOPGM("h ");
  SERIAL_ECHO(t % 60);

  SERIAL_ECHOPGM("min");

  #if ENABLED(DEBUG_PRINTCOUNTER)
    SERIAL_ECHOPGM(" (");
    SERIAL_ECHO(this->data.printTime);
    SERIAL_ECHOPGM(")");
  #endif

  // @todo longestPrint missing implementation
  SERIAL_EOL;
}

void PrintCounter::tick() {
  if (!this->isRunning()) return;

  static uint32_t update_before = millis(),
                  eeprom_before = millis();

  uint32_t now = millis();

  // Trying to get the amount of calculations down to the bare min
  const static uint16_t i = this->updateInterval * 1000;

  if (now - update_before >= i) {
    #if ENABLED(DEBUG_PRINTCOUNTER)
      PrintCounter::debug(PSTR("tick"));
    #endif

    this->data.printTime += this->deltaDuration();
    update_before = now;
  }

  // Trying to get the amount of calculations down to the bare min
  const static uint16_t j = this->saveInterval * 1000;

  if (now - eeprom_before >= j) {
    eeprom_before = now;
    this->saveStats();
  }
}

void PrintCounter::start() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("start"));
  #endif

  if (!this->isPaused()) this->data.totalPrints++;
  super::start();
}

void PrintCounter::stop() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("stop"));
  #endif

  if (!this->isRunning()) return;
  super::stop();

  this->data.finishedPrints++;
  this->data.printTime += this->deltaDuration();
  this->saveStats();
}

void PrintCounter::reset() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("stop"));
  #endif

  this->lastDuration = 0;
  super::reset();
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
