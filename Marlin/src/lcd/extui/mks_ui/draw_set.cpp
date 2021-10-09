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

#include "draw_ready_print.h"
#include "draw_set.h"
#include "draw_ui.h"
#include <lv_conf.h>

#include "pic_manager.h"

#include "../../../gcode/queue.h"
#include "../../../inc/MarlinConfig.h"

#if HAS_SUICIDE
  #include "../../../MarlinCore.h"
#endif

static lv_obj_t *scr;
extern lv_group_t*  g;

enum {
  ID_S_WIFI = 1,
  ID_S_FAN,
  ID_S_ABOUT,
  ID_S_CONTINUE,
  ID_S_MOTOR_OFF,
  ID_S_LANGUAGE,
  ID_S_MACHINE_PARA,
  ID_S_EEPROM_SET,
  ID_S_RETURN
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  if (obj->mks_obj_id == ID_S_CONTINUE) return;
  if (obj->mks_obj_id == ID_S_MOTOR_OFF) {
    TERN(HAS_SUICIDE, suicide(), queue.enqueue_now(F("M84")));
    return;
  }
  lv_clear_set();
  switch (obj->mks_obj_id) {
    case ID_S_FAN:
      lv_draw_fan();
      break;
    case ID_S_ABOUT:
      lv_draw_about();
      break;
    case ID_S_LANGUAGE:
      lv_draw_language();
      break;
    case ID_S_MACHINE_PARA:
      lv_draw_machine_para();
      break;
    case ID_S_EEPROM_SET:
      lv_draw_eeprom_settings();
      break;
    case ID_S_RETURN:
      lv_draw_ready_print();
      break;

    #if ENABLED(MKS_WIFI_MODULE)
      case ID_S_WIFI:
        if (gCfgItems.wifi_mode_sel == STA_MODEL) {
          if (wifi_link_state == WIFI_CONNECTED) {
            last_disp_state = SET_UI;
            lv_draw_wifi();
          }
          else {
            if (uiCfg.command_send) {
              uint8_t cmd_wifi_list[] = { 0xA5, 0x07, 0x00, 0x00, 0xFC };
              raw_send_to_wifi(cmd_wifi_list, COUNT(cmd_wifi_list));
              last_disp_state = SET_UI;
              lv_draw_wifi_list();
            }
            else {
              last_disp_state = SET_UI;
              lv_draw_dialog(DIALOG_WIFI_ENABLE_TIPS);
            }
          }
        }
        else {
          last_disp_state = SET_UI;
          lv_draw_wifi();
        }
        break;
    #endif
  }
}

void lv_draw_set() {
  scr = lv_screen_create(SET_UI);
  lv_big_button_create(scr, "F:/bmp_eeprom_settings.bin", set_menu.eepromSet, INTERVAL_V, titleHeight, event_handler, ID_S_EEPROM_SET);
  lv_big_button_create(scr, "F:/bmp_fan.bin", set_menu.fan, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_S_FAN);
  lv_big_button_create(scr, "F:/bmp_about.bin", set_menu.about, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_S_ABOUT);
  lv_big_button_create(scr, ENABLED(HAS_SUICIDE) ? "F:/bmp_manual_off.bin" : "F:/bmp_function1.bin", set_menu.TERN(HAS_SUICIDE, shutdown, motoroff), BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_S_MOTOR_OFF);
  lv_big_button_create(scr, "F:/bmp_machine_para.bin", set_menu.machine_para, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_S_MACHINE_PARA);
  #if HAS_LANG_SELECT_SCREEN
    lv_big_button_create(scr, "F:/bmp_language.bin", set_menu.language, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_S_LANGUAGE);
  #endif
  #if ENABLED(MKS_WIFI_MODULE)
    lv_big_button_create(scr, "F:/bmp_wifi.bin", set_menu.wifi, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_S_WIFI);
  #endif
  lv_big_button_create(scr, "F:/bmp_return.bin", common_menu.text_back, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_S_RETURN);
}

void lv_clear_set() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
