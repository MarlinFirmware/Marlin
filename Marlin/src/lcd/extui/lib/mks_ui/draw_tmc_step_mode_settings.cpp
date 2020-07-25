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

#include "lv_conf.h"
#include "draw_ui.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/planner.h"
#include "../../../../module/stepper/indirection.h"
#include "../../../../feature/tmc_util.h"
#include "../../../../gcode/gcode.h"
#include "../../../../module/planner.h"

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

#if AXIS_HAS_STEALTHCHOP(E1)
  static lv_obj_t *labelE1State = NULL, *buttonE1State = NULL;
#endif

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
    case ID_TMC_MODE_X:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        if (stepperX.stored.stealthChop_enabled == true) {
          stepperX.stored.stealthChop_enabled = false;
          stepperX.refresh_stepping_mode();
          lv_obj_set_event_cb_mks(buttonXState, event_handler, ID_TMC_MODE_X, "bmp_disable.bin", 0);
          lv_label_set_text(labelXState, machine_menu.disable);
          //lv_obj_align(labelXState, buttonE1State, LV_ALIGN_IN_LEFT_MID,0, 0);
          // gcode.process_subcommands_now_P(PSTR("M500"));
        }
        else {
          stepperX.stored.stealthChop_enabled = true;
          stepperX.refresh_stepping_mode();
          lv_obj_set_event_cb_mks(buttonXState, event_handler, ID_TMC_MODE_X, "bmp_enable.bin", 0);
          lv_label_set_text(labelXState, machine_menu.enable);
          // gcode.process_subcommands_now_P(PSTR("M500"));
        }
        gcode.process_subcommands_now_P(PSTR("M500"));
      }
      break;
    case ID_TMC_MODE_Y:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        if (stepperY.stored.stealthChop_enabled == true) {
          stepperY.stored.stealthChop_enabled = false;
          stepperY.refresh_stepping_mode();
          lv_obj_set_event_cb_mks(buttonYState, event_handler, ID_TMC_MODE_Y, "bmp_disable.bin", 0);
          lv_label_set_text(labelYState, machine_menu.disable);
          //lv_obj_align(labelXState, buttonE1State, LV_ALIGN_IN_LEFT_MID,0, 0);
        }
        else {
          stepperY.stored.stealthChop_enabled = true;
          stepperY.refresh_stepping_mode();
          lv_obj_set_event_cb_mks(buttonYState, event_handler, ID_TMC_MODE_Y, "bmp_enable.bin", 0);
          lv_label_set_text(labelYState, machine_menu.enable);
        }
        gcode.process_subcommands_now_P(PSTR("M500"));
      }
      break;
    case ID_TMC_MODE_Z:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        if (stepperZ.stored.stealthChop_enabled == true) {
          stepperZ.stored.stealthChop_enabled = false;
          stepperZ.refresh_stepping_mode();
          lv_obj_set_event_cb_mks(buttonZState, event_handler, ID_TMC_MODE_Z, "bmp_disable.bin", 0);
          lv_label_set_text(labelZState, machine_menu.disable);
          //lv_obj_align(labelXState, buttonE1State, LV_ALIGN_IN_LEFT_MID,0, 0);
        }
        else {
          stepperZ.stored.stealthChop_enabled = true;
          stepperZ.refresh_stepping_mode();
          lv_obj_set_event_cb_mks(buttonZState, event_handler, ID_TMC_MODE_Z, "bmp_enable.bin", 0);
          lv_label_set_text(labelZState, machine_menu.enable);
        }
        gcode.process_subcommands_now_P(PSTR("M500"));
      }
      break;
    case ID_TMC_MODE_E0:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        if (stepperE0.stored.stealthChop_enabled == true) {
          stepperE0.stored.stealthChop_enabled = false;
          stepperE0.refresh_stepping_mode();
          lv_obj_set_event_cb_mks(buttonE0State, event_handler, ID_TMC_MODE_E0, "bmp_disable.bin", 0);
          lv_label_set_text(labelE0State, machine_menu.disable);
          //lv_obj_align(labelXState, buttonE1State, LV_ALIGN_IN_LEFT_MID,0, 0);
        }
        else {
          stepperE0.stored.stealthChop_enabled = true;
          stepperE0.refresh_stepping_mode();
          lv_obj_set_event_cb_mks(buttonE0State, event_handler, ID_TMC_MODE_E0, "bmp_enable.bin", 0);
          lv_label_set_text(labelE0State, machine_menu.enable);
        }
        gcode.process_subcommands_now_P(PSTR("M500"));
      }
      break;
      #if AXIS_HAS_STEALTHCHOP(E1)
        case ID_TMC_MODE_E1:
          if (event == LV_EVENT_CLICKED) {

          }
          else if (event == LV_EVENT_RELEASED) {
            if (stepperE1.stored.stealthChop_enabled == true) {
              stepperE1.stored.stealthChop_enabled = false;
              stepperE1.refresh_stepping_mode();
              lv_obj_set_event_cb_mks(buttonE1State, event_handler, ID_TMC_MODE_E1, "bmp_disable.bin", 0);
              lv_label_set_text(labelE1State, machine_menu.disable);
              //lv_obj_align(labelXState, buttonE1State, LV_ALIGN_IN_LEFT_MID,0, 0);
            }
            else {
              stepperE1.stored.stealthChop_enabled = true;
              stepperE1.refresh_stepping_mode();
              lv_obj_set_event_cb_mks(buttonE1State, event_handler, ID_TMC_MODE_E1, "bmp_enable.bin", 0);
              lv_label_set_text(labelE1State, machine_menu.enable);
            }
            gcode.process_subcommands_now_P(PSTR("M500"));
          }
          break;

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
      #endif // if AXIS_HAS_STEALTHCHOP(E1)
  }
}

