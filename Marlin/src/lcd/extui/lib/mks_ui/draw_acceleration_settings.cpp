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
#include "../../../../module/planner.h"

static lv_obj_t * scr;

#define ID_ACCE_RETURN  1
#define ID_ACCE_PRINT   2
#define ID_ACCE_RETRA   3
#define ID_ACCE_TRAVEL  4
#define ID_ACCE_X       5
#define ID_ACCE_Y       6
#define ID_ACCE_Z       7
#define ID_ACCE_E0      8
#define ID_ACCE_E1      9
#define ID_ACCE_UP     10
#define ID_ACCE_DOWN   11

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_ACCE_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_acceleration_settings();
        draw_return_ui();
      }
      break;
    case ID_ACCE_PRINT:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = PrintAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_RETRA:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = RetractAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_TRAVEL:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = TravelAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_X:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = XAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_Y:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = YAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_Z:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = ZAcceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_E0:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = E0Acceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_E1:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        value = E1Acceleration;
        lv_clear_acceleration_settings();
        lv_draw_number_key();
      }
      break;
    case ID_ACCE_UP:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 0;
        lv_clear_acceleration_settings();
        lv_draw_acceleration_settings();
      }
      break;
    case ID_ACCE_DOWN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        uiCfg.para_ui_page = 1;
        lv_clear_acceleration_settings();
        lv_draw_acceleration_settings();
      }
      break;
  }
}

