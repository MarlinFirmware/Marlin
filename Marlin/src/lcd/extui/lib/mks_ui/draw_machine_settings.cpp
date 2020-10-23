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

extern lv_group_t * g;
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

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, machine_menu.MachineConfigTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  buttonAcceleration = lv_btn_create(scr, NULL);                                // Add a button the current screen
  lv_obj_set_pos(buttonAcceleration, PARA_UI_POS_X, PARA_UI_POS_Y);             // Set its position
  lv_obj_set_size(buttonAcceleration, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);          // Set its size
  //lv_obj_set_event_cb(buttonMachine, event_handler);
  lv_obj_set_event_cb_mks(buttonAcceleration, event_handler, ID_MACHINE_ACCELERATION, NULL, 0);
  lv_btn_use_label_style(buttonAcceleration);
  lv_btn_set_layout(buttonAcceleration, LV_LAYOUT_OFF);
  labelAcceleration = lv_label_create_empty(buttonAcceleration);                // Add a label to the button

  buttonAccelerationNarrow = lv_imgbtn_create(scr, "F:/bmp_arrow.bin", PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y + PARA_UI_ARROW_V, event_handler, ID_MACHINE_ACCELERATION_ARROW);

  line1 = lv_line_create(lv_scr_act(), NULL);
  lv_ex_line(line1, line_points[0]);

  buttonMaxFeedrate = lv_btn_create(scr, NULL);                                 // Add a button the current screen
  lv_obj_set_pos(buttonMaxFeedrate, PARA_UI_POS_X, PARA_UI_POS_Y * 2);          // Set its position
  lv_obj_set_size(buttonMaxFeedrate, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);           // Set its size
  //lv_obj_set_event_cb(buttonMachine, event_handler);
  lv_obj_set_event_cb_mks(buttonMaxFeedrate, event_handler, ID_MACHINE_FEEDRATE, NULL, 0);
  lv_btn_use_label_style(buttonMaxFeedrate);
  lv_btn_set_layout(buttonMaxFeedrate, LV_LAYOUT_OFF);
  labelMaxFeedrate = lv_label_create_empty(buttonMaxFeedrate);                  // Add a label to the button

  buttonMaxFeedrateNarrow = lv_imgbtn_create(scr, "F:/bmp_arrow.bin", PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 2 + PARA_UI_ARROW_V, event_handler, ID_MACHINE_FEEDRATE_ARROW);

  line2 = lv_line_create(lv_scr_act(), NULL);
  lv_ex_line(line2, line_points[1]);

  #if HAS_CLASSIC_JERK
    buttonJerk = lv_btn_create(scr, NULL);                                      // Add a button the current screen
    lv_obj_set_pos(buttonJerk, PARA_UI_POS_X, PARA_UI_POS_Y * 3);               // Set its position
    lv_obj_set_size(buttonJerk, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);                // Set its size
    //lv_obj_set_event_cb(buttonMotor, event_handler);
    lv_obj_set_event_cb_mks(buttonJerk, event_handler, ID_MACHINE_JERK, NULL, 0);
    lv_btn_use_label_style(buttonJerk);
    lv_btn_set_layout(buttonJerk, LV_LAYOUT_OFF);
    labelJerk = lv_label_create_empty(buttonJerk);                              // Add a label to the button

    buttonJerkNarrow = lv_imgbtn_create(scr, "F:/bmp_arrow.bin", PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 3 + PARA_UI_ARROW_V, event_handler, ID_MACHINE_JERK_ARROW);

    line3 = lv_line_create(lv_scr_act(), NULL);
    lv_ex_line(line3, line_points[2]);
  #endif

  buttonBack = lv_imgbtn_create(scr, "F:/bmp_back70x40.bin", event_handler, ID_MACHINE_RETURN);

  lv_obj_set_pos(buttonBack, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);
  label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
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
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonAcceleration);
      lv_group_add_obj(g, buttonMaxFeedrate);
      #if HAS_CLASSIC_JERK
        lv_group_add_obj(g, buttonJerk);
      #endif
      lv_group_add_obj(g, buttonBack);
    }
  #endif
}

void lv_clear_machine_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
