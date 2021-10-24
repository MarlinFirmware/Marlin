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
#pragma once

constexpr uint8_t DGUS_LINE_LEN             = 32;
constexpr uint8_t DGUS_STATUS_LEN           = 32;
constexpr uint8_t DGUS_FILE_COUNT           = 5;
constexpr uint8_t DGUS_FILENAME_LEN         = 32;
constexpr uint8_t DGUS_ELLAPSED_LEN         = 15;
constexpr uint8_t DGUS_LEVEL_GRID_SIZE      = 25;
constexpr uint8_t DGUS_MACHINE_LEN          = 24;
constexpr uint8_t DGUS_BUILDVOLUME_LEN      = 24;
constexpr uint8_t DGUS_VERSION_LEN          = 16;
constexpr uint8_t DGUS_PRINTTIME_LEN        = 24;
constexpr uint8_t DGUS_LONGESTPRINT_LEN     = 24;
constexpr uint8_t DGUS_FILAMENTUSED_LEN     = 24;
constexpr uint8_t DGUS_GCODE_LEN            = 32;

enum class DGUS_Addr : uint16_t {
  MESSAGE_Line1             = 0x1100, // 0x1100 - 0x111F
  MESSAGE_Line2             = 0x1120, // 0x1120 - 0x113F
  MESSAGE_Line3             = 0x1140, // 0x1140 - 0x115F
  MESSAGE_Line4             = 0x1160, // 0x1160 - 0x117F

  // READ-ONLY VARIABLES

  SCREENCHANGE              = 0x2000, // Screen change request. Data contains target screen in low byte.
  SCREENCHANGE_SD           = 0x2001, // Only change if SD card present.
  SCREENCHANGE_Idle         = 0x2002, // Only change if not printing.
  SCREENCHANGE_Printing     = 0x2003, // Only change if printing.
  SD_SelectFile             = 0x2004, // Data: file index (0-4)
  SD_Scroll                 = 0x2005, // Data: DGUS_Data::Scroll
  SD_Print                  = 0x2006,
  STATUS_Abort              = 0x2007, // Popup / Data: DGUS_Data::Popup
  STATUS_Pause              = 0x2008, // Popup / Data: DGUS_Data::Popup
  STATUS_Resume             = 0x2009, // Popup / Data: DGUS_Data::Popup
  ADJUST_SetFeedrate        = 0x200A, // Type: Integer (16 bits signed)
  ADJUST_SetFlowrate_CUR    = 0x200B, // Type: Integer (16 bits signed)
  #if HAS_MULTI_EXTRUDER
    ADJUST_SetFlowrate_E0   = 0x200C, // Type: Integer (16 bits signed)
    ADJUST_SetFlowrate_E1   = 0x200D, // Type: Integer (16 bits signed)
  #endif
  ADJUST_SetBabystep        = 0x200E, // Type: Fixed point, 2 decimals (16 bits signed)
  ADJUST_Babystep           = 0x200F, // Data: DGUS_Data::Adjust
  TEMP_Preset               = 0x2010, // Popup / Data: DGUS_Data::TempPreset
  TEMP_SetTarget_Bed        = 0x2011, // Type: Integer (16 bits signed)
  TEMP_SetTarget_H0         = 0x2012, // Type: Integer (16 bits signed)
  #if HOTENDS > 1
    TEMP_SetTarget_H1       = 0x2013, // Type: Integer (16 bits signed)
  #endif
  TEMP_Cool                 = 0x2014, // Data: DGUS_Data::Heater
  STEPPER_Control           = 0x2015, // Popup / Data: DGUS_Data::Control
  LEVEL_OFFSET_Set          = 0x2016, // Type: Fixed point, 2 decimals (16 bits signed)
  LEVEL_OFFSET_Step         = 0x2017, // Data: DGUS_Data::Adjust
  LEVEL_OFFSET_SetStep      = 0x2018, // Data: DGUS_Data::StepSize
  LEVEL_MANUAL_Point        = 0x2019, // Data: point index (1-5)
  LEVEL_AUTO_Probe          = 0x201A,
  LEVEL_AUTO_Disable        = 0x201B,
  FILAMENT_Select           = 0x201C, // Data: DGUS_Data::Extruder
  FILAMENT_SetLength        = 0x201D, // Type: Integer (16 bits unsigned)
  FILAMENT_Move             = 0x201E, // Data: DGUS_Data::FilamentMove
  MOVE_Home                 = 0x201F, // Data: DGUS_Data::Axis
  MOVE_SetX                 = 0x2020, // Type: Fixed point, 1 decimal (16 bits signed)
  MOVE_SetY                 = 0x2021, // Type: Fixed point, 1 decimal (16 bits signed)
  MOVE_SetZ                 = 0x2022, // Type: Fixed point, 1 decimal (16 bits signed)
  MOVE_Step                 = 0x2023, // Data: DGUS_Data::MoveDirection
  MOVE_SetStep              = 0x2024, // Data: DGUS_Data::StepSize
  GCODE_Clear               = 0x2025,
  GCODE_Execute             = 0x2026,
  EEPROM_Reset              = 0x2027, // Popup / Data: DGUS_Data::Popup
  SETTINGS2_Extra           = 0x2028, // Data: DGUS_Data::Extra
  PID_Select                = 0x2029, // Data: DGUS_Data::Heater
  PID_SetTemp               = 0x202A, // Type: Integer (16 bits unsigned)
  PID_Run                   = 0x202B,
  POWERLOSS_Abort           = 0x202C, // Popup / Data: DGUS_Data::Popup
  POWERLOSS_Resume          = 0x202D, // Popup / Data: DGUS_Data::Popup
  WAIT_Abort                = 0x202E, // Popup / Data: DGUS_Data::Popup
  WAIT_Continue             = 0x202F,

