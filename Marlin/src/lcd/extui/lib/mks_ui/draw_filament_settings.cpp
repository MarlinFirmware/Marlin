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

#include "../../../../inc/MarlinConfig.h"

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
  lv_obj_t *buttonInLengthValue = NULL, *labelInLengthValue = NULL;
  lv_obj_t *buttonInSpeedValue = NULL, *labelInSpeedValue = NULL;
  lv_obj_t *buttonOutLengthValue = NULL, *labelOutLengthValue = NULL;
  lv_obj_t *buttonOutSpeedValue = NULL, *labelOutSpeedValue = NULL;
  lv_obj_t *buttonTemperValue = NULL, *labelTemperValue = NULL;
  lv_obj_t *line1 = NULL, *line2 = NULL, *line3 = NULL, *line4 = NULL;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != FILAMENT_SETTINGS_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = FILAMENT_SETTINGS_UI;
  }
  disp_state = FILAMENT_SETTINGS_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, machine_menu.FilamentConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  if (uiCfg.para_ui_page != 1) {
    (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y + 10, machine_menu.InLength);

    buttonInLengthValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_FILAMENT_SET_IN_LENGTH);
    labelInLengthValue = lv_label_create_empty(buttonInLengthValue);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 2 + 10, machine_menu.InSpeed);

    buttonInSpeedValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_FILAMENT_SET_IN_SPEED);
    labelInSpeedValue = lv_label_create_empty(buttonInSpeedValue);

    line2 = lv_line_create(scr, NULL);
    lv_ex_line(line2, line_points[1]);

    (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 3 + 6, machine_menu.OutLength);

    buttonOutLengthValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_FILAMENT_SET_OUT_LENGTH);
    labelOutLengthValue = lv_label_create_empty(buttonOutLengthValue);

    line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3, line_points[2]);

    (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 4 + 10, machine_menu.OutSpeed);

    buttonOutSpeedValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_FILAMENT_SET_OUT_SPEED);
    labelOutSpeedValue = lv_label_create_empty(buttonOutSpeedValue);

    line4 = lv_line_create(scr, NULL);
    lv_ex_line(line4, line_points[3]);

    buttonTurnPage = lv_btn_create_back(scr, event_handler, ID_FILAMENT_SET_DOWN);

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
    (void)lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y + 10, machine_menu.FilamentTemperature);

    buttonTemperValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V_2, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_FILAMENT_SET_TEMP);
    labelTemperValue = lv_label_create_empty(buttonTemperValue);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    buttonTurnPage = lv_btn_create_back(scr, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, PARA_UI_BACK_BTN_X_SIZE, PARA_UI_BACK_BTN_Y_SIZE, event_handler, ID_FILAMENT_SET_UP);

    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) {
        lv_group_add_obj(g, buttonTemperValue);
        lv_group_add_obj(g, buttonTurnPage);
      }
    #endif
  }

  labelTurnPage = lv_label_create_empty(buttonTurnPage);

  buttonBack = lv_btn_create_back(scr, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, PARA_UI_BACK_BTN_X_SIZE, PARA_UI_BACK_BTN_Y_SIZE, event_handler, ID_FILAMENT_SET_RETURN);
  label_Back = lv_label_create_empty(buttonBack);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonBack);
  #endif

  if (gCfgItems.multiple_language) {
    if (uiCfg.para_ui_page != 1) {
      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.filamentchange_load_length);
      lv_label_set_text(labelInLengthValue, public_buf_l);
      lv_obj_align(labelInLengthValue, buttonInLengthValue, LV_ALIGN_CENTER, 0, 0);

      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.filamentchange_load_speed);
      lv_label_set_text(labelInSpeedValue, public_buf_l);
      lv_obj_align(labelInSpeedValue, buttonInSpeedValue, LV_ALIGN_CENTER, 0, 0);

      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.filamentchange_unload_length);
      lv_label_set_text(labelOutLengthValue, public_buf_l);
      lv_obj_align(labelOutLengthValue, buttonOutLengthValue, LV_ALIGN_CENTER, 0, 0);

      sprintf_P(public_buf_l, PSTR("%d"), gCfgItems.filamentchange_unload_speed);
      lv_label_set_text(labelOutSpeedValue, public_buf_l);
      lv_obj_align(labelOutSpeedValue, buttonOutSpeedValue, LV_ALIGN_CENTER, 0, 0);

      lv_label_set_text(labelTurnPage, machine_menu.next);
      lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);
    }
    else {
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