void lv_draw_tmc_step_mode_settings(void) {
  lv_obj_t *buttonBack = NULL, *label_Back = NULL;
  lv_obj_t *buttonXText = NULL, *labelXText = NULL;
  lv_obj_t *buttonYText = NULL, *labelYText = NULL;
  lv_obj_t *buttonZText = NULL, *labelZText = NULL;
  lv_obj_t *buttonE0Text = NULL, *labelE0Text = NULL;

  lv_obj_t * line1 = NULL, * line2 = NULL, * line3 = NULL, * line4 = NULL;
  #if AXIS_HAS_STEALTHCHOP(E1)
    lv_obj_t *buttonTurnPage = NULL, *labelTurnPage = NULL;
    lv_obj_t *buttonE1Text = NULL, *labelE1Text = NULL;
  #endif


  labelXState   = NULL;
  buttonXState  = NULL;
  labelYState   = NULL;
  buttonYState  = NULL;
  labelZState   = NULL;
  buttonZState  = NULL;
  labelE0State  = NULL;
  buttonE0State = NULL;
  #if AXIS_HAS_STEALTHCHOP(E1)
    labelE1State  = NULL;
    buttonE1State = NULL;
  #endif

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != TMC_MODE_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = TMC_MODE_UI;
  }
  disp_state = TMC_MODE_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, machine_menu.TmcStepModeConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  LV_IMG_DECLARE(bmp_para_back);
  LV_IMG_DECLARE(bmp_para_state);
  // LV_IMG_DECLARE(bmp_para_bank);

  if (uiCfg.para_ui_page != 1) {
    buttonXText = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
    lv_obj_set_pos(buttonXText, PARA_UI_POS_X, PARA_UI_POS_Y);              /*Set its position*/
    lv_obj_set_size(buttonXText, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);     /*Set its size*/
    lv_obj_set_event_cb(buttonXText, event_handler);
    lv_btn_set_style(buttonXText, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
    lv_btn_set_style(buttonXText, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
    lv_btn_set_layout(buttonXText, LV_LAYOUT_OFF);
    labelXText = lv_label_create(buttonXText, NULL);                        /*Add a label to the button*/

    buttonXState = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonXState, PARA_UI_STATE_POS_X, PARA_UI_POS_Y + PARA_UI_STATE_V);
    if (stepperX.get_stealthChop_status())
      lv_obj_set_event_cb_mks(buttonXState, event_handler, ID_TMC_MODE_X, "bmp_enable.bin", 0);
    else
      lv_obj_set_event_cb_mks(buttonXState, event_handler, ID_TMC_MODE_X, "bmp_disable.bin", 0);
    lv_imgbtn_set_src(buttonXState, LV_BTN_STATE_REL, &bmp_para_state);
    lv_imgbtn_set_src(buttonXState, LV_BTN_STATE_PR, &bmp_para_state);
    lv_imgbtn_set_style(buttonXState, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonXState, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_btn_set_layout(buttonXState, LV_LAYOUT_OFF);
    labelXState = lv_label_create(buttonXState, NULL);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    buttonYText = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
    lv_obj_set_pos(buttonYText, PARA_UI_POS_X, PARA_UI_POS_Y * 2);          /*Set its position*/
    lv_obj_set_size(buttonYText, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);     /*Set its size*/
    lv_obj_set_event_cb(buttonYText, event_handler);
    lv_btn_set_style(buttonYText, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
    lv_btn_set_style(buttonYText, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
    lv_btn_set_layout(buttonYText, LV_LAYOUT_OFF);
    labelYText = lv_label_create(buttonYText, NULL);                        /*Add a label to the button*/

    buttonYState = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonYState, PARA_UI_STATE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_STATE_V);
    if (stepperY.get_stealthChop_status())
      lv_obj_set_event_cb_mks(buttonYState, event_handler, ID_TMC_MODE_Y, "bmp_enable.bin", 0);
    else
      lv_obj_set_event_cb_mks(buttonYState, event_handler, ID_TMC_MODE_Y, "bmp_disable.bin", 0);
    lv_imgbtn_set_src(buttonYState, LV_BTN_STATE_REL, &bmp_para_state);
    lv_imgbtn_set_src(buttonYState, LV_BTN_STATE_PR, &bmp_para_state);
    lv_imgbtn_set_style(buttonYState, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonYState, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_btn_set_layout(buttonYState, LV_LAYOUT_OFF);
    labelYState = lv_label_create(buttonYState, NULL);

    line2 = lv_line_create(scr, NULL);
    lv_ex_line(line2, line_points[1]);

    buttonZText = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
    lv_obj_set_pos(buttonZText, PARA_UI_POS_X, PARA_UI_POS_Y * 3);          /*Set its position*/
    lv_obj_set_size(buttonZText, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);     /*Set its size*/
    lv_obj_set_event_cb(buttonZText, event_handler);
    lv_btn_set_style(buttonZText, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
    lv_btn_set_style(buttonZText, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
    lv_btn_set_layout(buttonZText, LV_LAYOUT_OFF);
    labelZText = lv_label_create(buttonZText, NULL);                        /*Add a label to the button*/

    buttonZState = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonZState, PARA_UI_STATE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_STATE_V);
    if (stepperZ.get_stealthChop_status())
      lv_obj_set_event_cb_mks(buttonZState, event_handler, ID_TMC_MODE_Z, "bmp_enable.bin", 0);
    else
      lv_obj_set_event_cb_mks(buttonZState, event_handler, ID_TMC_MODE_Z, "bmp_disable.bin", 0);
    lv_imgbtn_set_src(buttonZState, LV_BTN_STATE_REL, &bmp_para_state);
    lv_imgbtn_set_src(buttonZState, LV_BTN_STATE_PR, &bmp_para_state);
    lv_imgbtn_set_style(buttonZState, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonZState, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_btn_set_layout(buttonZState, LV_LAYOUT_OFF);
    labelZState = lv_label_create(buttonZState, NULL);

    line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3, line_points[2]);

    buttonE0Text = lv_btn_create(scr, NULL);                                /*Add a button the current screen*/
    lv_obj_set_pos(buttonE0Text, PARA_UI_POS_X, PARA_UI_POS_Y * 4);         /*Set its position*/
    lv_obj_set_size(buttonE0Text, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);    /*Set its size*/
    lv_obj_set_event_cb(buttonE0Text, event_handler);
    lv_btn_set_style(buttonE0Text, LV_BTN_STYLE_REL, &tft_style_label_rel); /*Set the button's released style*/
    lv_btn_set_style(buttonE0Text, LV_BTN_STYLE_PR, &tft_style_label_pre);  /*Set the button's pressed style*/
    lv_btn_set_layout(buttonE0Text, LV_LAYOUT_OFF);
    labelE0Text = lv_label_create(buttonE0Text, NULL);                      /*Add a label to the button*/

    buttonE0State = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonE0State, PARA_UI_STATE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_STATE_V);
    if (stepperE0.get_stealthChop_status())
      lv_obj_set_event_cb_mks(buttonE0State, event_handler, ID_TMC_MODE_E0, "bmp_enable.bin", 0);
    else
      lv_obj_set_event_cb_mks(buttonE0State, event_handler, ID_TMC_MODE_E0, "bmp_disable.bin", 0);
    lv_imgbtn_set_src(buttonE0State, LV_BTN_STATE_REL, &bmp_para_state);
    lv_imgbtn_set_src(buttonE0State, LV_BTN_STATE_PR, &bmp_para_state);
    lv_imgbtn_set_style(buttonE0State, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonE0State, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_btn_set_layout(buttonE0State, LV_LAYOUT_OFF);
    labelE0State = lv_label_create(buttonE0State, NULL);

    line4 = lv_line_create(scr, NULL);
    lv_ex_line(line4, line_points[3]);

    #if AXIS_HAS_STEALTHCHOP(E1)
      buttonTurnPage = lv_imgbtn_create(scr, NULL);
      lv_obj_set_event_cb_mks(buttonTurnPage, event_handler, ID_TMC_MODE_DOWN, "bmp_back70x40.bin", 0);
      lv_imgbtn_set_src(buttonTurnPage, LV_BTN_STATE_REL, &bmp_para_back);
      lv_imgbtn_set_src(buttonTurnPage, LV_BTN_STATE_PR, &bmp_para_back);
      lv_imgbtn_set_style(buttonTurnPage, LV_BTN_STATE_PR, &tft_style_label_pre);
      lv_imgbtn_set_style(buttonTurnPage, LV_BTN_STATE_REL, &tft_style_label_rel);
    #endif
  }
  else {
    #if AXIS_HAS_STEALTHCHOP(E1)
      buttonE1Text = lv_btn_create(scr, NULL);                                /*Add a button the current screen*/
      lv_obj_set_pos(buttonE1Text, PARA_UI_POS_X, PARA_UI_POS_Y);             /*Set its position*/
      lv_obj_set_size(buttonE1Text, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);    /*Set its size*/
      lv_obj_set_event_cb(buttonE1Text, event_handler);
      lv_btn_set_style(buttonE1Text, LV_BTN_STYLE_REL, &tft_style_label_rel); /*Set the button's released style*/
      lv_btn_set_style(buttonE1Text, LV_BTN_STYLE_PR, &tft_style_label_pre);  /*Set the button's pressed style*/
      lv_btn_set_layout(buttonE1Text, LV_LAYOUT_OFF);
      labelE1Text = lv_label_create(buttonE1Text, NULL);                      /*Add a label to the button*/

      buttonE1State = lv_imgbtn_create(scr, NULL);
      lv_obj_set_pos(buttonE1State, PARA_UI_STATE_POS_X, PARA_UI_POS_Y + PARA_UI_STATE_V);
      if (stepperE1.get_stealthChop_status())
        lv_obj_set_event_cb_mks(buttonE1State, event_handler, ID_TMC_MODE_E1, "bmp_enable.bin", 0);
      else
        lv_obj_set_event_cb_mks(buttonE1State, event_handler, ID_TMC_MODE_E1, "bmp_disable.bin", 0);
      lv_imgbtn_set_src(buttonE1State, LV_BTN_STATE_REL, &bmp_para_state);
      lv_imgbtn_set_src(buttonE1State, LV_BTN_STATE_PR, &bmp_para_state);
      lv_imgbtn_set_style(buttonE1State, LV_BTN_STATE_PR, &tft_style_label_pre);
      lv_imgbtn_set_style(buttonE1State, LV_BTN_STATE_REL, &tft_style_label_rel);
      lv_btn_set_layout(buttonE1State, LV_LAYOUT_OFF);
      labelE1State = lv_label_create(buttonE1State, NULL);

      line1 = lv_line_create(scr, NULL);
      lv_ex_line(line1, line_points[0]);

      buttonTurnPage = lv_imgbtn_create(scr, NULL);
      lv_obj_set_event_cb_mks(buttonTurnPage, event_handler, ID_TMC_MODE_UP, "bmp_back70x40.bin", 0);
      lv_imgbtn_set_src(buttonTurnPage, LV_BTN_STATE_REL, &bmp_para_back);
      lv_imgbtn_set_src(buttonTurnPage, LV_BTN_STATE_PR, &bmp_para_back);
      lv_imgbtn_set_style(buttonTurnPage, LV_BTN_STATE_PR, &tft_style_label_pre);
      lv_imgbtn_set_style(buttonTurnPage, LV_BTN_STATE_REL, &tft_style_label_rel);
    #endif
  }
  #if AXIS_HAS_STEALTHCHOP(E1)
    lv_obj_set_pos(buttonTurnPage, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y);
    lv_btn_set_layout(buttonTurnPage, LV_LAYOUT_OFF);
    labelTurnPage = lv_label_create(buttonTurnPage, NULL);
  #endif

  buttonBack = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_TMC_MODE_RETURN, "bmp_back70x40.bin", 0);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, &bmp_para_back);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, &bmp_para_back);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);

  lv_obj_set_pos(buttonBack, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);
  label_Back = lv_label_create(buttonBack, NULL);

  if (gCfgItems.multiple_language != 0) {
    if (uiCfg.para_ui_page != 1) {
      lv_label_set_text(labelXText, machine_menu.X_StepMode);
      lv_obj_align(labelXText, buttonXText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelYText, machine_menu.Y_StepMode);
      lv_obj_align(labelYText, buttonYText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelZText, machine_menu.Z_StepMode);
      lv_obj_align(labelZText, buttonZText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelE0Text, machine_menu.E0_StepMode);
      lv_obj_align(labelE0Text, buttonE0Text, LV_ALIGN_IN_LEFT_MID, 0, 0);

      if (stepperX.get_stealthChop_status())
        lv_label_set_text(labelXState, machine_menu.enable);
      else
        lv_label_set_text(labelXState, machine_menu.disable);
      lv_obj_align(labelXState, buttonXState, LV_ALIGN_CENTER, 0, 0);

      if (stepperY.get_stealthChop_status())
        lv_label_set_text(labelYState, machine_menu.enable);
      else
        lv_label_set_text(labelYState, machine_menu.disable);
      lv_obj_align(labelYState, buttonYState, LV_ALIGN_CENTER, 0, 0);

      if (stepperZ.get_stealthChop_status())
        lv_label_set_text(labelZState, machine_menu.enable);
      else
        lv_label_set_text(labelZState, machine_menu.disable);
      lv_obj_align(labelZState, buttonZState, LV_ALIGN_CENTER, 0, 0);

      if (stepperE0.get_stealthChop_status())
        lv_label_set_text(labelE0State, machine_menu.enable);
      else
        lv_label_set_text(labelE0State, machine_menu.disable);
      lv_obj_align(labelE0State, buttonE0State, LV_ALIGN_CENTER, 0, 0);

      #if AXIS_HAS_STEALTHCHOP(E1)
        lv_label_set_text(labelTurnPage, machine_menu.next);
        lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);
      #endif
    }
    else {
      #if AXIS_HAS_STEALTHCHOP(E1)
        lv_label_set_text(labelE1Text, machine_menu.E1_StepMode);
        lv_obj_align(labelE1Text, buttonE1Text, LV_ALIGN_IN_LEFT_MID, 0, 0);

        if (stepperE1.get_stealthChop_status())
          lv_label_set_text(labelE1State, machine_menu.enable);
        else
          lv_label_set_text(labelE1State, machine_menu.disable);
        lv_obj_align(labelE1State, buttonE1State, LV_ALIGN_CENTER, 0, 0);

        lv_label_set_text(labelTurnPage, machine_menu.previous);
        lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);

      #endif
    }

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);
  }
}

void lv_clear_tmc_step_mode_settings() { lv_obj_del(scr); }

#endif // HAS_TFT_LVGL_UI && HAS_STEALTHCHOP
