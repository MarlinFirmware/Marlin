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

#include "lv_conf.h"
#include "draw_ui.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/planner.h"

extern lv_group_t * g;
static lv_obj_t * scr;

#define ID_MANUAL_POS_RETURN   1
#define ID_MANUAL_POS_X1       2
#define ID_MANUAL_POS_Y1       3
#define ID_MANUAL_POS_X2       4
#define ID_MANUAL_POS_Y2       5
#define ID_MANUAL_POS_X3       6
#define ID_MANUAL_POS_Y3       7
#define ID_MANUAL_POS_X4       8
#define ID_MANUAL_POS_Y4       9
#define ID_MANUAL_POS_X5       10
#define ID_MANUAL_POS_Y5       11
#define ID_MANUAL_POS_DOWN     12
#define ID_MANUAL_POS_UP       13

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_MANUAL_POS_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_manual_level_pos_settings();
        draw_return_ui();
      }
      break;
    case ID_MANUAL_POS_X1:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_x1;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_Y1:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y1;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_X2:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_x2;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_Y2:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y2;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_X3:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_x3;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_Y3:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y3;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_X4:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_x4;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_Y4:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y4;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_X5:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y5;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_Y5:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = level_pos_y5;
        lv_clear_manual_level_pos_settings();
        lv_draw_number_key();
      }
      break;
    case ID_MANUAL_POS_UP:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_manual_level_pos_settings();
        lv_draw_manual_level_pos_settings();
      }
      break;
    case ID_MANUAL_POS_DOWN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 1;
        lv_clear_manual_level_pos_settings();
        lv_draw_manual_level_pos_settings();
      }
      break;
  }
}

