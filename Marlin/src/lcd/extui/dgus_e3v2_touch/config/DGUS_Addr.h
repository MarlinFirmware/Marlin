/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2026 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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

#include <inttypes.h>

#define DGUS_FILENAME_LEN       20
#define DGUS_FILE_COUNT         20
#define DGUS_ERRORSTRING_LEN    30
#define DGUS_INFOSTRING_LEN     20

enum class DGUS_Addr : uint16_t {
  END                       = 0,

  // Display registers
  SYS_PageSwitch            = 0x0084, // w, 5A 01 00 page

  BOOT_Logo                 = 0x1000, // w, icon, boot progress 0-100
  CMD_MenuSelect            = 0x1002, // r, int, DGUS_Data::MenuSelectCommand
  CMD_Adjust                = 0x1004, // r, int, DGUS_Data::AdjustCommand
  MAIN_PrintSpeedPercentage = 0x1006, // rw, int, 3.0
  CMD_Stop                  = 0x1008, // r, int, DGUS_Data::StopCommand
  CMD_Pause                 = 0x100A, // r, int, DGUS_Data::PauseCommand
  CMD_Resume                = 0x100C, // r, int, DGUS_Data::ResumeCommand
  MAIN_Icon_Percentage      = 0x100E, // w, icon, 0-100
  MAIN_ElapsedHours         = 0x1010, // w, int, 3.0
  MAIN_ElapsedMinutes       = 0x1012, // w, int, 2.0
  MAIN_PrintPercentage      = 0x1016, // w, int, 3.0
  ADJUST_Icon_Fan           = 0x101E, // w, icon, 1 on, 2 off
  MAIN_ZOffset              = 0x1026, // rw, int, 2.2
  CMD_TemperatureMenu       = 0x1030, // r, int, DGUS_Data::TemperatureMenuCommand
  CMD_Cooldown              = 0x1032, // r, int, DGUS_Data::CooldownCommand
  MAIN_ExtruderTargetTemp   = 0x1034, // rw, int, 3.0
  MAIN_ExtruderCurrentTemp  = 0x1036, // w, int, 3.0
  MAIN_BedTargetTemp        = 0x103A, // rw, int, 3.0
  MAIN_BedCurrentTemp       = 0x103C, // w, int, 3.0
  CMD_ControlMenu           = 0x103E, // r, int, DGUS_Data::ControlMenuCommand
  CMD_Leveling              = 0x1044, // r, int, DGUS_Data::LevelingCommand
  CMD_AxisControl           = 0x1046, // r, int, DGUS_Data::AxisControlCommand
  AXIS_X                    = 0x1048, // rw, int, 3.1
  AXIS_Y                    = 0x104A, // rw, int, 3.1
  AXIS_Z                    = 0x104C, // rw, int, 3.1
  IO_FilamentLength         = 0x1054, // rw, int, 3.1
  CMD_FilamentIO            = 0x1056, // r, int, DGUS_Data::FilamentIoCommand
  CMD_LanguageMenu          = 0x105C, // r, int, DGUS_Data::LanguageMenuCommand
  CMD_PowerLoss             = 0x105F, // r, int, DGUS_Data::PowerLossCommand
  INFO_FW_Version           = 0x106A, // w, text, 20
  INFO_Print_Size           = 0x1074, // w, text, 20
  INFO_Website              = 0x107E, // w, text, 20
  LEVELING_Progress_Icon    = 0x108D, // w, icon, probed point 1-16
  FILAMENT_Icon_Missing     = 0x108E, // w, icon, "no filament" label: language 1-9 when empty, 10 (blank) when loaded
  TEMP_PLA_ExtruderTemp     = 0x1102, // rw, int, 3.0
  TEMP_PLA_BedTemp          = 0x1104, // rw, int, 3.0
  TEMP_PLA_FanSpeed         = 0x1106, // rw, int, 3.0
  TEMP_ABS_ExtruderTemp     = 0x1108, // rw, int, 3.0
  TEMP_ABS_BedTemp          = 0x110A, // rw, int, 3.0
  TEMP_ABS_FanSpeed         = 0x110C, // rw, int, 3.0
  CMD_Refresh               = 0x110E, // r, any value: resend everything
  ABNORMAL_StatusMessage    = 0x1110, // w, text, 30
  CMD_Acknowledge           = 0x111A, // r, int, DGUS_Data::AcknowledgeCommand
  CMD_SetLanguage           = 0x111C, // r, int, DGUS_Data::Language

