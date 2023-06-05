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
#pragma once

#define DGUS_MAIN_STATUS_LEN        20
#define DGUS_FILENAME_LEN           20
#define DGUS_INFOSTRING_LEN         16 // 20 in the firmware, but really only shows 16 characters

enum class DGUS_Addr : uint16_t {
  END                       = 0,
  BOOT_Logo                 = 0x1000,
  CMD_MenuSelect            = 0x1002, // r, int, DGUS_Data::MenuSelectCommand
  CMD_Adjust                = 0x1004, // r, int, DGUS_Data::AdjustCommand
  MAIN_PrintSpeedPercentage = 0x1006, // rw, int, 3.0
  CMD_CheckKO               = 0x1008, // r, int, DGUS_Data::CheckKOCommand
  CMD_StopPause             = 0x100A, // r, int, DGUS_Data::StopPauseCommand
  CMD_CheckOK               = 0x100C, // r, int, DGUS_Data::CheckOKCommand
  MAIN_Icon_Percentage      = 0x100E, // w, icon, 0-100
  MAIN_ElapsedHours         = 0x1010, // w, int, 3.0
  MAIN_ElapsedMinutes       = 0x1012, // w, int, 2.0
  MAIN_PrintPercentage      = 0x1016, // w, int, 3.0
  MAIN_ZOffset              = 0x1026, // w, int, 2.2
  CMD_PresetControl         = 0x1030, // r, int, DGUS_Data::PresetControlCommand
  CMD_TemperatureControl    = 0x1032, // r, int, DGUS_Data::TemperatureControlCommand
  MAIN_ExtruderTargetTemp   = 0x1034, // rw, int, 3.0
  MAIN_ExtruderCurrentTemp  = 0x1036, // w, int, 3.0
  MAIN_BedTargetTemp        = 0x103A, // rw, int, 3.0
  MAIN_BedCurrentTemp       = 0x103C, // w, int, 3.0
  CMD_SettingsMenu          = 0x103E, // r, int, DGUS_Data::SettingsMenuCommand
  CMD_Leveling              = 0x1044, // r, int, DGUS_Data::LevelingCommand
  CMD_AxisControl           = 0x1046, // r, int, DGUS_Data::AxisControlCommand
  AXIS_X                    = 0x1048, // rw, int, 3.1
  AXIS_Y                    = 0x104A, // rw, int, 3.1
  AXIS_Z                    = 0x104C, // rw, int, 3.1
  IO_ExtrudeLength          = 0x1052, // r, int, 3.1
  IO_RetractLength          = 0x1054, // r, int, 3.1
  CMD_AxisIO                = 0x1056, // r, int, DGUS_Data::FilamentIoCommand
  LANGUAGE_SetLanguage      = 0x105C, // r, int, 1-9
  CMD_PowerLoss             = 0x105F, // r, int, DGUS_Data::PowerLossCommand
  LEVELING_Progress_Icon    = 0x108D, // w, icon, progress 1-50
  FILAMENT_Icon_Inserted    = 0x108E, // w, icon, on-off 0-1
  TEMP_PLA_ExtruderTemp     = 0x1090, // rw, int, 3.0
  TEMP_PLA_BedTemp          = 0x1092, // rw, int, 3.0
  TEMP_ABS_ExtruderTemp     = 0x1094, // rw, int, 3.0
  TEMP_ABS_BedTemp          = 0x1096, // rw, int, 3.0
  CMD_AdvancedSettings      = 0x1098, // r, int, DGUS_Data::AdvancedSettingsCommand
  LEVELING_Progress         = 0x109C, // w, int, 3.0
  MOTION_MaxSpeed_X         = 0x109E, // rw, int, 4.0
  MOTION_MaxSpeed_Y         = 0x10A0, // rw, int, 4.0
  MOTION_MaxSpeed_Z         = 0x10A2, // rw, int, 4.0
  MOTION_MaxSpeed_E         = 0x10A4, // rw, int, 4.0
  MOTION_Acceleration_X     = 0x10A6, // rw, int, 4.0
  MOTION_Acceleration_Y     = 0x10A8, // rw, int, 4.0
  MOTION_Acceleration_Z     = 0x10AA, // rw, int, 4.0
  MOTION_Acceleration_E     = 0x10AC, // rw, int, 4.0
  MOTION_MaxJerk_X          = 0x10AE, // rw, int, 2.2
  MOTION_MaxJerk_Y          = 0x10B0, // rw, int, 2.2
  MOTION_MaxJerk_Z          = 0x10B2, // rw, int, 2.2
  MOTION_MaxJerk_E          = 0x10B4, // rw, int, 2.2
  MOTION_StepsPerMm_X       = 0x10B6, // rw, int, 4.1
  MOTION_StepsPerMm_Y       = 0x10B8, // rw, int, 4.1
  MOTION_StepsPerMm_Z       = 0x10BA, // rw, int, 4.1
  MOTION_StepsPerMm_E       = 0x10BC, // rw, int, 4.1
  PID_Extruder_P            = 0x10BE, // rw, int, 3.2
  PID_Extruder_I            = 0x10C0, // rw, int, 3.2
  PID_Extruder_D            = 0x10C2, // rw, int, 3.2
  PID_Bed_P                 = 0x10C4, // rw, int, 3.2
  PID_Bed_I                 = 0x10C6, // rw, int, 3.2
  PID_Bed_D                 = 0x10C8, // rw, int, 4.1
  CONTROL_FanSpeed          = 0x10CA, // rw, int, 3.0
  SDCard_InsertionStatus    = 0x1168, // w, icon, on off 0-1
  FINISH_Icon_Finished      = 0x1170, // w, icon, translated 1-9
  MAIN_RemainingHours       = 0x1171, // w, int, 3.0
  MAIN_RemainingMinutes     = 0x1173, // w, int, 2.0

