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

#include "tft_Language_en.h"
#include "tft_Language_s_cn.h"
#include "tft_Language_t_cn.h"
#include "tft_Language_ru.h"
#include "tft_Language_fr.h"
#include "tft_Language_sp.h"
#include "tft_Language_it.h"

extern void disp_language_init();

#define LANG_SIMPLE_CHINESE    1
#define LANG_COMPLEX_CHINESE   2
#define LANG_ENGLISH           3
#define LANG_JAPAN             4
#define LANG_GERMAN            5
#define LANG_FRENCH            6
#define LANG_RUSSIAN           7
#define LANG_KOREAN            8
#define LANG_TURKISH           9
#define LANG_SPANISH          10
#define LANG_GREEK            11
#define LANG_ITALY            12
#define LANG_PORTUGUESE       13

#define MULTI_LANGUAGE_ENABLE   1
#define MULTI_LANGUAGE_DISABLE  0

typedef struct machine_common_disp{
  const char *default_value;

  const char *next;
  const char *previous;

  const char *MachineConfigTitle;
  const char *MachineType;
  const char *Stroke;
  const char *HomeDir;
  const char *EndStopType;
  const char *FilamentConf;

  const char *MachineTypeConfTitle;
  const char *xyz;
  const char *delta;
  const char *corexy;

  const char *StrokeConfTitle;
  const char *xStroke;
  const char *yStroke;
  const char *zStroke;

  const char *xmin;
  const char *ymin;
  const char *zmin;

  const char *HomeDirConfTitle;
  const char *xHomeDir;
  const char *yHomeDir;
  const char *zHomeDir;
  const char *min;
  const char *max;

  const char *EndstopConfTitle;
  const char *xEndstop_min;
  const char *yEndstop_min;
  const char *zEndstop_min;
  const char *xEndstop_max;
  const char *yEndstop_max;
  const char *zEndstop_max;
  const char *FilamentEndstop;
  const char *LevelingEndstop;
  const char *opened;
  const char *closed;

  const char *FilamentConfTitle;
  const char *InLength;
  const char *InSpeed;
  const char *FilamentTemperature;
  const char *OutLength;
  const char *OutSpeed;

  const char *LevelingParaConfTitle;
  const char *LevelingParaConf;
  const char *LevelingManuPosConf;
  const char *LevelingAutoCommandConf;
  const char *LevelingAutoZoffsetConf;

  const char *LevelingSubConfTitle;
  const char *AutoLevelEnable;
  const char *BLtouchEnable;
  const char *ProbePort;
  const char *ProbeXoffset;
  const char *ProbeYoffset;
  const char *ProbeZoffset;
  const char *ProbeXYspeed;
  const char *ProbeZspeed;
  const char *enable;
  const char *disable;
  const char *locked;
  const char *z_min;
  const char *z_max;

  const char *LevelingSubDeltaConfTitle;
  const char *MachineRadius;
  const char *DiagonalRod;
  const char *PrintableRadius;
  const char *DeltaHeight;
  const char *SmoothRodOffset;
  const char *EffectorOffset;
  const char *CalibrationRadius;

  const char *LevelingSubXYZConfTitle;

  const char *TemperatureConfTitle;
  const char *NozzleConf;
  const char *HotBedConf;
  const char *PreheatTemperConf;

  const char *NozzleCnt;
  const char *NozzleConfTitle;
  const char *NozzleType;
  const char *NozzleAdjustType;
  const char *NozzleMinTemperature;
  const char *NozzleMaxTemperature;
  const char *Extrude_Min_Temper;

  const char *HotbedEnable;
  const char *HotbedConfTitle;
  const char *HotbedAjustType;
  const char *HotbedMinTemperature;
  const char *HotbedMaxTemperature;

  const char *MotorConfTitle;
  const char *MaxFeedRateConf;
  const char *AccelerationConf;
  const char *JerkConf;
  const char *StepsConf;
  const char *MotorDirConf;
  const char *HomeFeedRateConf;
  const char *TMCcurrentConf;
  const char *TMCStepModeConf;
  const char *HomingSensitivityConf;

  const char *MaxFeedRateConfTitle;
  const char *XMaxFeedRate;
  const char *YMaxFeedRate;
  const char *ZMaxFeedRate;
  const char *E0MaxFeedRate;
  const char *E1MaxFeedRate;

  const char *AccelerationConfTitle;
  const char *PrintAcceleration;
  const char *RetractAcceleration;
  const char *TravelAcceleration;
  const char *X_Acceleration;
  const char *Y_Acceleration;
  const char *Z_Acceleration;
  const char *E0_Acceleration;
  const char *E1_Acceleration;

  const char *JerkConfTitle;
  const char *X_Jerk;
  const char *Y_Jerk;
  const char *Z_Jerk;
  const char *E_Jerk;

  const char *StepsConfTitle;
  const char *X_Steps;
  const char *Y_Steps;
  const char *Z_Steps;
  const char *E0_Steps;
  const char *E1_Steps;

  const char *TmcCurrentConfTitle;
  const char *X_Current;
  const char *Y_Current;
  const char *Z_Current;
  const char *E0_Current;
  const char *E1_Current;

  const char *TmcStepModeConfTitle;
  const char *X_StepMode;
  const char *Y_StepMode;
  const char *Z_StepMode;
  const char *E0_StepMode;
  const char *E1_StepMode;

  const char *HomingSensitivityConfTitle;
  const char *X_Sensitivity;
  const char *Y_Sensitivity;
  const char *Z_Sensitivity;
  const char *Z2_Sensitivity;

  const char *MotorDirConfTitle;
  const char *X_MotorDir;
  const char *Y_MotorDir;
  const char *Z_MotorDir;
  const char *E0_MotorDir;
  const char *E1_MotorDir;
  const char *Invert_1;
  const char *Invert_0;

  const char *HomeFeedRateConfTitle;
  const char *XY_HomeFeedRate;
  const char *Y_HomeFeedRate;
  const char *Z_HomeFeedRate;

  const char *AdvancedConfTitle;
  const char *PwrOffDection;
  const char *PwrOffAfterPrint;
  const char *HaveUps;
  const char *Z2andZ2Endstop;
  const char *EnablePinsInvert;
  const char *PausePosition;
  const char *WifiSettings;
  const char *EncoderSettings;

  const char *Z2ConfTitle;
  const char *Z2Enable;
  const char *Z2EndstopEnable;
  const char *Z2Port;

  const char *EnablePinsInvertTitle;
  const char *XInvert;
  const char *YInvert;
  const char *ZInvert;
  const char *EInvert;

  const char *key_1;
  const char *key_2;
  const char *key_3;
  const char *key_4;
  const char *key_5;
  const char *key_6;
  const char *key_7;
  const char *key_8;
  const char *key_9;
  const char *key_0;
  const char *key_point;
  const char *key_back;
  const char *key_reset;
  const char *key_confirm;
  const char *negative;
  const char *low_level;
  const char *high_level;

  const char *PausePosText;
  const char *xPos;
  const char *yPos;
  const char *zPos;

  const char *WifiConfTitle;
  const char *wifiMode;
  const char *wifiName;
  const char *wifiPassWord;
  const char *wifiCloud;
  const char *wifiConfig;
  const char *wifiEdit;
  const char *wifiConfigTips;

  const char *OffsetConfTitle;
  const char *Xoffset;
  const char *Yoffset;
  const char *Zoffset;

  const char *EncoderConfTitle;
  const char *EncoderConfText;

} machine_common_def;

