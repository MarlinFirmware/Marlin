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

#include "draw_ready_print.h"
#include "draw_set.h"
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

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  // Create image buttons
  //buttonWifi     = lv_imgbtn_create(scr, NULL);
  //buttonContinue = lv_imgbtn_create(scr, NULL);
  buttonHomeAll = lv_imgbtn_create(scr, "F:/bmp_zeroAll.bin", INTERVAL_V, titleHeight, event_handler, ID_H_ALL);
  buttonHomeX   = lv_imgbtn_create(scr, "F:/bmp_zeroX.bin", BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_H_X);
  buttonHomeY   = lv_imgbtn_create(scr, "F:/bmp_zeroY.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_H_Y);
  buttonHomeZ   = lv_imgbtn_create(scr, "F:/bmp_zeroZ.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_H_Z);
  buttonOffAll  = lv_imgbtn_create(scr, "F:/bmp_function1.bin", INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_H_OFF_ALL);
  buttonOffXY   = lv_imgbtn_create(scr, "F:/bmp_function1.bin", BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_H_OFF_XY);
  buttonBack    = lv_imgbtn_create(scr, "F:/bmp_return.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_H_RETURN);

  // Create labels on the image buttons
  lv_obj_t *labelHomeAll = lv_label_create_empty(buttonHomeAll);
  lv_obj_t *labelHomeX = lv_label_create_empty(buttonHomeX);
  lv_obj_t *labelHomeY = lv_label_create_empty(buttonHomeY);
  lv_obj_t *labelHomeZ = lv_label_create_empty(buttonHomeZ);
  lv_obj_t *labelOffAll = lv_label_create_empty(buttonOffAll);
  lv_obj_t *labelOffXY = lv_label_create_empty(buttonOffXY);
  lv_obj_t *label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
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
