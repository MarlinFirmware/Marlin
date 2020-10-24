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

#include "tft_lvgl_configuration.h"
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"

#include "SPI_TFT.h"
#include "mks_hardware_test.h"
#include "../../../../inc/MarlinConfig.h"

static lv_obj_t *scr;

void lv_draw_error_message(PGM_P const msg) {
  #if 0
    static lv_obj_t *message = nullptr, *kill_message = nullptr, *reset_tips = nullptr;
    if (disp_state_stack._disp_state[disp_state_stack._disp_index] != ERROR_MESSAGE_UI) {
      disp_state_stack._disp_index++;
      disp_state_stack._disp_state[disp_state_stack._disp_index] = ERROR_MESSAGE_UI;
    }
    disp_state = ERROR_MESSAGE_UI;

    scr = lv_screen_create();

    lv_refr_now(lv_refr_get_disp_refreshing());

    if (msg) {
      message = lv_label_create(scr, msg);
      lv_obj_align(message, nullptr, LV_ALIGN_CENTER, 0, -50);
    }

    kill_message = lv_label_create(scr, "PRINTER HALTED");
    lv_obj_align(kill_message, nullptr, LV_ALIGN_CENTER, 0, -10);

    reset_tips = lv_label_create(scr, "Please Reset");
    lv_obj_align(reset_tips, nullptr, LV_ALIGN_CENTER, 0, 30);

    lv_task_handler();
  #endif

  SPI_TFT.LCD_clear(0x0000);
  if (msg) disp_string((TFT_WIDTH - strlen(msg) * 16) / 2, 100, msg, 0xFFFF, 0x0000);
  disp_string((TFT_WIDTH - strlen("PRINTER HALTED") * 16) / 2, 140, "PRINTER HALTED", 0xFFFF, 0x0000);
  disp_string((TFT_WIDTH - strlen("Please Reset") * 16) / 2, 180, "Please Reset", 0xFFFF, 0x0000);
}

void lv_clear_error_message() { lv_obj_del(scr); }

#endif // HAS_TFT_LVGL_UI
