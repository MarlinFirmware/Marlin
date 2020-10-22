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

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  buttonMachine = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
  lv_obj_set_pos(buttonMachine, PARA_UI_POS_X, PARA_UI_POS_Y);              /*Set its position*/
  lv_obj_set_size(buttonMachine, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);           /*Set its size*/
  //lv_obj_set_event_cb(buttonMachine, event_handler);
  lv_obj_set_event_cb_mks(buttonMachine, event_handler, ID_PARA_MACHINE, NULL, 0);
  lv_btn_use_label_style(buttonMachine);
  lv_btn_set_layout(buttonMachine, LV_LAYOUT_OFF);
  labelMachine = lv_label_create_empty(buttonMachine);                      /*Add a label to the button*/

  buttonMachineNarrow = lv_imgbtn_create(scr, "F:/bmp_arrow.bin", PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y + PARA_UI_ARROW_V, event_handler, ID_PARA_MACHINE_ARROW);

  line1 = lv_line_create(scr, NULL);
  lv_ex_line(line1, line_points[0]);

  buttonMotor = lv_btn_create(scr, NULL);                                   /*Add a button the current screen*/
  lv_obj_set_pos(buttonMotor, PARA_UI_POS_X, PARA_UI_POS_Y * 2);            /*Set its position*/
  lv_obj_set_size(buttonMotor, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);             /*Set its size*/
  //lv_obj_set_event_cb(buttonMotor, event_handler);
  lv_obj_set_event_cb_mks(buttonMotor, event_handler, ID_PARA_MOTOR, NULL, 0);
  lv_btn_use_label_style(buttonMotor);
  lv_btn_set_layout(buttonMotor, LV_LAYOUT_OFF);
  labelMotor = lv_label_create_empty(buttonMotor);                          /*Add a label to the button*/

  buttonMotorNarrow = lv_imgbtn_create(scr, "F:/bmp_arrow.bin", PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 2 + PARA_UI_ARROW_V, event_handler, ID_PARA_MOTOR_ARROW);

  line2 = lv_line_create(scr, NULL);
  lv_ex_line(line2, line_points[1]);

  buttonLevel = lv_btn_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 3, PARA_UI_SIZE_X, PARA_UI_SIZE_Y, event_handler, ID_PARA_LEVEL);
  //lv_obj_set_event_cb(buttonMotor, event_handler);
  lv_btn_set_layout(buttonLevel, LV_LAYOUT_OFF);
  labelLevel = lv_label_create_empty(buttonLevel);                      /*Add a label to the button*/

  buttonLevelNarrow = lv_imgbtn_create(scr, "F:/bmp_arrow.bin", PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 3 + PARA_UI_ARROW_V, event_handler, ID_PARA_LEVEL_ARROW);

  line3 = lv_line_create(scr, NULL);
  lv_ex_line(line3, line_points[2]);

  buttonAdvance = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
  lv_obj_set_pos(buttonAdvance, PARA_UI_POS_X, PARA_UI_POS_Y * 4);          /*Set its position*/
  lv_obj_set_size(buttonAdvance, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);           /*Set its size*/
  //lv_obj_set_event_cb(buttonMotor, event_handler);
  lv_obj_set_event_cb_mks(buttonAdvance, event_handler, ID_PARA_ADVANCE, NULL, 0);
  lv_btn_use_label_style(buttonAdvance);
  lv_btn_set_layout(buttonAdvance, LV_LAYOUT_OFF);
  labelAdvance = lv_label_create_empty(buttonAdvance);                      /*Add a label to the button*/

  buttonAdvanceNarrow = lv_imgbtn_create(scr, "F:/bmp_arrow.bin", PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 4 + PARA_UI_ARROW_V, event_handler, ID_PARA_ADVANCE_ARROW);
  lv_btn_set_layout(buttonAdvanceNarrow, LV_LAYOUT_OFF);

  line4 = lv_line_create(scr, NULL);
  lv_ex_line(line4, line_points[3]);

  buttonBack = lv_imgbtn_create(scr, "F:/bmp_back70x40.bin", PARA_UI_BACL_POS_X + 10, PARA_UI_BACL_POS_Y, event_handler, ID_PARA_RETURN);
  label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
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
