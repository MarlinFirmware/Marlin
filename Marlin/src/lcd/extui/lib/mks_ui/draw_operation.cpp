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
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"

#include "../../../../MarlinCore.h"

static lv_obj_t * scr;

#define ID_O_PRE_HEAT   1
#define ID_O_EXTRUCT    2
#define ID_O_MOV        3
#define ID_O_FILAMENT   4
#define ID_O_SPEED      5
#define ID_O_RETURN     6
#define ID_O_FAN        7
#define ID_O_POWER_OFF  8

static lv_obj_t *label_PowerOff;
static lv_obj_t *buttonPowerOff;

static void event_handler(lv_obj_t * obj, lv_event_t event) {
  switch (obj->mks_obj_id) {
    case ID_O_PRE_HEAT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_operation();
        lv_draw_preHeat();
      }
      break;
    case ID_O_EXTRUCT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_operation();
        lv_draw_extrusion();
      }
      break;
    case ID_O_MOV:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_operation();
        lv_draw_move_motor();
      }
      break;
    case ID_O_FILAMENT:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
      }
      break;
    case ID_O_FAN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_operation();
        lv_draw_fan();
      }
      break;
    case ID_O_SPEED:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_operation();
        lv_draw_change_speed();
      }
      break;
    case ID_O_RETURN:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        clear_cur_ui();
        draw_return_ui();
      }
      break;
    case ID_O_POWER_OFF:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        if (gCfgItems.finish_power_off == 1) {
          gCfgItems.finish_power_off = 0;
          lv_obj_set_event_cb_mks(obj, event_handler, ID_O_POWER_OFF, "bmp_manual_off.bin", 0); // didn't find bmp_Mamual...
          lv_label_set_text(label_PowerOff, printing_more_menu.manual);
          lv_obj_align(label_PowerOff, buttonPowerOff, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
          lv_obj_refresh_ext_draw_pad(label_PowerOff);
          update_spi_flash();
        }
        else {
          gCfgItems.finish_power_off = 1;
          lv_obj_set_event_cb_mks(obj, event_handler, ID_O_POWER_OFF, "bmp_auto_off.bin", 0);
          lv_label_set_text(label_PowerOff, printing_more_menu.auto_close);
          lv_obj_align(label_PowerOff, buttonPowerOff, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
          lv_obj_refresh_ext_draw_pad(label_PowerOff);
          update_spi_flash();
        }
      }
      break;

  }
}

