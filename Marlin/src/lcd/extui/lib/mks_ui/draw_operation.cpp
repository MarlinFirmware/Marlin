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
//#include "../lvgl/src/lv_objx/lv_imgbtn.h"
//#include "../lvgl/src/lv_objx/lv_img.h"
//#include "../lvgl/src/lv_core/lv_disp.h"
//#include "../lvgl/src/lv_core/lv_refr.h"

#include "../../../../module/temperature.h"
#include "../../../../module/motion.h"
#include "../../../../sd/cardreader.h"
#include "../../../../inc/MarlinConfig.h"

extern lv_group_t * g;
static lv_obj_t * scr;

#define ID_O_PRE_HEAT   1
#define ID_O_EXTRUCT    2
#define ID_O_MOV        3
#define ID_O_FILAMENT   4
#define ID_O_SPEED      5
#define ID_O_RETURN     6
#define ID_O_FAN        7
#define ID_O_POWER_OFF  8
#define ID_O_BABY_STEP  9

static lv_obj_t *label_PowerOff;
static lv_obj_t *buttonPowerOff;

extern feedRate_t feedrate_mm_s;

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
        #if HAS_MULTI_EXTRUDER
          uiCfg.curSprayerChoose_bak = active_extruder;
        #endif
        if (uiCfg.print_state == WORKING) {
          #if ENABLED(SDSUPPORT)
            card.pauseSDPrint();
            stop_print_time();
            uiCfg.print_state = PAUSING;
          #endif
        }
        uiCfg.moveSpeed_bak = (uint16_t)feedrate_mm_s;
        uiCfg.desireSprayerTempBak = thermalManager.temp_hotend[active_extruder].target;
        lv_clear_operation();
        lv_draw_filament_change();
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
        if (gCfgItems.finish_power_off) {
          gCfgItems.finish_power_off = false;
          lv_imgbtn_set_src_both(buttonPowerOff, "F:/bmp_manual_off.bin");
          lv_label_set_text(label_PowerOff, printing_more_menu.manual);
        }
        else {
          gCfgItems.finish_power_off = true;
          lv_imgbtn_set_src_both(buttonPowerOff, "F:/bmp_auto_off.bin");
          lv_label_set_text(label_PowerOff, printing_more_menu.auto_close);
        }
        lv_obj_align(label_PowerOff, buttonPowerOff, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
        lv_obj_refresh_ext_draw_pad(label_PowerOff);
        update_spi_flash();
      }
      break;
    case ID_O_BABY_STEP:
      if (event == LV_EVENT_CLICKED) {
        // nothing to do
      }
      else if (event == LV_EVENT_RELEASED) {
        lv_clear_operation();
        lv_draw_baby_stepping();
      }
      break;
  }
}

