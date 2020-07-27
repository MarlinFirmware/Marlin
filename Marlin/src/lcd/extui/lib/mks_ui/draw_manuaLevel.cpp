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

//static lv_obj_t *buttonMoveZ,*buttonTest,*buttonZ0,*buttonStop,*buttonReturn;
static lv_obj_t * scr;

#define ID_M_POINT1      1
#define ID_M_POINT2      2
#define ID_M_POINT3      3
#define ID_M_POINT4      4
#define ID_M_POINT5      5
#define ID_MANUAL_RETURN 6

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_M_POINT1:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {

        if (queue.length == 0) {
          if (uiCfg.leveling_first_time) {
            queue.enqueue_now_P(PSTR("G28"));
            uiCfg.leveling_first_time = 0;
          }

          queue.enqueue_now_P(PSTR("G1 Z10"));

          ZERO(public_buf_l);
          sprintf_P(public_buf_l, PSTR("G1 X%d Y%d"), X_MIN_POS + 30, Y_MIN_POS + 30);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G1 Z0"));
        }
      }
      break;
    case ID_M_POINT2:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (queue.length == 0) {
          if (uiCfg.leveling_first_time) {
            queue.enqueue_now_P(PSTR("G28"));
            uiCfg.leveling_first_time = 0;
          }

          queue.enqueue_now_P(PSTR("G1 Z10"));

          ZERO(public_buf_l);
          sprintf_P(public_buf_l, PSTR("G1 X%d Y%d"), X_MAX_POS - 30, Y_MIN_POS + 30);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G1 Z0"));
        }
      }
      break;
    case ID_M_POINT3:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (queue.length == 0) {
          if (uiCfg.leveling_first_time) {
            queue.enqueue_now_P(PSTR("G28"));
            uiCfg.leveling_first_time = 0;
          }

          queue.enqueue_now_P(PSTR("G1 Z10"));

          ZERO(public_buf_l);
          sprintf_P(public_buf_l, PSTR("G1 X%d Y%d"), X_MAX_POS - 30, Y_MAX_POS - 30);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G1 Z0"));
        }
      }

      break;
    case ID_M_POINT4:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (queue.length == 0) {
          if (uiCfg.leveling_first_time) {
            queue.enqueue_now_P(PSTR("G28"));
            uiCfg.leveling_first_time = 0;
          }

          queue.enqueue_now_P(PSTR("G1 Z10"));

          ZERO(public_buf_l);
          sprintf_P(public_buf_l, PSTR("G1 X%d Y%d"), X_MIN_POS + 30, Y_MAX_POS - 30);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G1 Z0"));
        }
      }
      break;
    case ID_M_POINT5:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (queue.length == 0) {
          if (uiCfg.leveling_first_time) {
            queue.enqueue_now_P(PSTR("G28"));
            uiCfg.leveling_first_time = 0;
          }

          queue.enqueue_now_P(PSTR("G1 Z10"));

          ZERO(public_buf_l);
          sprintf_P(public_buf_l, PSTR("G1 X%d Y%d"), X_BED_SIZE / 2, Y_BED_SIZE / 2);
          queue.enqueue_one_now(public_buf_l);
          queue.enqueue_now_P(PSTR("G1 Z0"));
        }
      }

      break;
    case ID_MANUAL_RETURN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_manualLevel();
        lv_draw_tool();
      }
      break;
  }
}


void lv_draw_manualLevel(void) {
  lv_obj_t *buttonPoint1, *buttonPoint2, *buttonPoint3, *buttonPoint4, *buttonPoint5;
  lv_obj_t *buttonBack;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != LEVELING_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = LEVELING_UI;
  }
  disp_state = LEVELING_UI;

  scr = lv_obj_create(NULL, NULL);

  // static lv_style_t tool_style;

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  LV_IMG_DECLARE(bmp_pic);

  /*Create an Image button*/
  buttonPoint1 = lv_imgbtn_create(scr, NULL);
  buttonPoint2 = lv_imgbtn_create(scr, NULL);
  buttonPoint3 = lv_imgbtn_create(scr, NULL);
  buttonPoint4 = lv_imgbtn_create(scr, NULL);
  buttonPoint5 = lv_imgbtn_create(scr, NULL);
  buttonBack   = lv_imgbtn_create(scr, NULL);

  lv_obj_set_event_cb_mks(buttonPoint1, event_handler, ID_M_POINT1, "bmp_leveling1.bin", 0);
  lv_imgbtn_set_src(buttonPoint1, LV_BTN_STATE_REL, &bmp_pic);
  lv_imgbtn_set_src(buttonPoint1, LV_BTN_STATE_PR, &bmp_pic);
  lv_imgbtn_set_style(buttonPoint1, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonPoint1, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_obj_clear_protect(buttonPoint1, LV_PROTECT_FOLLOW);
  #if 1
    lv_obj_set_event_cb_mks(buttonPoint2, event_handler, ID_M_POINT2, "bmp_leveling2.bin", 0);
    lv_imgbtn_set_src(buttonPoint2, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonPoint2, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonPoint2, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonPoint2, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonPoint3, event_handler, ID_M_POINT3, "bmp_leveling3.bin", 0);
    lv_imgbtn_set_src(buttonPoint3, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonPoint3, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonPoint3, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonPoint3, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonPoint4, event_handler, ID_M_POINT4, "bmp_leveling4.bin", 0);
    lv_imgbtn_set_src(buttonPoint4, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonPoint4, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonPoint4, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonPoint4, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonPoint5, event_handler, ID_M_POINT5, "bmp_leveling5.bin", 0);
    lv_imgbtn_set_src(buttonPoint5, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonPoint5, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonPoint5, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonPoint5, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_MANUAL_RETURN, "bmp_return.bin", 0);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
  #endif

  lv_obj_set_pos(buttonPoint1, INTERVAL_V, titleHeight);
  lv_obj_set_pos(buttonPoint2, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight);
  lv_obj_set_pos(buttonPoint3, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight);
  lv_obj_set_pos(buttonPoint4, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
  lv_obj_set_pos(buttonPoint5, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

  /*Create a label on the Image button*/
  lv_btn_set_layout(buttonPoint1, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonPoint2, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonPoint3, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonPoint4, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonPoint5, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

  lv_obj_t * label_Point1 = lv_label_create(buttonPoint1, NULL);
  lv_obj_t * label_Point2 = lv_label_create(buttonPoint2, NULL);
  lv_obj_t * label_Point3 = lv_label_create(buttonPoint3, NULL);
  lv_obj_t * label_Point4 = lv_label_create(buttonPoint4, NULL);
  lv_obj_t * label_Point5 = lv_label_create(buttonPoint5, NULL);
  lv_obj_t * label_Back   = lv_label_create(buttonBack, NULL);

  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(label_Point1, leveling_menu.position1);
    lv_obj_align(label_Point1, buttonPoint1, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Point2, leveling_menu.position2);
    lv_obj_align(label_Point2, buttonPoint2, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Point3, leveling_menu.position3);
    lv_obj_align(label_Point3, buttonPoint3, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Point4, leveling_menu.position4);
    lv_obj_align(label_Point4, buttonPoint4, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Point5, leveling_menu.position5);
    lv_obj_align(label_Point5, buttonPoint5, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }
}

void lv_clear_manualLevel() { lv_obj_del(scr); }

#endif // HAS_TFT_LVGL_UI
