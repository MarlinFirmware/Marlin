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

#include "../gcode.h"
#include "../../module/configuration_store.h"
#include "../../core/serial.h"
#include "../../inc/MarlinConfig.h"
  #ifdef LONGER3D
  #include "../../longer/config_storedata.h"
  #include "../../longer/LGT_MACRO.h"
 // #include "../../longer/W25Qxx.h"
  extern config_storedata ConfigSettings;
  extern ARGUMENTS re_settings;
 // extern  W25Qxx W25QxxFlash;
  #endif
/**
 * M500: Store settings in EEPROM
 */
void GcodeSuite::M500() {
  (void)settings.save();
  #ifdef LONGER3D
  FLASH_WRITE_VAR(SAVE_ADDR_SETTINGS, re_settings);
  ConfigSettings.settingsLoad();
  #endif
}

/**
 * M501: Read settings from EEPROM
 */
void GcodeSuite::M501() {
  (void)settings.load();
  #ifdef LONGER3D
   FLASH_READ_VAR(SAVE_ADDR_SETTINGS, re_settings);
   ConfigSettings.settingsLoad();
  #endif
}

/**
 * M502: Revert to default settings
 */
void GcodeSuite::M502() {
  (void)settings.reset();
  #ifdef LONGER3D
  ConfigSettings.restoreDefaultSettings();
  FLASH_WRITE_VAR(SAVE_ADDR_SETTINGS, re_settings);
  ConfigSettings.settingsLoad();
  #endif
}

#if DISABLED(DISABLE_M503)

  /**
   * M503: print settings currently in memory
   */
  void GcodeSuite::M503() {
    (void)settings.report(parser.boolval('S', true));
  }

#endif // !DISABLE_M503

#if ENABLED(EEPROM_SETTINGS)
  /**
   * M504: Validate EEPROM Contents
   */
  void GcodeSuite::M504() {
    if (settings.validate())
      SERIAL_ECHO_MSG("EEPROM OK");
  }
#endif
