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

#define ID_FILAMENT_SET_RETURN      1
#define ID_FILAMENT_SET_IN_LENGTH   2
#define ID_FILAMENT_SET_IN_SPEED    3
#define ID_FILAMENT_SET_OUT_LENGTH  4
#define ID_FILAMENT_SET_OUT_SPEED   5
#define ID_FILAMENT_SET_TEMP        6
#define ID_FILAMENT_SET_DOWN        12
#define ID_FILAMENT_SET_UP          13

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_FILAMENT_SET_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_filament_settings();
        draw_return_ui();
      }
      break;
    case ID_FILAMENT_SET_IN_LENGTH:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = load_length;
        lv_clear_filament_settings();
        lv_draw_number_key();
      }
      break;
    case ID_FILAMENT_SET_IN_SPEED:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = load_speed;
        lv_clear_filament_settings();
        lv_draw_number_key();
      }
      break;
    case ID_FILAMENT_SET_OUT_LENGTH:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = unload_length;
        lv_clear_filament_settings();
        lv_draw_number_key();
      }
      break;
    case ID_FILAMENT_SET_OUT_SPEED:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = unload_speed;
        lv_clear_filament_settings();
        lv_draw_number_key();
      }
      break;
    case ID_FILAMENT_SET_TEMP:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = filament_temp;
        lv_clear_filament_settings();
        lv_draw_number_key();
      }
      break;
    case ID_FILAMENT_SET_UP:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_filament_settings();
        lv_draw_filament_settings();
      }
      break;
    case ID_FILAMENT_SET_DOWN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 1;
        lv_clear_filament_settings();
        lv_draw_filament_settings();
      }
      break;
  }
}

