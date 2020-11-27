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

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../gcode/gcode.h"
#include "../../../../module/temperature.h"
#include "../../../../module/planner.h"
#include "../../../../module/motion.h"
#include "../../../../sd/cardreader.h"
#include "../../../../inc/MarlinConfig.h"
#include "../../../../MarlinCore.h"

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

extern uint32_t To_pre_view;
extern bool flash_preview_begin, default_preview_flg, gcode_preview_over;

void printer_state_polling() {
  if (uiCfg.print_state == PAUSING) {
    #if ENABLED(SDSUPPORT)
      if (!planner.has_blocks_queued() && card.getIndex() > MIN_FILE_PRINTED)
        uiCfg.waitEndMoves++;

      if (uiCfg.waitEndMoves > 20) {
        uiCfg.waitEndMoves = 0;
        planner.synchronize();

        gcode.process_subcommands_now_P(PSTR("M25"));

        //save the positon
        uiCfg.current_x_position_bak = current_position.x;
        uiCfg.current_y_position_bak = current_position.y;

        if (gCfgItems.pausePosZ != (float)-1) {
          gcode.process_subcommands_now_P(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 Z%.1f"), gCfgItems.pausePosZ);
          gcode.process_subcommands_now(public_buf_l);
          gcode.process_subcommands_now_P(PSTR("G90"));
        }
        if (gCfgItems.pausePosX != (float)-1 && gCfgItems.pausePosY != (float)-1) {
          sprintf_P(public_buf_l, PSTR("G1 X%.1f Y%.1f"), gCfgItems.pausePosX, gCfgItems.pausePosY);
          gcode.process_subcommands_now(public_buf_l);
        }
        uiCfg.print_state = PAUSED;
        uiCfg.current_e_position_bak = current_position.e;

        // #if ENABLED(POWER_LOSS_RECOVERY)
        //  if (recovery.enabled) recovery.save(true);
        // #endif
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
    if (IS_SD_PAUSED()) {
      if (gCfgItems.pausePosX != (float)-1 && gCfgItems.pausePosY != (float)-1) {
        sprintf_P(public_buf_m, PSTR("G1 X%.1f Y%.1f"), uiCfg.current_x_position_bak, uiCfg.current_y_position_bak);
        gcode.process_subcommands_now(public_buf_m);
      }
      if (gCfgItems.pausePosZ != (float)-1) {
        gcode.process_subcommands_now_P(PSTR("G91"));
        sprintf_P(public_buf_l, PSTR("G1 Z-%.1f"), gCfgItems.pausePosZ);
        gcode.process_subcommands_now(public_buf_l);
        gcode.process_subcommands_now_P(PSTR("G90"));
      }
      gcode.process_subcommands_now_P(M24_STR);
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
            #if HAS_MULTI_HOTEND
              sprintf_P(public_buf_m, PSTR("T%i"), e);
              gcode.process_subcommands_now(public_buf_m);
            #endif
            sprintf_P(public_buf_m, PSTR("M109 S%i"), et);
            gcode.process_subcommands_now(public_buf_m);
          }
        }
      #endif

      recovery.resume();
      #if 0
        // Move back to the saved XY
        char str_1[16], str_2[16];
        sprintf_P(public_buf_m, PSTR("G1 X%s Y%s F2000"),
          dtostrf(recovery.info.current_position.x, 1, 3, str_1),
          dtostrf(recovery.info.current_position.y, 1, 3, str_2)
        );
        gcode.process_subcommands_now(public_buf_m);

        if (gCfgItems.pause_reprint && gCfgItems.pausePosZ != -1.0f) {
          gcode.process_subcommands_now_P(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 Z-%.1f"), gCfgItems.pausePosZ);
          gcode.process_subcommands_now(public_buf_l);
          gcode.process_subcommands_now_P(PSTR("G90"));
        }
      #endif
      uiCfg.print_state = WORKING;
      start_print_time();

      gCfgItems.pause_reprint = false;
      update_spi_flash();
    }
  #endif

  if (uiCfg.print_state == WORKING)
    filament_check();

  TERN_(MKS_WIFI_MODULE, wifi_looping());
}

void filament_pin_setup() {
  #if PIN_EXISTS(MT_DET_1)
    SET_INPUT_PULLUP(MT_DET_1_PIN);
  #endif
  #if PIN_EXISTS(MT_DET_2)
    SET_INPUT_PULLUP(MT_DET_2_PIN);
  #endif
  #if PIN_EXISTS(MT_DET_3)
    SET_INPUT_PULLUP(MT_DET_3_PIN);
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

  if (false
    #if PIN_EXISTS(MT_DET_1)
      || fil_det_count_1 >= FIL_DELAY
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

    if (gCfgItems.from_flash_pic)
      flash_preview_begin = true;
    else
      default_preview_flg = true;

    lv_draw_printing();
  }
}

#endif // HAS_TFT_LVGL_UI
