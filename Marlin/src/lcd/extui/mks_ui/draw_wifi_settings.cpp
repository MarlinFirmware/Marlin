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

#include "../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include <lv_conf.h>
#include "tft_lvgl_configuration.h"

#if ENABLED(MKS_WIFI_MODULE)

#include "draw_ui.h"

extern lv_group_t *g;
static lv_obj_t *scr, *labelModelValue = nullptr, *buttonModelValue = nullptr, *labelCloudValue = nullptr;

enum {
  ID_WIFI_RETURN = 1,
  ID_WIFI_MODEL,
  ID_WIFI_NAME,
  ID_WIFI_PASSWORD,
  ID_WIFI_CLOUD,
  ID_WIFI_CONFIG
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_WIFI_RETURN:
      lv_clear_wifi_settings();
      draw_return_ui();
      break;
    case ID_WIFI_MODEL:
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
      break;
    case ID_WIFI_NAME:
      keyboard_value = wifiName;
      lv_clear_wifi_settings();
      lv_draw_keyboard();
      break;
    case ID_WIFI_PASSWORD:
      keyboard_value = wifiPassWord;
      lv_clear_wifi_settings();
      lv_draw_keyboard();
      break;
    case ID_WIFI_CLOUD:
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
      break;
    case ID_WIFI_CONFIG:
      lv_clear_wifi_settings();
      lv_draw_dialog(DIALOG_WIFI_CONFIG_TIPS);
      break;
  }
}

void lv_draw_wifi_settings() {
  scr = lv_screen_create(WIFI_SETTINGS_UI, machine_menu.WifiConfTitle);

  lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y + 10, machine_menu.wifiMode);

  lv_obj_t *buttonModelValue = lv_imgbtn_create(scr, "F:/bmp_blank_sel.bin", PARA_UI_VALUE_POS_X, PARA_UI_POS_Y + PARA_UI_VALUE_V, event_handler, ID_WIFI_MODEL);
  lv_btn_set_style_both(buttonModelValue, &style_para_value_pre);
  labelModelValue = lv_label_create_empty(buttonModelValue);

  lv_obj_t *line1 = lv_line_create(scr, nullptr);
  lv_ex_line(line1, line_points[0]);

  lv_obj_t *labelNameText = lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 2 + 10, nullptr);
  lv_obj_t *buttonNameValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 2 + PARA_UI_VALUE_V, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_WIFI_NAME);
  lv_obj_t *labelNameValue = lv_label_create_empty(buttonNameValue);

  lv_obj_t *line2 = lv_line_create(scr, nullptr);
  lv_ex_line(line2, line_points[1]);

  lv_obj_t *labelPassWordText = lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 3 + 10, nullptr);
  lv_obj_t *buttonPassWordValue = lv_btn_create(scr, PARA_UI_VALUE_POS_X, PARA_UI_POS_Y * 3 + PARA_UI_VALUE_V, PARA_UI_VALUE_BTN_X_SIZE, PARA_UI_VALUE_BTN_Y_SIZE, event_handler, ID_WIFI_PASSWORD);
  lv_obj_t *labelPassWordValue = lv_label_create_empty(buttonPassWordValue);

  lv_obj_t *line3 = lv_line_create(scr, nullptr);
  lv_ex_line(line3, line_points[2]);

  lv_label_create(scr, PARA_UI_POS_X, PARA_UI_POS_Y * 4 + 10, machine_menu.wifiCloud);
  lv_obj_t *buttonCloudValue = lv_imgbtn_create(scr, gCfgItems.cloud_enable ? "F:/bmp_enable.bin" : "F:/bmp_disable.bin", PARA_UI_STATE_POS_X, PARA_UI_POS_Y * 4 + PARA_UI_STATE_V, event_handler, ID_WIFI_CLOUD);
  labelCloudValue = lv_label_create_empty(buttonCloudValue);

  lv_obj_t *line4 = lv_line_create(scr, nullptr);
  lv_ex_line(line4, line_points[3]);

  lv_obj_t *buttonConfig = lv_imgbtn_create(scr, "F:/bmp_back70x40.bin", PARA_UI_TURN_PAGE_POS_X, PARA_UI_TURN_PAGE_POS_Y, event_handler, ID_WIFI_CONFIG);
  lv_obj_t *labelConfig = lv_label_create_empty(buttonConfig);

  lv_obj_t *buttonBack = lv_imgbtn_create(scr, "F:/bmp_back70x40.bin", PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, event_handler, ID_WIFI_RETURN);
  lv_obj_t *label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
    if (gCfgItems.wifi_mode_sel == AP_MODEL) {
      lv_label_set_text(labelModelValue, WIFI_AP_TEXT);
      lv_obj_align(labelModelValue, buttonModelValue, LV_ALIGN_CENTER, 0, 0);
    }
    else {
      lv_label_set_text(labelModelValue, WIFI_STA_TEXT);
      lv_obj_align(labelModelValue, buttonModelValue, LV_ALIGN_CENTER, 0, 0);
    }
    strcpy(public_buf_m, machine_menu.wifiName);
    strcat(public_buf_m, (const char *)uiCfg.wifi_name);
    lv_label_set_text(labelNameText, public_buf_m);

    lv_label_set_text(labelNameValue, machine_menu.wifiEdit);
    lv_obj_align(labelNameValue, buttonNameValue, LV_ALIGN_CENTER, 0, 0);

    strcpy(public_buf_m, machine_menu.wifiPassWord);
    strcat(public_buf_m, (const char *)uiCfg.wifi_key);
    lv_label_set_text(labelPassWordText, public_buf_m);

    lv_label_set_text(labelPassWordValue, machine_menu.wifiEdit);
    lv_obj_align(labelPassWordValue, buttonPassWordValue, LV_ALIGN_CENTER, 0, 0);

    lv_label_set_text(labelCloudValue, gCfgItems.cloud_enable ? machine_menu.enable : machine_menu.disable);
    lv_obj_align(labelCloudValue, buttonCloudValue, LV_ALIGN_CENTER, 0, 0);

    lv_label_set_text(labelConfig, machine_menu.wifiConfig);
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

#endif // MKS_WIFI_MODULE
#endif // HAS_TFT_LVGL_UI
