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

enum class DGUS_ScreenID : uint8_t {
  START               = 0,
  MAIN,
  FILE1,
  FILE2,
  FILE3,
  FILE4,
  FILE5,
  FILAMENTUSEUP,
  FILAMENTLOAD,
  FINISH,
  PAUSE,
  PAUSE_STOP,
  RESUME,
  CONTINUE_STOP,
  ADJUST,
  PREPARE,
  MOVEAXIS_10,
  MOVEAXIS_1,
  MOVEAXIS_01,
  FEEDRETURN,
  CONTROL,
  TEMP,
  PLA_TEMP,
  ABS_TEMP,
  INFORMATION,
  LEVELINGMODE,
  LEVELING,
  POWERCONTINUE,
  LANGUAGE,
  KEYBOARD,
  KEYBOARD_CONFIRM,
  THERMAL_RUNAWAY,
  LEVEL_OFFSET,
  CONTROL_DEVICE,
  MOTION,
  MAX_FEEDRATE,
  ACCELERATION,
  JERK,
  STEPSMM,
  PIDCONTROL,
  AUTOHOME,
  ABNORMAL,
  WIFI_CONFIRM,
  FACTORYRESET_CONFIRM,
  SHUTDOWN,
  SMOKE_ALARM,
  FILAMENTCHECK,
  SDCARDCHECK,
  LASER_FDM,
  MAIN_LASER,
  _52_FILE1,
  _53_FILE2,
  _54_FILE3,
  _55_FILE4,
  SW_LASER_TIPS,
  SW_FDM_TIPS,
  STOP_CONFIRM,
  PAUSE_LASER,
  FINISH_LASER,
  RESUME_LASER,
  PAUSE_CONFIRM,
  FOCUS,
  DEVICE_LASER,
  LANGE,
  LASER_INFORMATION,
  ACCEL_LASER,
  JERK_SPEED_LASER,
  MAX_SPEED_LASER,
  AXIS_MOVE_10,
  AXIS_MOVE_1,
  AXIS_MOVE_01,
  ADJUST_LASER,
  ENGRAVE_TIPS,
  RATIO,
  SW_FOCUS,
  AXIS_ADJUST_10,
  AXIS_ADJUST_1,
  AXIS_ADJUST_01,

  BOOT                = START,
  HOME                = MAIN
};
