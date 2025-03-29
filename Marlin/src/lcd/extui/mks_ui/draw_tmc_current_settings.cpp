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

#if ALL(HAS_TFT_LVGL_UI, HAS_TRINAMIC_CONFIG)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../module/stepper/indirection.h"
#include "../../../feature/tmc_util.h"
#include "../../../inc/MarlinConfig.h"

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_TMC_CURRENT_RETURN = 1,
  ID_TMC_CURRENT_X,
  ID_TMC_CURRENT_Y,
  ID_TMC_CURRENT_Z,
  ID_TMC_CURRENT_E0,
  ID_TMC_CURRENT_E1,
  ID_TMC_CURRENT_DOWN,
  ID_TMC_CURRENT_UP
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  lv_clear_tmc_current_settings();
  switch (obj->mks_obj_id) {
    case ID_TMC_CURRENT_RETURN: uiCfg.para_ui_page = false; draw_return_ui(); return;

    case ID_TMC_CURRENT_X: TERN_(X_IS_TRINAMIC, value = Xcurrent); break;
    case ID_TMC_CURRENT_Y: TERN_(Y_IS_TRINAMIC, value = Ycurrent); break;
    case ID_TMC_CURRENT_Z: TERN_(Z_IS_TRINAMIC, value = Zcurrent); break;
    case ID_TMC_CURRENT_E0: TERN_(E0_IS_TRINAMIC, value = E0current); break;
    case ID_TMC_CURRENT_E1: TERN_(E1_IS_TRINAMIC, value = E1current); break;

    case ID_TMC_CURRENT_UP:   uiCfg.para_ui_page = false; lv_draw_tmc_current_settings(); return;
    case ID_TMC_CURRENT_DOWN: uiCfg.para_ui_page = true;  lv_draw_tmc_current_settings(); return;
  }
  lv_draw_number_key();

}

void lv_draw_tmc_current_settings() {
  scr = lv_screen_create(TMC_CURRENT_UI, machine_menu.TmcCurrentConfTitle);

  float milliamps;
  if (!uiCfg.para_ui_page) {
    milliamps = TERN(X_IS_TRINAMIC, stepperX.getMilliamps(), -1);
    dtostrf(milliamps, 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.X_Current, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_TMC_CURRENT_X, 0, public_buf_l);

    milliamps = TERN(Y_IS_TRINAMIC, stepperY.getMilliamps(), -1);
    dtostrf(milliamps, 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.Y_Current, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_TMC_CURRENT_Y, 1, public_buf_l);

    milliamps = TERN(Z_IS_TRINAMIC, stepperZ.getMilliamps(), -1);
    dtostrf(milliamps, 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.Z_Current, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_TMC_CURRENT_Z, 2, public_buf_l);

    milliamps = TERN(E0_IS_TRINAMIC, stepperE0.getMilliamps(), -1);
    dtostrf(milliamps, 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.E0_Current, PARA_UI_POS_X, PARA_UI_POS_Y * 4, event_handler, ID_TMC_CURRENT_E0, 3, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.next, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_TMC_CURRENT_DOWN, true);
  }
  else {
    milliamps = TERN(E1_IS_TRINAMIC, stepperE1.getMilliamps(), -1);
    dtostrf(milliamps, 1, 1, public_buf_l);
    lv_screen_menu_item_1_edit(scr, machine_menu.E1_Current, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_TMC_CURRENT_E1, 0, public_buf_l);

    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.previous, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_TMC_CURRENT_UP, true);
  }

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACK_POS_X, PARA_UI_BACK_POS_Y, event_handler, ID_TMC_CURRENT_RETURN, true);
}

void lv_clear_tmc_current_settings() {
  if (TERN0(HAS_ROTARY_ENCODER, gCfgItems.encoder_enable))
    lv_group_remove_all_objs(g);
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI && HAS_TRINAMIC_CONFIG