extern machine_common_def machine_menu;

typedef struct common_menu_disp {
  const char *text_back;
  const char *dialog_confirm_title;
  const char *close_machine_tips;
  const char *unbind_printer_tips;
  const char *print_special_title;
  const char *pause_special_title;
  const char *operate_special_title;
  const char *next;
  const char *previous;
} common_menu_def;

extern common_menu_def common_menu;

typedef struct main_menu_disp {
  const char *title;
  const char *preheat;
  const char *move;
  const char *home;
  const char *print;
  const char *extrude;
  const char *leveling;
  const char *autoleveling;
  const char *fan;
  const char *set;
  const char *tool;
  const char *more;
  const char *machine_para;
} main_menu_def;

extern main_menu_def main_menu;

typedef struct preheat_menu_disp {
  const char *adjust_title;
  const char *title;
  const char *add;
  const char *dec;
  const char *ext1;
  const char *ext2;
  const char *hotbed;
  const char *off;
  const char *step_1c;
  const char *step_5c;
  const char *step_10c;
  const char *back;

  const char *value_state;

  const char *dialog_tips;

} preheat_menu_def;

extern preheat_menu_def preheat_menu;

typedef struct move_menu_disp {
  const char *title;
  const char *x_add;
  const char *x_dec;
  const char *y_add;
  const char *y_dec;
  const char *z_add;
  const char *z_dec;
  const char *step_001mm;
  const char *step_005mm;
  const char *step_01mm;
  const char *step_1mm;
  const char *step_10mm;
  const char *back;
} move_menu_def;

