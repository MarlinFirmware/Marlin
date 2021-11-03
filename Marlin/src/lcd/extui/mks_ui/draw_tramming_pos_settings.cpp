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
  ID_MANUAL_POS_RETURN = 1,
  ID_MANUAL_POS_X1,
  ID_MANUAL_POS_Y1,
  ID_MANUAL_POS_X2,
  ID_MANUAL_POS_Y2,
  ID_MANUAL_POS_X3,
  ID_MANUAL_POS_Y3,
  ID_MANUAL_POS_X4,
  ID_MANUAL_POS_Y4,
  ID_MANUAL_POS_X5,
  ID_MANUAL_POS_Y5,
  ID_MANUAL_POS_DOWN,
  ID_MANUAL_POS_UP
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_MANUAL_POS_RETURN:
      uiCfg.para_ui_page = false;
      goto_previous_ui();
      return;
    case ID_MANUAL_POS_X1:
      value = level_pos_x1;
      break;
    case ID_MANUAL_POS_Y1:
      value = level_pos_y1;
      break;
    case ID_MANUAL_POS_X2:
      value = level_pos_x2;
      break;
    case ID_MANUAL_POS_Y2:
      value = level_pos_y2;
      break;
    case ID_MANUAL_POS_X3:
      value = level_pos_x3;
      break;
    case ID_MANUAL_POS_Y3:
      value = level_pos_y3;
      break;
    case ID_MANUAL_POS_X4:
      value = level_pos_x4;
      break;
    case ID_MANUAL_POS_Y4:
      value = level_pos_y4;
      break;
    case ID_MANUAL_POS_X5:
      value = level_pos_x5;
      break;
    case ID_MANUAL_POS_Y5:
      value = level_pos_y5;
      break;
    case ID_MANUAL_POS_UP:
      uiCfg.para_ui_page = false;
      lv_clear_tramming_pos_settings();
      lv_draw_tramming_pos_settings();
      return;
    case ID_MANUAL_POS_DOWN:
      uiCfg.para_ui_page = true;
      lv_clear_tramming_pos_settings();
      lv_draw_tramming_pos_settings();
      return;
  }
  lv_clear_tramming_pos_settings();
  lv_draw_number_key();
}

void lv_draw_tramming_pos_settings() {
  char buf2[50];

  scr = lv_screen_create(MANUAL_LEVELING_POSITION_UI, machine_menu.LevelingParaConfTitle);

  if (!uiCfg.para_ui_page) {
    itoa(gCfgItems.trammingPos[0].x, public_buf_l, 10);
    itoa(gCfgItems.trammingPos[0].y, buf2, 10);
    lv_screen_menu_item_2_edit(scr, leveling_menu.position1, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_MANUAL_POS_Y1, 0, buf2, ID_MANUAL_POS_X1, public_buf_l);

    itoa(gCfgItems.trammingPos[1].x, public_buf_l, 10);
    itoa(gCfgItems.trammingPos[1].y, buf2, 10);
    lv_screen_menu_item_2_edit(scr, leveling_menu.position2, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_MANUAL_POS_Y2, 1, buf2, ID_MANUAL_POS_X2, public_buf_l);

    itoa(gCfgItems.trammingPos[2].x, public_buf_l, 10);
    itoa(gCfgItems.trammingPos[2].y, buf2, 10);
    lv_screen_menu_item_2_edit(scr, leveling_menu.position3, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_MANUAL_POS_Y3, 2, buf2, ID_MANUAL_POS_X3, public_buf_l);

    itoa(gCfgItems.trammingPos[3].x, public_buf_l, 10);
    itoa(gCfgItems.trammingPos[3].y, buf2, 10);
    lv_screen_menu_item_2_edit(scr, leveling_menu.position4, PARA_UI_POS_X, PARA_UI_POS_Y * 4, event_handler, ID_MANUAL_POS_Y4, 3, buf2, ID_MANUAL_POS_X4, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.next, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_MANUAL_POS_DOWN, true);
  }
  else {
    itoa(gCfgItems.trammingPos[4].x, public_buf_l, 10);
    itoa(gCfgItems.trammingPos[4].y, buf2, 10);
    lv_screen_menu_item_2_edit(scr, leveling_menu.position5, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_MANUAL_POS_Y5, 0, buf2, ID_MANUAL_POS_X5, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.previous, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_MANUAL_POS_UP, true);
  }

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACK_POS_X + 10, PARA_UI_BACK_POS_Y, event_handler, ID_MANUAL_POS_RETURN, true);
}

void lv_clear_tramming_pos_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
