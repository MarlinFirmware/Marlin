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

/**
 * feature/runout.cpp - Runout sensor support
 */

#include "../inc/MarlinConfigPre.h"

#if HAS_FILAMENT_SENSOR

#include "runout.h"

FilamentMonitor runout;

bool FilamentMonitorBase::enabled = true,
     FilamentMonitorBase::filament_ran_out;  // = false

#if ENABLED(HOST_ACTION_COMMANDS)
  bool FilamentMonitorBase::host_handling; // = false
#endif

#if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
  //#define DEBUG_TOOLCHANGE_MIGRATION_FEATURE
  #include "../module/tool_change.h"
#endif

/**
 * Called by FilamentSensorSwitch::run when filament is detected.
 * Called by FilamentSensorEncoder::block_completed when motion is detected.
 */
void FilamentSensorBase::filament_present(const uint8_t extruder) {
  runout.filament_present(extruder); // calls response.filament_present(extruder)
}

#if HAS_FILAMENT_RUNOUT_DISTANCE
  float RunoutResponseDelayed::runout_distance_mm = FILAMENT_RUNOUT_DISTANCE_MM;
  volatile float RunoutResponseDelayed::runout_mm_countdown[EXTRUDERS];
  #if ENABLED(FILAMENT_MOTION_SENSOR)
    uint8_t FilamentSensorEncoder::motion_detected;
  #endif
#else
  int8_t RunoutResponseDebounced::runout_count; // = 0
#endif

//
// Filament Runout event handler
//
#include "../MarlinCore.h"
#include "../gcode/queue.h"

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "host_actions.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../lcd/extui/ui_api.h"
#endif

void event_filament_runout() {

  if (TERN0(ADVANCED_PAUSE_FEATURE, did_pause_print)) return;  // Action already in progress. Purge triggered repeated runout.

  #if ENABLED(TOOLCHANGE_MIGRATION_FEATURE)
    if (migration.in_progress) {
      #if ENABLED(DEBUG_TOOLCHANGE_MIGRATION_FEATURE)
        SERIAL_ECHOLN("Migration Already In Progress");
      #endif
      return;  // Action already in progress. Purge triggered repeated runout.
    }
    if (migration.automode) {
      #if ENABLED(DEBUG_TOOLCHANGE_MIGRATION_FEATURE)
        SERIAL_ECHOLN("Migration Starting");
      #endif
      if (extruder_migration()) return;
    }
  #endif

  TERN_(EXTENSIBLE_UI, ExtUI::onFilamentRunout(ExtUI::getActiveTool()));

  #if EITHER(HOST_PROMPT_SUPPORT, HOST_ACTION_COMMANDS)
    const char tool = '0'
      #if NUM_RUNOUT_SENSORS > 1
        + active_extruder
      #endif
    ;
  #endif

  //action:out_of_filament
  #if ENABLED(HOST_PROMPT_SUPPORT)
    host_action_prompt_begin(PROMPT_FILAMENT_RUNOUT, PSTR("FilamentRunout T"), tool);
    host_action_prompt_show();
  #endif

  const bool run_runout_script = !runout.host_handling;

  #if ENABLED(HOST_ACTION_COMMANDS)
    if (run_runout_script
      && ( strstr(FILAMENT_RUNOUT_SCRIPT, "M600")
        || strstr(FILAMENT_RUNOUT_SCRIPT, "M125")
        #if ENABLED(ADVANCED_PAUSE_FEATURE)
          || strstr(FILAMENT_RUNOUT_SCRIPT, "M25")
        #endif
      )
    ) {
      host_action_paused(false);
    }
    else {
      // Legacy Repetier command for use until newer version supports standard dialog
      // To be removed later when pause command also triggers dialog
      #ifdef ACTION_ON_FILAMENT_RUNOUT
        host_action(PSTR(ACTION_ON_FILAMENT_RUNOUT " T"), false);
        SERIAL_CHAR(tool);
        SERIAL_EOL();
      #endif

      host_action_pause(false);
    }
    SERIAL_ECHOPGM(" " ACTION_REASON_ON_FILAMENT_RUNOUT " ");
    SERIAL_CHAR(tool);
    SERIAL_EOL();
  #endif // HOST_ACTION_COMMANDS

  if (run_runout_script)
    queue.inject_P(PSTR(FILAMENT_RUNOUT_SCRIPT));
}

#endif // HAS_FILAMENT_SENSOR