extern move_menu_def move_menu;

typedef struct home_menu_disp {
  const char *title;
  const char *home_all;
  const char *home_x;
  const char *home_y;
  const char *home_z;
  const char *stopmove;
  const char *back;
} home_menu_def;

extern home_menu_def home_menu;

typedef struct file_menu_disp {
  const char *title;
  const char *page_up;
  const char *page_down;
  const char *back;

  const char *file_loading;
  const char *no_file;
  const char *no_file_and_check;

} file_menu_def;

extern file_menu_def file_menu;

typedef struct extrude_menu_disp {
  const char *title;
  const char *in;
  const char *out;
  const char *ext1;
  const char *ext2;
  const char *step_1mm;
  const char *step_5mm;
  const char *step_10mm;
  const char *low;
  const char *normal;
  const char *high;
  const char *back;

  const char *count_value_mm;
  const char *count_value_cm;
  const char *count_value_m;
  const char *temp_value;
  const char *temper_text;
} extrude_menu_def;

extern extrude_menu_def extrude_menu;

typedef struct leveling_menu_disp {
  const char *title;
  const char *position1;
  const char *position2;
  const char *position3;
  const char *position4;
  const char *position5;

  char *back;
} leveling_menu_def;

extern leveling_menu_def leveling_menu;

typedef struct set_menu_disp {
  const char *title;
  const char *filesys;
  const char *wifi;
  const char *about;
  const char *fan;
  const char *filament;
  const char *breakpoint;
  const char *motoroff;
  const char *motoroffXY;
  const char *shutdown;
  const char *language;
  const char *machine_para;
  const char *eepromSet;
  const char *back;
} set_menu_def;

extern set_menu_def set_menu;

typedef struct filesys_menu_disp {
  const char *title;
  const char *filesys;
  const char *sd_sys;
  const char *usb_sys;
  const char *back;
} filesys_menu_def;

extern filesys_menu_def filesys_menu;

typedef struct more_menu_disp {
  const char *title;
  const char *custom1;
  const char *custom2;
  const char *custom3;
  const char *custom4;
  const char *custom5;
  const char *custom6;
  const char *custom7;
  const char *back;
} more_menu_def;

extern more_menu_def more_menu;

typedef struct wifi_menu_disp {
  const char *title;
  const char *ip;
  const char *wifi;
  const char *key;
  const char *state_ap;
  const char *state_sta;
  const char *cloud;
  const char *connected;
  const char *disconnected;
  const char *exception;
  const char *back;
  const char *reconnect;
} wifi_menu_def;

extern wifi_menu_def wifi_menu;

typedef struct cloud_menu_disp {
  const char *title;
  const char *unbind;
  const char *unbinding;
  const char *unbinded;
  const char *bind;
  const char *binding;
  const char *binded;
  const char *disable;
  const char *disconnected;
  const char *back;
  const char *unbind_printer_tips;
} cloud_menu_def;

extern cloud_menu_def cloud_menu;

typedef struct about_menu_disp {
  const char *title;
  const char *type_name;
  const char *firmware_v;
  const char *type;
  const char *version;
  const char *wifi;
  const char *type_robin;
  const char *type_robin_mini;
  const char *back;
} about_menu_def;

extern about_menu_def about_menu;

typedef struct fan_menu_disp {
  const char *title;
  const char *add;
  const char *dec;
  const char *full;
  const char *half;
  const char *off;
  const char *back;

  const char *state;
  const char *state_value;
} fan_menu_def;

extern fan_menu_def fan_menu;

