/**
 * Marlin 3D Printer Firmware
 * Copyright (c) 2021 MarlinFirmware [https://github.com/MarlinFirmware/Marlin]
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include "../../../../inc/MarlinConfigPre.h"

#if ENABLED(DGUS_LCD_UI_RELOADED)

#include "DGUS_ScreenAddrList.h"

#include "../../ui_api.h"

constexpr DGUS_Addr LIST_HOME[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  (DGUS_Addr)0
};

#if ENABLED(SDSUPPORT)
  constexpr DGUS_Addr LIST_PRINT[] PROGMEM = {
    DGUS_Addr::SD_Type,
    DGUS_Addr::SD_FileName0,
    DGUS_Addr::SD_FileName1,
    DGUS_Addr::SD_FileName2,
    DGUS_Addr::SD_FileName3,
    DGUS_Addr::SD_FileName4,
    DGUS_Addr::SD_ScrollIcons,
    DGUS_Addr::SD_SelectedFileName,
    (DGUS_Addr)0
  };
#endif

constexpr DGUS_Addr LIST_PRINT_STATUS[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::STATUS_PositionZ,
  DGUS_Addr::STATUS_Ellapsed,
  DGUS_Addr::STATUS_Percent,
  DGUS_Addr::STATUS_Icons,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_PRINT_ADJUST[] PROGMEM = {
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::FAN0_Speed,
  DGUS_Addr::ADJUST_Feedrate,
  DGUS_Addr::ADJUST_Flowrate_CUR,
  DGUS_Addr::LEVEL_OFFSET_Current,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_PRINT_FINISHED[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::STATUS_PositionZ,
  DGUS_Addr::STATUS_Ellapsed,
  DGUS_Addr::STATUS_Percent_Complete,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_TEMP_MENU[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_TEMP_MANUAL[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Max_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::TEMP_Max_Bed,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_FAN[] PROGMEM = {
  DGUS_Addr::FAN0_Speed,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_SETTINGS_MENU[] PROGMEM = {
  DGUS_Addr::STEPPER_Status,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_LEVELING_OFFSET[] PROGMEM = {
  DGUS_Addr::LEVEL_OFFSET_Current,
  DGUS_Addr::LEVEL_OFFSET_StepIcons,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_LEVELING_MANUAL[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_LEVELING_AUTOMATIC[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::LEVEL_AUTO_DisableIcon,
  DGUS_Addr::LEVEL_AUTO_Grid,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_LEVELING_PROBING[] PROGMEM = {
  DGUS_Addr::LEVEL_PROBING_Icons1,
  DGUS_Addr::LEVEL_PROBING_Icons2,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_FILAMENT[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::FILAMENT_ExtruderIcons,
  DGUS_Addr::FILAMENT_Length,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_MOVE[] PROGMEM = {
  DGUS_Addr::MOVE_CurrentX,
  DGUS_Addr::MOVE_CurrentY,
  DGUS_Addr::MOVE_CurrentZ,
  DGUS_Addr::MOVE_StepIcons,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_GCODE[] PROGMEM = {
  DGUS_Addr::GCODE_Data,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_SETTINGS_MENU2[] PROGMEM = {
  DGUS_Addr::SETTINGS2_BLTouch,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_PID[] PROGMEM = {
  DGUS_Addr::PID_HeaterIcons,
  DGUS_Addr::PID_Temp,
  DGUS_Addr::PID_Cycles,
  DGUS_Addr::PID_Kp,
  DGUS_Addr::PID_Ki,
  DGUS_Addr::PID_Kd,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_VOLUME[] PROGMEM = {
  DGUS_Addr::VOLUME_Level,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_BRIGHTNESS[] PROGMEM = {
  DGUS_Addr::BRIGHTNESS_Level,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_INFOS[] PROGMEM = {
  DGUS_Addr::INFOS_Machine,
  DGUS_Addr::INFOS_BuildVolume,
  DGUS_Addr::INFOS_Version,
  DGUS_Addr::INFOS_TotalPrints,
  DGUS_Addr::INFOS_FinishedPrints,
  DGUS_Addr::INFOS_PrintTime,
  DGUS_Addr::INFOS_LongestPrint,
  DGUS_Addr::INFOS_FilamentUsed,
  (DGUS_Addr)0
};

constexpr DGUS_Addr LIST_WAIT[] PROGMEM = {
  DGUS_Addr::WAIT_Icons,
  (DGUS_Addr)0
};

#define MAP_HELPER(SCREEN, LIST) \
  { .screen = SCREEN, \
  .addr_list = LIST }

const struct DGUS_ScreenAddrList screen_addr_list_map[] PROGMEM = {
  MAP_HELPER(DGUS_Screen::HOME,                 LIST_HOME),
  #if ENABLED(SDSUPPORT)
    MAP_HELPER(DGUS_Screen::PRINT,              LIST_PRINT),
  #endif
  MAP_HELPER(DGUS_Screen::PRINT_STATUS,         LIST_PRINT_STATUS),
  MAP_HELPER(DGUS_Screen::PRINT_ADJUST,         LIST_PRINT_ADJUST),
  MAP_HELPER(DGUS_Screen::PRINT_FINISHED,       LIST_PRINT_FINISHED),
  MAP_HELPER(DGUS_Screen::TEMP_MENU,            LIST_TEMP_MENU),
  MAP_HELPER(DGUS_Screen::TEMP_MANUAL,          LIST_TEMP_MANUAL),
  MAP_HELPER(DGUS_Screen::FAN,                  LIST_FAN),
  MAP_HELPER(DGUS_Screen::SETTINGS_MENU,        LIST_SETTINGS_MENU),
  MAP_HELPER(DGUS_Screen::LEVELING_OFFSET,      LIST_LEVELING_OFFSET),
  MAP_HELPER(DGUS_Screen::LEVELING_MANUAL,      LIST_LEVELING_MANUAL),
  MAP_HELPER(DGUS_Screen::LEVELING_AUTOMATIC,   LIST_LEVELING_AUTOMATIC),
  MAP_HELPER(DGUS_Screen::LEVELING_PROBING,     LIST_LEVELING_PROBING),
  MAP_HELPER(DGUS_Screen::FILAMENT,             LIST_FILAMENT),
  MAP_HELPER(DGUS_Screen::MOVE,                 LIST_MOVE),
  MAP_HELPER(DGUS_Screen::GCODE,                LIST_GCODE),
  MAP_HELPER(DGUS_Screen::SETTINGS_MENU2,       LIST_SETTINGS_MENU2),
  MAP_HELPER(DGUS_Screen::PID,                  LIST_PID),
  MAP_HELPER(DGUS_Screen::VOLUME,               LIST_VOLUME),
  MAP_HELPER(DGUS_Screen::BRIGHTNESS,           LIST_BRIGHTNESS),
  MAP_HELPER(DGUS_Screen::INFOS,                LIST_INFOS),
  MAP_HELPER(DGUS_Screen::WAIT,                 LIST_WAIT),

  MAP_HELPER((DGUS_Screen)0, nullptr)
};

#endif // DGUS_LCD_UI_RELOADED
