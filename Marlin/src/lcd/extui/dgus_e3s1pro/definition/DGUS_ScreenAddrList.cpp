/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2023 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#if ENABLED(DGUS_LCD_UI_E3S1PRO)

#include "DGUS_ScreenAddrList.h"

#include "../../ui_api.h"

constexpr DGUS_Addr LIST_BOOT[] PROGMEM = {
  DGUS_Addr::BOOT_Logo,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_HOME[] PROGMEM = {
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_Icon_Percentage,
  DGUS_Addr::MAIN_ElapsedHours,
  DGUS_Addr::MAIN_ElapsedMinutes,
  DGUS_Addr::MAIN_PrintPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_RemainingHours,
  DGUS_Addr::MAIN_RemainingMinutes,
  DGUS_Addr::MAIN_StatusMessage,
  DGUS_Addr::MENU_Icon_Home_S,
  DGUS_Addr::MENU_Icon_Print,
  DGUS_Addr::MENU_Icon_Ready,
  DGUS_Addr::MENU_Icon_Settings,
  DGUS_Addr::FINISH_Icon_Finished,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PRINTING[] PROGMEM = {
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_Icon_Percentage,
  DGUS_Addr::MAIN_ElapsedHours,
  DGUS_Addr::MAIN_ElapsedMinutes,
  DGUS_Addr::MAIN_PrintPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_RemainingHours,
  DGUS_Addr::MAIN_RemainingMinutes,
  DGUS_Addr::MAIN_StatusMessage,
  DGUS_Addr::MENU_Icon_Home,
  DGUS_Addr::MENU_Icon_Print_S,
  DGUS_Addr::MENU_Icon_Ready,
  DGUS_Addr::MENU_Icon_Settings,
  DGUS_Addr::FINISH_Icon_Finished,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_RUNOUT[] PROGMEM = {
  DGUS_Addr::RESET_Icon_Yes,
  DGUS_Addr::RUNOUT_Icon_Stop,
  DGUS_Addr::RUNOUT_Icon_Message,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FILAMENTLOAD[] PROGMEM = {
  DGUS_Addr::LOAD_Icon_Continue,
  DGUS_Addr::RESET_Icon_No,
  DGUS_Addr::LOAD_Icon_Message,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PAUSE_STOP[] PROGMEM = {
  DGUS_Addr::RESET_Icon_Yes,
  DGUS_Addr::RESET_Icon_No,
  DGUS_Addr::PAUSE_Icon_Message,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_CONTINUE_STOP[] PROGMEM = {
  DGUS_Addr::RESET_Icon_Yes,
  DGUS_Addr::RESET_Icon_No,
  DGUS_Addr::RESUME_Icon_Message,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_MOVEAXIS[] PROGMEM = {
  DGUS_Addr::AXIS_StepperStatus,
  DGUS_Addr::AXIS_X,
  DGUS_Addr::AXIS_Y,
  DGUS_Addr::AXIS_Z,
  DGUS_Addr::MENU_Icon_Home,
  DGUS_Addr::MENU_Icon_Print_S,
  DGUS_Addr::MENU_Icon_Ready_S,
  DGUS_Addr::MENU_Icon_Settings,
  DGUS_Addr::AXIS_Icon_MoveAxis,
  DGUS_Addr::AXIS_Icon_IO,
  DGUS_Addr::AXIS_Icon_Manual,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_IO[] PROGMEM = {
  DGUS_Addr::AXIS_Icon_MoveAxis,
  DGUS_Addr::AXIS_Icon_IO,
  DGUS_Addr::AXIS_Icon_Manual,
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MENU_Icon_Home,
  DGUS_Addr::MENU_Icon_Print_S,
  DGUS_Addr::MENU_Icon_Ready_S,
  DGUS_Addr::MENU_Icon_Settings,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_CONTROL[] PROGMEM = {
  DGUS_Addr::AXIS_Icon_MoveAxis,
  DGUS_Addr::AXIS_Icon_IO,
  DGUS_Addr::AXIS_Icon_Manual,
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::IO_Icon_ExtruderTemp,
  DGUS_Addr::CONTROL_Icon_BedTemp,
  DGUS_Addr::AXIS_Icon_IO, // bogus, it's for "PLA temp" icon
  DGUS_Addr::AXIS_Icon_Manual, // bogus, it's for "ABS temp" icon
  DGUS_Addr::CONTROL_FanSpeed,
  DGUS_Addr::ADJUST_Icon_FanControl,
  DGUS_Addr::CONTROL_Icon_Cooling,
  DGUS_Addr::MENU_Icon_Home,
  DGUS_Addr::MENU_Icon_Print_S,
  DGUS_Addr::MENU_Icon_Ready_S,
  DGUS_Addr::MENU_Icon_Settings,
  DGUS_Addr::END
};


constexpr DGUS_Addr LIST_ADJUST[] PROGMEM = {
  DGUS_Addr::ADJUST_Icon_Adjust,
  DGUS_Addr::IO_Icon_ExtruderTemp,
  DGUS_Addr::CONTROL_Icon_BedTemp,
  DGUS_Addr::ADJUST_Icon_Feedrate,
  DGUS_Addr::ADJUST_Icon_FanControl,
  DGUS_Addr::ADJUST_Icon_ZOffset,
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::CONTROL_FanSpeed,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_ADJUST_ZOFFSET[] PROGMEM = {
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_AUTOHOME[] PROGMEM = {
  DGUS_Addr::AUTOHOME_Icon_WaitMessage,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_ABNORMAL[] PROGMEM = {
  DGUS_Addr::ABNORMAL_StatusMessage,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_SETTINGS[] PROGMEM = {
  DGUS_Addr::MENU_Icon_Home,
  DGUS_Addr::MENU_Icon_Print_S,
  DGUS_Addr::MENU_Icon_Ready_S,
  DGUS_Addr::MENU_Icon_Settings_S,
  DGUS_Addr::SETTINGS_Icon_Language,
  DGUS_Addr::SETTINGS_Icon_Leveling,
  DGUS_Addr::SETTINGS_Icon_Device,
  DGUS_Addr::SETTINGS_Icon_Advanced,
  DGUS_Addr::SETTINGS_Icon_About,
  DGUS_Addr::SETTINGS_Icon_PLA,
  DGUS_Addr::SETTINGS_Icon_ABS,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_TEMPERATURE_PRESETS[] PROGMEM = {
  DGUS_Addr::SETTINGS_Icon_PLA,
  DGUS_Addr::SETTINGS_Icon_ABS,
  DGUS_Addr::IO_Icon_ExtruderTemp,
  DGUS_Addr::CONTROL_Icon_BedTemp,
  DGUS_Addr::TEMP_PLA_ExtruderTemp,
  DGUS_Addr::TEMP_PLA_BedTemp,
  DGUS_Addr::TEMP_ABS_ExtruderTemp,
  DGUS_Addr::TEMP_ABS_BedTemp,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_TRAMMING[] PROGMEM = {
  DGUS_Addr::LEVELING_Icon_Leveling,
  DGUS_Addr::LEVELING_Icon_AutoLevel,
  DGUS_Addr::LEVELING_Icon_AuxLevel_S,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_AUTOLEVEL[] PROGMEM = {
  DGUS_Addr::LEVELING_Icon_Start,
  DGUS_Addr::LEVELING_Icon_Leveling,
  DGUS_Addr::LEVELING_Icon_AutoLevel_S,
  DGUS_Addr::LEVELING_Icon_AuxLevel,
  DGUS_Addr::LEVELING_Progress_Icon,
  DGUS_Addr::LEVELING_Progress,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_POWERLOSS[] PROGMEM = {
  DGUS_Addr::POWERLOSS_Icon_Message,
  DGUS_Addr::LOAD_Icon_Continue,
  DGUS_Addr::RESET_Icon_No,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_LANGUAGE[] PROGMEM = {
  DGUS_Addr::LANGUAGE_Icon_Language,
  DGUS_Addr::LANGUAGE_Icon_Chinese,
  DGUS_Addr::LANGUAGE_Icon_English,
  DGUS_Addr::LANGUAGE_Icon_German,
  DGUS_Addr::LANGUAGE_Icon_Spanish,
  DGUS_Addr::LANGUAGE_Icon_French,
  DGUS_Addr::LANGUAGE_Icon_Italian,
  DGUS_Addr::LANGUAGE_Icon_Portuguese,
  DGUS_Addr::LANGUAGE_Icon_Russian,
  DGUS_Addr::LANGUAGE_Icon_Turkish,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_INFO[] PROGMEM = {
  DGUS_Addr::MENU_Icon_Home,
  DGUS_Addr::MENU_Icon_Print_S,
  DGUS_Addr::MENU_Icon_Ready_S,
  DGUS_Addr::MENU_Icon_Settings_S,
  DGUS_Addr::INFO_Icon_Device,
  DGUS_Addr::SETTINGS_Icon_Advanced,
  DGUS_Addr::INFO_Icon_About,
  DGUS_Addr::INFO_Icon_Model,
  DGUS_Addr::INFO_Icon_FW_Version,
  DGUS_Addr::INFO_Icon_Screen_Version,
  DGUS_Addr::INFO_Icon_HW_Version,
  DGUS_Addr::INFO_Icon_Print_Size,
  DGUS_Addr::INFO_Icon_Website,
  DGUS_Addr::INFO_Model,
  DGUS_Addr::INFO_FW_Version,
  DGUS_Addr::INFO_Screen_Version,
  DGUS_Addr::INFO_HW_Version,
  DGUS_Addr::INFO_Print_Size,
  DGUS_Addr::INFO_Website,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_ADV_SETTINGS[] PROGMEM = {
  DGUS_Addr::MENU_Icon_Home,
  DGUS_Addr::MENU_Icon_Print_S,
  DGUS_Addr::MENU_Icon_Ready_S,
  DGUS_Addr::MENU_Icon_Settings_S,
  DGUS_Addr::SETTINGS_Icon_Device,
  DGUS_Addr::SETTINGS_Icon_Advanced,
  DGUS_Addr::INFO_Icon_About,
  DGUS_Addr::ADV_Icon_Movement,
  DGUS_Addr::ADV_Icon_ResetSettings,
  DGUS_Addr::ADV_Icon_PID,
  DGUS_Addr::ADV_Icon_SwitchToLaser,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_MOTION[] PROGMEM = {
  DGUS_Addr::ADV_Icon_MotionSettings,
  DGUS_Addr::ADV_Icon_MaxFeedrate,
  DGUS_Addr::ADV_Icon_Acceleration,
  DGUS_Addr::ADV_Icon_Jerk,
  DGUS_Addr::ADV_Icon_StepsPerMm,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_MAX_FEEDRATE[] PROGMEM = {
  DGUS_Addr::ADV_Icon_MaxSpeed,
  DGUS_Addr::ADV_Icon_MaxSpeed_X,
  DGUS_Addr::ADV_Icon_MaxSpeed_Y,
  DGUS_Addr::ADV_Icon_MaxSpeed_Z,
  DGUS_Addr::ADV_Icon_MaxSpeed_E,
  DGUS_Addr::MOTION_MaxSpeed_X,
  DGUS_Addr::MOTION_MaxSpeed_Y,
  DGUS_Addr::MOTION_MaxSpeed_Z,
  DGUS_Addr::MOTION_MaxSpeed_E,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_ACCELERATION[] PROGMEM = {
  DGUS_Addr::ADV_Icon_AccelerationMenu,
  DGUS_Addr::ADV_Icon_Acceleration_X,
  DGUS_Addr::ADV_Icon_Acceleration_Y,
  DGUS_Addr::ADV_Icon_Acceleration_Z,
  DGUS_Addr::ADV_Icon_Acceleration_E,
  DGUS_Addr::MOTION_Acceleration_X,
  DGUS_Addr::MOTION_Acceleration_Y,
  DGUS_Addr::MOTION_Acceleration_Z,
  DGUS_Addr::MOTION_Acceleration_E,

  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_JERK[] PROGMEM = {
  DGUS_Addr::ADV_Icon_MaxJerkMenu,
  DGUS_Addr::ADV_Icon_MaxJerk_X,
  DGUS_Addr::ADV_Icon_MaxJerk_Y,
  DGUS_Addr::ADV_Icon_MaxJerk_Z,
  DGUS_Addr::ADV_Icon_MaxJerk_E,
  DGUS_Addr::MOTION_MaxJerk_X,
  DGUS_Addr::MOTION_MaxJerk_Y,
  DGUS_Addr::MOTION_MaxJerk_Z,
  DGUS_Addr::MOTION_MaxJerk_E,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_STEPSMM[] PROGMEM = {
  DGUS_Addr::ADV_Icon_StepsPerMmMenu,
  DGUS_Addr::ADV_Icon_StepsPerMm_X,
  DGUS_Addr::ADV_Icon_StepsPerMm_Y,
  DGUS_Addr::ADV_Icon_StepsPerMm_Z,
  DGUS_Addr::ADV_Icon_StepsPerMm_E,
  DGUS_Addr::MOTION_StepsPerMm_X,
  DGUS_Addr::MOTION_StepsPerMm_Y,
  DGUS_Addr::MOTION_StepsPerMm_Z,
  DGUS_Addr::MOTION_StepsPerMm_E,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PIDCONTROL[] PROGMEM = {
  DGUS_Addr::PID_Icon_PIDMenu,
  DGUS_Addr::PID_Icon_Extruder_P,
  DGUS_Addr::PID_Icon_Extruder_I,
  DGUS_Addr::PID_Icon_Extruder_D,
  DGUS_Addr::PID_Icon_Bed_P,
  DGUS_Addr::PID_Icon_Bed_I,
  DGUS_Addr::PID_Icon_Bed_D,
  DGUS_Addr::PID_Extruder_P,
  DGUS_Addr::PID_Extruder_I,
  DGUS_Addr::PID_Extruder_D,
  DGUS_Addr::PID_Bed_P,
  DGUS_Addr::PID_Bed_I,
  DGUS_Addr::PID_Bed_D,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_RESET_CONFIRM[] PROGMEM = {
  DGUS_Addr::RESET_Icon_InfoMessage,
  DGUS_Addr::RESET_Icon_Yes,
  DGUS_Addr::RESET_Icon_No,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FILAMENTCHECK[] PROGMEM = {
  DGUS_Addr::FILAMENT_Icon_Inserted,
  DGUS_Addr::RESET_Icon_Yes,
  DGUS_Addr::RESET_Icon_No,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_SDCARDCHECK[] PROGMEM = {
  DGUS_Addr::RESET_Icon_Yes,
  DGUS_Addr::RESET_Icon_No,
  DGUS_Addr::SDCard_InsertionStatus,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PRINTMODE[] PROGMEM = {
  DGUS_Addr::PRTMODE_Icon_Message,
  DGUS_Addr::PRTMODE_Icon_FDM,
  DGUS_Addr::PRTMODE_Icon_Cutter,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PRINTMODE_CONFIRM[] PROGMEM = {
  DGUS_Addr::CUTTER_Icon_Message,
  DGUS_Addr::FDM_Icon_Message,
  DGUS_Addr::RESET_Icon_Yes,
  DGUS_Addr::RESET_Icon_No,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_SDCARD[] PROGMEM = {
  DGUS_Addr::MENU_Icon_Home_S,
  DGUS_Addr::MENU_Icon_Print_S,
  DGUS_Addr::MENU_Icon_Ready,
  DGUS_Addr::MENU_Icon_Settings,
  DGUS_Addr::SDCARD_Filename1,
  DGUS_Addr::SDCARD_Filename2,
  DGUS_Addr::SDCARD_Filename3,
  DGUS_Addr::SDCARD_Filename4,
  DGUS_Addr::SDCARD_Filename5,
  DGUS_Addr::SDCARD_Filename6,
  DGUS_Addr::SDCARD_Filename7,
  DGUS_Addr::SDCARD_Filename8,
  DGUS_Addr::SDCARD_Filename9,
  DGUS_Addr::SDCARD_Filename10,
  DGUS_Addr::SDCARD_Filename11,
  DGUS_Addr::SDCARD_Filename12,
  DGUS_Addr::SDCARD_Filename13,
  DGUS_Addr::SDCARD_Filename14,
  DGUS_Addr::SDCARD_Filename15,
  DGUS_Addr::SDCARD_Filename16,
  DGUS_Addr::SDCARD_Filename17,
  DGUS_Addr::SDCARD_Filename18,
  DGUS_Addr::SDCARD_Filename19,
  DGUS_Addr::SDCARD_Filename20,
  DGUS_Addr::SDCARD_Selection_File1,
  DGUS_Addr::SDCARD_Selection_File2,
  DGUS_Addr::SDCARD_Selection_File3,
  DGUS_Addr::SDCARD_Selection_File4,
  DGUS_Addr::SDCARD_Selection_File5,
  DGUS_Addr::SDCARD_Selection_File6,
  DGUS_Addr::SDCARD_Selection_File7,
  DGUS_Addr::SDCARD_Selection_File8,
  DGUS_Addr::SDCARD_Selection_File9,
  DGUS_Addr::SDCARD_Selection_File10,
  DGUS_Addr::SDCARD_Selection_File11,
  DGUS_Addr::SDCARD_Selection_File12,
  DGUS_Addr::SDCARD_Selection_File13,
  DGUS_Addr::SDCARD_Selection_File14,
  DGUS_Addr::SDCARD_Selection_File15,
  DGUS_Addr::SDCARD_Selection_File16,
  DGUS_Addr::SDCARD_Selection_File17,
  DGUS_Addr::SDCARD_Selection_File18,
  DGUS_Addr::SDCARD_Selection_File19,
  DGUS_Addr::SDCARD_Selection_File20,
  DGUS_Addr::END
};

#define MAP_HELPER(SCREEN, LIST) \
  { .screen = SCREEN, \
  .addr_list = LIST }

const struct DGUS_ScreenAddrList screen_addr_list_map[] PROGMEM = {
  MAP_HELPER(DGUS_ScreenID::BOOT,                 LIST_BOOT),
  MAP_HELPER(DGUS_ScreenID::HOME,                 LIST_HOME),
  MAP_HELPER(DGUS_ScreenID::FILE1,                LIST_SDCARD),
  MAP_HELPER(DGUS_ScreenID::FILE2,                LIST_SDCARD),
  MAP_HELPER(DGUS_ScreenID::FILE3,                LIST_SDCARD),
  MAP_HELPER(DGUS_ScreenID::FILE4,                LIST_SDCARD),
  MAP_HELPER(DGUS_ScreenID::FILAMENTUSEUP,        LIST_RUNOUT),
  MAP_HELPER(DGUS_ScreenID::FILAMENTLOAD,         LIST_FILAMENTLOAD),
  MAP_HELPER(DGUS_ScreenID::FINISH,               LIST_PRINTING),
  MAP_HELPER(DGUS_ScreenID::PAUSE,                LIST_PRINTING),
  MAP_HELPER(DGUS_ScreenID::PAUSE_STOP,           LIST_PAUSE_STOP),
  MAP_HELPER(DGUS_ScreenID::RESUME,               LIST_PRINTING),
  MAP_HELPER(DGUS_ScreenID::CONTINUE_STOP,        LIST_CONTINUE_STOP),
  MAP_HELPER(DGUS_ScreenID::ADJUST,               LIST_ADJUST),
  MAP_HELPER(DGUS_ScreenID::PREPARE,              LIST_ADJUST_ZOFFSET),
  MAP_HELPER(DGUS_ScreenID::MOVEAXIS_01,          LIST_MOVEAXIS),
  MAP_HELPER(DGUS_ScreenID::MOVEAXIS_1,           LIST_MOVEAXIS),
  MAP_HELPER(DGUS_ScreenID::MOVEAXIS_10,          LIST_MOVEAXIS),
  MAP_HELPER(DGUS_ScreenID::FEEDRETURN,           LIST_IO),
  MAP_HELPER(DGUS_ScreenID::CONTROL,              LIST_CONTROL),
  MAP_HELPER(DGUS_ScreenID::TEMP,                 LIST_SETTINGS),
  MAP_HELPER(DGUS_ScreenID::PLA_TEMP,             LIST_TEMPERATURE_PRESETS),
  MAP_HELPER(DGUS_ScreenID::ABS_TEMP,             LIST_TEMPERATURE_PRESETS),
  MAP_HELPER(DGUS_ScreenID::AUTOHOME,             LIST_AUTOHOME),
  MAP_HELPER(DGUS_ScreenID::ABNORMAL,             LIST_ABNORMAL),
  MAP_HELPER(DGUS_ScreenID::LEVELINGMODE,         LIST_TRAMMING),
  MAP_HELPER(DGUS_ScreenID::LEVELING,             LIST_AUTOLEVEL),
  MAP_HELPER(DGUS_ScreenID::POWERCONTINUE,        LIST_POWERLOSS),
  MAP_HELPER(DGUS_ScreenID::LANGUAGE,             LIST_LANGUAGE),
  MAP_HELPER(DGUS_ScreenID::INFORMATION,          LIST_INFO),
  MAP_HELPER(DGUS_ScreenID::CONTROL_DEVICE,       LIST_ADV_SETTINGS),
  MAP_HELPER(DGUS_ScreenID::MOTION,               LIST_MOTION),
  MAP_HELPER(DGUS_ScreenID::MAX_FEEDRATE,         LIST_MAX_FEEDRATE),
  MAP_HELPER(DGUS_ScreenID::ACCELERATION,         LIST_ACCELERATION),
  MAP_HELPER(DGUS_ScreenID::JERK,                 LIST_JERK),
  MAP_HELPER(DGUS_ScreenID::STEPSMM,              LIST_STEPSMM),
  MAP_HELPER(DGUS_ScreenID::PIDCONTROL,           LIST_PIDCONTROL),
  MAP_HELPER(DGUS_ScreenID::FACTORYRESET_CONFIRM, LIST_RESET_CONFIRM),
  MAP_HELPER(DGUS_ScreenID::FILAMENTCHECK,        LIST_FILAMENTCHECK),
  MAP_HELPER(DGUS_ScreenID::SDCARDCHECK,          LIST_SDCARDCHECK),
  MAP_HELPER(DGUS_ScreenID::LASER_FDM,            LIST_PRINTMODE),
  MAP_HELPER(DGUS_ScreenID::SW_LASER_TIPS,        LIST_PRINTMODE_CONFIRM),
  MAP_HELPER(DGUS_ScreenID::SW_FDM_TIPS,          LIST_PRINTMODE_CONFIRM),

  MAP_HELPER((DGUS_ScreenID)0, nullptr)
};

#endif // DGUS_LCD_UI_E3S1PRO
