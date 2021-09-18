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
  ID_PAUSE_RETURN = 1,
  ID_PAUSE_X,
  ID_PAUSE_Y,
  ID_PAUSE_Z
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  lv_clear_pause_position();
  switch (obj->mks_obj_id) {
    case ID_PAUSE_RETURN:
      draw_return_ui();
      return;
    case ID_PAUSE_X:
      value = pause_pos_x;
      break;
    case ID_PAUSE_Y:
      value = pause_pos_y;
      break;
    case ID_PAUSE_Z:
      value = pause_pos_z;
      break;
  }
  lv_draw_number_key();
}

void lv_draw_pause_position() {
  scr = lv_screen_create(PAUSE_POS_UI, machine_menu.PausePosText);

  dtostrf(gCfgItems.pausePosX, 1, 1, public_buf_l);
  lv_screen_menu_item_1_edit(scr, machine_menu.xPos, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_PAUSE_X, 0, public_buf_l);

  dtostrf(gCfgItems.pausePosY, 1, 1, public_buf_l);
  lv_screen_menu_item_1_edit(scr, machine_menu.yPos, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_PAUSE_Y, 1, public_buf_l);

  dtostrf(gCfgItems.pausePosZ, 1, 1, public_buf_l);
  lv_screen_menu_item_1_edit(scr, machine_menu.zPos, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_PAUSE_Z, 2, public_buf_l);

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, event_handler, ID_PAUSE_RETURN, true);
}

void lv_clear_pause_position() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
