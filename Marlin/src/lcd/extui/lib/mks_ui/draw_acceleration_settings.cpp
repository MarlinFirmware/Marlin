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

#define ID_ACCE_RETURN  1
#define ID_ACCE_PRINT   2
#define ID_ACCE_RETRA   3
#define ID_ACCE_TRAVEL  4
#define ID_ACCE_X       5
#define ID_ACCE_Y       6
#define ID_ACCE_Z       7
#define ID_ACCE_E0      8
#define ID_ACCE_E1      9
#define ID_ACCE_UP     10
#define ID_ACCE_DOWN   11

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_ACCE_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_acceleration_settings();
        draw_return_ui();
      }
      break;
    case ID_ACCE_PRINT:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = PrintAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_RETRA:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = RetractAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_TRAVEL:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = TravelAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_X:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = XAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_Y:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = YAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_Z:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = ZAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_E0:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = E0Acceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_E1:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = E1Acceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_UP:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_acceleration_settings();
        lv_draw_acceleration_settings();
      }
      break;
    case ID_ACCE_DOWN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 1;
        lv_clear_acceleration_settings();
        lv_draw_acceleration_settings();
      }
      break;
  }
}

void lv_draw_acceleration_settings(void) {
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != ACCELERATION_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = ACCELERATION_UI;
  }
  disp_state = ACCELERATION_UI;

  scr = lv_obj_create(NULL, NULL);
  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);
  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, machine_menu.AccelerationConfTitle);
  lv_refr_now(lv_refr_get_disp_refreshing());

  if (uiCfg.para_ui_page != 1) {
    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%.1f"), planner.settings.acceleration);
    lv_screen_menu_item_1_edit(scr, machine_menu.PrintAcceleration, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_ACCE_PRINT, 0, public_buf_l);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%.1f"), planner.settings.retract_acceleration);
    lv_screen_menu_item_1_edit(scr, machine_menu.RetractAcceleration, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_ACCE_RETRA, 1, public_buf_l);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%.1f"), planner.settings.travel_acceleration);
    lv_screen_menu_item_1_edit(scr, machine_menu.TravelAcceleration, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_ACCE_TRAVEL, 2, public_buf_l);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
    lv_screen_menu_item_1_edit(scr, machine_menu.X_Acceleration, PARA_UI_POS_X, PARA_UI_POS_Y * 4, event_handler, ID_ACCE_X, 3, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.next, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_ACCE_DOWN, true);
  }
  else {
    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
    lv_screen_menu_item_1_edit(scr, machine_menu.Y_Acceleration, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_ACCE_Y, 0, public_buf_l);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
    lv_screen_menu_item_1_edit(scr, machine_menu.Z_Acceleration, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_ACCE_Z, 1, public_buf_l);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
    lv_screen_menu_item_1_edit(scr, machine_menu.E0_Acceleration, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_ACCE_E0, 2, public_buf_l);

    ZERO(public_buf_l);
    sprintf_P(public_buf_l, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(1)]);
    lv_screen_menu_item_1_edit(scr, machine_menu.E1_Acceleration, PARA_UI_POS_X, PARA_UI_POS_Y * 4, event_handler, ID_ACCE_E1, 3, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.previous, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_ACCE_UP, true);
  }

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, event_handler, ID_ACCE_RETURN, true);
}

void lv_clear_acceleration_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