  AXIS_StepperStatus        = 0x1200, // w, icon, 1 on, 2 off
  SDCARD_Selection_File1    = 0x1221, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File2    = 0x1222, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File3    = 0x1223, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File4    = 0x1224, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File5    = 0x1225, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File6    = 0x1226, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File7    = 0x1227, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File8    = 0x1228, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File9    = 0x1229, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File10   = 0x122A, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File11   = 0x122B, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File12   = 0x122C, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File13   = 0x122D, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File14   = 0x122E, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File15   = 0x122F, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File16   = 0x1230, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File17   = 0x1231, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File18   = 0x1232, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File19   = 0x1233, // w, icon, 5 normal, 6 selected
  SDCARD_Selection_File20   = 0x1234, // w, icon, 5 normal, 6 selected

  // Labels drawn as language-specific icons: write the language number (1-9)
  LANG_Icon_1300            = 0x1300, // pages 1
  LANG_Icon_1301            = 0x1301, // pages 1
  LANG_Icon_1302            = 0x1302, // pages 1
  LANG_Icon_1303            = 0x1303, // pages 1
  LANG_Icon_1304            = 0x1304, // pages 1
  LANG_Icon_1305            = 0x1305, // pages 2, 3, 4, 5, 6
  LANG_Icon_1306            = 0x1306, // pages 2, 3, 4, 5, 6, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 28
  LANG_Icon_1307            = 0x1307, // pages 9, 10, 12
  LANG_Icon_1308            = 0x1308, // pages 9, 10, 12
  LANG_Icon_1309            = 0x1309, // pages 9
  LANG_Icon_130A            = 0x130A, // pages 10, 12
  LANG_Icon_130B            = 0x130B, // pages 10, 12
  LANG_Icon_130C            = 0x130C, // pages 10, 12
  LANG_Icon_130D            = 0x130D, // pages 14
  LANG_Icon_130E            = 0x130E, // pages 14
  LANG_Icon_130F            = 0x130F, // pages 14
  LANG_Icon_1310            = 0x1310, // pages 14
  LANG_Icon_1311            = 0x1311, // pages 14
  LANG_Icon_1312            = 0x1312, // pages 14
  LANG_Icon_1314            = 0x1314, // pages 15
  LANG_Icon_1315            = 0x1315, // pages 15
  LANG_Icon_1316            = 0x1316, // pages 15
  LANG_Icon_1317            = 0x1317, // pages 15
  LANG_Icon_1318            = 0x1318, // pages 15
  LANG_Icon_1319            = 0x1319, // pages 15
  LANG_Icon_131A            = 0x131A, // pages 15
  LANG_Icon_131B            = 0x131B, // pages 16, 17, 18
  LANG_Icon_131C            = 0x131C, // pages 16, 17, 18
  LANG_Icon_131D            = 0x131D, // pages 16, 17, 18
  LANG_Icon_131E            = 0x131E, // pages 16, 17, 18
  LANG_Icon_131F            = 0x131F, // pages 16, 17, 18
  LANG_Icon_1320            = 0x1320, // pages 19
  LANG_Icon_1321            = 0x1321, // pages 19
  LANG_Icon_1322            = 0x1322, // pages 19
  LANG_Icon_1323            = 0x1323, // pages 20, 28
  LANG_Icon_1324            = 0x1324, // pages 20
  LANG_Icon_1326            = 0x1326, // pages 20
  LANG_Icon_1327            = 0x1327, // pages 20
  LANG_Icon_1328            = 0x1328, // pages 20
  LANG_Icon_1329            = 0x1329, // pages 21
  LANG_Icon_132A            = 0x132A, // pages 21
  LANG_Icon_132B            = 0x132B, // pages 21
  LANG_Icon_132C            = 0x132C, // pages 21
  LANG_Icon_132D            = 0x132D, // pages 21
  LANG_Icon_132E            = 0x132E, // pages 21
  LANG_Icon_132F            = 0x132F, // pages 22
  LANG_Icon_1330            = 0x1330, // pages 22
  LANG_Icon_1331            = 0x1331, // pages 22
  LANG_Icon_1332            = 0x1332, // pages 22
  LANG_Icon_1333            = 0x1333, // pages 23
  LANG_Icon_1334            = 0x1334, // pages 23
  LANG_Icon_1335            = 0x1335, // pages 23
  LANG_Icon_1336            = 0x1336, // pages 23
  LANG_Icon_1337            = 0x1337, // pages 24
  LANG_Icon_1338            = 0x1338, // pages 24
  LANG_Icon_1339            = 0x1339, // pages 24
  LANG_Icon_133A            = 0x133A, // pages 24
  LANG_Icon_133B            = 0x133B, // pages 25, 26
  LANG_Icon_133C            = 0x133C, // pages 25
  LANG_Icon_133D            = 0x133D, // pages 25
  LANG_Icon_133E            = 0x133E, // pages 25
  LANG_Icon_133F            = 0x133F, // pages 7
  LANG_Icon_1340            = 0x1340, // pages 7
  LANG_Icon_1341            = 0x1341, // pages 7, 8
  LANG_Icon_1342            = 0x1342, // pages 8
  LANG_Icon_1343            = 0x1343, // pages 8, 27
  LANG_Icon_1344            = 0x1344, // pages 11
  LANG_Icon_1345            = 0x1345, // pages 11, 13, 29
  LANG_Icon_1346            = 0x1346, // pages 11, 13, 27
  LANG_Icon_1347            = 0x1347, // pages 13
  LANG_Icon_1348            = 0x1348, // pages 27
  LANG_Icon_1349            = 0x1349, // pages 60
  LANG_Icon_134C            = 0x134C, // pages 29

