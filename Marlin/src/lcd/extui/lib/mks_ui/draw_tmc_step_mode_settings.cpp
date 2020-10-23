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

#if BOTH(HAS_TFT_LVGL_UI, HAS_STEALTHCHOP)

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../module/stepper/indirection.h"
#include "../../../../feature/tmc_util.h"
#include "../../../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../../module/settings.h"
#endif

extern lv_group_t * g;
static lv_obj_t * scr;

#define ID_TMC_MODE_RETURN 1
#define ID_TMC_MODE_X      2
#define ID_TMC_MODE_Y      3
#define ID_TMC_MODE_Z      4
#define ID_TMC_MODE_E0     5
#define ID_TMC_MODE_E1     6
#define ID_TMC_MODE_DOWN   7
#define ID_TMC_MODE_UP     8

static lv_obj_t *labelXState = NULL, *labelYState = NULL, *labelZState = NULL, *labelE0State = NULL;
static lv_obj_t *buttonXState = NULL, *buttonYState = NULL, *buttonZState = NULL, *buttonE0State = NULL;

//#if AXIS_HAS_STEALTHCHOP(E1)
  static lv_obj_t *labelE1State = NULL, *buttonE1State = NULL;
//#endif

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_TMC_MODE_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_tmc_step_mode_settings();
        draw_return_ui();
      }
      break;

    #if AXIS_HAS_STEALTHCHOP(X)
      case ID_TMC_MODE_X:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          if (stepperX.toggle_stepping_mode()) {
            lv_imgbtn_set_src_both(buttonXState, "F:/bmp_enable.bin");
            lv_label_set_text(labelXState, machine_menu.enable);
          }
          else {
            lv_imgbtn_set_src_both(buttonXState, "F:/bmp_disable.bin");
            lv_label_set_text(labelXState, machine_menu.disable);
            //lv_obj_align(labelXState, buttonE1State, LV_ALIGN_IN_LEFT_MID,0, 0);
          }
          TERN_(EEPROM_SETTINGS, (void)settings.save());
        }
        break;
    #endif // if AXIS_HAS_STEALTHCHOP(X)

    #if AXIS_HAS_STEALTHCHOP(Y)
      case ID_TMC_MODE_Y:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          if (stepperY.toggle_stepping_mode()) {
            lv_imgbtn_set_src_both(buttonYState, "F:/bmp_enable.bin");
            lv_label_set_text(labelYState, machine_menu.enable);
          }
          else {
            lv_imgbtn_set_src_both(buttonYState, "F:/bmp_disable.bin");
            lv_label_set_text(labelYState, machine_menu.disable);
            //lv_obj_align(labelXState, buttonE1State, LV_ALIGN_IN_LEFT_MID,0, 0);
          }
          TERN_(EEPROM_SETTINGS, (void)settings.save());
        }
        break;
    #endif // if AXIS_HAS_STEALTHCHOP(Y)

    #if AXIS_HAS_STEALTHCHOP(Z)
      case ID_TMC_MODE_Z:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          if (stepperZ.toggle_stepping_mode()) {
            lv_imgbtn_set_src_both(buttonZState, "F:/bmp_enable.bin");
            lv_label_set_text(labelZState, machine_menu.enable);
          }
          else {
            lv_imgbtn_set_src_both(buttonZState, "F:/bmp_disable.bin");
            lv_label_set_text(labelZState, machine_menu.disable);
            //lv_obj_align(labelXState, buttonE1State, LV_ALIGN_IN_LEFT_MID,0, 0);
          }
          TERN_(EEPROM_SETTINGS, (void)settings.save());
        }
        break;
    #endif // if AXIS_HAS_STEALTHCHOP(Z)

    #if AXIS_HAS_STEALTHCHOP(E0)
      case ID_TMC_MODE_E0:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          if (stepperE0.toggle_stepping_mode()) {
            lv_imgbtn_set_src_both(buttonE0State, "F:/bmp_enable.bin");
            lv_label_set_text(labelE0State, machine_menu.enable);
          }
          else {
            lv_imgbtn_set_src_both(buttonE0State, "F:/bmp_disable.bin");
            lv_label_set_text(labelE0State, machine_menu.disable);
            //lv_obj_align(labelXState, buttonE1State, LV_ALIGN_IN_LEFT_MID,0, 0);
          }
          TERN_(EEPROM_SETTINGS, (void)settings.save());
        }
        break;
    #endif // if AXIS_HAS_STEALTHCHOP(E0)

    #if AXIS_HAS_STEALTHCHOP(E1)
      case ID_TMC_MODE_E1:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          if (stepperE1.toggle_stepping_mode()) {
            lv_imgbtn_set_src_both(buttonE1State, "F:/bmp_enable.bin");
            lv_label_set_text(labelE1State, machine_menu.enable);
          }
          else {
            lv_imgbtn_set_src_both(buttonE1State, "F:/bmp_disable.bin");
            lv_label_set_text(labelE1State, machine_menu.disable);
            //lv_obj_align(labelXState, buttonE1State, LV_ALIGN_IN_LEFT_MID,0, 0);
          }
          TERN_(EEPROM_SETTINGS, (void)settings.save());
        }
        break;
    #endif // if AXIS_HAS_STEALTHCHOP(E1)
    case ID_TMC_MODE_UP:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_tmc_step_mode_settings();
        lv_draw_tmc_step_mode_settings();
      }
      break;
    case ID_TMC_MODE_DOWN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 1;
        lv_clear_tmc_step_mode_settings();
        lv_draw_tmc_step_mode_settings();
      }
      break;
  }
}

