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
#include <avr/eeprom.h>

PrintCounter::PrintCounter(): super() {
  this->loadStats();
}

bool PrintCounter::isLoaded() {
  return this->loaded;
}

void PrintCounter::initStats() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("initStats"));
  #endif

  this->loaded = true;
  this->data = {
    0, 0, 0, 0
  };

  this->saveStats();
  eeprom_write_byte((uint8_t*) this->addr, 0x16);
}

void PrintCounter::loadStats() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("loadStats"));
  #endif

  uint16_t addr = this->addr;

  // Checks if the EEPROM block is initialized
  if (eeprom_read_byte((uint8_t*) addr) != 0x16) this->initStats();

  else {
    // Skip the magic header byte
    addr += sizeof(uint8_t);

    // @todo This section will need rewrite once the ConfigurationStore
    // and/or PersistentStorage object comes along.
    this->data.totalPrints = eeprom_read_word((uint16_t*) addr);
    addr += sizeof(uint16_t);

    this->data.finishedPrints = eeprom_read_word((uint16_t*) addr);
    addr += sizeof(uint16_t);

    this->data.printTime = eeprom_read_dword((uint32_t*) addr);
    addr += sizeof(uint32_t);

    this->data.longestPrint = eeprom_read_dword((uint32_t*) addr);
  }

  this->loaded = true;
}

void PrintCounter::saveStats() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("saveStats"));
  #endif

  // Refuses to save data is object is not loaded
  if (!this->isLoaded()) return;

  // Skip the magic header byte
  uint16_t addr = this->addr + sizeof(uint8_t);

  // @todo This section will need rewrite once the ConfigurationStore
  // and/or PersistentStorage object comes along.
  eeprom_write_word ((uint16_t*) addr, this->data.totalPrints);
  addr += sizeof(uint16_t);

  eeprom_write_word ((uint16_t*) addr, this->data.finishedPrints);
  addr += sizeof(uint16_t);

  eeprom_write_dword((uint32_t*) addr, this->data.printTime);
  addr += sizeof(uint32_t);

  eeprom_write_dword((uint32_t*) addr, this->data.longestPrint);
}

void PrintCounter::showStats() {
  SERIAL_ECHOPGM("Print statistics: Total: ");
  SERIAL_ECHO(this->data.totalPrints);

  SERIAL_ECHOPGM(", Finished: ");
  SERIAL_ECHO(this->data.finishedPrints);

  SERIAL_ECHOPGM(", Failed: ");
  SERIAL_ECHO(this->data.totalPrints - this->data.finishedPrints);

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

    uint16_t t = this->duration();;
    this->data.printTime += t - this->lastUpdate;
    this->lastUpdate = t;
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

  super::stop();
  this->data.finishedPrints++;
  this->data.printTime += this->duration() - this->lastUpdate;
  this->saveStats();
}

void PrintCounter::reset() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("stop"));
  #endif

  this->lastUpdate = 0;
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
