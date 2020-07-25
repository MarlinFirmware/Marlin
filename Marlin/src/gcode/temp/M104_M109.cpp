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

/**
 * gcode/temp/M104_M109.cpp
 *
 * Hotend target temperature control
 */

#include "../../inc/MarlinConfigPre.h"

#if EXTRUDERS

#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../module/motion.h"
#include "../../module/planner.h"
#include "../../lcd/ultralcd.h"

#include "../../MarlinCore.h" // for startOrResumeJob, etc.

#if ENABLED(PRINTJOB_TIMER_AUTOSTART)
  #include "../../module/printcounter.h"
  #if ENABLED(CANCEL_OBJECTS)
    #include "../../feature/cancel_object.h"
  #endif
#endif

#if ENABLED(SINGLENOZZLE)
  #include "../../module/tool_change.h"
#endif

/**
 * M104: Set Hotend Temperature target and return immediately
 *
 * Parameters:
 *  I<preset> : Material Preset index (if material presets are defined)
 *  T<index>  : Tool index. If omitted, applies to the active tool
 *  S<target> : The target temperature in current units
 */
void GcodeSuite::M104() {

  if (DEBUGGING(DRYRUN)) return;

  #if ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1
    constexpr int8_t target_extruder = 0;
  #else
    const int8_t target_extruder = get_target_extruder_from_command();
    if (target_extruder < 0) return;
  #endif

  bool got_temp = false;
  int16_t temp = 0;

  // Accept 'I' if temperature presets are defined
  #if PREHEAT_COUNT
    got_temp = parser.seenval('I');
    if (got_temp) {
      const uint8_t index = parser.value_byte();
      temp = ui.material_preset[_MIN(index, PREHEAT_COUNT - 1)].hotend_temp;
    }
  #endif

  // If no 'I' get the temperature from 'S'
  if (!got_temp) {
    got_temp = parser.seenval('S');
    if (got_temp) temp = parser.value_celsius();
  }

  if (got_temp) {
    #if ENABLED(SINGLENOZZLE_STANDBY_TEMP)
      singlenozzle_temp[target_extruder] = temp;
      if (target_extruder != active_extruder) return;
    #endif
    thermalManager.setTargetHotend(temp, target_extruder);

    #if ENABLED(DUAL_X_CARRIAGE)
      if (dxc_is_duplicating() && target_extruder == 0)
        thermalManager.setTargetHotend(temp ? temp + duplicate_extruder_temp_offset : 0, 1);
    #endif

    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      /**
       * Stop the timer at the end of print. Start is managed by 'heat and wait' M109.
       * Hotends use EXTRUDE_MINTEMP / 2 to allow nozzles to be put into hot standby
       * mode, for instance in a dual extruder setup, without affecting the running
       * print timer.
       */
      thermalManager.check_timer_autostart(false, true);
    #endif
  }

  TERN_(AUTOTEMP, planner.autotemp_M104_M109());
}

/**
 * M109: Set Hotend Temperature target and wait
 *
 * Parameters
 *  I<preset> : Material Preset index (if material presets are defined)
 *  T<index>  : Tool index. If omitted, applies to the active tool
 *  S<target> : The target temperature in current units. Wait for heating only.
 *  R<target> : The target temperature in current units. Wait for heating and cooling.
 *
 * With AUTOTEMP...
 *  F<factor> : Autotemp Scaling Factor. Set non-zero to enable Auto-temp.
 *  S<min>    : Minimum temperature, in current units.
 *  B<max>    : Maximum temperature, in current units.
 *
 * Examples
 *  M109 S100 : Set target to 100°. Wait until the hotend is at or above 100°.
 *  M109 R150 : Set target to 150°. Wait until the hotend gets close to 150°.
 *
 * With PRINTJOB_TIMER_AUTOSTART turning on heaters will start the print job timer
 *  (used by printingIsActive, etc.) and turning off heaters will stop the timer.
 */
void GcodeSuite::M109() {

  if (DEBUGGING(DRYRUN)) return;

  #if ENABLED(MIXING_EXTRUDER) && MIXING_VIRTUAL_TOOLS > 1
    constexpr int8_t target_extruder = 0;
  #else
    const int8_t target_extruder = get_target_extruder_from_command();
    if (target_extruder < 0) return;
  #endif

  bool got_temp = false;
  int16_t temp = 0;

  // Accept 'I' if temperature presets are defined
  #if PREHEAT_COUNT
    got_temp = parser.seenval('I');
    if (got_temp) {
      const uint8_t index = parser.value_byte();
      temp = ui.material_preset[_MIN(index, PREHEAT_COUNT - 1)].hotend_temp;
    }
  #endif

  // Get the temperature from 'S' or 'R'
  bool no_wait_for_cooling = false;
  if (!got_temp) {
    no_wait_for_cooling = parser.seenval('S');
    got_temp = no_wait_for_cooling || parser.seenval('R');
    if (got_temp) temp = int16_t(parser.value_celsius());
  }

  if (got_temp) {
    #if ENABLED(SINGLENOZZLE_STANDBY_TEMP)
      singlenozzle_temp[target_extruder] = temp;
      if (target_extruder != active_extruder) return;
    #endif
    thermalManager.setTargetHotend(temp, target_extruder);

    #if ENABLED(DUAL_X_CARRIAGE)
      if (dxc_is_duplicating() && target_extruder == 0)
        thermalManager.setTargetHotend(temp ? temp + duplicate_extruder_temp_offset : 0, 1);
    #endif

    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      /**
       * Use half EXTRUDE_MINTEMP to allow nozzles to be put into hot
       * standby mode, (e.g., in a dual extruder setup) without affecting
       * the running print timer.
       */
      thermalManager.check_timer_autostart(true, true);
    #endif

    #if HAS_DISPLAY
      if (thermalManager.isHeatingHotend(target_extruder) || !no_wait_for_cooling)
        thermalManager.set_heating_message(target_extruder);
    #endif
  }

  TERN_(AUTOTEMP, planner.autotemp_M104_M109());

  if (got_temp)
    (void)thermalManager.wait_for_hotend(target_extruder, no_wait_for_cooling);
}

#endif // EXTRUDERS
