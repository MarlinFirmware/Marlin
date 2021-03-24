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
#pragma once

#include <lvgl.h>

#include <stdint.h>
#include <string.h>

// the colors of the last MKS Ui
#undef LV_COLOR_BACKGROUND
#define LV_COLOR_BACKGROUND LV_COLOR_MAKE(0x1A, 0x1A, 0x1A)

#define TFT_LV_PARA_BACK_BODY_COLOR  LV_COLOR_MAKE(0x4A, 0x52, 0xFF)

#include "tft_lvgl_configuration.h"
#include "tft_multi_language.h"
#include "pic_manager.h"
#include "draw_ready_print.h"
#include "draw_language.h"
#include "draw_set.h"
#include "draw_tool.h"
#include "draw_print_file.h"
#include "draw_dialog.h"
#include "draw_printing.h"
#include "draw_operation.h"
#include "draw_preHeat.h"
#include "draw_extrusion.h"
#include "draw_home.h"
#include "draw_gcode.h"
#include "draw_more.h"
#include "draw_move_motor.h"
#include "draw_fan.h"
#include "draw_about.h"
#include "draw_change_speed.h"
#include "draw_manuaLevel.h"
#include "draw_error_message.h"
#include "printer_operation.h"
#include "draw_machine_para.h"
#include "draw_machine_settings.h"
#include "draw_motor_settings.h"
#include "draw_advance_settings.h"
#include "draw_acceleration_settings.h"
#include "draw_number_key.h"
#include "draw_jerk_settings.h"
#include "draw_pause_position.h"
#include "draw_step_settings.h"
#include "draw_tmc_current_settings.h"
#include "draw_eeprom_settings.h"
#include "draw_max_feedrate_settings.h"
#include "draw_tmc_step_mode_settings.h"
#include "draw_level_settings.h"
#include "draw_tramming_pos_settings.h"
#include "draw_auto_level_offset_settings.h"
#include "draw_filament_change.h"
#include "draw_filament_settings.h"
#include "draw_homing_sensitivity_settings.h"
#include "draw_baby_stepping.h"
#include "draw_keyboard.h"
#include "draw_encoder_settings.h"

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(MKS_WIFI_MODULE)
  #include "wifiSerial.h"
  #include "wifi_module.h"
  #include "wifi_upload.h"
  #include "draw_wifi_settings.h"
  #include "draw_wifi.h"
  #include "draw_wifi_list.h"
  #include "draw_wifi_tips.h"
  #include "draw_cloud_bind.h"
#endif

#define ESP_WIFI          0x02
#define AP_MODEL          0x01
#define STA_MODEL         0x02

#define FILE_SYS_USB      0
#define FILE_SYS_SD       1

#define TICK_CYCLE 1

#define PARA_SEL_ICON_TEXT_COLOR  LV_COLOR_MAKE(0x4A, 0x52, 0xFF);

#define TFT35

#ifdef TFT35

  #define TFT_WIDTH         480
  #define TFT_HEIGHT        320

  #define titleHeight        36   // TFT_screen.title_high
  #define INTERVAL_H          2   // TFT_screen.gap_h // 2
  #define INTERVAL_V          2   // TFT_screen.gap_v // 2
  #define BTN_X_PIXEL       117   // TFT_screen.btn_x_pixel
  #define BTN_Y_PIXEL       140   // TFT_screen.btn_y_pixel

  #define SIMPLE_FIRST_PAGE_GRAP   30

  #define BUTTON_TEXT_Y_OFFSET    -20

  #define TITLE_XPOS          3    // TFT_screen.title_xpos
  #define TITLE_YPOS          5    // TFT_screen.title_ypos

  #define FILE_BTN_CNT        6

  #define OTHER_BTN_XPIEL   117
  #define OTHER_BTN_YPIEL    92

  #define FILE_PRE_PIC_X_OFFSET 8
  #define FILE_PRE_PIC_Y_OFFSET 0

  #define PREVIEW_LITTLE_PIC_SIZE  40910  // 400*100+9*101+1
  #define PREVIEW_SIZE      202720        // (PREVIEW_LITTLE_PIC_SIZE+800*200+201*9+1)

  // machine parameter ui
  #define PARA_UI_POS_X             10
  #define PARA_UI_POS_Y             50

  #define PARA_UI_SIZE_X            450
  #define PARA_UI_SIZE_Y            40

  #define PARA_UI_ARROW_V          12

  #define PARA_UI_BACL_POS_X        400
  #define PARA_UI_BACL_POS_Y        270

  #define PARA_UI_TURN_PAGE_POS_X   320
  #define PARA_UI_TURN_PAGE_POS_Y   270

  #define PARA_UI_VALUE_SIZE_X      370
  #define PARA_UI_VALUE_POS_X       400
  #define PARA_UI_VALUE_V           5

  #define PARA_UI_STATE_POS_X       380
  #define PARA_UI_STATE_V           2

  #define PARA_UI_VALUE_SIZE_X_2    200
  #define PARA_UI_VALUE_POS_X_2     320
  #define PARA_UI_VALUE_V_2         5

  #define PARA_UI_VALUE_BTN_X_SIZE  70
  #define PARA_UI_VALUE_BTN_Y_SIZE  28

  #define PARA_UI_BACK_BTN_X_SIZE   70
  #define PARA_UI_BACK_BTN_Y_SIZE   40

  #define QRCODE_X                  20
  #define QRCODE_Y                  40
  #define QRCODE_WIDTH              160

