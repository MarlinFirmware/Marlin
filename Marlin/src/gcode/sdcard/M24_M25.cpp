/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2019 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
#include "../../sd/cardreader.h"
#include "../../module/printcounter.h"
#include "../../lcd/ultralcd.h"

#if ENABLED(PARK_HEAD_ON_PAUSE)
  #include "../../feature/pause.h"
  #include "../queue.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../feature/host_actions.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/power_loss_recovery.h"
#endif

#include "../../MarlinCore.h" // for startOrResumeJob

/**
 * M24: Start or Resume SD Print
 */
void GcodeSuite::M24() {

  #if ENABLED(POWER_LOSS_RECOVERY)
    if (parser.seenval('S')) card.setIndex(parser.value_long());
    if (parser.seenval('T')) print_job_timer.resume(parser.value_long());
  #endif

  #if ENABLED(PARK_HEAD_ON_PAUSE)
    if (did_pause_print) {
      resume_print(); // will call print_job_timer.start()
      return;
    }
  #endif

  if (card.isFileOpen()) {
    card.startFileprint();            // SD card will now be read for commands
    startOrResumeJob();               // Start (or resume) the print job timer
    #if ENABLED(POWER_LOSS_RECOVERY)
      recovery.prepare();
    #endif
  }

  #if ENABLED(HOST_ACTION_COMMANDS)
    #ifdef ACTION_ON_RESUME
      host_action_resume();
    #endif
    #if ENABLED(HOST_PROMPT_SUPPORT)
      host_prompt_open(PROMPT_INFO, PSTR("Resuming SD"), PSTR("Dismiss"));
    #endif
  #endif

  ui.reset_status();
}

/**
 * M25: Pause SD Print
 */
void GcodeSuite::M25() {

  #if ENABLED(POWER_LOSS_RECOVERY)
    if (recovery.enabled) recovery.save(true, false);
  #endif

  // Set initial pause flag to prevent more commands from landing in the queue while we try to pause
  #if ENABLED(SDSUPPORT)
    if (IS_SD_PRINTING()) card.pauseSDPrint();
  #endif

  #if ENABLED(PARK_HEAD_ON_PAUSE)

    M125();

  #else

    print_job_timer.pause();
    ui.reset_status();

    #if ENABLED(HOST_ACTION_COMMANDS)
      #if ENABLED(HOST_PROMPT_SUPPORT)
        host_prompt_open(PROMPT_PAUSE_RESUME, PSTR("Pause SD"), PSTR("Resume"));
      #endif
      #ifdef ACTION_ON_PAUSE
        host_action_pause();
      #endif
    #endif

  #endif
}

#endif // SDSUPPORT
