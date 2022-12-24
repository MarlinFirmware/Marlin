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
  ID_MOTOR_RETURN = 1,
  ID_MOTOR_STEPS,
  ID_MOTOR_TMC_CURRENT,
  ID_MOTOR_STEP_MODE,
  ID_HOME_SENSE
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  lv_clear_motor_settings();
  switch (obj->mks_obj_id) {
    case ID_MOTOR_RETURN:
      draw_return_ui();
      break;
    case ID_MOTOR_STEPS:
      lv_draw_step_settings();
      break;
    #if USE_SENSORLESS
      case ID_HOME_SENSE:
        lv_draw_homing_sensitivity_settings();
        break;
    #endif

    #if HAS_TRINAMIC_CONFIG
      case ID_MOTOR_TMC_CURRENT:
        lv_draw_tmc_current_settings();
        break;
      #if HAS_STEALTHCHOP
        case ID_MOTOR_STEP_MODE:
          lv_draw_tmc_step_mode_settings();
          break;
      #endif
    #endif
  }
}

void lv_draw_motor_settings() {
  int index = 0;

  scr = lv_screen_create(MOTOR_SETTINGS_UI, machine_menu.MotorConfTitle);
  lv_screen_menu_item(scr, machine_menu.StepsConf, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_MOTOR_STEPS, index++);
  #if USE_SENSORLESS
    lv_screen_menu_item(scr, machine_menu.HomingSensitivityConf, PARA_UI_POS_X, PARA_UI_POS_Y * (index + 1), event_handler, ID_HOME_SENSE, index);
    index++;
  #endif
  #if HAS_TRINAMIC_CONFIG
    lv_screen_menu_item(scr, machine_menu.TMCcurrentConf, PARA_UI_POS_X, PARA_UI_POS_Y * (index + 1), event_handler, ID_MOTOR_TMC_CURRENT, index);
    index++;
    #if HAS_STEALTHCHOP
      lv_screen_menu_item(scr, machine_menu.TMCStepModeConf, PARA_UI_POS_X, PARA_UI_POS_Y * (index + 1), event_handler, ID_MOTOR_STEP_MODE, index);
      index++;
    #endif
  #endif
  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACK_POS_X + 10, PARA_UI_BACK_POS_Y, event_handler, ID_MOTOR_RETURN, true);
}

void lv_clear_motor_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