#else // ifdef TFT35

  #define TFT_WIDTH     320
  #define TFT_HEIGHT    240

#endif // ifdef TFT35

#ifdef __cplusplus
  extern "C" { /* C-declarations for C++ */
#endif

extern char public_buf_m[100];
extern char public_buf_l[30];

typedef struct {
  uint32_t  spi_flash_flag;
  uint8_t   disp_rotation_180;
  bool      multiple_language;
  uint8_t   language;
  uint8_t   leveling_mode;
  bool      from_flash_pic;
  bool      finish_power_off;
  bool      pause_reprint;
  uint8_t   wifi_mode_sel;
  uint8_t   fileSysType;
  uint8_t   wifi_type;
  bool      cloud_enable,
            encoder_enable;
  int       trammingPos[5][2]; // XY
  int       filamentchange_load_length,
            filamentchange_load_speed,
            filamentchange_unload_length,
            filamentchange_unload_speed;
  celsius_t filament_limit_temp;
  float     pausePosX, pausePosY, pausePosZ;
  uint32_t  curFilesize;
} CFG_ITMES;

typedef struct {
  uint8_t curTempType:1,
          extruderIndex:3,
          stepHeat:4,
          extruderIndexBak:4;
  bool    leveling_first_time:1,
          para_ui_page:1,
          configWifi:1,
          command_send:1,
          filament_load_heat_flg:1,
          filament_heat_completed_load:1,
          filament_unload_heat_flg:1,
          filament_heat_completed_unload:1,
          filament_loading_completed:1,
          filament_unloading_completed:1,
          filament_loading_time_flg:1,
          filament_unloading_time_flg:1;
  uint8_t wifi_name[32];
  uint8_t wifi_key[64];
  uint8_t cloud_hostUrl[96];
  uint8_t extruStep;
  uint8_t extruSpeed;
  uint8_t print_state;
  uint8_t stepPrintSpeed;
  uint8_t waitEndMoves;
  uint8_t dialogType;
  uint8_t F[4];
  uint8_t filament_rate;
  uint16_t moveSpeed;
  uint16_t cloud_port;
  uint16_t moveSpeed_bak;
  uint32_t totalSend;
  uint32_t filament_loading_time,
           filament_unloading_time,
           filament_loading_time_cnt,
           filament_unloading_time_cnt;
  float move_dist;
  celsius_t hotendTargetTempBak;
  float current_x_position_bak,
        current_y_position_bak,
        current_z_position_bak,
        current_e_position_bak;
} UI_CFG;

typedef enum {
  MAIN_UI,
  PRINT_READY_UI,
  PRINT_FILE_UI,
  PRINTING_UI,
  MOVE_MOTOR_UI,
  OPERATE_UI,
  PAUSE_UI,
  EXTRUSION_UI,
  FAN_UI,
  PRE_HEAT_UI,
  CHANGE_SPEED_UI,
  TEMP_UI,
  SET_UI,
  ZERO_UI,
  SPRAYER_UI,
  MACHINE_UI,
  LANGUAGE_UI,
  ABOUT_UI,
  LOG_UI,
  DISK_UI,
  CALIBRATE_UI,
  DIALOG_UI,
  WIFI_UI,
  MORE_UI,
  FILETRANSFER_UI,
  FILETRANSFERSTATE_UI,
  PRINT_MORE_UI,
  FILAMENTCHANGE_UI,
  LEVELING_UI,
  MESHLEVELING_UI,
  BIND_UI,
  #if HAS_BED_PROBE
    NOZZLE_PROBE_OFFSET_UI,
  #endif
  TOOL_UI,
  HARDWARE_TEST_UI,
  WIFI_LIST_UI,
  KEY_BOARD_UI,
  WIFI_TIPS_UI,
  MACHINE_PARA_UI,
  MACHINE_SETTINGS_UI,
  TEMPERATURE_SETTINGS_UI,
  MOTOR_SETTINGS_UI,
  MACHINETYPE_UI,
  STROKE_UI,
  HOME_DIR_UI,
  ENDSTOP_TYPE_UI,
  FILAMENT_SETTINGS_UI,
  LEVELING_SETTIGNS_UI,
  LEVELING_PARA_UI,
  DELTA_LEVELING_PARA_UI,
  MANUAL_LEVELING_POSIGION_UI,
  MAXFEEDRATE_UI,
  STEPS_UI,
  ACCELERATION_UI,
  JERK_UI,
  MOTORDIR_UI,
  HOMESPEED_UI,
  NOZZLE_CONFIG_UI,
  HOTBED_CONFIG_UI,
  ADVANCED_UI,
  DOUBLE_Z_UI,
  ENABLE_INVERT_UI,
  NUMBER_KEY_UI,
  BABY_STEP_UI,
  ERROR_MESSAGE_UI,
  PAUSE_POS_UI,
  TMC_CURRENT_UI,
  TMC_MODE_UI,
  EEPROM_SETTINGS_UI,
  WIFI_SETTINGS_UI,
  HOMING_SENSITIVITY_UI,
  ENCODER_SETTINGS_UI,
  TOUCH_CALIBRATION_UI,
  GCODE_UI,
} DISP_STATE;

typedef struct {
  DISP_STATE _disp_state[100];
  int _disp_index;
} DISP_STATE_STACK;

typedef struct {
  int16_t days;
  uint16_t hours;
  uint8_t minutes;
  volatile int8_t seconds;
  int8_t ms_10;
  int8_t start;
} PRINT_TIME;
extern PRINT_TIME print_time;

typedef enum {
  PrintAcceleration,
  RetractAcceleration,
  TravelAcceleration,
  XAcceleration,
  YAcceleration,
  ZAcceleration,
  E0Acceleration,
  E1Acceleration,

  XMaxFeedRate,
  YMaxFeedRate,
  ZMaxFeedRate,
  E0MaxFeedRate,
  E1MaxFeedRate,

  XJerk,
  YJerk,
  ZJerk,
  EJerk,

  Xstep,
  Ystep,
  Zstep,
  E0step,
  E1step,

  Xcurrent,
  Ycurrent,
  Zcurrent,
  E0current,
  E1current,

  pause_pos_x,
  pause_pos_y,
  pause_pos_z,

  level_pos_x1,
  level_pos_y1,
  level_pos_x2,
  level_pos_y2,
  level_pos_x3,
  level_pos_y3,
  level_pos_x4,
  level_pos_y4,
  level_pos_x5,
  level_pos_y5,
  #if HAS_BED_PROBE
    x_offset,
    y_offset,
    z_offset,
  #endif
  load_length,
  load_speed,
  unload_length,
  unload_speed,
  filament_temp,

  x_sensitivity,
  y_sensitivity,
  z_sensitivity,
  z2_sensitivity
} num_key_value_state;
extern num_key_value_state value;

typedef enum {
  wifiName,
  wifiPassWord,
  wifiConfig,
  autoLevelGcodeCommand,
  GCodeCommand,
} keyboard_value_state;
extern keyboard_value_state keyboard_value;

extern CFG_ITMES gCfgItems;
extern UI_CFG uiCfg;
extern DISP_STATE disp_state;
extern DISP_STATE last_disp_state;
extern DISP_STATE_STACK disp_state_stack;

extern lv_style_t tft_style_scr;
extern lv_style_t tft_style_label_pre;
extern lv_style_t tft_style_label_rel;
extern lv_style_t style_line;
extern lv_style_t style_para_value_pre;
extern lv_style_t style_para_value_rel;
extern lv_style_t style_num_key_pre;
extern lv_style_t style_num_key_rel;
extern lv_style_t style_num_text;
extern lv_style_t style_sel_text;
extern lv_style_t style_para_value;
extern lv_style_t style_para_back;
extern lv_style_t lv_bar_style_indic;
extern lv_style_t style_btn_pr;
extern lv_style_t style_btn_rel;

extern lv_point_t line_points[4][2];

extern void gCfgItems_init();
extern void ui_cfg_init();
extern void tft_style_init();
extern char *creat_title_text();
extern void preview_gcode_prehandle(char *path);
extern void update_spi_flash();
extern void update_gcode_command(int addr,uint8_t *s);
extern void get_gcode_command(int addr,uint8_t *d);
extern void lv_serial_capt_hook(void *, uint8_t);
extern void lv_eom_hook(void *);
#if HAS_GCODE_PREVIEW
  extern void disp_pre_gcode(int xpos_pixel, int ypos_pixel);
#endif
extern void GUI_RefreshPage();
extern void clear_cur_ui();
extern void draw_return_ui();
extern void sd_detection();
extern void gCfg_to_spiFlah();
extern void print_time_count();

extern void LV_TASK_HANDLER();
extern void lv_ex_line(lv_obj_t *line, lv_point_t *points);

#ifdef __cplusplus
  } /* C-declarations for C++ */
