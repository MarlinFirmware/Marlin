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
 * gcode/temp/M140_M190.cpp
 *
 * Bed target temperature control
 */

#include "../../inc/MarlinConfig.h"

#if HAS_HEATED_BED

#include "../gcode.h"
#include "../../module/temperature.h"
#include "../../lcd/marlinui.h"

/**
 * M140: Set bed temperature
 *
 *  I<index>  : Preset index (if material presets are defined)
 *  S<target> : The target temperature in current units
 */
void GcodeSuite::M140() {
  if (DEBUGGING(DRYRUN)) return;

  bool got_temp = false;
  int16_t temp = 0;

  // Accept 'I' if temperature presets are defined
  #if PREHEAT_COUNT
    got_temp = parser.seenval('I');
    if (got_temp) {
      const uint8_t index = parser.value_byte();
      temp = ui.material_preset[_MIN(index, PREHEAT_COUNT - 1)].bed_temp;
    }
  #endif

  // If no 'I' get the temperature from 'S'
  if (!got_temp) {
    got_temp = parser.seenval('S');
    if (got_temp) temp = parser.value_celsius();
  }

  if (got_temp) {
    thermalManager.setTargetBed(temp);

    #if ENABLED(PRINTJOB_TIMER_AUTOSTART)
      /**
       * Stop the timer at the end of print. Hotend, bed target, and chamber
       * temperatures need to be set below mintemp. Order of M140, M104, and M141
       * at the end of the print does not matter.
       */
      thermalManager.auto_job_check_timer(false, true);
    #endif
  }
}

/**
 * M190 - Set Bed Temperature target and wait
 *
 * Parameters:
 *  I<index>  : Preset index (if material presets are defined)
 *  S<target> : The target temperature in current units. Wait for heating only.
 *  R<target> : The target temperature in current units. Wait for heating and cooling.
 *
 * Examples:
 *  M190 S60 : Set target to 60°. Wait until the bed is at or above 60°.
 *  M190 R40 : Set target to 40°. Wait until the bed gets close to 40°.
 *
 * With PRINTJOB_TIMER_AUTOSTART turning on heaters will start the print job timer
 *  (used by printingIsActive, etc.) and turning off heaters will stop the timer.
 */
void GcodeSuite::M190() {
  if (DEBUGGING(DRYRUN)) return;

  bool got_temp = false;
  int16_t temp = 0;

  // Accept 'I' if temperature presets are defined
  #if PREHEAT_COUNT
    got_temp = parser.seenval('I');
    if (got_temp) {
      const uint8_t index = parser.value_byte();
      temp = ui.material_preset[_MIN(index, PREHEAT_COUNT - 1)].bed_temp;
    }
  #endif

  // Get the temperature from 'S' or 'R'
  bool no_wait_for_cooling = false;
  if (!got_temp) {
    no_wait_for_cooling = parser.seenval('S');
    got_temp = no_wait_for_cooling || parser.seenval('R');
    if (got_temp) temp = int16_t(parser.value_celsius());
  }

  if (!got_temp) return;

  thermalManager.setTargetBed(temp);

  TERN_(PRINTJOB_TIMER_AUTOSTART, thermalManager.auto_job_check_timer(true, false));

  ui.set_status_P(thermalManager.isHeatingBed() ? GET_TEXT(MSG_BED_HEATING) : GET_TEXT(MSG_BED_COOLING));

  thermalManager.wait_for_bed(no_wait_for_cooling);
}

#endif // HAS_HEATED_BED
