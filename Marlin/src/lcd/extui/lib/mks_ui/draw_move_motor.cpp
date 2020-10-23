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

extern lv_group_t * g;
static lv_obj_t * scr;

static lv_obj_t *labelV, *buttonV;

#define ID_M_X_P    1
#define ID_M_X_N    2
#define ID_M_Y_P    3
#define ID_M_Y_N    4
#define ID_M_Z_P    5
#define ID_M_Z_N    6
#define ID_M_STEP   7
#define ID_M_RETURN 8

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_M_X_P:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (queue.length <= (BUFSIZE - 3)) {
          ZERO(public_buf_l);
          queue.enqueue_one_P(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 X%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_one_P(PSTR("G90"));
        }
      }
      break;
    case ID_M_X_N:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (queue.length <= (BUFSIZE - 3)) {
          ZERO(public_buf_l);
          queue.enqueue_now_P(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 X-%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G90"));
        }
      }
      break;
    case ID_M_Y_P:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (queue.length <= (BUFSIZE - 3)) {
          ZERO(public_buf_l);
          queue.enqueue_now_P(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 Y%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G90"));
        }
      }
      break;
    case ID_M_Y_N:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (queue.length <= (BUFSIZE - 3)) {
          ZERO(public_buf_l);
          queue.enqueue_now_P(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 Y-%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G90"));
        }
      }
      break;
    case ID_M_Z_P:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (queue.length <= (BUFSIZE - 3)) {
          ZERO(public_buf_l);
          queue.enqueue_now_P(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 Z%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G90"));
        }
      }
      break;
    case ID_M_Z_N:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (queue.length <= (BUFSIZE - 3)) {
          ZERO(public_buf_l);
          queue.enqueue_now_P(PSTR("G91"));
          sprintf_P(public_buf_l, PSTR("G1 Z-%3.1f F%d"), uiCfg.move_dist, uiCfg.moveSpeed);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G90"));
        }
      }
      break;
    case ID_M_STEP:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (abs(10 * (int)uiCfg.move_dist) == 100)
          uiCfg.move_dist = 0.1;
        else
          uiCfg.move_dist *= (float)10;

        disp_move_dist();
      }

      break;
    case ID_M_RETURN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        clear_cur_ui();
        draw_return_ui();
      }
      break;
  }
}

void lv_draw_move_motor(void) {
  lv_obj_t *buttonXI, *buttonXD, *buttonYI, *buttonYD, *buttonZI, *buttonZD, *buttonBack;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != MOVE_MOTOR_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = MOVE_MOTOR_UI;
  }
  disp_state = MOVE_MOTOR_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  // Create an Image button
  buttonXI = lv_imgbtn_create(scr, "F:/bmp_xAdd.bin", INTERVAL_V, titleHeight, event_handler, ID_M_X_P);
  lv_obj_clear_protect(buttonXI, LV_PROTECT_FOLLOW);
  buttonXD = lv_imgbtn_create(scr, "F:/bmp_xDec.bin", INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_X_N);
  buttonYI = lv_imgbtn_create(scr, "F:/bmp_yAdd.bin", BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_M_Y_P);
  buttonYD = lv_imgbtn_create(scr, "F:/bmp_yDec.bin", BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_Y_N);
  buttonZI = lv_imgbtn_create(scr, "F:/bmp_zAdd.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_M_Z_P);
  buttonZD = lv_imgbtn_create(scr, "F:/bmp_zDec.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_Z_N);

  buttonV = lv_imgbtn_create(scr, NULL, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_M_STEP);
  //lv_obj_set_event_cb_mks(buttonV, event_handler,ID_T_MORE,"bmp_More.bin",0);

  buttonBack = lv_imgbtn_create(scr, "F:/bmp_return.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_RETURN);

  // Create labels on the image buttons
  lv_obj_t *labelXI = lv_label_create_empty(buttonXI);
  lv_obj_t *labelXD = lv_label_create_empty(buttonXD);
  lv_obj_t *labelYI = lv_label_create_empty(buttonYI);
  lv_obj_t *labelYD = lv_label_create_empty(buttonYD);
  lv_obj_t *labelZI = lv_label_create_empty(buttonZI);
  lv_obj_t *labelZD = lv_label_create_empty(buttonZD);
  labelV = lv_label_create_empty(buttonV);
  lv_obj_t *label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
    lv_label_set_text(labelXI, move_menu.x_add);
    lv_obj_align(labelXI, buttonXI, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelXD, move_menu.x_dec);
    lv_obj_align(labelXD, buttonXD, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelYI, move_menu.y_add);
    lv_obj_align(labelYI, buttonYI, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelYD, move_menu.y_dec);
    lv_obj_align(labelYD, buttonYD, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelZI, move_menu.z_add);
    lv_obj_align(labelZI, buttonZI, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelZD, move_menu.z_dec);
    lv_obj_align(labelZD, buttonZD, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonXI);
      lv_group_add_obj(g, buttonXD);
      lv_group_add_obj(g, buttonYI);
      lv_group_add_obj(g, buttonYD);
      lv_group_add_obj(g, buttonZI);
      lv_group_add_obj(g, buttonZD);
      lv_group_add_obj(g, buttonV);
      lv_group_add_obj(g, buttonBack);
    }
  #endif

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
