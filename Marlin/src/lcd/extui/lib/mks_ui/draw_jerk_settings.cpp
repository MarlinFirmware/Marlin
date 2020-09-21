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

#if BOTH(HAS_TFT_LVGL_UI, HAS_CLASSIC_JERK)

#include "lv_conf.h"
#include "draw_ui.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/planner.h"

extern lv_group_t * g;
static lv_obj_t * scr;

#define ID_JERK_RETURN  1
#define ID_JERK_X       2
#define ID_JERK_Y       3
#define ID_JERK_Z       4
#define ID_JERK_E       5

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_JERK_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_jerk_settings();
        draw_return_ui();
      }
      break;
    case ID_JERK_X:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = XJerk;
        lv_clear_jerk_settings();
        lv_draw_number_key();
      }
      break;
    case ID_JERK_Y:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = YJerk;
        lv_clear_jerk_settings();
        lv_draw_number_key();
      }
      break;
    case ID_JERK_Z:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = ZJerk;
        lv_clear_jerk_settings();
        lv_draw_number_key();
      }
      break;
    case ID_JERK_E:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = EJerk;
        lv_clear_jerk_settings();
        lv_draw_number_key();
      }
      break;
  }
}

void lv_draw_jerk_settings(void) {
  lv_obj_t *buttonBack = NULL, *label_Back = NULL;
  lv_obj_t *labelXText = NULL, *buttonXValue = NULL, *labelXValue = NULL;
  lv_obj_t *labelYText = NULL, *buttonYValue = NULL, *labelYValue = NULL;
  lv_obj_t *labelZText = NULL, *buttonZValue = NULL, *labelZValue = NULL;
  lv_obj_t *labelEText = NULL, *buttonEValue = NULL, *labelEValue = NULL;
  lv_obj_t * line1 = NULL, * line2 = NULL, * line3 = NULL, * line4 = NULL;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != JERK_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = JERK_UI;
  }
  disp_state = JERK_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, machine_menu.JerkConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  labelXText = lv_label_create(scr, NULL);
  lv_obj_set_style(labelXText, &tft_style_label_rel);
  lv_obj_set_pos(labelXText, PARA_UI_POS_X, PARA_UI_POS_Y + 10);
  lv_label_set_text(labelXText, machine_menu.X_Jerk);

  buttonXValue = lv_btn_create(scr, NULL);
  lv_obj_set_pos(buttonXValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V);
  lv_obj_set_size(buttonXValue, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
  lv_obj_set_event_cb_mks(buttonXValue, event_handler, ID_JERK_X, NULL, 0);
  lv_btn_set_style(buttonXValue, LV_BTN_STYLE_REL, &style_para_value);
  lv_btn_set_style(buttonXValue, LV_BTN_STYLE_PR, &style_para_value);
  labelXValue = lv_label_create(buttonXValue, NULL);

  line1 = lv_line_create(scr, NULL);
  lv_ex_line(line1, line_points[0]);

  labelYText = lv_label_create(scr, NULL);
  lv_obj_set_style(labelYText, &tft_style_label_rel);
  lv_obj_set_pos(labelYText, PARA_UI_POS_X, PARA_UI_POS_Y * 2 + 10);
  lv_label_set_text(labelYText, machine_menu.Y_Jerk);

  buttonYValue = lv_btn_create(scr, NULL);
  lv_obj_set_pos(buttonYValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V);
  lv_obj_set_size(buttonYValue, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
  lv_obj_set_event_cb_mks(buttonYValue, event_handler, ID_JERK_Y, NULL, 0);
  lv_btn_set_style(buttonYValue, LV_BTN_STYLE_REL, &style_para_value);
  lv_btn_set_style(buttonYValue, LV_BTN_STYLE_PR, &style_para_value);
  labelYValue = lv_label_create(buttonYValue, NULL);

  line2 = lv_line_create(scr, NULL);
  lv_ex_line(line2, line_points[1]);

  labelZText = lv_label_create(scr, NULL);
  lv_obj_set_style(labelZText, &tft_style_label_rel);
  lv_obj_set_pos(labelZText, PARA_UI_POS_X, PARA_UI_POS_Y * 3 + 10);
  lv_label_set_text(labelZText, machine_menu.Z_Jerk);

  buttonZValue = lv_btn_create(scr, NULL);
  lv_obj_set_pos(buttonZValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V);
  lv_obj_set_size(buttonZValue, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
  lv_obj_set_event_cb_mks(buttonZValue, event_handler, ID_JERK_Z, NULL, 0);
  lv_btn_set_style(buttonZValue, LV_BTN_STYLE_REL, &style_para_value);
  lv_btn_set_style(buttonZValue, LV_BTN_STYLE_PR, &style_para_value);
  labelZValue = lv_label_create(buttonZValue, NULL);

  line3 = lv_line_create(scr, NULL);
  lv_ex_line(line3, line_points[2]);

  labelEText = lv_label_create(scr, NULL);
  lv_obj_set_style(labelEText, &tft_style_label_rel);
  lv_obj_set_pos(labelEText, PARA_UI_POS_X, PARA_UI_POS_Y * 4 + 10);
  lv_label_set_text(labelEText, machine_menu.E_Jerk);

  buttonEValue = lv_btn_create(scr, NULL);
  lv_obj_set_pos(buttonEValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_VALUE_V);
  lv_obj_set_size(buttonEValue, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
  lv_obj_set_event_cb_mks(buttonEValue, event_handler, ID_JERK_E, NULL, 0);
  lv_btn_set_style(buttonEValue, LV_BTN_STYLE_REL, &style_para_value);
  lv_btn_set_style(buttonEValue, LV_BTN_STYLE_PR, &style_para_value);
  labelEValue = lv_label_create(buttonEValue, NULL);

  line4 = lv_line_create(scr, NULL);
  lv_ex_line(line4, line_points[3]);

  buttonBack = lv_btn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_JERK_RETURN, NULL, 0);
  lv_btn_set_style(buttonBack, LV_BTN_STYLE_REL, &style_para_back);
  lv_btn_set_style(buttonBack, LV_BTN_STYLE_PR, &style_para_back);

  lv_obj_set_pos(buttonBack, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y);
  lv_obj_set_size(buttonBack, PARA_UI_BACK_BTN_X_SIZE, PARA_UI_BACK_BTN_Y_SIZE);
  label_Back = lv_label_create(buttonBack, NULL);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable == true) {
      lv_group_add_obj(g, buttonXValue);
      lv_group_add_obj(g, buttonYValue);
      lv_group_add_obj(g, buttonZValue);
      lv_group_add_obj(g, buttonEValue);
      lv_group_add_obj(g, buttonBack);
    }
  #endif

  if (gCfgItems.multiple_language != 0) {
    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%.1f"), planner.max_jerk[X_AXIS]);
    lv_label_set_text(labelXValue, public_buf_l);
    lv_obj_align(labelXValue, buttonXValue, LV_ALIGN_CENTER, 0, 0);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%.1f"), planner.max_jerk[Y_AXIS]);
    lv_label_set_text(labelYValue, public_buf_l);
    lv_obj_align(labelYValue, buttonYValue, LV_ALIGN_CENTER, 0, 0);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%.1f"), planner.max_jerk[Z_AXIS]);
    lv_label_set_text(labelZValue, public_buf_l);
    lv_obj_align(labelZValue, buttonZValue, LV_ALIGN_CENTER, 0, 0);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%.1f"), planner.max_jerk[E_AXIS]);
    lv_label_set_text(labelEValue, public_buf_l);
    lv_obj_align(labelEValue, buttonEValue, LV_ALIGN_CENTER, 0, 0);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);
  }
}

void lv_clear_jerk_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI && HAS_CLASSIC_JERK
