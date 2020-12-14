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

#if HAS_TFT_LVGL_UI && USE_WIFI_FUNCTION

#include "lv_conf.h"
#include "draw_ui.h"
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"

#include "../../../../MarlinCore.h"
#include "../../../../module/temperature.h"

#include "QR_Encode.h"

extern lv_group_t * g;
static lv_obj_t * scr;
// static lv_obj_t * qr;
static lv_obj_t *button_bind_or_not = NULL, *label_bind_or_not = NULL;
static lv_obj_t *buttonReleaseBind = NULL, *label_ReleaseBind = NULL;
static lv_obj_t * text_id;

static uint8_t unbinding_flag = 0;
static uint8_t id_mark = 0;

#define ID_CLOUD_BIND_RETURN    1
#define ID_CLOUD_BIND_OR_NOT    2
#define ID_CLOUD_RELEASE_BIND   3

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_CLOUD_BIND_RETURN:
      if (event == LV_EVENT_CLICKED) {
        // do nothing
      }
      else if (event == LV_EVENT_RELEASED) {
        // lv_qrcode_delete(qr);
        clear_cur_ui();
        draw_return_ui();
      }
      break;
    case ID_CLOUD_RELEASE_BIND:
      if (event == LV_EVENT_CLICKED) {
        // do nothing
      }
      else if (event == LV_EVENT_RELEASED) {
        if(cloud_para.state == 0x12) {			
            clear_cur_ui();
            lv_draw_dialog(DIALOG_TYPE_UNBIND);
        }
      }
      break;
  }
}

void lv_draw_cloud_bind(void) {
  lv_obj_t *buttonBack=NULL, *label_Back=NULL;
  if(disp_state_stack._disp_state[disp_state_stack._disp_index] != BIND_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = BIND_UI;
  }
  disp_state = BIND_UI;
  
  scr = lv_obj_create(NULL, NULL);
    
  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title,TITLE_XPOS,TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());
  
  lv_refr_now(lv_refr_get_disp_refreshing());

//   buttonBack = lv_imgbtn_create(scr, NULL);
  
  button_bind_or_not = lv_btn_create(scr, NULL);
  lv_obj_set_pos(button_bind_or_not, TFT_WIDTH - 130, TFT_HEIGHT - 80 * 3);
  lv_obj_set_size(button_bind_or_not, PARA_UI_VALUE_BTN_X_SIZE + 15, PARA_UI_VALUE_BTN_Y_SIZE + 15);
  lv_obj_set_event_cb_mks(button_bind_or_not, event_handler, ID_CLOUD_BIND_OR_NOT, NULL, 0);
  lv_btn_set_style(button_bind_or_not, LV_BTN_STYLE_REL, &style_para_value);
  lv_btn_set_style(button_bind_or_not, LV_BTN_STYLE_PR, &style_para_value);
  label_bind_or_not = lv_label_create(button_bind_or_not, NULL);

  buttonReleaseBind = lv_btn_create(scr, NULL);
  lv_obj_set_pos(buttonReleaseBind, TFT_WIDTH - 130, TFT_HEIGHT - 80 * 2);
  lv_obj_set_size(buttonReleaseBind, PARA_UI_VALUE_BTN_X_SIZE + 15, PARA_UI_VALUE_BTN_Y_SIZE + 15);
  lv_obj_set_event_cb_mks(buttonReleaseBind, event_handler, ID_CLOUD_RELEASE_BIND, NULL, 0);
  label_ReleaseBind = lv_label_create(buttonReleaseBind, NULL);
  lv_label_set_text(label_ReleaseBind, cloud_menu.unbind);
  lv_obj_align(label_ReleaseBind, buttonReleaseBind, LV_ALIGN_CENTER, 0, 0);

//   lv_obj_set_event_cb_mks(buttonBack, event_handler,ID_CLOUD_BIND_RETURN, NULL,0);	
//   lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, "F:/bmp_return.bin");
//   lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, "F:/bmp_return.bin");	
//   lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
//   lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);

//   lv_obj_set_pos(buttonBack,BTN_X_PIXEL*3+INTERVAL_V*4,  BTN_Y_PIXEL+INTERVAL_H+titleHeight);
//   lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

//   label_Back = lv_label_create(buttonBack, NULL);

//   if(gCfgItems.multiple_language !=0) {
// 	lv_label_set_text(label_Back, common_menu.text_back);
// 	lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
//   }

  buttonBack = lv_btn_create(scr, NULL);
  lv_obj_set_pos(buttonBack, TFT_WIDTH - 130, TFT_HEIGHT - 80);
  lv_obj_set_size(buttonBack, PARA_UI_VALUE_BTN_X_SIZE + 15, PARA_UI_VALUE_BTN_Y_SIZE + 15);
  lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_CLOUD_BIND_RETURN, NULL, 0);
  lv_btn_set_style(buttonBack, LV_BTN_STYLE_REL, &style_para_back);
  lv_btn_set_style(buttonBack, LV_BTN_STYLE_PR, &style_para_back);
  label_Back = lv_label_create(buttonBack, NULL);
  lv_label_set_text(label_Back, common_menu.text_back);
  lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable == true) {
        lv_group_add_obj(g, buttonReleaseBind);
        lv_group_add_obj(g, buttonBack);
    }
  #endif // HAS_ROTARY_ENCODER

  //lv_task_handler();

