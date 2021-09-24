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

#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_FILAMENT_SET_RETURN = 1,
  ID_FILAMENT_SET_IN_LENGTH,
  ID_FILAMENT_SET_IN_SPEED,
  ID_FILAMENT_SET_OUT_LENGTH,
  ID_FILAMENT_SET_OUT_SPEED,
  ID_FILAMENT_SET_TEMP,
  ID_FILAMENT_SET_DOWN,
  ID_FILAMENT_SET_UP
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_FILAMENT_SET_RETURN:
      uiCfg.para_ui_page = false;
      lv_clear_filament_settings();
      draw_return_ui();
      break;
    case ID_FILAMENT_SET_IN_LENGTH:
      value = load_length;
      lv_clear_filament_settings();
      lv_draw_number_key();
      break;
    case ID_FILAMENT_SET_IN_SPEED:
      value = load_speed;
      lv_clear_filament_settings();
      lv_draw_number_key();
      break;
    case ID_FILAMENT_SET_OUT_LENGTH:
      value = unload_length;
      lv_clear_filament_settings();
      lv_draw_number_key();
      break;
    case ID_FILAMENT_SET_OUT_SPEED:
      value = unload_speed;
      lv_clear_filament_settings();
      lv_draw_number_key();
      break;
    case ID_FILAMENT_SET_TEMP:
      value = filament_temp;
      lv_clear_filament_settings();
      lv_draw_number_key();
      break;
    case ID_FILAMENT_SET_UP:
      uiCfg.para_ui_page = false;
      lv_clear_filament_settings();
      lv_draw_filament_settings();
      break;
    case ID_FILAMENT_SET_DOWN:
      uiCfg.para_ui_page = true;
      lv_clear_filament_settings();
      lv_draw_filament_settings();
      break;
  }
}

void lv_draw_filament_settings() {
  scr = lv_screen_create(FILAMENT_SETTINGS_UI, machine_menu.FilamentConfTitle);

  if (!uiCfg.para_ui_page) {
    itoa(gCfgItems.filamentchange_load_length, public_buf_l, 10);
    lv_screen_menu_item_1_edit(scr, machine_menu.InLength, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_FILAMENT_SET_IN_LENGTH, 0, public_buf_l);

    itoa(gCfgItems.filamentchange_load_speed, public_buf_l, 10);
    lv_screen_menu_item_1_edit(scr, machine_menu.InSpeed, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_FILAMENT_SET_IN_SPEED, 1, public_buf_l);

    itoa(gCfgItems.filamentchange_unload_length, public_buf_l, 10);
    lv_screen_menu_item_1_edit(scr, machine_menu.OutLength, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_FILAMENT_SET_OUT_LENGTH, 2, public_buf_l);

    itoa(gCfgItems.filamentchange_unload_speed, public_buf_l, 10);
    lv_screen_menu_item_1_edit(scr, machine_menu.OutSpeed, PARA_UI_POS_X, PARA_UI_POS_Y * 4, event_handler, ID_FILAMENT_SET_OUT_SPEED, 3, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.next, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_FILAMENT_SET_DOWN, true);
  }
  else {
    itoa(gCfgItems.filament_limit_temp, public_buf_l, 10);
    lv_screen_menu_item_1_edit(scr, machine_menu.FilamentTemperature, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_FILAMENT_SET_TEMP, 0, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.previous, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_FILAMENT_SET_UP, true);
  }

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, event_handler, ID_FILAMENT_SET_RETURN, true);
}

void lv_clear_filament_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
