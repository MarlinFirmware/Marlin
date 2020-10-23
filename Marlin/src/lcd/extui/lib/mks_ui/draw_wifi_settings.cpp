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

#include <lv_conf.h>
#include "tft_lvgl_configuration.h"

#if ENABLED(USE_WIFI_FUNCTION)

#include "draw_ui.h"

extern lv_group_t * g;
static lv_obj_t *scr, *labelModelValue = NULL, *buttonModelValue = NULL, *labelCloudValue = NULL;

#define ID_WIFI_RETURN    1
#define ID_WIFI_MODEL     2
#define ID_WIFI_NAME      3
#define ID_WIFI_PASSWORD  4
#define ID_WIFI_CLOUD     5
#define ID_WIFI_CONFIG    6

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_WIFI_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_wifi_settings();
        draw_return_ui();
      }
    break;
    case ID_WIFI_MODEL:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        if (gCfgItems.wifi_mode_sel == AP_MODEL) {
          gCfgItems.wifi_mode_sel = STA_MODEL;
          lv_label_set_text(labelModelValue, WIFI_STA_TEXT);
          lv_obj_align(labelModelValue, buttonModelValue, LV_ALIGN_CENTER, 0, 0);
          update_spi_flash();
        }
        else {
          gCfgItems.wifi_mode_sel = AP_MODEL;
          lv_label_set_text(labelModelValue, WIFI_AP_TEXT);
          lv_obj_align(labelModelValue, buttonModelValue, LV_ALIGN_CENTER, 0, 0);
          update_spi_flash();
        }
      }
    break;
    case ID_WIFI_NAME:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        keyboard_value = wifiName;
        lv_clear_wifi_settings();
        lv_draw_keyboard();
      }
    break;
    case ID_WIFI_PASSWORD:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
      keyboard_value = wifiPassWord;
        lv_clear_wifi_settings();
        lv_draw_keyboard();
      }
    break;
    case ID_WIFI_CLOUD:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        if (gCfgItems.cloud_enable) {
          gCfgItems.cloud_enable = false;
          lv_obj_set_event_cb_mks(obj, event_handler, ID_WIFI_CLOUD, "bmp_disable.bin", 0);
          lv_label_set_text(labelCloudValue, machine_menu.disable);
        }
        else {
          gCfgItems.cloud_enable = true;
          lv_obj_set_event_cb_mks(obj, event_handler, ID_WIFI_CLOUD, "bmp_enable.bin", 0);
          lv_label_set_text(labelCloudValue, machine_menu.enable);
        }
        update_spi_flash();
      }
    break;
    case ID_WIFI_CONFIG:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_wifi_settings();
        lv_draw_dialog(DIALOG_WIFI_CONFIG_TIPS);
      }
    break;
  }
}

