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
static lv_obj_t *scr, *wifi_name_text, *wifi_key_text, *wifi_state_text, *wifi_ip_text;

enum {
  ID_W_RETURN = 1,
  ID_W_CLOUD,
  ID_W_RECONNECT
};

static void event_handler(lv_obj_t *obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  clear_cur_ui();
  switch (obj->mks_obj_id) {
    case ID_W_RETURN:
      lv_draw_set();
      break;
    case ID_W_CLOUD:
      lv_draw_cloud_bind();
      break;
    #if ENABLED(MKS_WIFI_MODULE)
      case ID_W_RECONNECT: {
        uint8_t cmd_wifi_list[] = { 0xA5, 0x07, 0x00, 0x00, 0xFC };
        raw_send_to_wifi(cmd_wifi_list, COUNT(cmd_wifi_list));
        lv_draw_wifi_list();
      } break;
    #endif
  }
}

void lv_draw_wifi() {
  scr = lv_screen_create(WIFI_UI);

  lv_obj_t *buttonReconnect = nullptr, *label_Reconnect = nullptr;
  lv_obj_t *buttonCloud = nullptr, *label_Cloud = nullptr;

  const bool enc_ena = TERN0(HAS_ROTARY_ENCODER, gCfgItems.encoder_enable);

  if (gCfgItems.wifi_mode_sel == STA_MODEL) {

    if (gCfgItems.cloud_enable)
      buttonCloud = lv_imgbtn_create(scr, "F:/bmp_cloud.bin", BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_W_CLOUD);

    buttonReconnect = lv_imgbtn_create(scr, "F:/bmp_wifi.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_W_RECONNECT);

    #if HAS_ROTARY_ENCODER
      if (gCfgItems.cloud_enable) lv_group_add_obj(g, buttonCloud);
      if (enc_ena) lv_group_add_obj(g, buttonReconnect);
    #endif

    label_Reconnect = lv_label_create_empty(buttonReconnect);
    if (gCfgItems.cloud_enable) label_Cloud = lv_label_create_empty(buttonCloud);
  }

  // Create an Image button
  lv_obj_t *buttonBack = lv_imgbtn_create(scr, "F:/bmp_return.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_W_RETURN);
  if (enc_ena) lv_group_add_obj(g, buttonBack);
  lv_obj_t *label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
    if (gCfgItems.wifi_mode_sel == STA_MODEL) {
      if (gCfgItems.cloud_enable) {
        lv_label_set_text(label_Cloud, wifi_menu.cloud);
        lv_obj_align(label_Cloud, buttonCloud, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
      }
      lv_label_set_text(label_Reconnect, wifi_menu.reconnect);
      lv_obj_align(label_Reconnect, buttonReconnect, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }

  wifi_ip_text = lv_label_create_empty(scr);
  lv_obj_set_style(wifi_ip_text, &tft_style_label_rel);
  wifi_name_text = lv_label_create_empty(scr);
  lv_obj_set_style(wifi_name_text, &tft_style_label_rel);
  wifi_key_text = lv_label_create_empty(scr);
  lv_obj_set_style(wifi_key_text, &tft_style_label_rel);
  wifi_state_text = lv_label_create_empty(scr);
  lv_obj_set_style(wifi_state_text, &tft_style_label_rel);

  disp_wifi_state();
}

void disp_wifi_state() {
  strcpy(public_buf_m, wifi_menu.ip);
  strcat(public_buf_m, ipPara.ip_addr);
  lv_label_set_text(wifi_ip_text, public_buf_m);
  lv_obj_align(wifi_ip_text, nullptr, LV_ALIGN_CENTER, 0, -100);

  strcpy(public_buf_m, wifi_menu.wifi);
  strcat(public_buf_m, wifiPara.ap_name);
  lv_label_set_text(wifi_name_text, public_buf_m);
  lv_obj_align(wifi_name_text, nullptr, LV_ALIGN_CENTER, 0, -70);

  if (wifiPara.mode == AP_MODEL) {
    strcpy(public_buf_m, wifi_menu.key);
    strcat(public_buf_m, wifiPara.keyCode);
    lv_label_set_text(wifi_key_text, public_buf_m);
    lv_obj_align(wifi_key_text, nullptr, LV_ALIGN_CENTER, 0, -40);

    strcpy(public_buf_m, wifi_menu.state_ap);
    if (wifi_link_state == WIFI_CONNECTED)
      strcat(public_buf_m, wifi_menu.connected);
    else if (wifi_link_state == WIFI_NOT_CONFIG)
      strcat(public_buf_m, wifi_menu.disconnected);
    else
      strcat(public_buf_m, wifi_menu.exception);
    lv_label_set_text(wifi_state_text, public_buf_m);
    lv_obj_align(wifi_state_text, nullptr, LV_ALIGN_CENTER, 0, -10);
  }
  else {
    strcpy(public_buf_m, wifi_menu.state_sta);
    if (wifi_link_state == WIFI_CONNECTED)
      strcat(public_buf_m, wifi_menu.connected);
    else if (wifi_link_state == WIFI_NOT_CONFIG)
      strcat(public_buf_m, wifi_menu.disconnected);
    else
      strcat(public_buf_m, wifi_menu.exception);
    lv_label_set_text(wifi_state_text, public_buf_m);
    lv_obj_align(wifi_state_text, nullptr, LV_ALIGN_CENTER, 0, -40);

    lv_label_set_text(wifi_key_text, "");
    lv_obj_align(wifi_key_text, nullptr, LV_ALIGN_CENTER, 0, -10);
  }
}

void lv_clear_wifi() {
  if (TERN0(HAS_ROTARY_ENCODER, gCfgItems.encoder_enable))
    lv_group_remove_all_objs(g);
  lv_obj_del(scr);
}

#endif // MKS_WIFI_MODULE
#endif // HAS_TFT_LVGL_UI
