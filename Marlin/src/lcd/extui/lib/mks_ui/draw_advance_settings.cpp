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

#define ID_ADVANCE_RETURN           1
#define ID_PAUSE_POS                2
#define ID_PAUSE_POS_ARROW          3
#define ID_WIFI_PARA                4
#define ID_WIFI_PARA_ARROW          5
#define ID_FILAMENT_SETTINGS        6
#define ID_FILAMENT_SETTINGS_ARROW  7
#define ID_ENCODER_SETTINGS         8
#define ID_ENCODER_SETTINGS_ARROW   9

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
    case ID_FILAMENT_SETTINGS:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_advance_settings();
        lv_draw_filament_settings();
      }
      break;
    case ID_FILAMENT_SETTINGS_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_advance_settings();
        lv_draw_filament_settings();
      }
      break;
    #if ENABLED(USE_WIFI_FUNCTION)
      case ID_WIFI_PARA:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_advance_settings();
        lv_draw_wifi_settings();
      }
      break;
      case ID_WIFI_PARA_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_advance_settings();
        lv_draw_wifi_settings();
      }
      break;
    #endif
    #if HAS_ROTARY_ENCODER
      case ID_ENCODER_SETTINGS:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          lv_clear_advance_settings();
          lv_draw_encoder_settings();
        }
        break;
      case ID_ENCODER_SETTINGS_ARROW:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          lv_clear_advance_settings();
          lv_draw_encoder_settings();
        }
        break;
    #endif
  }
}