  // WRITE-ONLY VARIABLES

  MESSAGE_Status            = 0x3000, // 0x3000 - 0x301F
  SD_Type                   = 0x3020, // 0x3020 - 0x3024 / Data: DGUS_Data::SDType
  SD_FileName0              = 0x3025, // 0x3025 - 0x3044
  SD_FileName1              = 0x3045, // 0x3045 - 0x3064
  SD_FileName2              = 0x3065, // 0x3065 - 0x3084
  SD_FileName3              = 0x3085, // 0x3085 - 0x30A4
  SD_FileName4              = 0x30A5, // 0x30A5 - 0x30C4
  SD_ScrollIcons            = 0x30C5, // Bits: DGUS_Data::ScrollIcon
  SD_SelectedFileName       = 0x30C6, // 0x30C6 - 0x30E5
  STATUS_PositionZ          = 0x30E6, // Type: Fixed point, 1 decimal (16 bits signed)
  STATUS_Ellapsed           = 0x30E7, // 0x30E7 - 0x30F5
  STATUS_Percent            = 0x30F6, // Type: Integer (16 bits unsigned)
  STATUS_Icons              = 0x30F7, // Bits: DGUS_Data::StatusIcon
  ADJUST_Feedrate           = 0x30F8, // Type: Integer (16 bits signed)
  ADJUST_Flowrate_CUR       = 0x30F9, // Type: Integer (16 bits signed)
  #if HAS_MULTI_EXTRUDER
    ADJUST_Flowrate_E0      = 0x30FA, // Type: Integer (16 bits signed)
    ADJUST_Flowrate_E1      = 0x30FB, // Type: Integer (16 bits signed)
  #endif
  TEMP_Current_Bed          = 0x30FC, // Type: Integer (16 bits signed)
  TEMP_Target_Bed           = 0x30FD, // Type: Integer (16 bits signed)
  TEMP_Max_Bed              = 0x30FE, // Type: Integer (16 bits unsigned)
  TEMP_Current_H0           = 0x30FF, // Type: Integer (16 bits signed)
  TEMP_Target_H0            = 0x3100, // Type: Integer (16 bits signed)
  TEMP_Max_H0               = 0x3101, // Type: Integer (16 bits unsigned)
  #if HOTENDS > 1
    TEMP_Current_H1         = 0x3102, // Type: Integer (16 bits signed)
    TEMP_Target_H1          = 0x3103, // Type: Integer (16 bits signed)
    TEMP_Max_H1             = 0x3104, // Type: Integer (16 bits unsigned)
  #endif
  STEPPER_Status            = 0x3105, // Data: DGUS_Data::Status
  LEVEL_OFFSET_Current      = 0x3106, // Type: Fixed point, 2 decimals (16 bits signed)
  LEVEL_OFFSET_StepIcons    = 0x3107, // Bits: DGUS_Data::StepIcon
  LEVEL_AUTO_DisableIcon    = 0x3108, // Data: DGUS_Data::Status
  LEVEL_AUTO_Grid           = 0x3109, // 0x3109 - 0x3121 / Type: Fixed point, 3 decimals (16 bits signed)
  LEVEL_PROBING_Icons1      = 0x3122, // Type: Integer (16 bits unsigned) / Each bit represents a grid point
  LEVEL_PROBING_Icons2      = 0x3123, // Type: Integer (16 bits unsigned) / Each bit represents a grid point
  FILAMENT_ExtruderIcons    = 0x3124, // Data: DGUS_Data::ExtruderIcon
  FILAMENT_Length           = 0x3125, // Type: Integer (16 bits unsigned)
  MOVE_CurrentX             = 0x3126, // Type: Fixed point, 1 decimal (16 bits signed)
  MOVE_CurrentY             = 0x3127, // Type: Fixed point, 1 decimal (16 bits signed)
  MOVE_CurrentZ             = 0x3128, // Type: Fixed point, 1 decimal (16 bits signed)
  MOVE_StepIcons            = 0x3129, // Bits: DGUS_Data::StepIcon
  SETTINGS2_BLTouch         = 0x312A, // Data: DGUS_Data::Status
  PID_HeaterIcons           = 0x312B, // Data: DGUS_Data::HeaterIcon
  PID_Temp                  = 0x312C, // Type: Integer (16 bits unsigned)
  PID_Kp                    = 0x312D, // Type: Fixed point, 2 decimals (32 bits signed)
  PID_Ki                    = 0x312F, // Type: Fixed point, 2 decimals (32 bits signed)
  PID_Kd                    = 0x3131, // Type: Fixed point, 2 decimals (32 bits signed)
  INFOS_Machine             = 0x3133, // 0x3133 - 0x314A
  INFOS_BuildVolume         = 0x314B, // 0x314B - 0x3162
  INFOS_Version             = 0x3163, // 0x3163 - 0x3172
  INFOS_TotalPrints         = 0x3173, // Type: Integer (16 bits unsigned)
  INFOS_FinishedPrints      = 0x3174, // Type: Integer (16 bits unsigned)
  INFOS_PrintTime           = 0x3175, // 0x3175 - 0x318C
  INFOS_LongestPrint        = 0x318D, // 0x318D - 0x31A4
  INFOS_FilamentUsed        = 0x31A5, // 0x31A5 - 0x31BC
  WAIT_Icons                = 0x31BD, // Bits: DGUS_Data::WaitIcon

  // READ-WRITE VARIABLES

  FAN0_Speed                = 0x4000, // Type: Integer (16 bits unsigned) / Data: fan speed as percent (0-100)
  GCODE_Data                = 0x4001, // 0x4001 - 0x4020
  PID_Cycles                = 0x4021, // Type: Integer (16 bits unsigned)
  VOLUME_Level              = 0x4022, // Type: Integer (16 bits unsigned) / Data: volume as percent (0-100)
  BRIGHTNESS_Level          = 0x4023, // Type: Integer (16 bits unsigned) / Data: brightness as percent (0-100)

  // SPECIAL CASES

  STATUS_Percent_Complete   = 0x5000, // Same as STATUS_Percent, but always 100%
  INFOS_Debug               = 0x5001,

};
