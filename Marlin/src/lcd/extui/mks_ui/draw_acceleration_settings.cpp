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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../module/planner.h"
#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_ACCE_RETURN = 1,
  ID_ACCE_PRINT,
  ID_ACCE_RETRA,
  ID_ACCE_TRAVEL,
  ID_ACCE_X,
  ID_ACCE_Y,
  ID_ACCE_Z,
  ID_ACCE_E0,
  ID_ACCE_E1,
  ID_ACCE_UP,
  ID_ACCE_DOWN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_ACCE_RETURN:
      uiCfg.para_ui_page = false;
      lv_clear_acceleration_settings();
      draw_return_ui();
      break;
    case ID_ACCE_PRINT:
      value = PrintAcceleration;
      lv_clear_acceleration_settings();
      lv_draw_number_key();
      break;
    case ID_ACCE_RETRA:
      value = RetractAcceleration;
      lv_clear_acceleration_settings();
      lv_draw_number_key();
      break;
    case ID_ACCE_TRAVEL:
      value = TravelAcceleration;
      lv_clear_acceleration_settings();
      lv_draw_number_key();
      break;
    case ID_ACCE_X:
      value = XAcceleration;
      lv_clear_acceleration_settings();
      lv_draw_number_key();
      break;
    case ID_ACCE_Y:
      value = YAcceleration;
      lv_clear_acceleration_settings();
      lv_draw_number_key();
      break;
    case ID_ACCE_Z:
      value = ZAcceleration;
      lv_clear_acceleration_settings();
      lv_draw_number_key();
      break;
    case ID_ACCE_E0:
      value = E0Acceleration;
      lv_clear_acceleration_settings();
      lv_draw_number_key();
      break;
    case ID_ACCE_E1:
      value = E1Acceleration;
      lv_clear_acceleration_settings();
      lv_draw_number_key();
      break;
    case ID_ACCE_UP:
      uiCfg.para_ui_page = false;
      lv_clear_acceleration_settings();
      lv_draw_acceleration_settings();
      break;
    case ID_ACCE_DOWN:
      uiCfg.para_ui_page = true;
      lv_clear_acceleration_settings();
      lv_draw_acceleration_settings();
      break;
  }
}

void lv_draw_acceleration_settings() {
  scr = lv_screen_create(ACCELERATION_UI, machine_menu.AccelerationConfTitle);
  lv_coord_t y = PARA_UI_POS_Y;
  if (!uiCfg.para_ui_page) {
    dtostrf(planner.settings.acceleration, 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.PrintAcceleration, PARA_UI_POS_X, y, event_handler, ID_ACCE_PRINT, 0, public_buf_l);

    y += PARA_UI_POS_Y;
    dtostrf(planner.settings.retract_acceleration, 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.RetractAcceleration, PARA_UI_POS_X, y, event_handler, ID_ACCE_RETRA, 1, public_buf_l);

    y += PARA_UI_POS_Y;
    dtostrf(planner.settings.travel_acceleration, 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.TravelAcceleration, PARA_UI_POS_X, y, event_handler, ID_ACCE_TRAVEL, 2, public_buf_l);

    y += PARA_UI_POS_Y;
    #if HAS_X_AXIS
      itoa(planner.settings.max_acceleration_mm_per_s2[X_AXIS], public_buf_l, 10);
      lv_screen_menu_item_1_edit(scr, machine_menu.X_Acceleration, PARA_UI_POS_X, y, event_handler, ID_ACCE_X, 3, public_buf_l);
    #endif

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.next, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_ACCE_DOWN, true);
  }
  else {
    #if HAS_Y_AXIS
      itoa(planner.settings.max_acceleration_mm_per_s2[Y_AXIS], public_buf_l, 10);
      lv_screen_menu_item_1_edit(scr, machine_menu.Y_Acceleration, PARA_UI_POS_X, y, event_handler, ID_ACCE_Y, 0, public_buf_l);
    #endif

    #if HAS_Z_AXIS
      y += PARA_UI_POS_Y;
      itoa(planner.settings.max_acceleration_mm_per_s2[Z_AXIS], public_buf_l, 10);
      lv_screen_menu_item_1_edit(scr, machine_menu.Z_Acceleration, PARA_UI_POS_X, y, event_handler, ID_ACCE_Z, 1, public_buf_l);
    #endif

    #if HAS_HOTEND
      y += PARA_UI_POS_Y;
      itoa(planner.settings.max_acceleration_mm_per_s2[E_AXIS], public_buf_l, 10);
      lv_screen_menu_item_1_edit(scr, machine_menu.E0_Acceleration, PARA_UI_POS_X, y, event_handler, ID_ACCE_E0, 2, public_buf_l);
    #endif

    #if ENABLED(DISTINCT_E_FACTORS)
      y += PARA_UI_POS_Y;
      itoa(planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(1)], public_buf_l, 10);
      lv_screen_menu_item_1_edit(scr, machine_menu.E1_Acceleration, PARA_UI_POS_X, y, event_handler, ID_ACCE_E1, 3, public_buf_l);
    #endif

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.previous, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_ACCE_UP, true);
  }

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACK_POS_X, PARA_UI_BACK_POS_Y, event_handler, ID_ACCE_RETURN, true);
}

void lv_clear_acceleration_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
