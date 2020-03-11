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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../gcode.h"
#include "../../inc/MarlinConfig.h"

#if ENABLED(EXTENDED_CAPABILITIES_REPORT)
  static void cap_line(PGM_P const name, bool ena=false) {
    SERIAL_ECHOPGM("Cap:");
    serialprintPGM(name);
    SERIAL_CHAR(':');
    SERIAL_ECHOLN(int(ena ? 1 : 0));
  }
#endif

/**
 * M115: Capabilities string and extended capabilities report
 *       If a capability is not reported, hosts should assume
 *       the capability is not present.
 */
void GcodeSuite::M115() {

  SERIAL_ECHOLNPGM(STR_M115_REPORT);

  #if ENABLED(EXTENDED_CAPABILITIES_REPORT)

    // PAREN_COMMENTS
    #if ENABLED(PAREN_COMMENTS)
      cap_line(PSTR("PAREN_COMMENTS"), true);
    #endif

    // QUOTED_STRINGS
    #if ENABLED(GCODE_QUOTED_STRINGS)
      cap_line(PSTR("QUOTED_STRINGS"), true);
    #endif

    // SERIAL_XON_XOFF
    cap_line(PSTR("SERIAL_XON_XOFF"), ENABLED(SERIAL_XON_XOFF));

    // BINARY_FILE_TRANSFER (M28 B1)
    cap_line(PSTR("BINARY_FILE_TRANSFER"), ENABLED(BINARY_FILE_TRANSFER));

    // EEPROM (M500, M501)
    cap_line(PSTR("EEPROM"), ENABLED(EEPROM_SETTINGS));

    // Volumetric Extrusion (M200)
    cap_line(PSTR("VOLUMETRIC"), DISABLED(NO_VOLUMETRICS));

    // AUTOREPORT_TEMP (M155)
    cap_line(PSTR("AUTOREPORT_TEMP"), ENABLED(AUTO_REPORT_TEMPERATURES));

    // PROGRESS (M530 S L, M531 <file>, M532 X L)
    cap_line(PSTR("PROGRESS"));

    // Print Job timer M75, M76, M77
    cap_line(PSTR("PRINT_JOB"), true);

    // AUTOLEVEL (G29)
    cap_line(PSTR("AUTOLEVEL"), ENABLED(HAS_AUTOLEVEL));

    // Z_PROBE (G30)
    cap_line(PSTR("Z_PROBE"), ENABLED(HAS_BED_PROBE));

    // MESH_REPORT (M420 V)
    cap_line(PSTR("LEVELING_DATA"), ENABLED(HAS_LEVELING));

    // BUILD_PERCENT (M73)
    cap_line(PSTR("BUILD_PERCENT"), ENABLED(LCD_SET_PROGRESS_MANUALLY));

    // SOFTWARE_POWER (M80, M81)
    cap_line(PSTR("SOFTWARE_POWER"), ENABLED(PSU_CONTROL));

    // CASE LIGHTS (M355)
    cap_line(PSTR("TOGGLE_LIGHTS"), ENABLED(HAS_CASE_LIGHT));

    cap_line(PSTR("CASE_LIGHT_BRIGHTNESS"), TERN(HAS_CASE_LIGHT, PWM_PIN(CASE_LIGHT_PIN), 0));

    // EMERGENCY_PARSER (M108, M112, M410, M876)
    cap_line(PSTR("EMERGENCY_PARSER"), ENABLED(EMERGENCY_PARSER));

    // PROMPT SUPPORT (M876)
    cap_line(PSTR("PROMPT_SUPPORT"), ENABLED(HOST_PROMPT_SUPPORT));

    // AUTOREPORT_SD_STATUS (M27 extension)
    cap_line(PSTR("AUTOREPORT_SD_STATUS"), ENABLED(AUTO_REPORT_SD_STATUS));

    // THERMAL_PROTECTION
    cap_line(PSTR("THERMAL_PROTECTION"), ENABLED(THERMALLY_SAFE));

    // MOTION_MODES (M80-M89)
    cap_line(PSTR("MOTION_MODES"), ENABLED(GCODE_MOTION_MODES));

    // CHAMBER_TEMPERATURE (M141, M191)
    cap_line(PSTR("CHAMBER_TEMPERATURE"), ENABLED(HAS_HEATED_CHAMBER));

  #endif // EXTENDED_CAPABILITIES_REPORT
}
