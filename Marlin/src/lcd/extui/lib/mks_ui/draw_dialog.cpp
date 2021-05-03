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

/**
 * draw_dialog.cpp
 */

#include "../../../../inc/MarlinConfigPre.h"

#if HAS_TFT_LVGL_UI

#include "draw_ui.h"
#include <lv_conf.h>

#include "../../../../sd/cardreader.h"
#include "../../../../gcode/queue.h"
#include "../../../../module/temperature.h"
#include "../../../../module/planner.h"
#include "../../../../gcode/gcode.h"
#include "../../../../inc/MarlinConfig.h"

#if ENABLED(EEPROM_SETTINGS)
  #include "../../../../module/settings.h"
#endif

#if ENABLED(POWER_LOSS_RECOVERY)
  #include "../../../../feature/powerloss.h"
#endif

#if ENABLED(PARK_HEAD_ON_PAUSE)
  #include "../../../../feature/pause.h"
#endif

#if ENABLED(TOUCH_SCREEN_CALIBRATION)
  #include "../../../tft_io/touch_calibration.h"
  #include "draw_touch_calibration.h"
#endif

extern lv_group_t *g;
static lv_obj_t *scr, *tempText1, *filament_bar;

extern uint8_t sel_id;
extern bool once_flag, gcode_preview_over;
extern int upload_result;
extern uint32_t upload_time;
extern uint32_t upload_size;
extern bool temps_update_flag;

//#define CANCEL_ON_RIGHT   // Put 'Cancel' on the right (as it was before)

#define BTN_OK_X      TERN(CANCEL_ON_RIGHT, 100, 280)
#define BTN_CANCEL_X  TERN(CANCEL_ON_RIGHT, 280, 100)
#define BTN_OK_Y      180
#define BTN_CANCEL_Y  180

static void btn_ok_event_cb(lv_obj_t *btn, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  if (DIALOG_IS(TYPE_PRINT_FILE)) {
    #if HAS_GCODE_PREVIEW
      preview_gcode_prehandle(list_file.file_name[sel_id]);
    #endif
    reset_print_time();
    start_print_time();

    uiCfg.print_state = WORKING;
    lv_clear_dialog();
    lv_draw_printing();

    #if ENABLED(SDSUPPORT)
      if (!gcode_preview_over) {
        char *cur_name;
        cur_name = strrchr(list_file.file_name[sel_id], '/');

        SdFile file, *curDir;
        card.endFilePrint();
        const char * const fname = card.diveToFile(true, curDir, cur_name);
        if (!fname) return;
        if (file.open(curDir, fname, O_READ)) {
          gCfgItems.curFilesize = file.fileSize();
          file.close();
          update_spi_flash();
        }
        card.openFileRead(cur_name);
        if (card.isFileOpen()) {
          feedrate_percentage = 100;
          planner.flow_percentage[0] = 100;
          planner.e_factor[0]        = planner.flow_percentage[0] * 0.01f;
          #if HAS_MULTI_EXTRUDER
            planner.flow_percentage[1] = 100;
            planner.e_factor[1]        = planner.flow_percentage[1] * 0.01f;
          #endif
          card.startFileprint();
          #if ENABLED(POWER_LOSS_RECOVERY)
            recovery.prepare();
          #endif
          once_flag = false;
        }
      }
    #endif
  }
  else if (DIALOG_IS(TYPE_STOP)) {
    wait_for_heatup = false;
    stop_print_time();
    lv_clear_dialog();
    lv_draw_ready_print();

    #if ENABLED(SDSUPPORT)
      uiCfg.print_state           = IDLE;
      card.flag.abort_sd_printing = true;
    #endif
  }
  else if (DIALOG_IS(TYPE_FINISH_PRINT)) {
    clear_cur_ui();
    lv_draw_ready_print();
  }
  #if ENABLED(ADVANCED_PAUSE_FEATURE)
    else if (DIALOG_IS(PAUSE_MESSAGE_WAITING, PAUSE_MESSAGE_INSERT, PAUSE_MESSAGE_HEAT))
      wait_for_user = false;
    else if (DIALOG_IS(PAUSE_MESSAGE_OPTION))
      pause_menu_response = PAUSE_RESPONSE_EXTRUDE_MORE;
    else if (DIALOG_IS(PAUSE_MESSAGE_RESUME)) {
      clear_cur_ui();
      draw_return_ui();
    }
  #endif
  else if (DIALOG_IS(STORE_EEPROM_TIPS)) {
    TERN_(EEPROM_SETTINGS, (void)settings.save());
    clear_cur_ui();
    draw_return_ui();
  }
  else if (DIALOG_IS(READ_EEPROM_TIPS)) {
    TERN_(EEPROM_SETTINGS, (void)settings.load());
    clear_cur_ui();
    draw_return_ui();
  }
  else if (DIALOG_IS(REVERT_EEPROM_TIPS)) {
    TERN_(EEPROM_SETTINGS, (void)settings.reset());
    clear_cur_ui();
    #if ENABLED(TOUCH_SCREEN_CALIBRATION)
      const bool do_draw_cal = touch_calibration.need_calibration();
      if (do_draw_cal) {
        disp_state_stack._disp_index--; // We are asynchronous from the dialog, so let's remove the dialog from the stack
        lv_draw_touch_calibration_screen();
      }
    #else
      constexpr bool do_draw_cal = false;
    #endif
    if (!do_draw_cal) draw_return_ui();
  }
  else if (DIALOG_IS(WIFI_CONFIG_TIPS)) {
    uiCfg.configWifi = true;
    clear_cur_ui();
    draw_return_ui();
  }
  else if (DIALOG_IS(TYPE_FILAMENT_HEAT_LOAD_COMPLETED))
    uiCfg.filament_heat_completed_load = true;
  else if (DIALOG_IS(TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED))
    uiCfg.filament_heat_completed_unload = true;
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_COMPLETED, TYPE_FILAMENT_UNLOAD_COMPLETED)) {
    clear_cur_ui();
    draw_return_ui();
  }
  #if ENABLED(MKS_WIFI_MODULE)
    else if (DIALOG_IS(TYPE_UNBIND)) {
      cloud_unbind();
      clear_cur_ui();
      draw_return_ui();
    }
  #endif
  else {
    clear_cur_ui();
    draw_return_ui();
  }
}

