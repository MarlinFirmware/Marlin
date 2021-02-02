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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../inc/MarlinConfig.h"

#if HAS_PID_HEATING

#include "../gcode.h"
#include "../../lcd/marlinui.h"
#include "../../module/temperature.h"

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extui/ui_api.h"
#endif

/**
 * M303: PID relay autotune
 *
 *  S<temperature>  Set the target temperature. (Default: 150C / 70C)
 *  E<extruder>     Extruder number to tune, or -1 for the bed. (Default: E0)
 *  C<cycles>       Number of times to repeat the procedure. (Minimum: 3, Default: 5)
 *  U<bool>         Flag to apply the result to the current PID values
 *
 * With PID_DEBUG:
 *  D               Toggle PID debugging and EXIT without further action.
 */

#if ENABLED(PID_DEBUG)
  bool pid_debug_flag = 0;
#endif

void GcodeSuite::M303() {

  #if ENABLED(PID_DEBUG)
    if (parser.seen('D')) {
      pid_debug_flag = !pid_debug_flag;
      SERIAL_ECHO_START();
      SERIAL_ECHOPGM("PID Debug ");
      serialprintln_onoff(pid_debug_flag);
      return;
    }
  #endif

  #define SI TERN(PIDTEMPBED, H_BED, H_E0)
  #define EI TERN(PIDTEMP, HOTENDS - 1, H_BED)
  const heater_id_t e = (heater_id_t)parser.intval('E');
  if (!WITHIN(e, SI, EI)) {
    SERIAL_ECHOLNPGM(STR_PID_BAD_EXTRUDER_NUM);
    TERN_(EXTENSIBLE_UI, ExtUI::onPidTuning(ExtUI::result_t::PID_BAD_EXTRUDER_NUM));
    return;
  }

  const int c = parser.intval('C', 5);
  const bool u = parser.boolval('U');
  const int16_t temp = parser.celsiusval('S', e < 0 ? PREHEAT_1_TEMP_BED : PREHEAT_1_TEMP_HOTEND);

  #if DISABLED(BUSY_WHILE_HEATING)
    KEEPALIVE_STATE(NOT_BUSY);
  #endif

  LCD_MESSAGEPGM(MSG_PID_AUTOTUNE);
  thermalManager.PID_autotune(temp, e, c, u);
  ui.reset_status();
}

#endif // HAS_PID_HEATING
