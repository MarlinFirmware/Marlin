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

extern lv_group_t * g;
static lv_obj_t * scr;

#define ID_PARA_RETURN        1
#define ID_PARA_MACHINE       2
#define ID_PARA_MACHINE_ARROW 3
#define ID_PARA_MOTOR         4
#define ID_PARA_MOTOR_ARROW   5
#define ID_PARA_LEVEL         6
#define ID_PARA_LEVEL_ARROW   7
#define ID_PARA_ADVANCE       8
#define ID_PARA_ADVANCE_ARROW 9

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_PARA_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_para();
        draw_return_ui();
      }
      break;
    case ID_PARA_MACHINE:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_para();
        lv_draw_machine_settings();
      }
      break;
    case ID_PARA_MACHINE_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_para();
        lv_draw_machine_settings();
      }
      break;
    case ID_PARA_MOTOR:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_para();
        lv_draw_motor_settings();
      }
      break;
    case ID_PARA_MOTOR_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_para();
        lv_draw_motor_settings();
      }
      break;
    case ID_PARA_LEVEL:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_para();
        lv_draw_level_settings();
      }
      break;
    case ID_PARA_LEVEL_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_para();
        lv_draw_level_settings();
      }
      break;
    case ID_PARA_ADVANCE:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_para();
        lv_draw_advance_settings();
      }
      break;
    case ID_PARA_ADVANCE_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_machine_para();
        lv_draw_advance_settings();
      }
      break;
  }
}

