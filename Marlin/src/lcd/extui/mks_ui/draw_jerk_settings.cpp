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

#if BOTH(HAS_TFT_LVGL_UI, HAS_CLASSIC_JERK)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../module/planner.h"
#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_JERK_RETURN = 1,
  ID_JERK_X,
  ID_JERK_Y,
  ID_JERK_Z,
  ID_JERK_E
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_JERK_RETURN:
      lv_clear_jerk_settings();
      draw_return_ui();
      break;
    case ID_JERK_X:
      value = XJerk;
      lv_clear_jerk_settings();
      lv_draw_number_key();
      break;
    case ID_JERK_Y:
      value = YJerk;
      lv_clear_jerk_settings();
      lv_draw_number_key();
      break;
    case ID_JERK_Z:
      value = ZJerk;
      lv_clear_jerk_settings();
      lv_draw_number_key();
      break;
    case ID_JERK_E:
      value = EJerk;
      lv_clear_jerk_settings();
      lv_draw_number_key();
      break;
  }
}

void lv_draw_jerk_settings() {
  scr = lv_screen_create(JERK_UI, machine_menu.JerkConfTitle);

  dtostrf(planner.max_jerk[X_AXIS], 1, 1, public_buf_l);
  lv_screen_menu_item_1_edit(scr, machine_menu.X_Jerk, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_JERK_X, 0, public_buf_l);

  dtostrf(planner.max_jerk[Y_AXIS], 1, 1, public_buf_l);
  lv_screen_menu_item_1_edit(scr, machine_menu.Y_Jerk, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_JERK_Y, 1, public_buf_l);

  dtostrf(planner.max_jerk[Z_AXIS], 1, 1, public_buf_l);
  lv_screen_menu_item_1_edit(scr, machine_menu.Z_Jerk, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_JERK_Z, 2, public_buf_l);

  dtostrf(planner.max_jerk[E_AXIS], 1, 1, public_buf_l);
  lv_screen_menu_item_1_edit(scr, machine_menu.E_Jerk, PARA_UI_POS_X, PARA_UI_POS_Y * 4, event_handler, ID_JERK_E, 3, public_buf_l);

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, event_handler, ID_JERK_RETURN, true);
}

void lv_clear_jerk_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI && HAS_CLASSIC_JERK
