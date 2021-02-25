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
#include "../../../../module/motion.h"
#include "../../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

static lv_obj_t *labelV, *buttonV, *labelP;
static lv_task_t *updatePosTask;
static char cur_label = 'Z';
static float cur_pos = 0;

void disp_cur_pos();

enum {
  ID_M_X_P = 1,
  ID_M_X_N,
  ID_M_Y_P,
  ID_M_Y_N,
  ID_M_Z_P,
  ID_M_Z_N,
  ID_M_STEP,
  ID_M_RETURN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  char str_1[16];
  if (event != LV_EVENT_RELEASED) return;
  if (queue.length <= (BUFSIZE - 3)) {
    bool do_inject = true;
    float dist = uiCfg.move_dist;
    switch (obj->mks_obj_id) {
      case ID_M_X_N: dist *= -1; case ID_M_X_P: cur_label = 'X'; break;
      case ID_M_Y_N: dist *= -1; case ID_M_Y_P: cur_label = 'Y'; break;
      case ID_M_Z_N: dist *= -1; case ID_M_Z_P: cur_label = 'Z'; break;
      default: do_inject = false;
    }
    if (do_inject) {
      sprintf_P(public_buf_l, PSTR("G91\nG1 %c%s F%d\nG90"), cur_label, dtostrf(dist, 1, 3, str_1), uiCfg.moveSpeed);
      queue.inject(public_buf_l);
    }
  }

  switch (obj->mks_obj_id) {
    case ID_M_STEP:
      if (abs(10 * (int)uiCfg.move_dist) == 100)
        uiCfg.move_dist = 0.1;
      else
        uiCfg.move_dist *= 10.0f;
      disp_move_dist();
      break;
    case ID_M_RETURN:
      clear_cur_ui();
      draw_return_ui();
      return;
  }
  disp_cur_pos();
}

void refresh_pos(lv_task_t *) {
  switch (cur_label) {
    case 'X': cur_pos = current_position.x; break;
    case 'Y': cur_pos = current_position.y; break;
    case 'Z': cur_pos = current_position.z; break;
    default: return;
  }
  disp_cur_pos();
}

void lv_draw_move_motor() {
  scr = lv_screen_create(MOVE_MOTOR_UI);
  lv_obj_t *buttonXI = lv_big_button_create(scr, "F:/bmp_xAdd.bin", move_menu.x_add, INTERVAL_V, titleHeight, event_handler, ID_M_X_P);
  lv_obj_clear_protect(buttonXI, LV_PROTECT_FOLLOW);
  lv_big_button_create(scr, "F:/bmp_xDec.bin", move_menu.x_dec, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_X_N);
  lv_big_button_create(scr, "F:/bmp_yAdd.bin", move_menu.y_add, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_M_Y_P);
  lv_big_button_create(scr, "F:/bmp_yDec.bin", move_menu.y_dec, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_Y_N);
  lv_big_button_create(scr, "F:/bmp_zAdd.bin", move_menu.z_add, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_M_Z_P);
  lv_big_button_create(scr, "F:/bmp_zDec.bin", move_menu.z_dec, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_Z_N);

  // button with image and label changed dynamically by disp_move_dist
  buttonV = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_M_STEP);
  labelV = lv_label_create_empty(buttonV);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonV);
  #endif

  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_RETURN);

  // We need to patch the title to leave some space on the right for displaying the status
  lv_obj_t * title = lv_obj_get_child_back(scr, NULL);
  if (title != NULL) lv_obj_set_width(title, TFT_WIDTH - 101);
  labelP = lv_label_create(scr, TFT_WIDTH - 100, TITLE_YPOS, "Z:0.0mm");
  if (labelP != NULL)
    updatePosTask = lv_task_create(refresh_pos, 300, LV_TASK_PRIO_LOWEST, 0);

  disp_move_dist();
  disp_cur_pos();
}

void disp_cur_pos() {
  char str_1[16];
  sprintf_P(public_buf_l, PSTR("%c:%s mm"), cur_label, dtostrf(cur_pos, 1, 1, str_1));
  if (labelP) lv_label_set_text(labelP, public_buf_l);
}

void disp_move_dist() {
  if ((int)(10 * uiCfg.move_dist) == 1)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_step_move0_1.bin");
  else if ((int)(10 * uiCfg.move_dist) == 10)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_step_move1.bin");
  else if ((int)(10 * uiCfg.move_dist) == 100)
    lv_imgbtn_set_src_both(buttonV, "F:/bmp_step_move10.bin");

  if (gCfgItems.multiple_language) {
    if ((int)(10 * uiCfg.move_dist) == 1) {
      lv_label_set_text(labelV, move_menu.step_01mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(10 * uiCfg.move_dist) == 10) {
      lv_label_set_text(labelV, move_menu.step_1mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else if ((int)(10 * uiCfg.move_dist) == 100) {
      lv_label_set_text(labelV, move_menu.step_10mm);
      lv_obj_align(labelV, buttonV, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
  }
}

void lv_clear_move_motor() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_task_del(updatePosTask);
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