void lv_draw_machine_para(void) {
  lv_obj_t *buttonBack, *label_Back;
  lv_obj_t *buttonMachine, *labelMachine, *buttonMachineNarrow;
  lv_obj_t *buttonMotor, *labelMotor, *buttonMotorNarrow;
  lv_obj_t *buttonLevel, *labelLevel, *buttonLevelNarrow;
  lv_obj_t *buttonAdvance, *labelAdvance, *buttonAdvanceNarrow;
  lv_obj_t * line1, * line2, * line3, * line4;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != MACHINE_PARA_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = MACHINE_PARA_UI;
  }
  disp_state = MACHINE_PARA_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  buttonMachine = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
  lv_obj_set_pos(buttonMachine, PARA_UI_POS_X, PARA_UI_POS_Y);              /*Set its position*/
  lv_obj_set_size(buttonMachine, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);           /*Set its size*/
  //lv_obj_set_event_cb(buttonMachine, event_handler);
  lv_obj_set_event_cb_mks(buttonMachine, event_handler, ID_PARA_MACHINE, NULL, 0);
  lv_btn_set_style(buttonMachine, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
  lv_btn_set_style(buttonMachine, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
  lv_btn_set_layout(buttonMachine, LV_LAYOUT_OFF);
  labelMachine = lv_label_create(buttonMachine, NULL);                      /*Add a label to the button*/

  buttonMachineNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonMachineNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonMachineNarrow, event_handler, ID_PARA_MACHINE_ARROW, NULL, 0);
  lv_imgbtn_set_src(buttonMachineNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
  lv_imgbtn_set_src(buttonMachineNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
  lv_imgbtn_set_style(buttonMachineNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonMachineNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonMachineNarrow, LV_LAYOUT_OFF);

  line1 = lv_line_create(scr, NULL);
  lv_ex_line(line1, line_points[0]);

  buttonMotor = lv_btn_create(scr, NULL);                                   /*Add a button the current screen*/
  lv_obj_set_pos(buttonMotor, PARA_UI_POS_X, PARA_UI_POS_Y * 2);            /*Set its position*/
  lv_obj_set_size(buttonMotor, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);             /*Set its size*/
  //lv_obj_set_event_cb(buttonMotor, event_handler);
  lv_obj_set_event_cb_mks(buttonMotor, event_handler, ID_PARA_MOTOR, NULL, 0);
  lv_btn_set_style(buttonMotor, LV_BTN_STYLE_REL, &tft_style_label_rel);    /*Set the button's released style*/
  lv_btn_set_style(buttonMotor, LV_BTN_STYLE_PR, &tft_style_label_pre);     /*Set the button's pressed style*/
  lv_btn_set_layout(buttonMotor, LV_LAYOUT_OFF);
  labelMotor = lv_label_create(buttonMotor, NULL);                          /*Add a label to the button*/

  buttonMotorNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonMotorNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 2 + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonMotorNarrow, event_handler, ID_PARA_MOTOR_ARROW, NULL, 0);
  lv_imgbtn_set_src(buttonMotorNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
  lv_imgbtn_set_src(buttonMotorNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
  lv_imgbtn_set_style(buttonMotorNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonMotorNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonMotorNarrow, LV_LAYOUT_OFF);

  line2 = lv_line_create(scr, NULL);
  lv_ex_line(line2, line_points[1]);

  buttonLevel = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
  lv_obj_set_pos(buttonLevel, PARA_UI_POS_X, PARA_UI_POS_Y * 3);          /*Set its position*/
  lv_obj_set_size(buttonLevel, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);           /*Set its size*/
  //lv_obj_set_event_cb(buttonMotor, event_handler);
  lv_obj_set_event_cb_mks(buttonLevel, event_handler, ID_PARA_LEVEL, NULL, 0);
  lv_btn_set_style(buttonLevel, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
  lv_btn_set_style(buttonLevel, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
  lv_btn_set_layout(buttonLevel, LV_LAYOUT_OFF);
  labelLevel = lv_label_create(buttonLevel, NULL);                      /*Add a label to the button*/

  buttonLevelNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonLevelNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 3 + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonLevelNarrow, event_handler, ID_PARA_LEVEL_ARROW, NULL, 0);
  lv_imgbtn_set_src(buttonLevelNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
  lv_imgbtn_set_src(buttonLevelNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
  lv_imgbtn_set_style(buttonLevelNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonLevelNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonLevelNarrow, LV_LAYOUT_OFF);

  line3 = lv_line_create(scr, NULL);
  lv_ex_line(line3, line_points[2]);

  buttonAdvance = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
  lv_obj_set_pos(buttonAdvance, PARA_UI_POS_X, PARA_UI_POS_Y * 4);          /*Set its position*/
  lv_obj_set_size(buttonAdvance, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);           /*Set its size*/
  //lv_obj_set_event_cb(buttonMotor, event_handler);
  lv_obj_set_event_cb_mks(buttonAdvance, event_handler, ID_PARA_ADVANCE, NULL, 0);
  lv_btn_set_style(buttonAdvance, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
  lv_btn_set_style(buttonAdvance, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
  lv_btn_set_layout(buttonAdvance, LV_LAYOUT_OFF);
  labelAdvance = lv_label_create(buttonAdvance, NULL);                      /*Add a label to the button*/

  buttonAdvanceNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonAdvanceNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 4 + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonAdvanceNarrow, event_handler, ID_PARA_ADVANCE_ARROW, NULL, 0);
  lv_imgbtn_set_src(buttonAdvanceNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
  lv_imgbtn_set_src(buttonAdvanceNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
  lv_imgbtn_set_style(buttonAdvanceNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonAdvanceNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonAdvanceNarrow, LV_LAYOUT_OFF);

  line4 = lv_line_create(scr, NULL);
  lv_ex_line(line4, line_points[3]);

  buttonBack = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_PARA_RETURN, NULL, 0);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_back70x40.bin");
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_back70x40.bin");
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);

  lv_obj_set_pos(buttonBack, PARA_UI_BACL_POS_X + 10, PARA_UI_BACL_POS_Y);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);
  label_Back = lv_label_create(buttonBack, NULL);

  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, -2);

    lv_label_set_text(labelMachine, MachinePara_menu.MachineSetting);
    lv_obj_align(labelMachine, buttonMachine, LV_ALIGN_IN_LEFT_MID, 0, -3);

    lv_label_set_text(labelMotor, MachinePara_menu.MotorSetting);
    lv_obj_align(labelMotor, buttonMotor, LV_ALIGN_IN_LEFT_MID, 0, -3);

    lv_label_set_text(labelLevel, MachinePara_menu.leveling);
    lv_obj_align(labelLevel, buttonLevel, LV_ALIGN_IN_LEFT_MID, 0, -3);

    lv_label_set_text(labelAdvance, MachinePara_menu.AdvanceSetting);
    lv_obj_align(labelAdvance, buttonAdvance, LV_ALIGN_IN_LEFT_MID, 0, -3);
  }
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonMachine);
      lv_group_add_obj(g, buttonMotor);
      lv_group_add_obj(g, buttonLevel);
      lv_group_add_obj(g, buttonAdvance);
      lv_group_add_obj(g, buttonBack);
    }
  #endif

}

void lv_clear_machine_para() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
