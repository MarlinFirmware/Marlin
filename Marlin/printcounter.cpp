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

PrintCounter::PrintCounter(): super() {}

void PrintCounter::tick() {
  if (!this->isRunning()) return;

  static uint32_t update_before = millis(),
                  eeprom_before = millis();

  uint32_t now = millis();

  // Trying to get the amount of calculations down to the bare min
  const static uint16_t i = this->updateInterval * 1000;

  if (now - update_before >= i) {
    //this->addToTimeCounter((uint16_t) (now - update_before) / 1000);
    update_before = now;
    PrintCounter::debug(PSTR("tick1"));
  }

  // Trying to get the amount of calculations down to the bare min
  const static uint16_t j = this->saveInterval * 1000;

  if (now - eeprom_before >= j) {
    eeprom_before = now;
    this->save();
  }
}

void PrintCounter::load() {
  uint16_t pos = this->addr;

  this->data.successPrints= eeprom_read_word ((uint16_t*) pos);
  this->data.failedPrints = eeprom_read_word ((uint16_t*) pos);
  this->data.printTime    = eeprom_read_dword((uint32_t*) pos);
  this->data.longestPrint = eeprom_read_dword((uint32_t*) pos);

  SERIAL_ECHOPGM("successPrints: ");
  SERIAL_ECHOLN(this->data.successPrints);

  SERIAL_ECHOPGM("failedPrints: ");
  SERIAL_ECHOLN(this->data.failedPrints);

  SERIAL_ECHOPGM("printTime: ");
  SERIAL_ECHOLN(this->data.printTime);

  SERIAL_ECHOPGM("longestPrint: ");
  SERIAL_ECHOLN(this->data.longestPrint);
}

void PrintCounter::save() {
  #if ENABLED(DEBUG_PRINTCOUNTER)
    PrintCounter::debug(PSTR("save"));
  #endif

  uint16_t pos = this->addr;

  eeprom_write_word ((uint16_t*) pos, this->data.successPrints);
  eeprom_write_word ((uint16_t*) pos, this->data.failedPrints);
  eeprom_write_dword((uint32_t*) pos, this->data.printTime);
  eeprom_write_dword((uint32_t*) pos, this->data.longestPrint);
}

void PrintCounter::start() {
  super::start();
  this->load();
}

void PrintCounter::stop() {
  super::stop();
  this->save();
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