typedef struct filament_menu_disp {
  const char *title;
  const char *in;
  const char *out;
  const char *ext1;
  const char *ext2;
  const char *back;
  const char *stat_temp;
  const char *ready_replace;
  const char *replacing;
  const char *loading;
  const char *unloading;
  const char *heating;
  const char *complete_and_back;
  const char *filament_dialog_load_heat;
  const char *filament_dialog_unload_heat;
  const char *filament_dialog_load_heat_confirm;
  const char *filament_dialog_unload_heat_confirm;
  const char *filament_dialog_loading;
  const char *filament_dialog_unloading;
  const char *filament_dialog_load_completed;
  const char *filament_dialog_unload_completed;
  const char *filament_dialog_ok;
  const char *filament_dialog_back;
} filament_menu_def;

extern filament_menu_def filament_menu;

typedef struct language_menu {
  const char *title;
  const char *chinese_s;
  const char *chinese_t;
  const char *english;
  const char *russian;
  const char *japan;
  const char *italy;
  const char *german;
  const char *spanish;
  const char *korean;
  const char *french;
  const char *brazil;
  const char *portuguese;
  const char *next;
  const char *up;
  const char *back;
} language_menu_def;

extern language_menu_def language_menu;

typedef struct printing_menu_disp {
  const char *title;
  const char *option;
  const char *temp1;
  const char *temp2;
  const char *bed_temp;
  const char *fan_speed;
  const char *pause;
  const char *resume;
  const char *stop;
} printing_menu_def;

extern printing_menu_def printing_menu;

typedef struct operation_menu_disp {
  const char *title;
  const char *pause;
  const char *stop;
  const char *temp;
  const char *fan;
  const char *filament;
  const char *extr;
  const char *speed;
  const char *move;
  const char *more;
  const char *auto_off;
  const char *manual_off;
  const char *back;
  const char *babystep;
} operation_menu_def;

extern operation_menu_def operation_menu;

typedef struct pause_menu_disp {
  const char *title;
  const char *resume;
  const char *stop;
  const char *extrude;
  const char *move;
  const char *filament;
  const char *more;
} pause_menu_def;

extern pause_menu_def pause_menu;

typedef struct speed_menu_disp {
  const char *title;
  const char *add;
  const char *dec;
  const char *extrude;
  const char *move;
  const char *step_1percent;
  const char *step_5percent;
  const char *step_10percent;
  const char *back;
  const char *move_speed;
  const char *extrude_speed;
} speed_menu_def;

extern speed_menu_def speed_menu;

typedef struct printing_more_menu_disp {
  const char *title;
  const char *fan;
  const char *auto_close;
  const char *manual;
  const char *temp;
  const char *speed;
  const char *back;
} printing_more_menu_def;

extern printing_more_menu_def printing_more_menu;

typedef struct dialog_menu_disp {
  const char *confirm_title;

  const char *error1_repint_no_file;
  const char *error2_communication_fail;
  const char *error3_filename_too_long;
  const char *error4_no_file;
  const char *error5_check_filesys;

  const char *tip1_print_file;
  const char *tip2_stop_file;
} dialog_menu_def;

extern dialog_menu_def dialog_menu;

typedef struct print_file_dialog_disp {
  const char *title;
  const char *confirm;
  const char *cancel;
  const char *print_file;
  const char *cancel_print;
  const char *retry;
  const char *stop;
  const char *no_file_print_tips;
  const char *print_from_breakpoint;
  const char *file_name_too_long_error;
  const char *close_machine_error;
  const char *filament_no_press;
  const char *print_finish;
  const char *print_time;
  const char *reprint;
  const char *wifi_enable_tips;
} print_file_dialog_menu_def;

extern print_file_dialog_menu_def print_file_dialog_menu;

typedef struct tool_menu_disp {
  const char *title;
  const char *preheat;
  const char *extrude;
  const char *move;
  const char *home;
  const char *leveling;
  const char *autoleveling;
  const char *filament;
  const char *more;
  const char *back;
} tool_menu_def;

extern tool_menu_def tool_menu;

typedef struct MachinePara_menu_disp {
  const char *title;
  const char *MachineSetting;
  const char *MotorSetting;
  const char *leveling;
  const char *AdvanceSetting;
} MachinePara_menu_def;

extern MachinePara_menu_def MachinePara_menu;

typedef struct pause_msg_disp {
  const char *pausing;
  const char *changing;
  const char *unload;
  const char *waiting;
  const char *insert;
  const char *load;
  const char *purge;
  const char *resume;
  const char *heat;
  const char *heating;
  const char *option;
  const char *purgeMore;
  const char *continuePrint;
} pause_msg_def;

extern pause_msg_def pause_msg_menu;

