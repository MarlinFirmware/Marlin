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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 *
 */

#include "../../../../inc/MarlinConfigPre.h"

#if DGUS_LCD_UI_RELOADED

#include "DGUS_ScreenAddrList.h"

#include "../../ui_api.h"

constexpr DGUS_Addr LIST_HOME[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::END
};

#if HAS_MEDIA
  constexpr DGUS_Addr LIST_PRINT[] PROGMEM = {
    DGUS_Addr::SD_Type,
    DGUS_Addr::SD_FileName0,
    DGUS_Addr::SD_FileName1,
    DGUS_Addr::SD_FileName2,
    DGUS_Addr::SD_FileName3,
    DGUS_Addr::SD_FileName4,
    DGUS_Addr::SD_ScrollIcons,
    DGUS_Addr::SD_SelectedFileName,
    DGUS_Addr::END
  };
#endif

constexpr DGUS_Addr LIST_PRINT_STATUS[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::STATUS_PositionZ,
  DGUS_Addr::STATUS_Elapsed,
  DGUS_Addr::STATUS_Percent,
  DGUS_Addr::STATUS_Icons,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PRINT_ADJUST[] PROGMEM = {
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::FAN0_Speed,
  DGUS_Addr::ADJUST_Feedrate,
  DGUS_Addr::ADJUST_Flowrate_CUR,
  DGUS_Addr::LEVEL_OFFSET_Current,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PRINT_FINISHED[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::STATUS_PositionZ,
  DGUS_Addr::STATUS_Elapsed,
  DGUS_Addr::STATUS_Percent_Complete,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_TEMP_MENU[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_TEMP_MANUAL[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Max_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::TEMP_Max_Bed,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FAN[] PROGMEM = {
  DGUS_Addr::FAN0_Speed,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_SETTINGS_MENU[] PROGMEM = {
  DGUS_Addr::STEPPER_Status,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_LEVELING_OFFSET[] PROGMEM = {
  DGUS_Addr::LEVEL_OFFSET_Current,
  DGUS_Addr::LEVEL_OFFSET_StepIcons,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_LEVELING_MANUAL[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_LEVELING_AUTOMATIC[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::TEMP_Current_Bed,
  DGUS_Addr::TEMP_Target_Bed,
  DGUS_Addr::LEVEL_AUTO_DisableIcon,
  DGUS_Addr::LEVEL_AUTO_Grid,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_LEVELING_PROBING[] PROGMEM = {
  DGUS_Addr::LEVEL_PROBING_Icons1,
  DGUS_Addr::LEVEL_PROBING_Icons2,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_FILAMENT[] PROGMEM = {
  DGUS_Addr::TEMP_Current_H0,
  DGUS_Addr::TEMP_Target_H0,
  DGUS_Addr::FILAMENT_ExtruderIcons,
  DGUS_Addr::FILAMENT_Length,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_MOVE[] PROGMEM = {
  DGUS_Addr::MOVE_CurrentX,
  DGUS_Addr::MOVE_CurrentY,
  DGUS_Addr::MOVE_CurrentZ,
  DGUS_Addr::MOVE_StepIcons,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_GCODE[] PROGMEM = {
  DGUS_Addr::GCODE_Data,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_SETTINGS_MENU2[] PROGMEM = {
  DGUS_Addr::SETTINGS2_BLTouch,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_PID[] PROGMEM = {
  DGUS_Addr::PID_HeaterIcons,
  DGUS_Addr::PID_Temp,
  DGUS_Addr::PID_Cycles,
  DGUS_Addr::PID_Kp,
  DGUS_Addr::PID_Ki,
  DGUS_Addr::PID_Kd,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_VOLUME[] PROGMEM = {
  DGUS_Addr::VOLUME_Level,
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_BRIGHTNESS[] PROGMEM = {
  DGUS_Addr::BRIGHTNESS_Level,
  DGUS_Addr::END
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
  DGUS_Addr::END
};

constexpr DGUS_Addr LIST_WAIT[] PROGMEM = {
  DGUS_Addr::WAIT_Icons,
  DGUS_Addr::END
};

#define MAP_HELPER(SCREEN, LIST) { .screenID = SCREEN, .addr_list = LIST }

const struct DGUS_ScreenAddrList screen_addr_list_map[] PROGMEM = {
  MAP_HELPER(DGUS_ScreenID::HOME,               LIST_HOME),
  #if HAS_MEDIA
    MAP_HELPER(DGUS_ScreenID::PRINT,            LIST_PRINT),
  #endif
  MAP_HELPER(DGUS_ScreenID::PRINT_STATUS,       LIST_PRINT_STATUS),
  MAP_HELPER(DGUS_ScreenID::PRINT_ADJUST,       LIST_PRINT_ADJUST),
  MAP_HELPER(DGUS_ScreenID::PRINT_FINISHED,     LIST_PRINT_FINISHED),
  MAP_HELPER(DGUS_ScreenID::TEMP_MENU,          LIST_TEMP_MENU),
  MAP_HELPER(DGUS_ScreenID::TEMP_MANUAL,        LIST_TEMP_MANUAL),
  MAP_HELPER(DGUS_ScreenID::FAN,                LIST_FAN),
  MAP_HELPER(DGUS_ScreenID::SETTINGS_MENU,      LIST_SETTINGS_MENU),
  MAP_HELPER(DGUS_ScreenID::LEVELING_OFFSET,    LIST_LEVELING_OFFSET),
  MAP_HELPER(DGUS_ScreenID::LEVELING_MANUAL,    LIST_LEVELING_MANUAL),
  MAP_HELPER(DGUS_ScreenID::LEVELING_AUTOMATIC, LIST_LEVELING_AUTOMATIC),
  MAP_HELPER(DGUS_ScreenID::LEVELING_PROBING,   LIST_LEVELING_PROBING),
  MAP_HELPER(DGUS_ScreenID::FILAMENT,           LIST_FILAMENT),
  MAP_HELPER(DGUS_ScreenID::MOVE,               LIST_MOVE),
  MAP_HELPER(DGUS_ScreenID::GCODE,              LIST_GCODE),
  MAP_HELPER(DGUS_ScreenID::SETTINGS_MENU2,     LIST_SETTINGS_MENU2),
  MAP_HELPER(DGUS_ScreenID::PID,                LIST_PID),
  MAP_HELPER(DGUS_ScreenID::VOLUME,             LIST_VOLUME),
  MAP_HELPER(DGUS_ScreenID::BRIGHTNESS,         LIST_BRIGHTNESS),
  MAP_HELPER(DGUS_ScreenID::INFOS,              LIST_INFOS),
  MAP_HELPER(DGUS_ScreenID::WAIT,               LIST_WAIT),

  MAP_HELPER((DGUS_ScreenID)0, nullptr)
};

#endif // DGUS_LCD_UI_RELOADED
