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
static lv_obj_t *scr, *wifi_name_text, *wifi_key_text, *wifi_state_text, *wifi_ip_text;

#define ID_W_RETURN     1
#define ID_W_CLOUD      2
#define ID_W_RECONNECT  3

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_W_RETURN:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        clear_cur_ui();
        lv_draw_set();
      }
    break;
    case ID_W_CLOUD:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        //clear_cur_ui();
        //draw_return_ui();
      }
    break;
    case ID_W_RECONNECT:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        clear_cur_ui();
        lv_draw_wifi_list();
      }
    break;
  }
}

void lv_draw_wifi(void) {
  lv_obj_t *buttonBack = NULL, *label_Back = NULL;
  lv_obj_t *buttonCloud = NULL, *label_Cloud = NULL;
  lv_obj_t *buttonReconnect = NULL, *label_Reconnect=NULL;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != WIFI_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = WIFI_UI;
  }
  disp_state = WIFI_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  // Create an Image button
  buttonBack = lv_imgbtn_create(scr, "F:/bmp_return.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_W_RETURN);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonBack);
  #endif

  if (gCfgItems.wifi_mode_sel == STA_MODEL) {
    //buttonCloud = lv_imgbtn_create(scr, NULL);
    buttonReconnect = lv_imgbtn_create(scr, NULL);
  }

  if (gCfgItems.wifi_mode_sel == STA_MODEL) {

    lv_obj_set_event_cb_mks(buttonReconnect, event_handler,ID_W_RECONNECT, NULL, 0);
    lv_imgbtn_set_src_both(buttonReconnect, "F:/bmp_wifi.bin");
    lv_imgbtn_use_label_style(buttonReconnect);

    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonReconnect);
    #endif

    lv_obj_set_pos(buttonReconnect, BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
    lv_btn_set_layout(buttonReconnect, LV_LAYOUT_OFF);
  }

  label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.wifi_mode_sel == STA_MODEL) {
    //label_Cloud = lv_label_create_empty(buttonCloud);
    label_Reconnect = lv_label_create_empty(buttonReconnect);
  }

  if (gCfgItems.multiple_language) {
    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    if (gCfgItems.wifi_mode_sel == STA_MODEL) {
      //lv_label_set_text(label_Cloud, common_menu.text_back);
      //lv_obj_align(label_Cloud, buttonCloud, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Reconnect, wifi_menu.reconnect);
      lv_obj_align(label_Reconnect, buttonReconnect, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
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
  memset(public_buf_m, 0, sizeof(public_buf_m));
  strcpy(public_buf_m,wifi_menu.ip);
  strcat(public_buf_m,ipPara.ip_addr);
  lv_label_set_text(wifi_ip_text, public_buf_m);
  lv_obj_align(wifi_ip_text, NULL, LV_ALIGN_CENTER, 0, -100);

  memset(public_buf_m, 0, sizeof(public_buf_m));
  strcpy(public_buf_m,wifi_menu.wifi);
  strcat(public_buf_m,wifiPara.ap_name);
  lv_label_set_text(wifi_name_text, public_buf_m);
  lv_obj_align(wifi_name_text, NULL, LV_ALIGN_CENTER, 0, -70);

  if (wifiPara.mode == AP_MODEL) {
    memset(public_buf_m, 0, sizeof(public_buf_m));
    strcpy(public_buf_m,wifi_menu.key);
    strcat(public_buf_m,wifiPara.keyCode);
    lv_label_set_text(wifi_key_text, public_buf_m);
    lv_obj_align(wifi_key_text, NULL, LV_ALIGN_CENTER, 0, -40);

    memset(public_buf_m, 0, sizeof(public_buf_m));
    strcpy(public_buf_m,wifi_menu.state_ap);
    if (wifi_link_state == WIFI_CONNECTED)
      strcat(public_buf_m,wifi_menu.connected);
    else if (wifi_link_state == WIFI_NOT_CONFIG)
      strcat(public_buf_m,wifi_menu.disconnected);
    else
      strcat(public_buf_m,wifi_menu.exception);
    lv_label_set_text(wifi_state_text, public_buf_m);
    lv_obj_align(wifi_state_text, NULL, LV_ALIGN_CENTER, 0, -10);
  }
  else {
    ZERO(public_buf_m);
    strcpy(public_buf_m, wifi_menu.state_sta);
    if (wifi_link_state == WIFI_CONNECTED)
      strcat(public_buf_m, wifi_menu.connected);
    else if (wifi_link_state == WIFI_NOT_CONFIG)
      strcat(public_buf_m, wifi_menu.disconnected);
    else
      strcat(public_buf_m, wifi_menu.exception);
    lv_label_set_text(wifi_state_text, public_buf_m);
    lv_obj_align(wifi_state_text, NULL, LV_ALIGN_CENTER, 0, -40);

    lv_label_set_text(wifi_key_text, "");
    lv_obj_align(wifi_key_text, NULL, LV_ALIGN_CENTER, 0, -10);
  }
}

void lv_clear_wifi() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // USE_WIFI_FUNCTION
#endif // HAS_TFT_LVGL_UI
