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
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"

#include "../../../../gcode/queue.h"
#include "../../../../module/temperature.h"
#include "../../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_T_PRE_HEAT = 1,
  ID_T_EXTRUCT,
  ID_T_MOV,
  ID_T_HOME,
  ID_T_LEVELING,
  ID_T_FILAMENT,
  ID_T_MORE,
  ID_T_RETURN
};

#if ENABLED(MKS_TEST)
  extern uint8_t curent_disp_ui;
#endif

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_T_PRE_HEAT:
      lv_clear_tool();
      lv_draw_preHeat();
      break;
    case ID_T_EXTRUCT:
      lv_clear_tool();
      lv_draw_extrusion();
      break;
    case ID_T_MOV:
      lv_clear_tool();
      lv_draw_move_motor();
      break;
    case ID_T_HOME:
      lv_clear_tool();
      lv_draw_home();
      break;
    case ID_T_LEVELING:
      #if ENABLED(AUTO_BED_LEVELING_BILINEAR)
        //queue.enqueue_one_P(PSTR("G28"));
        //queue.enqueue_one_P(PSTR("G29"));
        get_gcode_command(AUTO_LEVELING_COMMAND_ADDR,(uint8_t *)public_buf_m);
        public_buf_m[sizeof(public_buf_m)-1] = 0;
        queue.inject_P(PSTR(public_buf_m));
      #else
        uiCfg.leveling_first_time = 1;
        lv_clear_tool();
        lv_draw_manualLevel();
      #endif
      break;
    case ID_T_FILAMENT:
      uiCfg.desireSprayerTempBak = thermalManager.temp_hotend[uiCfg.curSprayerChoose].target;
      lv_clear_tool();
      lv_draw_filament_change();
      break;
    case ID_T_MORE: break;
    case ID_T_RETURN:
      TERN_(MKS_TEST, curent_disp_ui = 1);
      lv_clear_tool();
      lv_draw_ready_print();
      break;
  }
}

void lv_draw_tool(void) {
  scr = lv_screen_create(TOOL_UI);
  lv_big_button_create(scr, "F:/bmp_preHeat.bin", tool_menu.preheat, INTERVAL_V, titleHeight, event_handler, ID_T_PRE_HEAT);
  lv_big_button_create(scr, "F:/bmp_extruct.bin", tool_menu.extrude, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_T_EXTRUCT);
  lv_big_button_create(scr, "F:/bmp_mov.bin", tool_menu.move, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_T_MOV);
  lv_big_button_create(scr, "F:/bmp_zero.bin", tool_menu.home, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_T_HOME);
  lv_big_button_create(scr, "F:/bmp_leveling.bin", tool_menu.TERN(AUTO_BED_LEVELING_BILINEAR, autoleveling, leveling), INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_T_LEVELING);
  lv_big_button_create(scr, "F:/bmp_filamentchange.bin", tool_menu.filament, BTN_X_PIXEL+INTERVAL_V*2,BTN_Y_PIXEL+INTERVAL_H+titleHeight, event_handler,ID_T_FILAMENT);
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_T_RETURN);
}

void lv_clear_tool() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
