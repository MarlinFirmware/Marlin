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

#include "../../../gcode/queue.h"
#include "../../../inc/MarlinConfig.h"

extern const char G28_STR[];

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_M_POINT1 = 1,
  ID_M_POINT2,
  ID_M_POINT3,
  ID_M_POINT4,
  ID_M_POINT5,
  ID_MANUAL_RETURN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;

  switch (obj->mks_obj_id) {
    case ID_M_POINT1 ... ID_M_POINT5:
      if (queue.ring_buffer.empty()) {
        if (uiCfg.leveling_first_time) {
          uiCfg.leveling_first_time = false;
          queue.inject_P(G28_STR);
        }
        const int ind = obj->mks_obj_id - ID_M_POINT1;
        sprintf_P(public_buf_l, PSTR("G1Z10\nG1X%dY%d\nG1Z0"), gCfgItems.trammingPos[ind].x, gCfgItems.trammingPos[ind].y);
        queue.inject(public_buf_l);
      }
      break;
    case ID_MANUAL_RETURN:
      lv_clear_manualLevel();
      lv_draw_tool();
      break;
  }
}

void lv_draw_manualLevel() {
  scr = lv_screen_create(LEVELING_UI);
  // Create an Image button
  lv_obj_t *buttonPoint1 = lv_big_button_create(scr, "F:/bmp_leveling1.bin", leveling_menu.position1, BUTTON_X(0), BUTTON_Y(0), event_handler, ID_M_POINT1);
  lv_obj_clear_protect(buttonPoint1, LV_PROTECT_FOLLOW);
  lv_big_button_create(scr, "F:/bmp_leveling2.bin", leveling_menu.position2, BUTTON_X(1), BUTTON_Y(0), event_handler, ID_M_POINT2);
  lv_big_button_create(scr, "F:/bmp_leveling3.bin", leveling_menu.position3, BUTTON_X(2), BUTTON_Y(0), event_handler, ID_M_POINT3);
  lv_big_button_create(scr, "F:/bmp_leveling4.bin", leveling_menu.position4, BUTTON_X(3), BUTTON_Y(0), event_handler, ID_M_POINT4);
  lv_big_button_create(scr, "F:/bmp_leveling5.bin", leveling_menu.position5, BUTTON_X(0), BUTTON_Y(1), event_handler, ID_M_POINT5);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BUTTON_X(3), BUTTON_Y(1), event_handler, ID_MANUAL_RETURN);
}

void lv_clear_manualLevel() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