typedef struct eeprom_disp{
  const char *title;
  const char *store;
  const char *read;
  const char *revert;
  const char *storeTips;
  const char *readTips;
  const char *revertTips;
} eeprom_def;

extern eeprom_def eeprom_menu;
/*****************************************/
//
#define TEXT_VALUE          "%d/%d"

#define TEXT_VALUE_T        ": %d℃"
#define TEXT_VALUE_mm       ": %dmm"
#define TEXT_VALUE_cm       ": %dcm"
#define TEXT_VALUE_m        ": %dm"

#define TEMP_UNIT_SYBOL       "%d℃"
#define FLOAT_TEMP_UNIT_SYBOL "%.1f℃"

#define TEXT_1C             "1℃"
#define TEXT_5C             "5℃"
#define TEXT_10C            "10℃"

#define AXIS_X_ADD_TEXT     "X+"
#define AXIS_X_DEC_TEXT     "X-"
#define AXIS_Y_ADD_TEXT     "Y+"
#define AXIS_Y_DEC_TEXT     "Y-"
#define AXIS_Z_ADD_TEXT     "Z+"
#define AXIS_Z_DEC_TEXT     "Z-"
#define TEXT_001MM          "0.01 mm"
#define TEXT_005MM          "0.05 mm"
#define TEXT_01MM           "0.1 mm"
#define TEXT_1MM            "1 mm"
#define TEXT_10MM           "10 mm"

#define EXTRUDE_1MM_TEXT    "1 mm"
#define EXTRUDE_5MM_TEXT    "5 mm"
#define EXTRUDE_10MM_TEXT   "10 mm"

#define STEP_1PERCENT       "1%"
#define STEP_5PERCENT       "5%"
#define STEP_10PERCENT      "10%"

#define LANGUAGE_S_CN       "简体"
#define LANGUAGE_T_CN       "繁体"
#define LANGUAGE_EN         "English"
#define LANGUAGE_JP         "日本語"
#define LANGUAGE_GE         "Deutsch"
#define LANGUAGE_FR         "français"
#define LANGUAGE_IT         "Italiano"
#define LANGUAGE_PR         "português"
#define LANGUAGE_KR         "Korean"
#define LANGUAGE_BR         "Brazil"
#define LANGUAGE_RU         "русский"
#define LANGUAGE_SP         "español"

#define HOME_X_TEXT         "X"
#define HOME_Y_TEXT         "Y"
#define HOME_Z_TEXT         "Z"
#define HOME_ALL_TEXT       "All"

#define ABOUT_TYPE_TEXT     "MKS Robin Pro"

#define ABOUT_VERSION_TEXT  "1.0.0"

#define FAN_OPEN_TEXT       "100%"
#define FAN_HALF_TEXT       "50%"
#define FAN_CLOSE_TEXT      "0%"

#define WIFI_TEXT                       "WIFI"
#define WIFI_IP_TEXT                    "IP: "
#define WIFI_NAME_TEXT                  "WiFi: "
#define WIFI_KEY_TEXT                   "Key: "
#define WIFI_STATE_AP_TEXT              "State: AP"
#define WIFI_STATE_STA_TEXT             "State: STA"
#define WIFI_CONNECTED_TEXT             "Connected"
#define WIFI_DISCONNECTED_TEXT          "Disconnected"
#define WIFI_EXCEPTION_TEXT             "Exception"

#define FILAMENT_TIPS2_TEXT             "T:"

#define DIALOG_UPLOAD_ING_EN            "Uploading......"
#define DIALOG_UPLOAD_ERROR_EN          "Upload error"
#define DIALOG_UPLOAD_FINISH_EN         "Upload finished"
#define DIALOG_UPLOAD_SIZE_EN           "Size"
#define DIALOG_UPLOAD_TIME_EN           "Time"
#define DIALOG_UPLOAD_SPEED_EN          "Speed"
#define DIALOG_UPDATE_WIFI_FIRMWARE_EN  "Updating wifi model firmware"
#define DIALOG_UPDATE_WIFI_WEB_EN       "Updating wifi model web data"
#define DIALOG_UPDATE_NO_DEVICE_EN      "Please check whether\nmemory device inserted!"

#define ZOFFSET_STEP001                 "0.01 mm"
#define ZOFFSET_STEP01                  "0.1 mm"
#define ZOFFSET_STEP1                   "1 mm"
