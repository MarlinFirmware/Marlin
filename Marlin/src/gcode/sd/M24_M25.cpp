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

#if HAS_MEDIA

#include "../gcode.h"
#include "../../sd/cardreader.h"
#include "../../module/printcounter.h"
#include "../../lcd/marlinui.h"
#include "../../module/temperature.h"

#if ENABLED(PARK_HEAD_ON_PAUSE)
  #include "../../feature/pause.h"
#endif

#if ENABLED(HOST_ACTION_COMMANDS)
  #include "../../feature/host_actions.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../feature/powerloss.h"
#endif

#if DGUS_LCD_UI_MKS
  #include "../../lcd/extui/dgus/DGUSDisplayDef.h"
#endif

#include "../../MarlinCore.h" // for startOrResumeJob

#if DISABLED(PARK_HEAD_ON_PAUSE) && ENABLED(HEATER_IDLE_HANDLER) && PAUSE_PARK_NOZZLE_TIMEOUT
  #define MEDIA_PAUSE_PARK_NOZZLE_TIMEOUT 1
#endif

/**
 * M24: Start or Resume Media Print
 *
 * Parameters:
 *   With POWER_LOSS_RECOVERY:
 *     S<pos>   Position in file to resume from
 *     T<time>  Elapsed time since start of print
 */
void GcodeSuite::M24() {
  #if MEDIA_PAUSE_PARK_NOZZLE_TIMEOUT
    // Re-enable any timed-out heaters
    HOTEND_LOOP() thermalManager.reset_hotend_idle_timer(e);
    HOTEND_LOOP() thermalManager.wait_for_hotend(e);
  #endif

  #if ALL(ADVANCED_PAUSE_FANS_PAUSE, HAS_FAN)
    thermalManager.set_fans_paused(false);
  #endif

  #if DGUS_LCD_UI_MKS
    if ((print_job_timer.isPaused() || print_job_timer.isRunning()) && !parser.seen("ST"))
      MKS_resume_print_move();
  #endif

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
    card.startOrResumeFilePrinting(); // SD card will now be read for commands
    marlin.startOrResumeJob();        // Start (or resume) the print job timer
    TERN_(POWER_LOSS_RECOVERY, recovery.prepare());
  }

  #if ENABLED(HOST_ACTION_COMMANDS)
    #ifdef ACTION_ON_RESUME
      hostui.resume();
    #endif
    TERN_(HOST_PROMPT_SUPPORT, hostui.prompt_open(PROMPT_INFO, F("Resuming SD"), FPSTR(DISMISS_STR)));
  #endif

  ui.reset_status();
}

/**
 * M25: Pause Media Print
 *
 * With PARK_HEAD_ON_PAUSE:
 *   Invoke 'M125' to store the current position and move to the park
 *   position. 'M24' will move the head back before resuming the print.
 */
void GcodeSuite::M25() {

  #if ENABLED(PARK_HEAD_ON_PAUSE)

    M125();

  #else

    // Set initial pause flag to prevent more commands from landing in the queue while we try to pause
    if (card.isStillPrinting()) card.pauseSDPrint();

    #if ENABLED(POWER_LOSS_RECOVERY) && DISABLED(DGUS_LCD_UI_MKS)
      if (recovery.enabled) recovery.save(true);
    #endif

    print_job_timer.pause();

    TERN_(DGUS_LCD_UI_MKS, MKS_pause_print_move());

    IF_DISABLED(DWIN_CREALITY_LCD, ui.reset_status());

    #if ENABLED(HOST_ACTION_COMMANDS)
      TERN_(HOST_PROMPT_SUPPORT, hostui.prompt_open(PROMPT_PAUSE_RESUME, F("Pause SD"), F("Resume")));
      #ifdef ACTION_ON_PAUSE
        hostui.pause();
      #endif
    #endif

    #if ALL(ADVANCED_PAUSE_FANS_PAUSE, HAS_FAN)
      thermalManager.set_fans_paused(true);
    #endif

    #if MEDIA_PAUSE_PARK_NOZZLE_TIMEOUT
      // Start the heater idle timers
      constexpr millis_t nozzle_timeout_ms = SEC_TO_MS(PAUSE_PARK_NOZZLE_TIMEOUT);
      HOTEND_LOOP() thermalManager.heater_idle[e].start(nozzle_timeout_ms);
    #endif

  #endif
}

#endif // HAS_MEDIA