void lv_draw_wifi_settings(void) {
  lv_obj_t *buttonBack = NULL, *label_Back = NULL, *buttonConfig = NULL, *labelConfig = NULL;
  lv_obj_t *labelModelText = NULL;
  lv_obj_t *labelNameText = NULL, *buttonNameValue = NULL, *labelNameValue = NULL;
  lv_obj_t *labelPassWordText = NULL, *buttonPassWordValue = NULL, *labelPassWordValue = NULL;
  lv_obj_t *labelCloudText = NULL, *buttonCloudValue = NULL;
  lv_obj_t * line1 = NULL, *line2 = NULL, *line3 = NULL, *line4 = NULL;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != WIFI_SETTINGS_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = WIFI_SETTINGS_UI;
  }
  disp_state = WIFI_SETTINGS_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, machine_menu.WifiConfTitle);

  lv_refr_now(lv_refr_get_disp_refreshing());

  labelModelText = lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y + 10, machine_menu.wifiMode);

  buttonModelValue = lv_imgbtn_create(scr, NULL);
  lv_imgbtn_set_src_both(buttonModelValue, "F:/bmp_blank_sel.bin");
  lv_obj_set_pos(buttonModelValue, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V);
  lv_obj_set_event_cb_mks(buttonModelValue, event_handler, ID_WIFI_MODEL, NULL, 0);
  lv_btn_set_style_both(buttonModelValue, &style_para_value_pre);
  lv_btn_set_layout(buttonModelValue, LV_LAYOUT_OFF);
  labelModelValue = lv_label_create_empty(buttonModelValue);

  line1 = lv_line_create(scr, NULL);
  lv_ex_line(line1,line_points[0]);

  labelNameText = lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 2 + 10, NULL);

  buttonNameValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_WIFI_NAME);
  labelNameValue = lv_label_create_empty(buttonNameValue);

  line2 = lv_line_create(scr, NULL);
  lv_ex_line(line2,line_points[1]);

  labelPassWordText = lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 3 + 10, NULL);

  buttonPassWordValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_WIFI_PASSWORD);
  labelPassWordValue = lv_label_create_empty(buttonPassWordValue);

  line3 = lv_line_create(scr, NULL);
  lv_ex_line(line3,line_points[2]);

  labelCloudText = lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 4 + 10, machine_menu.wifiCloud);

  buttonCloudValue = lv_imgbtn_create(scr, gCfgItems.cloud_enable ? "F:/bmp_enable.bin" : "F:/bmp_disable.bin", PARA_UI_STATE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_STATE_V, event_handler, ID_WIFI_CLOUD);
  labelCloudValue = lv_label_create_empty(buttonCloudValue);

  line4 = lv_line_create(scr, NULL);
  lv_ex_line(line4,line_points[3]);

  buttonConfig = lv_imgbtn_create(scr, "F:/bmp_back70x40.bin", PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_WIFI_CONFIG);
  labelConfig = lv_label_create_empty(buttonConfig);

  buttonBack = lv_imgbtn_create(scr, "F:/bmp_back70x40.bin", PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, event_handler, ID_WIFI_RETURN);
  label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
    if (gCfgItems.wifi_mode_sel == AP_MODEL) {
      lv_label_set_text(labelModelValue, WIFI_AP_TEXT);
      lv_obj_align(labelModelValue, buttonModelValue, LV_ALIGN_CENTER, 0, 0);
    }
    else {
      lv_label_set_text(labelModelValue, WIFI_STA_TEXT);
      lv_obj_align(labelModelValue, buttonModelValue, LV_ALIGN_CENTER, 0, 0);
    }
    memset(public_buf_m,0,sizeof(public_buf_m));
    strcat(public_buf_m,machine_menu.wifiName);
    strcat(public_buf_m,(const char *)uiCfg.wifi_name);
    lv_label_set_text(labelNameText,public_buf_m);

    lv_label_set_text(labelNameValue,machine_menu.wifiEdit);
    lv_obj_align(labelNameValue, buttonNameValue, LV_ALIGN_CENTER, 0, 0);

    memset(public_buf_m,0,sizeof(public_buf_m));
    strcat(public_buf_m,machine_menu.wifiPassWord);
    strcat(public_buf_m,(const char *)uiCfg.wifi_key);
    lv_label_set_text(labelPassWordText,public_buf_m);

    lv_label_set_text(labelPassWordValue,machine_menu.wifiEdit);
    lv_obj_align(labelPassWordValue, buttonPassWordValue, LV_ALIGN_CENTER, 0, 0);

    lv_label_set_text(labelCloudValue, gCfgItems.cloud_enable ? machine_menu.enable : machine_menu.disable);
    lv_obj_align(labelCloudValue, buttonCloudValue, LV_ALIGN_CENTER, 0, 0);

    lv_label_set_text(labelConfig,machine_menu.wifiConfig);
    lv_obj_align(labelConfig, buttonConfig, LV_ALIGN_CENTER, 0, 0);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);
  }

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonModelValue);
      lv_group_add_obj(g, buttonNameValue);
      lv_group_add_obj(g, buttonPassWordValue);
      lv_group_add_obj(g, buttonCloudValue);
      lv_group_add_obj(g, buttonConfig);
      lv_group_add_obj(g, buttonBack);
    }
  #endif
}

void lv_clear_wifi_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // USE_WIFI_FUNCTION
#endif // HAS_TFT_LVGL_UI