void lv_draw_operation(void) {
  lv_obj_t *buttonPreHeat, *buttonExtrusion, *buttonSpeed;
  lv_obj_t *buttonBack, *buttonFan;
  lv_obj_t *labelPreHeat, *labelExtrusion;
  lv_obj_t *label_Back, *label_Speed, *label_Fan;
  lv_obj_t *buttonMove = NULL, *label_Move = NULL;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != OPERATE_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = OPERATE_UI;
  }
  disp_state = OPERATE_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  lv_obj_t * title = lv_label_create(scr, NULL);
  lv_obj_set_style(title, &tft_style_label_rel);
  lv_obj_set_pos(title, TITLE_XPOS, TITLE_YPOS);
  lv_label_set_text(title, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  LV_IMG_DECLARE(bmp_pic);

  /*Create an Image button*/
  buttonPreHeat   = lv_imgbtn_create(scr, NULL);
  buttonExtrusion = lv_imgbtn_create(scr, NULL);
  buttonFan       = lv_imgbtn_create(scr, NULL);
  buttonSpeed     = lv_imgbtn_create(scr, NULL);

  if (uiCfg.print_state != WORKING) {
    //buttonFilament = lv_imgbtn_create(scr, NULL);
    //} else {
    buttonMove = lv_imgbtn_create(scr, NULL);
  }

  buttonPowerOff = lv_imgbtn_create(scr, NULL);
  buttonBack     = lv_imgbtn_create(scr, NULL);

  lv_obj_set_event_cb_mks(buttonPreHeat, event_handler, ID_O_PRE_HEAT, "bmp_temp.bin", 0);
  lv_imgbtn_set_src(buttonPreHeat, LV_BTN_STATE_REL, &bmp_pic);
  lv_imgbtn_set_src(buttonPreHeat, LV_BTN_STATE_PR, &bmp_pic);
  lv_imgbtn_set_style(buttonPreHeat, LV_BTN_STATE_PR, &tft_style_label_pre);
  lv_imgbtn_set_style(buttonPreHeat, LV_BTN_STATE_REL, &tft_style_label_rel);
  lv_obj_clear_protect(buttonPreHeat, LV_PROTECT_FOLLOW);
  #if 1
    lv_obj_set_event_cb_mks(buttonExtrusion, event_handler, ID_O_EXTRUCT, "bmp_extrude_opr.bin", 0);
    lv_imgbtn_set_src(buttonExtrusion, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonExtrusion, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonExtrusion, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonExtrusion, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonFan, event_handler, ID_O_FAN, "bmp_fan.bin", 0);
    lv_imgbtn_set_src(buttonFan, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonFan, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonFan, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonFan, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonSpeed, event_handler, ID_O_SPEED, "bmp_speed.bin", 0);
    lv_imgbtn_set_src(buttonSpeed, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonSpeed, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonSpeed, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonSpeed, LV_BTN_STATE_REL, &tft_style_label_rel);

    if (uiCfg.print_state != WORKING) {
      /*
        lv_obj_set_event_cb_mks(buttonFilament, event_handler,ID_O_FILAMENT,"bmp_Filamentchange.bin",0);
        lv_imgbtn_set_src(buttonFilament, LV_BTN_STATE_REL, &bmp_pic);
        lv_imgbtn_set_src(buttonFilament, LV_BTN_STATE_PR, &bmp_pic);
        lv_imgbtn_set_style(buttonFilament, LV_BTN_STATE_PR, &tft_style_label_pre);
        lv_imgbtn_set_style(buttonFilament, LV_BTN_STATE_REL, &tft_style_label_rel);
      } else {
      */
      lv_obj_set_event_cb_mks(buttonMove, event_handler, ID_O_MOV, "bmp_move_opr.bin", 0);
      lv_imgbtn_set_src(buttonMove, LV_BTN_STATE_REL, &bmp_pic);
      lv_imgbtn_set_src(buttonMove, LV_BTN_STATE_PR, &bmp_pic);
      lv_imgbtn_set_style(buttonMove, LV_BTN_STATE_PR, &tft_style_label_pre);
      lv_imgbtn_set_style(buttonMove, LV_BTN_STATE_REL, &tft_style_label_rel);
    }
    if (gCfgItems.finish_power_off == 1)
      lv_obj_set_event_cb_mks(buttonPowerOff, event_handler, ID_O_POWER_OFF, "bmp_auto_off.bin", 0);
    else
      lv_obj_set_event_cb_mks(buttonPowerOff, event_handler, ID_O_POWER_OFF, "bmp_manual_off.bin", 0);
    lv_imgbtn_set_src(buttonPowerOff, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonPowerOff, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonPowerOff, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonPowerOff, LV_BTN_STATE_REL, &tft_style_label_rel);

    lv_obj_set_event_cb_mks(buttonBack, event_handler, ID_O_RETURN, "bmp_return.bin", 0);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_REL, &bmp_pic);
    lv_imgbtn_set_src(buttonBack, LV_BTN_STATE_PR, &bmp_pic);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_PR, &tft_style_label_pre);
    lv_imgbtn_set_style(buttonBack, LV_BTN_STATE_REL, &tft_style_label_rel);
  #endif // if 1

  lv_obj_set_pos(buttonPreHeat, INTERVAL_V, titleHeight);
  lv_obj_set_pos(buttonExtrusion, BTN_X_PIXEL + INTERVAL_V * 2, titleHeight);

  lv_obj_set_pos(buttonFan, BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight);
  lv_obj_set_pos(buttonSpeed, BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight);

  if (uiCfg.print_state != WORKING) {
    /*
      lv_obj_set_pos(buttonFilament,INTERVAL_V,BTN_Y_PIXEL+INTERVAL_H+titleHeight);
    } else {
    */
    lv_obj_set_pos(buttonMove, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
    lv_obj_set_pos(buttonPowerOff, BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  }
  else {
    lv_obj_set_pos(buttonPowerOff, INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight);
  }
  lv_obj_set_pos(buttonBack, BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight);

  /*Create a label on the Image button*/
  lv_btn_set_layout(buttonPreHeat, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonExtrusion, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonFan, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonSpeed, LV_LAYOUT_OFF);

  if (uiCfg.print_state != WORKING) {
    /*
      lv_btn_set_layout(buttonFilament, LV_LAYOUT_OFF);
    } else {
    */
    lv_btn_set_layout(buttonMove, LV_LAYOUT_OFF);
  }
  lv_btn_set_layout(buttonPowerOff, LV_LAYOUT_OFF);
  lv_btn_set_layout(buttonBack, LV_LAYOUT_OFF);

  labelPreHeat   = lv_label_create(buttonPreHeat, NULL);
  labelExtrusion = lv_label_create(buttonExtrusion, NULL);

  label_Fan   = lv_label_create(buttonFan, NULL);
  label_Speed = lv_label_create(buttonSpeed, NULL);

  if (uiCfg.print_state != WORKING) {
    /*
      label_Filament = lv_label_create(buttonFilament, NULL);
    } else {
    */
      label_Move = lv_label_create(buttonMove, NULL);
  }
  label_PowerOff = lv_label_create(buttonPowerOff, NULL);

  label_Back = lv_label_create(buttonBack, NULL);

  if (gCfgItems.multiple_language != 0) {
    lv_label_set_text(labelPreHeat, operation_menu.temp);
    lv_obj_align(labelPreHeat, buttonPreHeat, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(labelExtrusion, operation_menu.extr);
    lv_obj_align(labelExtrusion, buttonExtrusion, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Fan, operation_menu.fan);
    lv_obj_align(label_Fan, buttonFan, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Speed, operation_menu.speed);
    lv_obj_align(label_Speed, buttonSpeed, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    if (uiCfg.print_state != WORKING) {
      /*
        lv_label_set_text(label_Filament, operation_menu.filament);
        lv_obj_align(label_Filament, buttonFilament, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
      } else {
      */
      lv_label_set_text(label_Move, operation_menu.move);
      lv_obj_align(label_Move, buttonMove, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }

    if (gCfgItems.finish_power_off == 1)
      lv_label_set_text(label_PowerOff, printing_more_menu.auto_close);
    else
      lv_label_set_text(label_PowerOff, printing_more_menu.manual);
    lv_obj_align(label_PowerOff, buttonPowerOff, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }
}

void lv_clear_operation() { lv_obj_del(scr); }

#endif // HAS_TFT_LVGL_UI
