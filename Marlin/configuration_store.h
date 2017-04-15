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

#ifndef CONFIGURATION_STORE_H
#define CONFIGURATION_STORE_H

#include "MarlinConfig.h"

class MarlinSettings {
  public:
    MarlinSettings() { }

    static void reset();
    static bool save();

    #if ENABLED(EEPROM_SETTINGS)
      static bool load();
    #else
      FORCE_INLINE
      static bool load() { reset(); report(); return true; }
    #endif

    #if DISABLED(DISABLE_M503)
      static void report(bool forReplay=false);
    #else
      FORCE_INLINE
      static void report(bool forReplay=false) { }
    #endif

  private:
    static void postprocess();

    #if ENABLED(EEPROM_SETTINGS)
      static uint16_t eeprom_checksum;
      static bool eeprom_read_error, eeprom_write_error;
      static void write_data(int &pos, const uint8_t* value, uint16_t size);
      static void read_data(int &pos, uint8_t* value, uint16_t size);
    #endif
};

extern MarlinSettings settings;

#endif // CONFIGURATION_STORE_H