void lv_draw_operation(void) {
  lv_obj_t *buttonPreHeat = NULL, *buttonExtrusion = NULL, *buttonSpeed = NULL;
  lv_obj_t *buttonBack = NULL, *buttonFan = NULL;
  lv_obj_t *labelPreHeat = NULL, *labelExtrusion = NULL;
  lv_obj_t *label_Back = NULL, *label_Speed = NULL, *label_Fan = NULL;
  lv_obj_t *buttonMove = NULL, *label_Move = NULL;
  lv_obj_t *buttonBabyStep = NULL, *label_BabyStep = NULL;
  lv_obj_t *buttonFilament = NULL, *label_Filament = NULL;

  if (disp_state_stack._disp_state[disp_state_stack._disp_index] != OPERATE_UI) {
    disp_state_stack._disp_index++;
    disp_state_stack._disp_state[disp_state_stack._disp_index] = OPERATE_UI;
  }
  disp_state = OPERATE_UI;

  scr = lv_obj_create(NULL, NULL);

  lv_obj_set_style(scr, &tft_style_scr);
  lv_scr_load(scr);
  lv_obj_clean(scr);

  (void)lv_label_create(scr, TITLE_XPOS, TITLE_YPOS, creat_title_text());

  lv_refr_now(lv_refr_get_disp_refreshing());

  // Create image buttons
  buttonPreHeat  = lv_imgbtn_create(scr, "F:/bmp_temp.bin", INTERVAL_V, titleHeight, event_handler, ID_O_PRE_HEAT);
  buttonFilament = lv_imgbtn_create(scr, "F:/bmp_filamentchange.bin", BTN_X_PIXEL + INTERVAL_V * 2, titleHeight, event_handler, ID_O_FILAMENT);
  buttonFan      = lv_imgbtn_create(scr, "F:/bmp_fan.bin", BTN_X_PIXEL * 2 + INTERVAL_V * 3, titleHeight, event_handler, ID_O_FAN);
  buttonPowerOff = lv_imgbtn_create(scr, gCfgItems.finish_power_off ? "F:/bmp_auto_off.bin" : "F:/bmp_manual_off.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, titleHeight, event_handler, ID_O_POWER_OFF);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      lv_group_add_obj(g, buttonPreHeat);
      lv_group_add_obj(g, buttonFilament);
      lv_group_add_obj(g, buttonFan);
      lv_group_add_obj(g, buttonPowerOff);
    }
  #endif

  if (uiCfg.print_state != WORKING) {
    buttonExtrusion = lv_imgbtn_create(scr, "F:/bmp_extrude_opr.bin", INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_O_EXTRUCT);
    buttonMove      = lv_imgbtn_create(scr, "F:/bmp_move_opr.bin", BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_O_MOV);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) {
        lv_group_add_obj(g, buttonExtrusion);
        lv_group_add_obj(g, buttonMove);
      }
    #endif
  }
  else {
    buttonSpeed    = lv_imgbtn_create(scr, "F:/bmp_speed.bin", INTERVAL_V, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_O_SPEED);
    buttonBabyStep = lv_imgbtn_create(scr, "F:/bmp_mov.bin", BTN_X_PIXEL + INTERVAL_V * 2, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_O_BABY_STEP);
    #if HAS_ROTARY_ENCODER
      if (gCfgItems.encoder_enable) {
        lv_group_add_obj(g, buttonSpeed);
        lv_group_add_obj(g, buttonBabyStep);
      }
    #endif
  }

  buttonBack = lv_imgbtn_create(scr, "F:/bmp_return.bin", BTN_X_PIXEL * 3 + INTERVAL_V * 4, BTN_Y_PIXEL + INTERVAL_H + titleHeight, event_handler, ID_O_RETURN);

  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_add_obj(g, buttonBack);
  #endif

  // Create labels on the image buttons
  labelPreHeat   = lv_label_create_empty(buttonPreHeat);
  label_Filament = lv_label_create_empty(buttonFilament);
  label_Fan      = lv_label_create_empty(buttonFan);
  label_PowerOff = lv_label_create_empty(buttonPowerOff);

  if (uiCfg.print_state != WORKING) {
    /*
      label_Filament = lv_label_create_empty(buttonFilament);
    } else {
    */
    labelExtrusion = lv_label_create_empty(buttonExtrusion);
    label_Move = lv_label_create_empty(buttonMove);
  }
  else {
    label_Speed = lv_label_create_empty(buttonSpeed);
    label_BabyStep = lv_label_create_empty(buttonBabyStep);
  }
  label_Back = lv_label_create_empty(buttonBack);

  if (gCfgItems.multiple_language) {
    lv_label_set_text(labelPreHeat, operation_menu.temp);
    lv_obj_align(labelPreHeat, buttonPreHeat, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Filament, operation_menu.filament);
    lv_obj_align(label_Filament, buttonFilament, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    lv_label_set_text(label_Fan, operation_menu.fan);
    lv_obj_align(label_Fan, buttonFan, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    if (gCfgItems.finish_power_off)
      lv_label_set_text(label_PowerOff, printing_more_menu.auto_close);
    else
      lv_label_set_text(label_PowerOff, printing_more_menu.manual);
    lv_obj_align(label_PowerOff, buttonPowerOff, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

    if (uiCfg.print_state != WORKING) {
      /*
        lv_label_set_text(label_Filament, operation_menu.filament);
        lv_obj_align(label_Filament, buttonFilament, LV_ALIGN_IN_BOTTOM_MID,0, BUTTON_TEXT_Y_OFFSET);
      } else {
      */
      lv_label_set_text(labelExtrusion, operation_menu.extr);
      lv_obj_align(labelExtrusion, buttonExtrusion, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_Move, operation_menu.move);
      lv_obj_align(label_Move, buttonMove, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }
    else {
      lv_label_set_text(label_Speed, operation_menu.speed);
      lv_obj_align(label_Speed, buttonSpeed, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);

      lv_label_set_text(label_BabyStep, operation_menu.babystep);
      lv_obj_align(label_BabyStep, buttonBabyStep, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
    }

    lv_label_set_text(label_Back, common_menu.text_back);
    lv_obj_align(label_Back, buttonBack, LV_ALIGN_IN_BOTTOM_MID, 0, BUTTON_TEXT_Y_OFFSET);
  }
}

void lv_clear_operation() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
