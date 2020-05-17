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

#include "../../inc/MarlinConfig.h"

#if ENABLED(SDSUPPORT)

#include "../gcode.h"
#include "../../module/printcounter.h"

#if EITHER(LCD_SET_PROGRESS_MANUALLY, SD_REPRINT_LAST_SELECTED_FILE)
  #include "../../lcd/ultralcd.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/powerloss.h"
#endif

#if HAS_LEDS_OFF_FLAG
  #include "../../feature/leds/printer_event_leds.h"
#endif

#if ENABLED(EXTENSIBLE_UI)
  #include "../../lcd/extui/ui_api.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../feature/host_actions.h"
#endif

#if ENABLED(SD_FINISHED_STEPPERRELEASE) && defined(SD_FINISHED_RELEASECOMMAND)
  #include "../../module/planner.h"
#endif

#ifndef PE_LEDS_COMPLETED_TIME
  #define PE_LEDS_COMPLETED_TIME (30*60)
#endif

/**
 * M1001: Execute actions for SD print completion
 */
void GcodeSuite::M1001() {

  // Report total print time
  const bool long_print = print_job_timer.duration() > 60;
  if (long_print) gcode.process_subcommands_now_P(PSTR("M31"));

  // Stop the print job timer
  gcode.process_subcommands_now_P(PSTR("M77"));

  // Set the progress bar "done" state
  #if ENABLED(LCD_SET_PROGRESS_MANUALLY)
    ui.set_progress_done();
  #endif

  // Purge the recovery file
  #if ENABLED(POWER_LOSS_RECOVERY)
    recovery.purge();
  #endif

  // Announce SD file completion
  SERIAL_ECHOLNPGM(STR_FILE_PRINTED);

  // Update the status LED color
  #if HAS_LEDS_OFF_FLAG
    if (long_print) {
      printerEventLEDs.onPrintCompleted();
      #if ENABLED(EXTENSIBLE_UI)
        ExtUI::onUserConfirmRequired_P(GET_TEXT(MSG_PRINT_DONE));
      #endif
      #if ENABLED(HOST_PROMPT_SUPPORT)
        host_prompt_do(PROMPT_USER_CONTINUE, GET_TEXT(MSG_PRINT_DONE), CONTINUE_STR);
      #endif
      wait_for_user_response(1000UL * TERN(HAS_LCD_MENU, PE_LEDS_COMPLETED_TIME, 30));
      printerEventLEDs.onResumeAfterWait();
    }
  #endif

  // Wait for the queue to empty (and "clean"), inject SD_FINISHED_RELEASECOMMAND
  #if ENABLED(SD_FINISHED_STEPPERRELEASE) && defined(SD_FINISHED_RELEASECOMMAND)
    planner.finish_and_disable();
  #endif

  // Re-select the last printed file in the UI
  #if ENABLED(SD_REPRINT_LAST_SELECTED_FILE)
    ui.reselect_last_file();
  #endif
}

#endif // SDSUPPORT
