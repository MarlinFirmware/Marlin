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

#if ALL(DWIN_LCD_PROUI, CV_LASER_MODULE)
  #include "../../lcd/e3v2/proui/dwin.h"
  #include "../../feature/spindle_laser.h"
#endif

#include "../../MarlinCore.h" // for startOrResumeJob

/**
 * M24: Start or Resume SD Print
 */
void GcodeSuite::M24() {

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

  #if ENABLED(CV_LASER_MODULE)
    if(laser_device.is_laser_device()) // 107011-20210925 激光模式。
    {
      cutter.apply_power(laser_device.save_power); // 恢复激光功率
      //暂停后恢复时先跑到之前的位置。专业固件是最好的
      //if(print_job_timer.isPaused()) do_blocking_move_to_xy(laser_device.pause_before_position_x, laser_device.pause_before_position_y, homing_feedrate(X_AXIS));//107011- 20211105 暂停逻辑改为了，停在最后执行位置， 因此屏蔽掉此行
    }
  #endif

  if (card.isFileOpen()) {
    card.startOrResumeFilePrinting(); // SD card will now be read for commands
    startOrResumeJob();               // Start (or resume) the print job timer
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
 * M25: Pause SD Print
 *
 * With PARK_HEAD_ON_PAUSE:
 *   Invoke M125 to store the current position and move to the park
 *   position. M24 will move the head back before resuming the print.
 */
void GcodeSuite::M25() {

  #if ENABLED(PARK_HEAD_ON_PAUSE)

    //107011 -20210926
    #if ENABLED(CV_LASER_MODULE)
      if(laser_device.is_laser_device()) {
        if (print_job_timer.isPaused()) return;
        laser_device.save_power = cutter.power;
        cutter.apply_power(0);
        card.pauseSDPrint();
        print_job_timer.pause();
        #if ENABLED(HOST_ACTION_COMMANDS)
          TERN_(HOST_PROMPT_SUPPORT, hostui.prompt_open(PROMPT_PAUSE_RESUME, F("Pause SD"), F("Resume")));
          #ifdef ACTION_ON_PAUSE
            hostui.pause();
          #endif
        #endif
      }
      else
    #endif
    if (printingIsActive()) M125();  // ProUI Do only if printing

  #else

    // Set initial pause flag to prevent more commands from landing in the queue while we try to pause
    if (IS_SD_PRINTING()) card.pauseSDPrint();

    #if ENABLED(POWER_LOSS_RECOVERY) && DISABLED(DGUS_LCD_UI_MKS)
      if (recovery.enabled) recovery.save(true);
    #endif

    print_job_timer.pause();

    //107011 -20210926
    #if ENABLED(CV_LASER_MODULE)
      if(laser_device.is_laser_device()) {
        laser_device.save_power = cutter.power;
        cutter.apply_power(0);
      }
    #endif

    TERN_(DGUS_LCD_UI_MKS, MKS_pause_print_move());

    IF_DISABLED(DWIN_CREALITY_LCD, ui.reset_status());

    #if ENABLED(HOST_ACTION_COMMANDS)
      TERN_(HOST_PROMPT_SUPPORT, hostui.prompt_open(PROMPT_PAUSE_RESUME, F("Pause SD"), F("Resume")));
      #ifdef ACTION_ON_PAUSE
        hostui.pause();
      #endif
    #endif

  #endif
}

#endif // HAS_MEDIA
