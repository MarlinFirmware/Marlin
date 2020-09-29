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
#include "draw_ready_print.h"
#include "draw_set.h"
#include "lv_conf.h"
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"
#include "draw_ui.h"
#include "../../../../gcode/queue.h"

extern lv_group_t * g;
static lv_obj_t * scr;

#define ID_H_ALL      1
#define ID_H_X        2
#define ID_H_Y        3
#define ID_H_Z        4
#define ID_H_RETURN   5
#define ID_H_OFF_ALL  6
#define ID_H_OFF_XY   7

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_H_ALL:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        queue.inject_P(PSTR("G28"));
      }
      break;
    case ID_H_X:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        queue.inject_P(PSTR("G28 X0"));
      }
      break;
    case ID_H_Y:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        queue.inject_P(PSTR("G28 Y0"));
      }
      break;
    case ID_H_Z:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        queue.inject_P(PSTR("G28 Z0"));
      }
      break;
    case ID_H_OFF_ALL:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        queue.inject_P(PSTR("M84"));
      }
      break;
    case ID_H_OFF_XY:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        queue.inject_P(PSTR("M84 X Y"));
      }
      break;
    case ID_H_RETURN:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_home();
        lv_draw_tool();
      }
      break;
  }
}

void lv_draw_home(void) {
  lv_obj_t *buttonHomeAll, *buttonHomeX, *buttonHomeY, *buttonHomeZ;
  lv_obj_t *buttonBack;
  lv_obj_t *buttonOffAll, *buttonOffXY;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != ZERO_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = ZERO_UI;
  }
  disp_state = ZERO_UI;

  scr = lv_obj_create(NULL, NULL);

  //static lv_style_t tool_style;

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  // Create image buttons
  //buttonWifi     = lv_imgbtn_create(scr, NULL);
  buttonHomeAll    = lv_imgbtn_create(scr, NULL);
  buttonHomeX      = lv_imgbtn_create(scr, NULL);
  //buttonContinue = lv_imgbtn_create(scr, NULL);
  buttonHomeY      = lv_imgbtn_create(scr, NULL);
  buttonHomeZ      = lv_imgbtn_create(scr, NULL);
  buttonOffAll     = lv_imgbtn_create(scr, NULL);
  buttonOffXY      = lv_imgbtn_create(scr, NULL);
  buttonBack       = lv_imgbtn_create(scr, NULL);

  #if 1
    lv_obj_set_event_cb_mks(buttonHomeAll, event_handler,ID_H_ALL, NULL,0);
    lv_imgbtn_set_src(buttonHomeAll, LV_BTN_STATE_REL, "F:/bmp_zeroAll.bin");
    lv_imgbtn_set_src(buttonHomeAll, LV_BTN_STATE_PR, "F:/bmp_zeroAll.bin");
    lv_imgbtn_set_style(buttonHomeAll, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonHomeAll, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonHomeX, event_handler, ID_H_X, NULL, 0);
    lv_imgbtn_set_src(buttonHomeX, LV_BTN_STATE_REL, "F:/bmp_zeroX.bin");
    lv_imgbtn_set_src(buttonHomeX, LV_BTN_STATE_PR, "F:/bmp_zeroX.bin");
    lv_imgbtn_set_style(buttonHomeX, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonHomeX, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonHomeY, event_handler, ID_H_Y, NULL, 0);
    lv_imgbtn_set_src(buttonHomeY, LV_BTN_STATE_REL, "F:/bmp_zeroY.bin");
    lv_imgbtn_set_src(buttonHomeY, LV_BTN_STATE_PR, "F:/bmp_zeroY.bin");
    lv_imgbtn_set_style(buttonHomeY, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonHomeY, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonHomeZ, event_handler, ID_H_Z, NULL, 0);
    lv_imgbtn_set_src(buttonHomeZ, LV_BTN_STATE_REL, "F:/bmp_zeroZ.bin");
    lv_imgbtn_set_src(buttonHomeZ, LV_BTN_STATE_PR, "F:/bmp_zeroZ.bin");
    lv_imgbtn_set_style(buttonHomeZ, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonHomeZ, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonOffAll, event_handler,ID_H_OFF_ALL, NULL,0);
    lv_imgbtn_set_src(buttonOffAll, LV_BTN_STATE_REL, "F:/bmp_function1.bin");
    lv_imgbtn_set_src(buttonOffAll, LV_BTN_STATE_PR, "F:/bmp_function1.bin");
    lv_imgbtn_set_style(buttonOffAll, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonOffAll, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonOffXY, event_handler,ID_H_OFF_XY, NULL,0);
    lv_imgbtn_set_src(buttonOffXY, LV_BTN_STATE_REL, "F:/bmp_function1.bin");
    lv_imgbtn_set_src(buttonOffXY, LV_BTN_STATE_PR, "F:/bmp_function1.bin");
    lv_imgbtn_set_style(buttonOffXY, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonOffXY, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonBack, event_handler,ID_H_RETURN, NULL,0);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
  #endif

  lv_obj_set_pos(buttonHomeAll, INTERVAL_V, titleHeight);
  lv_obj_set_pos(buttonHomeX, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight);
  lv_obj_set_pos(buttonHomeY, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight);
  lv_obj_set_pos(buttonHomeZ, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
  lv_obj_set_pos(buttonOffAll, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonOffXY, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

  // Create labels on the image buttons
  lv_btn_set_layout(buttonHomeAll, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonHomeX, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonHomeY, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonHomeZ, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonOffAll, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonOffXY, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

  lv_obj_t *labelHomeAll = lv_label_create(buttonHomeAll, NULL);
  lv_obj_t *labelHomeX = lv_label_create(buttonHomeX, NULL);
  lv_obj_t *labelHomeY = lv_label_create(buttonHomeY, NULL);
  lv_obj_t *labelHomeZ = lv_label_create(buttonHomeZ, NULL);
  lv_obj_t *labelOffAll = lv_label_create(buttonOffAll, NULL);
  lv_obj_t *labelOffXY = lv_label_create(buttonOffXY, NULL);
  lv_obj_t *label_Back = lv_label_create(buttonBack, NULL);

  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(labelHomeAll, home_menu.home_all);
    lv_obj_align(labelHomeAll, buttonHomeAll, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelHomeX, home_menu.home_x);
    lv_obj_align(labelHomeX, buttonHomeX, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelHomeY, home_menu.home_y);
    lv_obj_align(labelHomeY, buttonHomeY, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelHomeZ, home_menu.home_z);
    lv_obj_align(labelHomeZ, buttonHomeZ, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelOffAll, set_menu.motoroff);
    lv_obj_align(labelOffAll, buttonOffAll, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelOffXY, set_menu.motoroffXY);
    lv_obj_align(labelOffXY, buttonOffXY, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonHomeAll);
      lv_group_add_obj(g, buttonHomeX);
      lv_group_add_obj(g, buttonHomeY);
      lv_group_add_obj(g, buttonHomeZ);
      lv_group_add_obj(g, buttonOffAll);
      lv_group_add_obj(g, buttonOffXY);
      lv_group_add_obj(g, buttonBack);
    }
  #endif
}

void lv_clear_home() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
