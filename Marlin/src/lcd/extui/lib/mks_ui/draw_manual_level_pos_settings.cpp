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

#include "../../../../module/planner.h"
#include "../../../../inc/MarlinConfig.h"

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
  lv_obj_t *buttonX1Value = NULL, *labelX1Value = NULL;
  lv_obj_t *buttonY1Value = NULL, *labelY1Value = NULL;
  lv_obj_t *buttonX2Value = NULL, *labelX2Value = NULL;
  lv_obj_t *buttonY2Value = NULL, *labelY2Value = NULL;
  lv_obj_t *buttonX3Value = NULL, *labelX3Value = NULL;
  lv_obj_t *buttonY3Value = NULL, *labelY3Value = NULL;
  lv_obj_t *buttonX4Value = NULL, *labelX4Value = NULL;
  lv_obj_t *buttonY4Value = NULL, *labelY4Value = NULL;
  lv_obj_t *buttonX5Value = NULL, *labelX5Value = NULL;
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

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, machine_menu.LevelingParaConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  if (uiCfg.para_ui_page != 1) {
    (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y + 10, leveling_menu.position1);

    buttonX1Value = lv_btn_create(scr, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_MANUAL_POS_X1);
    labelX1Value = lv_label_create_empty(buttonX1Value);

    buttonY1Value = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_MANUAL_POS_Y1);
    labelY1Value = lv_label_create_empty(buttonY1Value);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 2 + 10, leveling_menu.position2);

    buttonX2Value = lv_btn_create(scr, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_MANUAL_POS_X2);
    labelX2Value = lv_label_create_empty(buttonX2Value);

    buttonY2Value = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_MANUAL_POS_Y2);
    labelY2Value = lv_label_create_empty(buttonY2Value);

    line2 = lv_line_create(scr, NULL);
    lv_ex_line(line2, line_points[1]);

    (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 3 + 10, leveling_menu.position3);

    buttonX3Value = lv_btn_create(scr, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_MANUAL_POS_X3);
    labelX3Value = lv_label_create_empty(buttonX3Value);

    buttonY3Value = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_MANUAL_POS_Y3);
    labelY3Value = lv_label_create_empty(buttonY3Value);

    line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3, line_points[2]);

    (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 4 + 10, leveling_menu.position4);

    buttonX4Value = lv_btn_create(scr, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y * 4 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_MANUAL_POS_X4);
    labelX4Value = lv_label_create_empty(buttonX4Value);

    buttonY4Value = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_MANUAL_POS_Y4);
    labelY4Value = lv_label_create_empty(buttonY4Value);

    line4 = lv_line_create(scr, NULL);
    lv_ex_line(line4, line_points[3]);

    buttonTurnPage = lv_btn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonTurnPage, event_handler, ID_MANUAL_POS_DOWN, NULL, 0);
    lv_btn_set_style_both(buttonTurnPage, &style_para_back);

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
    (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y + 10, leveling_menu.position5);

    buttonX5Value = lv_btn_create(scr, PARA_UI_VALUE_POS_X_2, PARA_UI_POS_Y + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_MANUAL_POS_X5);
    labelX5Value = lv_label_create_empty(buttonX5Value);

    buttonY5Value = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_MANUAL_POS_Y5);
    labelY5Value = lv_label_create_empty(buttonY5Value);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    buttonTurnPage = lv_btn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonTurnPage, event_handler, ID_MANUAL_POS_UP, NULL, 0);
    lv_btn_set_style_both(buttonTurnPage, &style_para_back);

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
  labelTurnPage = lv_label_create_empty(buttonTurnPage);

  buttonBack = lv_btn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_MANUAL_POS_RETURN, NULL, 0);
  lv_btn_set_style_both(buttonBack, &style_para_back);
  lv_obj_set_pos(buttonBack, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y);
  lv_obj_set_size(buttonBack, PARA_UI_BACK_BTN_X_SIZE, PARA_UI_BACK_BTN_Y_SIZE);
  label_Back = lv_label_create_empty(buttonBack);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonBack);
  #endif

  if (gCfgItems.multiple_language) {
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
