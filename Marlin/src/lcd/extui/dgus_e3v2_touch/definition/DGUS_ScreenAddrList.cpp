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

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_E3V2_TOUCH)

#include "DGUS_ScreenAddrList.h"

#include "../../ui_api.h"

// Generated from the stock DWIN_SET: every variable shown on each page

constexpr DGUS_Addr LIST_BOOT[] PROGMEM = {
  DGUS_Addr::BOOT_Logo,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_MAIN[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::LANG_Icon_1300,
  DGUS_Addr::LANG_Icon_1301,
  DGUS_Addr::LANG_Icon_1302,
  DGUS_Addr::LANG_Icon_1304,
  DGUS_Addr::LANG_Icon_1303,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FILE1[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::SDCARD_Selection_File1,
  DGUS_Addr::SDCARD_Selection_File2,
  DGUS_Addr::SDCARD_Selection_File3,
  DGUS_Addr::SDCARD_Selection_File4,
  DGUS_Addr::SDCARD_Filename1,
  DGUS_Addr::SDCARD_Filename2,
  DGUS_Addr::SDCARD_Filename3,
  DGUS_Addr::SDCARD_Filename4,
  DGUS_Addr::LANG_Icon_1305,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::SDCARD_Color_File1,
  DGUS_Addr::SDCARD_Color_File2,
  DGUS_Addr::SDCARD_Color_File3,
  DGUS_Addr::SDCARD_Color_File4,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FILE2[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::SDCARD_Selection_File5,
  DGUS_Addr::SDCARD_Selection_File6,
  DGUS_Addr::SDCARD_Selection_File7,
  DGUS_Addr::SDCARD_Selection_File8,
  DGUS_Addr::SDCARD_Filename5,
  DGUS_Addr::SDCARD_Filename6,
  DGUS_Addr::SDCARD_Filename7,
  DGUS_Addr::SDCARD_Filename8,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1305,
  DGUS_Addr::SDCARD_Color_File5,
  DGUS_Addr::SDCARD_Color_File6,
  DGUS_Addr::SDCARD_Color_File7,
  DGUS_Addr::SDCARD_Color_File8,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FILE3[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::SDCARD_Selection_File9,
  DGUS_Addr::SDCARD_Selection_File10,
  DGUS_Addr::SDCARD_Selection_File11,
  DGUS_Addr::SDCARD_Selection_File12,
  DGUS_Addr::SDCARD_Filename9,
  DGUS_Addr::SDCARD_Filename10,
  DGUS_Addr::SDCARD_Filename11,
  DGUS_Addr::SDCARD_Filename12,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1305,
  DGUS_Addr::SDCARD_Color_File9,
  DGUS_Addr::SDCARD_Color_File10,
  DGUS_Addr::SDCARD_Color_File11,
  DGUS_Addr::SDCARD_Color_File12,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FILE4[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::SDCARD_Selection_File13,
  DGUS_Addr::SDCARD_Selection_File14,
  DGUS_Addr::SDCARD_Selection_File15,
  DGUS_Addr::SDCARD_Selection_File16,
  DGUS_Addr::SDCARD_Filename13,
  DGUS_Addr::SDCARD_Filename14,
  DGUS_Addr::SDCARD_Filename15,
  DGUS_Addr::SDCARD_Filename16,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1305,
  DGUS_Addr::SDCARD_Color_File13,
  DGUS_Addr::SDCARD_Color_File14,
  DGUS_Addr::SDCARD_Color_File15,
  DGUS_Addr::SDCARD_Color_File16,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FILE5[] PROGMEM = {
  DGUS_Addr::SDCARD_Selection_File17,
  DGUS_Addr::SDCARD_Selection_File18,
  DGUS_Addr::SDCARD_Selection_File19,
  DGUS_Addr::SDCARD_Selection_File20,
  DGUS_Addr::SDCARD_Filename17,
  DGUS_Addr::SDCARD_Filename19,
  DGUS_Addr::SDCARD_Filename20,
  DGUS_Addr::SDCARD_Filename18,
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1305,
  DGUS_Addr::SDCARD_Color_File17,
  DGUS_Addr::SDCARD_Color_File18,
  DGUS_Addr::SDCARD_Color_File19,
  DGUS_Addr::SDCARD_Color_File20,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FILAMENT_RUNOUT[] PROGMEM = {
  DGUS_Addr::LANG_Icon_133F,
  DGUS_Addr::LANG_Icon_1340,
  DGUS_Addr::LANG_Icon_1341,
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FILAMENT_INSERT[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::LANG_Icon_1343,
  DGUS_Addr::LANG_Icon_1342,
  DGUS_Addr::LANG_Icon_1341,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FINISH[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::MAIN_Icon_Percentage,
  DGUS_Addr::MAIN_ElapsedHours,
  DGUS_Addr::MAIN_ElapsedMinutes,
  DGUS_Addr::MAIN_PrintPercentage,
  DGUS_Addr::PRINT_Filename,
  DGUS_Addr::LANG_Icon_1307,
  DGUS_Addr::LANG_Icon_1308,
  DGUS_Addr::LANG_Icon_1309,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PRINTING[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::MAIN_Icon_Percentage,
  DGUS_Addr::MAIN_PrintPercentage,
  DGUS_Addr::PRINT_Filename,
  DGUS_Addr::LANG_Icon_1307,
  DGUS_Addr::LANG_Icon_130A,
  DGUS_Addr::LANG_Icon_130B,
  DGUS_Addr::LANG_Icon_130C,
  DGUS_Addr::MAIN_ElapsedHours,
  DGUS_Addr::MAIN_ElapsedMinutes,
  DGUS_Addr::LANG_Icon_1308,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PAUSE_CONFIRM[] PROGMEM = {
  DGUS_Addr::LANG_Icon_1344,
  DGUS_Addr::LANG_Icon_1345,
  DGUS_Addr::LANG_Icon_1346,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PAUSED[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::MAIN_Icon_Percentage,
  DGUS_Addr::MAIN_PrintPercentage,
  DGUS_Addr::PRINT_Filename,
  DGUS_Addr::LANG_Icon_130A,
  DGUS_Addr::LANG_Icon_130C,
  DGUS_Addr::LANG_Icon_1307,
  DGUS_Addr::LANG_Icon_130B,
  DGUS_Addr::MAIN_ElapsedHours,
  DGUS_Addr::MAIN_ElapsedMinutes,
  DGUS_Addr::LANG_Icon_1308,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_STOP_CONFIRM[] PROGMEM = {
  DGUS_Addr::LANG_Icon_1347,
  DGUS_Addr::LANG_Icon_1346,
  DGUS_Addr::LANG_Icon_1345,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_ADJUST[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::ADJUST_Icon_Fan,
  DGUS_Addr::LANG_Icon_130D,
  DGUS_Addr::LANG_Icon_130E,
  DGUS_Addr::LANG_Icon_130F,
  DGUS_Addr::LANG_Icon_1310,
  DGUS_Addr::LANG_Icon_1311,
  DGUS_Addr::LANG_Icon_1312,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PREPARE[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::AXIS_StepperStatus,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1314,
  DGUS_Addr::LANG_Icon_1315,
  DGUS_Addr::LANG_Icon_1316,
  DGUS_Addr::LANG_Icon_1317,
  DGUS_Addr::LANG_Icon_1318,
  DGUS_Addr::LANG_Icon_1319,
  DGUS_Addr::LANG_Icon_131A,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_MOVEAXIS_10[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::AXIS_X,
  DGUS_Addr::AXIS_Y,
  DGUS_Addr::AXIS_Z,
  DGUS_Addr::LANG_Icon_131B,
  DGUS_Addr::LANG_Icon_131C,
  DGUS_Addr::LANG_Icon_131D,
  DGUS_Addr::LANG_Icon_131E,
  DGUS_Addr::LANG_Icon_131F,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_MOVEAXIS_1[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::AXIS_X,
  DGUS_Addr::AXIS_Y,
  DGUS_Addr::AXIS_Z,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_131B,
  DGUS_Addr::LANG_Icon_131C,
  DGUS_Addr::LANG_Icon_131D,
  DGUS_Addr::LANG_Icon_131E,
  DGUS_Addr::LANG_Icon_131F,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_MOVEAXIS_01[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::AXIS_X,
  DGUS_Addr::AXIS_Y,
  DGUS_Addr::AXIS_Z,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_131B,
  DGUS_Addr::LANG_Icon_131C,
  DGUS_Addr::LANG_Icon_131D,
  DGUS_Addr::LANG_Icon_131E,
  DGUS_Addr::LANG_Icon_131F,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FEEDRETURN[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::IO_FilamentLength,
  DGUS_Addr::FILAMENT_Icon_Missing,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1320,
  DGUS_Addr::LANG_Icon_1321,
  DGUS_Addr::LANG_Icon_1322,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_CONTROL[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1323,
  DGUS_Addr::LANG_Icon_1324,
  DGUS_Addr::LANG_Icon_1326,
  DGUS_Addr::LANG_Icon_1327,
  DGUS_Addr::LANG_Icon_1328,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_TEMP[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::ADJUST_Icon_Fan,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1329,
  DGUS_Addr::LANG_Icon_132A,
  DGUS_Addr::LANG_Icon_132B,
  DGUS_Addr::LANG_Icon_132C,
  DGUS_Addr::LANG_Icon_132D,
  DGUS_Addr::LANG_Icon_132E,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PLA_TEMP[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::TEMP_PLA_ExtruderTemp,
  DGUS_Addr::TEMP_PLA_BedTemp,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_132F,
  DGUS_Addr::LANG_Icon_1330,
  DGUS_Addr::LANG_Icon_1331,
  DGUS_Addr::LANG_Icon_1332,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_ABS_TEMP[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::TEMP_ABS_ExtruderTemp,
  DGUS_Addr::TEMP_ABS_BedTemp,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1333,
  DGUS_Addr::LANG_Icon_1334,
  DGUS_Addr::LANG_Icon_1335,
  DGUS_Addr::LANG_Icon_1336,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_INFORMATION[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::INFO_Print_Size,
  DGUS_Addr::INFO_FW_Version,
  DGUS_Addr::INFO_Website,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1337,
  DGUS_Addr::LANG_Icon_1338,
  DGUS_Addr::LANG_Icon_1339,
  DGUS_Addr::LANG_Icon_133A,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_LEVELINGMODE[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_133B,
  DGUS_Addr::LANG_Icon_133D,
  DGUS_Addr::LANG_Icon_133E,
  DGUS_Addr::LANG_Icon_133C,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_LEVELING[] PROGMEM = {
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::LEVELING_Progress_Icon,
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_133B,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_POWERCONTINUE[] PROGMEM = {
  DGUS_Addr::LANG_Icon_1348,
  DGUS_Addr::LANG_Icon_1343,
  DGUS_Addr::LANG_Icon_1346,
  DGUS_Addr::PRINT_Filename,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_LANGUAGE[] PROGMEM = {
  DGUS_Addr::LANG_Icon_1306,
  DGUS_Addr::LANG_Icon_1323,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_NO_LEVEL[] PROGMEM = {
  DGUS_Addr::LANG_Icon_134C,
  DGUS_Addr::LANG_Icon_1345,
  DGUS_Addr::MAIN_ExtruderTargetTemp,
  DGUS_Addr::MAIN_ExtruderCurrentTemp,
  DGUS_Addr::MAIN_BedTargetTemp,
  DGUS_Addr::MAIN_BedCurrentTemp,
  DGUS_Addr::MAIN_PrintSpeedPercentage,
  DGUS_Addr::MAIN_ZOffset,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_AUTOHOME[] PROGMEM = {
  DGUS_Addr::LANG_Icon_1349,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_ABNORMAL[] PROGMEM = {
  DGUS_Addr::ABNORMAL_StatusMessage,
  DGUS_Addr::END
};

#define MAP_HELPER(SCREEN, LIST) \
  { .screen = SCREEN, \
  .addr_list = LIST }

const struct DGUS_ScreenAddrList screen_addr_list_map[] PROGMEM = {
  MAP_HELPER(DGUS_ScreenID::BOOT, LIST_BOOT),
  MAP_HELPER(DGUS_ScreenID::MAIN, LIST_MAIN),
  MAP_HELPER(DGUS_ScreenID::FILE1, LIST_FILE1),
  MAP_HELPER(DGUS_ScreenID::FILE2, LIST_FILE2),
  MAP_HELPER(DGUS_ScreenID::FILE3, LIST_FILE3),
  MAP_HELPER(DGUS_ScreenID::FILE4, LIST_FILE4),
  MAP_HELPER(DGUS_ScreenID::FILE5, LIST_FILE5),
  MAP_HELPER(DGUS_ScreenID::FILAMENT_RUNOUT, LIST_FILAMENT_RUNOUT),
  MAP_HELPER(DGUS_ScreenID::FILAMENT_INSERT, LIST_FILAMENT_INSERT),
  MAP_HELPER(DGUS_ScreenID::FINISH, LIST_FINISH),
  MAP_HELPER(DGUS_ScreenID::PRINTING, LIST_PRINTING),
  MAP_HELPER(DGUS_ScreenID::PAUSE_CONFIRM, LIST_PAUSE_CONFIRM),
  MAP_HELPER(DGUS_ScreenID::PAUSED, LIST_PAUSED),
  MAP_HELPER(DGUS_ScreenID::STOP_CONFIRM, LIST_STOP_CONFIRM),
  MAP_HELPER(DGUS_ScreenID::ADJUST, LIST_ADJUST),
  MAP_HELPER(DGUS_ScreenID::PREPARE, LIST_PREPARE),
  MAP_HELPER(DGUS_ScreenID::MOVEAXIS_10, LIST_MOVEAXIS_10),
  MAP_HELPER(DGUS_ScreenID::MOVEAXIS_1, LIST_MOVEAXIS_1),
  MAP_HELPER(DGUS_ScreenID::MOVEAXIS_01, LIST_MOVEAXIS_01),
  MAP_HELPER(DGUS_ScreenID::FEEDRETURN, LIST_FEEDRETURN),
  MAP_HELPER(DGUS_ScreenID::CONTROL, LIST_CONTROL),
  MAP_HELPER(DGUS_ScreenID::TEMP, LIST_TEMP),
  MAP_HELPER(DGUS_ScreenID::PLA_TEMP, LIST_PLA_TEMP),
  MAP_HELPER(DGUS_ScreenID::ABS_TEMP, LIST_ABS_TEMP),
  MAP_HELPER(DGUS_ScreenID::INFORMATION, LIST_INFORMATION),
  MAP_HELPER(DGUS_ScreenID::LEVELINGMODE, LIST_LEVELINGMODE),
  MAP_HELPER(DGUS_ScreenID::LEVELING, LIST_LEVELING),
  MAP_HELPER(DGUS_ScreenID::POWERCONTINUE, LIST_POWERCONTINUE),
  MAP_HELPER(DGUS_ScreenID::LANGUAGE, LIST_LANGUAGE),
  MAP_HELPER(DGUS_ScreenID::NO_LEVEL, LIST_NO_LEVEL),
  MAP_HELPER(DGUS_ScreenID::AUTOHOME, LIST_AUTOHOME),
  MAP_HELPER(DGUS_ScreenID::ABNORMAL, LIST_ABNORMAL),
  MAP_HELPER((DGUS_ScreenID)0, nullptr)
};

#endif // DGUS_LCD_UI_E3V2_TOUCH
