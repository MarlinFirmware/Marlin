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

#include "../../../../MarlinCore.h"
#include "draw_ready_print.h"
#include "draw_set.h"
#include "lv_conf.h"
#include "draw_ui.h"
#include "../../../../gcode/queue.h"

extern lv_group_t * g;
static lv_obj_t * scr;

enum {
  ID_GCODE = 1,
  #if HAS_USER_ITEM(1)
    ID_CUSTOM_1,
  #endif
  #if HAS_USER_ITEM(2)
    ID_CUSTOM_2,
  #endif
  #if HAS_USER_ITEM(3)
    ID_CUSTOM_3,
  #endif
  #if HAS_USER_ITEM(4)
    ID_CUSTOM_4,
  #endif
  #if HAS_USER_ITEM(5)
    ID_CUSTOM_5,
  #endif
  #if HAS_USER_ITEM(6)
    ID_CUSTOM_6,
  #endif
  ID_M_RETURN,
};

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  switch (obj->mks_obj_id) {
    case ID_GCODE: lv_clear_more(); lv_draw_gcode(true); break;
    #if HAS_USER_ITEM(1)
      case ID_CUSTOM_1: queue.inject_P(PSTR(USER_GCODE_1)); break;
    #endif
    #if HAS_USER_ITEM(2)
      case ID_CUSTOM_2: queue.inject_P(PSTR(USER_GCODE_2)); break;
    #endif
    #if HAS_USER_ITEM(3)
      case ID_CUSTOM_3: queue.inject_P(PSTR(USER_GCODE_3)); break;
    #endif
    #if HAS_USER_ITEM(4)
      case ID_CUSTOM_4: queue.inject_P(PSTR(USER_GCODE_4)); break;
    #endif
    #if HAS_USER_ITEM(5)
      case ID_CUSTOM_5: queue.inject_P(PSTR(USER_GCODE_5)); break;
    #endif
    #if HAS_USER_ITEM(6)
      case ID_CUSTOM_6: queue.inject_P(PSTR(USER_GCODE_6)); break;
    #endif
    case ID_M_RETURN:
      lv_clear_more();
      lv_draw_tool();
      break;
  }
}

void lv_draw_more() {
  scr = lv_screen_create(MORE_UI);

  const bool enc_ena = TERN0(HAS_ROTARY_ENCODER, gCfgItems.encoder_enable);

  lv_obj_t *buttonGCode = lv_imgbtn_create(scr, "F:/bmp_machine_para.bin", INTERVAL_V, titleHeight, event_handler, ID_GCODE);
  if (enc_ena) lv_group_add_obj(g, buttonGCode);
  lv_obj_t *labelGCode = lv_label_create_empty(buttonGCode);

  #if HAS_USER_ITEM(1)
    lv_obj_t *buttonCustom1 = lv_imgbtn_create(scr, "F:/bmp_custom1.bin", BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_CUSTOM_1);
    if (enc_ena) lv_group_add_obj(g, buttonCustom1);
    lv_obj_t *labelCustom1 = lv_label_create_empty(buttonCustom1);
  #endif

  #if HAS_USER_ITEM(2)
    lv_obj_t *buttonCustom2 = lv_imgbtn_create(scr, "F:/bmp_custom2.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_CUSTOM_2);
    if (enc_ena) lv_group_add_obj(g, buttonCustom2);
    lv_obj_t *labelCustom2 = lv_label_create_empty(buttonCustom2);
  #endif

  #if HAS_USER_ITEM(3)
    lv_obj_t *buttonCustom3 = lv_imgbtn_create(scr, "F:/bmp_custom3.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_CUSTOM_3);
    if (enc_ena) lv_group_add_obj(g, buttonCustom3);
    lv_obj_t *labelCustom3 = lv_label_create_empty(buttonCustom3);
  #endif

  #if HAS_USER_ITEM(4)
    lv_obj_t *buttonCustom4 = lv_imgbtn_create(scr, "F:/bmp_custom4.bin", INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_CUSTOM_4);
    if (enc_ena) lv_group_add_obj(g, buttonCustom4);
    lv_obj_t *labelCustom4 = lv_label_create_empty(buttonCustom4);
  #endif

  #if HAS_USER_ITEM(5)
    lv_obj_t *buttonCustom5 = lv_imgbtn_create(scr, "F:/bmp_custom5.bin", BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_CUSTOM_5);
    if (enc_ena) lv_group_add_obj(g, buttonCustom5);
    lv_obj_t *labelCustom5 = lv_label_create_empty(buttonCustom5);
  #endif

  #if HAS_USER_ITEM(6)
    lv_obj_t *buttonCustom6 = lv_imgbtn_create(scr, "F:/bmp_custom6.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_CUSTOM_6);
    if (enc_ena) lv_group_add_obj(g, buttonCustom6);
    lv_obj_t *labelCustom6 = lv_label_create_empty(buttonCustom6);
  #endif

  lv_obj_t *buttonBack = lv_imgbtn_create(scr, "F:/bmp_return.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_M_RETURN);
  if (enc_ena) lv_group_add_obj(g, buttonBack);
  lv_obj_t *label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(labelGCode, more_menu.gcode);
    lv_obj_align(labelGCode, buttonGCode, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    #if HAS_USER_ITEM(1)
      lv_label_set_text(labelCustom1, more_menu.custom1);
      lv_obj_align(labelCustom1, buttonCustom1, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if HAS_USER_ITEM(2)
      lv_label_set_text(labelCustom2, more_menu.custom2);
      lv_obj_align(labelCustom2, buttonCustom2, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if HAS_USER_ITEM(3)
      lv_label_set_text(labelCustom3, more_menu.custom3);
      lv_obj_align(labelCustom3, buttonCustom3, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if HAS_USER_ITEM(4)
      lv_label_set_text(labelCustom4, more_menu.custom4);
      lv_obj_align(labelCustom4, buttonCustom4, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if HAS_USER_ITEM(5)
      lv_label_set_text(labelCustom5, more_menu.custom5);
      lv_obj_align(labelCustom5, buttonCustom5, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    #if HAS_USER_ITEM(6)
      lv_label_set_text(labelCustom6, more_menu.custom6);
      lv_obj_align(labelCustom6, buttonCustom6, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    #endif
    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }

  #if BUTTONS_EXIST(EN1, EN2, ENC)
    if (enc_ena) {
      lv_group_add_obj(g, buttonGCode);
      #if HAS_USER_ITEM(1)
        lv_group_add_obj(g, buttonCustom1);
      #endif
      #if HAS_USER_ITEM(2)
        lv_group_add_obj(g, buttonCustom2);
      #endif
      #if HAS_USER_ITEM(3)
        lv_group_add_obj(g, buttonCustom3);
      #endif
      #if HAS_USER_ITEM(4)
        lv_group_add_obj(g, buttonCustom4);
      #endif
      #if HAS_USER_ITEM(5)
        lv_group_add_obj(g, buttonCustom5);
      #endif
      #if HAS_USER_ITEM(6)
        lv_group_add_obj(g, buttonCustom6);
      #endif
      lv_group_add_obj(g, buttonBack);
    }
  #endif
}

void lv_clear_more() {
  #if BUTTONS_EXIST(EN1, EN2, ENC)
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