//   lv_fill_rect(0, titleHeight + 30, TFT_HEIGHT - (titleHeight + 30), TFT_HEIGHT - (titleHeight + 30), LV_COLOR_WHITE);

//   /*Create a 100x100 QR code*/
//   qr = lv_qrcode_create(scr, 200, lv_color_hex3(0xffffff), lv_color_hex3(0x000000));

//   lv_obj_set_pos(qr, (TFT_HEIGHT - (titleHeight + 30)) / 2 - 100, (TFT_HEIGHT - (titleHeight + 30)) / 2 - 100 + titleHeight + 30);

//   /*Set data*/
//   lv_qrcode_update(qr, (char *)cloud_para.id, strlen((char *)cloud_para.id));

//   if(id_mark == 0) id_mark = 1;

  //display_qrcode((uint8_t *)cloud_para.id);

  text_id = lv_label_create(scr, NULL);
  lv_obj_set_pos(text_id, 50, 60 + 200 +20);
  lv_obj_set_style(text_id, &tft_style_label_rel);
  lv_label_set_text(text_id, (char *)cloud_para.id);

  id_mark = 0;

  disp_bind_state(); 
}

void disp_bind_state() {
    if(cloud_para.state != 0x12) {
        unbinding_flag = 0;
    }
    if(unbinding_flag) {
        lv_label_set_text(label_bind_or_not, cloud_menu.unbinding);
        lv_obj_align(label_bind_or_not, button_bind_or_not, LV_ALIGN_CENTER, 0, 0);
        lv_btn_set_style(buttonReleaseBind, LV_BTN_STYLE_REL, &style_para_value);
        lv_btn_set_style(buttonReleaseBind, LV_BTN_STYLE_PR, &style_para_value);
    }
    else {
        if(cloud_para.state == 0x10) {
              lv_label_set_text(label_bind_or_not, cloud_menu.disconnected);
              lv_obj_align(label_bind_or_not, button_bind_or_not, LV_ALIGN_CENTER, 0, 0);
        }
        else if(cloud_para.state == 0x11) {
              lv_label_set_text(label_bind_or_not, cloud_menu.unbinded);
              lv_obj_align(label_bind_or_not, button_bind_or_not, LV_ALIGN_CENTER, 0, 0);
        }
        else if(cloud_para.state == 0x12) {
              lv_label_set_text(label_bind_or_not, cloud_menu.binded);
              lv_obj_align(label_bind_or_not, button_bind_or_not, LV_ALIGN_CENTER, 0, 0);
        }
        else {
              lv_label_set_text(label_bind_or_not, cloud_menu.disable);
              lv_obj_align(label_bind_or_not, button_bind_or_not, LV_ALIGN_CENTER, 0, 0);
        }
    }

    if((cloud_para.state == 0x12) && (unbinding_flag == 0)) {
        lv_btn_set_style(buttonReleaseBind, LV_BTN_STYLE_REL, &style_para_back);
        lv_btn_set_style(buttonReleaseBind, LV_BTN_STYLE_PR, &style_para_back);	
    }
    else {
        lv_btn_set_style(buttonReleaseBind, LV_BTN_STYLE_REL, &style_para_value);
        lv_btn_set_style(buttonReleaseBind, LV_BTN_STYLE_PR, &style_para_value);
    }
}

static char last_cloud_state = 0;
void refresh_bind_ui() {
    if((last_cloud_state != cloud_para.state) || unbinding_flag) {
        disp_bind_state();
        last_cloud_state = cloud_para.state;
    }
    if(cloud_para.id[0] != 0) {		
        if(id_mark == 0) {
            // lv_qrcode_update(qr, (char *)cloud_para.id, strlen((char *)cloud_para.id));
            display_qrcode((uint8_t *)cloud_para.id);
            lv_label_set_text(text_id, (char *)cloud_para.id);
        }
    }
    else
        id_mark = 0;
}

void display_qrcode(uint8_t *qrcode_data)
{
    uint8_t i,j;
    uint16_t x,y,p;

    if(id_mark ==0) {
        EncodeData((char *)qrcode_data);
        id_mark = 1;
    }
    lv_fill_rect(10, QRCODE_Y, 300, QRCODE_Y + 300, LV_COLOR_WHITE);

    if(m_nSymbleSize*2 > QRCODE_WIDTH) {
        return;
    }
    
    for(i=0;i<40;i++) {
        if((m_nSymbleSize*i*2) > QRCODE_WIDTH)	break;
    }
    
    p = (i - 1) * 2;
    
    x = QRCODE_X + 70;
    y = QRCODE_Y + 70;
    
    for(i = 0; i < m_nSymbleSize; i++) {
        for(j = 0; j < m_nSymbleSize; j++) {
            if(m_byModuleData[i][j]==1)
                lv_fill_rect(x + p * i, y + p * j, x + p * (i + 1) - 1, y + p * (j + 1) - 1, LV_COLOR_BACKGROUND);
        }	
    }
}

void cloud_unbind() {
    package_to_wifi(WIFI_CLOUD_UNBIND, (char *)0, 0);
    unbinding_flag = 1;
}

void lv_clear_cloud_bind() {
    #if HAS_ROTARY_ENCODER
        if (gCfgItems.encoder_enable == true) {
            lv_group_remove_all_objs(g);
        }
    #endif // HAS_ROTARY_ENCODER
    lv_obj_del(scr); 
}

#endif // HAS_TFT_LVGL_UI