#endif

// Set the same image for both Released and Pressed
void lv_imgbtn_set_src_both(lv_obj_t *imgbtn, const void *src);

// Set label styles for Released and Pressed
void lv_imgbtn_use_label_style(lv_obj_t *imgbtn);

// Set label styles for Released and Pressed
void lv_btn_use_label_style(lv_obj_t *btn);

// Set the same style for both Released and Pressed
void lv_btn_set_style_both(lv_obj_t *btn, lv_style_t *style);

// Create a screen
lv_obj_t* lv_screen_create(DISP_STATE newScreenType, const char* title = nullptr);

// Create an empty label
lv_obj_t* lv_label_create_empty(lv_obj_t *par);

// Create a label with style and text
lv_obj_t* lv_label_create(lv_obj_t *par, const char *text);

// Create a label with style, position, and text
lv_obj_t* lv_label_create(lv_obj_t *par, lv_coord_t x, lv_coord_t y, const char *text);

// Create a button with callback, ID, and Style.
lv_obj_t* lv_btn_create(lv_obj_t *par, lv_event_cb_t cb, const int id, lv_style_t *style=&style_para_value);

// Create a button with callback and ID, with label style.
lv_obj_t* lv_label_btn_create(lv_obj_t *par, lv_event_cb_t cb, const int id=0);

