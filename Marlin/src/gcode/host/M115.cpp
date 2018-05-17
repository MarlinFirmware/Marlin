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

#if NUM_SERIAL > 1
  #include "../../gcode/queue.h"
#endif

#if ENABLED(EXTENDED_CAPABILITIES_REPORT)
  static void cap_line(const char * const name, bool ena=false) {
    SERIAL_PROTOCOLPGM("Cap:");
    serialprintPGM(name);
    SERIAL_CHAR(':');
    SERIAL_PROTOCOLLN(int(ena ? 1 : 0));
  }
#endif

/**
 * M115: Capabilities string
 */
void GcodeSuite::M115() {
  #if NUM_SERIAL > 1
    const int8_t port = command_queue_port[cmd_queue_index_r];
    #define CAPLINE(STR,...) cap_line(PSTR(STR), port, __VA_ARGS__)
  #else
    #define CAPLINE(STR,...) cap_line(PSTR(STR), __VA_ARGS__)
  #endif

  SERIAL_PROTOCOLLNPGM_P(port, MSG_M115_REPORT);

  #if ENABLED(EXTENDED_CAPABILITIES_REPORT)

    // SERIAL_XON_XOFF
    cap_line(PSTR("SERIAL_XON_XOFF")
      #if ENABLED(SERIAL_XON_XOFF)
        , true
      #endif
    );

    // EEPROM (M500, M501)
    cap_line(PSTR("EEPROM")
      #if ENABLED(EEPROM_SETTINGS)
        , true
      #endif
    );

    // Volumetric Extrusion (M200)
    cap_line(PSTR("VOLUMETRIC")
      #if DISABLED(NO_VOLUMETRICS)
        , true
      #endif
    );

    // AUTOREPORT_TEMP (M155)
    cap_line(PSTR("AUTOREPORT_TEMP")
      #if ENABLED(AUTO_REPORT_TEMPERATURES)
        , true
      #endif
    );

    // PROGRESS (M530 S L, M531 <file>, M532 X L)
    cap_line(PSTR("PROGRESS"));

    // Print Job timer M75, M76, M77
    cap_line(PSTR("PRINT_JOB"), true);

    // AUTOLEVEL (G29)
    cap_line(PSTR("AUTOLEVEL")
      #if HAS_AUTOLEVEL
        , true
      #endif
    );

    // Z_PROBE (G30)
    cap_line(PSTR("Z_PROBE")
      #if HAS_BED_PROBE
        , true
      #endif
    );

    // MESH_REPORT (M420 V)
    cap_line(PSTR("LEVELING_DATA")
      #if HAS_LEVELING
        , true
      #endif
    );

    // BUILD_PERCENT (M73)
    cap_line(PSTR("BUILD_PERCENT")
      #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
        , true
      #endif
    );

    // SOFTWARE_POWER (M80, M81)
    cap_line(PSTR("SOFTWARE_POWER")
      #if HAS_POWER_SWITCH
        , true
      #endif
    );

    // CASE LIGHTS (M355)
    cap_line(PSTR("TOGGLE_LIGHTS")
      #if HAS_CASE_LIGHT
        , true
      #endif
    );
    cap_line(PSTR("CASE_LIGHT_BRIGHTNESS")
      #if HAS_CASE_LIGHT
        , USEABLE_HARDWARE_PWM(CASE_LIGHT_PIN)
      #endif
    );

    // EMERGENCY_PARSER (M108, M112, M410)
    cap_line(PSTR("EMERGENCY_PARSER")
      #if ENABLED(EMERGENCY_PARSER)
        , true
      #endif
    );

    // AUTOREPORT_SD_STATUS (M27 extension)
    cap_line(PSTR("AUTOREPORT_SD_STATUS")
      #if ENABLED(AUTO_REPORT_SD_STATUS)
        , true
      #endif
    );

    // THERMAL_PROTECTION
    cap_line(PSTR("THERMAL_PROTECTION")
      #if ENABLED(THERMAL_PROTECTION_HOTENDS) && ENABLED(THERMAL_PROTECTION_BED)
        , true
      #endif
    );

  #endif // EXTENDED_CAPABILITIES_REPORT
}
