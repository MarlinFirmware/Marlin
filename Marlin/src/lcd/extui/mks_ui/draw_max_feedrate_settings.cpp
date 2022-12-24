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
  ID_FEED_RETURN = 1,
  ID_FEED_X,
  ID_FEED_Y,
  ID_FEED_Z,
  ID_FEED_E0,
  ID_FEED_E1,
  ID_FEED_DOWN,
  ID_FEED_UP
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;

  lv_clear_max_feedrate_settings();
  switch (obj->mks_obj_id) {
    case ID_FEED_RETURN:
      uiCfg.para_ui_page = false;
      draw_return_ui();
      return;
    case ID_FEED_X:
      value = XMaxFeedRate;
      break;
    case ID_FEED_Y:
      value = YMaxFeedRate;
      break;
    case ID_FEED_Z:
      value = ZMaxFeedRate;
      break;
    case ID_FEED_E0:
      value = E0MaxFeedRate;
      break;
    case ID_FEED_E1:
      value = E1MaxFeedRate;
      break;
    case ID_FEED_UP:
      uiCfg.para_ui_page = false;
      lv_draw_max_feedrate_settings();
      return;
    case ID_FEED_DOWN:
      uiCfg.para_ui_page = true;
      lv_draw_max_feedrate_settings();
      return;
  }
  lv_draw_number_key();
}

void lv_draw_max_feedrate_settings() {
  scr = lv_screen_create(MAXFEEDRATE_UI, machine_menu.MaxFeedRateConfTitle);

  if (!uiCfg.para_ui_page) {
    dtostrf(planner.settings.max_feedrate_mm_s[X_AXIS], 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.XMaxFeedRate, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_FEED_X, 0, public_buf_l);

    dtostrf(planner.settings.max_feedrate_mm_s[Y_AXIS], 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.YMaxFeedRate, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_FEED_Y, 1, public_buf_l);

    dtostrf(planner.settings.max_feedrate_mm_s[Z_AXIS], 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.ZMaxFeedRate, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_FEED_Z, 2, public_buf_l);

    dtostrf(planner.settings.max_feedrate_mm_s[E_AXIS], 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.E0MaxFeedRate, PARA_UI_POS_X, PARA_UI_POS_Y * 4, event_handler, ID_FEED_E0, 3, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.next, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_FEED_DOWN, true);
  }
  else {
    dtostrf(planner.settings.max_feedrate_mm_s[E_AXIS_N(1)], 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.E1MaxFeedRate, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_FEED_E1, 0, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.previous, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_FEED_UP, true);
  }

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACK_POS_X, PARA_UI_BACK_POS_Y, event_handler, ID_FEED_RETURN, true);
}

void lv_clear_max_feedrate_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
