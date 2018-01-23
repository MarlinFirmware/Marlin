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

#include "../gcode.h"
#include "../../module/configuration_store.h"
#include "../../inc/MarlinConfig.h"

#if NUM_SERIAL > 1
  #include "../../gcode/queue.h"
#endif

/**
 * M500: Store settings in EEPROM
 */
void GcodeSuite::M500() {
  (void)settings.save(
    #if ENABLED(EEPROM_CHITCHAT) && NUM_SERIAL > 1
      command_queue_port[cmd_queue_index_r]
    #endif
  );
}

/**
 * M501: Read settings from EEPROM
 */
void GcodeSuite::M501() {
  (void)settings.load(
    #if ENABLED(EEPROM_SETTINGS) && ENABLED(EEPROM_CHITCHAT) && NUM_SERIAL > 1
      command_queue_port[cmd_queue_index_r]
    #endif
  );
}

/**
 * M502: Revert to default settings
 */
void GcodeSuite::M502() {
  (void)settings.reset(
    #if ENABLED(EEPROM_CHITCHAT) && NUM_SERIAL > 1
      command_queue_port[cmd_queue_index_r]
    #endif
  );
}

#if DISABLED(DISABLE_M503)

  /**
   * M503: print settings currently in memory
   */
  void GcodeSuite::M503() {
    (void)settings.report(parser.seen('S') && !parser.value_bool()
      #if ADD_PORT_ARG
        , command_queue_port[cmd_queue_index_r]
      #endif
    );
  }

#endif // !DISABLE_M503

#if ENABLED(EEPROM_SETTINGS)
  /**
   * M504: Validate EEPROM Contents
   */
  void GcodeSuite::M504() {
    if (settings.validate()) {
      SERIAL_ECHO_START();
      SERIAL_ECHOLNPGM("EEPROM OK");
    }
  }
#endif
