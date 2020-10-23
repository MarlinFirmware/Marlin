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

#define ID_EEPROM_RETURN        1
#define ID_EEPROM_STORE         2
#define ID_EEPROM_STORE_ARROW   3
#define ID_EEPROM_READ          4
#define ID_EEPROM_READ_ARROW    5
#define ID_EEPROM_REVERT        6
#define ID_EEPROM_REVERT_ARROW  7

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_EEPROM_RETURN:
      if (event == LV_EVENT_CLICKED) {
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_eeprom_settings();
        draw_return_ui();
      }
      break;
    case ID_EEPROM_STORE:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_eeprom_settings();
        lv_draw_dialog(DIALOG_STORE_EEPROM_TIPS);
      }
      break;
    case ID_EEPROM_STORE_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_eeprom_settings();
        lv_draw_dialog(DIALOG_STORE_EEPROM_TIPS);
      }
      break;
    #if 0
      case ID_EEPROM_READ:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          lv_clear_eeprom_settings();
          lv_draw_dialog(DIALOG_READ_EEPROM_TIPS);
        }
        break;
      case ID_EEPROM_READ_ARROW:
        if (event == LV_EVENT_CLICKED) {

        }
        else if (event == LV_EVENT_RELEASED) {
          lv_clear_eeprom_settings();
          lv_draw_dialog(DIALOG_READ_EEPROM_TIPS);
        }
        break;
    #endif

    case ID_EEPROM_REVERT:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_eeprom_settings();
        lv_draw_dialog(DIALOG_REVERT_EEPROM_TIPS);
      }
      break;
    case ID_EEPROM_REVERT_ARROW:
      if (event == LV_EVENT_CLICKED) {

      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_eeprom_settings();
        lv_draw_dialog(DIALOG_REVERT_EEPROM_TIPS);
      }
      break;
  }
}

void lv_draw_eeprom_settings(void) {
  lv_obj_t *buttonBack, *label_Back;
  lv_obj_t *buttonStore, *labelStore;
  //lv_obj_t *buttonRead,*labelRead,*buttonReadNarrow;
  lv_obj_t *buttonRevert, *labelRevert;
  lv_obj_t * line1, * line2; //* line3;
  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != EEPROM_SETTINGS_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = EEPROM_SETTINGS_UI;
  }
  disp_state = EEPROM_SETTINGS_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  buttonRevert = lv_btn_create(scr, NULL);   /*Add a button the current screen*/
  lv_obj_set_pos(buttonRevert, PARA_UI_POS_X, PARA_UI_POS_Y);                         /*Set its position*/
  lv_obj_set_size(buttonRevert, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);                       /*Set its size*/
  //lv_obj_set_event_cb(buttonMotor, event_handler);
  lv_obj_set_event_cb_mks(buttonRevert, event_handler, ID_EEPROM_REVERT, NULL, 0);
  lv_btn_use_label_style(buttonRevert);
  lv_btn_set_layout(buttonRevert, LV_LAYOUT_OFF);
  labelRevert = lv_label_create_empty(buttonRevert);        /*Add a label to the button*/

  (void)lv_imgbtn_create(scr, "F:/bmp_arrow.bin", PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y + PARA_UI_ARROW_V, event_handler, ID_EEPROM_REVERT_ARROW);

  //line3 = lv_line_create(scr, NULL);
  //lv_ex_line(line3,line_points[2]);
  line1 = lv_line_create(scr, NULL);
  lv_ex_line(line1, line_points[0]);

  buttonStore = lv_btn_create(scr, NULL);   /*Add a button the current screen*/
  lv_obj_set_pos(buttonStore, PARA_UI_POS_X, PARA_UI_POS_Y * 2);                      /*Set its position*/
  lv_obj_set_size(buttonStore, PARA_UI_SIZE_X, PARA_UI_SIZE_Y);                       /*Set its size*/
  //lv_obj_set_event_cb(buttonMotor, event_handler);
  lv_obj_set_event_cb_mks(buttonStore, event_handler, ID_EEPROM_STORE, NULL, 0);
  lv_btn_use_label_style(buttonStore);
  lv_btn_set_layout(buttonStore, LV_LAYOUT_OFF);
  labelStore = lv_label_create_empty(buttonStore);        /*Add a label to the button*/

  (void)lv_imgbtn_create(scr, "F:/bmp_arrow.bin", PARA_UI_POS_X + PARA_UI_SIZE_X, PARA_UI_POS_Y * 2 + PARA_UI_ARROW_V, event_handler, ID_EEPROM_STORE_ARROW);

  line2 = lv_line_create(scr, NULL);
  lv_ex_line(line2, line_points[1]);

  buttonBack = lv_imgbtn_create(scr, "F:/bmp_back70x40.bin", PARA_UI_BACL_POS_X, PARA_UI_BACL_POS_Y, event_handler, ID_EEPROM_RETURN);
  label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_CENTER, 0, 0);

    lv_label_set_text(labelStore, eeprom_menu.store);
    lv_obj_align(labelStore, buttonStore, LV_ALIGN_IN_LEFT_MID,0, 0);

    //lv_label_set_text(labelRead, eeprom_menu.read);
    //lv_obj_align(labelRead, buttonRead, LV_ALIGN_IN_LEFT_MID,0, 0);

    lv_label_set_text(labelRevert, eeprom_menu.revert);
    lv_obj_align(labelRevert, buttonRevert, LV_ALIGN_IN_LEFT_MID, 0, 0);
  }
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonRevert);
      lv_group_add_obj(g, buttonStore);
      lv_group_add_obj(g, buttonBack);
    }
  #endif

}

void lv_clear_eeprom_settings() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
