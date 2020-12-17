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

#include "../../../../gcode/queue.h"
#include "../../../../gcode/gcode.h"
#include "../../../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../../module/settings.h"
#endif

#if HAS_BED_PROBE
  #include "../../../../module/probe.h"
#endif

extern lv_group_t *g;
static lv_obj_t *scr;

static lv_obj_t *labelV, *buttonV, *zOffsetText;

enum {
  ID_BABY_STEP_X_P = 1,
  ID_BABY_STEP_X_N,
  ID_BABY_STEP_Y_P,
  ID_BABY_STEP_Y_N,
  ID_BABY_STEP_Z_P,
  ID_BABY_STEP_Z_N,
  ID_BABY_STEP_DIST,
  ID_BABY_STEP_RETURN
};

static float babystep_dist=0.01;
static uint8_t has_adjust_z = 0;

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  char baby_buf[30] = { 0 };
  switch (obj->mks_obj_id) {
    case ID_BABY_STEP_X_P:
      sprintf_P(baby_buf, PSTR("M290 X%.3f"), babystep_dist);
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_X_N:
      sprintf_P(baby_buf, PSTR("M290 X%.3f"), -babystep_dist);
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Y_P:
      sprintf_P(baby_buf, PSTR("M290 Y%.3f"), babystep_dist);
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Y_N:
      sprintf_P(baby_buf, PSTR("M290 Y%.3f"), -babystep_dist);
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Z_P:
      sprintf_P(baby_buf, PSTR("M290 Z%.3f"), babystep_dist);
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_Z_N:
      sprintf_P(baby_buf, PSTR("M290 Z%.3f"), -babystep_dist);
      gcode.process_subcommands_now_P(PSTR(baby_buf));
      has_adjust_z = 1;
      break;
    case ID_BABY_STEP_DIST:
      if (abs((int)(100 * babystep_dist)) == 1)
        babystep_dist = 0.05;
      else if (abs((int)(100 * babystep_dist)) == 5)
        babystep_dist = 0.1;
      else
        babystep_dist = 0.01;
      disp_baby_step_dist();
      break;
    case ID_BABY_STEP_RETURN:
      if (has_adjust_z == 1) {
        TERN_(EEPROM_SETTINGS, (void)settings.save());
        has_adjust_z = 0;
      }
      clear_cur_ui();
      draw_return_ui();
      break;
  }
}

void lv_draw_baby_stepping(void) {
  scr = lv_screen_create(BABY_STEP_UI);
  lv_big_button_create(scr, "F:/bmp_xAdd.bin", move_menu.x_add, INTERVAL_V, titleHeight, event_handler, ID_BABY_STEP_X_P);
  lv_big_button_create(scr, "F:/bmp_xDec.bin", move_menu.x_dec, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_X_N);
  lv_big_button_create(scr, "F:/bmp_yAdd.bin", move_menu.y_add, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_BABY_STEP_Y_P);
  lv_big_button_create(scr, "F:/bmp_yDec.bin", move_menu.y_dec, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_Y_N);
  lv_big_button_create(scr, "F:/bmp_zAdd.bin", move_menu.z_add, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_BABY_STEP_Z_P);
  lv_big_button_create(scr, "F:/bmp_zDec.bin", move_menu.z_dec, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_Z_N);
  buttonV = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_BABY_STEP_DIST);
  labelV = lv_label_create_empty(buttonV);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonV);
    }
  #endif

  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_BABY_STEP_RETURN);

  disp_baby_step_dist();

  zOffsetText = lv_label_create(scr, 290, TITLE_YPOS, nullptr);
  disp_z_offset_value();
}

void disp_baby_step_dist() {
  // char buf[30] = {0};
  if ((int)(100 * babystep_dist) == 1)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_01.bin");
  else if ((int)(100 * babystep_dist) == 5)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_05.bin");
  else if ((int)(100 * babystep_dist) == 10)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_baby_move0_1.bin");

  if (gCfgItems.multiple_language) {
    if ((int)(100 * babystep_dist) == 1) {
      lv_label_set_text(labelV, move_menu.step_001mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(100 * babystep_dist) == 5) {
      lv_label_set_text(labelV, move_menu.step_005mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(100 * babystep_dist) == 10) {
      lv_label_set_text(labelV, move_menu.step_01mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void disp_z_offset_value() {
  char buf[20];
  sprintf_P(buf, PSTR("offset Z: %.3f"), (float)TERN(HAS_BED_PROBE, probe.offset.z, 0));
  lv_label_set_text(zOffsetText, buf);
}

void lv_clear_baby_stepping() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
