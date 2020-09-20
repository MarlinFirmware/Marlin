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

#include "../../../../MarlinCore.h"
#include "lv_conf.h"
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"
#include "draw_ui.h"
#include "../../../../gcode/queue.h"

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

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  // Create an Image button
  buttonXI   = lv_imgbtn_create(scr, NULL);
  buttonXD   = lv_imgbtn_create(scr, NULL);
  buttonYI   = lv_imgbtn_create(scr, NULL);
  buttonYD   = lv_imgbtn_create(scr, NULL);
  buttonZI   = lv_imgbtn_create(scr, NULL);
  buttonZD   = lv_imgbtn_create(scr, NULL);
  buttonV    = lv_imgbtn_create(scr, NULL);
  buttonBack = lv_imgbtn_create(scr, NULL);

  lv_obj_set_event_cb_mks(buttonXI, event_handler, ID_M_X_P, NULL, 0);
  lv_imgbtn_set_src(buttonXI, LV_BTN_STATE_REL, "F:/bmp_xAdd.bin");
  lv_imgbtn_set_src(buttonXI, LV_BTN_STATE_PR, "F:/bmp_xAdd.bin");
  lv_imgbtn_set_style(buttonXI, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonXI, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_obj_clear_protect(buttonXI, LV_PROTECT_FOLLOW);

  #if 1
    lv_obj_set_event_cb_mks(buttonXD, event_handler, ID_M_X_N, NULL, 0);
    lv_imgbtn_set_src(buttonXD, LV_BTN_STATE_REL, "F:/bmp_xDec.bin");
    lv_imgbtn_set_src(buttonXD, LV_BTN_STATE_PR, "F:/bmp_xDec.bin");
    lv_imgbtn_set_style(buttonXD, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonXD, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonYI, event_handler, ID_M_Y_P, NULL, 0);
    lv_imgbtn_set_src(buttonYI, LV_BTN_STATE_REL, "F:/bmp_yAdd.bin");
    lv_imgbtn_set_src(buttonYI, LV_BTN_STATE_PR, "F:/bmp_yAdd.bin");
    lv_imgbtn_set_style(buttonYI, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonYI, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonYD, event_handler, ID_M_Y_N, NULL, 0);
    lv_imgbtn_set_src(buttonYD, LV_BTN_STATE_REL, "F:/bmp_yDec.bin");
    lv_imgbtn_set_src(buttonYD, LV_BTN_STATE_PR, "F:/bmp_yDec.bin");
    lv_imgbtn_set_style(buttonYD, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonYD, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonZI, event_handler, ID_M_Z_P, NULL, 0);
    lv_imgbtn_set_src(buttonZI, LV_BTN_STATE_REL, "F:/bmp_zAdd.bin");
    lv_imgbtn_set_src(buttonZI, LV_BTN_STATE_PR, "F:/bmp_zAdd.bin");
    lv_imgbtn_set_style(buttonZI, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonZI, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonZD, event_handler, ID_M_Z_N, NULL, 0);
    lv_imgbtn_set_src(buttonZD, LV_BTN_STATE_REL, "F:/bmp_zDec.bin");
    lv_imgbtn_set_src(buttonZD, LV_BTN_STATE_PR, "F:/bmp_zDec.bin");
    lv_imgbtn_set_style(buttonZD, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonZD, LV_BTN_STATE_REL, &tft_style_label_rel);

    //lv_obj_set_event_cb_mks(buttonV, event_handler,ID_T_MORE,"bmp_More.bin",0);
    lv_obj_set_event_cb_mks(buttonV, event_handler, ID_M_STEP, NULL, 0);
    lv_imgbtn_set_style(buttonV, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonV, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_M_RETURN, NULL, 0);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
  #endif // if 1

  lv_obj_set_pos(buttonXI, INTERVAL_V, titleHeight);
  lv_obj_set_pos(buttonYI, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight);
  lv_obj_set_pos(buttonZI, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight);
  lv_obj_set_pos(buttonV, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
  lv_obj_set_pos(buttonXD, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonYD, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonZD, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

  // Create labels on the image buttons
  lv_btn_set_layout(buttonXI, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonXD, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonYI, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonYD, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonZI, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonZD, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonV, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

  lv_obj_t *labelXI = lv_label_create(buttonXI, NULL);
  lv_obj_t *labelXD = lv_label_create(buttonXD, NULL);
  lv_obj_t *labelYI = lv_label_create(buttonYI, NULL);
  lv_obj_t *labelYD = lv_label_create(buttonYD, NULL);
  lv_obj_t *labelZI = lv_label_create(buttonZI, NULL);
  lv_obj_t *labelZD = lv_label_create(buttonZD, NULL);
  labelV = lv_label_create(buttonV, NULL);
  lv_obj_t *label_Back = lv_label_create(buttonBack, NULL);

  if (gCfgItems.multiple_language != 0) {
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

  if ((int)(10 * uiCfg.move_dist) == 1) {
    lv_imgbtn_set_src(buttonV, LV_BTN_STATE_REL, "F:/bmp_step_move0_1.bin");
    lv_imgbtn_set_src(buttonV, LV_BTN_STATE_PR, "F:/bmp_step_move0_1.bin");
  }
  else if ((int)(10 * uiCfg.move_dist) == 10) {
    lv_imgbtn_set_src(buttonV, LV_BTN_STATE_REL, "F:/bmp_step_move1.bin");
    lv_imgbtn_set_src(buttonV, LV_BTN_STATE_PR, "F:/bmp_step_move1.bin");
  }
  else if ((int)(10 * uiCfg.move_dist) == 100) {
    lv_imgbtn_set_src(buttonV, LV_BTN_STATE_REL, "F:/bmp_step_move10.bin");
    lv_imgbtn_set_src(buttonV, LV_BTN_STATE_PR, "F:/bmp_step_move10.bin");
  }
  if (gCfgItems.multiple_language != 0) {
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
