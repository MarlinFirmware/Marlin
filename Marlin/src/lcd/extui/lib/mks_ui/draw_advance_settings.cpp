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

#define ID_ADVANCE_RETURN   1
#define ID_PAUSE_POS        2
#define ID_PAUSE_POS_ARROW  3

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_ADVANCE_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_advance_settings();
        draw_return_ui();
      }
      break;
    case ID_PAUSE_POS:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_advance_settings();
        lv_draw_pause_position();
      }
      break;
    case ID_PAUSE_POS_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_advance_settings();
        lv_draw_pause_position();
      }
      break;
  }
}

void lv_draw_advance_settings(void) {
  lv_obj_t *buttonBack, *label_Back;
  lv_obj_t *buttonPausePos, *labelPausePos, *buttonPausePosNarrow;
  lv_obj_t * line1;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != ADVANCED_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = ADVANCED_UI;
  }
  disp_state = ADVANCED_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, machine_menu.AdvancedConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  LV_IMG_DECLARE(bmp_para_back);
  LV_IMG_DECLARE(bmp_para_arrow);

  buttonPausePos = lv_btn_create(scr, NULL);   /*Add a button the current screen*/
  lv_obj_set_pos(buttonPausePos, PARA_UI_POS_X, PARA_UI_POS_Y);                         /*Set its position*/
  lv_obj_set_size(buttonPausePos, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);                       /*Set its size*/
  //lv_obj_set_event_cb(buttonMachine, event_handler);
  lv_obj_set_event_cb_mks(buttonPausePos, event_handler, ID_PAUSE_POS, NULL, 0);
  lv_btn_set_style(buttonPausePos, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
  lv_btn_set_style(buttonPausePos, LV_BTN_STYLE_PR, &tft_style_label_pre);    /*Set the button's pressed style*/
  lv_btn_set_layout(buttonPausePos, LV_LAYOUT_OFF);
  labelPausePos = lv_label_create(buttonPausePos, NULL);        /*Add a label to the button*/

  buttonPausePosNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonPausePosNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonPausePosNarrow, event_handler, ID_PAUSE_POS_ARROW, "bmp_arrow.bin", 0);
  lv_imgbtn_set_src(buttonPausePosNarrow, LV_BTN_STATE_REL, &bmp_para_arrow);
  lv_imgbtn_set_src(buttonPausePosNarrow, LV_BTN_STATE_PR, &bmp_para_arrow);
  lv_imgbtn_set_style(buttonPausePosNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonPausePosNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonPausePosNarrow, LV_LAYOUT_OFF);

  line1 = lv_line_create(lv_scr_act(), NULL);
  lv_ex_line(line1, line_points[0]);

  buttonBack = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_ADVANCE_RETURN, "bmp_back70x40.bin", 0);
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

    lv_label_set_text(labelPausePos, machine_menu.PausePosition);
    lv_obj_align(labelPausePos, buttonPausePos, LV_ALIGN_IN_LEFT_MID, 0, 0);
  }

}

void lv_clear_advance_settings() { lv_obj_del(scr); }

#endif // HAS_TFT_LVGL_UI