  AXIS_StepperStatus        = 0x1200, // w, icon, on off 0-1
  SDCARD_Selection_File1    = 0x1221, // w, icon, on off 0-1
  SDCARD_Selection_File2    = 0x1222, // w, icon, on off 0-1
  SDCARD_Selection_File3    = 0x1223, // w, icon, on off 0-1
  SDCARD_Selection_File4    = 0x1224, // w, icon, on off 0-1
  SDCARD_Selection_File5    = 0x1225, // w, icon, on off 0-1
  SDCARD_Selection_File6    = 0x1226, // w, icon, on off 0-1
  SDCARD_Selection_File7    = 0x1227, // w, icon, on off 0-1
  SDCARD_Selection_File8    = 0x1228, // w, icon, on off 0-1
  SDCARD_Selection_File9    = 0x1229, // w, icon, on off 0-1
  SDCARD_Selection_File10   = 0x122A, // w, icon, on off 0-1
  SDCARD_Selection_File11   = 0x122B, // w, icon, on off 0-1
  SDCARD_Selection_File12   = 0x122C, // w, icon, on off 0-1
  SDCARD_Selection_File13   = 0x122D, // w, icon, on off 0-1
  SDCARD_Selection_File14   = 0x122E, // w, icon, on off 0-1
  SDCARD_Selection_File15   = 0x122F, // w, icon, on off 0-1
  SDCARD_Selection_File16   = 0x1230, // w, icon, on off 0-1
  SDCARD_Selection_File17   = 0x1231, // w, icon, on off 0-1
  SDCARD_Selection_File18   = 0x1232, // w, icon, on off 0-1
  SDCARD_Selection_File19   = 0x1233, // w, icon, on off 0-1
  SDCARD_Selection_File20   = 0x1234, // w, icon, on off 0-1
  MENU_Icon_Home_S          = 0x1300, // w, icon, translated 1-9
  MENU_Icon_Print_S         = 0x1301, // w, icon, translated 1-9
  MENU_Icon_Ready_S         = 0x1302, // w, icon, translated 1-9
  MENU_Icon_Settings_S      = 0x1303, // w, icon, translated 1-9
  MENU_Icon_Home            = 0x1304, // w, icon, translated 1-9
  MENU_Icon_Print           = 0x1305, // w, icon, translated 1-9
  MENU_Icon_Ready           = 0x1306, // w, icon, translated 1-9
  MENU_Icon_Settings        = 0x1307, // w, icon, translated 1-9
  ADJUST_Icon_Adjust        = 0x130D, // w, icon, translated 1-9
  ADJUST_Icon_Feedrate      = 0x130E, // w, icon, translated 1-9
  IO_Icon_ExtruderTemp      = 0x130F, // w, icon, translated 1-9
  CONTROL_Icon_BedTemp      = 0x1310, // w, icon, translated 1-9
  ADJUST_Icon_ZOffset       = 0x1311, // w, icon, translated 1-9
  CONTROL_Icon_FanControl   = 0x1312, // w, icon, translated 1-9
  ADJUST_Icon_FanControl    = 0x1313, // w, icon, translated 1-9
  AXIS_Icon_MoveAxis        = 0x1314, // w, icon, translated 1-9
  AXIS_Icon_IO              = 0x1318, // w, icon, translated 1-9
  AXIS_Icon_Manual          = 0x1319, // w, icon, translated 1-9
  CONTROL_Icon_Cooling      = 0x131A, // w, icon, translated 1-9
  IO_Icon_Extrude           = 0x1321, // w, icon, translated 1-9
  IO_Icon_Retract           = 0x1322, // w, icon, translated 1-9
  SETTINGS_Icon_Language    = 0x1323, // w, icon, translated 1-9
  SETTINGS_Icon_Leveling    = 0x1325, // w, icon, translated 1-9
  SETTINGS_Icon_Device      = 0x1326, // w, icon, translated 1-9
  ADV_Icon_ResetSettings    = 0x1327, // w, icon, translated 1-9
  INFO_Icon_About           = 0x1328, // w, icon, translated 1-9
  INFO_Icon_Device          = 0x1329, // w, icon, translated 1-9
  SETTINGS_Icon_Advanced    = 0x132A, // w, icon, translated 1-9
  SETTINGS_Icon_About       = 0x132B, // w, icon, translated 1-9
  SETTINGS_Icon_PLA         = 0x132D, // w, icon, translated 1-9
  SETTINGS_Icon_ABS         = 0x132E, // w, icon, translated 1-9
  RUNOUT_Icon_Stop          = 0x133E, // w, icon, translated 1-9
  RUNOUT_Icon_Message       = 0x133F, // w, icon, translated 1-9
  RESET_Icon_Yes            = 0x1340, // w, icon, translated 1-9
  RESET_Icon_No             = 0x1341, // w, icon, translated 1-9
  LOAD_Icon_Message         = 0x1342, // w, icon, translated 1-9
  LOAD_Icon_Continue        = 0x1343, // w, icon, translated 1-9
  PAUSE_Icon_Message        = 0x1344, // w, icon, translated 1-9
  RESUME_Icon_Message       = 0x1347, // w, icon, translated 1-9
  POWERLOSS_Icon_Message    = 0x1348, // w, icon, translated 1-9
  AUTOHOME_Icon_WaitMessage = 0x1349, // w, icon, translated 1-9
  LEVELING_Icon_Leveling    = 0x134B, // w, icon, translated 1-9
  RESET_Icon_InfoMessage    = 0x134D, // w, icon, translated 1-9
  LEVELING_Icon_Start       = 0x1356, // w, icon, translated 1-9
  LEVELING_Icon_AuxLevel    = 0x1357, // w, icon, translated 1-9
  LEVELING_Icon_AutoLevel   = 0x1358, // w, icon, translated 1-9
  LEVELING_Icon_AuxLevel_S  = 0x1359, // w, icon, translated 1-9
  LEVELING_Icon_AutoLevel_S = 0x135A, // w, icon, translated 1-9
  LANGUAGE_Icon_Language    = 0x135B, // w, icon, translated 1-9
  ADV_Icon_Movement         = 0x135C, // w, icon, translated 1-9
  ADV_Icon_PID              = 0x135D, // w, icon, translated 1-9
  ADV_Icon_MotionSettings   = 0x135F, // w, icon, translated 1-9
  ADV_Icon_MaxFeedrate      = 0x1360, // w, icon, translated 1-9
  ADV_Icon_Acceleration     = 0x1361, // w, icon, translated 1-9
  ADV_Icon_Jerk             = 0x1362, // w, icon, translated 1-9
  ADV_Icon_StepsPerMm       = 0x1363, // w, icon, translated 1-9
  ADV_Icon_MaxSpeed         = 0x1364, // w, icon, translated 1-9
  ADV_Icon_MaxSpeed_X       = 0x1365, // w, icon, translated 1-9
  ADV_Icon_MaxSpeed_Y       = 0x1366, // w, icon, translated 1-9
  ADV_Icon_MaxSpeed_Z       = 0x1367, // w, icon, translated 1-9
  ADV_Icon_MaxSpeed_E       = 0x1368, // w, icon, translated 1-9
  ADV_Icon_AccelerationMenu = 0x1369, // w, icon, translated 1-9
  ADV_Icon_Acceleration_X   = 0x136A, // w, icon, translated 1-9
  ADV_Icon_Acceleration_Y   = 0x136B, // w, icon, translated 1-9
  ADV_Icon_Acceleration_Z   = 0x136C, // w, icon, translated 1-9
  ADV_Icon_Acceleration_E   = 0x136D, // w, icon, translated 1-9
  ADV_Icon_MaxJerkMenu      = 0x136E, // w, icon, translated 1-9
  ADV_Icon_MaxJerk_X        = 0x136F, // w, icon, translated 1-9
  ADV_Icon_MaxJerk_Y        = 0x1370, // w, icon, translated 1-9
  ADV_Icon_MaxJerk_Z        = 0x1371, // w, icon, translated 1-9
  ADV_Icon_MaxJerk_E        = 0x1372, // w, icon, translated 1-9
  ADV_Icon_StepsPerMmMenu   = 0x1373, // w, icon, translated 1-9
  ADV_Icon_StepsPerMm_X     = 0x1374, // w, icon, translated 1-9
  ADV_Icon_StepsPerMm_Y     = 0x1375, // w, icon, translated 1-9
  ADV_Icon_StepsPerMm_Z     = 0x1376, // w, icon, translated 1-9
  ADV_Icon_StepsPerMm_E     = 0x1377, // w, icon, translated 1-9
  PID_Icon_PIDMenu          = 0x1378, // w, icon, translated 1-9
  PID_Icon_Extruder_P       = 0x1379, // w, icon, translated 1-9
  PID_Icon_Extruder_I       = 0x137A, // w, icon, translated 1-9
  PID_Icon_Extruder_D       = 0x137B, // w, icon, translated 1-9
  PID_Icon_Bed_P            = 0x137C, // w, icon, translated 1-9
  PID_Icon_Bed_I            = 0x137D, // w, icon, translated 1-9
  PID_Icon_Bed_D            = 0x137E, // w, icon, translated 1-9
  CUTTER_Icon_Message       = 0x1381, // w, icon, translated 1-9
  FDM_Icon_Message          = 0x1382, // w, icon, translated 1-9
  ADV_Icon_SwitchToLaser    = 0x1388, // w, icon, translated 1-9
  PRTMODE_Icon_Message      = 0x1389, // w, icon, translated 1-9
  PRTMODE_Icon_FDM          = 0x138D, // w, icon, translated 1-9
  PRTMODE_Icon_Cutter       = 0x138E, // w, icon, translated 1-9
  INFO_Icon_Model           = 0x1400, // w, icon, translated 1-9
  INFO_Icon_FW_Version      = 0x1401, // w, icon, translated 1-9
  INFO_Icon_Screen_Version  = 0x1402, // w, icon, translated 1-9
  INFO_Icon_HW_Version      = 0x1403, // w, icon, translated 1-9
  INFO_Icon_Website         = 0x1405, // w, icon, translated 1-9
  INFO_Icon_Print_Size      = 0x1406, // w, icon, translated 1-9
  LANGUAGE_Icon_Chinese     = 0x1411, // w, icon, on off 0-1
  LANGUAGE_Icon_English     = 0x1412, // w, icon, on off 0-1
  LANGUAGE_Icon_German      = 0x1413, // w, icon, on off 0-1
  LANGUAGE_Icon_Spanish     = 0x1414, // w, icon, on off 0-1
  LANGUAGE_Icon_French      = 0x1415, // w, icon, on off 0-1
  LANGUAGE_Icon_Italian     = 0x1416, // w, icon, on off 0-1
  LANGUAGE_Icon_Portuguese  = 0x1417, // w, icon, on off 0-1
  LANGUAGE_Icon_Russian     = 0x1418, // w, icon, on off 0-1
  LANGUAGE_Icon_Turkish     = 0x1419, // w, icon, on off 0-1
  INFO_Model                = 0x17B0, // w, text, 20
  INFO_FW_Version           = 0x17C4, // w, text, 20
  INFO_Screen_Version       = 0x17D8, // w, text, 20
  INFO_HW_Version           = 0x17EC, // w, text, 20
  INFO_Print_Size           = 0x1800, // w, text, 20
  INFO_Website              = 0x1814, // w, text, 20
  SDCARD_Filename1          = 0x200A, // w, text, 20
  SDCARD_Filename2          = 0x201E, // w, text, 20
  SDCARD_Filename3          = 0x2032, // w, text, 20
  SDCARD_Filename4          = 0x2046, // w, text, 20
  SDCARD_Filename5          = 0x205A, // w, text, 20
  SDCARD_Filename6          = 0x206E, // w, text, 20
  SDCARD_Filename7          = 0x2082, // w, text, 20
  SDCARD_Filename8          = 0x2096, // w, text, 20
  SDCARD_Filename9          = 0x20AA, // w, text, 20
  SDCARD_Filename10         = 0x20BE, // w, text, 20
  SDCARD_Filename11         = 0x20D2, // w, text, 20
  SDCARD_Filename12         = 0x20E6, // w, text, 20
  SDCARD_Filename13         = 0x20FA, // w, text, 20
  SDCARD_Filename14         = 0x210E, // w, text, 20
  SDCARD_Filename15         = 0x2122, // w, text, 20
  SDCARD_Filename16         = 0x2136, // w, text, 20
  SDCARD_Filename17         = 0x214A, // w, text, 20
  SDCARD_Filename18         = 0x215E, // w, text, 20
  SDCARD_Filename19         = 0x2172, // w, text, 20
  SDCARD_Filename20         = 0x2186, // w, text, 20
  CMD_FilelistControl       = 0x2198, // w, int, DGUS_Data::FilelistControlCommand
  SDCARD_FileSelection      = 0x2199, // r, int 1-20
  MAIN_StatusMessage        = 0x219A, // w, text, 20
  ABNORMAL_StatusMessage    = 0x21D4, // w, text, 30
  CMD_LaserControl          = 0x2201, // w, int, DGUS_Data::LaserControlCommand
};
