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
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"

#include "../../../../gcode/queue.h"
#include "../../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

static lv_obj_t *labelV, *buttonV;

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
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_M_X_P:
      if (queue.length <= (BUFSIZE - 3)) {
        queue.enqueue_one_P(PSTR("G91"));
        sprintf_P(public_buf_l, PSTR("G1 X%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
        queue.enqueue_one_now(public_buf_l);
        queue.enqueue_one_P(PSTR("G90"));
      }
      break;
    case ID_M_X_N:
      if (queue.length <= (BUFSIZE - 3)) {
        queue.enqueue_now_P(PSTR("G91"));
        sprintf_P(public_buf_l, PSTR("G1 X-%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
        queue.enqueue_one_now(public_buf_l);
        queue.enqueue_now_P(PSTR("G90"));
      }
      break;
    case ID_M_Y_P:
      if (queue.length <= (BUFSIZE - 3)) {
        queue.enqueue_now_P(PSTR("G91"));
        sprintf_P(public_buf_l, PSTR("G1 Y%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
        queue.enqueue_one_now(public_buf_l);
        queue.enqueue_now_P(PSTR("G90"));
      }
      break;
    case ID_M_Y_N:
      if (queue.length <= (BUFSIZE - 3)) {
        queue.enqueue_now_P(PSTR("G91"));
        sprintf_P(public_buf_l, PSTR("G1 Y-%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
        queue.enqueue_one_now(public_buf_l);
        queue.enqueue_now_P(PSTR("G90"));
      }
      break;
    case ID_M_Z_P:
      if (queue.length <= (BUFSIZE - 3)) {
        queue.enqueue_now_P(PSTR("G91"));
        sprintf_P(public_buf_l, PSTR("G1 Z%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
        queue.enqueue_one_now(public_buf_l);
        queue.enqueue_now_P(PSTR("G90"));
      }
      break;
    case ID_M_Z_N:
      if (queue.length <= (BUFSIZE - 3)) {
        queue.enqueue_now_P(PSTR("G91"));
        sprintf_P(public_buf_l, PSTR("G1 Z-%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
        queue.enqueue_one_now(public_buf_l);
        queue.enqueue_now_P(PSTR("G90"));
      }
      break;
    case ID_M_STEP:
      if (abs(10 * (int)uiCfg.move_dist) == 100)
        uiCfg.move_dist = 0.1;
      else
        uiCfg.move_dist *= (float)10;
      disp_move_dist();
      break;
    case ID_M_RETURN:
      clear_cur_ui();
      draw_return_ui();
      break;
  }
}

void lv_draw_move_motor(void) {
  scr = lv_screen_create(MOVE_MOTOR_UI);
  lv_obj_t *buttonXI = lv_big_button_create(scr, "F:/bmp_xAdd.bin", move_menu.x_add, INTERVAL_V, titleHeight, event_handler, ID_M_X_P);
  lv_obj_clear_protect(buttonXI, LV_PROTECT_FOLLOW);
  lv_big_button_create(scr, "F:/bmp_xDec.bin", move_menu.x_dec, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_X_N);
  lv_big_button_create(scr, "F:/bmp_yAdd.bin", move_menu.y_add, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_M_Y_P);
  lv_big_button_create(scr, "F:/bmp_yDec.bin", move_menu.y_dec, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_Y_N);
  lv_big_button_create(scr, "F:/bmp_zAdd.bin", move_menu.z_add, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_M_Z_P);
  lv_big_button_create(scr, "F:/bmp_zDec.bin", move_menu.z_dec, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_Z_N);

  // button with image and label changed dinamycally by disp_move_dist
  buttonV = lv_imgbtn_create(scr, nullptr, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_M_STEP);
  labelV = lv_label_create_empty(buttonV);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonV);
    }
  #endif

  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_RETURN);

  disp_move_dist();
}

void disp_move_dist() {
  // char buf[30] = {0};

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
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
