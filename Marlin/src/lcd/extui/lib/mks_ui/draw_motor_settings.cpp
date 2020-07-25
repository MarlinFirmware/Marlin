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

#include "lv_conf.h"
#include "draw_ui.h"

#include "../../../../MarlinCore.h"

static lv_obj_t * scr;

#define ID_MOTOR_RETURN            1
#define ID_MOTOR_STEPS             2
#define ID_MOTOR_STEPS_ARROW       3
#define ID_MOTOR_TMC_CURRENT       4
#define ID_MOTOR_TMC_CURRENT_ARROW 5
#define ID_MOTOR_STEP_MODE         6
#define ID_MOTOR_STEP_MODE_ARROW   7

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_MOTOR_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_motor_settings();
        draw_return_ui();
      }
      break;
    case ID_MOTOR_STEPS:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_motor_settings();
        lv_draw_step_settings();
      }
      break;
    case ID_MOTOR_STEPS_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_motor_settings();
        lv_draw_step_settings();
      }
      break;
      #if HAS_TRINAMIC_CONFIG
        case ID_MOTOR_TMC_CURRENT:
          if (event == LV_EVENT_CLICKED) {

          }
          else if (event == LV_EVENT_RELEASED) {
            lv_clear_motor_settings();
            lv_draw_tmc_current_settings();
          }
          break;
        case ID_MOTOR_TMC_CURRENT_ARROW:
          if (event == LV_EVENT_CLICKED) {

          }
          else if (event == LV_EVENT_RELEASED) {
            lv_clear_motor_settings();
            lv_draw_tmc_current_settings();
          }
          break;
          #if HAS_STEALTHCHOP
            case ID_MOTOR_STEP_MODE:
              if (event == LV_EVENT_CLICKED) {

              }
              else if (event == LV_EVENT_RELEASED) {
                lv_clear_motor_settings();
                lv_draw_tmc_step_mode_settings();
              }
              break;
            case ID_MOTOR_STEP_MODE_ARROW:
              if (event == LV_EVENT_CLICKED) {

              }
              else if (event == LV_EVENT_RELEASED) {
                lv_clear_motor_settings();
                lv_draw_tmc_step_mode_settings();
              }
              break;
          #endif
      #endif
  }
}