static void btn_cancel_event_cb(lv_obj_t *btn, lv_event_t event) {
  if (event != LV_EVENT_RELEASED) return;
  if (DIALOG_IS(PAUSE_MESSAGE_OPTION)) {
    TERN_(ADVANCED_PAUSE_FEATURE, pause_menu_response = PAUSE_RESPONSE_RESUME_PRINT);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT, TYPE_FILAMENT_UNLOAD_HEAT, TYPE_FILAMENT_HEAT_LOAD_COMPLETED, TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED)) {
    thermalManager.setTargetHotend(uiCfg.hotendTargetTempBak, uiCfg.extruderIndex);
    clear_cur_ui();
    draw_return_ui();
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOADING, TYPE_FILAMENT_UNLOADING)) {
    queue.enqueue_one_P(PSTR("M410"));
    uiCfg.filament_rate                = 0;
    uiCfg.filament_loading_completed   = false;
    uiCfg.filament_unloading_completed = false;
    uiCfg.filament_loading_time_flg    = false;
    uiCfg.filament_loading_time_cnt    = 0;
    uiCfg.filament_unloading_time_flg  = false;
    uiCfg.filament_unloading_time_cnt  = 0;
    thermalManager.setTargetHotend(uiCfg.hotendTargetTempBak, uiCfg.extruderIndex);
    clear_cur_ui();
    draw_return_ui();
  }
  else {
    clear_cur_ui();
    draw_return_ui();
  }
}

