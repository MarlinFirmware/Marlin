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
#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(TFT_LVGL_UI)

  #include "../../../../MarlinCore.h"

  #include "lv_conf.h"
  #include "draw_ui.h"
  #include "../../../../module/temperature.h"
  #include "../../../../module/motion.h"
  #include "../../../../sd/cardreader.h"
  #include "../../../../gcode/queue.h"

  #if ENABLED(POWER_LOSS_RECOVERY)
    #include "../../../../feature/powerloss.h"
  #endif

  #include "../../../../gcode/gcode.h"
  #include "../../../../module/planner.h"

  extern uint32_t To_pre_view;
  extern uint8_t flash_preview_begin, default_preview_flg, gcode_preview_over;

  void printer_state_polling() {
    if (uiCfg.print_state == PAUSING) {
      #if ENABLED(SDSUPPORT)
        if (!planner.has_blocks_queued() && card.getIndex() > MIN_FILE_PRINTED) //���� �ļ��� M109��M190ָ��
          uiCfg.waitEndMoves++;

        if (uiCfg.waitEndMoves > 20) {
          uiCfg.waitEndMoves = 0;
          planner.synchronize();
          gcode.process_subcommands_now_P(PSTR("M25\nG91\nG1 Z5\nG90"));

          uiCfg.print_state = PAUSED;

          //#if ENABLED(POWER_LOSS_RECOVERY)
          //  if (recovery.enabled) recovery.save(true);
          //#endif
          gCfgItems.pause_reprint = 1;
          update_spi_flash();
        }
      #endif
    }
    else {
      uiCfg.waitEndMoves = 0;
    }

    if (uiCfg.print_state == PAUSED) {

    }

    if (uiCfg.print_state == RESUMING) {
      if (IS_SD_PAUSED()) {
        gcode.process_subcommands_now_P(PSTR("G91\nG1 Z-5\nG90\nM24"));
        uiCfg.print_state = WORKING;
        start_print_time();

        gCfgItems.pause_reprint = 0;
        update_spi_flash();
      }
    }
    #if ENABLED(POWER_LOSS_RECOVERY)
    if (uiCfg.print_state == REPRINTED) {
      memset(public_buf_m, 0, sizeof(public_buf_m));
      #if HOTENDS
      HOTEND_LOOP() {
        const int16_t et = recovery.info.target_temperature[e];
        if (et) {
          #if HOTENDS > 1
            sprintf_P(public_buf_m, PSTR("T%i"), e);
            gcode.process_subcommands_now(public_buf_m);
          #endif
          sprintf_P(public_buf_m, PSTR("M109 S%i"), et);
          gcode.process_subcommands_now(public_buf_m);
        }
      }
      #endif

      if (gCfgItems.pause_reprint == 1)
        gcode.process_subcommands_now_P(PSTR("G91\nG1 Z-5\nG90"));
      recovery.resume();

      uiCfg.print_state = WORKING;
      start_print_time();

      gCfgItems.pause_reprint = 0;
      update_spi_flash();
    }
    #endif

    if (uiCfg.print_state == WORKING)
      filament_check();
  }

  void filament_pin_setup() {
    #if PIN_EXISTS(MT_DET_1)
      pinMode(MT_DET_1_PIN, INPUT_PULLUP);
    #endif
    #if PIN_EXISTS(MT_DET_2)
      pinMode(MT_DET_2_PIN, INPUT_PULLUP);
    #endif
    #if PIN_EXISTS(MT_DET_3)
      pinMode(MT_DET_3_PIN, INPUT_PULLUP);
    #endif
  }

  void filament_check() {
    const int FIL_DELAY = 20;
    #if PIN_EXISTS(MT_DET_1)
      static int fil_det_count_1 = 0;
      if (!READ(MT_DET_1_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_1++;
      else if (READ(MT_DET_1_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_1++;
      else if (fil_det_count_1 > 0)
        fil_det_count_1--;

      if (!READ(MT_DET_1_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_1++;
      else if (READ(MT_DET_1_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_1++;
      else if (fil_det_count_1 > 0)
        fil_det_count_1--;
    #endif

    #if PIN_EXISTS(MT_DET_2)
      static int fil_det_count_2 = 0;
      if (!READ(MT_DET_2_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_2++;
      else if (READ(MT_DET_2_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_2++;
      else if (fil_det_count_2 > 0)
        fil_det_count_2--;

      if (!READ(MT_DET_2_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_2++;
      else if (READ(MT_DET_2_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_2++;
      else if (fil_det_count_2 > 0)
        fil_det_count_2--;
    #endif

    #if PIN_EXISTS(MT_DET_3)
      static int fil_det_count_3 = 0;
      if (!READ(MT_DET_3_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_3++;
      else if (READ(MT_DET_3_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_3++;
      else if (fil_det_count_3 > 0)
        fil_det_count_3--;

      if (!READ(MT_DET_3_PIN) && !MT_DET_PIN_INVERTING)
        fil_det_count_3++;
      else if (READ(MT_DET_3_PIN) && MT_DET_PIN_INVERTING)
        fil_det_count_3++;
      else if (fil_det_count_3 > 0)
        fil_det_count_3--;
    #endif

    if (
      #if PIN_EXISTS(MT_DET_1)
        fil_det_count_1 >= FIL_DELAY
      #else
        false
      #endif
      #if PIN_EXISTS(MT_DET_2)
        || fil_det_count_2 >= FIL_DELAY
      #endif
      #if PIN_EXISTS(MT_DET_3)
        || fil_det_count_3 >= FIL_DELAY
      #endif
      ) {
      clear_cur_ui();
      card.pauseSDPrint();
      stop_print_time();
      uiCfg.print_state = PAUSING;

      if (gCfgItems.from_flash_pic == 1)
        flash_preview_begin = 1;
      else
        default_preview_flg = 1;

      lv_draw_printing();
    }
  }

#endif // TFT_LVGL_UI
