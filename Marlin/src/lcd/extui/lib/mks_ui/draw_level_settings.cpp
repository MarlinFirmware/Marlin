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

#define ID_LEVEL_RETURN           1
#define ID_LEVEL_POSITION         2
#define ID_LEVEL_POSITION_ARROW   3
#define ID_LEVEL_COMMAND          4
#define ID_LEVEL_COMMAND_ARROW    5
#define ID_LEVEL_ZOFFSET          6
#define ID_LEVEL_ZOFFSET_ARROW    7


static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_LEVEL_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_level_settings();
        draw_return_ui();
      }
      break;
    case ID_LEVEL_POSITION:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_level_settings();
        lv_draw_manual_level_pos_settings();
      }
      break;
    case ID_LEVEL_POSITION_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_level_settings();
        lv_draw_manual_level_pos_settings();
      }
      break;
    case ID_LEVEL_COMMAND:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        keyboard_value = gcodeCommand;
        lv_clear_level_settings();
        lv_draw_keyboard();
      }
      break;
    case ID_LEVEL_COMMAND_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        keyboard_value = gcodeCommand;
        lv_clear_level_settings();
        lv_draw_keyboard();
      }
      break;
    #if HAS_BED_PROBE
      case ID_LEVEL_ZOFFSET:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          lv_clear_level_settings();
          lv_draw_auto_level_offset_settings();
        }
        break;
      case ID_LEVEL_ZOFFSET_ARROW:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          lv_clear_level_settings();
          lv_draw_auto_level_offset_settings();
        }
        break;
    #endif
  }
}

void lv_draw_level_settings(void) {
  lv_obj_t *buttonBack, *label_Back;
  lv_obj_t *buttonPosition, *labelPosition, *buttonPositionNarrow;
  lv_obj_t *buttonCommand, *labelCommand, *buttonCommandNarrow;
  #if HAS_BED_PROBE
    lv_obj_t *buttonZoffset, *labelZoffset, *buttonZoffsetNarrow;
    lv_obj_t * line3;
  #endif
  lv_obj_t * line1, * line2;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != LEVELING_PARA_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = LEVELING_PARA_UI;
  }
  disp_state = LEVELING_PARA_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, machine_menu.LevelingParaConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());


  buttonPosition = lv_btn_create(scr, NULL);                                   /*Add a button the current screen*/
  lv_obj_set_pos(buttonPosition, PARA_UI_POS_X, PARA_UI_POS_Y);                /*Set its position*/
  lv_obj_set_size(buttonPosition, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);             /*Set its size*/
  lv_obj_set_event_cb_mks(buttonPosition, event_handler, ID_LEVEL_POSITION, NULL, 0);
  lv_btn_set_style(buttonPosition, LV_BTN_STYLE_REL, &tft_style_label_rel);    /*Set the button's released style*/
  lv_btn_set_style(buttonPosition, LV_BTN_STYLE_PR, &tft_style_label_pre);     /*Set the button's pressed style*/
  lv_btn_set_layout(buttonPosition, LV_LAYOUT_OFF);
  labelPosition = lv_label_create(buttonPosition, NULL);                       /*Add a label to the button*/

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonPosition);
  #endif

  buttonPositionNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonPositionNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonPositionNarrow, event_handler, ID_LEVEL_POSITION_ARROW, NULL, 0);
  lv_imgbtn_set_src(buttonPositionNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
  lv_imgbtn_set_src(buttonPositionNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
  lv_imgbtn_set_style(buttonPositionNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonPositionNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonPositionNarrow, LV_LAYOUT_OFF);

  line1 = lv_line_create(scr, NULL);
  lv_ex_line(line1, line_points[0]);

  buttonCommand = lv_btn_create(scr, NULL);
  lv_obj_set_pos(buttonCommand, PARA_UI_POS_X, PARA_UI_POS_Y * 2);
  lv_obj_set_size(buttonCommand, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);
  lv_obj_set_event_cb_mks(buttonCommand, event_handler, ID_LEVEL_COMMAND, NULL, 0);
  lv_btn_set_style(buttonCommand, LV_BTN_STYLE_REL, &tft_style_label_rel);
  lv_btn_set_style(buttonCommand, LV_BTN_STYLE_PR, &tft_style_label_pre);
  lv_btn_set_layout(buttonCommand, LV_LAYOUT_OFF);
  labelCommand = lv_label_create(buttonCommand, NULL);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonCommand);
  #endif

  buttonCommandNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonCommandNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 2 + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonCommandNarrow, event_handler, ID_LEVEL_COMMAND_ARROW, NULL, 0);
  lv_imgbtn_set_src(buttonCommandNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
  lv_imgbtn_set_src(buttonCommandNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
  lv_imgbtn_set_style(buttonCommandNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonCommandNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonCommandNarrow, LV_LAYOUT_OFF);

  line2 = lv_line_create(scr, NULL);
  lv_ex_line(line2, line_points[1]);

  #if HAS_BED_PROBE

    buttonZoffset = lv_btn_create(scr, NULL);                                 /*Add a button the current screen*/
    lv_obj_set_pos(buttonZoffset, PARA_UI_POS_X, PARA_UI_POS_Y * 3);          /*Set its position*/
    lv_obj_set_size(buttonZoffset, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);           /*Set its size*/
    lv_obj_set_event_cb_mks(buttonZoffset, event_handler, ID_LEVEL_ZOFFSET, NULL, 0);
    lv_btn_set_style(buttonZoffset, LV_BTN_STYLE_REL, &tft_style_label_rel);  /*Set the button's released style*/
    lv_btn_set_style(buttonZoffset, LV_BTN_STYLE_PR, &tft_style_label_pre);   /*Set the button's pressed style*/
    lv_btn_set_layout(buttonZoffset, LV_LAYOUT_OFF);
    labelZoffset = lv_label_create(buttonZoffset, NULL);                      /*Add a label to the button*/

    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonZoffset);
    #endif

    buttonZoffsetNarrow = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonZoffsetNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 3 + PARA_UI_ARROW_V);
    lv_obj_set_event_cb_mks(buttonZoffsetNarrow, event_handler, ID_LEVEL_ZOFFSET_ARROW, NULL, 0);
    lv_imgbtn_set_src(buttonZoffsetNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
    lv_imgbtn_set_src(buttonZoffsetNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
    lv_imgbtn_set_style(buttonZoffsetNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonZoffsetNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_btn_set_layout(buttonZoffsetNarrow, LV_LAYOUT_OFF);

    line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3, line_points[2]);

  #endif // HAS_BED_PROBE

  buttonBack = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_LEVEL_RETURN, NULL, 0);
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_back70x40.bin");
  lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_back70x40.bin");
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonBack);
  #endif

  lv_obj_set_pos(buttonBack, PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);
  label_Back = lv_label_create(buttonBack, NULL);

  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);

    lv_label_set_text(labelPosition, machine_menu.LevelingManuPosConf);
    lv_obj_align(labelPosition, buttonPosition, LV_ALIGN_IN_LEFT_MID, 0, 0);

    lv_label_set_text(labelCommand, machine_menu.LevelingAutoCommandConf);
    lv_obj_align(labelCommand, buttonCommand, LV_ALIGN_IN_LEFT_MID, 0, 0);
    #if HAS_BED_PROBE
      lv_label_set_text(labelZoffset, machine_menu.LevelingAutoZoffsetConf);
      lv_obj_align(labelZoffset, buttonZoffset, LV_ALIGN_IN_LEFT_MID, 0, 0);
    #endif
  }

}

void lv_clear_level_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