void lv_draw_dialog(uint8_t type) {
  lv_obj_t *btnOk = nullptr, *btnCancel = nullptr;
  uiCfg.dialogType = type;
  scr = lv_screen_create(DIALOG_UI);

  lv_obj_t *labelDialog = lv_label_create(scr, "");

  if (DIALOG_IS(TYPE_FINISH_PRINT, PAUSE_MESSAGE_RESUME)) {
      btnOk = lv_button_btn_create(scr, BTN_OK_X + 90, BTN_OK_Y, 100, 50, btn_ok_event_cb);
      lv_obj_t *labelOk = lv_label_create_empty(btnOk);             // Add a label to the button
      lv_label_set_text(labelOk, print_file_dialog_menu.confirm);    // Set the labels text
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_WAITING, PAUSE_MESSAGE_INSERT, PAUSE_MESSAGE_HEAT)) {
    btnOk = lv_button_btn_create(scr, BTN_OK_X + 90, BTN_OK_Y, 100, 50, btn_ok_event_cb);
    lv_obj_t *labelOk = lv_label_create_empty(btnOk);             // Add a label to the button
    lv_label_set_text(labelOk, print_file_dialog_menu.confirm);    // Set the labels text
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_PAUSING, PAUSE_MESSAGE_CHANGING, PAUSE_MESSAGE_UNLOAD, PAUSE_MESSAGE_LOAD, PAUSE_MESSAGE_PURGE, PAUSE_MESSAGE_RESUME, PAUSE_MESSAGE_HEATING)) {
    // nothing to do
  }
  else if (DIALOG_IS(WIFI_ENABLE_TIPS)) {
    btnCancel = lv_button_btn_create(scr, BTN_OK_X + 90, BTN_OK_Y, 100, 50, btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
    lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);
  }
  else if (DIALOG_IS(TRANSFER_NO_DEVICE)) {
    btnCancel = lv_button_btn_create(scr, BTN_OK_X + 90, BTN_OK_Y, 100, 50, btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
    lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);
  }
  #if ENABLED(MKS_WIFI_MODULE)
    else if (DIALOG_IS(TYPE_UPLOAD_FILE)) {
      if (upload_result == 2) {
        btnCancel = lv_button_btn_create(scr, BTN_OK_X + 90, BTN_OK_Y, 100, 50, btn_cancel_event_cb);
        lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
        lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);
      }
      else if (upload_result == 3) {
        btnOk = lv_button_btn_create(scr, BTN_OK_X + 90, BTN_OK_Y, 100, 50, btn_ok_event_cb);
        lv_obj_t *labelOk = lv_label_create_empty(btnOk);
        lv_label_set_text(labelOk, print_file_dialog_menu.confirm);
      }
    }
    else if (DIALOG_IS(TYPE_UPDATE_ESP_FIRMWARE)) {
      // nothing to do
    }
  #endif
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT, TYPE_FILAMENT_UNLOAD_HEAT)) {
    btnCancel = lv_button_btn_create(scr, BTN_OK_X+90, BTN_OK_Y, 100, 50, btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
    lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);

    tempText1 = lv_label_create_empty(scr);
    filament_sprayer_temp();
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_COMPLETED, TYPE_FILAMENT_UNLOAD_COMPLETED)) {
    btnOk = lv_button_btn_create(scr, BTN_OK_X + 90, BTN_OK_Y, 100, 50, btn_ok_event_cb);
    lv_obj_t *labelOk = lv_label_create_empty(btnOk);
    lv_label_set_text(labelOk, print_file_dialog_menu.confirm);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOADING, TYPE_FILAMENT_UNLOADING)) {
    btnCancel = lv_button_btn_create(scr, BTN_OK_X + 90, BTN_OK_Y, 100, 50, btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);
    lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);

    filament_bar = lv_bar_create(scr, nullptr);
    lv_obj_set_pos(filament_bar, (TFT_WIDTH-400)/2, ((TFT_HEIGHT - titleHeight)-40)/2);
    lv_obj_set_size(filament_bar, 400, 25);
    lv_bar_set_style(filament_bar, LV_BAR_STYLE_INDIC, &lv_bar_style_indic);
    lv_bar_set_anim_time(filament_bar, 1000);
    lv_bar_set_value(filament_bar, 0, LV_ANIM_ON);
  }
  else {
    btnOk = lv_button_btn_create(scr, BTN_OK_X, BTN_OK_Y, 100, 50, btn_ok_event_cb);
    lv_obj_t *labelOk = lv_label_create_empty(btnOk);             // Add a label to the button

    btnCancel = lv_button_btn_create(scr, BTN_CANCEL_X, BTN_CANCEL_Y, 100, 50, btn_cancel_event_cb);
    lv_obj_t *labelCancel = lv_label_create_empty(btnCancel);     // Add a label to the button

    if (DIALOG_IS(PAUSE_MESSAGE_OPTION)) {
      lv_label_set_text(labelOk, pause_msg_menu.purgeMore);        // Set the labels text
      lv_label_set_text(labelCancel, pause_msg_menu.continuePrint);
    }
    else {
      lv_label_set_text(labelOk, print_file_dialog_menu.confirm);  // Set the labels text
      lv_label_set_text(labelCancel, print_file_dialog_menu.cancel);
    }
  }
  if (DIALOG_IS(TYPE_PRINT_FILE)) {
    lv_label_set_text(labelDialog, print_file_dialog_menu.print_file);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);

    lv_obj_t *labelFile = lv_label_create(scr, list_file.long_name[sel_id]);
    lv_obj_align(labelFile, nullptr, LV_ALIGN_CENTER, 0, -60);
  }
  else if (DIALOG_IS(TYPE_STOP)) {
    lv_label_set_text(labelDialog, print_file_dialog_menu.cancel_print);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(TYPE_FINISH_PRINT)) {
    lv_label_set_text(labelDialog, print_file_dialog_menu.print_finish);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_PAUSING)) {
    lv_label_set_text(labelDialog, pause_msg_menu.pausing);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_CHANGING)) {
    lv_label_set_text(labelDialog, pause_msg_menu.changing);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_UNLOAD)) {
    lv_label_set_text(labelDialog, pause_msg_menu.unload);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_WAITING)) {
    lv_label_set_text(labelDialog, pause_msg_menu.waiting);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_INSERT)) {
    lv_label_set_text(labelDialog, pause_msg_menu.insert);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_LOAD)) {
    lv_label_set_text(labelDialog, pause_msg_menu.load);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_PURGE)) {
    lv_label_set_text(labelDialog, pause_msg_menu.purge);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_RESUME)) {
    lv_label_set_text(labelDialog, pause_msg_menu.resume);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_HEAT)) {
    lv_label_set_text(labelDialog, pause_msg_menu.heat);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_HEATING)) {
    lv_label_set_text(labelDialog, pause_msg_menu.heating);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(PAUSE_MESSAGE_OPTION)) {
    lv_label_set_text(labelDialog, pause_msg_menu.option);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(STORE_EEPROM_TIPS)) {
    lv_label_set_text(labelDialog, eeprom_menu.storeTips);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(READ_EEPROM_TIPS)) {
    lv_label_set_text(labelDialog, eeprom_menu.readTips);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(REVERT_EEPROM_TIPS)) {
    lv_label_set_text(labelDialog, eeprom_menu.revertTips);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(WIFI_CONFIG_TIPS)) {
    lv_label_set_text(labelDialog, machine_menu.wifiConfigTips);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(WIFI_ENABLE_TIPS)) {
    lv_label_set_text(labelDialog, print_file_dialog_menu.wifi_enable_tips);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(TRANSFER_NO_DEVICE)) {
    lv_label_set_text(labelDialog, DIALOG_UPDATE_NO_DEVICE_EN);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  #if ENABLED(MKS_WIFI_MODULE)
    else if (DIALOG_IS(TYPE_UPLOAD_FILE)) {
      if (upload_result == 1) {
        lv_label_set_text(labelDialog, DIALOG_UPLOAD_ING_EN);
        lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
      }
      else if (upload_result == 2) {
        lv_label_set_text(labelDialog, DIALOG_UPLOAD_ERROR_EN);
        lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
      }
      else if (upload_result == 3) {
        char buf[200];
        int _index = 0;

        strcpy(buf, DIALOG_UPLOAD_FINISH_EN);
        _index = strlen(buf);
        buf[_index] = '\n';
        _index++;
        strcat(buf, DIALOG_UPLOAD_SIZE_EN);

        _index = strlen(buf);
        buf[_index] = ':';
        _index++;
        sprintf(&buf[_index], " %d KBytes\n", (int)(upload_size / 1024));

        strcat(buf, DIALOG_UPLOAD_TIME_EN);
        _index = strlen(buf);
        buf[_index] = ':';
        _index++;
        sprintf(&buf[_index], " %d s\n", (int)upload_time);

        strcat(buf, DIALOG_UPLOAD_SPEED_EN);
        _index = strlen(buf);
        buf[_index] = ':';
        _index++;
        sprintf(&buf[_index], " %d KBytes/s\n", (int)(upload_size / upload_time / 1024));

        lv_label_set_text(labelDialog, buf);
        lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
      }
    }
    else if (DIALOG_IS(TYPE_UPDATE_ESP_FIRMWARE)) {
      lv_label_set_text(labelDialog, DIALOG_UPDATE_WIFI_FIRMWARE_EN);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -20);
    }
  #endif // MKS_WIFI_MODULE
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_load_heat);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_HEAT_LOAD_COMPLETED)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_load_heat_confirm);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_HEAT)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_unload_heat);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_unload_heat_confirm);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOAD_COMPLETED)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_load_completed);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_UNLOAD_COMPLETED)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_unload_completed);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -20);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_LOADING)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_loading);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -70);
  }
  else if (DIALOG_IS(TYPE_FILAMENT_UNLOADING)) {
    lv_label_set_text(labelDialog, filament_menu.filament_dialog_unloading);
    lv_obj_align(labelDialog, nullptr, LV_ALIGN_CENTER, 0, -70);
  }
  #if ENABLED(MKS_WIFI_MODULE)
    else if (DIALOG_IS(TYPE_UNBIND)) {
      lv_label_set_text(labelDialog, common_menu.unbind_printer_tips);
      lv_obj_align(labelDialog, NULL, LV_ALIGN_CENTER, 0, -70);
    }
  #endif
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) {
      if (btnOk) lv_group_add_obj(g, btnOk);
      if (btnCancel) lv_group_add_obj(g, btnCancel);
    }
  #endif
}