void lv_draw_acceleration_settings(void) {
  lv_obj_t *buttonBack = NULL, *label_Back = NULL, *buttonTurnPage = NULL, *labelTurnPage = NULL;
  lv_obj_t *buttonPrintText = NULL, *labelPrintText = NULL, *buttonPrintValue = NULL, *labelPrintValue = NULL;
  lv_obj_t *buttonRetraText = NULL, *labelRetraText = NULL, *buttonRetraValue = NULL, *labelRetraValue = NULL;
  lv_obj_t *buttonTravelText = NULL, *labelTravelText = NULL, *buttonTravelValue = NULL, *labelTravelValue = NULL;
  lv_obj_t *buttonXText = NULL, *labelXText = NULL, *buttonXValue = NULL, *labelXValue = NULL;
  lv_obj_t *buttonYText = NULL, *labelYText = NULL, *buttonYValue = NULL, *labelYValue = NULL;
  lv_obj_t *buttonZText = NULL, *labelZText = NULL, *buttonZValue = NULL, *labelZValue = NULL;
  lv_obj_t *buttonE0Text = NULL, *labelE0Text = NULL, *buttonE0Value = NULL, *labelE0Value = NULL;
  lv_obj_t *buttonE1Text = NULL, *labelE1Text = NULL, *buttonE1Value = NULL, *labelE1Value = NULL;
  lv_obj_t * line1 = NULL, * line2 = NULL, * line3 = NULL, * line4 = NULL;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != ACCELERATION_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = ACCELERATION_UI;
  }
  disp_state = ACCELERATION_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, machine_menu.AccelerationConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  LV_IMG_DECLARE(bmp_para_back);
  // LV_IMG_DECLARE(bmp_para_arrow);
  LV_IMG_DECLARE(bmp_para_bank);

  if (uiCfg.para_ui_page != 1) {
    buttonPrintText = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
    lv_obj_set_pos(buttonPrintText, PARA_UI_POS_X, PARA_UI_POS_Y);              /*Set its position*/
    lv_obj_set_size(buttonPrintText, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);     /*Set its size*/
    lv_obj_set_event_cb(buttonPrintText, event_handler);
    lv_btn_set_style(buttonPrintText, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
    lv_btn_set_style(buttonPrintText, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
    lv_btn_set_layout(buttonPrintText, LV_LAYOUT_OFF);
    labelPrintText = lv_label_create(buttonPrintText, NULL);                    /*Add a label to the button*/

    buttonPrintValue = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonPrintValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V);
    lv_obj_set_event_cb_mks(buttonPrintValue, event_handler, ID_ACCE_PRINT, "bmp_value_blank.bin", 0);
    lv_imgbtn_set_src(buttonPrintValue, LV_BTN_STATE_REL, &bmp_para_bank);
    lv_imgbtn_set_src(buttonPrintValue, LV_BTN_STATE_PR, &bmp_para_bank);
    lv_imgbtn_set_style(buttonPrintValue, LV_BTN_STATE_PR, &style_para_value_pre);
    lv_imgbtn_set_style(buttonPrintValue, LV_BTN_STATE_REL, &style_para_value_rel);
    lv_btn_set_layout(buttonPrintValue, LV_LAYOUT_OFF);
    labelPrintValue = lv_label_create(buttonPrintValue, NULL);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    buttonRetraText = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
    lv_obj_set_pos(buttonRetraText, PARA_UI_POS_X, PARA_UI_POS_Y * 2);          /*Set its position*/
    lv_obj_set_size(buttonRetraText, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);     /*Set its size*/
    lv_obj_set_event_cb(buttonRetraText, event_handler);
    lv_btn_set_style(buttonRetraText, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
    lv_btn_set_style(buttonRetraText, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
    lv_btn_set_layout(buttonRetraText, LV_LAYOUT_OFF);
    labelRetraText = lv_label_create(buttonRetraText, NULL);                    /*Add a label to the button*/

    buttonRetraValue = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonRetraValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V);
    lv_obj_set_event_cb_mks(buttonRetraValue, event_handler, ID_ACCE_RETRA, "bmp_value_blank.bin", 0);
    lv_imgbtn_set_src(buttonRetraValue, LV_BTN_STATE_REL, &bmp_para_bank);
    lv_imgbtn_set_src(buttonRetraValue, LV_BTN_STATE_PR, &bmp_para_bank);
    lv_imgbtn_set_style(buttonRetraValue, LV_BTN_STATE_PR, &style_para_value_pre);
    lv_imgbtn_set_style(buttonRetraValue, LV_BTN_STATE_REL, &style_para_value_rel);
    lv_btn_set_layout(buttonRetraValue, LV_LAYOUT_OFF);
    labelRetraValue = lv_label_create(buttonRetraValue, NULL);

    line2 = lv_line_create(scr, NULL);
    lv_ex_line(line2, line_points[1]);

    buttonTravelText = lv_btn_create(scr, NULL);                                /*Add a button the current screen*/
    lv_obj_set_pos(buttonTravelText, PARA_UI_POS_X, PARA_UI_POS_Y * 3);         /*Set its position*/
    lv_obj_set_size(buttonTravelText, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);    /*Set its size*/
    lv_obj_set_event_cb(buttonTravelText, event_handler);
    lv_btn_set_style(buttonTravelText, LV_BTN_STYLE_REL, &tft_style_label_rel); /*Set the button's released style*/
    lv_btn_set_style(buttonTravelText, LV_BTN_STYLE_PR, &tft_style_label_pre);  /*Set the button's pressed style*/
    lv_btn_set_layout(buttonTravelText, LV_LAYOUT_OFF);
    labelTravelText = lv_label_create(buttonTravelText, NULL);                  /*Add a label to the button*/

    buttonTravelValue = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonTravelValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V);
    lv_obj_set_event_cb_mks(buttonTravelValue, event_handler, ID_ACCE_TRAVEL, "bmp_value_blank.bin", 0);
    lv_imgbtn_set_src(buttonTravelValue, LV_BTN_STATE_REL, &bmp_para_bank);
    lv_imgbtn_set_src(buttonTravelValue, LV_BTN_STATE_PR, &bmp_para_bank);
    lv_imgbtn_set_style(buttonTravelValue, LV_BTN_STATE_PR, &style_para_value_pre);
    lv_imgbtn_set_style(buttonTravelValue, LV_BTN_STATE_REL, &style_para_value_rel);
    lv_btn_set_layout(buttonTravelValue, LV_LAYOUT_OFF);
    labelTravelValue = lv_label_create(buttonTravelValue, NULL);

    line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3, line_points[2]);

    buttonXText = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
    lv_obj_set_pos(buttonXText, PARA_UI_POS_X, PARA_UI_POS_Y * 4);          /*Set its position*/
    lv_obj_set_size(buttonXText, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);     /*Set its size*/
    lv_obj_set_event_cb(buttonXText, event_handler);
    lv_btn_set_style(buttonXText, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
    lv_btn_set_style(buttonXText, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
    lv_btn_set_layout(buttonXText, LV_LAYOUT_OFF);
    labelXText = lv_label_create(buttonXText, NULL);                        /*Add a label to the button*/

    buttonXValue = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonXValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_VALUE_V);
    lv_obj_set_event_cb_mks(buttonXValue, event_handler, ID_ACCE_X, "bmp_value_blank.bin", 0);
    lv_imgbtn_set_src(buttonXValue, LV_BTN_STATE_REL, &bmp_para_bank);
    lv_imgbtn_set_src(buttonXValue, LV_BTN_STATE_PR, &bmp_para_bank);
    lv_imgbtn_set_style(buttonXValue, LV_BTN_STATE_PR, &style_para_value_pre);
    lv_imgbtn_set_style(buttonXValue, LV_BTN_STATE_REL, &style_para_value_rel);
    lv_btn_set_layout(buttonXValue, LV_LAYOUT_OFF);
    labelXValue = lv_label_create(buttonXValue, NULL);

    line4 = lv_line_create(scr, NULL);
    lv_ex_line(line4, line_points[3]);

    buttonTurnPage = lv_imgbtn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonTurnPage, event_handler, ID_ACCE_DOWN, "bmp_back70x40.bin", 0);
    lv_imgbtn_set_src(buttonTurnPage, LV_BTN_STATE_REL, &bmp_para_back);
    lv_imgbtn_set_src(buttonTurnPage, LV_BTN_STATE_PR, &bmp_para_back);
    lv_imgbtn_set_style(buttonTurnPage, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonTurnPage, LV_BTN_STATE_REL, &tft_style_label_rel);
  }
  else {
    buttonYText = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
    lv_obj_set_pos(buttonYText, PARA_UI_POS_X, PARA_UI_POS_Y);              /*Set its position*/
    lv_obj_set_size(buttonYText, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);     /*Set its size*/
    lv_obj_set_event_cb(buttonYText, event_handler);
    lv_btn_set_style(buttonYText, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
    lv_btn_set_style(buttonYText, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
    lv_btn_set_layout(buttonYText, LV_LAYOUT_OFF);
    labelYText = lv_label_create(buttonYText, NULL);                        /*Add a label to the button*/

    buttonYValue = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonYValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V);
    lv_obj_set_event_cb_mks(buttonYValue, event_handler, ID_ACCE_Y, "bmp_value_blank.bin", 0);
    lv_imgbtn_set_src(buttonYValue, LV_BTN_STATE_REL, &bmp_para_bank);
    lv_imgbtn_set_src(buttonYValue, LV_BTN_STATE_PR, &bmp_para_bank);
    lv_imgbtn_set_style(buttonYValue, LV_BTN_STATE_PR, &style_para_value_pre);
    lv_imgbtn_set_style(buttonYValue, LV_BTN_STATE_REL, &style_para_value_rel);
    lv_btn_set_layout(buttonYValue, LV_LAYOUT_OFF);
    labelYValue = lv_label_create(buttonYValue, NULL);

    line1 = lv_line_create(scr, NULL);
    lv_ex_line(line1, line_points[0]);

    buttonZText = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
    lv_obj_set_pos(buttonZText, PARA_UI_POS_X, PARA_UI_POS_Y * 2);          /*Set its position*/
    lv_obj_set_size(buttonZText, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);     /*Set its size*/
    lv_obj_set_event_cb(buttonZText, event_handler);
    lv_btn_set_style(buttonZText, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
    lv_btn_set_style(buttonZText, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
    lv_btn_set_layout(buttonZText, LV_LAYOUT_OFF);
    labelZText = lv_label_create(buttonZText, NULL);                        /*Add a label to the button*/

    buttonZValue = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonZValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V);
    lv_obj_set_event_cb_mks(buttonZValue, event_handler, ID_ACCE_Z, "bmp_value_blank.bin", 0);
    lv_imgbtn_set_src(buttonZValue, LV_BTN_STATE_REL, &bmp_para_bank);
    lv_imgbtn_set_src(buttonZValue, LV_BTN_STATE_PR, &bmp_para_bank);
    lv_imgbtn_set_style(buttonZValue, LV_BTN_STATE_PR, &style_para_value_pre);
    lv_imgbtn_set_style(buttonZValue, LV_BTN_STATE_REL, &style_para_value_rel);
    lv_btn_set_layout(buttonZValue, LV_LAYOUT_OFF);
    labelZValue = lv_label_create(buttonZValue, NULL);

    line2 = lv_line_create(scr, NULL);
    lv_ex_line(line2, line_points[1]);

    buttonE0Text = lv_btn_create(scr, NULL);                                /*Add a button the current screen*/
    lv_obj_set_pos(buttonE0Text, PARA_UI_POS_X, PARA_UI_POS_Y * 3);         /*Set its position*/
    lv_obj_set_size(buttonE0Text, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);    /*Set its size*/
    lv_obj_set_event_cb(buttonE0Text, event_handler);
    lv_btn_set_style(buttonE0Text, LV_BTN_STYLE_REL, &tft_style_label_rel); /*Set the button's released style*/
    lv_btn_set_style(buttonE0Text, LV_BTN_STYLE_PR, &tft_style_label_pre);  /*Set the button's pressed style*/
    lv_btn_set_layout(buttonE0Text, LV_LAYOUT_OFF);
    labelE0Text = lv_label_create(buttonE0Text, NULL);                      /*Add a label to the button*/

    buttonE0Value = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonE0Value, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V);
    lv_obj_set_event_cb_mks(buttonE0Value, event_handler, ID_ACCE_E0, "bmp_value_blank.bin", 0);
    lv_imgbtn_set_src(buttonE0Value, LV_BTN_STATE_REL, &bmp_para_bank);
    lv_imgbtn_set_src(buttonE0Value, LV_BTN_STATE_PR, &bmp_para_bank);
    lv_imgbtn_set_style(buttonE0Value, LV_BTN_STATE_PR, &style_para_value_pre);
    lv_imgbtn_set_style(buttonE0Value, LV_BTN_STATE_REL, &style_para_value_rel);
    lv_btn_set_layout(buttonE0Value, LV_LAYOUT_OFF);
    labelE0Value = lv_label_create(buttonE0Value, NULL);

    line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3, line_points[2]);

    buttonE1Text = lv_btn_create(scr, NULL);                                /*Add a button the current screen*/
    lv_obj_set_pos(buttonE1Text, PARA_UI_POS_X, PARA_UI_POS_Y * 4);         /*Set its position*/
    lv_obj_set_size(buttonE1Text, PARA_UI_VALUE_SIZE_X, PARA_UI_SIZE_Y);    /*Set its size*/
    lv_obj_set_event_cb(buttonE1Text, event_handler);
    lv_btn_set_style(buttonE1Text, LV_BTN_STYLE_REL, &tft_style_label_rel); /*Set the button's released style*/
    lv_btn_set_style(buttonE1Text, LV_BTN_STYLE_PR, &tft_style_label_pre);  /*Set the button's pressed style*/
    lv_btn_set_layout(buttonE1Text, LV_LAYOUT_OFF);
    labelE1Text = lv_label_create(buttonE1Text, NULL);                      /*Add a label to the button*/

    buttonE1Value = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonE1Value, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_VALUE_V);
    lv_obj_set_event_cb_mks(buttonE1Value, event_handler, ID_ACCE_E1, "bmp_value_blank.bin", 0);
    lv_imgbtn_set_src(buttonE1Value, LV_BTN_STATE_REL, &bmp_para_bank);
    lv_imgbtn_set_src(buttonE1Value, LV_BTN_STATE_PR, &bmp_para_bank);
    lv_imgbtn_set_style(buttonE1Value, LV_BTN_STATE_PR, &style_para_value_pre);
    lv_imgbtn_set_style(buttonE1Value, LV_BTN_STATE_REL, &style_para_value_rel);
    lv_btn_set_layout(buttonE1Value, LV_LAYOUT_OFF);
    labelE1Value = lv_label_create(buttonE1Value, NULL);

    line4 = lv_line_create(scr, NULL);
    lv_ex_line(line4, line_points[3]);

    buttonTurnPage = lv_imgbtn_create(scr, NULL);
    lv_obj_set_event_cb_mks(buttonTurnPage, event_handler, ID_ACCE_UP, "bmp_back70x40.bin", 0);
    lv_imgbtn_set_src(buttonTurnPage, LV_BTN_STATE_REL, &bmp_para_back);
    lv_imgbtn_set_src(buttonTurnPage, LV_BTN_STATE_PR, &bmp_para_back);
    lv_imgbtn_set_style(buttonTurnPage, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonTurnPage, LV_BTN_STATE_REL, &tft_style_label_rel);
  }

  lv_obj_set_pos(buttonTurnPage, PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y);
  lv_btn_set_layout(buttonTurnPage, LV_LAYOUT_OFF);
  labelTurnPage = lv_label_create(buttonTurnPage, NULL);

  buttonBack = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_ACCE_RETURN, "bmp_back70x40.bin", 0);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, &bmp_para_back);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, &bmp_para_back);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);

  lv_obj_set_pos(buttonBack, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);
  label_Back = lv_label_create(buttonBack, NULL);

  if (gCfgItems.multiple_language != 0) {
    if (uiCfg.para_ui_page != 1) {
      lv_label_set_text(labelPrintText, machine_menu.PrintAcceleration);
      lv_obj_align(labelPrintText, buttonPrintText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelRetraText, machine_menu.RetractAcceleration);
      lv_obj_align(labelRetraText, buttonRetraText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelTravelText, machine_menu.TravelAcceleration);
      lv_obj_align(labelTravelText, buttonTravelText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelXText, machine_menu.X_Acceleration);
      lv_obj_align(labelXText, buttonXText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelTurnPage, machine_menu.next);
      lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%.1f"), planner.settings.acceleration);
      lv_label_set_text(labelPrintValue, public_buf_l);
      lv_obj_align(labelPrintValue, buttonPrintValue, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%.1f"), planner.settings.retract_acceleration);
      lv_label_set_text(labelRetraValue, public_buf_l);
      lv_obj_align(labelRetraValue, buttonRetraValue, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%.1f"), planner.settings.travel_acceleration);
      lv_label_set_text(labelTravelValue, public_buf_l);
      lv_obj_align(labelTravelValue, buttonTravelValue, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[X_AXIS]);
      lv_label_set_text(labelXValue, public_buf_l);
      lv_obj_align(labelXValue, buttonXValue, LV_ALIGN_CENTER, 0, 0);
    }
    else {
      lv_label_set_text(labelYText, machine_menu.Y_Acceleration);
      lv_obj_align(labelYText, buttonYText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelZText, machine_menu.Z_Acceleration);
      lv_obj_align(labelZText, buttonZText, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelE0Text, machine_menu.E0_Acceleration);
      lv_obj_align(labelE0Text, buttonE0Text, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelE1Text, machine_menu.E1_Acceleration);
      lv_obj_align(labelE1Text, buttonE1Text, LV_ALIGN_IN_LEFT_MID, 0, 0);

      lv_label_set_text(labelTurnPage, machine_menu.previous);
      lv_obj_align(labelTurnPage, buttonTurnPage, LV_ALIGN_CENTER, 0, 0);
      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[Y_AXIS]);
      lv_label_set_text(labelYValue, public_buf_l);
      lv_obj_align(labelYValue, buttonYValue, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[Z_AXIS]);
      lv_label_set_text(labelZValue, public_buf_l);
      lv_obj_align(labelZValue, buttonZValue, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[E_AXIS]);
      lv_label_set_text(labelE0Value, public_buf_l);
      lv_obj_align(labelE0Value, buttonE0Value, LV_ALIGN_CENTER, 0, 0);

      ZERO(public_buf_l);
      sprintf_P(public_buf_l, PSTR("%d"), (int)planner.settings.max_acceleration_mm_per_s2[E_AXIS_N(1)]);
      lv_label_set_text(labelE1Value, public_buf_l);
      lv_obj_align(labelE1Value, buttonE1Value, LV_ALIGN_CENTER, 0, 0);
    }

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);
  }
}

void lv_clear_acceleration_settings() { lv_obj_del(scr); }

#endif // HAS_TFT_LVGL_UI
