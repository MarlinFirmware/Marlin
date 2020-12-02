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

#define ID_CUSTOM_1   1
#define ID_CUSTOM_2   2
#define ID_CUSTOM_3   3
#define ID_CUSTOM_4   4
#define ID_CUSTOM_5   5
#define ID_CUSTOM_6   6
#define ID_CUSTOM_7   7
#define ID_M_RETURN   8

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_CUSTOM_1:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        #if ENABLED(USER_CMD_1_ENABLE)
          queue.inject_P(PSTR(USER_GCODE_1));
        #endif
      }
      break;
    case ID_CUSTOM_2:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        #if ENABLED(USER_CMD_2_ENABLE)
          queue.inject_P(PSTR(USER_GCODE_2));
        #endif
      }
      break;
    case ID_CUSTOM_3:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        #if ENABLED(USER_CMD_3_ENABLE)
          queue.inject_P(PSTR(USER_GCODE_3));
        #endif
      }
      break;
    case ID_CUSTOM_4:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        #if ENABLED(USER_CMD_4_ENABLE)
          queue.inject_P(PSTR(USER_GCODE_4));
        #endif
      }
      break;
    case ID_CUSTOM_5:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        #if ENABLED(USER_CMD_5_ENABLE)
          queue.inject_P(PSTR(USER_GCODE_5));
        #endif
      }
      break;
    case ID_CUSTOM_6:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        #if ENABLED(USER_CMD_6_ENABLE)
          queue.inject_P(PSTR(USER_GCODE_6));
        #endif
      }
      break;
    case ID_CUSTOM_7:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        #if ENABLED(USER_CMD_7_ENABLE)
          queue.inject_P(PSTR(USER_GCODE_7));
        #endif
      }
      break;
    case ID_M_RETURN:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_more();
        lv_draw_tool();
      }
      break;
  }
}