void lv_draw_motor_settings(void) {
  lv_obj_t *buttonBack, *label_Back;
  lv_obj_t *buttonSteps, *labelSteps, *buttonStepsNarrow;
  #if HAS_TRINAMIC_CONFIG
    lv_obj_t *buttonTMCcurrent, *labelTMCcurrent, *buttonTMCcurrentNarrow;
    lv_obj_t * line2;
    #if HAS_STEALTHCHOP
      lv_obj_t *buttonStepMode, *labelStepMode, *buttonStepModeNarrow;
      lv_obj_t * line3;
    #endif
  #endif
  lv_obj_t * line1;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != MOTOR_SETTINGS_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = MOTOR_SETTINGS_UI;
  }
  disp_state = MOTOR_SETTINGS_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, machine_menu.MotorConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  LV_IMG_DECLARE(bmp_para_back);
  LV_IMG_DECLARE(bmp_para_arrow);

  buttonSteps = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
  lv_obj_set_pos(buttonSteps, PARA_UI_POS_X, PARA_UI_POS_Y);              /*Set its position*/
  lv_obj_set_size(buttonSteps, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);           /*Set its size*/
  //lv_obj_set_event_cb(buttonMachine, event_handler);
  lv_obj_set_event_cb_mks(buttonSteps, event_handler, ID_MOTOR_STEPS, NULL, 0);
  lv_btn_set_style(buttonSteps, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
  lv_btn_set_style(buttonSteps, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
  lv_btn_set_layout(buttonSteps, LV_LAYOUT_OFF);
  labelSteps = lv_label_create(buttonSteps, NULL);                        /*Add a label to the button*/

  buttonStepsNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonStepsNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonStepsNarrow, event_handler, ID_MOTOR_STEPS_ARROW, "bmp_arrow.bin", 0);
  lv_imgbtn_set_src(buttonStepsNarrow, LV_BTN_STATE_REL, &bmp_para_arrow);
  lv_imgbtn_set_src(buttonStepsNarrow, LV_BTN_STATE_PR, &bmp_para_arrow);
  lv_imgbtn_set_style(buttonStepsNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonStepsNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonStepsNarrow, LV_LAYOUT_OFF);

  line1 = lv_line_create(lv_scr_act(), NULL);
  lv_ex_line(line1, line_points[0]);
  #if HAS_TRINAMIC_CONFIG
    buttonTMCcurrent = lv_btn_create(scr, NULL);                                /*Add a button the current screen*/
    lv_obj_set_pos(buttonTMCcurrent, PARA_UI_POS_X, PARA_UI_POS_Y * 2);         /*Set its position*/
    lv_obj_set_size(buttonTMCcurrent, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);          /*Set its size*/
    //lv_obj_set_event_cb(buttonMachine, event_handler);
    lv_obj_set_event_cb_mks(buttonTMCcurrent, event_handler, ID_MOTOR_TMC_CURRENT, NULL, 0);
    lv_btn_set_style(buttonTMCcurrent, LV_BTN_STYLE_REL, &tft_style_label_rel); /*Set the button's released style*/
    lv_btn_set_style(buttonTMCcurrent, LV_BTN_STYLE_PR, &tft_style_label_pre);  /*Set the button's pressed style*/
    lv_btn_set_layout(buttonTMCcurrent, LV_LAYOUT_OFF);
    labelTMCcurrent = lv_label_create(buttonTMCcurrent, NULL);                  /*Add a label to the button*/

    buttonTMCcurrentNarrow = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonTMCcurrentNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 2 + PARA_UI_ARROW_V);
    lv_obj_set_event_cb_mks(buttonTMCcurrentNarrow, event_handler, ID_MOTOR_TMC_CURRENT_ARROW, "bmp_arrow.bin", 0);
    lv_imgbtn_set_src(buttonTMCcurrentNarrow, LV_BTN_STATE_REL, &bmp_para_arrow);
    lv_imgbtn_set_src(buttonTMCcurrentNarrow, LV_BTN_STATE_PR, &bmp_para_arrow);
    lv_imgbtn_set_style(buttonTMCcurrentNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonTMCcurrentNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_btn_set_layout(buttonTMCcurrentNarrow, LV_LAYOUT_OFF);

    line2 = lv_line_create(lv_scr_act(), NULL);
    lv_ex_line(line2, line_points[1]);

    #if HAS_STEALTHCHOP
      buttonStepMode = lv_btn_create(scr, NULL);                                /*Add a button the current screen*/
      lv_obj_set_pos(buttonStepMode, PARA_UI_POS_X, PARA_UI_POS_Y * 3);         /*Set its position*/
      lv_obj_set_size(buttonStepMode, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);          /*Set its size*/
      //lv_obj_set_event_cb(buttonMachine, event_handler);
      lv_obj_set_event_cb_mks(buttonStepMode, event_handler, ID_MOTOR_STEP_MODE, NULL, 0);
      lv_btn_set_style(buttonStepMode, LV_BTN_STYLE_REL, &tft_style_label_rel); /*Set the button's released style*/
      lv_btn_set_style(buttonStepMode, LV_BTN_STYLE_PR, &tft_style_label_pre);  /*Set the button's pressed style*/
      lv_btn_set_layout(buttonStepMode, LV_LAYOUT_OFF);
      labelStepMode = lv_label_create(buttonStepMode, NULL);                    /*Add a label to the button*/

      buttonStepModeNarrow = lv_imgbtn_create(scr, NULL);
      lv_obj_set_pos(buttonStepModeNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 3 + PARA_UI_ARROW_V);
      lv_obj_set_event_cb_mks(buttonStepModeNarrow, event_handler, ID_MOTOR_STEP_MODE_ARROW, "bmp_arrow.bin", 0);
      lv_imgbtn_set_src(buttonStepModeNarrow, LV_BTN_STATE_REL, &bmp_para_arrow);
      lv_imgbtn_set_src(buttonStepModeNarrow, LV_BTN_STATE_PR, &bmp_para_arrow);
      lv_imgbtn_set_style(buttonStepModeNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
      lv_imgbtn_set_style(buttonStepModeNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
      lv_btn_set_layout(buttonStepModeNarrow, LV_LAYOUT_OFF);

      line3 = lv_line_create(lv_scr_act(), NULL);
      lv_ex_line(line3, line_points[2]);
    #endif
  #endif // HAS_TRINAMIC_CONFIG

  buttonBack = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_MOTOR_RETURN, "bmp_back70x40.bin", 0);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, &bmp_para_back);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, &bmp_para_back);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);

  lv_obj_set_pos(buttonBack, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);
  label_Back = lv_label_create(buttonBack, NULL);

  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);

    lv_label_set_text(labelSteps, machine_menu.StepsConf);
    lv_obj_align(labelSteps, buttonSteps, LV_ALIGN_IN_LEFT_MID, 0, 0);
    #if HAS_TRINAMIC_CONFIG
      lv_label_set_text(labelTMCcurrent, machine_menu.TMCcurrentConf);
      lv_obj_align(labelTMCcurrent, buttonTMCcurrent, LV_ALIGN_IN_LEFT_MID, 0, 0);
      #if HAS_STEALTHCHOP
        lv_label_set_text(labelStepMode, machine_menu.TMCStepModeConf);
        lv_obj_align(labelStepMode, buttonStepMode, LV_ALIGN_IN_LEFT_MID, 0, 0);
      #endif
    #endif
  }

}

void lv_clear_motor_settings() { lv_obj_del(scr); }

#endif // HAS_TFT_LVGL_UI
