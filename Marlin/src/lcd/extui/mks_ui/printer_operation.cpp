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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../gcode/gcode.h"
#include "../../../module/planner.h"
#include "../../../module/motion.h"
#include "../../../sd/cardreader.h"
#include "../../../inc/MarlinConfig.h"
#include "../../../MarlinCore.h"
#include "../../../gcode/queue.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../feature/powerloss.h"
#endif
#include "../../../module/printcounter.h"

#define FILAMENT_IS_OUT(N...) (READ(FIL_RUNOUT##N##_PIN) == FIL_RUNOUT##N##_STATE)
#ifndef FILAMENT_RUNOUT_THRESHOLD
  #define FILAMENT_RUNOUT_THRESHOLD 20
#endif

extern uint32_t To_pre_view;
extern bool flash_preview_begin, default_preview_flg, gcode_preview_over;

void printer_state_polling() {
  if (uiCfg.print_state == PAUSING) {
    #if HAS_MEDIA
      if (!planner.has_blocks_queued() && card.getIndex() > MIN_FILE_PRINTED)
        uiCfg.waitEndMoves++;

      if (uiCfg.waitEndMoves > 20) {
        uiCfg.waitEndMoves = 0;
        planner.synchronize();

        card.pauseSDPrint();
        print_job_timer.pause();

        TERN_(POWER_LOSS_RECOVERY, if (recovery.enabled) recovery.save(true));

        // save the position
        uiCfg.current_position_bak = motion.position;

        if (gCfgItems.pausePos.z != -1.0f) {
          gcode.process_subcommands_now(TS(F("G91\nG1Z"), p_float_t(gCfgItems.pausePos.z, 1), F("\nG90")));
        }
        if (gCfgItems.pausePos.x != -1.0f && gCfgItems.pausePos.y != -1.0f) {
          gcode.process_subcommands_now(TS(F("G1X"), p_float_t(gCfgItems.pausePos.x, 1), C('Y'), p_float_t(gCfgItems.pausePos.y, 1)));
        }
        uiCfg.print_state = PAUSED;

        gCfgItems.pause_reprint = true;
        update_spi_flash();
      }
    #endif
  }
  else
    uiCfg.waitEndMoves = 0;

  if (uiCfg.print_state == PAUSED) {
  }

  if (uiCfg.print_state == RESUMING) {
    if (card.isPaused()) {
      if (gCfgItems.pausePos.x != -1.0f && gCfgItems.pausePos.y != -1.0f)
        gcode.process_subcommands_now(TS(F("G1X"), p_float_t(uiCfg.current_position_bak.x, 1), C('Y'), p_float_t(uiCfg.current_position_bak.y, 1)));
      if (gCfgItems.pausePos.z != -1.0f)
        gcode.process_subcommands_now(TS(F("G1Z"), p_float_t(uiCfg.current_position_bak.z, 1)));
      card.startOrResumeFilePrinting();
      marlin.startOrResumeJob();
      TERN_(POWER_LOSS_RECOVERY, recovery.prepare());
      uiCfg.print_state = WORKING;
      start_print_time();

      gCfgItems.pause_reprint = false;
      update_spi_flash();
    }
  }

  #if ENABLED(POWER_LOSS_RECOVERY)
    if (uiCfg.print_state == REPRINTED) {
      #if HAS_HOTEND
        HOTEND_LOOP() {
          const int16_t et = recovery.info.target_temperature[e];
          if (et) {
            TERN_(HAS_MULTI_HOTEND, gcode.process_subcommands_now(TS(C('T'), e)));
            gcode.process_subcommands_now(TS(F("M109S"), et));
          }
        }
      #endif

      recovery.resume();

      #if 0
        // Move back to the saved XY
        gcode.process_subcommands_now(TS(F("G1F2000X"), p_float_t(recovery.info.current_position.x, 3), C('Y'), p_float_t(recovery.info.current_position.y, 3)));
        if (gCfgItems.pause_reprint && gCfgItems.pausePos.z != -1.0f)
          gcode.process_subcommands_now(TS(F("G91\nG1Z-"), p_float_t(gCfgItems.pausePos.z, 1), F("\nG90")));
      #endif
      uiCfg.print_state = WORKING;
      start_print_time();

      gCfgItems.pause_reprint = false;
      update_spi_flash();
    }
  #endif

  if (uiCfg.print_state == WORKING) filament_check();

  TERN_(MKS_WIFI_MODULE, wifi_looping());
}

void filament_pin_setup() {
  #if PIN_EXISTS(FIL_RUNOUT1)
    SET_INPUT_PULLUP(FIL_RUNOUT1_PIN);
  #endif
  #if PIN_EXISTS(FIL_RUNOUT2)
    SET_INPUT_PULLUP(FIL_RUNOUT2_PIN);
  #endif
  #if PIN_EXISTS(FIL_RUNOUT3)
    SET_INPUT_PULLUP(FIL_RUNOUT3_PIN);
  #endif
}

void filament_check() {
  #if PIN_EXISTS(FIL_RUNOUT1)
    static int fil_det_count_1 = 0;
    if (FILAMENT_IS_OUT(1))
      fil_det_count_1++;
    else if (fil_det_count_1 > 0)
      fil_det_count_1--;
  #endif

  #if PIN_EXISTS(FIL_RUNOUT2)
    static int fil_det_count_2 = 0;
    if (FILAMENT_IS_OUT(2))
      fil_det_count_2++;
    else if (fil_det_count_2 > 0)
      fil_det_count_2--;
  #endif

  #if PIN_EXISTS(FIL_RUNOUT3)
    static int fil_det_count_3 = 0;
    if (FILAMENT_IS_OUT(3))
      fil_det_count_3++;
    else if (fil_det_count_3 > 0)
      fil_det_count_3--;
  #endif

  if (false
    #if PIN_EXISTS(FIL_RUNOUT1)
      || fil_det_count_1 >= FILAMENT_RUNOUT_THRESHOLD
    #endif
    #if PIN_EXISTS(FIL_RUNOUT2)
      || fil_det_count_2 >= FILAMENT_RUNOUT_THRESHOLD
    #endif
    #if PIN_EXISTS(FIL_RUNOUT3)
      || fil_det_count_3 >= FILAMENT_RUNOUT_THRESHOLD
    #endif
  ) {
    clear_cur_ui();
    card.pauseSDPrint();
    stop_print_time();
    uiCfg.print_state = PAUSING;

    if (gCfgItems.from_flash_pic)
      flash_preview_begin = true;
    else
      default_preview_flg = true;

    lv_draw_dialog(DIALOG_TYPE_FILAMENT_NO_PRESS);
  }
}

#endif // HAS_TFT_LVGL_UI
