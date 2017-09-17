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
#include "../../inc/MarlinConfig.h"

/**
 * M115: Capabilities string
 */
void GcodeSuite::M115() {
  SERIAL_PROTOCOLLNPGM(MSG_M115_REPORT);

  #if ENABLED(EXTENDED_CAPABILITIES_REPORT)

    // EEPROM (M500, M501)
    #if ENABLED(EEPROM_SETTINGS)
      SERIAL_PROTOCOLLNPGM("Cap:EEPROM:1");
    #else
      SERIAL_PROTOCOLLNPGM("Cap:EEPROM:0");
    #endif

    // AUTOREPORT_TEMP (M155)
    #if ENABLED(AUTO_REPORT_TEMPERATURES)
      SERIAL_PROTOCOLLNPGM("Cap:AUTOREPORT_TEMP:1");
    #else
      SERIAL_PROTOCOLLNPGM("Cap:AUTOREPORT_TEMP:0");
    #endif

    // PROGRESS (M530 S L, M531 <file>, M532 X L)
    SERIAL_PROTOCOLLNPGM("Cap:PROGRESS:0");

    // Print Job timer M75, M76, M77
    SERIAL_PROTOCOLLNPGM("Cap:PRINT_JOB:1");

    // AUTOLEVEL (G29)
    #if HAS_AUTOLEVEL
      SERIAL_PROTOCOLLNPGM("Cap:AUTOLEVEL:1");
    #else
      SERIAL_PROTOCOLLNPGM("Cap:AUTOLEVEL:0");
    #endif

    // Z_PROBE (G30)
    #if HAS_BED_PROBE
      SERIAL_PROTOCOLLNPGM("Cap:Z_PROBE:1");
    #else
      SERIAL_PROTOCOLLNPGM("Cap:Z_PROBE:0");
    #endif

    // MESH_REPORT (M420 V)
    #if HAS_LEVELING
      SERIAL_PROTOCOLLNPGM("Cap:LEVELING_DATA:1");
    #else
      SERIAL_PROTOCOLLNPGM("Cap:LEVELING_DATA:0");
    #endif

    // SOFTWARE_POWER (M80, M81)
    #if HAS_POWER_SWITCH
      SERIAL_PROTOCOLLNPGM("Cap:SOFTWARE_POWER:1");
    #else
      SERIAL_PROTOCOLLNPGM("Cap:SOFTWARE_POWER:0");
    #endif

    // CASE LIGHTS (M355)
    #if HAS_CASE_LIGHT
      SERIAL_PROTOCOLLNPGM("Cap:TOGGLE_LIGHTS:1");
      if (USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN)) {
        SERIAL_PROTOCOLLNPGM("Cap:CASE_LIGHT_BRIGHTNESS:1");
      }
      else
        SERIAL_PROTOCOLLNPGM("Cap:CASE_LIGHT_BRIGHTNESS:0");
    #else
      SERIAL_PROTOCOLLNPGM("Cap:TOGGLE_LIGHTS:0");
      SERIAL_PROTOCOLLNPGM("Cap:CASE_LIGHT_BRIGHTNESS:0");
    #endif

    // EMERGENCY_PARSER (M108, M112, M410)
    #if ENABLED(EMERGENCY_PARSER)
      SERIAL_PROTOCOLLNPGM("Cap:EMERGENCY_PARSER:1");
    #else
      SERIAL_PROTOCOLLNPGM("Cap:EMERGENCY_PARSER:0");
    #endif

  #endif // EXTENDED_CAPABILITIES_REPORT
}
