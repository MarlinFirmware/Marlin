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

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_ADVANCE_RETURN = 1,
  ID_PAUSE_POS,
  ID_WIFI_PARA,
  ID_FILAMENT_SETTINGS,
  ID_ENCODER_SETTINGS
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_ADVANCE_RETURN:
      lv_clear_advance_settings();
      draw_return_ui();
      break;
    case ID_PAUSE_POS:
      lv_clear_advance_settings();
      lv_draw_pause_position();
      break;
    case ID_FILAMENT_SETTINGS:
      lv_clear_advance_settings();
      lv_draw_filament_settings();
      break;
    #if ENABLED(USE_WIFI_FUNCTION)
      case ID_WIFI_PARA:
      lv_clear_advance_settings();
      lv_draw_wifi_settings();
      break;
    #endif
    #if HAS_ROTARY_ENCODER
      case ID_ENCODER_SETTINGS:
        lv_clear_advance_settings();
        lv_draw_encoder_settings();
        break;
    #endif
  }
}

void lv_draw_advance_settings(void) {
  scr = lv_screen_create(ADVANCED_UI, machine_menu.AdvancedConfTitle);

  int index = 0;
  lv_screen_menu_item(scr, machine_menu.PausePosition, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_PAUSE_POS, index++);
  lv_screen_menu_item(scr, machine_menu.FilamentConf, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_FILAMENT_SETTINGS, index++);
  #if ENABLED(USE_WIFI_FUNCTION)
    lv_screen_menu_item(scr, machine_menu.WifiSettings, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_WIFI_PARA, index++);
  #endif
  #if HAS_ROTARY_ENCODER
    lv_screen_menu_item(scr, machine_menu.EncoderSettings, PARA_UI_POS_X, PARA_UI_POS_Y * (index + 1), event_handler, ID_ENCODER_SETTINGS, index);
    index++;
  #endif

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACL_POS_X + 10, PARA_UI_BACL_POS_Y, event_handler, ID_ADVANCE_RETURN, true);
}

void lv_clear_advance_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
