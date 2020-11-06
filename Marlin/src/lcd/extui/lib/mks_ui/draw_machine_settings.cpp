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

#define ID_MACHINE_RETURN             1
#define ID_MACHINE_ACCELERATION       2
#define ID_MACHINE_ACCELERATION_ARROW 3
#define ID_MACHINE_FEEDRATE           4
#define ID_MACHINE_FEEDRATE_ARROW     5
#define ID_MACHINE_JERK               6
#define ID_MACHINE_JERK_ARROW         7

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_MACHINE_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_settings();
        draw_return_ui();
      }
      break;
    case ID_MACHINE_ACCELERATION:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_settings();
        lv_draw_acceleration_settings();
      }
      break;
    case ID_MACHINE_ACCELERATION_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_settings();
        lv_draw_acceleration_settings();
      }
      break;
    case ID_MACHINE_FEEDRATE:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_settings();
        lv_draw_max_feedrate_settings();
      }
      break;
    case ID_MACHINE_FEEDRATE_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_settings();
        lv_draw_max_feedrate_settings();
      }
      break;
      #if HAS_CLASSIC_JERK
        case ID_MACHINE_JERK:
          if (event == LV_EVENT_CLICKED) {

          }
          else if (event == LV_EVENT_RELEASED) {
            lv_clear_machine_settings();
            lv_draw_jerk_settings();
          }
          break;
        case ID_MACHINE_JERK_ARROW:
          if (event == LV_EVENT_CLICKED) {

          }
          else if (event == LV_EVENT_RELEASED) {
            lv_clear_machine_settings();
            lv_draw_jerk_settings();
          }
          break;
      #endif
  }
}

