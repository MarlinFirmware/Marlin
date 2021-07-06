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

#if BOTH(HAS_TFT_LVGL_UI, HAS_STEALTHCHOP)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../module/stepper/indirection.h"
#include "../../../feature/tmc_util.h"
#include "../../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../module/settings.h"
#endif

extern lv_group_t *g;
static lv_obj_t *scr;

enum {
  ID_TMC_MODE_RETURN = 1,
  ID_TMC_MODE_X,
  ID_TMC_MODE_Y,
  ID_TMC_MODE_Z,
  ID_TMC_MODE_E0,
  ID_TMC_MODE_E1,
  ID_TMC_MODE_DOWN,
  ID_TMC_MODE_UP
};

static lv_obj_t *buttonXState = nullptr, *buttonYState = nullptr, *buttonZState = nullptr, *buttonE0State = nullptr;

static lv_obj_t *buttonE1State = nullptr;

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;

  auto toggle_chop = [&](auto &stepper, auto &button) {
    const bool isena = stepper.toggle_stepping_mode();
    lv_screen_menu_item_onoff_update(button, isena);
    TERN_(EEPROM_SETTINGS, (void)settings.save());
  };

  switch (obj->mks_obj_id) {
    case ID_TMC_MODE_RETURN:
      uiCfg.para_ui_page = false;
      lv_clear_tmc_step_mode_settings();
      draw_return_ui();
      break;

    #if AXIS_HAS_STEALTHCHOP(X)
      case ID_TMC_MODE_X:
        toggle_chop(stepperX, buttonXState);
        break;
    #endif
    #if AXIS_HAS_STEALTHCHOP(Y)
      case ID_TMC_MODE_Y:
        toggle_chop(stepperY, buttonYState);
        break;
    #endif
    #if AXIS_HAS_STEALTHCHOP(Z)
      case ID_TMC_MODE_Z:
        toggle_chop(stepperZ, buttonZState);
        break;
    #endif
    #if AXIS_HAS_STEALTHCHOP(E0)
      case ID_TMC_MODE_E0:
        toggle_chop(stepperE0, buttonE0State);
        break;
    #endif
    #if AXIS_HAS_STEALTHCHOP(E1)
      case ID_TMC_MODE_E1:
        toggle_chop(stepperE1, buttonE1State);
        break;
    #endif

    case ID_TMC_MODE_UP:
      uiCfg.para_ui_page = false;
      lv_clear_tmc_step_mode_settings();
      lv_draw_tmc_step_mode_settings();
      break;
    case ID_TMC_MODE_DOWN:
      uiCfg.para_ui_page = true;
      lv_clear_tmc_step_mode_settings();
      lv_draw_tmc_step_mode_settings();
      break;
  }
}

void lv_draw_tmc_step_mode_settings() {
  buttonXState = buttonYState = buttonZState = buttonE0State = buttonE1State = nullptr;

  scr = lv_screen_create(TMC_MODE_UI, machine_menu.TmcStepModeConfTitle);

  bool stealth_X = false, stealth_Y = false, stealth_Z = false, stealth_E0 = false, stealth_E1 = false;
  #if AXIS_HAS_STEALTHCHOP(X)
    stealth_X = stepperX.get_stealthChop();
  #endif
  #if AXIS_HAS_STEALTHCHOP(Y)
    stealth_Y = stepperY.get_stealthChop();
  #endif
  #if AXIS_HAS_STEALTHCHOP(Z)
    stealth_Z = stepperZ.get_stealthChop();
  #endif
  #if AXIS_HAS_STEALTHCHOP(E0)
    stealth_E0 = stepperE0.get_stealthChop();
  #endif
  #if AXIS_HAS_STEALTHCHOP(E1)
    stealth_E1 = stepperE1.get_stealthChop();
  #endif

  if (!uiCfg.para_ui_page) {
    buttonXState = lv_screen_menu_item_onoff(scr, machine_menu.X_StepMode, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_TMC_MODE_X, 0, stealth_X);
    buttonYState = lv_screen_menu_item_onoff(scr, machine_menu.Y_StepMode, PARA_UI_POS_X, PARA_UI_POS_Y * 2, event_handler, ID_TMC_MODE_Y, 1, stealth_Y);
    buttonZState = lv_screen_menu_item_onoff(scr, machine_menu.Z_StepMode, PARA_UI_POS_X, PARA_UI_POS_Y * 3, event_handler, ID_TMC_MODE_Z, 2, stealth_Z);
    buttonE0State = lv_screen_menu_item_onoff(scr, machine_menu.E0_StepMode, PARA_UI_POS_X, PARA_UI_POS_Y * 4, event_handler, ID_TMC_MODE_E0, 2, stealth_E0);
    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.next, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_TMC_MODE_DOWN, true);
  }
  else {
    buttonE1State = lv_screen_menu_item_onoff(scr, machine_menu.E1_StepMode, PARA_UI_POS_X, PARA_UI_POS_Y, event_handler, ID_TMC_MODE_E1, 0, stealth_E1);
    lv_big_button_create(scr, "F:/bmp_back70x40.bin", machine_menu.previous, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_TMC_MODE_UP, true);
  }

  lv_big_button_create(scr, "F:/bmp_back70x40.bin", common_menu.text_back, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, event_handler, ID_TMC_MODE_RETURN, true);
}

void lv_clear_tmc_step_mode_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI && HAS_STEALTHCHOP