void lv_draw_filament_settings(void) {
  lv_obj_t *buttonBack = NULL, *label_Back = NULL, *buttonTurnPage = NULL, *labelTurnPage = NULL;
  lv_obj_t *labelInLengthText = NULL, *buttonInLengthValue = NULL, *labelInLengthValue = NULL;
  lv_obj_t *labelInSpeedText = NULL, *buttonInSpeedValue = NULL, *labelInSpeedValue = NULL;
  lv_obj_t *labelOutLengthText = NULL, *buttonOutLengthValue = NULL, *labelOutLengthValue = NULL;
  lv_obj_t *labelOutSpeedText = NULL, *buttonOutSpeedValue = NULL, *labelOutSpeedValue = NULL;
  lv_obj_t *labelTemperText = NULL, *buttonTemperValue = NULL, *labelTemperValue = NULL;
  lv_obj_t * line1 = NULL, * line2 = NULL, * line3 = NULL, * line4 = NULL;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != FILAMENT_SETTINGS_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = FILAMENT_SETTINGS_UI;
  }
  disp_state = FILAMENT_SETTINGS_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, machine_menu.FilamentConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  if (uiCfg.para_ui_page != 1) {
    labelInLengthText = lv_label_create(scr, NULL);
    lv_obj_set_style(labelInLengthText, &tft_style_label_rel);
    lv_obj_set_pos(labelInLengthText, PARA_UI_POS_X, PARA_UI_POS_Y + 10);
    lv_label_set_text(labelInLengthText, machine_menu.InLength);

    buttonInLengthValue = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonInLengthValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonInLengthValue, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonInLengthValue, event_handler, ID_FILAMENT_SET_IN_LENGTH, NULL, 0);
    lv_btn_set_style(buttonInLengthValue, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonInLengthValue, LV_BTN_STYLE_PR, &style_para_value);
    labelInLengthValue = lv_label_create(buttonInLengthValue, NULL);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    labelInSpeedText = lv_label_create(scr, NULL);
    lv_obj_set_style(labelInSpeedText, &tft_style_label_rel);
    lv_obj_set_pos(labelInSpeedText, PARA_UI_POS_X, PARA_UI_POS_Y * 2 + 10);
    lv_label_set_text(labelInSpeedText, machine_menu.InSpeed);

    buttonInSpeedValue = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonInSpeedValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonInSpeedValue, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonInSpeedValue, event_handler, ID_FILAMENT_SET_IN_SPEED, NULL, 0);
    lv_btn_set_style(buttonInSpeedValue, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonInSpeedValue, LV_BTN_STYLE_PR, &style_para_value);
    labelInSpeedValue = lv_label_create(buttonInSpeedValue, NULL);

    line2 = lv_line_create(scr, NULL);
    lv_ex_line(line2, line_points[1]);

    labelOutLengthText = lv_label_create(scr, NULL);
    lv_obj_set_style(labelOutLengthText, &tft_style_label_rel);
    lv_obj_set_pos(labelOutLengthText, PARA_UI_POS_X, PARA_UI_POS_Y * 3 + 6);
    lv_label_set_text(labelOutLengthText, machine_menu.OutLength);

    buttonOutLengthValue = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonOutLengthValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonOutLengthValue, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonOutLengthValue, event_handler, ID_FILAMENT_SET_OUT_LENGTH, NULL, 0);
    lv_btn_set_style(buttonOutLengthValue, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonOutLengthValue, LV_BTN_STYLE_PR, &style_para_value);
    labelOutLengthValue = lv_label_create(buttonOutLengthValue, NULL);

    line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3, line_points[2]);

    labelOutSpeedText = lv_label_create(scr, NULL);
    lv_obj_set_style(labelOutSpeedText, &tft_style_label_rel);
    lv_obj_set_pos(labelOutSpeedText, PARA_UI_POS_X, PARA_UI_POS_Y * 4 + 10);
    lv_label_set_text(labelOutSpeedText, machine_menu.OutSpeed);

    buttonOutSpeedValue = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonOutSpeedValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonOutSpeedValue, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonOutSpeedValue, event_handler, ID_FILAMENT_SET_OUT_SPEED, NULL, 0);
    lv_btn_set_style(buttonOutSpeedValue, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonOutSpeedValue, LV_BTN_STYLE_PR, &style_para_value);
    labelOutSpeedValue = lv_label_create(buttonOutSpeedValue, NULL);

    line4 = lv_line_create(scr, NULL);
    lv_ex_line(line4, line_points[3]);

    buttonTurnPage = lv_btn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonTurnPage, event_handler, ID_FILAMENT_SET_DOWN, NULL, 0);
    lv_btn_set_style(buttonTurnPage, LV_BTN_STYLE_REL, &style_para_back);
    lv_btn_set_style(buttonTurnPage, LV_BTN_STYLE_PR, &style_para_back);

    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) {
        lv_group_add_obj(g, buttonInLengthValue);
        lv_group_add_obj(g, buttonInSpeedValue);
        lv_group_add_obj(g, buttonOutLengthValue);
        lv_group_add_obj(g, buttonOutSpeedValue);
        lv_group_add_obj(g, buttonTurnPage);
      }
    #endif
  }
  else {
    labelTemperText = lv_label_create(scr, NULL);
    lv_obj_set_style(labelTemperText, &tft_style_label_rel);
    lv_obj_set_pos(labelTemperText, PARA_UI_POS_X, PARA_UI_POS_Y + 10);
    lv_label_set_text(labelTemperText, machine_menu.FilamentTemperature);

    buttonTemperValue = lv_btn_create(scr, NULL);
    lv_obj_set_pos(buttonTemperValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V_2);
    lv_obj_set_size(buttonTemperValue, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE);
    lv_obj_set_event_cb_mks(buttonTemperValue, event_handler, ID_FILAMENT_SET_TEMP, NULL, 0);
    lv_btn_set_style(buttonTemperValue, LV_BTN_STYLE_REL, &style_para_value);
    lv_btn_set_style(buttonTemperValue, LV_BTN_STYLE_PR, &style_para_value);
    labelTemperValue = lv_label_create(buttonTemperValue, NULL);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    buttonTurnPage = lv_btn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonTurnPage, event_handler, ID_FILAMENT_SET_UP, NULL, 0);
    lv_btn_set_style(buttonTurnPage, LV_BTN_STYLE_REL, &style_para_back);
    lv_btn_set_style(buttonTurnPage, LV_BTN_STYLE_PR, &style_para_back);

    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) {
        lv_group_add_obj(g, buttonTemperValue);
        lv_group_add_obj(g, buttonTurnPage);
      }
    #endif
  }

  lv_obj_set_pos(buttonTurnPage, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y);
  lv_obj_set_size(buttonTurnPage, PARA_UI_BACK_BTN_X_SIZE, PARA_UI_BACK_BTN_Y_SIZE);
  labelTurnPage = lv_label_create(buttonTurnPage, NULL);

  buttonBack = lv_btn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_FILAMENT_SET_RETURN, NULL, 0);
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
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.filamentchange_load_length);
      lv_label_set_text(labelInLengthValue, public_buf_l);
      lv_obj_align(labelInLengthValue, buttonInLengthValue, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.filamentchange_load_speed);
      lv_label_set_text(labelInSpeedValue, public_buf_l);
      lv_obj_align(labelInSpeedValue, buttonInSpeedValue, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.filamentchange_unload_length);
      lv_label_set_text(labelOutLengthValue, public_buf_l);
      lv_obj_align(labelOutLengthValue, buttonOutLengthValue, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.filamentchange_unload_speed);
      lv_label_set_text(labelOutSpeedValue, public_buf_l);
      lv_obj_align(labelOutSpeedValue, buttonOutSpeedValue, LV_ALIGN_CENTER, 0, 0);

      lv_label_set_text(labelTurnPage, machine_menu.next);
      lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);
    }
    else {
      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.filament_limit_temper);
      lv_label_set_text(labelTemperValue, public_buf_l);
      lv_obj_align(labelTemperValue, buttonTemperValue, LV_ALIGN_CENTER, 0, 0);

      lv_label_set_text(labelTurnPage, machine_menu.previous);
      lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);
    }

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);
  }
}

void lv_clear_filament_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
