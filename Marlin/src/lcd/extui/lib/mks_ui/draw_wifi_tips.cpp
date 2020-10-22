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

#include <lv_conf.h>
#include "tft_lvgl_configuration.h"

#if ENABLED(USE_WIFI_FUNCTION)

#include "draw_ui.h"

static lv_obj_t * scr;

TIPS_TYPE wifi_tips_type;
TIPS_DISP tips_disp;
tips_menu_def tips_menu;

void lv_draw_wifi_tips(void) {
  static lv_obj_t * text_tips,*wifi_name;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != WIFI_TIPS_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = WIFI_TIPS_UI;
  }
  disp_state = WIFI_TIPS_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);
  lv_refr_now(lv_refr_get_disp_refreshing());

  wifi_name = lv_label_create(scr, (const char *)wifi_list.wifiName[wifi_list.nameIndex]);
  lv_obj_align(wifi_name, NULL, LV_ALIGN_CENTER, 0, -20);

  text_tips = lv_label_create_empty(scr);
  if (wifi_tips_type == TIPS_TYPE_JOINING) {
    lv_label_set_text(text_tips, tips_menu.joining);
    lv_obj_align(text_tips, NULL, LV_ALIGN_CENTER, 0, -60);
  }
  else if (wifi_tips_type == TIPS_TYPE_TAILED_JOIN) {
    lv_label_set_text(text_tips, tips_menu.failedJoin);
    lv_obj_align(text_tips, NULL, LV_ALIGN_CENTER, 0, -60);
  }
  else if (wifi_tips_type == TIPS_TYPE_WIFI_CONECTED) {
    lv_label_set_text(text_tips, tips_menu.wifiConected);
    lv_obj_align(text_tips, NULL, LV_ALIGN_CENTER, 0, -60);
  }

  tips_disp.timer = TIPS_TIMER_START;
  tips_disp.timer_count = 0;
}

void lv_clear_wifi_tips() { lv_obj_del(scr); }

#endif // USE_WIFI_FUNCTION
#endif // HAS_TFT_LVGL_UI