  PRINT_Filename            = 0x2000, // w, text, 20
  SDCARD_Filename1          = 0x200A, // w, text, 20
  SDCARD_Filename2          = 0x2014, // w, text, 20
  SDCARD_Filename3          = 0x201E, // w, text, 20
  SDCARD_Filename4          = 0x2028, // w, text, 20
  SDCARD_Filename5          = 0x2032, // w, text, 20
  SDCARD_Filename6          = 0x203C, // w, text, 20
  SDCARD_Filename7          = 0x2046, // w, text, 20
  SDCARD_Filename8          = 0x2050, // w, text, 20
  SDCARD_Filename9          = 0x205A, // w, text, 20
  SDCARD_Filename10         = 0x2064, // w, text, 20
  SDCARD_Filename11         = 0x206E, // w, text, 20
  SDCARD_Filename12         = 0x2078, // w, text, 20
  SDCARD_Filename13         = 0x2082, // w, text, 20
  SDCARD_Filename14         = 0x208C, // w, text, 20
  SDCARD_Filename15         = 0x2096, // w, text, 20
  SDCARD_Filename16         = 0x20A0, // w, text, 20
  SDCARD_Filename17         = 0x20AA, // w, text, 20
  SDCARD_Filename18         = 0x20B4, // w, text, 20
  SDCARD_Filename19         = 0x20BE, // w, text, 20
  SDCARD_Filename20         = 0x20C8, // w, text, 20
  CMD_FilelistControl       = 0x20D2, // r, int, DGUS_Data::FilelistControlCommand
  SDCARD_FileSelection      = 0x20D3, // r, int, 1-20

  // Text colour, through each filename's description pointer (SP)
  SDCARD_Color_File1        = 0x6013, // w, colour
  SDCARD_Color_File2        = 0x6023, // w, colour
  SDCARD_Color_File3        = 0x6033, // w, colour
  SDCARD_Color_File4        = 0x6043, // w, colour
  SDCARD_Color_File5        = 0x6053, // w, colour
  SDCARD_Color_File6        = 0x6063, // w, colour
  SDCARD_Color_File7        = 0x6073, // w, colour
  SDCARD_Color_File8        = 0x6083, // w, colour
  SDCARD_Color_File9        = 0x6093, // w, colour
  SDCARD_Color_File10       = 0x60A3, // w, colour
  SDCARD_Color_File11       = 0x60B3, // w, colour
  SDCARD_Color_File12       = 0x60C3, // w, colour
  SDCARD_Color_File13       = 0x60D3, // w, colour
  SDCARD_Color_File14       = 0x60E3, // w, colour
  SDCARD_Color_File15       = 0x60F3, // w, colour
  SDCARD_Color_File16       = 0x6103, // w, colour
  SDCARD_Color_File17       = 0x6113, // w, colour
  SDCARD_Color_File18       = 0x6123, // w, colour
  SDCARD_Color_File19       = 0x6133, // w, colour
  SDCARD_Color_File20       = 0x6143, // w, colour
};

#define DGUS_FILE_COLOR_NORMAL   0xFFFF
#define DGUS_FILE_COLOR_SELECTED 0x87F0
#define DGUS_FILE_ICON_NORMAL    5
#define DGUS_FILE_ICON_SELECTED  6
#define DGUS_FILAMENT_ICON_BLANK 10
#define DGUS_FAN_ICON_ON         1
#define DGUS_FAN_ICON_OFF        2
#define DGUS_STEPPER_ICON_ON     1
#define DGUS_STEPPER_ICON_OFF    2