void lv_draw_more(void) {
  #if ENABLED(USER_CMD_1_ENABLE)
    lv_obj_t *buttonCustom1;
  #endif
  #if ENABLED(USER_CMD_2_ENABLE)
    lv_obj_t *buttonCustom2;
  #endif
  #if ENABLED(USER_CMD_3_ENABLE)
    lv_obj_t *buttonCustom3;
  #endif
  #if ENABLED(USER_CMD_4_ENABLE)
    lv_obj_t *buttonCustom4;
  #endif
  #if ENABLED(USER_CMD_5_ENABLE)
    lv_obj_t *buttonCustom5;
  #endif
  #if ENABLED(USER_CMD_6_ENABLE)
    lv_obj_t *buttonCustom6;
  #endif
  #if ENABLED(USER_CMD_7_ENABLE)
    lv_obj_t *buttonCustom7;
  #endif
  lv_obj_t *buttonBack;

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


  /*Create an Image button and label*/

  #if ENABLED(USER_CMD_1_ENABLE)
    buttonCustom1 = lv_imgbtn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonCustom1, event_handler,ID_CUSTOM_1, NULL,0);
    lv_imgbtn_set_src(buttonCustom1, LV_BTN_STATE_REL, "F:/bmp_custom1.bin");
    lv_imgbtn_set_src(buttonCustom1, LV_BTN_STATE_PR, "F:/bmp_custom1.bin");
    lv_imgbtn_set_style(buttonCustom1, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonCustom1, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_obj_set_pos(buttonCustom1, INTERVAL_V, titleHeight);
    lv_btn_set_layout(buttonCustom1, LV_LAYOUT_OFF);
    lv_obj_t * labelCustom1 = lv_label_create(buttonCustom1, NULL);
  #endif

  #if ENABLED(USER_CMD_2_ENABLE)
    buttonCustom2 = lv_imgbtn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonCustom2, event_handler, ID_CUSTOM_2, NULL, 0);
    lv_imgbtn_set_src(buttonCustom2, LV_BTN_STATE_REL, "F:/bmp_custom2.bin");
    lv_imgbtn_set_src(buttonCustom2, LV_BTN_STATE_PR, "F:/bmp_custom2.bin");
    lv_imgbtn_set_style(buttonCustom2, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonCustom2, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_obj_set_pos(buttonCustom2, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight);
    lv_btn_set_layout(buttonCustom2, LV_LAYOUT_OFF);
    lv_obj_t * labelCustom2 = lv_label_create(buttonCustom2, NULL);
  #endif

  #if ENABLED(USER_CMD_3_ENABLE)
    buttonCustom3 = lv_imgbtn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonCustom3, event_handler, ID_CUSTOM_3, NULL, 0);
    lv_imgbtn_set_src(buttonCustom3, LV_BTN_STATE_REL, "F:/bmp_custom3.bin");
    lv_imgbtn_set_src(buttonCustom3, LV_BTN_STATE_PR, "F:/bmp_custom3.bin");
    lv_imgbtn_set_style(buttonCustom3, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonCustom3, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_obj_set_pos(buttonCustom3, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight);
    lv_btn_set_layout(buttonCustom3, LV_LAYOUT_OFF);
    lv_obj_t * labelCustom3 = lv_label_create(buttonCustom3, NULL);
  #endif

  #if ENABLED(USER_CMD_4_ENABLE)
    buttonCustom4 = lv_imgbtn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonCustom4, event_handler, ID_CUSTOM_4, NULL, 0);
    lv_imgbtn_set_src(buttonCustom4, LV_BTN_STATE_REL, "F:/bmp_custom4.bin");
    lv_imgbtn_set_src(buttonCustom4, LV_BTN_STATE_PR, "F:/bmp_custom4.bin");
    lv_imgbtn_set_style(buttonCustom4, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonCustom4, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_obj_set_pos(buttonCustom4, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);
    lv_btn_set_layout(buttonCustom4, LV_LAYOUT_OFF);
    lv_obj_t * labelCustom4 = lv_label_create(buttonCustom4, NULL);
  #endif

  #if ENABLED(USER_CMD_5_ENABLE)
    buttonCustom5 = lv_imgbtn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonCustom5, event_handler,ID_CUSTOM_5, NULL,0);
    lv_imgbtn_set_src(buttonCustom5, LV_BTN_STATE_REL, "F:/bmp_custom5.bin");
    lv_imgbtn_set_src(buttonCustom5, LV_BTN_STATE_PR, "F:/bmp_custom5.bin");
    lv_imgbtn_set_style(buttonCustom5, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonCustom5, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_obj_set_pos(buttonCustom5, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
    lv_btn_set_layout(buttonCustom5, LV_LAYOUT_OFF);
    lv_obj_t * labelCustom5 = lv_label_create(buttonCustom5, NULL);
  #endif

  #if ENABLED(USER_CMD_6_ENABLE)
    buttonCustom6 = lv_imgbtn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonCustom6, event_handler,ID_CUSTOM_6, NULL,0);
    lv_imgbtn_set_src(buttonCustom6, LV_BTN_STATE_REL, "F:/bmp_custom6.bin");
    lv_imgbtn_set_src(buttonCustom6, LV_BTN_STATE_PR, "F:/bmp_custom6.bin");
    lv_imgbtn_set_style(buttonCustom6, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonCustom6, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_obj_set_pos(buttonCustom6, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
    lv_btn_set_layout(buttonCustom6, LV_LAYOUT_OFF);
    lv_obj_t * labelCustom6 = lv_label_create(buttonCustom6, NULL);
  #endif

  #if ENABLED(USER_CMD_7_ENABLE)
    buttonCustom7 = lv_imgbtn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonCustom7, event_handler,ID_CUSTOM_7, NULL,0);
    lv_imgbtn_set_src(buttonCustom7, LV_BTN_STATE_REL, "F:/bmp_custom7.bin");
    lv_imgbtn_set_src(buttonCustom7, LV_BTN_STATE_PR, "F:/bmp_custom7.bin");
    lv_imgbtn_set_style(buttonCustom7, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonCustom7, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_obj_set_pos(buttonCustom7 ,BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
    lv_btn_set_layout(buttonCustom7, LV_LAYOUT_OFF);
    lv_obj_t * labelCustom7 = lv_label_create(buttonCustom7, NULL);
  #endif

  buttonBack = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler,ID_M_RETURN, NULL,0);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);
  lv_obj_t * label_Back = lv_label_create(buttonBack, NULL);


  if (gCfgItems.multiple_language != 0) {
    #if ENABLED(USER_CMD_1_ENABLE)
      lv_label_set_text(labelCustom1, more_menu.custom1);
      lv_obj_align(labelCustom1, buttonCustom1, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if ENABLED(USER_CMD_2_ENABLE)
      lv_label_set_text(labelCustom2, more_menu.custom2);
      lv_obj_align(labelCustom2, buttonCustom2, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if ENABLED(USER_CMD_3_ENABLE)
      lv_label_set_text(labelCustom3, more_menu.custom3);
      lv_obj_align(labelCustom3, buttonCustom3, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if ENABLED(USER_CMD_4_ENABLE)
      lv_label_set_text(labelCustom4, more_menu.custom4);
      lv_obj_align(labelCustom4, buttonCustom4, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if ENABLED(USER_CMD_5_ENABLE)
      lv_label_set_text(labelCustom5, more_menu.custom5);
      lv_obj_align(labelCustom5, buttonCustom5, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if ENABLED(USER_CMD_6_ENABLE)
      lv_label_set_text(labelCustom6, more_menu.custom6);
      lv_obj_align(labelCustom6, buttonCustom6, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if ENABLED(USER_CMD_7_ENABLE)
      lv_label_set_text(labelCustom7, more_menu.custom7);
      lv_obj_align(labelCustom7, buttonCustom7, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }

  #if HAS_ROTARY_ENCODER
	if (gCfgItems.encoder_enable == true) {
    #if ENABLED(USER_CMD_1_ENABLE)
      lv_group_add_obj(g, buttonCustom1);
    #endif
    #if ENABLED(USER_CMD_2_ENABLE)
      lv_group_add_obj(g, buttonCustom2);
    #endif
    #if ENABLED(USER_CMD_3_ENABLE)
      lv_group_add_obj(g, buttonCustom3);
    #endif
    #if ENABLED(USER_CMD_4_ENABLE)
      lv_group_add_obj(g, buttonCustom4);
    #endif
    #if ENABLED(USER_CMD_5_ENABLE)
      lv_group_add_obj(g, buttonCustom5);
    #endif
    #if ENABLED(USER_CMD_6_ENABLE)
      lv_group_add_obj(g, buttonCustom6);
    #endif
    #if ENABLED(USER_CMD_7_ENABLE)
      lv_group_add_obj(g, buttonCustom7);
    #endif
		lv_group_add_obj(g, buttonBack);
	}
  #endif // HAS_ROTARY_ENCODER
}

void lv_clear_more() {
	#if HAS_ROTARY_ENCODER
	if (gCfgItems.encoder_enable == true) {
		lv_group_remove_all_objs(g);
	}
  	#endif // HAS_ROTARY_ENCODER
	lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
