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

#include "../../../../inc/MarlinConfigPre.h"

#ifndef LCD_LANGUAGE
  #define LCD_LANGUAGE en
  #warning "LCD_LANGUAGE not defined, defaulting to English."
#endif

namespace DGUS_Data {

  // 111C. Also the value written to every language icon.
  enum class Language : uint16_t {
    Chinese_Simplified = 1,
    English,
    German,
    Spanish,
    French,
    Italian,
    Portuguese,
    Russian,
    Turkish,

    // Compatibility with LCD_LANGUAGE
    zh_CN = Chinese_Simplified,
    en = English,
    de = German,
    es = Spanish,
    fr = French,
    fr_na = French,
    it = Italian,
    pt = Portuguese,
    ru = Russian,
    tr = Turkish,

    Default = LCD_LANGUAGE
  };

  // 1002
  enum class MenuSelectCommand : uint16_t {
    Print = 1,          // MAIN -> FILE1
    Prepare = 2,        // MAIN -> PREPARE
    Control = 3,        // MAIN -> CONTROL
    Level = 4,          // MAIN -> LEVELINGMODE, or NO_LEVEL without a probe
    PrintFinished = 5,  // FINISH -> MAIN
    StartAutoLevel = 6  // LEVELINGMODE -> LEVELING
  };

  // 1004
  enum class AdjustCommand : uint16_t {
    Show_Adjust = 1,    // PRINTING, PAUSED -> ADJUST
    Exit_Adjust = 2,    // ADJUST -> PRINTING or PAUSED
    Toggle_Fan = 3      // ADJUST, TEMP
  };

  // 1008
  enum class StopCommand : uint16_t {
    Show_Confirm = 1,   // PRINTING, PAUSED -> STOP_CONFIRM
    Confirm = 2         // STOP_CONFIRM, FILAMENT_RUNOUT, FILAMENT_INSERT
  };

  // 100A
  enum class PauseCommand : uint16_t {
    Show_Confirm = 1,   // PRINTING -> PAUSE_CONFIRM
    Confirm = 2,        // PAUSE_CONFIRM
    Cancel = 3          // PAUSE_CONFIRM, STOP_CONFIRM -> PRINTING or PAUSED
  };

  // 100C
  enum class ResumeCommand : uint16_t {
    Resume = 1,         // PAUSED, FILAMENT_INSERT
    Reheat = 2          // FILAMENT_RUNOUT
  };

  // 1030
  enum class TemperatureMenuCommand : uint16_t {
    Show_Temp = 2,      // CONTROL -> TEMP
    Show_PLA = 3,       // TEMP -> PLA_TEMP
    Show_ABS = 4,       // TEMP -> ABS_TEMP
    Preheat_PLA = 5,    // PREPARE
    Preheat_ABS = 6,    // PREPARE
    Show_Control = 7    // TEMP, INFORMATION -> CONTROL
  };

  // 1032
  enum class CooldownCommand : uint16_t {
    Cooldown = 1,       // PREPARE
    Show_Temp = 2       // PLA_TEMP, ABS_TEMP -> TEMP
  };

  // 103E
  enum class ControlMenuCommand : uint16_t {
    Show_MoveAxis = 3,  // PREPARE -> MOVEAXIS_10
    Show_Info = 5,      // CONTROL -> INFORMATION
    Disable_Steppers = 6, // PREPARE
    Reset_Settings = 7, // CONTROL
    Save_Presets = 8,   // PLA_TEMP, ABS_TEMP
    Show_Main = 9       // PREPARE, CONTROL, LEVELINGMODE -> MAIN
  };

  // 1044
  enum class LevelingCommand : uint16_t {
    Home = 1,           // LEVELINGMODE
    ZOffset_Up = 2,     // LEVELINGMODE
    ZOffset_Down = 3,   // LEVELINGMODE
    Exit_Leveling = 4   // LEVELING -> LEVELINGMODE
  };

  // 1046
  enum class AxisControlCommand : uint16_t {
    Jog_10mm = 1,       // MOVEAXIS_* -> MOVEAXIS_10
    Jog_1mm = 2,        // MOVEAXIS_* -> MOVEAXIS_1
    Jog_0_1mm = 3,      // MOVEAXIS_* -> MOVEAXIS_01
    Home = 4            // MOVEAXIS_*
  };

  // 1056
  enum class FilamentIoCommand : uint16_t {
    Retract = 1,        // FEEDRETURN
    Extrude = 2,        // FEEDRETURN
    Show_Prepare = 3,   // MOVEAXIS_*, FEEDRETURN -> PREPARE
    Show_FeedReturn = 4 // PREPARE -> FEEDRETURN
  };

  // 105C
  enum class LanguageMenuCommand : uint16_t {
    Show_Language = 1,  // CONTROL -> LANGUAGE
    Show_Control = 2    // LANGUAGE -> CONTROL
  };

  // 105F
  enum class PowerLossCommand : uint16_t {
    PowerLoss_Continue = 1,
    PowerLoss_No = 2
  };

  // 111A
  enum class AcknowledgeCommand : uint16_t {
    Abnormal_OK = 1,    // ABNORMAL
    NoLevel_OK = 2      // NO_LEVEL -> MAIN
  };

  // 20D2
  enum class FilelistControlCommand : uint16_t {
    Start_Print = 1,
    Prev_Page = 2,      // FILE2..FILE5 -> previous
    Next_Page = 3,      // FILE1..FILE4 -> next
    Show_Main = 4       // FILE* -> MAIN
  };
};