void filament_sprayer_temp() {
  char buf[20] = {0};
  sprintf(buf, preheat_menu.value_state, thermalManager.wholeDegHotend(uiCfg.extruderIndex), thermalManager.degTargetHotend(uiCfg.extruderIndex));

  strcpy(public_buf_l, uiCfg.extruderIndex < 1 ? extrude_menu.ext1 : extrude_menu.ext2);
  strcat_P(public_buf_l, PSTR(": "));
  strcat(public_buf_l, buf);
  lv_label_set_text(tempText1, public_buf_l);
  lv_obj_align(tempText1, nullptr, LV_ALIGN_CENTER, 0, -50);
}

void filament_dialog_handle() {
  if (temps_update_flag && (DIALOG_IS(TYPE_FILAMENT_LOAD_HEAT, TYPE_FILAMENT_UNLOAD_HEAT))) {
    filament_sprayer_temp();
    temps_update_flag = false;
  }
  if (uiCfg.filament_heat_completed_load) {
    uiCfg.filament_heat_completed_load = false;
    lv_clear_dialog();
    lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOADING);
    planner.synchronize();
    uiCfg.filament_loading_time_flg = true;
    uiCfg.filament_loading_time_cnt = 0;
    sprintf_P(public_buf_m, PSTR("T%d\nG91\nG1 E%d F%d\nG90"), uiCfg.extruderIndex, gCfgItems.filamentchange_load_length, gCfgItems.filamentchange_load_speed);
    queue.inject(public_buf_m);
  }
  if (uiCfg.filament_heat_completed_unload) {
    uiCfg.filament_heat_completed_unload = false;
    lv_clear_dialog();
    lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOADING);
    planner.synchronize();
    uiCfg.filament_unloading_time_flg = true;
    uiCfg.filament_unloading_time_cnt = 0;
    sprintf_P(public_buf_m, PSTR("T%d\nG91\nG1 E-%d F%d\nG90"), uiCfg.extruderIndex, gCfgItems.filamentchange_unload_length, gCfgItems.filamentchange_unload_speed);
    queue.inject(public_buf_m);
  }

  if (uiCfg.filament_load_heat_flg) {
    const celsius_t diff = thermalManager.wholeDegHotend(uiCfg.extruderIndex) - gCfgItems.filament_limit_temp;
    if (abs(diff) < 2 || diff > 0) {
      uiCfg.filament_load_heat_flg = false;
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_LOAD_COMPLETED);
    }
  }

  if (uiCfg.filament_loading_completed) {
    uiCfg.filament_rate = 0;
    uiCfg.filament_loading_completed = false;
    lv_clear_dialog();
    lv_draw_dialog(DIALOG_TYPE_FILAMENT_LOAD_COMPLETED);
  }

  if (uiCfg.filament_unload_heat_flg) {
    const celsius_t diff = thermalManager.wholeDegHotend(uiCfg.extruderIndex) - gCfgItems.filament_limit_temp;
    if (abs(diff) < 2 || diff > 0) {
      uiCfg.filament_unload_heat_flg = false;
      lv_clear_dialog();
      lv_draw_dialog(DIALOG_TYPE_FILAMENT_HEAT_UNLOAD_COMPLETED);
    }
  }

  if (uiCfg.filament_unloading_completed) {
    uiCfg.filament_rate = 0;
    uiCfg.filament_unloading_completed = false;
    lv_clear_dialog();
    lv_draw_dialog(DIALOG_TYPE_FILAMENT_UNLOAD_COMPLETED);
  }

  if (DIALOG_IS(TYPE_FILAMENT_LOADING, TYPE_FILAMENT_UNLOADING))
    lv_filament_setbar();
}

void lv_filament_setbar() {
  lv_bar_set_value(filament_bar, uiCfg.filament_rate, LV_ANIM_ON);
}

void lv_clear_dialog() {
  #if HAS_ROTARY_ENCODER
    if (gCfgItems.encoder_enable) lv_group_remove_all_objs(g);
  #endif
  lv_obj_del(scr);
}

#endif // HAS_TFT_LVGL_UI