void lv_draw_tmc_step_mode_settings(void) {
  lv_obj_t *buttonBack = NULL, *label_Back = NULL;
  lv_obj_t *buttonXText = NULL, *labelXText = NULL;
  lv_obj_t *buttonYText = NULL, *labelYText = NULL;
  lv_obj_t *buttonZText = NULL, *labelZText = NULL;
  lv_obj_t *buttonE0Text = NULL, *labelE0Text = NULL;

  lv_obj_t * line1 = NULL, * line2 = NULL, * line3 = NULL, * line4 = NULL;
  //#if AXIS_HAS_STEALTHCHOP(E1)
    lv_obj_t *buttonTurnPage = NULL, *labelTurnPage = NULL;
    lv_obj_t *buttonE1Text = NULL, *labelE1Text = NULL;
  //#endif

  labelXState   = NULL;
  buttonXState  = NULL;
  labelYState   = NULL;
  buttonYState  = NULL;
  labelZState   = NULL;
  buttonZState  = NULL;
  labelE0State  = NULL;
  buttonE0State = NULL;
  //#if AXIS_HAS_STEALTHCHOP(E1)
    labelE1State  = NULL;
    buttonE1State = NULL;
  //#endif

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != TMC_MODE_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = TMC_MODE_UI;
  }
  disp_state = TMC_MODE_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, machine_menu.TmcStepModeConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

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

  if (uiCfg.para_ui_page != 1) {
    buttonXText = lv_btn_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y, event_handler, 0);
    labelXText = lv_label_create_empty(buttonXText);                        /*Add a label to the button*/

    buttonXState = lv_imgbtn_create(scr, stealth_X ? "F:/bmp_enable.bin" : "F:/bmp_disable.bin", PARA_UI_STATE_POS_X, PARA_UI_POS_Y + PARA_UI_STATE_V, event_handler, ID_TMC_MODE_X);
    labelXState = lv_label_create_empty(buttonXState);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonXState);
    #endif

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    buttonYText = lv_btn_create(scr, NULL, PARA_UI_POS_X, PARA_UI_POS_Y * 2, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y, event_handler, 0);
    labelYText = lv_label_create_empty(buttonYText);                        /*Add a label to the button*/

    buttonYState = lv_imgbtn_create(scr, stealth_Y ? "F:/bmp_enable.bin" : "F:/bmp_disable.bin", PARA_UI_STATE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_STATE_V, event_handler, ID_TMC_MODE_Y);
    labelYState = lv_label_create_empty(buttonYState);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonYState);
    #endif

    line2 = lv_line_create(scr, NULL);
    lv_ex_line(line2, line_points[1]);

    buttonZText = lv_btn_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 3, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y, event_handler, 0);
    labelZText = lv_label_create_empty(buttonZText);                        /*Add a label to the button*/

    buttonZState = lv_imgbtn_create(scr, stealth_Z ? "F:/bmp_enable.bin" : "F:/bmp_disable.bin", PARA_UI_STATE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_STATE_V, event_handler, ID_TMC_MODE_Z);
    labelZState = lv_label_create_empty(buttonZState);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonZState);
    #endif

    line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3, line_points[2]);

    buttonE0Text = lv_btn_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 4, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y, event_handler, 0);
    labelE0Text = lv_label_create_empty(buttonE0Text);                      /*Add a label to the button*/

    buttonE0State = lv_imgbtn_create(scr, stealth_E0 ? "F:/bmp_enable.bin" : "F:/bmp_disable.bin", PARA_UI_STATE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_STATE_V, event_handler, ID_TMC_MODE_E0);
    labelE0State = lv_label_create_empty(buttonE0State);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonE0State);
    #endif

    line4 = lv_line_create(scr, NULL);
    lv_ex_line(line4, line_points[3]);

    //#if AXIS_HAS_STEALTHCHOP(E1)
      buttonTurnPage = lv_imgbtn_create(scr, "F:/bmp_back70x40.bin", event_handler, ID_TMC_MODE_DOWN);
      #if HAS_ROTARY_ENCODER
        if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonTurnPage);
      #endif
    //#endif
  }
  else {
    //#if AXIS_HAS_STEALTHCHOP(E1)
      buttonE1Text = lv_btn_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y, event_handler, 0);
      labelE1Text = lv_label_create_empty(buttonE1Text);                      /*Add a label to the button*/

      buttonE1State = lv_imgbtn_create(scr, stealth_E1 ? "F:/bmp_enable.bin" : "F:/bmp_disable.bin", PARA_UI_STATE_POS_X, PARA_UI_POS_Y + PARA_UI_STATE_V, event_handler, ID_TMC_MODE_E1);
      labelE1State = lv_label_create_empty(buttonE1State);
      #if HAS_ROTARY_ENCODER
        if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonE1State);
      #endif

      line1 = lv_line_create(scr, NULL);
      lv_ex_line(line1, line_points[0]);

      buttonTurnPage = lv_imgbtn_create(scr, "F:/bmp_back70x40.bin", event_handler, ID_TMC_MODE_UP);
    //#endif
  }
  //#if AXIS_HAS_STEALTHCHOP(E1)
    lv_obj_set_pos(buttonTurnPage, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y);
    lv_btn_set_layout(buttonTurnPage, LV_LAYOUT_OFF);
    labelTurnPage = lv_label_create_empty(buttonTurnPage);
  //#endif

  buttonBack = lv_imgbtn_create(scr, "F:/bmp_back70x40.bin", PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, event_handler, ID_TMC_MODE_RETURN);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonBack);
  #endif
  label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
    if (uiCfg.para_ui_page != 1) {
      lv_label_set_text(labelXText, machine_menu.X_StepMode);
      lv_obj_align(labelXText, buttonXText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelYText, machine_menu.Y_StepMode);
      lv_obj_align(labelYText, buttonYText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelZText, machine_menu.Z_StepMode);
      lv_obj_align(labelZText, buttonZText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelE0Text, machine_menu.E0_StepMode);
      lv_obj_align(labelE0Text, buttonE0Text, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelXState, stealth_X ? machine_menu.enable : machine_menu.disable);
      lv_obj_align(labelXState, buttonXState, LV_ALIGN_CENTER, 0, 0);

      lv_label_set_text(labelYState, stealth_Y ? machine_menu.enable : machine_menu.disable);
      lv_obj_align(labelYState, buttonYState, LV_ALIGN_CENTER, 0, 0);

      lv_label_set_text(labelZState, stealth_Z ? machine_menu.enable : machine_menu.disable);
      lv_obj_align(labelZState, buttonZState, LV_ALIGN_CENTER, 0, 0);

      lv_label_set_text(labelE0State, stealth_E0 ? machine_menu.enable : machine_menu.disable);
      lv_obj_align(labelE0State, buttonE0State, LV_ALIGN_CENTER, 0, 0);

      //#if AXIS_HAS_STEALTHCHOP(E1)
        lv_label_set_text(labelTurnPage, machine_menu.next);
        lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);
      //#endif
    }
    else {
      //#if AXIS_HAS_STEALTHCHOP(E1)
        lv_label_set_text(labelE1Text, machine_menu.E1_StepMode);
        lv_obj_align(labelE1Text, buttonE1Text, LV_ALIGN_IN_LEFT_MID, 0, 0);
        lv_label_set_text(labelE1State, stealth_E1 ? machine_menu.enable : machine_menu.disable);
        lv_obj_align(labelE1State, buttonE1State, LV_ALIGN_CENTER, 0, 0);

        lv_label_set_text(labelTurnPage, machine_menu.previous);
        lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);
      //#endif
    }

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);
  }
}

void lv_clear_tmc_step_mode_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI && HAS_STEALTHCHOP