void lv_draw_advance_settings(void) {
  lv_obj_t *buttonBack, *label_Back;
  lv_obj_t *buttonPausePos, *labelPausePos, *buttonPausePosNarrow;
  lv_obj_t *buttonFilamentSettings, *labelFilamentSettings, *buttonFilamentSettingsNarrow;
  lv_obj_t * line1,* line2;
  #if ENABLED(USE_WIFI_FUNCTION)
    lv_obj_t *buttonWifiSet,*labelWifiSet,*buttonWifiSetNarrow;
  #endif
  #if HAS_ROTARY_ENCODER
    lv_obj_t *buttonEcoder,*labelEcoder,*buttonEcoderNarrow;
  #endif

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

  buttonPausePos = lv_btn_create(scr, NULL);
  lv_obj_set_pos(buttonPausePos, PARA_UI_POS_X, PARA_UI_POS_Y);
  lv_obj_set_size(buttonPausePos, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);
  //lv_obj_set_event_cb(buttonMachine, event_handler);
  lv_obj_set_event_cb_mks(buttonPausePos, event_handler, ID_PAUSE_POS, NULL, 0);
  lv_btn_set_style(buttonPausePos, LV_BTN_STYLE_REL, &tft_style_label_rel);
  lv_btn_set_style(buttonPausePos, LV_BTN_STYLE_PR, &tft_style_label_pre);
  lv_btn_set_layout(buttonPausePos, LV_LAYOUT_OFF);
  labelPausePos = lv_label_create(buttonPausePos, NULL);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonPausePos);
  #endif

  buttonPausePosNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonPausePosNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonPausePosNarrow, event_handler, ID_PAUSE_POS_ARROW, NULL, 0);
  lv_imgbtn_set_src(buttonPausePosNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
  lv_imgbtn_set_src(buttonPausePosNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
  lv_imgbtn_set_style(buttonPausePosNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonPausePosNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonPausePosNarrow, LV_LAYOUT_OFF);

  line1 = lv_line_create(lv_scr_act(), NULL);
  lv_ex_line(line1, line_points[0]);

  buttonFilamentSettings = lv_btn_create(scr, NULL);
  lv_obj_set_pos(buttonFilamentSettings, PARA_UI_POS_X, PARA_UI_POS_Y*2);
  lv_obj_set_size(buttonFilamentSettings, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);
  lv_obj_set_event_cb_mks(buttonFilamentSettings, event_handler, ID_FILAMENT_SETTINGS, NULL, 0);
  lv_btn_set_style(buttonFilamentSettings, LV_BTN_STYLE_REL, &tft_style_label_rel);
  lv_btn_set_style(buttonFilamentSettings, LV_BTN_STYLE_PR, &tft_style_label_pre);
  lv_btn_set_layout(buttonFilamentSettings, LV_LAYOUT_OFF);
  labelFilamentSettings = lv_label_create(buttonFilamentSettings, NULL);
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonFilamentSettings);
  #endif

  buttonFilamentSettingsNarrow = lv_imgbtn_create(scr, NULL);
  lv_obj_set_pos(buttonFilamentSettingsNarrow, PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y*2 + PARA_UI_ARROW_V);
  lv_obj_set_event_cb_mks(buttonFilamentSettingsNarrow, event_handler, ID_FILAMENT_SETTINGS_ARROW, NULL, 0);
  lv_imgbtn_set_src(buttonFilamentSettingsNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
  lv_imgbtn_set_src(buttonFilamentSettingsNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
  lv_imgbtn_set_style(buttonFilamentSettingsNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonFilamentSettingsNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_btn_set_layout(buttonFilamentSettingsNarrow, LV_LAYOUT_OFF);

  line2 = lv_line_create(lv_scr_act(), NULL);
  lv_ex_line(line2, line_points[1]);

  #if ENABLED(USE_WIFI_FUNCTION)

    buttonWifiSet = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
    lv_obj_set_pos(buttonWifiSet, PARA_UI_POS_X,PARA_UI_POS_Y*3);
    lv_obj_set_size(buttonWifiSet, PARA_UI_SIZE_X,PARA_UI_SIZE_Y);
    lv_obj_set_event_cb_mks(buttonWifiSet, event_handler,ID_WIFI_PARA,NULL,0);
    lv_btn_set_style(buttonWifiSet, LV_BTN_STYLE_REL, &tft_style_label_rel);
    lv_btn_set_style(buttonWifiSet, LV_BTN_STYLE_PR, &tft_style_label_pre);
    lv_btn_set_layout(buttonWifiSet, LV_LAYOUT_OFF);
    labelWifiSet = lv_label_create(buttonWifiSet, NULL);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonWifiSet);
    #endif

    buttonWifiSetNarrow = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonWifiSetNarrow,PARA_UI_POS_X+PARA_UI_SIZE_X,PARA_UI_POS_Y*3+PARA_UI_ARROW_V);
    lv_obj_set_event_cb_mks(buttonWifiSetNarrow, event_handler,ID_WIFI_PARA_ARROW, NULL,0);
    lv_imgbtn_set_src(buttonWifiSetNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
    lv_imgbtn_set_src(buttonWifiSetNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
    lv_imgbtn_set_style(buttonWifiSetNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonWifiSetNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_btn_set_layout(buttonWifiSetNarrow, LV_LAYOUT_OFF);

    lv_obj_t * line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3,line_points[2]);

    #if HAS_ROTARY_ENCODER
      buttonEcoder = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
      lv_obj_set_pos(buttonEcoder, PARA_UI_POS_X,PARA_UI_POS_Y*4);
      lv_obj_set_size(buttonEcoder, PARA_UI_SIZE_X,PARA_UI_SIZE_Y);
      lv_obj_set_event_cb_mks(buttonEcoder, event_handler,ID_ENCODER_SETTINGS,NULL,0);
      lv_btn_set_style(buttonEcoder, LV_BTN_STYLE_REL, &tft_style_label_rel);
      lv_btn_set_style(buttonEcoder, LV_BTN_STYLE_PR, &tft_style_label_pre);
      lv_btn_set_layout(buttonEcoder, LV_LAYOUT_OFF);
      labelEcoder = lv_label_create(buttonEcoder, NULL);

      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonEcoder);

      buttonEcoderNarrow = lv_imgbtn_create(scr, NULL);
      lv_obj_set_pos(buttonEcoderNarrow,PARA_UI_POS_X+PARA_UI_SIZE_X,PARA_UI_POS_Y*4+PARA_UI_ARROW_V);
      lv_obj_set_event_cb_mks(buttonEcoderNarrow, event_handler,ID_ENCODER_SETTINGS_ARROW, NULL,0);
      lv_imgbtn_set_src(buttonEcoderNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
      lv_imgbtn_set_src(buttonEcoderNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
      lv_imgbtn_set_style(buttonEcoderNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
      lv_imgbtn_set_style(buttonEcoderNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
      lv_btn_set_layout(buttonEcoderNarrow, LV_LAYOUT_OFF);

      lv_obj_t * line4 = lv_line_create(scr, NULL);
      lv_ex_line(line4,line_points[3]);
    #endif

  #elif HAS_ROTARY_ENCODER
    buttonEcoder = lv_btn_create(scr, NULL);     /*Add a button the current screen*/
    lv_obj_set_pos(buttonEcoder, PARA_UI_POS_X,PARA_UI_POS_Y*3);
    lv_obj_set_size(buttonEcoder, PARA_UI_SIZE_X,PARA_UI_SIZE_Y);
    lv_obj_set_event_cb_mks(buttonEcoder, event_handler,ID_ENCODER_SETTINGS,NULL,0);
    lv_btn_set_style(buttonEcoder, LV_BTN_STYLE_REL, &tft_style_label_rel);
    lv_btn_set_style(buttonEcoder, LV_BTN_STYLE_PR, &tft_style_label_pre);
    lv_btn_set_layout(buttonEcoder, LV_LAYOUT_OFF);
    labelEcoder = lv_label_create(buttonEcoder, NULL);

    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonEcoder);

    buttonEcoderNarrow = lv_imgbtn_create(scr, NULL);
    lv_obj_set_pos(buttonEcoderNarrow,PARA_UI_POS_X+PARA_UI_SIZE_X,PARA_UI_POS_Y*3+PARA_UI_ARROW_V);
    lv_obj_set_event_cb_mks(buttonEcoderNarrow, event_handler,ID_ENCODER_SETTINGS_ARROW, NULL,0);
    lv_imgbtn_set_src(buttonEcoderNarrow, LV_BTN_STATE_REL, "F:/bmp_arrow.bin");
    lv_imgbtn_set_src(buttonEcoderNarrow, LV_BTN_STATE_PR, "F:/bmp_arrow.bin");
    lv_imgbtn_set_style(buttonEcoderNarrow, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonEcoderNarrow, LV_BTN_STATE_REL, &tft_style_label_rel);
    lv_btn_set_layout(buttonEcoderNarrow, LV_LAYOUT_OFF);

    lv_obj_t * line3 = lv_line_create(scr, NULL);
    lv_ex_line(line3,line_points[2]);
  #endif

  buttonBack = lv_imgbtn_create(scr, NULL);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_ADVANCE_RETURN, NULL, 0);
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

    lv_label_set_text(labelPausePos, machine_menu.PausePosition);
    lv_obj_align(labelPausePos, buttonPausePos, LV_ALIGN_IN_LEFT_MID, 0, 0);

    lv_label_set_text(labelFilamentSettings, machine_menu.FilamentConf);
    lv_obj_align(labelFilamentSettings, buttonFilamentSettings, LV_ALIGN_IN_LEFT_MID, 0, 0);

    #if ENABLED(USE_WIFI_FUNCTION)
      lv_label_set_text(labelWifiSet, machine_menu.WifiSettings);
      lv_obj_align(labelWifiSet, buttonWifiSet, LV_ALIGN_IN_LEFT_MID,0, 0);
    #endif
    #if HAS_ROTARY_ENCODER
      lv_label_set_text(labelEcoder, machine_menu.EncoderSettings);
      lv_obj_align(labelEcoder, buttonEcoder, LV_ALIGN_IN_LEFT_MID,0, 0);
    #endif
  }

}

void lv_clear_advance_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