void lv_draw_machine_settings(void) {
  lv_obj_t *buttonBack, *label_Back;
  lv_obj_t *buttonAcceleration, *labelAcceleration, *buttonAccelerationNarrow;
  lv_obj_t *buttonMaxFeedrate, *labelMaxFeedrate, *buttonMaxFeedrateNarrow;
  #if HAS_CLASSIC_JERK
    lv_obj_t *buttonJerk, *labelJerk, *buttonJerkNarrow;
  #endif
  lv_obj_t * line1, * line2;
  #if HAS_CLASSIC_JERK
    lv_obj_t * line3;
  #endif
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != MACHINE_SETTINGS_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = MACHINE_SETTINGS_UI;
  }
  disp_state = MACHINE_SETTINGS_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, machine_menu.MachineConfigTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  LV_IMG_DECLARE(bmp_para_back);
  LV_IMG_DECLARE(bmp_para_arrow);

  buttonAcceleration = lv_btn_create(scr, NULL);                                /*Add a button the current screen*/
  lv_obj_set_pos(buttonAcceleration, PARA_UI_POS_X, PARA_UI_POS_Y);             /*Set its position*/
  lv_obj_set_size(buttonAcceleration, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);          /*Set its size*/
  //lv_obj_set_event_cb(buttonMachine, event_handler);
  lv_obj_set_event_cb_mks(buttonAcceleration, event_handler, ID_MACHINE_ACCELERATION, NULL, 0);
  lv_btn_set_style(buttonAcceleration, LV_BTN_STYLE_REL, &tft_style_label_rel); /*Set the button's released style*/
  lv_btn_set_style(buttonAcceleration, LV_BTN_STYLE_PR, &tft_style_label_pre);  /*Set the button's pressed style*/
  lv_btn_set_layout(buttonAcceleration, LV_LAYOUT_OFF);
  labelAcceleration = lv_label_create(buttonAcceleration, NULL);                /*Add a label to the button*/

  buttonAccelerationNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonAccelerationNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonAccelerationNarrow, event_handler, ID_MACHINE_ACCELERATION_ARROW, "bmp_arrow.bin", 0);
  lv_imgbtn_set_src(buttonAccelerationNarrow, LV_BTN_STATE_REL, &bmp_para_arrow);
  lv_imgbtn_set_src(buttonAccelerationNarrow, LV_BTN_STATE_PR, &bmp_para_arrow);
  lv_imgbtn_set_style(buttonAccelerationNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonAccelerationNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonAccelerationNarrow, LV_LAYOUT_OFF);

  line1 = lv_line_create(lv_scr_act(), NULL);
  lv_ex_line(line1, line_points[0]);

  buttonMaxFeedrate = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
  lv_obj_set_pos(buttonMaxFeedrate, PARA_UI_POS_X, PARA_UI_POS_Y * 2);          /*Set its position*/
  lv_obj_set_size(buttonMaxFeedrate, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);           /*Set its size*/
  //lv_obj_set_event_cb(buttonMachine, event_handler);
  lv_obj_set_event_cb_mks(buttonMaxFeedrate, event_handler, ID_MACHINE_FEEDRATE, NULL, 0);
  lv_btn_set_style(buttonMaxFeedrate, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
  lv_btn_set_style(buttonMaxFeedrate, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
  lv_btn_set_layout(buttonMaxFeedrate, LV_LAYOUT_OFF);
  labelMaxFeedrate = lv_label_create(buttonMaxFeedrate, NULL);                  /*Add a label to the button*/

  buttonMaxFeedrateNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonMaxFeedrateNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 2 + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonMaxFeedrateNarrow, event_handler, ID_MACHINE_FEEDRATE_ARROW, "bmp_arrow.bin", 0);
  lv_imgbtn_set_src(buttonMaxFeedrateNarrow, LV_BTN_STATE_REL, &bmp_para_arrow);
  lv_imgbtn_set_src(buttonMaxFeedrateNarrow, LV_BTN_STATE_PR, &bmp_para_arrow);
  lv_imgbtn_set_style(buttonMaxFeedrateNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonMaxFeedrateNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonMaxFeedrateNarrow, LV_LAYOUT_OFF);

  line2 = lv_line_create(lv_scr_act(), NULL);
  lv_ex_line(line2, line_points[1]);

  #if HAS_CLASSIC_JERK
    buttonJerk = lv_btn_create(scr, NULL);                                      /*Add a button the current screen*/
    lv_obj_set_pos(buttonJerk, PARA_UI_POS_X, PARA_UI_POS_Y * 3);               /*Set its position*/
    lv_obj_set_size(buttonJerk, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);                /*Set its size*/
    //lv_obj_set_event_cb(buttonMotor, event_handler);
    lv_obj_set_event_cb_mks(buttonJerk, event_handler, ID_MACHINE_JERK, NULL, 0);
    lv_btn_set_style(buttonJerk, LV_BTN_STYLE_REL, &tft_style_label_rel);       /*Set the button's released style*/
    lv_btn_set_style(buttonJerk, LV_BTN_STYLE_PR, &tft_style_label_pre);        /*Set the button's pressed style*/
    lv_btn_set_layout(buttonJerk, LV_LAYOUT_OFF);
    labelJerk = lv_label_create(buttonJerk, NULL);                              /*Add a label to the button*/

    buttonJerkNarrow = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonJerkNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 3 + PARA_UI_ARROW_V);
    lv_obj_set_event_cb_mks(buttonJerkNarrow, event_handler, ID_MACHINE_JERK_ARROW, "bmp_arrow.bin", 0);
    lv_imgbtn_set_src(buttonJerkNarrow, LV_BTN_STATE_REL, &bmp_para_arrow);
    lv_imgbtn_set_src(buttonJerkNarrow, LV_BTN_STATE_PR, &bmp_para_arrow);
    lv_imgbtn_set_style(buttonJerkNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonJerkNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_btn_set_layout(buttonJerkNarrow, LV_LAYOUT_OFF);

    line3 = lv_line_create(lv_scr_act(), NULL);
    lv_ex_line(line3, line_points[2]);
  #endif

  buttonBack = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_MACHINE_RETURN, "bmp_back70x40.bin", 0);
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

    lv_label_set_text(labelAcceleration, machine_menu.AccelerationConf);
    lv_obj_align(labelAcceleration, buttonAcceleration, LV_ALIGN_IN_LEFT_MID, 0, 0);

    lv_label_set_text(labelMaxFeedrate, machine_menu.MaxFeedRateConf);
    lv_obj_align(labelMaxFeedrate, buttonMaxFeedrate, LV_ALIGN_IN_LEFT_MID, 0, 0);
    #if HAS_CLASSIC_JERK
      lv_label_set_text(labelJerk, machine_menu.JerkConf);
      lv_obj_align(labelJerk, buttonJerk, LV_ALIGN_IN_LEFT_MID, 0, 0);
    #endif
  }
}

void lv_clear_machine_settings() { lv_obj_del(scr); }

#endif // HAS_TFT_LVGL_UI