// Create a button with callback and ID, with button style.
lv_obj_t* lv_button_btn_create(lv_obj_t *par, lv_event_cb_t cb, const int id=0);

// Create a button with position, size, callback, ID, and style.
lv_obj_t* lv_btn_create(lv_obj_t *par, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_event_cb_t cb, const int id, lv_style_t *style);

// Create a button with position, size, callback, and ID. Style set to style_para_value.
lv_obj_t* lv_btn_create(lv_obj_t *par, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_event_cb_t cb, const int id=0);

// Create a button with position, size, callback, and ID, with label style.
lv_obj_t* lv_label_btn_create(lv_obj_t *par, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_event_cb_t cb, const int id=0);

// Create a button with position, size, callback, and ID, with button style.
lv_obj_t* lv_button_btn_create(lv_obj_t *par, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_event_cb_t cb, const int id=0);

// Create a button with callback and ID. Style set to style_para_back.
lv_obj_t* lv_btn_create_back(lv_obj_t *par, lv_event_cb_t cb, const int id=0);

// Create a button with position, size, callback, and ID. Style set to style_para_back.
lv_obj_t* lv_btn_create_back(lv_obj_t *par, lv_coord_t x, lv_coord_t y, lv_coord_t w, lv_coord_t h, lv_event_cb_t cb, const int id=0);

// Create an image button with image, callback, and ID. Use label style.
lv_obj_t* lv_imgbtn_create(lv_obj_t *par, const char *img, lv_event_cb_t cb, const int id=0);

// Create an image button with image, position, callback, and ID. Use label style.
lv_obj_t* lv_imgbtn_create(lv_obj_t *par, const char *img, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id=0);

// Create a big image button with a label, follow the LVGL UI standard.
lv_obj_t* lv_big_button_create(lv_obj_t *par, const char *img, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id, bool centerLabel = false);

// Create a menu item, follow the LVGL UI standard.
lv_obj_t* lv_screen_menu_item(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id, const int index, bool drawArrow = true);
lv_obj_t* lv_screen_menu_item_1_edit(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id, const int index, const char *editValue);
lv_obj_t* lv_screen_menu_item_2_edit(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id, const int index, const char *editValue, const int idEdit2, const char *editValue2);
lv_obj_t* lv_screen_menu_item_onoff(lv_obj_t *par, const char *text, lv_coord_t x, lv_coord_t y, lv_event_cb_t cb, const int id, const int index, const bool curValue);
void lv_screen_menu_item_onoff_update(lv_obj_t *btn, const bool curValue);

#define _DIA_1(T)       (uiCfg.dialogType == DIALOG_##T)
#define DIALOG_IS(V...) DO(DIA,||,V)