void lv_draw_manual_level_pos_settings(void) {
  lv_obj_t *buttonBack = NULL, *label_Back = NULL, *buttonTurnPage = NULL, *labelTurnPage = NULL;
  lv_obj_t *labelPoint1Text = NULL, *buttonX1Value = NULL, *labelX1Value = NULL;
  lv_obj_t *buttonY1Value = NULL, *labelY1Value = NULL;
  lv_obj_t *labelPoint2Text = NULL, *buttonX2Value = NULL, *labelX2Value = NULL;
  lv_obj_t *buttonY2Value = NULL, *labelY2Value = NULL;
  lv_obj_t *labelPoint3Text = NULL, *buttonX3Value = NULL, *labelX3Value = NULL;
  lv_obj_t *buttonY3Value = NULL, *labelY3Value = NULL;
  lv_obj_t *labelPoint4Text = NULL, *buttonX4Value = NULL, *labelX4Value = NULL;
  lv_obj_t *buttonY4Value = NULL, *labelY4Value = NULL;
  lv_obj_t *labelPoint5Text = NULL, *buttonX5Value = NULL, *labelX5Value = NULL;
  lv_obj_t *buttonY5Value = NULL, *labelY5Value = NULL;
  lv_obj_t * line1 = NULL, * line2 = NULL, * line3 = NULL, * line4 = NULL;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != MANUAL_LEVELING_POSIGION_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = MANUAL_LEVELING_POSIGION_UI;
  }
  disp_state = MANUAL_LEVELING_POSIGION_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, machine_menu.LevelingParaConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  if (uiCfg.para_ui_page != 1) {
    labelPoint1Text = lv_label_create(scr, NULL);
    lv_obj_set_style(labelPoint1Text, &tft_style_label_rel);
    lv_obj_set_pos(labelPoint1Text, PARA_UI_POS_X, PARA_UI_POS_Y + 10);
    lv_label_set_text(labelPoint1Text, leveling_menu.position1);

    buttonX1Value = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonX1Value, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonX1Value, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonX1Value, event_handler, ID_MANUAL_POS_X1, NULL, 0);
    lv_btn_set_style(buttonX1Value, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonX1Value, LV_BTN_STYLE_PR, &style_para_value);
    labelX1Value = lv_label_create(buttonX1Value, NULL);

    buttonY1Value = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonY1Value, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonY1Value, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonY1Value, event_handler, ID_MANUAL_POS_Y1, NULL, 0);
    lv_btn_set_style(buttonY1Value, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonY1Value, LV_BTN_STYLE_PR, &style_para_value);
    labelY1Value = lv_label_create(buttonY1Value, NULL);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    labelPoint2Text = lv_label_create(scr, NULL);
    lv_obj_set_style(labelPoint2Text, &tft_style_label_rel);
    lv_obj_set_pos(labelPoint2Text, PARA_UI_POS_X, PARA_UI_POS_Y * 2 + 10);
    lv_label_set_text(labelPoint2Text, leveling_menu.position2);

    buttonX2Value = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonX2Value, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonX2Value, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonX2Value, event_handler, ID_MANUAL_POS_X2, NULL, 0);
    lv_btn_set_style(buttonX2Value, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonX2Value, LV_BTN_STYLE_PR, &style_para_value);
    labelX2Value = lv_label_create(buttonX2Value, NULL);

    buttonY2Value = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonY2Value, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonY2Value, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonY2Value, event_handler, ID_MANUAL_POS_Y2, NULL, 0);
    lv_btn_set_style(buttonY2Value, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonY2Value, LV_BTN_STYLE_PR, &style_para_value);
    labelY2Value = lv_label_create(buttonY2Value, NULL);

    line2 = lv_line_create(scr, NULL);
    lv_ex_line(line2, line_points[1]);

    labelPoint3Text = lv_label_create(scr, NULL);
    lv_obj_set_style(labelPoint3Text, &tft_style_label_rel);
    lv_obj_set_pos(labelPoint3Text, PARA_UI_POS_X, PARA_UI_POS_Y * 3 + 10);
    lv_label_set_text(labelPoint3Text, leveling_menu.position3);

    buttonX3Value = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonX3Value, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonX3Value, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonX3Value, event_handler, ID_MANUAL_POS_X3, NULL, 0);
    lv_btn_set_style(buttonX3Value, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonX3Value, LV_BTN_STYLE_PR, &style_para_value);
    labelX3Value = lv_label_create(buttonX3Value, NULL);

    buttonY3Value = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonY3Value, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonY3Value, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonY3Value, event_handler, ID_MANUAL_POS_Y3, NULL, 0);
    lv_btn_set_style(buttonY3Value, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonY3Value, LV_BTN_STYLE_PR, &style_para_value);
    labelY3Value = lv_label_create(buttonY3Value, NULL);

    line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3, line_points[2]);

    labelPoint4Text = lv_label_create(scr, NULL);
    lv_obj_set_style(labelPoint4Text, &tft_style_label_rel);
    lv_obj_set_pos(labelPoint4Text, PARA_UI_POS_X, PARA_UI_POS_Y * 4 + 10);
    lv_label_set_text(labelPoint4Text, leveling_menu.position4);

    buttonX4Value = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonX4Value, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y * 4 + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonX4Value, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonX4Value, event_handler, ID_MANUAL_POS_X4, NULL, 0);
    lv_btn_set_style(buttonX4Value, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonX4Value, LV_BTN_STYLE_PR, &style_para_value);
    labelX4Value = lv_label_create(buttonX4Value, NULL);

    buttonY4Value = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonY4Value, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonY4Value, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonY4Value, event_handler, ID_MANUAL_POS_Y4, NULL, 0);
    lv_btn_set_style(buttonY4Value, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonY4Value, LV_BTN_STYLE_PR, &style_para_value);
    labelY4Value = lv_label_create(buttonY4Value, NULL);

    line4 = lv_line_create(scr, NULL);
    lv_ex_line(line4, line_points[3]);

    buttonTurnPage = lv_btn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonTurnPage, event_handler, ID_MANUAL_POS_DOWN, NULL, 0);
    lv_btn_set_style(buttonTurnPage, LV_BTN_STYLE_REL, &style_para_back);
    lv_btn_set_style(buttonTurnPage, LV_BTN_STYLE_PR, &style_para_back);

    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) {
        lv_group_add_obj(g, buttonX1Value);
        lv_group_add_obj(g, buttonY1Value);
        lv_group_add_obj(g, buttonX2Value);
        lv_group_add_obj(g, buttonY2Value);
        lv_group_add_obj(g, buttonX3Value);
        lv_group_add_obj(g, buttonY3Value);
        lv_group_add_obj(g, buttonX4Value);
        lv_group_add_obj(g, buttonY4Value);
        lv_group_add_obj(g, buttonTurnPage);
      }
    #endif
  }
  else {
    labelPoint5Text = lv_label_create(scr, NULL);
    lv_obj_set_style(labelPoint5Text, &tft_style_label_rel);
    lv_obj_set_pos(labelPoint5Text, PARA_UI_POS_X, PARA_UI_POS_Y + 10);
    lv_label_set_text(labelPoint5Text, leveling_menu.position5);

    buttonX5Value = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonX5Value, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonX5Value, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonX5Value, event_handler, ID_MANUAL_POS_X5, NULL, 0);
    lv_btn_set_style(buttonX5Value, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonX5Value, LV_BTN_STYLE_PR, &style_para_value);
    labelX5Value = lv_label_create(buttonX5Value, NULL);

    buttonY5Value = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonY5Value, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonY5Value, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonY5Value, event_handler, ID_MANUAL_POS_Y5, NULL, 0);
    lv_btn_set_style(buttonY5Value, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonY5Value, LV_BTN_STYLE_PR, &style_para_value);
    labelY5Value = lv_label_create(buttonY5Value, NULL);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    buttonTurnPage = lv_btn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonTurnPage, event_handler, ID_MANUAL_POS_UP, NULL, 0);
    lv_btn_set_style(buttonTurnPage, LV_BTN_STYLE_REL, &style_para_back);
    lv_btn_set_style(buttonTurnPage, LV_BTN_STYLE_PR, &style_para_back);

    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) {
        lv_group_add_obj(g, buttonX5Value);
        lv_group_add_obj(g, buttonY5Value);
        lv_group_add_obj(g, buttonTurnPage);
      }
    #endif
  }

  lv_obj_set_pos(buttonTurnPage, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y);
  lv_obj_set_size(buttonTurnPage, PARA_UI_BACK_BTN_X_SIZE, PARA_UI_BACK_BTN_Y_SIZE);
  labelTurnPage = lv_label_create(buttonTurnPage, NULL);

  buttonBack = lv_btn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_MANUAL_POS_RETURN, NULL, 0);
  lv_btn_set_style(buttonBack, LV_BTN_STYLE_REL, &style_para_back);
  lv_btn_set_style(buttonBack, LV_BTN_STYLE_PR, &style_para_back);
  lv_obj_set_pos(buttonBack, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y);
  lv_obj_set_size(buttonBack, PARA_UI_BACK_BTN_X_SIZE, PARA_UI_BACK_BTN_Y_SIZE);
  label_Back = lv_label_create(buttonBack, NULL);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonBack);
  #endif

  if (gCfgItems.multiple_language != 0) {
    if (uiCfg.para_ui_page != 1) {
      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[0][0]);
      lv_label_set_text(labelX1Value, public_buf_l);
      lv_obj_align(labelX1Value, buttonX1Value, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[0][1]);
      lv_label_set_text(labelY1Value, public_buf_l);
      lv_obj_align(labelY1Value, buttonY1Value, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[1][0]);
      lv_label_set_text(labelX2Value, public_buf_l);
      lv_obj_align(labelX2Value, buttonX2Value, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[1][1]);
      lv_label_set_text(labelY2Value, public_buf_l);
      lv_obj_align(labelY2Value, buttonY2Value, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[2][0]);
      lv_label_set_text(labelX3Value, public_buf_l);
      lv_obj_align(labelX3Value, buttonX3Value, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[2][1]);
      lv_label_set_text(labelY3Value, public_buf_l);
      lv_obj_align(labelY3Value, buttonY3Value, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[3][0]);
      lv_label_set_text(labelX4Value, public_buf_l);
      lv_obj_align(labelX4Value, buttonX4Value, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[3][1]);
      lv_label_set_text(labelY4Value, public_buf_l);
      lv_obj_align(labelY4Value, buttonY4Value, LV_ALIGN_CENTER, 0, 0);

      lv_label_set_text(labelTurnPage, machine_menu.next);
      lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);
    }
    else {
      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[4][0]);
      lv_label_set_text(labelX5Value, public_buf_l);
      lv_obj_align(labelX5Value, buttonX5Value, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.levelingPos[4][1]);
      lv_label_set_text(labelY5Value, public_buf_l);
      lv_obj_align(labelY5Value, buttonY5Value, LV_ALIGN_CENTER, 0, 0);

      lv_label_set_text(labelTurnPage, machine_menu.previous);
      lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);
    }

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);
  }
}

void lv_clear_manual_level_pos_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
